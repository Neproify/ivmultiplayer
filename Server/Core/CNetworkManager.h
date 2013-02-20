//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkManager.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include "CServer.h"
#include "Interfaces/InterfaceCommon.h"
#include <Network/CNetServerInterface.h>
#include "CServerPacketHandler.h"
#include "CServerRPCHandler.h"

class CNetworkManager : public CNetworkManagerInterface
{
private:
	CNetServerInterface  * m_pNetServer;
	CServerPacketHandler * m_pServerPacketHandler;
	CServerRPCHandler    * m_pServerRPCHandler;

public:
	CNetworkManager();
	~CNetworkManager();

	CNetServerInterface * GetNetServer() { return m_pNetServer; }
	bool                  Startup(int iPort, int iMaxPlayers, String strPassword, String strHostAddress);
	static void           PacketHandler(CPacket * pPacket);
	void                  Process();
	void                  RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId = INVALID_ENTITY_ID, bool bBroadcast = true, char cOrderingChannel = PACKET_CHANNEL_DEFAULT);
	String                GetPlayerIp(EntityId playerId);
	unsigned short        GetPlayerPort(EntityId playerId);
	String                GetPlayerSerial(EntityId playerId);
	bool                  AddBan(String strIp, unsigned int uiSeconds);
	bool                  LoadBans();
};
