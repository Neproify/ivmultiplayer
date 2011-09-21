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
}

// createObject(modelhash, x, y, z, rx, ry, rz)
SQInteger CObjectNatives::Create(SQVM * pVM)
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
SQInteger CObjectNatives::Delete(SQVM * pVM)
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
SQInteger CObjectNatives::GetModel(SQVM * pVM)
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
SQInteger CObjectNatives::SetCoordinates(SQVM * pVM)
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
SQInteger CObjectNatives::GetCoordinates(SQVM * pVM)
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
SQInteger CObjectNatives::SetRotation(SQVM * pVM)
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
SQInteger CObjectNatives::GetRotation(SQVM * pVM)
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
