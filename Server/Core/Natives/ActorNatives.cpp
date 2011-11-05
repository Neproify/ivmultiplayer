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

extern CActorManager * g_pActorManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Actor functions

void CActorNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createActor", Create, 5, "iffff");
	pScriptingManager->RegisterFunction("deleteActor", Delete, 1, "i");
	pScriptingManager->RegisterFunction("isActorValid",IsValid,1,"i");
	pScriptingManager->RegisterFunction("setActorCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("setActorPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("setActorHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("actorWalkToCoordinatesForPlayer", WalkToCoordinates, 6, "iifffi");
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
		bsSend.Write(iType);
		g_pNetworkManager->RPC(RPC_ScriptingActorWalkToCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
