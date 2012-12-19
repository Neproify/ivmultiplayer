//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDirect3DHook.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once


#include <DXSDK\Include\d3d9.h>
#include <DXSDK\Include\d3dx9.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")

class CDirect3DHook
{
private:
	typedef HRESULT (__stdcall* Reset_t)(LPDIRECT3DDEVICE9,D3DPRESENT_PARAMETERS*);
	typedef HRESULT (__stdcall* EndScene_t)(LPDIRECT3DDEVICE9);

	static Reset_t				m_pReset;
	static EndScene_t			m_pEndScene;

	static bool					m_bHookInstalled;
	static bool					m_bInitialized;

	static HRESULT WINAPI		hkEndScene(LPDIRECT3DDEVICE9 pDevice);
	static HRESULT WINAPI		hkReset(LPDIRECT3DDEVICE9 pDevice,D3DPRESENT_PARAMETERS* pPresentationParameters);
	

public:
	static void Install();
	static void Uninstall();
};
