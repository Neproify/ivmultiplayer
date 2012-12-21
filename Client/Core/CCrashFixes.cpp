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
	// Fix vehicle crash(Be carefull, we have to look if this function disables important vehicle stuff..) -> 8 xrefs
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// Deformation check
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xB7CC4F), (CGame::GetBase() + 0xB7CC68));
	// Fix some random crashes(Don't disable this function!)
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0x72B240), (CGame::GetBase() + 0x72B813));

	// Disables Warning Messages(like "Unkown resource found") -> Disables only the window(and exit code part)...
	// Note: Don't enable it, the window only appears when GTA IV fails to initialize sth...
	//CPatcher::InstallJmpPatch((CGame::GetBase() + /*0x5A932D*/0x5A8CB0), (CGame::GetBase() + 0x5A9361));

}

/*
int __thiscall sub_CBA1F0(int this, unsigned int *a2)
{
  int result; // eax@1
  unsigned int v3; // edx@1
  int v4; // esi@1

  v4 = *(_DWORD *)(this + 1536);
  v3 = *a2;
  result = 0;
  if ( v4 > 0 )
  {
    while ( v3 < *(_DWORD *)(this + 4 * result) || v3 >= *(_DWORD *)(this + 4 * result + 512) )
    {
      ++result;
      if ( result >= v4 )
        return result;
    }
    if ( result != -1 )
    {
      result = v3 + *(_DWORD *)(this + 4 * result + 1024);
      *a2 = result;
    }
  }
  return result;
}
*/