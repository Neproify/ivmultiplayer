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

#include "ModuleNatives.h"

namespace Modules
{
	class CObjectModuleNatives : public ICObjectModuleNatives
	{
	public:
		EntityId Create(int modelhash, CVector3 vecPosition, CVector3 vecRotation);
		bool Delete(EntityId objectId);
		int GetModel(EntityId objectId);
		bool SetCoordinates(EntityId objectId, CVector3 vecPosition);
		CVector3 GetCoordinates(EntityId objectId);
		bool SetRotation(EntityId objectId, CVector3 vecRotation);
		CVector3 GetRotation(EntityId objectId);

		bool Move(EntityId objectId, CVector3 vecMoveTarget, CVector3 vecMoveRot, float fSpeed);
		bool Rotate(EntityId objectId, CVector3 vecRotation, float fSpeed);
		bool AttachToPlayer(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot, int iBone);
		bool SetDimension(EntityId objectId, unsigned char ucDimension);
		unsigned char GetDimension(EntityId objectId);

		void CreateExplosion(CVector3 vecPos, float fDensity);
		EntityId CreateFire(CVector3 vecPos, float fDensity);
		void DeleteFire(EntityId fireId);
		bool AttachPed(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot);
		bool AttachVehicle(EntityId objectId, EntityId vehicleId, CVector3 vecPos, CVector3 vecRot);
	};
}