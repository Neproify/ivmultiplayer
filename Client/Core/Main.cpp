//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include <winsock2.h>
#include <windows.h>
#include <d3d9.h>
#include <CLogFile.h>
#include "CGame.h"
#include "CXLiveHook.h"
#include "CChatWindow.h"
#include "CInputWindow.h"
#include "CDirect3DHook.h"
#include "CDirectInputHook.h"
#include "CCursorHook.h"
#include "CNetworkManager.h"
#include "CPlayerManager.h"
#include "CLocalPlayer.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CCheckpointManager.h"
#include "CPickupManager.h"
#include "CModelManager.h"
#include "Scripting.h"
#include "Commands.h"
#include "CCamera.h"
#include "CGUI.h"
#include <CSettings.h>
#include "CClientScriptManager.h"
#include "CMainMenu.h"
#include "CFPSCounter.h"
#include "CDebugView.h"
#include "SharedUtility.h"
#include "CFileTransfer.h"
#include "CGraphics.h"
#include "KeySync.h"
#include "CStreamer.h"
#include <Game/CTime.h>
#include "CEvents.h"
#include <Game/CTrafficLights.h>
#include <Network/CNetworkModule.h>
#include "CCredits.h"
#include "CNameTags.h"
#include "CClientTaskManager.h"
#include "CPools.h"
#include "CIVWeather.h"
#include <CExceptionHandler.h>
#include "CScreenShot.h"
#include "CAudio.h"
#include "CFireManager.h"
#include <Threading/CThread.h>
#include <Threading/CMutex.h>
#include "C3DLabels.h"

#ifdef IVMP_WEBKIT
	//#include "CD3D9Webkit.hpp"
#endif

IDirect3DDevice9     * g_pDevice = NULL;
CChatWindow          * g_pChatWindow = NULL;
CInputWindow         * g_pInputWindow = NULL;
CNetworkManager      * g_pNetworkManager = NULL;
CPlayerManager       * g_pPlayerManager = NULL;
CLocalPlayer         * g_pLocalPlayer = NULL;
CVehicleManager      * g_pVehicleManager = NULL;
CObjectManager       * g_pObjectManager = NULL;
CBlipManager         * g_pBlipManager = NULL;
CActorManager        * g_pActorManager = NULL;
CCheckpointManager   * g_pCheckpointManager = NULL;
CPickupManager       * g_pPickupManager = NULL;
CModelManager        * g_pModelManager = NULL;
CCamera              * g_pCamera = NULL;
CGUI                 * g_pGUI = NULL;
CGraphics            * g_pGraphics = NULL;
CScriptingManager    * g_pScriptingManager = NULL;
CClientScriptManager * g_pClientScriptManager = NULL;
CMainMenu            * g_pMainMenu = NULL;
CFPSCounter          * g_pFPSCounter = NULL;
CDebugView			 * g_pDebugView = NULL;
CGUIStaticText       * g_pVersionIdentifier = NULL;
CFileTransfer        * g_pFileTransfer = NULL;
CStreamer            * g_pStreamer = NULL;
CTime                * g_pTime = NULL;
CEvents              * g_pEvents = NULL;
CTrafficLights       * g_pTrafficLights = NULL;
CCredits             * g_pCredits = NULL;
CNameTags            * g_pNameTags = NULL;
CClientTaskManager   * g_pClientTaskManager = NULL;
CFireManager		 * g_pFireManager = NULL;
C3DLabelManager		* g_p3DLabelManager = NULL;

#ifdef IVMP_WEBKIT
	//CD3D9WebKit * g_pWebkit;
	//CD3D9WebView * g_pWebView;
#endif

bool		   g_bGameLoaded = false;
bool           g_bWindowedMode = false;
bool           g_bFPSToggle = false;
unsigned short g_usPort;
String         g_strHost;
String         g_strNick;
String         g_strPassword;
bool		   g_bLoadingScreenloaded = false;
int			   g_iCameraState = 1;
int			   g_iCameraTime = 0;
bool		   g_bNetworkStatsDisplayed = false;

CHttpClient *httpClient;

// TODO: Move this to another class?
extern float fTextPos[2];
extern String strTextText;
extern int iTextTime;
extern DWORD dwTextStartTick;

