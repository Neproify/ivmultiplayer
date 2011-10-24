//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: TimerNatives.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include "../CSquirrel.h"
#include "../CScriptingManager.h"
#include "../CScriptTimerManager.h"
#include "../CSquirrelArguments.h"
#include "../CScriptTimer.h"

extern CScriptTimerManager * g_pScriptTimerManager;
extern CScriptingManager * g_pScriptingManager;

// Timer functions
_BEGIN_CLASS(timer)
_MEMBER_FUNCTION(timer, constructor, -1, NULL)
_MEMBER_FUNCTION(timer, isActive, 0, NULL)
_MEMBER_FUNCTION(timer, kill, 0, NULL)
_MEMBER_FUNCTION(timer, setTraditionalBehavior, 1, "b")
_END_CLASS(timer)

void RegisterTimerNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(timer));
}

_MEMBER_FUNCTION_RELEASE_HOOK(timer)
{
	CScriptTimer * pTimer = (CScriptTimer *)pInst;

	if(pTimer != NULL && g_pScriptTimerManager->contains(pTimer))
		pTimer->Kill();

	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, constructor)
{
	CHECK_PARAMS_MIN("timer", 3);

	if(sq_gettype(pVM, 2) != OT_NATIVECLOSURE)
		CHECK_TYPE("timer", 1, 2, OT_CLOSURE);

	CHECK_TYPE("timer", 2, 3, OT_INTEGER);
	CHECK_TYPE("timer", 3, 4, OT_INTEGER);

	SQInteger interval = 0;
	SQInteger repeations = 0;
	sq_getinteger(pVM, 3, &interval);
	sq_getinteger(pVM, 4, &repeations);

	// Ensure we have a valid repeations value
	if(repeations < -1 || repeations == 0)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	SQObjectPtr pFunction;
	pFunction = stack_get(pVM, 2);
	CSquirrelArguments * pArguments = new CSquirrelArguments(pVM, 5);

	CScriptTimer * pTimer = new CScriptTimer(g_pScriptingManager->Get(pVM), pFunction, interval, repeations, pArguments);

	if(SQ_FAILED(sq_setinstance(pVM, pTimer)))
	{
		delete pTimer;
		delete pArguments;
		sq_pushbool(pVM, false);
		return 1;
	}

	g_pScriptTimerManager->push_back(pTimer);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, isActive)
{
	CScriptTimer * pTimer = sq_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	if(g_pScriptTimerManager->contains(pTimer))
	{
		if(pTimer->IsDead())
		{
			sq_setinstance(pVM, NULL);
			sq_pushbool(pVM, false);
			return 1;
		}

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, kill)
{
	CScriptTimer * pTimer = sq_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	if(g_pScriptTimerManager->contains(pTimer) && !pTimer->IsDead())
	{
		pTimer->Kill();
		sq_pushbool(pVM, true);
		sq_setinstance(pVM, NULL);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, setTraditionalBehavior)
{
	CScriptTimer * pTimer = sq_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	if(pTimer != 0 && g_pScriptTimerManager->contains(pTimer) && !pTimer->IsDead())
	{
		bool b;
		sq_getbool(pVM, 2, (SQBool*)&b);
		pTimer->SetTraditionalBehavior(b);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
