//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerQuery.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>
#include <list>
#include <Network/CBitStream.h>

struct ServerQueryItem
{
	String         strHost;
	unsigned short usPort;
	String         strQuery;
	unsigned long  ulTime;
};

typedef void (* ServerQueryHandler_t)(String strHost, unsigned short usPort, String strQuery, CBitStream * pReply);

class CServerQuery
{
private:
	int                          m_iSocket;
	std::list<ServerQueryItem *> m_serverQueryList;
	ServerQueryHandler_t         m_pfnServerQueryHandler;

	ServerQueryItem * GetServerQuery(String strHost, unsigned short usPort);

public:
	CServerQuery();
	~CServerQuery();

	void              Reset();
	bool              Query(String strHost, unsigned short usPort, String strQuery);
	void              Process();
	void              SetServerQueryHandler(ServerQueryHandler_t pfnServerQueryHandler) { m_pfnServerQueryHandler = pfnServerQueryHandler; }
};
