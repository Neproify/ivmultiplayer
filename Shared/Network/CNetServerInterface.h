//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetServerInterface.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CPacket.h"
#include "CBitStream.h"
#include "PacketPriorities.h"
#include "PacketReliabilities.h"
#include "PacketChannels.h"
#include "RPCIdentifiers.h"

typedef void (* PacketHandler_t)(CPacket * pPacket);

class CNetServerInterface
{
public:
	virtual ~CNetServerInterface() { }

	virtual bool            Startup(unsigned short usPort, int iMaxPlayers, String strHostAddress = "") = 0;
	virtual void            Shutdown(int iBlockDuration) = 0;
	virtual void            Process() = 0;
	virtual void            SetPassword(String strPassword) = 0;
	virtual const char    * GetPassword() = 0;
	virtual unsigned int    Send(CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = PACKET_CHANNEL_DEFAULT) = 0;
	virtual unsigned int    RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = PACKET_CHANNEL_DEFAULT) = 0;
	virtual const char    * GetPlayerIp(EntityId playerId) = 0;
	virtual unsigned short  GetPlayerPort(EntityId playerId) = 0;
	virtual void            SetPacketHandler(PacketHandler_t pfnPacketHandler) = 0;
	virtual PacketHandler_t GetPacketHandler() = 0;
	virtual const char    * GetPlayerSerial(EntityId playerId) = 0;
	virtual void            KickPlayer(EntityId playerId, bool bSendDisconnectionNotification = true, ePacketPriority disconnectionPacketPriority = PRIORITY_LOW) = 0;
	virtual CPlayerSocket * GetPlayerSocket(EntityId playerId) = 0;
	virtual bool            IsPlayerConnected(EntityId playerId) = 0;
	virtual void            BanIp(String strIpAddress, unsigned int uiTimeMillseconds) = 0;
	virtual void            UnbanIp(String strIpAddress) = 0;
	virtual int             GetPlayerLastPing(EntityId playerId) = 0;
	virtual int             GetPlayerAveragePing(EntityId playerId) = 0;
};
