//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.h
//===============================================================================

#pragma once

#include "SDK/SDK.h"

void event_playerSpawn(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

// Insert forward declarations for your scripting functions here
int sq_helloworld(HSQUIRRELVM pVM);