//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: EventNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterEventNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(event, constructor);
_MEMBER_FUNCTION_IMPL(event, call);
_MEMBER_FUNCTION_IMPL(event, remove);