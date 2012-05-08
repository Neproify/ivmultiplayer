//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"
namespace Modules
{
	class ICCheckpointModuleNatives
	{
	public:
		virtual EntityId Create(int iType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius) = 0;
		virtual bool Delete(EntityId checkpointId) = 0;
		virtual bool ShowForPlayer(EntityId checkpointId, EntityId playerId) = 0;
		virtual bool HideForPlayer(EntityId checkpointId, EntityId playerId) = 0;
		virtual bool ShowForAll(EntityId checkpointId) = 0;
		virtual bool HideForAll(EntityId checkpointId) = 0;
		virtual bool SetType(EntityId checkpointId, int iType) = 0;
		virtual int GetType(EntityId checkpointId) = 0;
		virtual bool SetPosition(EntityId checkpointId, CVector3 vecPosition) = 0;
		virtual CVector3 GetPosition(EntityId checkpointId) = 0;
		virtual bool SetTargetPosition(EntityId checkpointId, CVector3 vecTargetPosition) = 0;
		virtual CVector3 GetTargetPosition(EntityId checkpointId) = 0;
		virtual bool SetRadius(EntityId checkpointId, float fRadius) = 0;
		virtual float GetRadius(EntityId checkpointId) = 0;
		virtual bool SetDimension(EntityId checkpointId, int dimensionId) = 0;
		virtual int GetDimension(EntityId checkpointId) = 0;
	};
}