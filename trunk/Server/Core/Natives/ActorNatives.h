//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CActorNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger IsValid(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetHeading(SQVM * pVM);
	static SQInteger GetHeading(SQVM * pVM);
	static SQInteger GetModel(SQVM * pVM);
	static SQInteger GetCount(SQVM * pVM);
	static SQInteger WalkToCoordinates(SQVM * pVM);
	static SQInteger SetName(SQVM * pVM);
	static SQInteger GetName(SQVM * pVM);
	static SQInteger ToggleNametag(SQVM * pVM);
	static SQInteger ToggleBlip(SQVM * pVM);
	static SQInteger SetColor(SQVM * pVM);
	static SQInteger GetColor(SQVM * pVM);
	static SQInteger ToggleFrozen(SQVM * pVM);
	static SQInteger ToggleHelmet(SQVM * pVM);
	static SQInteger WarpIntoVehicle(SQVM * pVM);
	static SQInteger RemoveFromVehicle(SQVM * pVM);
	static SQInteger DeleteAll(SQVM * pVM);
	static SQInteger ForceAnim(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
