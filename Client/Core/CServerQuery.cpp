//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerQuery.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CServerQuery.h"
#include <winsock2.h>
#include <winsock.h>
#include <SharedUtility.h>

CServerQuery::CServerQuery()
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Prepare the socket
	m_iSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Set the socket to non blocking
	u_long sockopt = 1;
	ioctlsocket(m_iSocket, FIONBIO, &sockopt);
}

CServerQuery::~CServerQuery()
{
	// Reset all queries
	Reset();

	// Is the socket valid?
	if(m_iSocket != INVALID_SOCKET)
	{
		// Close the socket
		closesocket(m_iSocket);

		// Invalidate the socket
		m_iSocket = INVALID_SOCKET;
	}

	// If windows cleanup winsock
#ifdef WIN32
	WSACleanup();
#endif
}

ServerQueryItem * CServerQuery::GetServerQuery(String strHost, unsigned short usPort)
{
	for(std::list<ServerQueryItem *>::iterator iter = m_serverQueryList.begin(); iter != m_serverQueryList.end(); iter++)
	{
		ServerQueryItem * pServerQuery = *iter;

		if(pServerQuery->strHost == strHost && pServerQuery->usPort == usPort)
			return pServerQuery;
	}

	return NULL;
}

void CServerQuery::Reset()
{
	for(std::list<ServerQueryItem *>::iterator iter = m_serverQueryList.begin(); iter != m_serverQueryList.end(); iter++)
		delete *iter;

	m_serverQueryList.clear();
}

bool CServerQuery::Query(String strHost, unsigned short usPort, String strQuery)
{
	// Create the query bit stream
	CBitStream bitStream;

	// Write the query identifier
	bitStream.Write("IVMP", 4);

	// Write the query
	bitStream.Write(strQuery.Get(), strQuery.GetLength());

	// Prepare the query address
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(usPort + QUERY_PORT_OFFSET);
	addr.sin_addr.s_addr = inet_addr(strHost.Get());

	// Send the query
	if(sendto(m_iSocket, (char *)bitStream.GetData(), bitStream.GetNumberOfBytesUsed(), 0, (sockaddr *)&addr, sizeof(sockaddr_in)) == bitStream.GetNumberOfBytesUsed())
	{
		// Create the server query
		ServerQueryItem * pServerQuery = new ServerQueryItem;

		// Ensure the server query created successfully
		if(!pServerQuery)
			return false;

		// Set the server query information
		pServerQuery->strHost = strHost;
		pServerQuery->usPort = usPort;
		pServerQuery->strQuery = strQuery;
		pServerQuery->ulTime = SharedUtility::GetTime();

		// Add the server query to the server query list
		m_serverQueryList.push_back(pServerQuery);
		return true;
	}

	// Query not sent
	return false;
}

void CServerQuery::Process()
{
	// Remove any stale queries
	for(std::list<ServerQueryItem *>::iterator iter = m_serverQueryList.begin(); iter != m_serverQueryList.end(); )
	{
		ServerQueryItem * pServerQuery = *iter;

		if((SharedUtility::GetTime() - pServerQuery->ulTime) > 10000) // 10 seconds
		{
			// Remove the query
			delete pServerQuery;
			iter = m_serverQueryList.erase(iter);
		}
		else
			iter++;
	}

	char szBuffer[1024];
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	int iFromLen = sizeof(sockaddr_in);

	// Loop until we've read everything in the receive buffer
	int iBytesRead = -1;

	while((iBytesRead = recvfrom(m_iSocket, szBuffer, sizeof(szBuffer), 0, (sockaddr *)&addr, &iFromLen)) != -1)
	{
		// Convert the ip address to a string
		char szIpAddress[64];
		SharedUtility::inet_ntop(addr.sin_family, &addr.sin_addr, szIpAddress, sizeof(szIpAddress));

		// Get the port
		unsigned short usPort = ntohs(addr.sin_port);

		// Get the server query item
		ServerQueryItem * pServerQuery = GetServerQuery(szIpAddress, (usPort - QUERY_PORT_OFFSET));

		// Invalid server query item?
		if(!pServerQuery)
			continue;

		// Create a bit stream from the data
		CBitStream bitStream((unsigned char *)szBuffer, iBytesRead, true);

		// Read the first 4 bytes
		char szIdentifier[4];

		if(!bitStream.Read(szIdentifier, sizeof(szIdentifier)))
			continue;

		// Ensure the first 4 bytes are 'IVMP'
		if(szIdentifier[0] != 'I' || szIdentifier[1] != 'V' || szIdentifier[2] != 'M' || szIdentifier[3] != 'P')
			continue;

		// Call the server query callback (if we have one)
		if(m_pfnServerQueryHandler)
			m_pfnServerQueryHandler(pServerQuery->strHost, pServerQuery->usPort, pServerQuery->strQuery, &bitStream);
		
		// Remove the server query
		delete pServerQuery;
		for(std::list<ServerQueryItem *>::iterator iter = m_serverQueryList.begin(); iter != m_serverQueryList.end(); iter++)
		{
			if(*iter == pServerQuery)
			{
				m_serverQueryList.erase(iter);
				break;
			}
		}
	}
}
