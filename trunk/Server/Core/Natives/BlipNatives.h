//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: BlipNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CBlipNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetColor(SQVM * pVM);
	static SQInteger GetColor(SQVM * pVM);
	static SQInteger SetSize(SQVM * pVM);
	static SQInteger GetSize(SQVM * pVM);
	static SQInteger ToggleShortRange(SQVM * pVM);
	static SQInteger ToggleRoute(SQVM * pVM);
	static SQInteger SetName(SQVM * pVM);
	static SQInteger GetName(SQVM * pVM);
	static SQInteger SwitchIcon(SQVM * pVM);
	static SQInteger SwitchIconPlayer(SQVM * pVM);
	static SQInteger CreatePlayerBlip(SQVM * pVM);
	static SQInteger DeletePlayerBlip(SQVM * pVM);
	static SQInteger TogglePlayerShortRange(SQVM * pVM);
	static SQInteger TogglePlayerBlipDisplay(SQVM * pVM);
	static SQInteger TogglePlayerShortRangeForPlayer(SQVM * pVM);
	static SQInteger TogglePlayerBlipDisplayForPlayer(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
