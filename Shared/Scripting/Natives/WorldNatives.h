//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: WorldNatives.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterWorldNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(world, setTime);
_MEMBER_FUNCTION_IMPL(world, getTime);
_MEMBER_FUNCTION_IMPL(world, setMinuteDuration);
_MEMBER_FUNCTION_IMPL(world, getMinuteDuration);
_MEMBER_FUNCTION_IMPL(world, setDayOfWeek);
_MEMBER_FUNCTION_IMPL(world, getDayOfWeek);
