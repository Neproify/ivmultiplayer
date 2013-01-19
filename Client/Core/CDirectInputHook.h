//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDirectInputHook.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CDirectInput8Proxy.h"
#include "DXSDK/Include/XInput.h"

class CDirectInputHook
{
private:
	typedef HRESULT				 (WINAPI * DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
	typedef DWORD                (WINAPI * XInputGetState_t)(DWORD, XINPUT_STATE *);

	static DirectInput8Create_t  m_pDirectInput8Create;
	static XInputGetState_t      m_pXInputGetState;
	static bool					 m_bHookInstalled;

	static HRESULT WINAPI		 DirectInput8Create_Hook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
	static DWORD WINAPI          XInputGetState_Hook(DWORD dwUserIndex, XINPUT_STATE * pState);

public:
	static void                  Install();
	static void                  Uninstall();
};
