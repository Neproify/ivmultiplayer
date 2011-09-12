//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
//
// File: Natives.h
// Project: Server
// Author(s): jenksta
//            Einstein
//            Sebihunter
//
// Unless specified above, The copyright to the contents herein is the
// property of the IV:MP Team. The contents may be used and/or copied only
// with the written permission of the current IV:MP Developer(s).
//
//===============================================================================

// TODO: Add error checking for ALL functions.
// TODO: Only allow 1 and 0 with as integer boolean types.
// TODO: MODEL_ auto defines.
// TODO: OT_PLAYER, OT_VEHICLE, OT_OBJECT so they all have their own types.
// TODO: CServerNatives class, CPlayerNatives class, CVehicleNatives class, CObjectNatives class, e.t.c
// TODO: SQ_RETURN(ret, val) which does (sq_pushbool(pVM, val); return ret;)

#pragma once

#include "../CScriptingManager.h"
#include "../../../Vendor/Squirrel/sqstdio.h"
#include "../../CLogFile.h"
#include "../../Common.h"

#define SQUIRREL_FUNCTION(name) SQInteger sq_##name(SQVM * pVM)

// TODO: PARAM_ERROR define, Then CHECK_PARAMS should use the PARAM_ERROR define
#define CHECK_PARAMS(szNativeName, iAmount) \
	{ \
		int iTop = sq_gettop(pVM) - 1; \
		if(iTop != iAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d, Amount: %d).", iAmount, iTop); \
			sq_pushbool(pVM, false); \
			return 1; \
		} \
	}

#define CHECK_PARAMS_MIN(szNativeName, iMinAmount) \
	{ \
		int iTop = sq_gettop(pVM) - 1; \
		if(iTop < iMinAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d+, Amount: %d).", iMinAmount, iTop); \
			sq_pushbool(pVM, false); \
			return 1; \
		} \
	}

#define CHECK_PARAMS_MIN_MAX(szNativeName, iMinAmount, iMaxAmount) \
	{ \
		int iTop = sq_gettop(pVM) - 1; \
		if(iTop < iMinAmount || iTop > iMaxAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d+, Amount: %d).", iMinAmount, iTop); \
			sq_pushbool(pVM, false); \
			return 1; \
		} \
	}

// TODO: TYPE_ERROR define, Then CHECK_TYPE should use the TYPE_ERROR define
// TODO: Give the type required and the type given in the log message (Types should be a string)
#define CHECK_TYPE(szNativeName, iParam, iIndex, iTypeRequired) \
	{ \
		int iType = sq_gettype(pVM, iIndex); \
		if(iType != iTypeRequired) \
		{ \
			if(!(iTypeRequired == OT_BOOL && iType == OT_INTEGER) && !(iTypeRequired == OT_FLOAT && iType == OT_INTEGER)) /* Allow integers for booleans and integers for floats */ \
			{ \
				CLogFile::Printf("Invalid parameter type for function " szNativeName " parameter %d.", iParam); \
				sq_pushbool(pVM, false); \
				return 1; \
			} \
		} \
	}

static void sq_pushentity(SQVM * pVM, EntityId entity)
{
	sq_pushinteger(pVM, (SQInteger)entity);
}

static SQRESULT sq_getentity(SQVM * pVM, SQInteger iIndex, EntityId * entity)
{
	SQInteger i;
	SQRESULT res = sq_getinteger(pVM, iIndex, &i);

	if(entity)
		*entity = (EntityId)i;

	return res;
}

static SQRESULT sq_getvector3(SQVM * pVM, SQInteger iIndex, CVector3 * pVector)
{
	sq_getfloat(pVM, iIndex, (SQFloat *)&(pVector->fX));
	sq_getfloat(pVM, iIndex + 1, (SQFloat *)&(pVector->fY));
	sq_getfloat(pVM, iIndex + 2, (SQFloat *)&(pVector->fZ));
	return SQ_OK;
}

// Natives

#include "EventNatives.h"
#include "AreaNatives.h"
#include "XMLNatives.h"
#include "SQLiteNatives.h"
#include "TimerNatives.h"
#include "HashNatives.h"
#include "WorldNatives.h"
