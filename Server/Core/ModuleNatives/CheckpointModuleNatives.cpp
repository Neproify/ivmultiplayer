//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.cpp
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "../CCheckpointManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CCheckpointManager * g_pCheckpointManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Checkpoint functions
namespace Modules
{
	// createCheckpoint(type, x, y, z, targetx, targety, targetz, radius)
	EntityId CCheckpointModuleNatives::Create(int iType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius)
	{
		return g_pCheckpointManager->Add(iType, vecPosition, vecTargetPosition, fRadius);
	}

	// deleteCheckpoint(checkpointid)
	bool CCheckpointModuleNatives::Delete(EntityId checkpointId)
	{
		if(g_pCheckpointManager->DoesExist(checkpointId))
		{
			return g_pCheckpointManager->Delete(checkpointId);
		}
		return false;
	}

	// showCheckpointForPlayer(checkpointid, playerid)
	bool CCheckpointModuleNatives::ShowForPlayer(EntityId checkpointId, EntityId playerId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->ShowForPlayer(playerId);
			return true;
		}

		return false;
	}

	// hideCheckpointForPlayer(checkpointid, playerid)
	bool CCheckpointModuleNatives::HideForPlayer(EntityId checkpointId, EntityId playerId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->HideForPlayer(playerId);
			return true;
		}

		return false;
	}

	// showCheckpointForAll(checkpointid)
	bool CCheckpointModuleNatives::ShowForAll(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->ShowForWorld();
			return true;
		}

		return false;
	}

	// hideCheckpointForAll(checkpointid)
	bool CCheckpointModuleNatives::HideForAll(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->HideForWorld();
			return true;
		}

		return false;
	}

	// setCheckpointType(checkpointid, type)
	bool CCheckpointModuleNatives::SetType(EntityId checkpointId, int iType)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->SetType(iType);
			return true;
		}

		return false;
	}

	// getCheckpointType(checkpointid)
	int CCheckpointModuleNatives::GetType(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			return pCheckpoint->GetType();
		}

		return false;
	}

	// setCheckpointPosition(checkpointid, position)
	bool CCheckpointModuleNatives::SetPosition(EntityId checkpointId, CVector3 vecPosition)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->SetPosition(vecPosition);
			return true;
		}

		return false;
	}

	// getCheckpointPosition(checkpointid)
	CVector3 CCheckpointModuleNatives::GetPosition(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			CVector3 vecPosition;
			pCheckpoint->GetPosition(vecPosition);
			return vecPosition;
		}

		return CVector3();
	}

	// setCheckpointTargetPosition(checkpointid, targetposition)
	bool CCheckpointModuleNatives::SetTargetPosition(EntityId checkpointId, CVector3 vecTargetPosition)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->SetTargetPosition(vecTargetPosition);
			return true;
		}

		return false;
	}

	// getCheckpointTargetPosition(checkpointid)
	CVector3 CCheckpointModuleNatives::GetTargetPosition(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			CVector3 vecTargetPosition;
			pCheckpoint->GetTargetPosition(vecTargetPosition);
			return vecTargetPosition;
		}

		return CVector3();
	}

	// setCheckpointRadius(checkpointid, radius)
	bool CCheckpointModuleNatives::SetRadius(EntityId checkpointId, float fRadius)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			pCheckpoint->SetRadius(fRadius);
			return true;
		}

		return false;
	}

	// getCheckpointRadius(checkpointid)
	float CCheckpointModuleNatives::GetRadius(EntityId checkpointId)
	{
		CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

		if(pCheckpoint)
		{
			return pCheckpoint->GetRadius();
		}

		return -0.1f;
	}
	
	bool CCheckpointModuleNatives::SetDimension(EntityId checkpointId, unsigned char ucDimension)
	{
		if(g_pCheckpointManager->DoesExist(checkpointId))
		{
			g_pCheckpointManager->Get(checkpointId)->SetDimension(ucDimension);
			return true;
		}
		return false;
	}

	int CCheckpointModuleNatives::GetDimension(EntityId checkpointId)
	{
		if(g_pCheckpointManager->DoesExist(checkpointId))
		{
			return g_pCheckpointManager->Get(checkpointId)->GetDimension();
		}
		return false;
	}
}
