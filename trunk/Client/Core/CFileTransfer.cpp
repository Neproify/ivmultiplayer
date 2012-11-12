//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.cpp
// Project: Client.Core
// Author(s): Einstein
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileTransfer.h"
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
{
	// Set the http client receive handler
	m_httpClient.SetReceiveHandle(ReceiveHandler, this);

	m_bDownloadingFile = false;
	m_pDownloadFile = NULL;
	m_fDownloadFile = NULL;
	m_pFileImage = NULL;
	m_pFileText = NULL;
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
		m_pFileText->setFont(g_pGUI->GetFont("BebasNeue",28U));
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
	String strFilePath(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strFolderName.Get(), strFileName.Get()));

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
				String strFilePath(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strFolderName.Get(), m_pDownloadFile->strName.Get()));

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
