//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpoint.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCheckpoint.h"
#include "CNetworkManager.h"
#include "CPlayerManager.h"

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager  * g_pPlayerManager;

CCheckpoint::CCheckpoint(EntityId checkpointId, WORD wType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius)
{
	m_checkpointId = checkpointId;
	m_wType = wType;
	m_vecPosition = vecPosition;
	m_vecTargetPosition = vecTargetPosition;
	m_fRadius = fRadius;
	m_bShow = true;
}

CCheckpoint::~CCheckpoint()
{

}

void CCheckpoint::AddForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.Write(m_checkpointId);
	bsSend.Write(m_wType);
	bsSend.Write(m_vecPosition);
	bsSend.Write(m_vecTargetPosition);
	bsSend.Write(m_fRadius);
	g_pNetworkManager->RPC(RPC_NewCheckpoint, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

	if(m_bShow)
		ShowForPlayer(playerId);
	else
		HideForPlayer(playerId);
}

void CCheckpoint::AddForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			AddForPlayer(i);
	}
}

void CCheckpoint::DeleteForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.Write(m_checkpointId);
	g_pNetworkManager->RPC(RPC_DeleteCheckpoint, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CCheckpoint::DeleteForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			DeleteForPlayer(i);
	}
}

void CCheckpoint::ShowForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.Write(m_checkpointId);
	bsSend.Write(m_vecPosition);
	bsSend.Write(m_vecTargetPosition);
	g_pNetworkManager->RPC(RPC_ScriptingShowCheckpointForPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CCheckpoint::ShowForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			ShowForPlayer(i);
	}
	m_bShow = true;
}

void CCheckpoint::HideForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.Write(m_checkpointId);
	g_pNetworkManager->RPC(RPC_ScriptingHideCheckpointForPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CCheckpoint::HideForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			HideForPlayer(i);
	}
	m_bShow = false;
}

void CCheckpoint::SetType(WORD wType)
{
	// Set the type
	m_wType = wType;

	// Respawn the checkpoint
	HideForWorld();
	ShowForWorld();
}

void CCheckpoint::SetPosition(CVector3 vecPosition)
{
	// Set the position
	m_vecPosition = vecPosition;

	// Respawn the checkpoint
	HideForWorld();
	ShowForWorld();
}

void CCheckpoint::SetTargetPosition(CVector3 vecTargetPosition)
{
	// Set the target position
	m_vecTargetPosition = vecTargetPosition;

	// Respawn the checkpoint
	HideForWorld();
	ShowForWorld();
}

void CCheckpoint::SetRadius(float fRadius)
{
	// Set the radius
	m_fRadius = fRadius;

	// Respawn the checkpoint
	HideForWorld();
	ShowForWorld();
}


void CCheckpoint::SetDimension(DimensionId ucDimension)
{
	m_ucDimension = ucDimension;

	CBitStream bsSend;
	bsSend.WriteCompressed(GetCheckpointId());
	bsSend.Write(ucDimension);

	g_pNetworkManager->RPC(RPC_ScriptingSetCheckpointDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}
