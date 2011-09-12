//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: TimerNatives.h
// Project: Shared
// Author(s): jenksta
//            MaVe
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterTimerNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(timer, constructor);
_MEMBER_FUNCTION_IMPL(timer, isActive);
_MEMBER_FUNCTION_IMPL(timer, kill);
_MEMBER_FUNCTION_IMPL(timer, setTraditionalBehavior);
