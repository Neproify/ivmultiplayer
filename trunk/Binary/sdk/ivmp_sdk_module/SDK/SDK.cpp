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

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate)
{
	sq_pushroottable(pVM);
	sq_pushstring(pVM, szName, -1);
	sq_newclosure(pVM, pfnFunction, 0);

	if(iParameterCount != -1)
	{
		String strTypeMask;
		if(strFunctionTemplate.IsNotEmpty())
			strTypeMask.Format(".%s", strFunctionTemplate.Get());
		sq_setparamscheck(pVM, (iParameterCount + 1), strTypeMask.Get());
	}

	sq_createslot(pVM, -3);
	sq_pop(pVM, 1);
}

void RegisterConstant(HSQUIRRELVM pVM, const char *szName, int value)
{
	sq_pushstring(pVM, szName, -1);
	sq_pushinteger(pVM, value);
	sq_createslot(pVM, -3);
}
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, bool value)
{
	sq_pushstring(pVM, szName, -1);
	sq_pushbool(pVM, value);
	sq_createslot(pVM, -3);
}
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, const char* value, int len)
{
	sq_pushstring(pVM, szName, -1);
	sq_pushstring(pVM, value, len);
	sq_createslot(pVM, -3);
}
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, float value)
{
	sq_pushstring(pVM, szName, -1);
	sq_pushfloat(pVM, value);
	sq_createslot(pVM, -3);
}
