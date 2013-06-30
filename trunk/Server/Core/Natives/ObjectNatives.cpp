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
	pScriptingManager->RegisterFunction("createExplosion", CreateExplosion, 8, "fffifbbf");
	pScriptingManager->RegisterFunction("createFire", CreateFire, 4, "ffff");
	pScriptingManager->RegisterFunction("deleteFire", DeleteFire, 1, "i");
	pScriptingManager->RegisterFunction("attachObjectToPlayer", AttachPed, 8, "iiffffff");
	pScriptingManager->RegisterFunction("attachObjectToPlayerBone", AttachPedBone, 9, "iiffffffi");
	pScriptingManager->RegisterFunction("attachObjectToVehicle", AttachVehicle, 8, "iiffffff");
	pScriptingManager->RegisterFunction("detachObject", DetachObject, 1, "i");
	pScriptingManager->RegisterFunction("moveObject", MoveObject, -1, NULL);
	pScriptingManager->RegisterFunction("rotateObject", RotateObject, 5, "ifffi");
	pScriptingManager->RegisterFunction("setObjectDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getObjectDimension", GetDimension, 1, "i");
	pScriptingManager->RegisterFunction("setObjectInterior", SetInterior, 2, "ii");
	pScriptingManager->RegisterFunction("getObjectInterior", GetInterior, 1, "i");
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
	SQInteger uiExplosionType;
	float fRadius;
	SQBool bSound;
	SQBool bInvisible;
	float fCameraShake;

	sq_getfloat(pVM,-8,&vecPos.fX);
	sq_getfloat(pVM,-7,&vecPos.fY);
	sq_getfloat(pVM,-6,&vecPos.fZ);
	sq_getinteger(pVM,-5,&uiExplosionType);
	sq_getfloat(pVM,-4,&fRadius);
	sq_getbool(pVM,-3,&bSound);
	sq_getbool(pVM,-2,&bInvisible);
	sq_getfloat(pVM,-1,&fCameraShake);

	g_pObjectManager->CreateExplosion(vecPos, uiExplosionType, fRadius, bSound, bInvisible, fCameraShake);

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
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CObjectNatives::AttachPedBone(SQVM *pVM)
{
	EntityId	objectId;
	EntityId	playerId;
	CVector3	vecPos;
	CVector3	vecRot;
	int			iBone;

	sq_getentity(pVM,-9,&objectId);
	sq_getentity(pVM,-8,&playerId);
	sq_getfloat(pVM,-7,&vecPos.fX);
	sq_getfloat(pVM,-6,&vecPos.fY);
	sq_getfloat(pVM,-5,&vecPos.fZ);
	sq_getfloat(pVM,-4,&vecRot.fX);
	sq_getfloat(pVM,-3,&vecRot.fY);
	sq_getfloat(pVM,-2,&vecRot.fZ);
	sq_getinteger(pVM,-1,&iBone);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot, iBone);
		sq_pushbool(pVM,true);
		return 1;
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
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CObjectNatives::DetachObject(SQVM *pVM)
{
	EntityId	objectId;
	sq_getentity(pVM, -1, &objectId);

	if(g_pObjectManager->DoesExist(objectId) && g_pObjectManager->GetAttachState(objectId))
	{
		g_pObjectManager->Detach(objectId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CObjectNatives::MoveObject(SQVM * pVM)
{
	SQInteger iParams = sq_gettop(pVM) - 1;

	CHECK_PARAMS_MIN("moveObject", 5);
	CHECK_TYPE("moveObject", 1, -iParams, OT_INTEGER);
	CHECK_TYPE("moveObject", 2, -iParams + 1, OT_FLOAT);
	CHECK_TYPE("moveObject", 3, -iParams + 2, OT_FLOAT);
	CHECK_TYPE("moveObject", 4, -iParams + 3, OT_FLOAT);
	CHECK_TYPE("moveObject", 5, -iParams + 4, OT_INTEGER);

	if(iParams >= 6)
	{
		CHECK_TYPE("moveObject", 6, -iParams + 5, OT_FLOAT);
		CHECK_TYPE("moveObject", 7, -iParams + 6, OT_FLOAT);
		CHECK_TYPE("moveObject", 8, -iParams + 7, OT_FLOAT);
	}

	EntityId objectId;
	CVector3 vecMoveTarget;
	CVector3 vecMoveRot;
	int iTime;

	sq_getentity(pVM, 2, &objectId);
	sq_getfloat(pVM, 3, &vecMoveTarget.fX);
	sq_getfloat(pVM, 4, &vecMoveTarget.fY);
	sq_getfloat(pVM, 5, &vecMoveTarget.fZ);
	sq_getinteger(pVM, 6, &iTime);

	if(g_pObjectManager->DoesExist(objectId))
	{
		if(sq_gettop(pVM) > 5)
		{
			sq_getfloat(pVM, 7, &vecMoveRot.fX);
			sq_getfloat(pVM, 8, &vecMoveRot.fY);
			sq_getfloat(pVM, 9, &vecMoveRot.fZ);
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, iTime, true, vecMoveRot);
		}
		else
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, iTime);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CObjectNatives::RotateObject(SQVM * pVM)
{
	EntityId objectId;
	CVector3 vecMoveRot;
	int iTime;
	sq_getentity(pVM, -5, &objectId);
	sq_getfloat(pVM, -4, &vecMoveRot.fX);
	sq_getfloat(pVM, -3, &vecMoveRot.fY);
	sq_getfloat(pVM, -2, &vecMoveRot.fZ);
	sq_getinteger(pVM, -1, &iTime);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->RotateObject(objectId, vecMoveRot, iTime);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}


SQInteger CObjectNatives::SetDimension(SQVM * pVM)
{
	SQInteger iDimension;
	EntityId objectId;

	sq_getinteger(pVM, -1, &iDimension);
	sq_getentity(pVM, -2, &objectId);
	
	g_pObjectManager->SetDimension(objectId, iDimension);

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger CObjectNatives::GetDimension(SQVM * pVM)
{ 
	EntityId objectId;

	sq_getentity(pVM, -1, &objectId);

	sq_pushinteger(pVM, (SQInteger)g_pObjectManager->GetDimension(objectId));
	return 1;
}

SQInteger CObjectNatives::SetInterior(SQVM * pVM)
{
	EntityId objectId;
	int iInterior;

	sq_getentity(pVM, -2, &objectId);
	sq_getinteger(pVM, -1, &iInterior);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->SetInterior(objectId, iInterior);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CObjectNatives::GetInterior(SQVM * pVM)
{
	EntityId objectId;
	sq_getentity(pVM, -2, &objectId);

	if(g_pObjectManager->DoesExist(objectId)) {
		sq_pushinteger(pVM, g_pObjectManager->GetInterior(objectId));
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}