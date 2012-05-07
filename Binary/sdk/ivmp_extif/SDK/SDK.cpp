//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: SDK.cpp
//===============================================================================

#include "SDK.h"

#include <Squirrel/sqstdio.h>
#include <Squirrel/sqstdaux.h>
#include <Squirrel/sqstdblob.h>
#include <Squirrel/sqstdmath.h>
#include <Squirrel/sqstdstring.h>
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>

InterfaceContainer_t InterfaceContainer;

EXPORT void SetupFunctions(void * pContainer)
{
	// Not used for extif module
}

EXPORT void SetupInterfaces(InterfaceContainer_t * pContainer)
{
	InterfaceContainer = *pContainer;
}

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction)
{
	sq_pushroottable(pVM);
	sq_pushstring(pVM, (SQChar*)szName, -1);
	sq_newclosure(pVM, pfnFunction, 0);
	sq_createslot(pVM, -3);
	sq_pop(pVM, 1);
}

void CallFuction()
{

}