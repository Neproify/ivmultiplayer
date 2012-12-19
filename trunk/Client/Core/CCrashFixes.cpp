//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#include <Patcher/CPatcher.h>
#include "CCrashFixes.h"
#include "CGame.h"

void CCrashFixes::Install()
{
	// Fix vehicle crash
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// Fix some random crashes
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x72B240), (CGame::GetBase() + 0x72B813));

	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x5A932D), (CGame::GetBase() + 0x5A9361));

}