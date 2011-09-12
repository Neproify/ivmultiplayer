//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CAreaNatives
{
private:
	static SQInteger Points2D(SQVM * pVM);
	static SQInteger Points3D(SQVM * pVM);
	static SQInteger PointInCircle(SQVM * pVM);
	static SQInteger PointInTube(SQVM * pVM);
	static SQInteger PointInBall(SQVM * pVM);
	static SQInteger PointInArea(SQVM * pVM);
	static SQInteger PointInCuboid(SQVM * pVM);
	static SQInteger PointInPolygon(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
