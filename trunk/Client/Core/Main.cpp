//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <windows.h>
#include <Common.h>
#include "CClient.h"

CClient * g_pClient = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread library notifications
			DisableThreadLibraryCalls(hModule);

			// Create the client instance
			g_pClient = new CClient();

			// If the client instance was created call the client OnLoad function, if
			// it wasn't, exit
			if(!g_pClient || !g_pClient->OnLoad())
			{
				// Something bad happened, exit
				ExitProcess(0);
			}
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			// Call the client OnUnload function
			g_pClient->OnUnload();

			// Delete the client instance
			SAFE_DELETE(g_pClient);
		}
		break;
	}

	return TRUE;
}
