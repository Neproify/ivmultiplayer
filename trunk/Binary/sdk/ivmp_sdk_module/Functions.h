//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.h
//===============================================================================

#pragma once

#include "SDK/SDK.h"

// Here forward declarations for events handlers for this module:
void event_playerSpawn(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk);

// Insert forward declarations for your scripting functions here
int sq_helloworld(HSQUIRRELVM pVM);
int sq_createVehicleEx(HSQUIRRELVM pVM);