//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: TrafficLightsNatives.h
// Project: Shared
// Author(s): mabako, Neproify
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterTrafficLightsNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(trafficLights, setState);
_MEMBER_FUNCTION_IMPL(trafficLights, getState);
_MEMBER_FUNCTION_IMPL(trafficLights, setLocked);
_MEMBER_FUNCTION_IMPL(trafficLights, areLocked);
_MEMBER_FUNCTION_IMPL(trafficLights, setPhaseDuration);
_MEMBER_FUNCTION_IMPL(trafficLights, getPhaseDuration);
_MEMBER_FUNCTION_IMPL(trafficLights, resetPhaseDuration);