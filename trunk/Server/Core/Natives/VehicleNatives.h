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

#include "../Natives.h"

class CVehicleNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetRotation(SQVM * pVM);
	static SQInteger GetRotation(SQVM * pVM);
	static SQInteger IsValid(SQVM * pVM);
	static SQInteger SetColor(SQVM * pVM);
	static SQInteger GetColor(SQVM * pVM);
	static SQInteger GetModel(SQVM * pVM);
	static SQInteger SetHealth(SQVM * pVM);
	static SQInteger GetHealth(SQVM * pVM);
	static SQInteger SetEngineHealth(SQVM * pVM);
	static SQInteger GetEngineHealth(SQVM * pVM);
	static SQInteger SetVelocity(SQVM * pVM);
	static SQInteger GetVelocity(SQVM * pVM);
	static SQInteger SetAngularVelocity(SQVM * pVM);
	static SQInteger GetAngularVelocity(SQVM * pVM);
	static SQInteger Respawn(SQVM * pVM);
	static SQInteger IsOccupied(SQVM * pVM);
	static SQInteger GetOccupants(SQVM * pVM);
	static SQInteger SetDirtLevel(SQVM * pVM);
	static SQInteger GetDirtLevel(SQVM * pVM);
	static SQInteger SetSirenState(SQVM * pVM);
	static SQInteger SoundHorn(SQVM * pVM);
	static SQInteger SetLocked(SQVM * pVM);
	static SQInteger GetLocked(SQVM * pVM);
	static SQInteger SetIndicators(SQVM * pVM);
	static SQInteger GetIndicators(SQVM * pVM);
	static SQInteger SetComponent(SQVM * pVM);
	static SQInteger ResetComponents(SQVM * pVM);
	static SQInteger GetComponents(SQVM * pVM);
	static SQInteger SetVariation(SQVM * pVM);
	static SQInteger GetVariation(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
