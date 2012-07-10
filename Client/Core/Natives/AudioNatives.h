//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AudioNatives.h
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

#include "../Natives.h"

//Audio
_MEMBER_FUNCTION_IMPL(Audio, constructor);
_MEMBER_FUNCTION_IMPL(Audio, deleteSound);
_MEMBER_FUNCTION_IMPL(Audio, play);
_MEMBER_FUNCTION_IMPL(Audio, stop);
_MEMBER_FUNCTION_IMPL(Audio, pause);
_MEMBER_FUNCTION_IMPL(Audio, isStarted);
_MEMBER_FUNCTION_IMPL(Audio, setVolume);
_MEMBER_FUNCTION_IMPL(Audio, getVolume);
_MEMBER_FUNCTION_IMPL(Audio, restoreVolume);
_MEMBER_FUNCTION_IMPL(Audio, setPosition);
_MEMBER_FUNCTION_IMPL(Audio, clearPosition);
_MEMBER_FUNCTION_IMPL(Audio, usePositionSystem);