//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.cpp
// Project: Client.Core
// Author(s):	Einstein
//				jenksta
//				CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileTransfer.h"
#include <stdio.h>
#include "SharedUtility.h"
#include "CClientScriptManager.h"
#include "CNetworkManager.h"
#include "CLogFile.h"
#include "CLocalPlayer.h"
#include <Threading\CThread.h>

extern CClientScriptManager * g_pClientScriptManager;
extern CLocalPlayer			* g_pLocalPlayer;

CFileTransfer::CFileTransfer()
{
	m_thread = new CThread();
	m_userdata = new ThreadUserData();	
}

// Returns true if download thread is working NOW
bool CFileTransfer::IsBusy()
{
	return m_userdata->busy || m_thread->IsRunning();
}

// Starts download missing files thread
bool CFileTransfer::Process()
{
	// Doin nuthin if there is nothing todo or we already doin somethin
	if(IsBusy() || GetTransferListSize() == 0)
		return false;

	// Creating directories to download files to..
	String strFolderName = SharedUtility::GetAbsolutePath("clientfiles");
	if(!SharedUtility::Exists(strFolderName))
		SharedUtility::CreateDirectoryA(strFolderName);
	if(!SharedUtility::Exists(strFolderName + "\\clientscripts"))
		SharedUtility::CreateDirectoryA(strFolderName + "\\clientscripts");
	if(!SharedUtility::Exists(strFolderName + "\\resources"))
		SharedUtility::CreateDirectoryA(strFolderName + "\\resources");	
	
	CLogFile::Print("Starting download client files thread...");
	m_thread->SetUserData<ThreadUserData*>(m_userdata);
	m_thread->Start(WorkAsync);

	return true;
}

// Returns failed to download file.
FileDownload * CFileTransfer::GetFailedResource()
{
	// Loop all files and check they all okay (downloaded)
	for(std::list<FileDownload *>::iterator iter = m_userdata->m_fileList.begin(); iter != m_userdata->m_fileList.end(); iter++)
	{
		if((*iter)->okay == false)
			return *iter;
	}

	return NULL;
}

// Adds a file to download list
void CFileTransfer::AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource)
{
	CLogFile::Printf("ADDING FILE %s:", strFileName.Get());

	String strFolderName;
	if(bIsResource) strFolderName = "resources";
	else strFolderName = "clientscripts";

	String strFilePath(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strFolderName.Get(), strFileName.Get()));
	if(SharedUtility::Exists(strFilePath))
	{
		// If file already exists, compare checksums, and if they match we dont need to download.
		CFileChecksum currentFileChecksum;
		currentFileChecksum.Calculate(strFilePath);
		if(currentFileChecksum == fileChecksum)
		{
			if(!bIsResource)
			{
				// Load script
				g_pClientScriptManager->AddScript(strFileName, strFilePath);
				if(g_pLocalPlayer->GetFirstSpawn())
					g_pClientScriptManager->Load(strFileName);				
			}

			CLogFile::Printf("FILE %s FINISHED WITHOUT NEED TO DOWNLOAD.", strFileName.Get());
			return;
		}
	}
		
	m_userdata->m_fileList.push_back(new FileDownload(strFileName, fileChecksum, bIsResource ? FileDownloadCategory::Resource : FileDownloadCategory::Script));
	CLogFile::Printf("FILE %s SET TO DOWNLOAD LIST.", strFileName.Get());
}

// Returns count of files need to be downloaded
int CFileTransfer::GetTransferListSize()
{
	// Loop all files and count which of them who are not downloaded.
	int count = 0;
	for(std::list<FileDownload *>::iterator iter = m_userdata->m_fileList.begin(); iter != m_userdata->m_fileList.end(); iter++)
	{
		if(!(*iter)->okay)
			count++;
	}
	return count;
}

// RESET file transferer: remove all files from to-download and downloaded list. Stop downloading if in progress
void CFileTransfer::Reset()
{
	// Terminate thread if working (hard - is it cool?):
	if(IsBusy())
		m_thread->Stop(false, true);

	// Clear file list and reset userdata (with saving handlers set)
	// TODO: ThreadUserData.Reset() instead of this
	m_userdata->m_fileList.clear();
	DownloadHandler_t downloadedHandler = m_userdata->downloadedHandler;
	DownloadHandler_t failedHandler = m_userdata->downloadFailedHandler;
	delete m_userdata;
	m_userdata = new ThreadUserData();
	m_userdata->downloadedHandler = downloadedHandler;
	m_userdata->downloadFailedHandler = failedHandler;
}

