//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CPickupNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger GetModel(SQVM * pVM);
	static SQInteger GetType(SQVM * pVM);
	static SQInteger SetValue(SQVM * pVM);
	static SQInteger GetValue(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetRotation(SQVM * pVM);
	static SQInteger GetRotation(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