void ResetGame(bool now);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread library notifications
			DisableThreadLibraryCalls(hModule);

			// Install the exception handler
			CExceptionHandler::Install();

			// Delete chatlog
			CLogFile::Open("Chatlog.log");
			CLogFile::Printf("New chatlog created!");
			CLogFile::Close();

			// Open the log file
			CLogFile::Open("Client.log");

			// Log the version
			CLogFile::Printf(VERSION_IDENTIFIER "| " __DATE__ " - " __TIME__ "");

			// Open the settings file
			CSettings::Open(SharedUtility::GetAbsolutePath("clientsettings.xml"));

			// Parse the command line
			CSettings::ParseCommandLine(GetCommandLine());

			// Load the global vars from the settings
			g_strHost = CVAR_GET_STRING("ip");
			g_usPort = CVAR_GET_INTEGER("port");
			g_strNick = CVAR_GET_STRING("nick");
			g_strPassword = CVAR_GET_STRING("pass");
			g_bWindowedMode = CVAR_GET_BOOL("windowed");
			g_bFPSToggle = CVAR_GET_BOOL("fps");

			// IE9 fix - disabled if disableie9fix is set or shift is pressed
			if(!CVAR_GET_BOOL("disableie9fix") || GetAsyncKeyState(VK_SHIFT) > 0)
			{
				// Get the version info
				DWORD dwHandle;
				DWORD dwSize = GetFileVersionInfoSize("wininet.dll", &dwHandle);
				BYTE* byteFileInfo = new BYTE[dwSize];
				GetFileVersionInfo("wininet.dll", dwHandle, dwSize, byteFileInfo);

				unsigned int uiLen;
				VS_FIXEDFILEINFO* fileInfo;
				VerQueryValue(byteFileInfo, "\\", (LPVOID *)&fileInfo, &uiLen);
				delete byteFileInfo;

				// using IE9?
				if(fileInfo->dwFileVersionMS == 0x90000)
				{
					// Try and load a wininet.dll from the iv:mp directory
					if(!LoadLibrary(SharedUtility::GetAbsolutePath("wininet.dll")))
					{
						// Get path to it
						char szFindPath[MAX_PATH] = {0};
						char szWinSxsPath[MAX_PATH] = {0};
						char szBuildVersion[] = "00000";
						GetEnvironmentVariable("windir", szWinSxsPath, sizeof(szWinSxsPath));
						strcat_s(szWinSxsPath, sizeof(szWinSxsPath), "\\WinSxS\\");
						strcpy_s(szFindPath, sizeof(szFindPath), szWinSxsPath);
						strcat_s(szFindPath, sizeof(szFindPath), "x86_microsoft-windows-i..tocolimplementation_31bf3856ad364e35*");

						// try to find a usable wininet.dll in WinSXS (basically any non-9.x version)
						bool bLoaded = false;
						WIN32_FIND_DATA lpFindFileData;
						HANDLE hFindFile = FindFirstFile(szFindPath, &lpFindFileData);
						do
						{
							if(hFindFile == INVALID_HANDLE_VALUE)
								break;

							if(strlen(lpFindFileData.cFileName) > 63)
							{
								if(lpFindFileData.cFileName[62] < '9')
								{
									char szFullPath[MAX_PATH];
									sprintf_s(szFullPath, MAX_PATH, "%s%s\\wininet.dll", szWinSxsPath, lpFindFileData.cFileName);
									if(LoadLibrary(szFullPath))
									{
										CLogFile::Printf("Using %s to address IE9 issue", szFullPath);
										bLoaded = true;
										break;
									}
								}
							}
						}
						while(FindNextFile(hFindFile, &lpFindFileData));

						// Still failed, tell the user
						if(!bLoaded)
						{
							if(MessageBox(0, "Unfortunately, you have Internet Explorer 9 installed which is not compatible with GTA:IV. Do you want proceed anyway (and possibly crash?)", "IV:MP", MB_YESNO | MB_ICONERROR) == IDNO)
							{
								// Doesn't want to continue
								ExitProcess(0);
							}

							// Save the user's choice
							CVAR_SET_BOOL("disableie9fix", true);
						}
					}
				}
			}

			// Initialize the streamer
			g_pStreamer = new CStreamer();

			// Initialize the time
			g_pTime = new CTime();

			// Initialize the traffic lights
			g_pTrafficLights = new CTrafficLights();

			// Initialize the client task manager
			g_pClientTaskManager = new CClientTaskManager();
			
			// Initialize audio manager
			CAudioManager::Init();

			// Initialize the game
			CGame::Initialize();

			// Install the XLive hook
			CXLiveHook::Install();

			// Install the Direct3D hook
			CDirect3DHook::Install();

			// Install the DirectInput hook
			CDirectInputHook::Install();

			// Install the Cursor hook
#ifdef IVMP_DEBUG
			CCursorHook::Install();
			g_pDebugView = new CDebugView();
#endif
#ifdef IVMP_DEV_VER
			g_pDebugView = new CDebugView();
