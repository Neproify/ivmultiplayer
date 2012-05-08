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

#include "IModuleNatives.h"

namespace Modules
{
	class ICActorModuleNatives
	{
	public:
		virtual EntityId Create(int iModelId, CVector3 vecPosition, float fHeading) = 0;
		virtual bool Delete(EntityId actorId) = 0;
		virtual bool IsValid(EntityId actorId) = 0;
		virtual bool SetCoordinates(EntityId actorId, CVector3 vecPosition) = 0;
		virtual bool SetHeading(EntityId actorId, float fHeading) = 0;
		virtual bool WalkToCoordinates(EntityId playerId, EntityId actorId, CVector3 vecPosition, int iType) = 0;
	};
}