//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CNetworkManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CVehicleManager.h"
#include "../CPlayer.h"

extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;

// Vehicle functions

void CVehicleNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createVehicle", Create, -1, NULL);
	pScriptingManager->RegisterFunction("deleteVehicle", Delete, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setVehiclePosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehiclePosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleRotation", SetRotation, 4, "ifff");
	pScriptingManager->RegisterFunction("setVehicleSirenState", SetSirenState, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleSirenState", GetSirenState, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleDirtLevel", SetDirtLevel, 2, "if");
	pScriptingManager->RegisterFunction("getVehicleDirtLevel", GetDirtLevel, 1, "i");
	pScriptingManager->RegisterFunction("soundVehicleHorn", SoundHorn, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleRotation", GetRotation, 1, "i");
	pScriptingManager->RegisterFunction("isVehicleValid", IsValid, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleColor", SetColor, -1, NULL);
	pScriptingManager->RegisterFunction("getVehicleColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleHealth", SetHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleEngineHealth", SetEngineHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleEngineHealth", GetEngineHealth, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleVelocity", SetVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleAngularVelocity", SetAngularVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleAngularVelocity", GetAngularVelocity, 1, "i");
	pScriptingManager->RegisterFunction("respawnVehicle", Respawn, 1, "i");
	pScriptingManager->RegisterFunction("isVehicleOccupied", IsOccupied, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleOccupants", GetOccupants, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleLocked", SetLocked, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleLocked", GetLocked, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleIndicators", SetIndicators, 5, "ibbbb");
	pScriptingManager->RegisterFunction("getVehicleIndicators", GetIndicators, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleComponent", SetComponent, 3, "iib");
	pScriptingManager->RegisterFunction("getVehicleComponents", GetComponents, 1, "i");
	pScriptingManager->RegisterFunction("resetVehicleComponents", ResetComponents, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleVariation", SetVariation, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleVariation", GetVariation, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleTaxiLights", SwitchTaxiLights, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleTaxiLights", GetTaxiLights, 1, "i");
	pScriptingManager->RegisterFunction("controlCarDoors", ControlCar, 4, "iibf");
	pScriptingManager->RegisterFunction("setVehicleEngineState", SetEngineStatus, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleEngineState", GetEngineStatus, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleLights",SetLights,2,"ib");
	pScriptingManager->RegisterFunction("getVehicleLights",GetLights,1,"i");
	pScriptingManager->RegisterFunction("repairVehicleWindows", RepairWindows, 1, "i");
	pScriptingManager->RegisterFunction("repairVehicleWheels", RepairWheels, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleGpsState", SetGpsState, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleGpsState", GetGpsState, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleAlarm", SetAlarm, 2, "ii");
	pScriptingManager->RegisterFunction("markVehicleAsActorVehicle", MarkVehicle, 2, "ib");
}

// createVehicle(model, x, y, z, rx, ry, rz, color1, color2, color3, color4)
// TODO: Rotation and colors optional
SQInteger CVehicleNatives::Create(SQVM * pVM)
{
	SQInteger iModelId;
	CVector3 vecPosition;
	CVector3 vecRotation;
	SQInteger color1, color2, color3 = 0, color4 = 0;
	sq_getinteger(pVM, 2, &iModelId);

	if(iModelId < 0 || iModelId == 41 || iModelId == 96 || iModelId == 107 || iModelId == 111 || iModelId > 123)
	{
		#ifdef IVMP_DEBUG
			CLogFile::Printf("Invalid vehicle model (%d)", iModelId);
		#endif
		sq_pushinteger(pVM, INVALID_ENTITY_ID);
		return 1;
	}

	sq_getvector3(pVM, 3, &vecPosition); // 3..5
	sq_getvector3(pVM, 6, &vecRotation); // 6..8
	sq_getinteger(pVM, 9, &color1);
	sq_getinteger(pVM, 10, &color2);
	sq_getinteger(pVM, 11, &color3);
	sq_getinteger(pVM, 12, &color4);

	if(iModelId >= 0 && iModelId <= 125)
	{
		sq_pushinteger(pVM, g_pVehicleManager->Add(iModelId, vecPosition, vecRotation, color1, color2, color3, color4));
		return 1;
	}

	sq_pushinteger(pVM, INVALID_ENTITY_ID);
	return 1;
}

// deleteVehicle(vehicleid)
SQInteger CVehicleNatives::Delete(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, 2, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		g_pVehicleManager->Remove(vehicleid);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleCoordinates(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetCoordinates(SQVM * pVM)
{
	SQInteger vehicleid;
	float x, y, z;
	sq_getinteger(pVM, -4, &vehicleid);
	sq_getfloat(pVM, -3, &x);
	sq_getfloat(pVM, -2, &y);
	sq_getfloat(pVM, -1, &z);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);
		
		if(pVehicle)
		{
			pVehicle->SetPosition(CVector3(x, y, z));
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleCoordinates(vehicleid)
SQInteger CVehicleNatives::GetCoordinates(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

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

// setVehicleRotation(vehicleid, rotation)
SQInteger CVehicleNatives::SetRotation(SQVM * pVM)
{
	EntityId vehicleId;
	CVector3 vecRotation;
	sq_getentity(pVM, -4, &vehicleId);
	sq_getvector3(pVM, -3, &vecRotation);

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetRotation(vecRotation);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleDirtLevel(vehicleid, level)
SQInteger CVehicleNatives::SetDirtLevel(SQVM * pVM)
{
	SQInteger vehicleid;

	float fLevel;

	sq_getinteger(pVM, -2, &vehicleid);

	sq_getfloat(pVM, -1, &fLevel);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetDirtLevel(fLevel);

			sq_pushbool(pVM, true);

			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetDirtLevel(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushfloat(pVM, pVehicle->GetDirtLevel());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleSirenState(vehicleid, state)
SQInteger CVehicleNatives::SetSirenState(SQVM * pVM)
{
	EntityId vehicleId;
	SQBool sqbState;

	sq_getentity(pVM, -2, &vehicleId);
	sq_getbool(pVM, -1, &sqbState);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetSirenState(sqbState != 0);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleSirenState(vehicleid)
SQInteger CVehicleNatives::GetSirenState(SQVM * pVM)
{
	EntityId vehicleId;

	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->GetSirenState());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// soundVehicleHorn(vehicleid, duration)
SQInteger CVehicleNatives::SoundHorn(SQVM * pVM)
{
	EntityId vehicleId;
	SQInteger iDuration;

	sq_getentity(pVM, -2, &vehicleId);
	sq_getinteger(pVM, -1, &iDuration);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SoundHorn(iDuration);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleRotation(vehicleid)
SQInteger CVehicleNatives::GetRotation(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

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
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	sq_pushbool(pVM, g_pVehicleManager->DoesExist(vehicleid));
	return 1;
}

// setVehicleColor(vehicleid, color1, color2, color3, color4)
// TODO: Only require two colors
SQInteger CVehicleNatives::SetColor(SQVM * pVM)
{
	SQInteger vehicleid;
	SQInteger colors[4];
	sq_getinteger(pVM, -5, &vehicleid);
	sq_getinteger(pVM, -4, &colors[0]);
	sq_getinteger(pVM, -3, &colors[1]);
	sq_getinteger(pVM, -2, &colors[2]);
	sq_getinteger(pVM, -1, &colors[3]);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetColors(colors[0], colors[1], colors[2], colors[3]);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleColor(vehicleid)
SQInteger CVehicleNatives::GetColor(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

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
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			sq_pushinteger(pVM, pVehicle->GetModel());
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleHealth(vehicleid, health)
SQInteger CVehicleNatives::SetHealth(SQVM * pVM)
{
	SQInteger vehicleid;
	SQInteger health;
	sq_getinteger(pVM, -2, &vehicleid);
	sq_getinteger(pVM, -1, &health);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetHealth(health);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleHealth(vehicleid)
SQInteger CVehicleNatives::GetHealth(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			sq_pushinteger(pVM, pVehicle->GetHealth());
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleEngineHealth(vehicleid, enginehealth)
SQInteger CVehicleNatives::SetEngineHealth(SQVM * pVM)
{
	SQInteger vehicleid;
	SQInteger health;
	sq_getinteger(pVM, -2, &vehicleid);
	sq_getinteger(pVM, -1, &health);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CLogFile::Printf("Function setVehicleEngineHealth is depreciated: please use setVehicleHealth.");
			pVehicle->SetHealth(health);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineHealth(vehicleid)
SQInteger CVehicleNatives::GetEngineHealth(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CLogFile::Printf("Function getVehicleEngineHealth is depreciated: please use getVehicleHealth.");
			sq_pushinteger(pVM, pVehicle->GetHealth());
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleVelocity(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetVelocity(SQVM * pVM)
{
	SQInteger vehicleid;
	CVector3 vecMoveSpeed;
	sq_getinteger(pVM, -4, &vehicleid);
	sq_getvector3(pVM, -3, &vecMoveSpeed);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetMoveSpeed(vecMoveSpeed);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleVelocity(vehicleid)
SQInteger CVehicleNatives::GetVelocity(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

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

// setVehicleAngularVelocity(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetAngularVelocity(SQVM * pVM)
{
	SQInteger vehicleid;
	CVector3 vecTurnSpeed;
	sq_getinteger(pVM, -4, &vehicleid);
	sq_getvector3(pVM, -3, &vecTurnSpeed);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetTurnSpeed(vecTurnSpeed);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleAngularVelocity(vehicleid)
SQInteger CVehicleNatives::GetAngularVelocity(SQVM * pVM)
{
	SQInteger vehicleid;
	sq_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

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

// respawnVehicle(vehicleid)
SQInteger CVehicleNatives::Respawn(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->Respawn();
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isVehicleOccupied(vehicleid)
SQInteger CVehicleNatives::IsOccupied(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->IsOccupied());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleOccupants(vehicleid)
SQInteger CVehicleNatives::GetOccupants(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CSquirrelArguments args;

		for(BYTE i = 0; i < (MAX_VEHICLE_PASSENGERS + 1); i++)
		{
			args.push((int)(i + 1));
			CPlayer * pOccupant = pVehicle->GetOccupant(i);
			args.push(pOccupant ? (int)pOccupant->GetPlayerId() : (int)INVALID_ENTITY_ID);
		}

		sq_pusharg(pVM, CSquirrelArgument(args, false));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleLocked(vehicleid, locked)
SQInteger CVehicleNatives::SetLocked(SQVM * pVM)
{
	EntityId vehicleId;
	int iLocked;

	sq_getentity(pVM, -2, &vehicleId);
	sq_getinteger(pVM, -1, &iLocked);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->SetLocked(iLocked));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleLocked(vehicleid)
SQInteger CVehicleNatives::GetLocked(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetLocked());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleIndicators(vehicleid, frontleft, frontright, backleft, backright)
SQInteger CVehicleNatives::SetIndicators(SQVM * pVM)
{
	EntityId vehicleId;
	bool bFrontLeft, bFrontRight, bBackLeft, bBackRight;
	SQBool sqbOn;

	sq_getentity(pVM, 2, &vehicleId);

	sq_getbool(pVM, 3, &sqbOn);
	bFrontLeft = sqbOn != 0;

	sq_getbool(pVM, 4, &sqbOn);
	bFrontRight = sqbOn != 0;

	sq_getbool(pVM, 5, &sqbOn);
	bBackLeft = sqbOn != 0;

	sq_getbool(pVM, 6, &sqbOn);
	bBackRight = sqbOn != 0;

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetIndicatorState(bFrontLeft, bFrontRight, bBackLeft, bBackRight);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetIndicators(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, 2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_newarray(pVM, 0);
		for(unsigned char uc = 0; uc <= 3; ++ uc)
		{
			sq_pushbool(pVM, pVehicle->GetIndicatorState(uc));
			sq_arrayappend(pVM, -2);
		}
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::SetComponent(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, 2, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		SQInteger iSlot;
		SQBool bOn;
		sq_getinteger(pVM, 3, &iSlot);
		sq_getbool(pVM, 4, &bOn);
		
		if(iSlot >= 0 && iSlot <= 8)
		{
			pVehicle->SetComponentState((unsigned char)iSlot, bOn != 0);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::ResetComponents(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, 2, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		pVehicle->ResetComponents();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetComponents(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, 2, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		sq_newarray(pVM, 0);
		for(unsigned char i = 0; i < 9; ++ i)
		{
			sq_pushbool(pVM, pVehicle->GetComponentState(i));
			sq_arrayappend(pVM, -2);
		}
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::SetVariation(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		SQInteger iVariation;
		sq_getinteger(pVM, -1, &iVariation);
		pVehicle->SetVariation((unsigned char)iVariation);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetVariation(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, 2, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetVariation());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::SetEngineStatus(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		SQBool bEngineStatusx;
		sq_getbool(pVM, -1, &bEngineStatusx);
		pVehicle->SetEngineStatus(bEngineStatusx != 0);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetEngineStatus(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->GetEngineStatus());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::SwitchTaxiLights(SQVM *pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM,-2,&vehicleId);

	SQBool check;
	sq_getbool(pVM, -1, &check);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		bool bToggle = (check != 0);
		pVehicle->TurnTaxiLights(bToggle);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::ControlCar(SQVM *pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM,-4,&vehicleId);

	int iDoor;
	sq_getinteger(pVM,-3,&iDoor);

	SQBool bDoor;
	sq_getbool(pVM,-2,&bDoor);

	float fAngle;
	sq_getfloat(pVM,-1,&fAngle); 

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		bool bToggle = (bDoor != 0);
		pVehicle->SetCarDoorAngle(iDoor,bToggle,fAngle);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::SetLights(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		SQBool bLights;
		sq_getbool(pVM, -1, &bLights);
		bool bToggle = (bLights != 0);
		pVehicle->SetLights(bToggle);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::GetLights(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM,pVehicle->GetLights());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::GetTaxiLights(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM,pVehicle->GetTaxiLights());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::RepairWheels(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWheels();
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::RepairWindows(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWindows();
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

// setVehicleGpsState(vehicleid, state)
SQInteger CVehicleNatives::SetGpsState(SQVM *pVM)
{
	EntityId vehicleId;
	SQBool bState;

	sq_getentity(pVM, -1, &vehicleId);
	sq_getbool(pVM, -2, &bState);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		pVehicle->SetVehicleGPSState((bState==SQTrue?true:false));
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleGpsState(vehicleid)
SQInteger CVehicleNatives::GetGpsState(SQVM *pVM)
{
	EntityId vehicleId;

	sq_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->GetVehicleGPSState());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleAlarm(vehicleid, alarmduration)
SQInteger CVehicleNatives::SetAlarm(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM,-2, &vehicleId);

	SQInteger iDuration;
	sq_getinteger(pVM, -1, &iDuration);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		pVehicle->SetAlarm((int)iDuration);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}


// setVehicleAlarm(vehicleid, alarmduration)
SQInteger CVehicleNatives::MarkVehicle(SQVM * pVM)
{
	EntityId vehicleId;
	sq_getentity(pVM,-2, &vehicleId);

	SQBool bToggle;
	sq_getbool(pVM, -1, &bToggle);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		bool bSwitch = (bToggle != 0);
		pVehicle->MarkVehicle(bSwitch);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}