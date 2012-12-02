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
	// Fix for 72B240 crash ("Near bridge from Dukes to Bohan" #2)
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0x72B240), (DWORD)CRASH_FIX_72B240);

	// Fix for B2B220 crash ("Near bridge from Dukes to Bohan" #1)
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xB2B224), (DWORD)CRASH_FIX_B2B224);

	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xB21C50), (DWORD)CRASH_FIX_B21C50);

	// Fix vehicle crash
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));
	//CPatcher::InstallRetnPatch((CGame::GetBase() + 0xB21C50));
}