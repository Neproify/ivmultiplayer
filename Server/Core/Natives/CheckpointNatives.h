//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CCheckpointNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger ShowForPlayer(SQVM * pVM);
	static SQInteger HideForPlayer(SQVM * pVM);
	static SQInteger ShowForAll(SQVM * pVM);
	static SQInteger HideForAll(SQVM * pVM);
	static SQInteger SetType(SQVM * pVM);
	static SQInteger GetType(SQVM * pVM);
	static SQInteger SetPosition(SQVM * pVM);
	static SQInteger GetPosition(SQVM * pVM);
	static SQInteger SetTargetPosition(SQVM * pVM);
	static SQInteger GetTargetPosition(SQVM * pVM);
	static SQInteger SetRadius(SQVM * pVM);
	static SQInteger GetRadius(SQVM * pVM);
	static SQInteger SetDimension(SQVM * pVM);
	static SQInteger GetDimension(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
