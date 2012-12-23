//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CObjectNatives
{
private:
	static SQInteger Create(SQVM * pVM);
	static SQInteger Delete(SQVM * pVM);
	static SQInteger GetModel(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetRotation(SQVM * pVM);
	static SQInteger SetRotation(SQVM * pVM);
	static SQInteger CreateFire(SQVM * pVM);
	static SQInteger DeleteFire(SQVM * pVM);
	static SQInteger CreateExplosion(SQVM * pVM);
	static SQInteger AttachVehicle(SQVM * pVM);
	static SQInteger AttachPed(SQVM * pVM);
	static SQInteger AttachPedBone(SQVM * pVM);
	static SQInteger DetachObject(SQVM * pVM);
	static SQInteger MoveObject(SQVM * pVM);
	static SQInteger RotateObject(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
