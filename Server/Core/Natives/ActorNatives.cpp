//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CActorManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"
#include <CLogFile.h>

extern CActorManager * g_pActorManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Actor functions

void CActorNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createActor", Create, 5, "iffff");
	pScriptingManager->RegisterFunction("deleteActor", Delete, 1, "i");
	pScriptingManager->RegisterFunction("isActorValid",IsValid,1,"i");
	pScriptingManager->RegisterFunction("getActorModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("getActorCount",GetCount,0,NULL);
	pScriptingManager->RegisterFunction("setActorCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getActorCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setActorPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("setActorHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("getActorHeading", GetHeading, 1, "i");
	pScriptingManager->RegisterFunction("actorWalkToCoordinatesForPlayer", WalkToCoordinates, 6, "iifffi");
	pScriptingManager->RegisterFunction("setActorName",SetName, 2, "is");
	pScriptingManager->RegisterFunction("getActorName",GetName, 1, "i");
	pScriptingManager->RegisterFunction("toggleActorNametag", ToggleNametag, 2, "ib");
	pScriptingManager->RegisterFunction("toggleActorBlip", ToggleBlip, 2, "ib");
	pScriptingManager->RegisterFunction("setActorNametagColor",SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("getActorNametagColor",GetColor, 1, "i");
	pScriptingManager->RegisterFunction("toggleActorFrozen",ToggleFrozen, 2, "ib");
	pScriptingManager->RegisterFunction("toggleActorHelmet",ToggleHelmet, 2, "ib");
	pScriptingManager->RegisterFunction("warpActorIntoVehicle",WarpIntoVehicle, 3, "iii");
	pScriptingManager->RegisterFunction("removeActorFromVehicle",RemoveFromVehicle, 1, "i");
	pScriptingManager->RegisterFunction("deleteAllActors",DeleteAll, 0, NULL);
	pScriptingManager->RegisterFunction("driveActorToCoordinates", DriveToCoordinates, 4,"ifff");
	pScriptingManager->RegisterFunction("forceAnimationAtActor", ForceAnim, 3, "iss");
	pScriptingManager->RegisterFunction("sayActorSpeech", SaySpeech, 3, "iss");
}

// createActor(modelhash, x, y, z, r)
SQInteger CActorNatives::Create(SQVM * pVM)
{
	SQInteger iModelId;
	CVector3 vecPosition;
	float fHeading;
	sq_getinteger(pVM, -5, &iModelId);
	sq_getvector3(pVM, -4, &vecPosition);
	sq_getfloat(pVM, -1, &fHeading);
	sq_pushentity(pVM, g_pActorManager->Create(iModelId, vecPosition, fHeading));
	return 1;
}

// deleteActor(actorid)
SQInteger CActorNatives::Delete(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->Delete(actorId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isActorValid(actorid)
SQInteger CActorNatives::IsValid(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		sq_pushbool(pVM, true);
		return 1;
	}
	else
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	return 1;
}

// setActorCoordinates(actorid, x, y, z)
SQInteger CActorNatives::SetCoordinates(SQVM * pVM)
{
	EntityId actorId;
	CVector3 vecPosition;
	sq_getentity(pVM, -4, &actorId);
	sq_getvector3(pVM, -3, &vecPosition);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetPosition(actorId, vecPosition);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getActorCoordinates(actorid)
SQInteger CActorNatives::GetCoordinates(SQVM * pVM)
{
	EntityId actorId;
    sq_getentity(pVM, -1, &actorId);

    if(g_pActorManager->DoesExist(actorId))
    {
		CVector3 vecPosition = g_pActorManager->GetPosition(actorId);
		CSquirrelArguments args;
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}
    sq_pushbool(pVM, false);
    return 1;
}

// setActorHeading(actorid, r)
SQInteger CActorNatives::SetHeading(SQVM * pVM)
{
	EntityId actorId;
	float fHeading;
	sq_getentity(pVM, -2, &actorId);
	sq_getfloat(pVM, -1, &fHeading);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetHeading(actorId, fHeading);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getActorHeading(actorid)
SQInteger CActorNatives::GetHeading(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		float fHeading = g_pActorManager->GetHeading(actorId);
		sq_pushfloat(pVM, fHeading);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getActorModel(actorid)
SQInteger CActorNatives::GetModel(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		int aModel = g_pActorManager->GetModel(actorId);
		sq_pushinteger(pVM, aModel);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getActorCount()
SQInteger CActorNatives::GetCount(SQVM * pVM)
{
	if(g_pActorManager)
	{
		int Count = g_pActorManager->GetActorCount();
		sq_pushinteger(pVM, Count);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// actorWalkToCoordinatesForPlayer(playerid, actorid, x, y, z)
SQInteger CActorNatives::WalkToCoordinates(SQVM * pVM)
{
	EntityId playerId;
	EntityId actorId;
	CVector3 vecPosition;
	SQInteger iType;
	sq_getentity(pVM, -6, &playerId);
	sq_getentity(pVM, -5, &actorId);
	sq_getvector3(pVM, -4, &vecPosition);
	sq_getinteger(pVM, -1, &iType);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(vecPosition);
		bsSend.Write((int)iType);
		g_pNetworkManager->RPC(RPC_ScriptingActorWalkToCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setActorName(actorid, name)
SQInteger CActorNatives::SetName(SQVM * pVM)
{
	EntityId actorId;
	const char * name = NULL;
	sq_getentity(pVM, -2, &actorId);
	sq_getstring(pVM, -1, &name);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetActorName(actorId, name);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getActorName(actorid)
SQInteger CActorNatives::GetName(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		sq_pushstring(pVM, g_pActorManager->GetActorName(actorId), -1);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// toggleActorNametag(actorid, toggle)
SQInteger CActorNatives::ToggleNametag(SQVM * pVM)
{
	EntityId actorId;
	SQBool show;
	sq_getentity(pVM, -2, &actorId);
	sq_getbool(pVM, -1, &show);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (show != 0);
		g_pActorManager->ToggleNametag(actorId, bToggle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// toggleActorBlip(actorid, toggle)
SQInteger CActorNatives::ToggleBlip(SQVM * pVM)
{
	EntityId actorId;
	SQBool show;
	sq_getentity(pVM, -2, &actorId);
	sq_getbool(pVM, -1, &show);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (show != 0);
		g_pActorManager->ToggleBlip(actorId, bToggle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}
// setActorNametagColor(actorid, color)
SQInteger CActorNatives::SetColor(SQVM * pVM)
{
	EntityId actorId;
	SQInteger color = 0xFFFFFFFF;
	sq_getentity(pVM, -2, &actorId);
	sq_getinteger(pVM, -1, &color);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetColor(actorId,color);
		return 1;
	}
	sq_pushbool(pVM,false);
	return 1;
}

// getActorNametagColor(actorid)
SQInteger CActorNatives::GetColor(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		sq_pushinteger(pVM, g_pActorManager->GetColor(actorId));
		return 1;
	}
	sq_pushbool(pVM,false);
	return 1;
}

// toggleActorFrozen(actorid, toggle)
SQInteger CActorNatives::ToggleFrozen(SQVM * pVM)
{
	EntityId actorId;
	SQBool frozen;
	sq_getentity(pVM, -2, &actorId);
	sq_getbool(pVM, -1, &frozen);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (frozen != 0);
		g_pActorManager->ToggleFrozen(actorId, bToggle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// toggleActorHelmet(actorid, toggle)
SQInteger CActorNatives::ToggleHelmet(SQVM * pVM)
{
	EntityId actorId;
	SQBool helmet;
	sq_getentity(pVM, -2, &actorId);
	sq_getbool(pVM, -1, &helmet);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (helmet != 0);
		g_pActorManager->ToggleHelmet(actorId, bToggle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// warpActorIntoVehicle(actorid, vehicleid, seatid)
SQInteger CActorNatives::WarpIntoVehicle(SQVM * pVM)
{
	EntityId actorId;
	EntityId vehicleId;
	SQInteger seatid;
	
	sq_getentity(pVM, -3, &actorId);
	sq_getentity(pVM, -2, &vehicleId);
	sq_getinteger(pVM, -1, &seatid);

	if(g_pActorManager->DoesExist(actorId))
	{
		if(seatid >= 0 &&  seatid <= 3)
		{
			g_pActorManager->WarpIntoVehicle(actorId,vehicleId,(int)seatid);
			sq_pushbool(pVM, true);
			return 1;
		}
		else
		{
			CLogFile::Printf("Can't warp actor %d on the seat %d from the vehicle %d(Seats are only supported from 0 to 3(driver and passenger))",actorId,seatid,vehicleId);
			sq_pushbool(pVM, false);
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

// removeActorFromVehicle(actorid)
SQInteger CActorNatives::RemoveFromVehicle(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->RemoveFromVehicle(actorId);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CActorNatives::DeleteAll(SQVM * pVM)
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(g_pActorManager->DoesExist(x))
		{
			g_pActorManager->Delete(x);
		}
	}

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger CActorNatives::ForceAnim(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM,-3, &actorId);

	const char * szGroup = NULL;
	sq_getstring(pVM,-2,&szGroup);

	const char * szAnim = NULL;
	sq_getstring(pVM,-1,&szAnim);


	if(g_pActorManager->DoesExist(actorId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(String(szGroup));
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingForceActorAnimation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
	return 1;
}

SQInteger CActorNatives::DriveToCoordinates(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM,-4, &actorId);

	CVector3 vecPos;
	sq_getfloat(pVM, -3, &vecPos.fX);
	sq_getfloat(pVM, -2, &vecPos.fY);
	sq_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pActorManager->DoesExist(actorId))
	{
		if(!g_pActorManager->UpdateDrivePos(actorId,vecPos, false))
			CLogFile::Printf("Failed to set actor's %d state to driving(not in a vehicle!)",actorId);
	}
	return 1;
}

SQInteger CActorNatives::SaySpeech(SQVM * pVM)
{
	EntityId actorId;
	sq_getentity(pVM, -3, &actorId);
	
	const char * szVoice;
	sq_getstring(pVM, -2, &szVoice);

	const char * szText;
	sq_getstring(pVM, -1, &szText);

	if(g_pActorManager->DoesExist(actorId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(String(szVoice));
		bsSend.Write(String(szText));
		g_pNetworkManager->RPC(RPC_ScriptingActorSaySpeech, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}
