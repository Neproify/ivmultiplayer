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

extern CClientScriptManager * g_pClientScriptManager;
extern CChatWindow          * g_pChatWindow;
extern CNetworkManager      * g_pNetworkManager;

CFileTransfer::CFileTransfer()
{
	// Set the http client receive handler
	m_httpClient.SetReceiveHandle(ReceiveHandler, this);

	m_bDownloadingFile = false;
	m_pDownloadFile = NULL;
	m_fDownloadFile = NULL;
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

				// Load the script
				g_pClientScriptManager->Load(strFileName);
			}

			return;
		}
	}

	ServerFile * pServerFile = new  ServerFile;
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
					g_pChatWindow->AddInfoMessage("Failed to start download of %s %s", pServerFile->strType.Get(), pServerFile->strName.Get());

					// Reset all transfers
					Reset();

					// Disconnect from the server
					g_pNetworkManager->Disconnect();
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

						// Load the script
						g_pClientScriptManager->Load(m_pDownloadFile->strName);
					}

					// Notice user that file is sucessfully downloaded
					g_pChatWindow->AddInfoMessage("File %s sucessfully downloaded.", m_pDownloadFile->strName.C_String());

					
					// Remove the download file from the file list
					m_fileList.remove(m_pDownloadFile);

					// Delete the download file
					delete m_pDownloadFile;

					// Reset the download file
					m_pDownloadFile = NULL;

					// Flag ourselves as no longer downloading
					m_bDownloadingFile = false;

					// Reset out http client
					m_httpClient.Reset();
					
				}
				else
				{
					g_pChatWindow->AddInfoMessage("Failed to download %s %s (Checksum mismatch)", m_pDownloadFile->strType.Get(), m_pDownloadFile->strName.Get());

					// Reset all transfers
					Reset();

					// Disconnect from the server
					g_pNetworkManager->Disconnect();
				}
			}
			else
			{
				g_pChatWindow->AddInfoMessage("Failed to download %s %s (%s)", m_pDownloadFile->strType.Get(), m_pDownloadFile->strName.Get(), m_httpClient.GetLastErrorString().Get());

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
		delete (*iter);

	m_fileList.clear();

	// Reset download vars
	m_bDownloadingFile = false;

	m_pDownloadFile = NULL;

	if(m_fDownloadFile)
		fclose(m_fDownloadFile);

	m_fDownloadFile = NULL;
}
