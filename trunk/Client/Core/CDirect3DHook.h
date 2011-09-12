//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDirect3DHook.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CDirect3D9Proxy.h"

class CDirect3DHook
{
private:
	typedef IDirect3D9 *	   (WINAPI * Direct3DCreate9_t)(UINT);

	static Direct3DCreate9_t   m_pDirect3DCreate9;
	static bool				   m_bHookInstalled;

	static IDirect3D9 * WINAPI Direct3DCreate9_Hook(UINT SDKVersion);

public:
	static void Install();
	static void Uninstall();
};
