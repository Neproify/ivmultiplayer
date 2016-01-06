//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
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

// Event functions
_BEGIN_CLASS(event)
_MEMBER_FUNCTION(event, constructor, -1, NULL)
_MEMBER_FUNCTION(event, call, -1, NULL)
_MEMBER_FUNCTION(event, remove, 2, "sc")
_END_CLASS(event)

void RegisterEventNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(event));
}

// add(eventName, function, ...)
_MEMBER_FUNCTION_IMPL(event, constructor)
{
	const char * szEventName;
	SQObjectPtr pFunction;
	sq_getstring(pVM, 2, &szEventName);
	pFunction = stack_get(pVM, 3);

	sq_pushbool(pVM, CEvents::GetInstance()->Add(szEventName, new CSquirrelEventHandler(pVM, pFunction)));
	return 1;
}

// call(eventname, defaultretval, ...)
_MEMBER_FUNCTION_IMPL(event, call)
{
	CHECK_PARAMS_MIN("callEvent", 2);
	CHECK_TYPE("callEvent", 1, 2, OT_STRING);

	SQInteger iTop = sq_gettop(pVM);

	// Get the name
	const char * szEventName;
	sq_getstring(pVM, 2, &szEventName);

	// Grab all arguments
	CSquirrelArguments pArguments;
	for (SQInteger i = 3; i <= iTop; ++i)
		pArguments.pushFromStack(pVM, i);

	// Call the event
	CSquirrelArgument pReturn = CEvents::GetInstance()->Call(szEventName, &pArguments);
	pReturn.push(pVM);
	return 1;
}

// remove(eventname, function)
_MEMBER_FUNCTION_IMPL(event, remove)
{
	const char * szEventName;
	SQObjectPtr pFunction;
	sq_getstring(pVM, -2, &szEventName);
	pFunction = stack_get(pVM, -1);

	sq_pushbool(pVM, CEvents::GetInstance()->Remove(szEventName, &CSquirrelEventHandler(pVM, pFunction)));
	return 1;
}