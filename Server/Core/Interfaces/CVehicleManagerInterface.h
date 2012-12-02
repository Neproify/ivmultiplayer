//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CVehicleInterface.h"

class CPlayerInterface;

class CVehicleManagerInterface
{
public:
	virtual EntityId Add(int iModelId, CVector3 vecSpawnPosition, CVector3 vecSpawnRotation, BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4, int respawn_delay = 1) = 0;
	virtual void Remove(EntityId vehicleId) = 0;
	virtual bool DoesExist(EntityId vehicleId) = 0;
	virtual int GetVehicleCount() = 0;

	virtual CVehicleInterface * GetAt(EntityId vehicleId) = 0;
};