#endif
			// Initialize the client script manager
			g_pClientScriptManager = new CClientScriptManager();

			// Initialize the events manager
			g_pEvents = new CEvents();

			// Initialize the network module, if it fails, exit
			if(!CNetworkModule::Init())
			{
				CLogFile::Printf("Failed to initialize the network module!\n");
				ExitProcess(0);
			}

			// Initialize the file transfer
			g_pFileTransfer = new CFileTransfer();
			g_pFileTransfer->SetDownloadedHandler(FileTransfer_DownloadedFile);
			g_pFileTransfer->SetDownloadFailedHandler(FileTransfer_DownloadFailed);
		}
		break;
	case DLL_PROCESS_DETACH:
		{

			// Delete our file transfer
			SAFE_DELETE(g_pFileTransfer);

			// Delete our camera
			SAFE_DELETE(g_pCamera);

			// Delete our model manager
			SAFE_DELETE(g_pModelManager);

			// Delete our pickup manager
			SAFE_DELETE(g_pPickupManager);

			// Delete our checkpoint manager
			SAFE_DELETE(g_pCheckpointManager);

			// Delete our object manager
			SAFE_DELETE(g_pObjectManager);

			// Delete our blip manager
			SAFE_DELETE(g_pBlipManager);

			// Delete our actor manager
			SAFE_DELETE(g_pActorManager);

			// Delete our vehicle manager
			SAFE_DELETE(g_pVehicleManager);

			// Delete our local player
			SAFE_DELETE(g_pLocalPlayer);

			// Delete our player manager
			SAFE_DELETE(g_pPlayerManager);

			// Delete our network manager
			SAFE_DELETE(g_pNetworkManager);

			// Delete our name tags
			SAFE_DELETE(g_pNameTags);

			// Delete our input window
			SAFE_DELETE(g_pInputWindow);

			// Delete our chat window
			SAFE_DELETE(g_pChatWindow);

			// Delete our fps counter
			SAFE_DELETE(g_pFPSCounter);

#ifdef IVMP_DEBUG
			// Delete out debug viewer
			SAFE_DELETE(g_pDebugView);
#endif
#ifdef IVMP_DEV_VER
			// Delete out debug viewer
			SAFE_DELETE(g_pDebugView);
#endif
			// Delete our credits
			SAFE_DELETE(g_pCredits);

			// Delete our main menu
			SAFE_DELETE(g_pMainMenu);

			// Delete our gui
			SAFE_DELETE(g_pGUI);

			// Delete our streamer class
			SAFE_DELETE(g_pStreamer);

			// Delete our time class
			SAFE_DELETE(g_pTime);

			// Delete our traffic lights
			SAFE_DELETE(g_pTrafficLights);

			// Delete our client script manager
			SAFE_DELETE(g_pClientScriptManager);

			// Delete our client task manager
			SAFE_DELETE(g_pClientTaskManager);

			// Delete our events manager
			SAFE_DELETE(g_pEvents);

			SAFE_DELETE(g_p3DLabelManager);

			// Uninstall the Cursor hook
#ifdef IVMP_DEBUG
			CCursorHook::Uninstall();
#endif

			// Uninstall the DirectInput hook
			CDirectInputHook::Uninstall();

			// Uninstall the Direct3D hook
			CDirect3DHook::Uninstall();

			// Shutdown audio manager
			CAudioManager::SetAllVolume(0.0f);
			CAudioManager::RemoveAll();

			// Shutdown our game
			CGame::Shutdown();

			// Close the settings file
			CSettings::Close();

			// Close the log file
			CLogFile::Close();

			// Uninstall the XLive hook
			//CXLiveHook::Uninstall(); // Not needed
		}
		break;
	}

	return TRUE;
}

