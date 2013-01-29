//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransferManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <CString.h>
#include <Threading\CThread.h>
#include "CFileTransfer.h"
#include "CFileChecksum.h"
#include "CGUI.h"

class CFileTransferManager
{
private:
	String                     m_strHost;
	unsigned short             m_usPort;
	CMutex                     m_transferListMutex;
	std::list<CFileTransfer *> m_transferList;
	CThread                    m_transferThread;
	CGUIStaticText           * m_pFileText;
	CGUIStaticImage	         * m_pFileImage;

	static void    TransferThread(CThread * pCreator);

public:
	CFileTransferManager();
	~CFileTransferManager();

	void           SetHost(String strHost) { m_strHost = strHost; }
	String         GetHost() { return m_strHost; }
	void           SetPort(unsigned short usPort) { m_usPort = usPort; }
	unsigned short GetPort() { return m_usPort; }

	void           Add(bool bIsResource, String strName, CFileChecksum checksum);
	bool           Remove(String strName);
	bool           Clear(bool bKillThread = false);
	bool           IsComplete();
	void           Process();
};
