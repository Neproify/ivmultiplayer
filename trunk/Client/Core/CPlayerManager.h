//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerManager.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetworkPlayer.h"
#include "CLocalPlayer.h"
#include "CIVPed.h"

extern CLocalPlayer * g_pLocalPlayer;

DWORD SkinIdToModelHash(int modelid);
int ModelHashToSkinId(DWORD modelhash);

class CPlayerManager
{
private:
	bool             m_bActive[MAX_PLAYERS];
	bool             m_bCreated[MAX_PLAYERS];
	CNetworkPlayer * m_pPlayers[MAX_PLAYERS];

public:
	CPlayerManager();
	~CPlayerManager();

	void             Add(EntityId playerId, String szPlayerName);
	bool             Remove(EntityId playerId);
	void             Spawn(EntityId playerId, int iModelId, CVector3 vecSpawnPos, float fSpawnHeading);
	void             Pulse();
	void             SetLocalPlayer(EntityId playerId, CNetworkPlayer * pPlayer);
	bool             DoesExist(EntityId playerId);
	bool			 IsActive(EntityId playerId) { return m_bActive[playerId]; }
	CNetworkPlayer * GetAt(EntityId playerId);
	CNetworkPlayer * GetFrom(IVPed * pIVPed);
};
