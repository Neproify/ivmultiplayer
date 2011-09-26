//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CObjectManager.h"
#include "../CNetworkManager.h"

extern CObjectManager * g_pObjectManager;
extern CNetworkManager * g_pNetworkManager;

// Object functions

void CObjectNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createObject", Create, 7, "iffffff");
	pScriptingManager->RegisterFunction("deleteObject", Delete, 1, "i");
	pScriptingManager->RegisterFunction("getObjectModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("setObjectCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setObjectPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setObjectRotation", SetRotation, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectRotation", GetRotation, 1, "i");
}

// createObject(modelhash, x, y, z, rx, ry, rz)
SQInteger CObjectNatives::Create(SQVM * pVM)
{
	SQInteger modelhash;
	CVector3 vecPosition;
	CVector3 vecRotation;
	sq_getinteger(pVM, -7, &modelhash);
	sq_getvector3(pVM, -6, &vecPosition);
	sq_getvector3(pVM, -3, &vecRotation);
	sq_pushentity(pVM, g_pObjectManager->Create(modelhash, vecPosition, vecRotation));
	return 1;
}

// deleteObject(objectid)
SQInteger CObjectNatives::Delete(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, -1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->Delete(objectId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getObjectModel(objectid)
SQInteger CObjectNatives::GetModel(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, 2, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		sq_pushinteger(pVM, g_pObjectManager->GetModel(objectId));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setObjectCoordinates(objectid, x, y, z)
SQInteger CObjectNatives::SetCoordinates(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, 2, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;
		sq_getvector3(pVM, 3, &vecPosition);
		sq_pushbool(pVM, g_pObjectManager->SetPosition(objectId, vecPosition));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getObjectCoordinates(objectid)
SQInteger CObjectNatives::GetCoordinates(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, 2, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;

		if(g_pObjectManager->GetPosition(objectId, vecPosition))
		{
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


// setObjectRotation(objectid, x, y, z)
SQInteger CObjectNatives::SetRotation(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, 2, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;
		sq_getvector3(pVM, 3, &vecPosition);
		sq_pushbool(pVM, g_pObjectManager->SetRotation(objectId, vecPosition));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getObjectRotation(objectid)
SQInteger CObjectNatives::GetRotation(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, 2, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecRotation;

		if(g_pObjectManager->GetRotation(objectId, vecRotation))
		{
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
