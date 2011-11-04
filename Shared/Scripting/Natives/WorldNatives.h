//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CWorldNatives
{
private:
	static SQInteger SetTime(SQVM * pVM);
	static SQInteger GetTime(SQVM * pVM);
	static SQInteger SetMinuteDuration(SQVM * pVM);
	static SQInteger GetMinuteDuration(SQVM * pVM);
	static SQInteger SetDayOfWeek(SQVM * pVM);
	static SQInteger GetDayOfWeek(SQVM * pVM);
	static SQInteger SetTrafficLightsState(SQVM * pVM);
	static SQInteger GetTrafficLightsState(SQVM * pVM);
	static SQInteger SetTrafficLightsLocked(SQVM * pVM);
	static SQInteger AreTrafficLightsLocked(SQVM * pVM);
	static SQInteger SetTrafficLightsPhaseDuration(SQVM * pVM);
	static SQInteger GetTrafficLightsPhaseDuration(SQVM * pVM);
	static SQInteger ResetTrafficLightsPhaseDuration(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