// Direct3DDevice9::EndScene
void Direct3DRender()
{
	// Check for pause menu
	if(g_pNetworkManager)
	{
		if(CGame::IsMenuActive() && CGame::GetState() == GAME_STATE_INGAME && g_pNetworkManager->IsConnected())
			CGame::SetState(GAME_STATE_PAUSE_MENU);
		else if(!CGame::IsMenuActive() && CGame::GetState() == GAME_STATE_PAUSE_MENU && g_pNetworkManager->IsConnected())
			CGame::SetState(GAME_STATE_INGAME);
	}
	// Are we in the main menu?
	if(CGame::GetState() == GAME_STATE_MAIN_MENU || CGame::GetState() == GAME_STATE_LOADING)
	{
		// Is the main menu shown?
		if(CGame::IsMenuActive())
		{
			// Clear the screen
			g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0, 0);
		}
	}
	if(g_pMainMenu && CGame::GetState() == GAME_STATE_MAIN_MENU || CGame::GetState() == GAME_STATE_IVMP_PAUSE_MENU)
	{
		if(!g_pMainMenu->IsVisible())
		{
			if(g_pNetworkManager && g_pNetworkManager->IsConnected())
			{
				g_pMainMenu->SetDisconnectButtonVisible(true);
				g_pMainMenu->SetVisible(true);
			}
			else
			{
				g_pMainMenu->SetDisconnectButtonVisible(false);
				g_pMainMenu->SetVisible(true);
			}
			if(CGame::GetState() == GAME_STATE_MAIN_MENU) {
				httpClient = new CHttpClient();
				httpClient->SetRequestTimeout(10000);
				httpClient->SetHost(MASTERLIST_ADDRESS);
				String strPostPath("/getlatestversion.php");
				httpClient->Get(strPostPath);
			}
		}
	}
	else
	{
		if(g_pMainMenu->IsVisible())
			g_pMainMenu->SetVisible(false);
	}

	if(g_pClientScriptManager && g_pClientScriptManager->GetGUIManager())
	{
		if(CGame::GetState() != GAME_STATE_INGAME)
		{
			if(!g_pClientScriptManager->GetGUIManager()->IsHidden())
				g_pClientScriptManager->GetGUIManager()->Hide();
		}
		else
		{
			if(g_pClientScriptManager->GetGUIManager()->IsHidden())
				g_pClientScriptManager->GetGUIManager()->Show();
		}
	}

	// If our GUI class exists render it
	if(g_pGUI)
		g_pGUI->Render();

	// If our WebKit class exists render it
	/*	
	#ifdef IVMP_WEBKIT
		if(g_pWebkit)
		{
			g_pWebkit->RenderAll();
		}
	#endif
	*/
	// If our main menu exists process it
	if(g_pMainMenu)
		g_pMainMenu->Process();

	// if our chat exist draw it
	if(g_pChatWindow && !CGame::IsMenuActive())
		g_pChatWindow->Draw();

	// If our fps class exists update it
	if(g_pFPSCounter)
		g_pFPSCounter->Pulse();

	// If our credits class exists process it
	if(g_pCredits)
		g_pCredits->Process();

	if(g_p3DLabelManager)
		g_p3DLabelManager->Render();

	// If our scripting manager exists, call the frame event
	if(g_pEvents && !g_pMainMenu->IsVisible())
		g_pEvents->Call("frameRender");

#ifdef IVMP_DEV_VER
	if(g_pDebugView && g_pGUI && g_pLocalPlayer)
		g_pDebugView->Draw();
