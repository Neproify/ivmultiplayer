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
	pScriptingManager->RegisterFunction("createExplosion", CreateExplosion, 4, "ffff");
	pScriptingManager->RegisterFunction("createFire", CreateFire, 4, "ffff");
	pScriptingManager->RegisterFunction("deleteFire", DeleteFire, 1, "i");
	pScriptingManager->RegisterFunction("attachObjectToPlayer", AttachPed, 8, "iiffffff");
	pScriptingManager->RegisterFunction("attachObjectToVehicle", AttachVehicle, 8, "iiffffff");
	pScriptingManager->RegisterFunction("detachObject", DetachObject, 1, "i");
	pScriptingManager->RegisterFunction("moveObject", MoveObject, -1, NULL);
	pScriptingManager->RegisterFunction("rotateObject", RotateObject, 5, "iffff");
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

SQInteger CObjectNatives::CreateExplosion(SQVM * pVM)
{
	CVector3 vecPos;
	float fdensity;
	sq_getfloat(pVM,-4,&vecPos.fX);
	sq_getfloat(pVM,-3,&vecPos.fY);
	sq_getfloat(pVM,-2,&vecPos.fZ);
	sq_getfloat(pVM,-1,&fdensity);
	g_pObjectManager->CreateExplosion(vecPos, fdensity);

	sq_pushbool(pVM,true);
	return 1;
}

SQInteger CObjectNatives::CreateFire(SQVM *pVM)
{
	CVector3 vecPos;
	float fdensity;
	sq_getfloat(pVM,-4,&vecPos.fX);
	sq_getfloat(pVM,-3,&vecPos.fY);
	sq_getfloat(pVM,-2,&vecPos.fZ);
	sq_getfloat(pVM,-1,&fdensity);

	sq_pushentity(pVM, g_pObjectManager->CreateFire(vecPos,fdensity));
	return 1;
}

SQInteger CObjectNatives::DeleteFire(SQVM *pVM)
{
	EntityId fireId;
	sq_getentity(pVM,-1,&fireId);

	g_pObjectManager->DeleteFire(fireId);
	return 1;
}

SQInteger CObjectNatives::AttachPed(SQVM *pVM)
{
	EntityId	objectId;
	EntityId	playerId;
	CVector3	vecPos;
	CVector3	vecRot;

	sq_getentity(pVM,-8,&objectId);
	sq_getentity(pVM,-7,&playerId);
	sq_getfloat(pVM,-6,&vecPos.fX);
	sq_getfloat(pVM,-5,&vecPos.fY);
	sq_getfloat(pVM,-4,&vecPos.fZ);
	sq_getfloat(pVM,-3,&vecRot.fX);
	sq_getfloat(pVM,-2,&vecRot.fY);
	sq_getfloat(pVM,-1,&vecRot.fZ);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot);
		sq_pushbool(pVM,true);
	}
	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CObjectNatives::AttachVehicle(SQVM * pVM)
{
	EntityId	objectId;
	EntityId	vehicleId;
	CVector3	vecPos;
	CVector3	vecRot;

	sq_getentity(pVM,-8,&objectId);
	sq_getentity(pVM,-7,&vehicleId);
	sq_getfloat(pVM,-6,&vecPos.fX);
	sq_getfloat(pVM,-5,&vecPos.fY);
	sq_getfloat(pVM,-4,&vecPos.fZ);
	sq_getfloat(pVM,-3,&vecRot.fX);
	sq_getfloat(pVM,-2,&vecRot.fY);
	sq_getfloat(pVM,-1,&vecRot.fZ);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToVehicle(objectId,vehicleId,vecPos,vecRot);
		sq_pushbool(pVM,true);
	}
	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CObjectNatives::DetachObject(SQVM *pVM)
{
	EntityId	objectId;
	sq_getentity(pVM, -1, &objectId);
	if(g_pObjectManager->DoesExist(objectId) && g_pObjectManager->GetAttachState(objectId))
	{
		g_pObjectManager->Detach(objectId);
		sq_pushbool(pVM,true);
	}
	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CObjectNatives::MoveObject(SQVM * pVM)
{
	EntityId objectId;
	CVector3 vecMoveTarget;
	CVector3 vecMoveRot;
	float fSpeed;
	if(sq_gettop(pVM) >= 5) {
		sq_getentity(pVM, 2, &objectId);
		sq_getfloat(pVM, 3, &vecMoveTarget.fX);
		sq_getfloat(pVM, 4, &vecMoveTarget.fY);
		sq_getfloat(pVM, 5, &vecMoveTarget.fZ);
		sq_getfloat(pVM, 6, &fSpeed);

		if(g_pObjectManager->DoesExist(objectId)) {
			g_pObjectManager->GetRotation(objectId, vecMoveRot);
		}

		if(sq_gettop(pVM) > 5)
		{
			sq_getfloat(pVM, 7, &vecMoveRot.fX);
			sq_getfloat(pVM, 8, &vecMoveRot.fY);
			sq_getfloat(pVM, 9, &vecMoveRot.fZ);
		}

		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, vecMoveRot, fSpeed);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CObjectNatives::RotateObject(SQVM * pVM)
{
	EntityId objectId;
	CVector3 vecMoveRot;
	float fSpeed;
	sq_getentity(pVM, -5, &objectId);
	sq_getfloat(pVM, -4, &vecMoveRot.fX);
	sq_getfloat(pVM, -3, &vecMoveRot.fY);
	sq_getfloat(pVM, -2, &vecMoveRot.fZ);
	sq_getfloat(pVM, -1, &fSpeed);
	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->RotateObject(objectId, vecMoveRot, fSpeed);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}
