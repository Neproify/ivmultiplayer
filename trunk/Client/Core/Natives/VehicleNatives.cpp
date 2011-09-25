//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "Squirrel/sqstate.h"
#include "Squirrel/sqvm.h"
#include "Squirrel/sqstring.h"
#include "../CVehicleManager.h"
#include "../CModelManager.h"

extern CVehicleManager * g_pVehicleManager;
extern CModelManager * g_pModelManager;

// Vehicle functions

void CVehicleNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isVehicleValid", IsValid, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getVehiclePosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleRotation", GetRotation, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleEngineHealth", GetEngineHealth, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleAngularVelocity", GetAngularVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleLocked", GetLocked, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleDirtLevel", GetDirtLevel, 1, "i");
}

// getVehicleCoordinates(vehicleid)
SQInteger CVehicleNatives::GetCoordinates(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			CVector3 vecPosition;
			pVehicle->GetPosition(vecPosition);
			CSquirrelArguments args;
			args.push(vecPosition.fX);
			args.push(vecPosition.fY);
			args.push(vecPosition.fZ);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleRotation(vehicleid)
SQInteger CVehicleNatives::GetRotation(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	if(g_pVehicleManager->Exists(vehicleId))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

		if(pVehicle)
		{
			CVector3 vecRotation;
			pVehicle->GetRotation(vecRotation);
			CSquirrelArguments args;
			args.push(vecRotation.fX);
			args.push(vecRotation.fY);
			args.push(vecRotation.fZ);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isVehicleValid(vehicleid)
SQInteger CVehicleNatives::IsValid(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	sq_pushbool(pVM, g_pVehicleManager->Exists(vehicleid));
	return 1;
}

// getVehicleColor(vehicleid)
SQInteger CVehicleNatives::GetColor(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			BYTE byteColors[4];
			pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
			CSquirrelArguments args;
			args.push((int)byteColors[0]);
			args.push((int)byteColors[1]);
			args.push((int)byteColors[2]);
			args.push((int)byteColors[3]);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleModel(vehicleid)
SQInteger CVehicleNatives::GetModel(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			sq_pushinteger(pVM, g_pModelManager->ModelHashToVehicleId(pVehicle->GetModelInfo()->GetHash()));
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleHealth(vehicleid)
SQInteger CVehicleNatives::GetHealth(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			sq_pushinteger(pVM, pVehicle->GetHealth());
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineHealth(vehicleid)
SQInteger CVehicleNatives::GetEngineHealth(SQVM * pVM)
{
	int iVehicleId;
	sq_getinteger(pVM, -1, &iVehicleId);

	if(g_pVehicleManager->Exists(iVehicleId))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(iVehicleId);

		if(pVehicle)
		{
			CLogFile::Printf("Function getVehicleEngineHealth is depreciated: please use getVehicleHealth.");
			sq_pushfloat(pVM, (float)pVehicle->GetHealth());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleVelocity(vehicleid)
SQInteger CVehicleNatives::GetVelocity(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			CVector3 vecMoveSpeed;
			pVehicle->GetMoveSpeed(vecMoveSpeed);
			CSquirrelArguments args;
			args.push(vecMoveSpeed.fX);
			args.push(vecMoveSpeed.fY);
			args.push(vecMoveSpeed.fZ);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleAngularVelocity(vehicleid)
SQInteger CVehicleNatives::GetAngularVelocity(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);

		if(pVehicle)
		{
			CVector3 vecTurnSpeed;
			pVehicle->GetTurnSpeed(vecTurnSpeed);
			CSquirrelArguments args;
			args.push(vecTurnSpeed.fX);
			args.push(vecTurnSpeed.fY);
			args.push(vecTurnSpeed.fZ);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleLocked(vehicleid)
SQInteger CVehicleNatives::GetLocked(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);
		if(pVehicle)
		{
			sq_pushinteger(pVM, pVehicle->GetDoorLockState());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetDirtLevel(SQVM * pVM)
{
	int vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->Exists(vehicleid))
	{
		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleid);
		if(pVehicle)
		{
			sq_pushfloat(pVM, (float)pVehicle->GetDirtLevel());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}
