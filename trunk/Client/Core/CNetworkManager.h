//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <SharedUtility.h>
#include <Network/CNetClientInterface.h>
#include "CClientPacketHandler.h"
#include "CClientRPCHandler.h"

enum eNetState
{
	NET_STATE_NONE,
	NET_STATE_CONNECTING,
	NET_STATE_JOINING,
	NET_STATE_CONNECTED,
	NET_STATE_DISCONNECTED
};

class CNetworkManager
{
private:
	CNetClientInterface  * m_pNetClient;
	CClientPacketHandler * m_pClientPacketHandler;
	CClientRPCHandler    * m_pClientRPCHandler;
	String                 m_sHostName;
	bool                   m_bJoinedServer;
	bool                   m_bJoinedGame;
	int					   m_iMaxPlayers;

public:
	CNetworkManager();
	~CNetworkManager();

	CNetClientInterface * GetNetClient() { return m_pNetClient; }
	String                GetHostName() { return m_sHostName; };
	void                  SetHostName(String sHostName) { m_sHostName = sHostName; };
	void				  SetMaxPlayers(int iPlayers) { m_iMaxPlayers = iPlayers; };
	int					  GetMaxPlayers() { return m_iMaxPlayers; };
	void                  SetJoinedServer(bool bJoinedServer) { m_bJoinedServer = bJoinedServer; }
	bool                  HasJoinedGame() { return m_bJoinedGame; }
	void                  Startup(String strHost, unsigned short usPort, String strPassword);
	static void           PacketHandler(CPacket * pPacket);
	void                  Process();
	void                  Connect();
	bool                  IsConnected();
	void                  Disconnect();
	void                  RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel = PACKET_CHANNEL_DEFAULT);
};
