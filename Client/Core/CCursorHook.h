//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCursorHook.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Common.h>
#ifndef IVMP_DEBUG
#include <winsock2.h>
#include <windows.h>

class CCursorHook
{
private:
	typedef int			(WINAPI * ShowCursor_t)(BOOL);

	static ShowCursor_t m_pShowCursor;
	static bool			m_bHookInstalled;

	static int WINAPI	ShowCursor_Hook(BOOL bShow);

public:
	static void Install();
	static void Uninstall();
};
#endif
