//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <Common.h>
#include "CServer.h"

CServer * g_pServer = NULL;

EXPORT CServerInterface * GetServerInterface()
{
	SAFE_DELETE(g_pServer);
	g_pServer = new CServer();
	return g_pServer;
}

EXPORT void DestroyServerInterface(CServerInterface * pServer)
{
	SAFE_DELETE(g_pServer);
}

#ifdef WIN32
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, void *)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread calls
			DisableThreadLibraryCalls(hModule);
		}
		break;
	}

	return TRUE;
}
#endif