#endif

	// Check if our screen shot write failed
	if(CScreenShot::IsDone())
	{
		if(CScreenShot::HasSucceeded())
			g_pChatWindow->AddInfoMessage("Screen shot written (%s).", CScreenShot::GetWriteName().Get());
		else
			g_pChatWindow->AddInfoMessage("Screen shot write failed (%s).", CScreenShot::GetError().Get());

		CScreenShot::Reset();
	}
	
	// Moving Camera for main menu, problem: g_pCamera->SetLookAt() cause an crash at change
	if(g_pNetworkManager && g_pCamera)
	{
		if(!g_pNetworkManager->IsConnected() && CGame::IsGameLoaded())
		{
			if(g_iCameraTime > 500)
			{
				if(g_iCameraState == 1)
					g_iCameraState = 2;
				else if(g_iCameraState == 2)
					g_iCameraState = 3;
				else if(g_iCameraState == 3)
					g_iCameraState = 1;

				g_iCameraTime = 0;

				if(g_iCameraState == 1)
				{
					g_pCamera->SetPosition(CVector3(HAPPINESS_CAMERA_POS));
					g_pCamera->SetLookAt(CVector3(HAPPINESS_CAMERA_LOOK_AT),false);
				}
				if(g_iCameraState == 2)
				{
					g_pCamera->SetPosition(CVector3(TRIANGLE_CAMERA_POS));
					g_pCamera->SetLookAt(CVector3(TRIANGLE_CAMERA_LOOK_AT),false);
				}
				if(g_iCameraState == 3)
				{
					g_pCamera->SetPosition(CVector3(TRIANGLE_CAMERA_POS_OLD));
					g_pCamera->SetLookAt(CVector3(TRIANGLE_CAMERA_LOOK_AT_OLD),false);
				}
			}
			else
			{
				g_iCameraTime++;

				CVector3 vecPosition;
				g_pCamera->GetPosition(vecPosition);

				vecPosition.fX += 0.1f;
				vecPosition.fY += 0.1f;

				g_pCamera->SetPosition(vecPosition);
			}
		}
	}/*
	if(g_pNetworkManager)
	{
		if(!g_pNetworkManager->IsConnected() && CGame::IsGameLoaded())
		{
			if(CGame::GetState() == (eState)3)
				CGame::SetState(GAME_STATE_IVMP_PAUSE_MENU);

			if(!g_pMainMenu->IsVisible() && !CGame::IsMenuActive())
				CGame::SetState(GAME_STATE_IVMP_PAUSE_MENU);
		}
	}*/

	if(CGame::GetState() == GAME_STATE_MAIN_MENU)
	{
		if(httpClient->IsBusy()) {
			httpClient->Process();
			if(httpClient->GotData()) {
				// Get the data
				String * strData = httpClient->GetData();

				// Did we not get any data?
				if(strData->IsEmpty())
					return;

#ifndef IVMP_DEV_VER
				if(strcmp(strData->Get(),MOD_VERSION_STRING)) {
					if(g_pGUI)
						g_pGUI->ShowMessageBox("New version is avaiable!","New Version avaiable!");
				}
#endif
			}
		}
	}

	// Are we in game?
	if(CGame::GetState() == GAME_STATE_INGAME)
	{
		// Is F4 held down and do we have a network manager?
		if(GetAsyncKeyState(VK_F4) && g_pNetworkManager && g_pActorManager)
		{
			// Get the network statistics
			CNetStats * pNetStats = g_pNetworkManager->GetNetClient()->GetNetStats();

			// Convert the network statistics to a string
			char szNetworkStats[10000];
			pNetStats->ToString(szNetworkStats, 2);

			// Create the statistics string
			String strStats(szNetworkStats);

			// Append loaded and unloaded model counts to the stats
			// jenksta: too performance heavy to be done every frame
			//strStats.AppendF("Models (Loaded/Unload): %d/%d\n", CGame::GetLoadedModelCount(), CGame::GetUnloadedModelCount());

			// Append streamed in/out entity counts and streamed in limits to the stats
			strStats.AppendF("Players (StreamedIn/StreamedInLimit): %d/%d\n", g_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_PLAYER)+1, g_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_PLAYER));
			strStats.AppendF("Vehicles (StreamedIn/StreamedInLimit): %d/%d\n", g_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_VEHICLE), g_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_VEHICLE));
			strStats.AppendF("Actors (Actor-Count/Max-Actors): %d/%d\n", g_pActorManager->GetActorCount(), MAX_ACTORS);
			strStats.AppendF("Pickups (StreamedIn/StreamedInLimit): %d/%d\n", g_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_PICKUP), g_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_PICKUP));
			strStats.AppendF("Objects (StreamedIn/StreamedInLimit): %d/%d\n", g_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_OBJECT), g_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_OBJECT));
			strStats.AppendF("Checkpoints (StreamedIn/StreamedInLimit): %d/%d\n", g_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_CHECKPOINT), g_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_CHECKPOINT));

			// Draw the string
			g_pGUI->DrawText(strStats, CEGUI::Vector2(26, 30), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), g_pGUI->GetFont("tahoma-bold", 10));
			g_pChatWindow->SetEnabled(false);
			g_bNetworkStatsDisplayed = true;
		}
		else
		{
			// If our network stats were displayed
			if(g_bNetworkStatsDisplayed) {
				if(g_pChatWindow && !g_pChatWindow->IsEnabled())
					g_pChatWindow->SetEnabled(true);
				
				g_bNetworkStatsDisplayed = false;
			}

			// If our input window exists draw it
			if(g_pInputWindow)
				g_pInputWindow->Draw();
		}

		// Update the time
		if(g_pTime)
		{
			// jenksta: Setting this every frame causes the weather to change
			// rapidly
			unsigned char ucHour = 0, ucMinute = 0;
			g_pTime->GetTime(&ucHour, &ucMinute);
			CGame::SetTime(ucHour, ucMinute);
			CGame::SetDayOfWeek(g_pTime->GetDayOfWeek());
		}
		/* Vehicle Headlights testcode
			IVVehicle * pGameVehicle = pVehicle->GetGameVehicle()->GetVehicle();
			//*(BYTE *)(pVehicle->GetGameVehicle()->GetVehicle() + 0xF71) |= 1;
			*((BYTE *)pGameVehicle + 3953) = *((BYTE *)pGameVehicle + 3953) & 0xFE | 2;
				
		}

		 CViewportManager + 0x00 = sysArray (CViewport *)
		 CViewport + 0x53C = Viewport ID
		 GET_VIEWPORT_POS_AND_SIZE(unsigned int uiViewportId, float * fPosX, float * fPosY, float * fSizeX, float * fSizeY)
		 (pViewport + 0x10) is always used
		 ((pViewport + 0x10) + 0x298) = float fPosX;
		 (((pViewport + 0x10) + 0x298) + 0x4) = float fPosY;
		 (((pViewport + 0x10) + 0x298) + 0x8) = float fSizeX;
		 (((pViewport + 0x10) + 0x298) + 0xC) = float fSizeY;
		 GET_VIEWPORT_POSITION_OF_COORD(float fCoordX, float fCoordY, float fCoordZ, unsigned int uiViewportId, float * fPosX, float * fPosY)
		 Viewport 1 = CViewportPrimaryOrtho
		 Viewport 2 = CViewportGame
		 Viewport 3 = CViewportRadar
		*/
		if(g_pLocalPlayer)
		{
			if(g_pLocalPlayer->GetVehicleInfos())
			{
				if(g_pVehicleManager)
				{
					CVector3 vecWorldPosition;
					Vector2 vecScreenPosition;

					for(EntityId i = 0; i < MAX_VEHICLES; i++)
					{
						if(g_pVehicleManager->Exists(i))
						{
							CNetworkVehicle * pVehicle = g_pVehicleManager->Get(i);

							if(!pVehicle->IsStreamedIn())
								continue;

							if(!pVehicle->IsOnScreen())
								continue;
					
							pVehicle->GetPosition(vecWorldPosition); 
							if(!CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition))
								continue;

							
							int health = pVehicle->GetHealth();
							int model = pVehicle->GetEngineState();
							float petrol = pVehicle->GetPetrolTankHealth();
							CVector3 vecPos; pVehicle->GetPosition(vecPos);
							CVector3 vecRot; pVehicle->GetRotation(vecRot);
							g_pGUI->DrawText(String("VehicleId %d, Enginestate: %d, Health: %d, PetrolTankHealth: %f\nPosition(%.3f,%.3f,%.3f), Rot(%.3f,%.3f,%.3f)", i, model, health, petrol, vecPos.fX,vecPos.fY,vecPos.fZ,vecRot.fX,vecRot.fY,vecRot.fZ), CEGUI::Vector2(vecScreenPosition.X, vecScreenPosition.Y));
						}
					}
				}
			}
		}
		if(g_pNameTags)
			g_pNameTags->Draw();
	}
}

