//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.h
// Project: Client.Core
// Author(s): Einstein
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <CFileChecksum.h>
#include <Network/CHttpClient.h>
#include <Threading\CThread.h>

// Categories of client files
enum FileDownloadCategory { Resource, Script };

// Represents client file
struct FileDownload							// represents information about 1 file we need to process
{
	String	name;							// client resource name
	String	fileName;						// full file name (local file system)
	String	failReason;						// compile/download fail message
	bool	okay;							// downloaded & compiled OK
	CFileChecksum checksum;					// checksum (of target file on server)
	FileDownloadCategory type;				// type of file(category)
	FILE * File;							// file handle to write to (should be NULL)

public: 
	FileDownload(String fName, CFileChecksum fChecksum, FileDownloadCategory fCategory)
	{
		name = fName;
		checksum = fChecksum;
		type = fCategory;
		okay = false;
	}
};

// Function type for "file downloaded" callback
typedef void (* DownloadHandler_t)();

// Communicate object between threads
struct ThreadUserData	
{	
	std::list<FileDownload *> m_fileList;
	bool busy;
	bool bDownloadCompleted;
	CHttpClient * httpDownloader;
	FileDownload * currentFile;
	DownloadHandler_t downloadedHandler;
	DownloadHandler_t downloadFailedHandler;

public:
	ThreadUserData()
	{
		httpDownloader = new CHttpClient();
		m_fileList = std::list<FileDownload *>();
		bDownloadCompleted = false;
		busy = false;
		downloadedHandler = NULL;
	}
};

class CFileTransfer	
{
private:
	CThread *	m_thread;
	ThreadUserData * m_userdata;

public:
	CFileTransfer();
	void AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource);
	bool Process();
	void SetServerInformation(String strAddress, unsigned short usPort);
	bool IsBusy();
	FileDownload * GetFailedResource();
	FileDownload * GetCurrentFile() { return m_userdata->currentFile; }
	void CompileScripts();
	int GetTransferListSize();
	void Reset();
	void SetDownloadImageVisible(bool visible) { };
	void SetDownloadedHandler(DownloadHandler_t handler) { m_userdata->downloadedHandler = handler; }
	void SetDownloadFailedHandler(DownloadHandler_t handler) { m_userdata->downloadFailedHandler = handler; }
};
// Threaded static functions:
void WorkAsync(CThread * pCreator);
//static bool ReceiveHandler(const char * szData, unsigned int uiDataSize, void * pUserData_);
//bool WorkAsync_FileRecv(const char * szData, unsigned int uiDataSize,	void * pUserData);

/*
private:
	CHttpClient             m_httpClient;
	std::list<ServerFile *> m_fileList;
	bool                    m_bDownloadingFile;
	ServerFile            * m_pDownloadFile;
	FILE                  * m_fDownloadFile;
	CGUIStaticText		  * m_pFileText;
	CGUIStaticImage		  * m_pFileImage;
public:
	CFileTransfer();

	unsigned int GetTransferListSize() { return m_fileList.size(); }
	void         SetServerInformation(String strAddress, unsigned short usPort);
	void         AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource);
	void         Process();
	void		 SetDownloadImageVisible(bool bVisible) { m_pFileImage->setVisible(bVisible); }
	void         Reset();
};*/


