//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.h
// Project: Client.Core
// Author(s): FRi, XForce
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once
#include <Patcher/CPatcher.h>
#include "CGame.h"

class CCrashFixes
{
public:
	static void				Install();
	static unsigned int		GetBase() { return CGame::GetBase(); }
};