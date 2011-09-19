//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ServerNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CServerNatives
{
private:
	static SQInteger Log(SQVM * pVM);
	static SQInteger LogF(SQVM * pVM);
	static SQInteger AddRule(SQVM * pVM);
	static SQInteger RemoveRule(SQVM * pVM);
	static SQInteger SetRule(SQVM * pVM);
	static SQInteger GetRule(SQVM * pVM);
	static SQInteger Shutdown(SQVM * pVM);
	static SQInteger SetPassword(SQVM * pVM);
	static SQInteger GetPassword(SQVM * pVM);
	static SQInteger GetPlayerCount(SQVM * pVM);
	static SQInteger GetPlayers(SQVM * pVM);
	static SQInteger GetPlayerSlots(SQVM * pVM);
	static SQInteger GetTickCount(SQVM * pVM);
	static SQInteger SetHostName(SQVM * pVM);
	static SQInteger GetHostName(SQVM * pVM);
	static SQInteger TogglePayAndSpray(SQVM * pVM);
	static SQInteger ToggleAutoAim(SQVM * pVM);
	static SQInteger SendInput(SQVM * pVM);
	static SQInteger GetConfig(SQVM * pVM);
	static SQInteger GetWeaponName(SQVM * pVM);
	static SQInteger GetVehicleName(SQVM * pVM);
	static SQInteger ToggleFrequentEvents(SQVM * pVM);
	static SQInteger AreFrequentEventsEnabled(SQVM * pVM);
	static SQInteger SetWeather(SQVM * pVM);
	static SQInteger GetWeather(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
