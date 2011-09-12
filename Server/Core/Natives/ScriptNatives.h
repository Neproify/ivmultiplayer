//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterScriptNatives(CScriptingManager * pScriptingManager);

SQUIRREL_FUNCTION(server_getscripts);
SQUIRREL_FUNCTION(server_getclientscripts);
SQUIRREL_FUNCTION(server_getclientresources);
SQUIRREL_FUNCTION(server_getscriptname);
SQUIRREL_FUNCTION(server_loadscript);
SQUIRREL_FUNCTION(server_unloadscript);
SQUIRREL_FUNCTION(server_reloadscript);
SQUIRREL_FUNCTION(server_loadclientscript);
SQUIRREL_FUNCTION(server_unloadclientscript);
SQUIRREL_FUNCTION(server_reloadclientscript);
SQUIRREL_FUNCTION(server_loadclientresource);
SQUIRREL_FUNCTION(server_unloadclientresource);
SQUIRREL_FUNCTION(server_reloadclientresource);
