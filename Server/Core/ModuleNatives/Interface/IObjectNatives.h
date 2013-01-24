//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICObjectModuleNatives
	{
	public:
		virtual EntityId Create(int modelhash, CVector3 vecPosition, CVector3 vecRotation) = 0;
		virtual bool Delete(EntityId objectId) = 0;
		virtual int GetModel(EntityId objectId) = 0;
		virtual bool SetCoordinates(EntityId objectId, CVector3 vecPosition) = 0;
		virtual CVector3 GetCoordinates(EntityId objectId) = 0;
		virtual bool SetRotation(EntityId objectId, CVector3 vecRotation) = 0;
		virtual CVector3 GetRotation(EntityId objectId) = 0;
		virtual void CreateExplosion(CVector3 vecPos, float fDensity) = 0;
		virtual EntityId CreateFire(CVector3 vecPos, float fDensity) = 0;
		virtual void DeleteFire(EntityId fireId) = 0;
		virtual bool AttachPed(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot) = 0;
		virtual bool AttachVehicle(EntityId objectId, EntityId vehicleId, CVector3 vecPos, CVector3 vecRot) = 0;		
	
		virtual bool Move(EntityId objectId, CVector3 vecMoveTarget, CVector3 vecMoveRot, int iTime) = 0;
		virtual bool Rotate(EntityId objectId, CVector3 vecRotation, int iTime) = 0;
		virtual bool AttachToPlayer(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot, int iBone) = 0;
		virtual bool SetDimension(EntityId objectId, unsigned char ucDimension) = 0;
		virtual unsigned char GetDimension(EntityId objectId) = 0;
	};
}