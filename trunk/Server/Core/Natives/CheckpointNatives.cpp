//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.cpp
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CCheckpointManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CCheckpointManager * g_pCheckpointManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Checkpoint functions

void CCheckpointNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createCheckpoint", Create, 8, "ifffffff");
	pScriptingManager->RegisterFunction("deleteCheckpoint", Delete, 1, "i");
	pScriptingManager->RegisterFunction("showCheckpointForPlayer", ShowForPlayer, 2, "ii");
	pScriptingManager->RegisterFunction("hideCheckpointForPlayer", HideForPlayer, 2, "ii");
	pScriptingManager->RegisterFunction("showCheckpointForAll", ShowForAll, 1, "i");
	pScriptingManager->RegisterFunction("hideCheckpointForAll", HideForAll, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointType", SetType, 2, "ii");
	pScriptingManager->RegisterFunction("getCheckpointType", GetType, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointPosition", SetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("getCheckpointPosition", GetPosition, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointTargetPosition", SetTargetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("getCheckpointTargetPosition", GetTargetPosition, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointRadius", SetRadius, 2, "if");
	pScriptingManager->RegisterFunction("getCheckpointRadius", GetRadius, 1, "i");
	//pScriptingManager->RegisterFunction("setCheckpointDimension", SetDimension, 2, "ii");
	//pScriptingManager->RegisterFunction("getCheckpointDimension", GetDimension, 1, "i");
}

// createCheckpoint(type, x, y, z, targetx, targety, targetz, radius)
SQInteger CCheckpointNatives::Create(SQVM * pVM)
{
	SQInteger iType;
	CVector3 vecPosition;
	CVector3 vecTargetPosition;
	float fRadius;
	sq_getinteger(pVM, -8, &iType);
	sq_getvector3(pVM, -7, &vecPosition);
	sq_getvector3(pVM, -4, &vecTargetPosition);
	sq_getfloat(pVM, -1, &fRadius);
	sq_pushentity(pVM, g_pCheckpointManager->Add(iType, vecPosition, vecTargetPosition, fRadius));
	return 1;
}

// deleteCheckpoint(checkpointid)
SQInteger CCheckpointNatives::Delete(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	sq_pushbool(pVM, g_pCheckpointManager->Delete(checkpointId));
	return 1;
}

// showCheckpointForPlayer(checkpointid, playerid)
SQInteger CCheckpointNatives::ShowForPlayer(SQVM * pVM)
{
	EntityId checkpointId;
	EntityId playerId;
	sq_getentity(pVM, -2, &checkpointId);
	sq_getentity(pVM, -1, &playerId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->ShowForPlayer(playerId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// hideCheckpointForPlayer(checkpointid, playerid)
SQInteger CCheckpointNatives::HideForPlayer(SQVM * pVM)
{
	EntityId checkpointId;
	EntityId playerId;
	sq_getentity(pVM, -2, &checkpointId);
	sq_getentity(pVM, -1, &playerId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->HideForPlayer(playerId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// showCheckpointForAll(checkpointid)
SQInteger CCheckpointNatives::ShowForAll(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->ShowForWorld();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// hideCheckpointForAll(checkpointid)
SQInteger CCheckpointNatives::HideForAll(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->HideForWorld();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCheckpointType(checkpointid, type)
SQInteger CCheckpointNatives::SetType(SQVM * pVM)
{
	EntityId checkpointId;;
	SQInteger iType;
	sq_getentity(pVM, -1, &checkpointId);
	sq_getinteger(pVM, -1, &iType);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetType(iType);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getCheckpointType(checkpointid, type)
SQInteger CCheckpointNatives::GetType(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		sq_pushinteger(pVM, pCheckpoint->GetType());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCheckpointPosition(checkpointid, position)
SQInteger CCheckpointNatives::SetPosition(SQVM * pVM)
{
	EntityId checkpointId;;
	CVector3 vecPosition;
	sq_getentity(pVM, -4, &checkpointId);
	sq_getvector3(pVM, -3, &vecPosition);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetPosition(vecPosition);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getCheckpointPosition(checkpointid)
SQInteger CCheckpointNatives::GetPosition(SQVM * pVM)
{
	EntityId checkpointId;;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		CVector3 vecPosition;
		pCheckpoint->GetPosition(vecPosition);
		sq_newtable(pVM);
		sq_pushinteger(pVM, 0);
		sq_pushfloat(pVM, vecPosition.fX);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 1);
		sq_pushfloat(pVM, vecPosition.fY);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 2);
		sq_pushfloat(pVM, vecPosition.fZ);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCheckpointTargetPosition(checkpointid, targetposition)
SQInteger CCheckpointNatives::SetTargetPosition(SQVM * pVM)
{
	EntityId checkpointId;
	CVector3 vecTargetPosition;
	sq_getentity(pVM, -4, &checkpointId);
	sq_getvector3(pVM, -3, &vecTargetPosition);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetTargetPosition(vecTargetPosition);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getCheckpointTargetPosition(checkpointid)
SQInteger CCheckpointNatives::GetTargetPosition(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		CVector3 vecTargetPosition;
		pCheckpoint->GetTargetPosition(vecTargetPosition);
		sq_newtable(pVM);
		sq_pushinteger(pVM, 0);
		sq_pushfloat(pVM, vecTargetPosition.fX);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 1);
		sq_pushfloat(pVM, vecTargetPosition.fY);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 2);
		sq_pushfloat(pVM, vecTargetPosition.fZ);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCheckpointRadius(checkpointid, radius)
SQInteger CCheckpointNatives::SetRadius(SQVM * pVM)
{
	EntityId checkpointId;
	float fRadius;
	sq_getentity(pVM, -2, &checkpointId);
	sq_getfloat(pVM, -1, &fRadius);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetRadius(fRadius);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getCheckpointRadius(checkpointid)
SQInteger CCheckpointNatives::GetRadius(SQVM * pVM)
{
	EntityId checkpointId;
	sq_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		sq_pushfloat(pVM, pCheckpoint->GetRadius());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CCheckpointNatives::SetDimension(SQVM * pVM)
{
	return 1;
}

SQInteger CCheckpointNatives::GetDimension(SQVM * pVM)
{
	return 1;
}
