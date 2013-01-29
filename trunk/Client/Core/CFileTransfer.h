//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CFileChecksum.h"
#include <Network\CHttpClient.h>

class CFileTransfer
{
private:
	bool          m_bIsResource;
	String        m_strName;
	String        m_strHttpPath;
	String        m_strPath;
	CFileChecksum m_checksum;
	CHttpClient   m_httpClient;
	bool          m_bComplete;
	bool          m_bSucceeded;
	String        m_strError;

public:
	CFileTransfer(bool bIsResource, String strHost, unsigned short usPort, String strName, CFileChecksum checksum);
	~CFileTransfer();

	bool   IsResource() { return m_bIsResource; }
	String GetName() { return m_strName; }
	String GetHTTPPath() { return m_strHttpPath; }
	String GetPath() { return m_strPath; }
	bool   IsComplete() { return m_bComplete; }
	bool   HasSucceeded() { return m_bSucceeded; }
	String GetError() { return m_strError; }
	bool   Download();
};
