//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: 3DLabelNatives.h
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class _3DLabelNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger SetPosition(SQVM * pVM);
	static SQInteger GetPosition(SQVM * pVM);
	static SQInteger SetColor(SQVM * pVM);
	static SQInteger GetColor(SQVM * pVM);
	static SQInteger SetDimension(SQVM * pVM);
	static SQInteger GetDimension(SQVM * pVM);
	static SQInteger SetText(SQVM * pVM);
	static SQInteger GetText(SQVM * pVM);
	static SQInteger SetVisible(SQVM * pVM);
	static SQInteger GetVisible(SQVM * pVM);
	static SQInteger SetStreamingDistance(SQVM * pVM);
	static SQInteger GetStreamingDistance(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
