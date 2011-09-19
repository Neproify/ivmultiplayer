//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CQuery.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CQuery.h"
#include <CSettings.h>
#include <CEvents.h>
#include <SharedUtility.h>
#include "CPlayerManager.h"
#include <CLogFile.h>
#ifdef _LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define closesocket close
#else
typedef int socklen_t;
#endif


extern CEvents        * g_pEvents;
extern CPlayerManager * g_pPlayerManager;

CQuery::CQuery(unsigned short usPort, String strHostAddress)
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Create an address
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(usPort + QUERY_PORT_OFFSET);

	// Do we have a host address?
	if(strHostAddress.IsNotEmpty())
		addr.sin_addr.s_addr = inet_addr(strHostAddress.Get());

	// Create a socket
	m_iSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// TODO: ERROR CHECKING

	// Set the socket to non blocking
#ifdef WIN32
	unsigned long sockopt = 1;
	ioctlsocket (m_iSocket, FIONBIO, &sockopt);
#else
	fcntl(m_iSocket, F_SETFL, fcntl(m_iSocket, F_GETFL) | O_NONBLOCK); 
#endif

	// Bind the socket to the address
	if(bind(m_iSocket, (sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		CLogFile::Printf("Failed to bind query port %d. Server will not respond to queries.\n", (usPort + QUERY_PORT_OFFSET));
}

CQuery::~CQuery()
{
	if(m_iSocket != -1)
		closesocket(m_iSocket);

	// If windows cleanup winsock
#ifdef WIN32
	WSACleanup();
#endif
}

// TODO: Flood protection
void CQuery::Process()
{
	// Do we have a valid socket?
	if(m_iSocket != -1)
	{
		// Reset the buffer
		static char szBuffer[1024];
		memset(szBuffer, 0, sizeof(szBuffer));

		// Create the address
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		int iFromLen = sizeof(sockaddr_in);

		// Attempt to read data from the socket
		int iBytesRead = -1;

		while((iBytesRead = recvfrom(m_iSocket, szBuffer, sizeof(szBuffer), NULL, (sockaddr *)&addr, (socklen_t *)&iFromLen)) != -1)
		{
			// Convert the ip address to a string
			char szIpAddress[64];
			SharedUtility::inet_ntop(addr.sin_family, &addr.sin_addr, szIpAddress, sizeof(szIpAddress));

			// Get the port
			unsigned short usPort = ntohs(addr.sin_port);

			// Create a bit stream from the data
			CBitStream bitStream((unsigned char *)szBuffer, iBytesRead, true);

			// Read the first 4 bytes
			char szIdentifier[4];

			if(!bitStream.Read(szIdentifier, sizeof(szIdentifier)))
				continue;

			// Ensure the first 4 bytes are 'IVMP'
			if(szIdentifier[0] != 'I' || szIdentifier[1] != 'V' || szIdentifier[2] != 'M' || szIdentifier[3] != 'P')
				continue;

			// Create the reply bit stream
			CBitStream reply;

			// Are frequent events enabled?
			if(CVAR_GET_BOOL("frequentevents"))
			{
				// Create the arguments
				CSquirrelArguments pArguments;
				pArguments.push(szIpAddress);
				pArguments.push(usPort - QUERY_PORT_OFFSET);
				pArguments.push(String((char *)bitStream.GetData()));
				pArguments.push((int)bitStream.GetNumberOfBytesUsed());

				// Call the 'serverQueryReceived' event
				CSquirrelArgument result = g_pEvents->Call("serverQueryReceived", &pArguments);

				// Was the query refused?
				if((result.GetType() == OT_INTEGER && result.GetInteger() == 0) || (result.GetType() == OT_STRING && strlen(result.GetString()) == 0))
				{
					// A script refused the query
					continue;
				}

				// Was a string returned?
				if(result.GetType() == OT_STRING)
				{
					// Write the string to the reply
					reply.Write(result.GetString(), strlen(result.GetString()));
				}
			}

			// Do we not have a reply from the script?
			if(reply.GetNumberOfBytesUsed() == 0)
			{
				// Read the query type
				char cQueryType;

				if(!bitStream.Read(cQueryType))
					continue;

				// Process the query
				if(cQueryType == 'i') // Server Information
				{
					// Write 'IVMP' and the query type
					reply.Write(szIdentifier, sizeof(szIdentifier));
					reply.Write(cQueryType);

					// Write the host name
					reply.Write(CVAR_GET_STRING("hostname"));

					// Write the player count
					reply.Write((int)g_pPlayerManager->GetPlayerCount());

					// Write the max player limit
					reply.Write(CVAR_GET_INTEGER("maxplayers"));

					// Write if the server is passworded or not
					reply.Write((CVAR_GET_STRING("password").IsEmpty() ? 0 : 1));
				}
				else if(cQueryType == 'p') // Ping
				{
					// Write 'IVMP' and the query type
					reply.Write(szIdentifier, sizeof(szIdentifier));
					reply.Write(cQueryType);

					// Write a 'PONG' string
					reply.Write("PONG");
				}
				else if(cQueryType == 'l') // Player List
				{
					// Write 'IVMP' and the query type
					reply.Write(szIdentifier, sizeof(szIdentifier));
					reply.Write(cQueryType);

					// Write the player count
					reply.Write(g_pPlayerManager->GetPlayerCount());

					// Loop through all players
					for(int x = (MAX_PLAYERS - 1); x >= 0; x--)
					{
						// Check if the current player is connected
						if(g_pPlayerManager->DoesExist(x))
						{	
							CPlayer * pPlayer = g_pPlayerManager->GetAt(x);

							if(pPlayer)
							{
								// Write the player id
								reply.Write(x);

								// Write the name
								reply.Write(pPlayer->GetName());

								// Write the player ping
								reply.Write(pPlayer->GetPing());

								// Get the players vehicle
								CVehicle * pVehicle = pPlayer->GetVehicle();

								// Is in the player in a vehicle?
								if(pVehicle)
									reply.Write(pVehicle->GetVehicleId());
								else
									reply.Write((EntityId)INVALID_ENTITY_ID);

								// Write the player weapon
								reply.Write(pPlayer->GetWeapon());
							}
						}
					}
				}
				else if(cQueryType == 'r') // Rule List
				{
					// Write 'IVMP' and the query type
					reply.Write(szIdentifier, sizeof(szIdentifier));
					reply.Write(cQueryType);

					// Write the rules count
					reply.Write(m_rules.size());

					// Loop through all rules
					for(std::list<QueryRule *>::iterator iter = m_rules.begin(); iter != m_rules.end(); iter++)
					{
						// Get the rule pointer
						QueryRule * pRule = (*iter);

						// Write the rule
						reply.Write(pRule->strRule);

						// Write the rule value
						reply.Write(pRule->strValue);
					}
				}
				else if(cQueryType == 'v') // Version
				{
					// Write 'IVMP' and the query type
					reply.Write(szIdentifier, sizeof(szIdentifier));
					reply.Write(cQueryType);

					// Get the version string
					String strVersion(VERSION_IDENTIFIER);

					// Write the version string
					reply.Write(strVersion);
				}
			}

			// Do we have a reply to send?
			if(reply.GetNumberOfBytesUsed() > 0)
			{
				// Send the reply
				if(sendto(m_iSocket, (char *)reply.GetData(), reply.GetNumberOfBytesUsed(), NULL, (sockaddr *)&addr, sizeof(sockaddr_in)) != reply.GetNumberOfBytesUsed())
				{
					// Sending failed
					// TODO: When flood protection is done, enable this
					//LogPrintf("Warning: Failed to send query response.");
				}
			}
		}
	}
}

bool CQuery::DoesRuleExist(String strRule)
{
	// Loop through all rules
	for(std::list<QueryRule *>::iterator iter = m_rules.begin(); iter != m_rules.end(); iter++)
	{
		// Get the rule pointer
		QueryRule * pRule = (*iter);

		// Is this the rule we are looking for?
		if(pRule->strRule == strRule)
			return true;
	}

	return false;
}

QueryRule * CQuery::GetRule(String strRule)
{
	// Loop through all rules
	for(std::list<QueryRule *>::iterator iter = m_rules.begin(); iter != m_rules.end(); iter++)
	{
		// Get the rule pointer
		QueryRule * pRule = (*iter);

		// Is this the rule we are looking for?
		if(pRule->strRule == strRule)
			return pRule;
	}

	return NULL;
}

bool CQuery::AddRule(String strRule, String strValue)
{
	// Does the rule already exist?
	if(DoesRuleExist(strRule))
		return false;

	// Create a new rule
	QueryRule * pRule = new QueryRule;
	pRule->strRule = strRule;
	pRule->strValue = strValue;

	// Add it to the rule list
	m_rules.push_back(pRule);
	return true;
}

bool CQuery::RemoveRule(String strRule)
{
	// Get the rule
	QueryRule * pRule = GetRule(strRule);

	// Does the rule not already exist?
	if(!pRule)
		return false;

	// Delete the rule and remove it from the rule list
	delete pRule;
	m_rules.remove(pRule);
	return true;
}

bool CQuery::SetRule(String strRule, String strValue)
{
	// Get the rule
	QueryRule * pRule = GetRule(strRule);

	// Does the rule not already exist?
	if(!pRule)
		return false;

	// Set the rule value
	pRule->strValue = strValue;
	return true;
}

String CQuery::GetRuleValue(String strRule)
{
	// Get the rule
	QueryRule * pRule = GetRule(strRule);

	// Does the rule not already exist?
	if(!pRule)
		return false;

	return pRule->strValue;
}
