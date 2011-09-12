//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMasterList.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetworkManager.h"
#include <Network/CHttpClient.h>

class CMasterList
{
private:
	CHttpClient * m_pHttpClient;
	unsigned long m_ulLastPulse;
	String        m_strPostPath;
	unsigned long m_ulTimeout;
	bool          m_bSentListedMessage;
	bool          m_bSentErrorMessage;

public:
	CMasterList(String strHost, String strVersion, unsigned long ulTimeout, int iServerPort);
	~CMasterList();
	
	void Pulse();
};
