//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGlobalsManager.cpp
// Project: Server.Core
// Author(s): CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include <Scripting/Natives/Natives.h>
#include "CSquirrel.h"
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include "CGlobalsManager.h"
extern CGlobalsManager * g_pGlobalsManager;

void CGlobalsManager::RegisterNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isGlobalSet", IsSet, 1, "s");
	pScriptingManager->RegisterFunction("setGlobal", Set, -1, NULL);
	pScriptingManager->RegisterFunction("getGlobal", Get, 1, "s");
	pScriptingManager->RegisterFunction("removeGlobal", Remove, 1, "s");

	pScriptingManager->RegisterFunction("isPVarSet", IsPVarSet, 2, "is");
	pScriptingManager->RegisterFunction("setPVar", SetPVar, -1, NULL);
	pScriptingManager->RegisterFunction("getPVar", GetPVar, 2, "is");
	pScriptingManager->RegisterFunction("removePVar", RemovePVar, 2, "is");
	//pScriptingManager->RegisterFunction("clearPVars", ClearPVars, 1, "i");
}

SQInteger CGlobalsManager::IsSet(SQVM* pVM)
{
	const char* szKey;
	sq_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	sq_pushbool(pVM, g_pGlobalsManager->IsSet(playerId, String(szKey)));
	return 1;
}
SQInteger CGlobalsManager::Set(SQVM* pVM)
{
	const char* szKey;
	CSquirrelArgument pVar;

	sq_getstring(pVM, -2, &szKey);
	sq_getarg(pVM, -1, &pVar);
	
	EntityId playerId = MAX_PLAYERS + 1;
	g_pGlobalsManager->Set(playerId, String(szKey), pVar);
	sq_pushbool(pVM, true);
	return 1;
}
SQInteger CGlobalsManager::Get(SQVM* pVM)
{
	const char* szKey;
	sq_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	CSquirrelArgument pVar = g_pGlobalsManager->Get(playerId, String(szKey));
	pVar.push(pVM);
	return 1;
}
SQInteger CGlobalsManager::Remove(SQVM* pVM)
{
	const char* szKey;
	sq_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	sq_pushbool(pVM, g_pGlobalsManager->Remove(playerId, String(szKey)));
	return 1;
}

SQInteger CGlobalsManager::IsPVarSet(SQVM* pVM)
{
	EntityId playerId;
	const char* szKey;
	
	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szKey);

	//if(IvmpNatives::IsPlayerConnected(playerId))
		sq_pushbool(pVM, g_pGlobalsManager->IsSet(playerId, String(szKey)));
	//else
	//	sq_pushbool(pVM, false);
	return 1;
}
SQInteger CGlobalsManager::SetPVar(SQVM* pVM)
{
	EntityId playerId;
	const char* szKey;
	CSquirrelArgument pVar;

	sq_getentity(pVM, -3, &playerId);
	sq_getstring(pVM, -2, &szKey);
	sq_getarg(pVM, -1, &pVar);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
	//{
		g_pGlobalsManager->Set(playerId, String(szKey), pVar);
		sq_pushbool(pVM, true);
	//}
	//else
	//	sq_pushbool(pVM, false);
	return 1;
}
SQInteger CGlobalsManager::GetPVar(SQVM* pVM)
{
	EntityId playerId;
	const char* szKey;

	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szKey);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
	//{
		CSquirrelArgument pVar = g_pGlobalsManager->Get(playerId, String(szKey));
		pVar.push(pVM);
	//}
	//else
	//	sq_pushbool(pVM, false);
	return 1;
}
SQInteger CGlobalsManager::RemovePVar(SQVM* pVM)
{
	EntityId playerId;
	const char* szKey;

	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szKey);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
		sq_pushbool(pVM, g_pGlobalsManager->Remove(playerId, String(szKey)));
	//else
	//	sq_pushbool(pVM, false);
	return 1;
}
/*SQInteger CGlobalsManager::ClearPVars(SQVM* pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
	
	if(IvmpNatives::IsPlayerConnected(playerId))
		sq_pushbool(pVM, g_pGlobalsManager->Remove(playerId));
	else
		sq_pushbool(pVM, false);
	return 1;
}*/