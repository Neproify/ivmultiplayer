//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"
namespace Modules
{
	class CCheckpointModuleNatives
	{
	public:
		EntityId Create(int iType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius);
		bool Delete(EntityId checkpointId);
		bool ShowForPlayer(EntityId checkpointId, EntityId playerId);
		bool HideForPlayer(EntityId checkpointId, EntityId playerId);
		bool ShowForAll(EntityId checkpointId);
		bool HideForAll(EntityId checkpointId);
		bool SetType(EntityId checkpointId, int iType);
		int GetType(EntityId checkpointId);
		bool SetPosition(EntityId checkpointId, CVector3 vecPosition);
		CVector3 GetPosition(EntityId checkpointId);
		bool SetTargetPosition(EntityId checkpointId, CVector3 vecTargetPosition);
		CVector3 GetTargetPosition(EntityId checkpointId);
		bool SetRadius(EntityId checkpointId, float fRadius);
		float GetRadius(EntityId checkpointId);
		bool SetDimension(EntityId checkpointId, int dimensionId);
		int GetDimension(EntityId checkpointId);
	};
}