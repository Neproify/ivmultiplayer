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
	pScriptingManager->RegisterFunction("repairVehicle", RepairVehicle, 1, "i");

	pScriptingManager->RegisterFunction("setVehicleRespawnDelay", SetRespawnDelay, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleRespawnDelay", GetRespawnDelay, -1, NULL);
	pScriptingManager->RegisterFunction("setVehicleDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleDimension", GetDimension, 1, "i");
}

// createVehicle(model, x, y, z, rx, ry, rz, color1, color2, color3, color4, respawn_delay)
SQInteger CVehicleNatives::Create(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Ensure we have the least required arguments
	if(arguments.size() < 4)
	{
		CLogFile::Printf("createVehicle: Native requires at least 4 parameters.");
		sq_pushinteger(pVM, INVALID_ENTITY_ID);
		return 1;
	}

	// Get the model id
	int iModelId = arguments.pop().GetInteger();

	if(iModelId < 0 || iModelId == 41 || iModelId == 96 || iModelId == 107 || iModelId == 111 || iModelId > 123)
	{
		CLogFile::Printf("createVehicle: Invalid vehicle model (%d).", iModelId);
		sq_pushinteger(pVM, INVALID_ENTITY_ID);
		return 1;
	}

	// Get the position
	CVector3 vecPosition;
	arguments.popVector3(vecPosition);

	// Do we have any more parameters?
	CVector3 vecRotation;
	int iRespawnDelay = -1;
	int iColors[4];

	if(arguments.size() >= 3)
	{
		// Get the rotation
		arguments.popVector3(vecRotation);

		// Do we have any more parameters?
		if(arguments.size() >= 4)
		{
			iColors[0] = arguments.pop().GetInteger();
			iColors[1] = arguments.pop().GetInteger();
			iColors[2] = arguments.pop().GetInteger();
			iColors[3] = arguments.pop().GetInteger();

			// Do we have any more parameters?
			if(arguments.size() == 1)
				iRespawnDelay = arguments.pop().GetInteger();
		}
	}

	if(iModelId >= 0 && iModelId <= 125)
	{
		EntityId vehicleId =  g_pVehicleManager->Add(iModelId, vecPosition, vecRotation, iColors[0], iColors[1], 
													 iColors[2], iColors[3], iRespawnDelay);
		sq_pushinteger(pVM, vehicleId);
		return 1;
	}

	CLogFile::Printf("createVehicle: Invalid vehicle model (%d).", iModelId);
	sq_pushinteger(pVM, INVALID_ENTITY_ID);
	return 1;
}

