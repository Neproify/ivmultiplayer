//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CActorModuleNatives : public ICActorModuleNatives
	{
	public:
		EntityId Create(int iModelId, CVector3 vecPosition, float fHeading);
		bool Delete(EntityId actorId);
		bool IsValid(EntityId actorId);
		bool SetCoordinates(EntityId actorId, CVector3 vecPosition);
		bool SetHeading(EntityId actorId, float fHeading);
		bool WalkToCoordinates(EntityId playerId, EntityId actorId, CVector3 vecPosition, int iType);
	};
}