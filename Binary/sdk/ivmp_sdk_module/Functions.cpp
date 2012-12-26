//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

int sq_helloworld(HSQUIRRELVM pVM)
{
	printf("Hello world from a module!");
	sq_pushbool(pVM, true);
	return 1;
}