// deleteVehicle(vehicleid)
SQInteger CVehicleNatives::Delete(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		g_pVehicleManager->Remove(vehicleId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleCoordinates(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetCoordinates(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	CVector3 vecPosition;
	arguments.popVector3(vecPosition);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
		
	if(pVehicle)
	{
		pVehicle->SetPosition(vecPosition);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleCoordinates(vehicleid)
SQInteger CVehicleNatives::GetCoordinates(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CVector3 vecPosition;
		pVehicle->GetPosition(vecPosition);

		CSquirrelArguments retargs;
		retargs.pushVector3(vecPosition);
		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleRotation(vehicleid, rotation)
SQInteger CVehicleNatives::SetRotation(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	CVector3 vecRotation;
	arguments.popVector3(vecRotation);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetRotation(vecRotation);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleDirtLevel(vehicleid, level)
SQInteger CVehicleNatives::SetDirtLevel(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	float fDirtLevel = arguments.pop().GetFloat();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetDirtLevel(fDirtLevel);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::GetDirtLevel(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bSirenState = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetSirenState(bSirenState);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleSirenState(vehicleid)
SQInteger CVehicleNatives::GetSirenState(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iDuration = arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CVector3 vecRotation;
		pVehicle->GetRotation(vecRotation);

		CSquirrelArguments retargs;
		retargs.pushVector3(vecRotation);
		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isVehicleValid(vehicleid)
SQInteger CVehicleNatives::IsValid(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	sq_pushbool(pVM, g_pVehicleManager->DoesExist(vehicleId));
	return 1;
}

// setVehicleColor(vehicleid, color1, color2, color3, color4)
// TODO: Only require two colors
SQInteger CVehicleNatives::SetColor(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Ensure we have the least required arguments
	if(arguments.size() < 3)
	{
		CLogFile::Printf("setVehicleColor: Natives requires at least 3 arguments.");
		sq_pushbool(pVM, false);
		return 1;
	}

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	int iColors[4];
	memset(&iColors, 0, sizeof(iColors));
	iColors[0] = arguments.pop().GetInteger();
	iColors[1] = arguments.pop().GetInteger();

	if(arguments.size() == 2)
	{
		iColors[2] = arguments.pop().GetInteger();
		iColors[3] = arguments.pop().GetInteger();
	}

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetColors(iColors[0], iColors[1], iColors[2], iColors[3]);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleColor(vehicleid)
SQInteger CVehicleNatives::GetColor(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		BYTE byteColors[4];
		pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);

		CSquirrelArguments retargs;
		retargs.push((int)byteColors[0]);
		retargs.push((int)byteColors[1]);
		retargs.push((int)byteColors[2]);
		retargs.push((int)byteColors[3]);
		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleRespawnDelay(vehicleid, respawn_delay)
SQInteger CVehicleNatives::SetRespawnDelay(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iRespawnDelay = arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetRespawnDelay(iRespawnDelay);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleRespawnDelay(vehicleid)
SQInteger CVehicleNatives::GetRespawnDelay(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetRespawnDelay());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleModel(vehicleid)
SQInteger CVehicleNatives::GetModel(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetModel());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleHealth(vehicleid, health)
SQInteger CVehicleNatives::SetHealth(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iHealth = arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetHealth(iHealth);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleHealth(vehicleid)
SQInteger CVehicleNatives::GetHealth(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetHealth());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleEngineHealth(vehicleid, enginehealth)
SQInteger CVehicleNatives::SetEngineHealth(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iHealth = arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CLogFile::Printf("Function setVehicleEngineHealth is depreciated: please use setVehicleHealth.");
		pVehicle->SetHealth(iHealth);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineHealth(vehicleid)
SQInteger CVehicleNatives::GetEngineHealth(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CLogFile::Printf("Function getVehicleEngineHealth is depreciated: please use getVehicleHealth.");
		sq_pushinteger(pVM, pVehicle->GetHealth());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleVelocity(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetVelocity(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	CVector3 vecMoveSpeed;
	arguments.popVector3(vecMoveSpeed);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle && pVehicle->GetDriver())
	{
		if(pVehicle->GetDriver()->GetVehicle()->GetVehicleId() == pVehicle->GetVehicleId())
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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CVector3 vecMoveSpeed;
		pVehicle->GetMoveSpeed(vecMoveSpeed);

		CSquirrelArguments retargs;
		retargs.pushVector3(vecMoveSpeed);
		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleAngularVelocity(vehicleid, x, y, z)
SQInteger CVehicleNatives::SetAngularVelocity(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	CVector3 vecTurnSpeed;
	arguments.popVector3(vecTurnSpeed);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetTurnSpeed(vecTurnSpeed);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleAngularVelocity(vehicleid)
SQInteger CVehicleNatives::GetAngularVelocity(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CVector3 vecTurnSpeed;
		pVehicle->GetTurnSpeed(vecTurnSpeed);

		CSquirrelArguments retargs;
		retargs.pushVector3(vecTurnSpeed);
		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// respawnVehicle(vehicleid)
SQInteger CVehicleNatives::Respawn(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->Respawn();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isVehicleOccupied(vehicleid)
SQInteger CVehicleNatives::IsOccupied(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CSquirrelArguments retargs;

		for(BYTE i = 0; i < (MAX_VEHICLE_PASSENGERS + 1); i++)
		{
			retargs.push((int)(i + 1));
			CPlayer * pOccupant = pVehicle->GetOccupant(i);
			retargs.push(pOccupant ? (int)pOccupant->GetPlayerId() : (int)INVALID_ENTITY_ID);
		}

		sq_pusharg(pVM, CSquirrelArgument(retargs, false));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleLocked(vehicleid, locked)
SQInteger CVehicleNatives::SetLocked(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iLocked = arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bFrontLeft = arguments.pop().GetBool();
	bool bFrontRight = arguments.pop().GetBool();
	bool bBackLeft = arguments.pop().GetBool();
	bool bBackRight = arguments.pop().GetBool();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CSquirrelArguments retargs;

		for(unsigned char uc = 0; uc <= 3; ++ uc)
			retargs.push(pVehicle->GetIndicatorState(uc));

		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::SetComponent(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iSlot = arguments.pop().GetInteger();
	bool bOn = arguments.pop().GetBool();
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		if(iSlot >= 0 && iSlot <= 8)
		{
			pVehicle->SetComponentState((unsigned char)iSlot, bOn);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CVehicleNatives::ResetComponents(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	
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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		CSquirrelArguments retargs;

		for(unsigned char i = 0; i < 9; ++ i)
			retargs.push(pVehicle->GetComponentState(i));

		sq_pusharg(pVM, CSquirrelArgument(retargs, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleVariation(vehicleid, variation)
SQInteger CVehicleNatives::SetVariation(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iVariation = arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetVariation((unsigned char)iVariation);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleVariation(vehicleid)
SQInteger CVehicleNatives::GetVariation(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		sq_pushinteger(pVM, pVehicle->GetVariation());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleEngineState(vehicleid, turned_on?)
SQInteger CVehicleNatives::SetEngineStatus(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bEngineStatus = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetEngineStatus(bEngineStatus);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineState(vehicleid)
SQInteger CVehicleNatives::GetEngineStatus(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	
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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bToggle = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		pVehicle->TurnTaxiLights(bToggle);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

// controlCarDoors
SQInteger CVehicleNatives::ControlCar(SQVM *pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iDoor = arguments.pop().GetInteger();
	bool bToggle = arguments.pop().GetBool();
	float fAngle = arguments.pop().GetFloat();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		pVehicle->SetCarDoorAngle(iDoor, bToggle, fAngle);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::SetLights(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bToggle = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetLights(bToggle);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

SQInteger CVehicleNatives::GetLights(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushbool(pVM, pVehicle->GetLights());
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

// repairVehicleWheels(vehicleid)
SQInteger CVehicleNatives::RepairWheels(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWheels();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

// repairVehicleWindows(vehicleid)
SQInteger CVehicleNatives::RepairWindows(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWindows();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 0;
}

// setVehicleGpsState(vehicleid, state)
SQInteger CVehicleNatives::SetGpsState(SQVM *pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bState = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetVehicleGPSState(bState);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleGpsState(vehicleid)
SQInteger CVehicleNatives::GetGpsState(SQVM *pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

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
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iDuration = arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetAlarm(iDuration);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}


// markVehicleAsActorVehicle(vehicleid)
SQInteger CVehicleNatives::MarkVehicle(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	bool bToggle = arguments.pop().GetBool();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->MarkVehicle(bToggle);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// repairVehicle(vehicleid)
SQInteger CVehicleNatives::RepairVehicle(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairVehicle();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleDimension(vehicleid, demnsion)
SQInteger CVehicleNatives::SetDimension(SQVM * pVM)
{
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();
	int iDimension = arguments.pop().GetInteger();
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetDimension(iDimension);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getVehicleDimension(vehicleid)
SQInteger CVehicleNatives::GetDimension(SQVM * pVM)
{ 
	// Create the arguments
	CSquirrelArguments arguments(pVM, 2);

	// Get the parameters
	EntityId vehicleId = (EntityId)arguments.pop().GetInteger();

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		sq_pushinteger(pVM, (SQInteger)pVehicle->GetDimension());
		return 1;
	}

	sq_pushinteger(pVM, INVALID_DIMENSION_ID);
	return 1;
}
