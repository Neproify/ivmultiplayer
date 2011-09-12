//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModelManager.h
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"

class CModelManager
{
public:
	// TODO: Merge player and vehicle model ids
	DWORD VehicleIdToModelHash(int iModelId);
	int ModelHashToVehicleId(DWORD dwModelHash);
};
