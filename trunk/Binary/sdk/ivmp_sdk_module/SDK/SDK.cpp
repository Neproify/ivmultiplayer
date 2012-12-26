//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: SDK.cpp
//===============================================================================

#include "SDK.h"

InterfaceContainer_t InterfaceContainer;
NewInterfaceContainer_t NewInterfaceContainer;
FuncContainer_t FuncContainer;

EXPORT void SetupFunctions(FuncContainer_t * pContainer)
{
	FuncContainer = *pContainer;
}

EXPORT void SetupInterfaces(InterfaceContainer_t * pContainer)
{
	InterfaceContainer = *pContainer;
}

EXPORT void SetupNewInterfaces(NewInterfaceContainer_t * pContainer)
{
	NewInterfaceContainer = *pContainer;
}

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction)
{
	sq_pushroottable(pVM);
	sq_pushstring(pVM, szName, -1);
	sq_newclosure(pVM, pfnFunction, 0);
	sq_createslot(pVM, -3);
	sq_pop(pVM, 1);
}
