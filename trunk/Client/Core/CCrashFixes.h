//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.h
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

class CCrashFixes
{
private:

 static unsigned long CRASH_FIX_9CCF40_JMP;

 static unsigned long CRASH_FIX_8AB200_JMP;
public:
	static void Install();
};