// Compiles all missing clientscipts (which downloaded)
void CFileTransfer::CompileScripts()
{
	// Loading and running all not yet loaded scripts.
	for(std::list<FileDownload *>::iterator iter = m_userdata->m_fileList.begin(); iter != m_userdata->m_fileList.end(); iter++)
	{
		CLogFile::Printf("Compile: %s", (*iter)->name);
	}
}

// Sets a http server info
void CFileTransfer::SetServerInformation(String strAddress, unsigned short usPort)
{
	m_userdata->httpDownloader->SetHost(strAddress);
	m_userdata->httpDownloader->SetPort(usPort);
}

// Threaded functions:
void WorkAsync(CThread * pCreator)
{
	ThreadUserData * pUserdata = pCreator->GetUserData<ThreadUserData *>();
	if(pUserdata->busy)
		return;

	pUserdata->busy = true;
	pUserdata->bDownloadCompleted = false;
		pUserdata->currentFile = NULL;

	// If we connecting to local host (very fast) wait a little to give world time to initialize.
	String host = pUserdata->httpDownloader->GetHost();
	if(host == "127.0.0.1" || host.ToLower() == "localhost")
		Sleep(2000);

	// Download all files queued.
	for(std::list<FileDownload *>::iterator iter = pUserdata->m_fileList.begin(); iter != pUserdata->m_fileList.end(); iter++)
	{
		if((*iter)->okay)
			continue;
		pUserdata->currentFile = (*iter);

		// Get a file path for this file to download to
		String strFolderName = "clientscripts";
		if((*iter)->type == FileDownloadCategory::Script);
		else if((*iter)->type == FileDownloadCategory::Resource)
			strFolderName = "resources";
		String strFilePath(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strFolderName.Get(), (*iter)->name.Get()));
			
		// Opening file handle:
		FILE * f = fopen(strFilePath.Get(), "wb");
		if(!f)
		{
			(*iter)->failReason = "Failed to open file handle (write)";
			if(pUserdata->downloadFailedHandler != NULL)
				pUserdata->downloadFailedHandler();
			return;
		}
		(*iter)->fileName = strFilePath;

		// Connecting:
		String strDownloadUrl("/%s/%s", strFolderName.Get(), (*iter)->name.Get());
		if(!pUserdata->httpDownloader->Get(strDownloadUrl))
		{
			// We can't connect or something like this.
			(*iter)->failReason = pUserdata->httpDownloader->GetLastErrorString();
			return;
		}
		// Downloading:
		pUserdata->httpDownloader->SetFile(f);
		while(pUserdata->httpDownloader->GettingData() || !pUserdata->httpDownloader->GotData())
			pUserdata->httpDownloader->Process();
		pUserdata->httpDownloader->SetFile();
		fclose(f);

		// Checksum check:
		CFileChecksum chk;
		chk.Calculate(strFilePath);
		if(chk != (*iter)->checksum)
		{
			// Something strange happend while downloading...
			(*iter)->failReason = "Checksum mismatch";
			if(pUserdata->downloadFailedHandler != NULL)
				pUserdata->downloadFailedHandler();
			return;
		}

		// Finished
		fclose(f);
		(*iter)->okay = true;	
		if(pUserdata->downloadedHandler != NULL)
			pUserdata->downloadedHandler();	
		pUserdata->currentFile = NULL;
	}
	pUserdata->bDownloadCompleted = true;
	pUserdata->busy = false;
}
/*bool WorkAsync_FileRecv(const char * szData, unsigned int uiDataSize,	void * pUserData_)
{
	// Get current file handle and write data there.
	ThreadUserData * pUserData = (ThreadUserData*)pUserData_;
	FileDownload * current = pUserData->currentFile;
	FILE * f = current->File;
	fwrite(szData, 1, uiDataSize, f);
	//fflush(f);

	// Discard data in http client:
	return false;
}*/

