//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Input.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <algorithm>
#include <list>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include "Input.h"
#include "CChatWindow.h"
#include "CInputWindow.h"
#include "CGame.h"
#include "Scripting.h"
#include "CLocalPlayer.h"
#include "CGUI.h"
#include "CMainMenu.h"
#include "Scripting/CScriptingManager.h"
#include <CEvents.h>
#include <SharedUtility.h>
#include "CScreenShot.h"

WNDPROC           m_wWndProc = NULL;
std::list<String> pressedKeys;

extern CChatWindow       * g_pChatWindow;
extern CInputWindow      * g_pInputWindow;
extern CLocalPlayer      * g_pLocalPlayer;
extern CScriptingManager * g_pScriptingManager;
extern CGUI              * g_pGUI;
extern CMainMenu         * g_pMainMenu;
extern CEvents           * g_pEvents;

String GetKeyNameByCode(DWORD dwCode)
{
	String strCode;

	if(dwCode >= 0x30 && dwCode <= 0x39)
	{
		// Number 0-9
		strCode = (unsigned char)dwCode;
	}
	else if(dwCode >= 0x41 && dwCode <= 0x5A)
	{
		// Lower case ASCII a-z
		strCode = (unsigned char)(dwCode + 0x20);
	}
	else if(dwCode >= VK_NUMPAD0 && dwCode <= VK_NUMPAD9)
	{
		// Numpad 0-9
		strCode.Format("num_%d", (dwCode - VK_NUMPAD0));
	}
	else if(dwCode >= VK_F1 && dwCode <= VK_F12)
	{
		// Function 1-12
		strCode.Format("f%d", (dwCode - VK_F1 + 1));
	}
	else
	{
		// Other keys
		switch(dwCode)
		{
		case VK_TAB:
			strCode = "tab"; break;
		case VK_RETURN:
			strCode = "enter"; break;
		case VK_CONTROL:
			strCode = "ctrl"; break;
		//case VK_CONTROL: // fix
		//	strCode = "lcontrol"; break;
		//case VK_CONTROL: // fix
		//	strCode = "rcontrol"; break;
		case VK_SHIFT:
			strCode = "shift"; break;
		//case VK_SHIFT:
		//	strCode = "lshift"; break;
		//case VK_SHIFT: // fix
		//	strCode = "rshift"; break;
		case VK_MENU:
			strCode = "alt"; break;
		//case VK_MENU: // fix
		//	strCode = "lalt"; break;
		//case VK_MENU: // fix
		//	strCode = "ralt"; break;
		case VK_MULTIPLY:
			strCode = "num_mul"; break;
		case VK_ADD:
			strCode = "num_add"; break;
		case VK_SUBTRACT:
			strCode = "num_sub"; break;
		case VK_DECIMAL:
			strCode = "num_dec"; break;
		case VK_DIVIDE:
			strCode = "num_div"; break;
		case VK_SPACE:
			strCode = "space"; break;
		case VK_LEFT:
			strCode = "arrow_left"; break;
		case VK_RIGHT:
			strCode = "arrow_right"; break;
		case VK_UP:
			strCode = "arrow_up"; break;
		case VK_DOWN:
			strCode = "arrow_down"; break;
		case VK_PRIOR:
			strCode = "page_up"; break;
		case VK_NEXT:
			strCode = "page_down"; break;
		case VK_END:
			strCode = "end"; break;
		case VK_HOME:
			strCode = "home"; break;
		case VK_INSERT:
			strCode = "insert"; break;
		case VK_DELETE:
			strCode = "delete"; break;
		case 0x1E:
			strCode = "backspace"; break;
		case 0x01:
			strCode = "esc"; break;
		case VK_CAPITAL:
			strCode = "capslock"; break;
		case VK_PAUSE:
			strCode = "pause"; break;
		// German "umlaute"
		case 0xC4:
			strCode = "ä"; break;
		case 0xE4:
			strCode = "ä"; break;
		case 0xD4:
			strCode = "ö"; break;
		case 0xF4:
			strCode = "ö"; break;
		case 0xDC:
			strCode = "ü"; break;
		case 0xFC:
			strCode = "ü"; break;
		}
	}

	return strCode;
}

