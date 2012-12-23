//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpointManager.cpp
// Project: Server.Core
// Author(s): MaVe
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCheckpointManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents         * g_pEvents;

CCheckpointManager::CCheckpointManager()
{
	for(EntityId i = 0; i < MAX_CHECKPOINTS; i++)
		m_pCheckpoints[i] = NULL;
}

CCheckpointManager::~CCheckpointManager()
{
	for(EntityId i = 0; i < MAX_CHECKPOINTS; i++)
	{
		if(m_pCheckpoints[i])
			Delete(i);
	}
}

EntityId CCheckpointManager::Add(WORD wType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius)
{
	// Loop through all checkpoints
	for(EntityId i = 0; i < MAX_CHECKPOINTS; i++)
	{
		// Does this checkpoint not exist?
		if(!m_pCheckpoints[i])
		{
			// Create the checkpoint
			CCheckpoint * pCheckpoint = new CCheckpoint(i, wType, vecPosition, vecTargetPosition, fRadius);

			// Set the checkpoint
			m_pCheckpoints[i] = pCheckpoint;

			// Add it for all players
			pCheckpoint->AddForWorld();

			// Call the 'checkpointCreate' scripting event
			CSquirrelArguments pArguments;
			pArguments.push(i);
			g_pEvents->Call("checkpointCreate", &pArguments);
			return i;
		}
	}

	return INVALID_ENTITY_ID;
}

bool CCheckpointManager::Delete(EntityId checkpointId)
{
	// Does this checkpoint not exist?
	if(!m_pCheckpoints[checkpointId])
		return false;

	// Call the 'checkpointDelete' scripting event
	CSquirrelArguments pArguments;
	pArguments.push(checkpointId);
	g_pEvents->Call("checkpointDelete", &pArguments);

	// Delete the checkpoint for all players
	m_pCheckpoints[checkpointId]->DeleteForWorld();

	// Delete the checkpoint
	SAFE_DELETE(m_pCheckpoints[checkpointId]);
	return true;
}

void CCheckpointManager::HandleClientJoin(EntityId playerId)
{
	// Loop through all checkpoints
	for(EntityId i = 0; i < MAX_CHECKPOINTS; i++)
	{
		// Does this checkpoint exist?
		if(m_pCheckpoints[i])
		{
			// Add it for the player
			m_pCheckpoints[i]->AddForPlayer(playerId);
			m_pCheckpoints[i]->SetDimension(m_pCheckpoints[i]->GetDimension());
		}
	}
}

bool CCheckpointManager::DoesExist(EntityId checkpointId)
{
	if(checkpointId >= MAX_CHECKPOINTS)
		return false;

	return (m_pCheckpoints[checkpointId] != NULL);
}

CCheckpoint * CCheckpointManager::Get(EntityId checkpointId)
{
	if(checkpointId >= MAX_CHECKPOINTS)
		return NULL;

	return m_pCheckpoints[checkpointId];
}

EntityId CCheckpointManager::GetCheckpointCount()
{
	EntityId checkpointCount = 0;

	for(EntityId i = 0; i < MAX_CHECKPOINTS; i++)
	{
		if(m_pCheckpoints[i])
			checkpointCount++;
	}

	return checkpointCount;
}
