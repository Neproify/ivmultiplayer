//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMasterListQuery.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <vector>
#include <CString.h>
#include <Network/CHttpClient.h>

typedef void (* MasterListQueryHandler_t)(std::vector<String> serverVector);

class CMasterListQuery
{
private:
	CHttpClient            * m_pHttpClient;
	String                   m_strVersion;
	MasterListQueryHandler_t m_pfnMasterListQueryHandler;

public:
	CMasterListQuery(String strHost, String strVersion);
	~CMasterListQuery();

	CHttpClient * GetHttpClient() { return m_pHttpClient; }
	void          Reset();
	bool          Query(int iType);
	void          Process();
	void          SetMasterListQueryHandler(MasterListQueryHandler_t pfnMasterListQueryHandler) { m_pfnMasterListQueryHandler = pfnMasterListQueryHandler; }
};
