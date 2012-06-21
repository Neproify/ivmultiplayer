//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.h
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
	class CVehicleModuleNatives : public ICVehicleModuleNatives
	{
	public:
		int Create(int iModelId, CVector3 vecPosition, CVector3 vecRotation, int color1, int color2, int color3, int color4);
		bool Delete(EntityId vehicleid);
		bool SetCoordinates(EntityId vehicleid, CVector3 vecPosition);
		CVector3 GetCoordinates(EntityId vehicleid);
		bool SetRotation(EntityId vehicleId, CVector3 vecRotation);
		bool SetDirtLevel(EntityId vehicleid, float fLevel);
		float GetDirtLevel(EntityId vehicleId);
		bool SetSirenState(EntityId vehicleId, bool sqbState);
		bool GetSirenState(EntityId vehicleId);
		bool SoundHorn(EntityId vehicleId, int iDuration);
		CVector3 GetRotation(EntityId vehicleId);
		bool IsValid(EntityId vehicleid);
		bool SetColor(EntityId vehicleid, int * colors);
		BYTE * GetColor(EntityId vehicleid);
		int GetModel(EntityId vehicleid);
		bool SetHealth(EntityId vehicleid, int health);
		int GetHealth(EntityId vehicleid);
		bool SetVelocity(EntityId vehicleid, CVector3 vecMoveSpeed);
		CVector3 GetVelocity(EntityId vehicleid);
		bool SetAngularVelocity(EntityId vehicleid, CVector3 vecTurnSpeed);
		CVector3 GetAngularVelocity(EntityId vehicleid);
		bool Respawn(EntityId vehicleId);
		bool IsOccupied(EntityId vehicleId);
		bool SetLocked(EntityId vehicleId, int iLocked);
		int GetLocked(EntityId vehicleId);
		bool SetIndicators(EntityId vehicleId, bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight);
		int * GetIndicators(EntityId vehicleId);
		bool SetComponent(EntityId vehicleId, int iSlot, bool bOn);
		bool ResetComponents(EntityId vehicleId);
		int * GetComponents(EntityId vehicleId);
		bool SetVariation(EntityId vehicleId, int iVariation);
		int GetVariation(EntityId vehicleId);
		bool SetEngineStatus(EntityId vehicleId, bool bEngineStatusx);
		bool GetEngineStatus(EntityId vehicleId);

		bool SwitchTaxiLights(EntityId vehicleId, bool check);
		bool ControlCarDoors(EntityId vehicleId, int door, bool door2, float door3);
		bool SetLights(EntityId vehicleId, bool bLights);
		bool GetLights(EntityId vehicleId);
		bool GetTaxiLights(EntityId vehicleId);
		bool RepairWheels(EntityId vehicleId);
		bool RepairWindows(EntityId vehicleId);
	};
}