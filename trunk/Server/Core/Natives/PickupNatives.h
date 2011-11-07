//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterPickupNatives(CScriptingManager * pScriptingManager);

SQUIRREL_FUNCTION(pickup_create);
SQUIRREL_FUNCTION(pickup_delete);
SQUIRREL_FUNCTION(pickup_getmodel);
SQUIRREL_FUNCTION(pickup_gettype);
SQUIRREL_FUNCTION(pickup_setvalue);
SQUIRREL_FUNCTION(pickup_getvalue);
SQUIRREL_FUNCTION(pickup_setcoordinates);
SQUIRREL_FUNCTION(pickup_getcoordinates);
SQUIRREL_FUNCTION(pickup_setrotation);
SQUIRREL_FUNCTION(pickup_getrotation);
