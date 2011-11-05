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
	static SQInteger SetHeading(SQVM * pVM);
	static SQInteger WalkToCoordinates(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
