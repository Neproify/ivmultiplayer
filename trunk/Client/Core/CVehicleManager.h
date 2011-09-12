//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleManager.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"
#include "CNetworkVehicle.h"
#include "CLocalPlayer.h"
#include "CNetworkEntityManager.h"

class CVehicleManager : public CNetworkEntityManager<CNetworkVehicle, MAX_VEHICLES>
{
public:
	void              Pulse();
};
