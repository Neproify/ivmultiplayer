//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SystemNatives.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CSystemNatives
{
private:
	static SQInteger Clock(SQVM * pVM);
	static SQInteger Time(SQVM * pVM);
	static SQInteger Date(SQVM * pVM);
	static SQInteger MkDir(SQVM * pVM);
	static SQInteger RmDir(SQVM * pVM);
	static SQInteger Remove(SQVM * pVM);
	static SQInteger Rename(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
