//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Input.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>

LRESULT APIENTRY WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
String GetKeyNameByCode(DWORD dwCode);
