//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: HashNatives.h
// Project: Shared
// Author(s): Einstein, Neproify
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterHashNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(hash, md5);
_MEMBER_FUNCTION_IMPL(hash, md5File);