/*#include "CFileTransfer.h"
#include <stdio.h>
#include "SharedUtility.h"
#include "CClientScriptManager.h"
#include "CChatWindow.h"
#include "CNetworkManager.h"
#include "CLogFile.h"
#include "CMainMenu.h"
#include "CLocalPlayer.h"

extern CClientScriptManager * g_pClientScriptManager;
extern CChatWindow          * g_pChatWindow;
extern CNetworkManager      * g_pNetworkManager;
extern CGUI					* g_pGUI;
extern CMainMenu			* g_pMainMenu;
extern CLocalPlayer			* g_pLocalPlayer;

#define TRANSFERBOX_WIDTH       350
#define TRANSFERBOX_HEIGHT      58

CFileTransfer::CFileTransfer()
	: m_bDownloadingFile(false),
	m_pDownloadFile(NULL),
	m_fDownloadFile(NULL),
	m_pFileImage(NULL),
	m_pFileText(NULL)
{
	// Set the http client receive handler
	m_httpClient.SetReceiveHandle(ReceiveHandler, this);
}

bool CFileTransfer::ReceiveHandler(const char * szData, unsigned int uiDataSize, void * pUserData)
{
	// Get the CFileTransfer pointer
	CFileTransfer * pFileTransfer = (CFileTransfer *)pUserData;

	// Write the data to the download file
	if(pFileTransfer->m_fDownloadFile)
		fwrite(szData, 1, uiDataSize, pFileTransfer->m_fDownloadFile);

	// Discard the data
	return false;
}

bool CFileTransfer::StartDownload(String strName, String strType)
{
	if(m_pFileImage)
		m_pFileImage->setVisible(true);

	// Ensure we have a server address set
	if(m_httpClient.GetHost().IsNotEmpty())
	{
		// Get the folder name
		String strFolderName;

		if(strType == "resource")
			strFolderName = "resources";
		else if(strType == "script")
			strFolderName = "clientscripts";

		// Create the client files folder string
		String strClientFilesFolder(SharedUtility::GetAbsolutePath("clientfiles"));

		// Create the client files folder if needed
		if(!SharedUtility::Exists(strClientFilesFolder.Get()))
			SharedUtility::CreateDirectory(strClientFilesFolder.Get());

		// Create the destination type folder string
		String strDestinationFolder("%s/%s", strClientFilesFolder.Get(), strFolderName.Get());

		// Create the destination type folder if needed
		if(!SharedUtility::Exists(strDestinationFolder.Get()))
			SharedUtility::CreateDirectory(strDestinationFolder.Get());

		// Create the destination file string
		String strDestinationFile("%s/%s", strDestinationFolder.Get(), strName.Get());

		// Open the destination file
		m_fDownloadFile = fopen(strDestinationFile.Get(), "wb");

		// Did the destination file not open sucessfully?
		if(!m_fDownloadFile)
			return false;

		// Create the post path string
		String strPostPath("/%s/%s", strFolderName.Get(), strName.Get());

		// Show message stuff
		String strText;
		strText.AppendF("%s",strName.Get());

		// Set stuff & check if we have our gui stuff
		if(m_pFileImage && m_pFileText)
		{
			// Set the filename
			m_pFileText->setText(strText.Get());

			// Set the image and text visible
			m_pFileText->setVisible(true);
		}

		// Send the request
		m_httpClient.Get(strPostPath);
		return true;
	}

	return false;
}

void CFileTransfer::SetServerInformation(String strAddress, unsigned short usPort)
{
	m_httpClient.SetHost(strAddress);
	m_httpClient.SetPort(usPort);

	float fWidth = (float)g_pGUI->GetDisplayWidth();
	float fHeight = (float)g_pGUI->GetDisplayHeight();

	// Check if we have created image and text stuff for download, if not create it
	if(!m_pFileImage)
	{
		// Try to load the image download.png - NOW!
		try
		{
			CEGUI::ImagesetManager::getSingleton().createFromImageFile("Download", "Download.png");
		}
		catch(CEGUI::InvalidRequestException e)
		{
			String strFile = e.getMessage().c_str();
			strFile = strFile.SubStr(strFile.Find(" - ")+3, (unsigned int)-1);
			String str("IV:MP failed to load. (%s)", strFile.Get());
			MessageBox(NULL, str.C_String(), "IV:MP Error", MB_OK | MB_ICONERROR);
			ExitProcess(0);
		}
		catch(CEGUI::Exception e)
		{
			MessageBox(NULL, "IV:MP failed to load. Check CEGUI.log for details.", "IV:MP Error", MB_OK | MB_ICONERROR);
			ExitProcess(0);
		}

		m_pFileImage = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
		m_pFileImage->setProperty("FrameEnabled", "false");
		m_pFileImage->setProperty("BackgroundEnabled", "false");
		m_pFileImage->setSize(CEGUI::UVector2(CEGUI::UDim(0, (386.0f*1.5f)), CEGUI::UDim(0, (114.0f*1.5f))));
		m_pFileImage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/3),  CEGUI::UDim(0, fHeight/2+(fHeight/(float)6.5))));
		m_pFileImage->setProperty("Image", "set:Download image:full_image");
		m_pFileImage->setProperty("InheritsAlpha","false");
		m_pFileImage->setAlpha(0.90f);
		m_pFileImage->setVisible(false);
	}
	if(!m_pFileText)
	{
		m_pFileText = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
		m_pFileText->setText("Waiting for resources download");
		m_pFileText->setSize(CEGUI::UVector2(CEGUI::UDim(TRANSFERBOX_WIDTH, 0), CEGUI::UDim(TRANSFERBOX_HEIGHT, 0)));
		m_pFileText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/(float)2.75),  CEGUI::UDim(0, fHeight/2-(fHeight/4))));
		m_pFileText->setProperty("FrameEnabled", "false");
		m_pFileText->setProperty("BackgroundEnabled", "false");
		m_pFileText->setFont(g_pGUI->GetFont("tahoma",28U));
		m_pFileText->setVisible(false);
	}
}

void CFileTransfer::AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource)
{
	// Get the folder name
	String strFolderName;

	if(bIsResource)
		strFolderName = "resources";
	else
		strFolderName = "clientscripts";

	// Create the file path string
	String strFilePath(SharedUtility::GetAbsolutePath("clientfiles/%s/%s", strFolderName.Get(), strFileName.Get()));

	// Does the file exist?
	if(SharedUtility::Exists(strFilePath))
	{
		// Create a file checksum of the file
		CFileChecksum currentFileChecksum;
		currentFileChecksum.Calculate(strFilePath);

		// Does the file checksum match the server file checksum (We don't need to download the file)?
		if(currentFileChecksum == fileChecksum)
		{
			// Is the downloaded file a script?
			if(!bIsResource)
			{
				// Add the script to the client script manager
				g_pClientScriptManager->AddScript(strFileName, strFilePath);

				// Check if we had already our first spawn
				if(g_pLocalPlayer->GetFirstSpawn())
					g_pClientScriptManager->Load(strFileName);
			}

			return;
		}
	}

	ServerFile * pServerFile = new  ServerFile();
	pServerFile->strName = strFileName;
	memcpy(&pServerFile->fileChecksum, &fileChecksum, sizeof(CFileChecksum));
	pServerFile->strType = (bIsResource ? "resource" : "script");
	m_fileList.push_back(pServerFile);
}

void CFileTransfer::Process()
{	
	// Are we not downloading a file?
	if(!m_bDownloadingFile)
	{
		// Do we have no files to download?
		if(m_fileList.empty())
			return;

		// Loop through all server files
		for(std::list<ServerFile *>::iterator iter = m_fileList.begin(); iter != m_fileList.end(); iter++)
		{
			// Get the server file pointer
			ServerFile * pServerFile = (*iter);

			// Get the folder name
			String strFolderName;

			if(pServerFile->strType == "resource")
				strFolderName = "resources";
			else if(pServerFile->strType == "script")
				strFolderName = "clientscripts";

			// Create the file path string
			String strFilePath(SharedUtility::GetAbsolutePath("clientfiles/%s/%s", strFolderName.Get(), pServerFile->strName.Get()));

			// Generate a checksum of the file we have
			CFileChecksum fileChecksum;
			fileChecksum.Calculate(strFilePath.Get());

			// Does the checksum differ from the server file checksum?
			if((!SharedUtility::Exists(strFilePath)) || (fileChecksum != pServerFile->fileChecksum))
			{
				// Start the download of the new file
				if(StartDownload(pServerFile->strName, pServerFile->strType))
				{
					// Flag ourselves as downloading
					m_bDownloadingFile = true;

					// Set the download file
					m_pDownloadFile = pServerFile;
				}
				else
				{
					// Reset all transfers
					Reset();

					// Show Message
					String strMessage; 
					strMessage.AppendF("Failed to start download of %s %s", pServerFile->strType.Get(), pServerFile->strName.Get());
					g_pMainMenu->ShowMessageBox(strMessage.Get(),"Download failed",true,false, false);
				}
			}
		}
	}
	else
	{
		// Is the http client busy?
		if(m_httpClient.IsBusy())
		{
			// Process the http client
			m_httpClient.Process();
		}
		else
		{
			// Has the http client successfully downloaded the file?
			if(m_httpClient.GotData())
			{
				// Close the file
				if(m_fDownloadFile)
					fclose(m_fDownloadFile);
				m_fDownloadFile = NULL;

				// Get the folder name
				String strFolderName;

				if(m_pDownloadFile->strType == "resource")
					strFolderName = "resources";
				else if(m_pDownloadFile->strType == "script")
					strFolderName = "clientscripts";

				// Create the file path string
				String strFilePath(SharedUtility::GetAbsolutePath("clientfiles/%s/%s", strFolderName.Get(), m_pDownloadFile->strName.Get()));

				// Create a checksum of the file
				CFileChecksum fileChecksum;
				fileChecksum.Calculate(strFilePath);

				// Do the checksums match?
				if(fileChecksum == m_pDownloadFile->fileChecksum)
				{
					// Is the downloaded file a script?
					if(m_pDownloadFile->strType == "script")
					{
						// Add the script to the client script manager
						g_pClientScriptManager->AddScript(m_pDownloadFile->strName, strFilePath);

						// Check if we had already our first spawn
						if(g_pLocalPlayer->GetFirstSpawn())
							g_pClientScriptManager->Load(m_pDownloadFile->strName);

						// If we're spawned and had our spawn, hide the download image stuff because we're already connected and the files we're refreshed serverside
						if(g_pLocalPlayer->GetFirstSpawn() && g_pLocalPlayer->IsSpawned())
							SetDownloadImageVisible(false);
					}

					// Remove the download file from the file list
					m_fileList.remove(m_pDownloadFile);

					// Delete the download file
					SAFE_DELETE(m_pDownloadFile);

					// Flag ourselves as no longer downloading
					m_bDownloadingFile = false;

					// Reset out http client
					m_httpClient.Reset();

					// Hide message & image
					m_pFileText->setVisible(false);
					
				}
				else
				{
					// Show Message
					String strMessage; 
					strMessage.AppendF("Failed to download %s %s (Checksum mismatch)", m_pDownloadFile->strType.Get(), m_pDownloadFile->strName.Get());
					g_pMainMenu->ShowMessageBox(strMessage.Get(),"Download failed",true,false, false);

					// Hide message stuff
					String strText = "";
					// Set stuff & check if we have our gui stuff
					if(m_pFileImage && m_pFileText)
					{
						// Set the filename
						m_pFileText->setText(strText.Get());

						// Set the image and text visible
						m_pFileText->setVisible(false);
						m_pFileImage->setVisible(false);
					}

					// Reset all transfers
					Reset();

					// Disconnect from the server
					g_pNetworkManager->Disconnect();
				}
			}
			else
			{
				// Show Message
				String strMessage; 
				strMessage.AppendF("Failed to download %s %s (%s)", m_pDownloadFile->strType.Get(), m_pDownloadFile->strName.Get(), m_httpClient.GetLastErrorString().Get());
				g_pMainMenu->ShowMessageBox(strMessage.Get(),"Download failed",true,false, false);

				// Hide message stuff
				String strText = "";
				// Set stuff & check if we have our gui stuff
				if(m_pFileImage && m_pFileText)
				{
					// Set the filename
					m_pFileText->setText(strText.Get());

					// Set the image and text visible
					m_pFileText->setVisible(false);
					m_pFileImage->setVisible(false);
				}

				// Reset all transfers
				Reset();

				// Disconnect from the server
				g_pNetworkManager->Disconnect();
			}
		}
	}
}

void CFileTransfer::Reset()
{
	// Reset the HTTP client
	m_httpClient.Reset();

	// Clear the file transfer list
	for(std::list<ServerFile *>::iterator iter = m_fileList.begin(); iter != m_fileList.end(); iter++)
		SAFE_DELETE(*iter);

	m_fileList.clear();

	// Reset download vars
	m_bDownloadingFile = false;
	m_pDownloadFile = NULL;

	if(m_fDownloadFile)
		fclose(m_fDownloadFile);

	m_fDownloadFile = NULL;
}
*/