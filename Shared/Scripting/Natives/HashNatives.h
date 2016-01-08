//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: HashNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CHashNatives
{
private:
	static SQInteger md5(SQVM * pVM);
	static SQInteger md5File(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
