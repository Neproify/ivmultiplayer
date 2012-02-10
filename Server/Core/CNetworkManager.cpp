//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkManager.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Refresh ban list function
// TODO: Convert ban list to XML?

#ifdef _LINUX
#include <stdlib.h>
#endif

#include <ctime>
#include "CPlayerManager.h"
#include "CNetworkManager.h"
#include <Network/CNetworkModule.h>
#include <CLogFile.h>

extern CPlayerManager  * g_pPlayerManager;
extern CNetworkManager * g_pNetworkManager;

CNetworkManager::CNetworkManager()
{
	// Create the net server instance
	m_pNetServer = CNetworkModule::GetNetServerInterface();

	// Set the net server packet handler function
	m_pNetServer->SetPacketHandler(PacketHandler);

	// Create the packet handler instance
	m_pServerPacketHandler = new CServerPacketHandler();

	// Create the rpc handler instance
	m_pServerRPCHandler = new CServerRPCHandler();

	// Flag ourselves as running
	bRunning = true;
}

CNetworkManager::~CNetworkManager()
{
	// Flag ourselves as not running
	bRunning = false;

	// Unregister the rpcs
	m_pServerRPCHandler->Unregister();

	// Delete the rpc handler instance
	SAFE_DELETE(m_pServerRPCHandler);

	// Unregister the packets
	m_pServerPacketHandler->Unregister();

	// Delete the packet handler instance
	SAFE_DELETE(m_pServerPacketHandler);

	// Shutdown the net server instance
	m_pNetServer->Shutdown(500);

	// Delete the net server instance
	CNetworkModule::DestroyNetServerInterface(m_pNetServer);
}

bool CNetworkManager::Startup(int iPort, int iMaxPlayers, String strPassword, String strHostAddress)
{
	// Start up the net server
	if(!m_pNetServer->Startup(iPort, iMaxPlayers, strHostAddress.Get()))
		return false;

	// Set the net server password
	m_pNetServer->SetPassword(strPassword);

	// Register the packets
	m_pServerPacketHandler->Register();

	// Register the rpcs
	m_pServerRPCHandler->Register();
	return true;
}

void CNetworkManager::PacketHandler(CPacket * pPacket)
{
	// Get the network manager pointer
	CNetworkManager * pNetworkManager = g_pNetworkManager;

	// Pass it to the packet handler, if that doesn't handle it, pass it to the rpc handler
	if(!pNetworkManager->m_pServerPacketHandler->HandlePacket(pPacket) && 
		!pNetworkManager->m_pServerRPCHandler->HandlePacket(pPacket))
	{
#ifdef IVMP_DEBUG
		CLogFile::Printf("Warning: Unhandled packet (Id: %d, Player: %d)\n", pPacket->packetId, pPacket->pPlayerSocket->playerId);
#endif
	}
}

void CNetworkManager::Process()
{
	// Process the net server
	m_pNetServer->Process();

	// Process the player manager
	g_pPlayerManager->Pulse();
}

void CNetworkManager::RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel)
{
	m_pNetServer->RPC(rpcId, pBitStream, priority, reliability, playerId, bBroadcast, cOrderingChannel);
}

String CNetworkManager::GetPlayerIp(EntityId playerId)
{
	return m_pNetServer->GetPlayerIp(playerId);
}

unsigned short CNetworkManager::GetPlayerPort(EntityId playerId)
{
	return m_pNetServer->GetPlayerPort(playerId);
}

String CNetworkManager::GetPlayerSerial(EntityId playerId)
{
	return m_pNetServer->GetPlayerSerial(playerId);
}

bool CNetworkManager::AddBan(String strIp, unsigned int uiSeconds)
{
	FILE * fBanList = fopen("bans.banlist", "a");

	if(!fBanList)
		return false;

	int uiTimestamp = (int)time(0);
	fprintf(fBanList, "%s:%d:%d\n", strIp.Get(), uiTimestamp, uiSeconds);
	fclose(fBanList);
	m_pNetServer->BanIp(strIp, uiSeconds * 1000);
	return true;
}

bool CNetworkManager::LoadBans()
{
	FILE * fBanList = fopen("bans.banlist", "r");

	if(!fBanList)
		return false;

	char szBuf[256];
	unsigned int uiTimestamp = (unsigned int)time(0);
	while(!feof(fBanList))
	{
		fgets(szBuf, sizeof(szBuf), fBanList);

		if(szBuf && szBuf[0] != '\0')
		{
			szBuf[strlen(szBuf) - 1] = '\0';
			char * szIP = strtok(szBuf, ":");
			unsigned int uiBanStart = atoi(strtok(NULL, ":"));
			unsigned int uiBanSeconds = (atoi(strtok(NULL, "")) + uiBanStart);

			if(uiBanSeconds > uiTimestamp)
				m_pNetServer->BanIp(szIP, (uiBanSeconds - uiTimestamp));

			szBuf[0] = '\0';
		}
	}

	fclose(fBanList);
	return true;
}
