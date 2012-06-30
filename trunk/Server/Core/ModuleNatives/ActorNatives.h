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
		CVector3 GetCoordinates(EntityId actorId);

		float GetHeading(EntityId actorId);
		int GetModel(EntityId actorId);
		int GetCount();
		bool SetName(EntityId actorId, const char * szName);
		const char * GetName(EntityId actorId);
		bool ToggleNametag(EntityId actorId, bool bShow);
		bool ToggleBlip(EntityId actorId, bool bShow);
		bool SetColor(EntityId actorId, int color);
		int GetColor(EntityId actorId);
		bool ToggleFrozen(EntityId actorId, bool frozen);
		bool ToggleHelmet(EntityId actorId, bool helmet);
		bool WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid);
		bool RemoveFromVehicle(EntityId actorId);
		void DeleteAll();
		bool ForceAnim(EntityId actorId, const char * szGroup, const char * szAnim);
	};
}