// Direct3DDevice9::Reset
void Direct3DInvalidate()
{
	// If our gui instance exists inform it of the device loss
	if(g_pGUI)
		g_pGUI->OnLostDevice();

	// If our graphics instance exists inform it of the device loss
	if(g_pGraphics)
		g_pGraphics->OnLostDevice();
}

// Direct3DDevice9::Reset
void Direct3DReset()
{
	// If our GUI instance does not exist create it
	if(!g_pGUI)
	{
		g_pGUI = new CGUI(g_pDevice);

		if(g_pGUI->Initialize())
		{
			// Version identifier text
			g_pVersionIdentifier = g_pGUI->CreateGUIStaticText();
			g_pVersionIdentifier->setText(VERSION_IDENTIFIER);
			CEGUI::Font * pFont = g_pGUI->GetFont("tahoma-bold");
			float fTextWidth = pFont->getTextExtent(VERSION_IDENTIFIER);
			float fTextHeight = pFont->getFontHeight();
			g_pVersionIdentifier->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
			float fTextX = pFont->getTextExtent("_");
			float fTextY = -(fTextX + fTextHeight);
			g_pVersionIdentifier->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fTextX), CEGUI::UDim(1, fTextY)));
			g_pVersionIdentifier->setProperty("FrameEnabled", "false");
			g_pVersionIdentifier->setProperty("BackgroundEnabled", "false");
			g_pVersionIdentifier->setFont(pFont);
			g_pVersionIdentifier->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
			g_pVersionIdentifier->setAlpha(0.6f);
			g_pVersionIdentifier->setVisible(true);

			/*#ifdef IVMP_WEBKIT
				g_pWebkit = new CD3D9WebKit();
			#endif
			*/

			// TODO: Make the default stuff (Chat window, main menu, e.t.c) a xml layout so it
			// can be edited by users
			// TODO: Also load the font from an xml layout so it can be edited by users
			// TODO: Ability to output all server messages to the client console?
			// TODO: A script console when client side scripts are implemented

			CLogFile::Printf("GUI initialized");
		}
		else
			CLogFile::Printf("GUI initialization failed");
	}
	else
	{
		// If our GUI class does exist inform it of the device reset
		g_pGUI->OnResetDevice();
	}

	// If our graphics instance does not exist create it
	if(!g_pGraphics && g_pDevice)
		g_pGraphics = new CGraphics(g_pDevice);
	else
		g_pGraphics->OnResetDevice();

	// If our main menu class does not exist create it
	if(!g_pMainMenu)
		g_pMainMenu = new CMainMenu();
	else
		g_pMainMenu->OnResetDevice();


	if(!g_p3DLabelManager)
		g_p3DLabelManager = new C3DLabelManager();

	//// Show loading screen
	//if(!CGame::IsGameLoaded() && !CGame::IsMenuActive())
	//	g_pMainMenu->ShowLoadingScreen();

	// If our credits class does not exist create it
	if(!g_pCredits)
		g_pCredits = new CCredits(g_pGUI);

	// If our fps counter class does not exist create it
	if(!g_pFPSCounter)
		g_pFPSCounter = new CFPSCounter();

	// If our fps counter class does not exist create it
	if(!g_pChatWindow)
		g_pChatWindow = new CChatWindow();

	// If our input window class does not exist create it
	if(!g_pInputWindow)
	{
		g_pInputWindow = new CInputWindow();
		RegisterCommands();
	}

	// If our name tags class does not exist create it
	if(!g_pNameTags)
		g_pNameTags = new CNameTags();
}

bool g_bResetGame = false;
void InternalResetGame(bool bToggle);

