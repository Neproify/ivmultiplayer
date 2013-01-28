//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: EventNatives.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include "../CScriptingManager.h"
#include "CEvents.h"

extern CEvents * g_pEvents;

// Event functions

void CEventNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("addEvent", Add, -1, NULL);
	pScriptingManager->RegisterFunction("callEvent", Call, -1, NULL);
	pScriptingManager->RegisterFunction("removeEvent", Remove, 2, "sc");
}

// addEvent(eventname, function, ...)
SQInteger CEventNatives::Add(SQVM * pVM)
{
	const char * szEventName;
	SQObjectPtr pFunction;
	sq_getstring(pVM, 2, &szEventName);
	pFunction = stack_get(pVM, 3);

	sq_pushbool(pVM, g_pEvents->Add(szEventName, new CSquirrelEventHandler(pVM, pFunction)));
	return 1;
}

// TODO: 'Call for this script only' argument
// callEvent(eventname, defaultretval, ...)
SQInteger CEventNatives::Call(SQVM * pVM)
{
	CHECK_PARAMS_MIN("callEvent", 2);
	CHECK_TYPE("callEvent", 1, 2, OT_STRING);

	SQInteger iTop = sq_gettop(pVM);

	// Get the name
	const char * szEventName;
	sq_getstring(pVM, 2, &szEventName);
	
	// Grab all arguments
	CSquirrelArguments pArguments;
	for(SQInteger i = 3; i <= iTop; ++ i)
		pArguments.pushFromStack(pVM, i);

	// Call the event
	CSquirrelArgument pReturn = g_pEvents->Call(szEventName, &pArguments);
	pReturn.push(pVM);
	return 1;
}

// removeEvent(eventname, function)
SQInteger CEventNatives::Remove(SQVM * pVM)
{
	const char * szEventName;
	SQObjectPtr pFunction;
	sq_getstring(pVM, -2, &szEventName);
	pFunction = stack_get(pVM, -1);

	sq_pushbool(pVM, g_pEvents->Remove(szEventName, &CSquirrelEventHandler(pVM, pFunction)));
	return 1;
}
