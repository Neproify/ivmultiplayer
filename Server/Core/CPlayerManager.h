//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerManager.h
// Project: Server.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "Interfaces/InterfaceCommon.h"
#include "CPlayer.h"

class CPlayerManager : public CPlayerManagerInterface
{
private:
	bool m_bActive[MAX_PLAYERS];
	CPlayer * m_pPlayers[MAX_PLAYERS];

public:
	CPlayerManager();
	~CPlayerManager();

	bool DoesExist(EntityId playerId);
	void Add(EntityId playerId, String sPlayerName);
	void Add(EntityId playerId, char * sPlayerName);
	bool Remove(EntityId playerId, BYTE byteReason);
	void Pulse();
	void HandleClientJoin(EntityId playerId);
	bool IsNameInUse(String szNick);
	bool IsNameInUse(char * sNick);
	EntityId GetPlayerFromName(String sNick);
	EntityId GetPlayerFromName(char * sNick);
	EntityId GetPlayerCount();
	CPlayer * GetAt(EntityId playerId);
};