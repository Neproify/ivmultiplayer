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

void RegisterObjectNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createObject", sq_object_create, 7, "iffffff");
	pScriptingManager->RegisterFunction("deleteObject", sq_object_delete, 1, "i");
	pScriptingManager->RegisterFunction("getObjectModel", sq_object_getmodel, 1, "i");
	pScriptingManager->RegisterFunction("setObjectCoordinates", sq_object_setcoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectCoordinates", sq_object_getcoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setObjectPosition", sq_object_setcoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectPosition", sq_object_getcoordinates, 1, "i");
}

// createObject(modelhash, x, y, z, rx, ry, rz)
SQInteger sq_object_create(SQVM * pVM)
{
	SQInteger modelhash;
	float x, y, z, rx, ry, rz;
	sq_getinteger(pVM, -7, &modelhash);
	sq_getfloat(pVM, -6, &x);
	sq_getfloat(pVM, -5, &y);
	sq_getfloat(pVM, -4, &z);
	sq_getfloat(pVM, -3, &rx);
	sq_getfloat(pVM, -2, &ry);
	sq_getfloat(pVM, -1, &rz);
	sq_pushinteger(pVM, g_pObjectManager->Create(modelhash, x, y, z, rx, ry, rz));
	return 1;
}

// deleteObject(objectid)
SQInteger sq_object_delete(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, -1, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		g_pObjectManager->Delete(objectid);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getObjectModel(objectid)
SQInteger sq_object_getmodel(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, 2, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		sq_pushinteger(pVM, g_pObjectManager->GetModel(objectid));
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setObjectCoordinates(objectid, x, y, z)
SQInteger sq_object_setcoordinates(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, 2, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		CVector3 vecPosition;
		sq_getvector3(pVM, 3, &vecPosition);
		sq_pushbool(pVM, g_pObjectManager->SetPosition(objectid, vecPosition));
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getObjectCoordinates(objectid)
SQInteger sq_object_getcoordinates(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, 2, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		CVector3 vecPosition;
		if(g_pObjectManager->GetPosition(objectid, &vecPosition))
		{
			sq_newarray(pVM, 0);
			sq_pushfloat(pVM, vecPosition.fX);
			sq_arrayappend(pVM, -2);
			sq_pushfloat(pVM, vecPosition.fY);
			sq_arrayappend(pVM, -2);
			sq_pushfloat(pVM, vecPosition.fZ);
			sq_arrayappend(pVM, -2);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}


// setObjectRotation(objectid, x, y, z)
SQInteger sq_object_setrotation(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, 2, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		CVector3 vecPosition;
		sq_getvector3(pVM, 3, &vecPosition);
		sq_pushbool(pVM, g_pObjectManager->SetRotation(objectid, vecPosition));
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getObjectRotation(objectid)
SQInteger sq_object_getrotation(SQVM * pVM)
{
	SQInteger objectid;
	sq_getinteger(pVM, 2, &objectid);
	if(g_pObjectManager->DoesExist(objectid))
	{
		CVector3 vecPosition;
		if(g_pObjectManager->GetRotation(objectid, &vecPosition))
		{
			sq_newarray(pVM, 0);
			sq_pushfloat(pVM, vecPosition.fX);
			sq_arrayappend(pVM, -2);
			sq_pushfloat(pVM, vecPosition.fY);
			sq_arrayappend(pVM, -2);
			sq_pushfloat(pVM, vecPosition.fZ);
			sq_arrayappend(pVM, -2);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}
