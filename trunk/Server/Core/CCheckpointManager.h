//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpointManager.h
// Project: Server.Core
// Author(s): MaVe
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "Interfaces/InterfaceCommon.h"
#include "CCheckpoint.h"

class CCheckpointManager : CCheckpointManagerInterface
{
private:
	CCheckpoint * m_pCheckpoints[MAX_CHECKPOINTS];

public:
	CCheckpointManager();
	~CCheckpointManager();

	EntityId      Add(WORD wType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius);
	bool          Delete(EntityId checkpointId);
	void          HandleClientJoin(EntityId playerId);
	bool          DoesExist(EntityId checkpointId);
	CCheckpoint * Get(EntityId checkpointId);
	EntityId      GetCheckpointCount();
};
