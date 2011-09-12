//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterObjectNatives(CScriptingManager * pScriptingManager);

SQUIRREL_FUNCTION(object_create);
SQUIRREL_FUNCTION(object_delete);
SQUIRREL_FUNCTION(object_getmodel);
SQUIRREL_FUNCTION(object_getcoordinates);
SQUIRREL_FUNCTION(object_setcoordinates);
SQUIRREL_FUNCTION(object_getrotation);
SQUIRREL_FUNCTION(object_setrotation);
