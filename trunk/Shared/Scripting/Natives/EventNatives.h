//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: EventNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CEventNatives
{
private:
	static SQInteger Add(SQVM * pVM);
	static SQInteger Call(SQVM * pVM);
	static SQInteger Remove(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
