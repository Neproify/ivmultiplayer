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
#include "CClient.h"
#include "CGame.h"
#include "Scripting.h"
#include "CGUI.h"
#include <SharedUtility.h>
#include "CScreenShot.h"
#include "CGame.h"
#include "IV/CIVTrain.h"
#include "Patcher/CPatcher.h"

WNDPROC           m_wWndProc = NULL;
std::list<String> pressedKeys;

extern CClient * g_pClient;

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
		case VK_BACK:
			strCode = "backspace"; break;
		case VK_ESCAPE:
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
	bool bFocused = (GetForegroundWindow() == hWnd);

	// Have we gained or lost focus recently?
	if(bFocused != CGame::IsFocused())
	{
		// Update game focused flag
		CGame::SetFocused(bFocused);

		// (John) - why is this needed?
		CActorManager * pActorManager = g_pClient->GetActorManager();
		if(pActorManager)
			pActorManager->bGameFocused = bFocused;

		ShowCursor(bFocused);

		if(!g_pClient->GetChatWindow())
				g_pClient->GetChatWindow()->SetEnabled(g_pClient->GetChatWindow()->IsEnabled());

	}

	// Are we focused?
	// (John) - This should all be moved elsewhere..
	if(bFocused)
	{
		// Get our input window and local player
		CInputWindow * pInputWindow = g_pClient->GetInputWindow();
		CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

		// Get our GUI instance
		CGUI * pGUI = g_pClient->GetGUI();

		// Pass the input to the GUI
		if(pGUI && pGUI->MsgProc(hWnd, uMsg, wParam, lParam))
			return 1;

		CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();

		if(pNetworkManager)
		{
			// Is this a F12 key up?
			if(CGame::IsGameLoaded() && !CGame::IsKickedFromServer())
			{
				if(uMsg == WM_KEYUP && wParam == VK_F12)
				{
					// Get our main menu
					CMainMenu * pMainMenu = g_pClient->GetMainMenu();

					if(pMainMenu)
					{
						if(!pMainMenu->IsVisible())
						{
							if(pInputWindow && !pInputWindow->IsEnabled())
							{
								if(CGame::GetState() == GAME_STATE_INGAME)
								{
									CGame::SetState(GAME_STATE_IVMP_PAUSE_MENU);
									CGame::SetInputState(false);
									g_pClient->GetChatWindow()->SetEnabled(false);

									// jenksta: TODO client CPlayerManager::GetPlayerCount
									int players = 0;
									for(int i = 0; i < MAX_PLAYERS; i++)
									{
										if(g_pClient->GetPlayerManager()->DoesExist(i))
											players++;
									}

									if(pNetworkManager->IsConnected())
										pMainMenu->SetNetworkStats(pNetworkManager->GetHostName(),players,pNetworkManager->GetMaxPlayers(),pLocalPlayer->GetName());

									CGame::SetRadarVisible(false);
									return 1;
								}
							}
						}
						else
						{
							if(CGame::GetState() == GAME_STATE_IVMP_PAUSE_MENU)
							{
								CGame::SetState(GAME_STATE_INGAME);
								CGame::SetInputState(pLocalPlayer->GetControl());
								g_pClient->GetChatWindow()->SetEnabled(true);

								if(pLocalPlayer && !pLocalPlayer->IsRadarVisible())
									CGame::SetRadarVisible(false);
								else
									CGame::SetRadarVisible(true);

								return 1;
							}
						}
					}
				}
			}
		}

		// Is this a F10 key up?
		if(uMsg == WM_SYSKEYUP && wParam == VK_F10)
		{
			// Take a screen shot
			if(CScreenShot::Take())
				g_pClient->GetChatWindow()->AddInfoMessage("Screen shot captured.");
			else
			{
				g_pClient->GetChatWindow()->AddInfoMessage("Screen shot capture failed (%s).", CScreenShot::GetError().Get());
				CScreenShot::Reset();
			}
		}
		if(uMsg == WM_KEYUP && wParam == VK_F3)
		{
			/* ViruZz: Temporary disabled to avoid crash issues
			IVVehicle * pGameVehicle = pLocalPlayer->GetVehicle()->GetGameVehicle()->GetVehicle();
			*(BYTE *)(pGameVehicle + 0xF71) |= 1;
			*((BYTE *)pGameVehicle + 3953) = *((BYTE *)pGameVehicle + 3953) & 0xFE | 2;*/
			return 1;
		}
		/*
		if(uMsg == WM_KEYUP && wParam == VK_F4)
		{
			IVVehicle *  pVehicle = g_pLocalPlayer->GetVehicle()->GetGameVehicle()->GetVehicle();
			DWORD dwTest = *(DWORD *)(pVehicle + 0x194);

			//int v1 = sub_426700((void *)VAR_VehiclePool, **(_DWORD **)(a1 + 8));
			g_pClient->GetChatWindow()->AddInfoMessage("Testvar 0x%p(%x)%d",dwTest,dwTest,dwTest);
			CLogFile::Printf("Testvar 0x%p(%x)%d",dwTest,dwTest,dwTest);
		}*/
		
		//if(uMsg == WM_KEYUP && wParam == VK_F4)
		//{
		//	DWORD dwPointer;
		//	//dwPointer = *(DWORD*)((DWORD)GetModuleHandle("d3d9.dll")+0x165A40);
		//	//CPatcher::Unprotect((CGame::GetBase() + 0x1716C2C),5);
		//	//D3DXMATRIX dwMatrix = *(D3DXMATRIX*)(CGame::GetBase() + (/*0x1716C2C + 0x9A0));
		//	//CLogFile::Printf("MATRIX %x(%p)",dwMatrix,dwMatrix);
		//	//CLogFile::Printf("MATRIX (%f,%f,%f,%f,%f)",dwMatrix._11,dwMatrix._12,dwMatrix._13,dwMatrix._14,dwMatrix._21);

		//	//CVector3 vecPos = g_pActorManager->GetPosition(1);
		//	//CVector3 vecScreen;
		//	//g_pGraphics->GetScreenPositionFromWorldPosition(vecPos, &vecScreen);
		//	//CLogFile::Printf("Coords: %f, %f, %f",vecScreen.fX,vecScreen.fY,vecScreen.fZ);
		//	// Get the player position + add z coord
		//	CVector3 vecWorldPosition = g_pActorManager->GetPosition(0);
		//	//vecWorldPosition.fZ += 1.0f;

		//	// Check if he's on the screen
		//	CVector3 vecScreen;
		//	g_pGraphics->GetScreenPositionFromWorldPosition(vecWorldPosition,&vecScreen);
		//	g_pClient->GetChatWindow()->AddInfoMessage("Z: %f",vecScreen.fZ);
		//}

		/*if(uMsg == WM_KEYUP && wParam == VK_F2)
		{
			//CVector3 vecPos; g_pLocalPlayer->GetPosition(vecPos);
			//unsigned int uiObjectHandle;
			//Scripting::CreateObject((Scripting::eModel)2999442604, vecPos.fX,vecPos.fY,vecPos.fZ,&uiObjectHandle,true);
			//Scripting::TaskHoldObject(g_pLocalPlayer->GetScriptingHandle(),uiObjectHandle);

			CLogFile::Printf("TRACK QUEEN");
            DWORD t1 = *(DWORD *)(CGame::GetBase() + 0x15BE4BC);
            DWORD t2 = *(DWORD *)(CGame::GetBase() + 0x15BE48C);
            DWORD t3 = *(DWORD *)(CGame::GetBase() + 0x15BD4D8);
            DWORD t4 = *(DWORD *)(CGame::GetBase() + 0x15BD908);
            DWORD t5 = *(DWORD *)(CGame::GetBase() + 0x15BE4F0);
            DWORD t6 = *(DWORD *)(CGame::GetBase() + 0x15BDD00);
            BYTE t7 = *(BYTE *)(CGame::GetBase() + 0x15BD3E8);
            DWORD t8 = *(DWORD *)(CGame::GetBase() + 0x15BE0C8);
            DWORD t9 = *(DWORD *)(CGame::GetBase() + 0x15BD940);
			CLogFile::Printf("AT 0x15BE4BC: 0x(%p)",t1);
			CLogFile::Printf("AT 0x15BE48C: 0x(%p)",t2);
			CLogFile::Printf("AT 0x15BD4D8: 0x(%p)",t3);
			CLogFile::Printf("AT 0x15BD908: 0x(%p)",t4);
			CLogFile::Printf("AT 0x15BE4F0: 0x(%p)",t5);
			CLogFile::Printf("AT 0x15BDD00: 0x(%p)",t6);
			CLogFile::Printf("AT 0x15BD3E8: 0x(%p)",t7);
			CLogFile::Printf("AT 0x15BE0C8: 0x(%p)",t8);
			CLogFile::Printf("AT 0x15BD940: 0x(%p)",t9);

			CLogFile::Printf("TRACK QUEEN2");
            t1 = *(DWORD *)(CGame::GetBase() + 0x15BE4C0);
            t2 = *(DWORD *)(CGame::GetBase() + 0x15BE490);
			t3 = *(DWORD *)(CGame::GetBase() + 0x15BD4DC);
            t4 = *(DWORD *)(CGame::GetBase() + 0x15BD90C);
            t5 = *(DWORD *)(CGame::GetBase() + 0x15BE630);
            t6 = *(DWORD *)(CGame::GetBase() + 0x15BDD50);
            t7 = *(BYTE *)(CGame::GetBase() + 0x15BD3FC);
            t8 = *(DWORD *)(CGame::GetBase() + 0x15BE118);
            t9 = *(DWORD *)(CGame::GetBase() + 0x15BD990);
			CLogFile::Printf("AT 0x15BE4C0: 0x(%p)",t1);
			CLogFile::Printf("AT 0x15BE490: 0x(%p)",t2);
			CLogFile::Printf("AT 0x15BD4DC: 0x(%p)",t3);
			CLogFile::Printf("AT 0x15BD90C: 0x(%p)",t4);
			CLogFile::Printf("AT 0x15BE630: 0x(%p)",t5);
			CLogFile::Printf("AT 0x15BDD00: 0x(%p)",t6);
			CLogFile::Printf("AT 0x15BDD50: 0x(%p)",t7);
			CLogFile::Printf("AT 0x15BE118: 0x(%p)",t8);
			CLogFile::Printf("AT 0x15BD990: 0x(%p)",t9);

			CLogFile::Printf("TRACK BRONX");
            t1 = *(DWORD *)(CGame::GetBase() + 0x15BE4C4);
            t2 = *(DWORD *)(CGame::GetBase() + 0x15BE494);
            t3 = *(DWORD *)(CGame::GetBase() + 0x15BD4E0);
            t4 = *(DWORD *)(CGame::GetBase() + 0x15BD910);
            t5 = *(DWORD *)(CGame::GetBase() + 0x15BE770);
            t6 = *(DWORD *)(CGame::GetBase() + 0x15BDDA0);
            t7 = *(BYTE *)(CGame::GetBase() + 0x15BD410);
            t8 = *(DWORD *)(CGame::GetBase() + 0x15BE168);
            t9 = *(DWORD *)(CGame::GetBase() + 0x15BD9E0);
			CLogFile::Printf("AT 0x15BE4C4: 0x(%p)",t1);
			CLogFile::Printf("AT 0x15BE494: 0x(%p)",t2);
			CLogFile::Printf("AT 0x15BD4E0: 0x(%p)",t3);
			CLogFile::Printf("AT 0x15BD910: 0x(%p)",t4);
			CLogFile::Printf("AT 0x15BE770: 0x(%p)",t5);
			CLogFile::Printf("AT 0x15BDDA0: 0x(%p)",t6);
			CLogFile::Printf("AT 0x15BD410: 0x(%p)",t7);
			CLogFile::Printf("AT 0x15BE168: 0x(%p)",t8);
			CLogFile::Printf("AT 0x15BD9E0: 0x(%p)",t9);

			CLogFile::Printf("TRACK BRONX2");
            t1 = *(DWORD *)(CGame::GetBase() + 0x15BE4C8);
            t2 = *(DWORD *)(CGame::GetBase() + 0x15BE498);
			t3 = *(DWORD *)(CGame::GetBase() + 0x15BD4E4);
            t4 = *(DWORD *)(CGame::GetBase() + 0x15BD914);
            t5 = *(DWORD *)(CGame::GetBase() + 0x15BE8B0);
            t6 = *(DWORD *)(CGame::GetBase() + 0x15BDDF0);
            t7 = *(BYTE *)(CGame::GetBase() + 0x15BD424);
            t8 = *(DWORD *)(CGame::GetBase() + 0x15BE1B8);
            t9 = *(DWORD *)(CGame::GetBase() + 0x15BDA30);
			CLogFile::Printf("AT 0x15BE4C8: 0x(%p)",t1);
			CLogFile::Printf("AT 0x15BE498: 0x(%p)",t2);
			CLogFile::Printf("AT 0x15BD4E4: 0x(%p)",t3);
			CLogFile::Printf("AT 0x15BD914: 0x(%p)",t4);
			CLogFile::Printf("AT 0x15BE8B0: 0x(%p)",t5);
			CLogFile::Printf("AT 0x15BDDF0: 0x(%p)",t6);
			CLogFile::Printf("AT 0x15BD424: 0x(%p)",t7);
			CLogFile::Printf("AT 0x15BE1B8: 0x(%p)",t8);
			CLogFile::Printf("AT 0x15BDA30: 0x(%p)",t9);

			DWORD dwBase = *(DWORD *)(CGame::GetBase() + 0x15BE4C8);
			DWORD dwHandle = *(DWORD *)(CGame::GetBase() + (dwBase + 0x4));
			CLogFile::Printf("Tack Bronx2: %d",dwHandle);

			unsigned int uiHandle = g_pVehicleManager->Get(7)->GetScriptingHandle();
			CLogFile::Printf("Normale Vehicle Handle: %p(%d)",uiHandle,uiHandle);
			
		}*/

		// Is this a F7 key up?
		if(uMsg == WM_KEYUP && wParam == VK_F7)
		{
			if(g_pClient->GetChatWindow())
				g_pClient->GetChatWindow()->SetEnabled(!g_pClient->GetChatWindow()->IsEnabled());
		}

		// Are we in game?
		if(CGame::GetState() == GAME_STATE_INGAME)
		{
			// Is this a Page Up key up?
			if(uMsg == WM_KEYUP && wParam == VK_PRIOR)
			{
				// Move the chat window page upwards
				g_pClient->GetChatWindow()->PageUp();
				return 1;
			}

			// Is this a Page Down key up?
			if(uMsg == WM_KEYUP && wParam == VK_NEXT)
			{
				// Move the chat window page downwards
				g_pClient->GetChatWindow()->PageDown();
				return 1;
			}

			// If the input window exists pass the input to it
			if(pInputWindow)
			{
				bool bInputEnabled = pInputWindow->IsEnabled();

				if(pInputWindow->HandleInput(uMsg, (DWORD)wParam))
				{
					// Have we just enabled the chat window?
					if(!bInputEnabled && pInputWindow->IsEnabled())
					{
						// Clear all keys
						for(std::list<String>::iterator iter = pressedKeys.begin(); pressedKeys.size() > 0; iter = pressedKeys.begin())
						{
							CSquirrelArguments pArguments;
							pArguments.push(*iter);
							pArguments.push("up");
							g_pClient->GetEvents()->Call("keyPress", &pArguments);
							pressedKeys.erase(iter);
						}

						return 1;
					}
				}
			}

			// Is the input window disabled?
			if(pInputWindow && !pInputWindow->IsEnabled())
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

					CEvents * pEvents = g_pClient->GetEvents();

					if(strState.IsNotEmpty() && pEvents)
					{
						CSquirrelArguments pArguments;
						pArguments.push(strCode);
						pArguments.push(strState);
						pEvents->Call("keyPress", &pArguments);
					}
				}
			}
		}
	}

	return CallWindowProc(m_wWndProc, hWnd, uMsg, wParam, lParam);
}
