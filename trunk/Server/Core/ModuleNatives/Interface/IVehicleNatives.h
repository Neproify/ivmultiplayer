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

#include "IModuleNatives.h"

namespace Modules
{
	class ICVehicleModuleNatives
	{
	public:
		virtual int Create(int iModelId, CVector3 vecPosition, CVector3 vecRotation, int color1, int color2, int color3, int color4) = 0;
		virtual bool Delete(EntityId vehicleid) = 0;
		virtual bool SetCoordinates(EntityId vehicleid, CVector3 vecPosition) = 0;
		virtual CVector3 GetCoordinates(EntityId vehicleid) = 0;
		virtual bool SetRotation(EntityId vehicleId, CVector3 vecRotation) = 0;
		virtual bool SetDirtLevel(EntityId vehicleid, float fLevel) = 0;
		virtual float GetDirtLevel(EntityId vehicleId) = 0;
		virtual bool SetSirenState(EntityId vehicleId, bool sqbState) = 0;
		virtual bool GetSirenState(EntityId vehicleId) = 0;
		virtual bool SoundHorn(EntityId vehicleId, int iDuration) = 0;
		virtual CVector3 GetRotation(EntityId vehicleId) = 0;
		virtual bool IsValid(EntityId vehicleid) = 0;
		virtual bool SetColor(EntityId vehicleid, int * colors) = 0;
		virtual BYTE * GetColor(EntityId vehicleid) = 0;
		virtual int GetModel(EntityId vehicleid) = 0;
		virtual bool SetHealth(EntityId vehicleid, int health) = 0;
		virtual int GetHealth(EntityId vehicleid) = 0;
		virtual bool SetVelocity(EntityId vehicleid, CVector3 vecMoveSpeed) = 0;
		virtual CVector3 GetVelocity(EntityId vehicleid) = 0;
		virtual bool SetAngularVelocity(EntityId vehicleid, CVector3 vecTurnSpeed) = 0;
		virtual CVector3 GetAngularVelocity(EntityId vehicleid) = 0;
		virtual bool Respawn(EntityId vehicleId) = 0;
		virtual bool IsOccupied(EntityId vehicleId) = 0;
		virtual bool SetLocked(EntityId vehicleId, int iLocked) = 0;
		virtual int GetLocked(EntityId vehicleId) = 0;
		virtual bool SetIndicators(EntityId vehicleId, bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight) = 0;
		virtual int * GetIndicators(EntityId vehicleId) = 0;
		virtual bool SetComponent(EntityId vehicleId, int iSlot, bool bOn) = 0;
		virtual bool ResetComponents(EntityId vehicleId) = 0;
		virtual int * GetComponents(EntityId vehicleId) = 0;
		virtual bool SetVariation(EntityId vehicleId, int iVariation) = 0;
		virtual int GetVariation(EntityId vehicleId) = 0;
		virtual bool SetEngineStatus(EntityId vehicleId, bool bEngineStatusx) = 0;
		virtual bool GetEngineStatus(EntityId vehicleId) = 0;
		virtual bool SwitchTaxiLights(EntityId vehicleId, bool check) = 0;
		virtual bool ControlCarDoors(EntityId vehicleId, int door, bool door2, float door3) = 0;
		virtual bool SetLights(EntityId vehicleId, bool bLights) = 0;
		virtual bool GetLights(EntityId vehicleId) = 0;
		virtual bool GetTaxiLights(EntityId vehicleId) = 0;
		virtual bool RepairWheels(EntityId vehicleId) = 0;
		virtual bool RepairWindows(EntityId vehicleId) = 0;
	};
}