void GameLoad()
{
	// Initialize the pools
	CGame::GetPools()->Initialize();
	CLogFile::Printf("Initialized pools");

	// Reset the game
	InternalResetGame(true);
}

void GameScriptProcess()
{
	// Do we need to reset the game?
	if(g_bResetGame)
	{
		// Reset the game
		InternalResetGame(true);

		// Flag the game as no longer needed to reset
		g_bResetGame = false;
	}

	// If our network manager exists process it
	if(g_pNetworkManager)
		g_pNetworkManager->Process();

	//Scripting::SetTextScale(0.40f,0.30f);
	//Scripting::DisplayTextWithLiteralString(0.025f, 0.960f, "STRING", "Testmessage");
	//Scripting::DisplayTextWithString(0.832f, 0.069f, "STRING", "SPECAL"); 
	//Scripting::PrintHelpForever("TX_H07");

	// HACKY!
	// TEMP! TODO: Anywhere in GTA there's a function which checks if the engine is turned on or off...
	//		       ...If the player is in the vehicle, it will turn it automatic on -.-
	if(g_pLocalPlayer)
	{
		if(g_pLocalPlayer->GetVehicle() != NULL)
		{
			// TEMP! TODO: Anywhere in GTA there's a function which checks if the engine is turned on or off...
			//		 ...If the player is in the vehicle, it will turn it automatic on -.-
			if(!g_pLocalPlayer->GetVehicle()->GetEngineState())
				g_pLocalPlayer->GetVehicle()->SetEngineState(false);
		}
	}
	for(EntityId playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			if(g_pPlayerManager->GetAt(playerId)->GetVehicle() != NULL)
			{
				if(!g_pPlayerManager->GetAt(playerId)->GetVehicle()->GetEngineState() != NULL)
					g_pPlayerManager->GetAt(playerId)->GetVehicle()->SetEngineState(false);
			}
		}
	}

	// Hide the loading screens
	g_pMainMenu->HideLoadingScreen();

	// If we have text to draw draw it
	if(iTextTime > 0)
	{
		if((int)(SharedUtility::GetTime() - dwTextStartTick) < iTextTime)
			Scripting::DisplayTextWithLiteralString(fTextPos[0], fTextPos[1], "STRING", strTextText);
		else
			iTextTime = 0;
	}
}

void ResetGame(bool now)
{
	// reset game right at function call is needed to reset clientside scripting stuff on lost connection
	// clean up this if looks like shit.
	if(now)
		InternalResetGame(false);
	else 
		g_bResetGame = true;
}

