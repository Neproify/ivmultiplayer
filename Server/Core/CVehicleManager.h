//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleManager.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "./Main.h"
#include "Interfaces/InterfaceCommon.h"
#include "CVehicle.h"

class CVehicleManager : public CVehicleManagerInterface
{
private:
	bool m_bActive[MAX_VEHICLES];
	CVehicle * m_pVehicles[MAX_VEHICLES];

public:
	CVehicleManager();
	~CVehicleManager();

	EntityId Add(int iModelId, CVector3 vecSpawnPosition, CVector3 vecSpawnRotation, BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4, int respawn_delay = -1);
	void Remove(EntityId vehicleId);
	void HandleClientJoin(EntityId playerId);
	bool DoesExist(EntityId vehicleId);
	int GetVehicleCount();
	void Process();
	CVehicle * GetAt(EntityId vehicleId);
};