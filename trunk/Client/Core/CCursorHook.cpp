//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCursorHook.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCursorHook.h"
#include "Patcher/CPatcher.h"
#include <CLogFile.h>

CCursorHook::ShowCursor_t CCursorHook::m_pShowCursor;
bool CCursorHook::m_bHookInstalled = false;

int WINAPI CCursorHook::ShowCursor_Hook(BOOL bShow)
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	//CLogFile::Printf("ShowCursor(%d)", bShow);

	/*if(m_bWindowFocused)
	{
		SetCursor(NULL);
		return m_pShowCursor(false);
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return m_pShowCursor(true);
	}*/

	SetCursor(NULL);
	return m_pShowCursor(false);

	return 0;
}

void CCursorHook::Install()
{
	if(!m_bHookInstalled)
	{
		m_pShowCursor = (ShowCursor_t)CPatcher::InstallDetourPatch("User32.dll", "ShowCursor", (DWORD)ShowCursor_Hook);
		CLogFile::Printf("Hooked 'ShowCursor' in 'User32.dll' (0x%p)", m_pShowCursor);
		m_bHookInstalled = true;
	}
}

void CCursorHook::Uninstall()
{
	if(m_bHookInstalled)
	{
		if(m_pShowCursor)
			CPatcher::UninstallDetourPatch((PBYTE)m_pShowCursor, (DWORD)ShowCursor_Hook);

		m_bHookInstalled = false;
	}
}