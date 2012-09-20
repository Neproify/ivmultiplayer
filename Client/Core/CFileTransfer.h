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
#include "CGUI.h"

struct ServerFile
{
	String        strName;
	CFileChecksum fileChecksum;
	String        strType;
};

class CFileTransfer
{
private:
	CHttpClient             m_httpClient;
	std::list<ServerFile *> m_fileList;
	bool                    m_bDownloadingFile;
	ServerFile            * m_pDownloadFile;
	FILE                  * m_fDownloadFile;
	CGUIStaticText		  * m_pFileText;
	CGUIStaticImage		  * m_pFileImage;

private:
	static bool  ReceiveHandler(const char * szData, unsigned int uiDataSize, void * pUserData);
	bool         StartDownload(String strName, String strType);

public:
	CFileTransfer();

	unsigned int GetTransferListSize() { return m_fileList.size(); }
	void         SetServerInformation(String strAddress, unsigned short usPort);
	void         AddFile(String strFileName, CFileChecksum fileChecksum, bool bIsResource);
	void         Process();
	void		 SetDownloadImageVisible(bool bVisible) { m_pFileImage->setVisible(bVisible); }
	void         Reset();
};
