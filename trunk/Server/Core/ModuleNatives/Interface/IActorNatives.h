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
		virtual CVector3 GetCoordinates(EntityId actorId) = 0;
		virtual float GetHeading(EntityId actorId) = 0;
		virtual int GetModel(EntityId actorId) = 0;
		virtual int GetCount() = 0;
		virtual bool SetName(EntityId actorId, const char * szName) = 0;
		virtual const char * GetName(EntityId actorId) = 0;
		virtual bool ToggleNametag(EntityId actorId, bool bShow) = 0;
		virtual bool ToggleBlip(EntityId actorId, bool bShow) = 0;
		virtual bool SetColor(EntityId actorId, int color) = 0;
		virtual int GetColor(EntityId actorId) = 0;
		virtual bool ToggleFrozen(EntityId actorId, bool frozen) = 0;
		virtual bool ToggleHelmet(EntityId actorId, bool helmet) = 0;
		virtual bool WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid) = 0;
		virtual bool RemoveFromVehicle(EntityId actorId) = 0;
		virtual bool DriveToCoordinates(EntityId actorId, CVector3 vecDriveTo, CVector3 vecDriveRot, bool bStop) = 0;
		virtual void DeleteAll() = 0;
		virtual bool ForceAnim(EntityId actorId, const char * szGroup, const char * szAnim) = 0;
	};
}