void InternalResetGame(bool bAutoConnect)
{
	CLogFile::Printf("Initializing game for multiplayer activities");

	// Remove local player from vehicle
	if(g_pLocalPlayer)
		g_pLocalPlayer->RemoveFromVehicle();

	SAFE_DELETE(g_pClientScriptManager);
	g_pClientScriptManager = new CClientScriptManager();
 	g_pEvents->clear();
	g_pGUI->SetScriptedCursorVisible(false);
	CLogFile::Printf("Reset clientside scripting stuff");

	SAFE_DELETE(g_pModelManager);
	g_pModelManager = new CModelManager();
	CLogFile::Printf("Created model manager instance");

	SAFE_DELETE(g_pPickupManager);
	g_pPickupManager = new CPickupManager();
	CLogFile::Printf("Created pickup manager instance");

	SAFE_DELETE(g_pCheckpointManager);
	g_pCheckpointManager = new CCheckpointManager();
	CLogFile::Printf("Created checkpoint manager instance");

	SAFE_DELETE(g_pBlipManager);
	g_pBlipManager = new CBlipManager();
	CLogFile::Printf("Created blip manager instance");

	SAFE_DELETE(g_pObjectManager);
	g_pObjectManager = new CObjectManager();
	CLogFile::Printf("Created object manager instance");

	SAFE_DELETE(g_pFireManager);
	g_pFireManager = new CFireManager();
	CLogFile::Printf("Created fire manager instance");

	SAFE_DELETE(g_p3DLabelManager);
	g_p3DLabelManager = new C3DLabelManager();
	CLogFile::Printf("Created 3d label manager instance");

	// Set all vehicles to destroyable
	if(g_pVehicleManager)
	{
		for(EntityId i = 0; i < g_pVehicleManager->GetCount(); i++)
			g_pVehicleManager->Get(i)->MarkAsActorVehicle(false);
	}

	SAFE_DELETE(g_pVehicleManager);
	g_pVehicleManager = new CVehicleManager();
	CLogFile::Printf("Created vehicle manager instance");

	SAFE_DELETE(g_pActorManager);
	g_pActorManager = new CActorManager();
	CLogFile::Printf("Created actor manager instance");

	// Reset the streamer
	g_pStreamer->Reset();
	CLogFile::Printf("Reset streamer instance");

	if(!g_pLocalPlayer)
	{
		g_pLocalPlayer = new CLocalPlayer();
		CLogFile::Printf("Created local player instance");
	}

	g_pLocalPlayer->Reset();
	g_pLocalPlayer->SetPlayerId(INVALID_ENTITY_ID);
	g_pLocalPlayer->SetModel(Scripting::MODEL_PLAYER);
	g_pLocalPlayer->Teleport(CVector3());
	g_pLocalPlayer->SetPlayerControlAdvanced(false, false);
	g_pLocalPlayer->RemoveAllWeapons();
	g_pLocalPlayer->ResetMoney();
	g_pLocalPlayer->SetHealth(200);
	CLogFile::Printf("Reset local player instance");

	SAFE_DELETE(g_pPlayerManager);
	g_pPlayerManager = new CPlayerManager();
	CLogFile::Printf("Created player manager instance");

	// Do we have a network manager instance?
	if(g_pNetworkManager)
	{
		// If we are connected disconnect
		if(g_pNetworkManager->IsConnected())
			g_pNetworkManager->Disconnect();

		// Delete our network manager instance
		SAFE_DELETE(g_pNetworkManager);
	}

	g_pNetworkManager = new CNetworkManager();
	CLogFile::Printf("Created network manager instance");

	g_pNetworkManager->Startup(g_strHost, g_usPort, g_strPassword);
	CLogFile::Printf("Started network manager instance");

	// Reset file transfer
	g_pFileTransfer->Reset();

	g_pTime->SetDayOfWeek(2);
	g_pTime->SetMinuteDuration(0);
	g_pTrafficLights->Reset();
	g_pNameTags->SetEnabled(true);
	CGame::SetWantedLevel(0);
	CGame::SetHudVisible(false);
	CGame::SetRadarVisible(false);
	CGame::SetAreaNamesEnabled(false);
	CGame::ResetScrollBars();
	CGame::SetScrollBarColor();
	CGame::ToggleLazlowStation(true);
	Scripting::SetScenarioPedDensityMultiplier(0, 0);
	// SetCanBurstCarTyres(bool canburst);
	Scripting::SetMaxWantedLevel(0);
	Scripting::SetCreateRandomCops(false);
	// Test if this is needed (Script is unloaded, so it shouldn't be)
	Scripting::AllowStuntJumpsToTrigger(false);
	CLogFile::Printf("Reset world");

	// Destroy the camera and create it again
	if(g_pCamera)
	{
		g_pCamera->~CCamera();
		SAFE_DELETE(g_pCamera);
		g_pCamera = new CCamera(); 
	}
	else if(!g_pCamera)
		g_pCamera = new CCamera(); 
	
	CLogFile::Printf("Created/Reseted camera instance");

	if(g_pCamera) {
		g_pCamera->ActivateScriptCam();
		g_pCamera->SetPosition(CVector3(HAPPINESS_CAMERA_POS));
		//if(!g_pNetworkManager->IsConnected())
		g_pCamera->SetLookAt(CVector3(HAPPINESS_CAMERA_LOOK_AT),false);
		CLogFile::Printf("Reset camera stuff");
	}

	// Reset radio logo properties
	DWORD FUNC__IMPORT_RADIOLOGO = (CGame::GetBase() + 0x822E30);
	_asm call FUNC__IMPORT_RADIOLOGO;

	// Reset hud properties
	DWORD FUNC__IMPORT_HUD = (CGame::GetBase() + 0x848390);
	DWORD VAR__HUD_FILE = (CGame::GetBase() + /*0xD5DCF4*/0x848419);
	char *szTxt = "common:/DATA/HUD.DAT";
	CPatcher::InstallPushPatch(VAR__HUD_FILE, (DWORD)szTxt);
	_asm call FUNC__IMPORT_HUD;

	// Set the time and weather after the camera set, one of the camera stuff changes the time and the weather
	CGame::GetWeather()->SetWeather(WEATHER_SUNNY);
	g_pTime->SetTime(0, 0);
	CGame::SetTime(0,0);
	Scripting::SetTimeScale(1.0f);
	
	CGame::InitializeDefaultGameComponents();
	// TODO: Remove all gui stuff(images etc, after disconnect)
	// XForce: OnResetDevice causes blackscreen at maximize

	if(g_pLocalPlayer)
		g_pLocalPlayer->SetControl(true);

	// Mark the game as loaded.
	if(!g_bGameLoaded)
		g_bGameLoaded = true;
	CGame::SetGameLoaded(g_bGameLoaded);

	// Reset the network stats
	g_pMainMenu->ResetNetworkStats();

	// Auto connect(if needed)
	if(g_pNetworkManager && bAutoConnect)
		g_pNetworkManager->Connect();

	CLogFile::Printf("Sucessfully (re)initialized game for multiplayer activities");
}