LRESULT APIENTRY WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Are we focused?
	// jenksta: This is bad, but the only way i could get it working
	bool bFocused = (GetForegroundWindow() == hWnd);

	// Have we gained focus?
	if(bFocused && !CGame::IsFocused())
	{
		// Set the game focused flag
		CGame::SetFocused(true);

		// Show the cursor
		ShowCursor(true);
		CLogFile::Print("Gained window focus");
		return 1;
	}
	// Have we lost focus?
	else if(!bFocused && CGame::IsFocused())
	{
		// Set the game focused flag
		CGame::SetFocused(false);

		// Hide the cursor
		ShowCursor(false);
		CLogFile::Print("Lost window focus");
		return 1;
	}

	// Are we focused?
	if(bFocused)
	{
		// Pass the input to the GUI
		if(g_pGUI && g_pGUI->MsgProc(hWnd, uMsg, wParam, lParam))
			return 1;

		// Is this a F12 key up?
		if(uMsg == WM_KEYUP && wParam == VK_F12)
		{
			if(g_pMainMenu)
			{
				if(!g_pMainMenu->IsVisible())
				{
					if(g_pInputWindow && !g_pInputWindow->IsEnabled())
					{
						if(CGame::GetState() == GAME_STATE_INGAME)
						{
							CGame::SetState(GAME_STATE_IVMP_PAUSE_MENU);
							CGame::SetInputState(false);
							return 1;
						}
					}
				}
				else
				{
					if(CGame::GetState() == GAME_STATE_IVMP_PAUSE_MENU)
					{
						CGame::SetState(GAME_STATE_INGAME);
						CGame::SetInputState(true);
						return 1;
					}
				}
			}
		}

		// Is this a F10 key up?
		if(uMsg == WM_SYSKEYUP && wParam == VK_F10)
		{
			// Take a screen shot
			if(CScreenShot::Take())
				g_pChatWindow->AddInfoMessage("Screen shot captured.");
			else
			{
				g_pChatWindow->AddInfoMessage("Screen shot capture failed (%s).", CScreenShot::GetError().Get());
				CScreenShot::Reset();
			}
		}

		// Is this a F7 key up?
		if(uMsg == WM_KEYUP && wParam == VK_F7)
		{
			if(g_pChatWindow)
			{
				if(g_pChatWindow->IsEnabled())
					g_pChatWindow->SetEnabled(false);
				else
					g_pChatWindow->SetEnabled(true);
			}
		}

		// Are we in game?
		if(CGame::GetState() == GAME_STATE_INGAME)
		{
			// Is this a Page Up key up?
			if(uMsg == WM_KEYUP && wParam == VK_PRIOR)
			{
				// Move the chat window page upwards
				g_pChatWindow->PageUp();
				return 1;
			}

			// Is this a Page Down key up?
			if(uMsg == WM_KEYUP && wParam == VK_NEXT)
			{
				// Move the chat window page downwards
				g_pChatWindow->PageDown();
				return 1;
			}

			// If the input window exists pass the input to it
			if(g_pInputWindow)
			{
				bool bInputEnabled = g_pInputWindow->IsEnabled();

				if(g_pInputWindow->HandleInput(uMsg, (DWORD)wParam))
				{
					// Have we just enabled the chat window?
					if(!bInputEnabled && g_pInputWindow->IsEnabled())
					{
						// Clear all keys
						for(std::list<String>::iterator iter = pressedKeys.begin(); pressedKeys.size() > 0; iter = pressedKeys.begin())
						{
							CSquirrelArguments pArguments;
							pArguments.push(*iter);
							pArguments.push("up");
							g_pEvents->Call("keyPress", &pArguments);
							pressedKeys.erase(iter);
						}

						return 1;
					}
				}
			}

			// Is the input window disabled?
			if(g_pInputWindow && !g_pInputWindow->IsEnabled())
			{
				String strCode = GetKeyNameByCode((DWORD)wParam);

				if(strCode.IsNotEmpty())
				{
					String strState;

					if(uMsg == WM_KEYUP)
					{
						// Check if key was pressed beforehand
						if(std::find(pressedKeys.begin(), pressedKeys.end(), strCode) != pressedKeys.end())
						{
							pressedKeys.remove(strCode);
							strState = "up";
						}
					}
					else if(uMsg == WM_KEYDOWN)
					{
						// Check if key was pressed beforehand
						if(std::find(pressedKeys.begin(), pressedKeys.end(), strCode) == pressedKeys.end())
						{
							pressedKeys.push_back(strCode);
							strState = "down";
						}
					}

					if(strState.IsNotEmpty() && g_pEvents)
					{
						CSquirrelArguments pArguments;
						pArguments.push(strCode);
						pArguments.push(strState);
						g_pEvents->Call("keyPress", &pArguments);
					}
				}
			}
		}
	}

	return CallWindowProc(m_wWndProc, hWnd, uMsg, wParam, lParam);
}
