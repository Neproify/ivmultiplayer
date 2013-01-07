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
#include "CClientScriptManager.h"
#include "CMainMenu.h"
#include "CChatWindow.h"
#pragma warning(disable:4482)

extern CClientScriptManager * g_pClientScriptManager;
extern CLocalPlayer			* g_pLocalPlayer;
extern CMainMenu			* g_pMainMenu;
extern CFileTransfer		* g_pFileTransfer;
extern CChatWindow			* g_pChatWindow;
extern CGUI					* g_pGUI;
extern CNetworkManager		* g_pNetworkManager;

#define TRANSFERBOX_WIDTH       350
#define TRANSFERBOX_HEIGHT      58

CFileTransfer::CFileTransfer()
	: m_pFileImage(NULL),
	m_pFileText(NULL),
	m_bFinishedDownload(false)
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
	if(IsBusy() || GetTransferListSize() == 0)
		return false;

	CLogFile::Print("Creating directories to download files to..");
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

// Adds a file to download list
void CFileTransfer::AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource)
{
	CLogFile::PrintDebugf("ADDING FILE %s:", strFileName.Get());

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

			CLogFile::PrintDebugf("FILE %s FINISHED WITHOUT NEED TO DOWNLOAD.", strFileName.Get());
			return;
		}
	}
		
	m_userdata->m_fileList.push_back(new FileDownload(strFileName, fileChecksum, bIsResource ? FileDownloadCategory::Resource : FileDownloadCategory::Script));
	CLogFile::PrintDebugf("FILE %s SET TO DOWNLOAD LIST.", strFileName.Get());
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

	if(count == 0)
		g_pFileTransfer->SetDownloadFinished(true);

	return count;
}

// RESET file transferer: remove all files from to-download and downloaded list. Stop downloading if in progress
void CFileTransfer::Reset()
{
	// Terminate thread if working (hard - is it cool?):
	if(IsBusy())
	{
		m_thread->Stop(false, true);
		SetCurrentFileText();
	}

	// TODO: ThreadUserData.Reset() instead of this
	m_userdata->m_fileList.clear();
	DownloadHandler_t downloadedHandler = m_userdata->downloadedHandler;
	DownloadHandler_t failedHandler = m_userdata->downloadFailedHandler;
	delete m_userdata;
	m_userdata = new ThreadUserData();
	m_userdata->downloadedHandler = downloadedHandler;
	m_userdata->downloadFailedHandler = failedHandler;
}

// Sets a http server info
void CFileTransfer::SetServerInformation(String strAddress, unsigned short usPort)
{
	m_userdata->httpDownloader->SetHost(strAddress);
	m_userdata->httpDownloader->SetPort(usPort);

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

// Sets text in GUI infox box (and shows/hides this box)
void CFileTransfer::SetCurrentFileText(const char * fileNameText)
{
	bool bVisible = fileNameText != NULL;

	m_pFileImage->setVisible(bVisible);
	m_pFileText->setVisible(bVisible);
	if(bVisible)
		m_pFileText->setText(fileNameText);
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

	// If connecting to local host (very fast) wait a little to give world time to initialize.
	String host = pUserdata->httpDownloader->GetHost();
	if(host == "127.0.0.1" || host.ToLower() == "localhost")
		Sleep(2000);

	// Download all files queued.
	for(std::list<FileDownload *>::iterator iter = pUserdata->m_fileList.begin(); iter != pUserdata->m_fileList.end(); iter++)
	{
		if((*iter)->okay)
			continue;
		pUserdata->currentFile = (*iter);
		g_pFileTransfer->SetCurrentFileText((*iter)->name.Get());

		// Get a file path for this file to download to
		String strFolderName = "clientscripts";
		if((*iter)->type == FileDownloadCategory::Script);
		else if((*iter)->type == FileDownloadCategory::Resource)
			strFolderName = "resources";
		String strFilePath(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strFolderName.Get(), (*iter)->name.Get()));
		(*iter)->fileName = strFilePath;

		// Opening file handle:
		FILE * f = fopen(strFilePath.Get(), "wb");
		if(!f)
		{
			// Error occured when opening file for writing
			(*iter)->failReason = "Failed to open file handle (write)";
			if(pUserdata->downloadFailedHandler != NULL)
				pUserdata->downloadFailedHandler();
			return;
		}

		// Connecting:
		String strDownloadUrl("/%s/%s", strFolderName.Get(), (*iter)->name.Get());		
		CLogFile::PrintDebugf("[TRANSFER]: Downloading %s, %s, %s", (*iter)->name.Get(), strDownloadUrl.Get(), strFilePath.Get());
		if(!pUserdata->httpDownloader->Get(strDownloadUrl))
		{
			// We can't connect or something like this.
			(*iter)->failReason = String("HTTP: %s", pUserdata->httpDownloader->GetLastErrorString());
			if(pUserdata->downloadFailedHandler != NULL)
				pUserdata->downloadFailedHandler();
			return;
		}
		// Downloading:
		pUserdata->httpDownloader->SetFile(f);
		while(pUserdata->httpDownloader->GettingData() || !pUserdata->httpDownloader->GotData())
			pUserdata->httpDownloader->Process();
		pUserdata->httpDownloader->Reset();
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
		(*iter)->okay = true;	
		if(pUserdata->downloadedHandler != NULL)
			pUserdata->downloadedHandler();	
		pUserdata->currentFile = NULL;
		g_pFileTransfer->SetCurrentFileText();
	}
	pUserdata->bDownloadCompleted = true;
	g_pFileTransfer->SetDownloadFinished(true);
	pUserdata->busy = false;
}

// Handlers for client files download
void FileTransfer_DownloadedFile()
{
	FileDownload * file = g_pFileTransfer->GetCurrentFile();
	
	 if(file->type == FileDownloadCategory::Script)
	{
		CLogFile::PrintDebugf("Loading client script: %s.", file->name.Get());
		g_pClientScriptManager->AddScript(file->name.Get(), file->fileName.Get());
		if(g_pLocalPlayer->GetFirstSpawn())
			g_pClientScriptManager->Load(file->name);	
	}
}
void FileTransfer_DownloadFailed()
{
	FileDownload * file = g_pFileTransfer->GetCurrentFile();
	String errorMessage("Download failed: %s (%s)", file->name.Get(), file->failReason.Get());	
	CLogFile::Print(errorMessage);

	g_pFileTransfer->Process();
	/*g_pNetworkManager->Disconnect();	
	g_pMainMenu->ResetNetworkStats();
	g_pChatWindow->SetEnabled(false);
	g_pMainMenu->SetDisconnectButtonVisible(false);
	g_pMainMenu->SetVisible(true);
	g_pGUI->ShowMessageBox(errorMessage.Get(), "Failed to join server");*/
}