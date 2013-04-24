//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClient.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClient.h"
#include <CExceptionHandler.h>
#include <CSettings.h>
#include "CDirect3DHook.h"
#include "CDirectInputHook.h"
#include "CCursorHook.h"
#include <Network\CNetworkModule.h>
#include "CScreenShot.h"
#include "Commands.h"
#include "CPools.h"

// TODO: Move this to another class?
extern float fTextPos[2];
extern String strTextText;
extern int iTextTime;
extern DWORD dwTextStartTick;

CClient::CClient() : m_pDevice(NULL), m_pChatWindow(NULL), m_pInputWindow(NULL), m_pNetworkManager(NULL), m_pPlayerManager(NULL), 
					 m_pLocalPlayer(NULL), m_pVehicleManager(NULL), m_pObjectManager(NULL), m_pBlipManager(NULL), 
					 m_pActorManager(NULL), m_pCheckpointManager(NULL), m_pPickupManager(NULL), m_pModelManager(NULL), 
					 m_pCamera(NULL), m_pGUI(NULL), m_pGraphics(NULL), m_pScriptingManager(NULL), m_pClientScriptManager(NULL), 
					 m_pMainMenu(NULL), m_pFPSCounter(NULL), m_pDebugView(NULL), m_pVersionIdentifier(NULL), 
					 m_pFileTransfer(NULL), m_pStreamer(NULL), m_pTime(NULL), m_pEvents(NULL), m_pTrafficLights(NULL), 
					 m_pCredits(NULL), m_pNameTags(NULL), m_pClientTaskManager(NULL), m_pFireManager(NULL), m_p3DLabelManager(NULL), 
					 m_pAudioManager(NULL), m_bGameLoaded(false), m_bWindowedMode(false), m_bFPSToggle(false), m_usPort(0), 
					 m_bNetworkStatsDisplayed(false), m_bResetGame(false), m_pHttpClient(NULL), m_bAutoConnect(false)
{

}

CClient::~CClient()
{

}

bool CClient::OnLoad()
{
	// Install the exception handler
	CExceptionHandler::Install();

	// Set our exception handler callback
	CExceptionHandler::SetCallback(ExceptionHandlerCallback);

	// jenksta: wtf?
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
	m_strHost = CVAR_GET_STRING("ip");
	m_usPort = CVAR_GET_INTEGER("port");
	m_strNick = CVAR_GET_STRING("nick");
	m_strPassword = CVAR_GET_STRING("pass");
	m_bWindowedMode = CVAR_GET_BOOL("windowed");
	m_bFPSToggle = CVAR_GET_BOOL("fps");

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
						return false;
					}

					// Save the user's choice
					CVAR_SET_BOOL("disableie9fix", true);
				}
			}
		}
	}

	// Initialize the streamer
	m_pStreamer = new CStreamer();

	// Initialize the time
	m_pTime = new CTime();

	// Initialize the traffic lights
	m_pTrafficLights = new CTrafficLights();

	// Initialize the client task manager
	m_pClientTaskManager = new CClientTaskManager();

	// Initialize the game
	CGame::Initialize();

	// Install the XLive hook
	CXLiveHook::Install();

	// Install the Direct3D hook
	CDirect3DHook::Install();

	// Install the DirectInput hook
	CDirectInputHook::Install();

	// Install the Cursor hook
	CCursorHook::Install();


#ifdef IVMP_DEBUG
	m_pDebugView = new CDebugView();
#endif

	// Initialize the client script manager
	m_pClientScriptManager = new CClientScriptManager();

	// Initialize the events manager
	m_pEvents = new CEvents();

	// Initialize the network module, if it fails, exit
	if(!CNetworkModule::Init())
	{
		CLogFile::Printf("Failed to initialize the network module!\n");
		return false;
	}

	// Initialize the file transfer manager
	m_pFileTransfer = new CFileTransferManager();

	// Initialize the http client
	m_pHttpClient = new CHttpClient();
	m_pHttpClient->SetRequestTimeout(10000);
	m_pHttpClient->SetHost(MASTERLIST_ADDRESS);
	return true;
}

void CClient::OnUnload()
{
	// Delete our file transfer manager
	SAFE_DELETE(m_pFileTransfer);

	// Delete our camera
	SAFE_DELETE(m_pCamera);

	// Delete our model manager
	SAFE_DELETE(m_pModelManager);

	// Delete our pickup manager
	SAFE_DELETE(m_pPickupManager);

	// Delete our checkpoint manager
	SAFE_DELETE(m_pCheckpointManager);

	// Delete our object manager
	SAFE_DELETE(m_pObjectManager);

	// Delete our blip manager
	SAFE_DELETE(m_pBlipManager);

	// Delete our actor manager
	SAFE_DELETE(m_pActorManager);

	// Delete our vehicle manager
	SAFE_DELETE(m_pVehicleManager);

	// Delete our local player
	SAFE_DELETE(m_pLocalPlayer);

	// Delete our player manager
	SAFE_DELETE(m_pPlayerManager);

	// Delete our network manager
	SAFE_DELETE(m_pNetworkManager);

	// Delete our name tags
	SAFE_DELETE(m_pNameTags);

	// Delete our input window
	SAFE_DELETE(m_pInputWindow);

	// Delete our chat window
	SAFE_DELETE(m_pChatWindow);

	// Delete our fps counter
	SAFE_DELETE(m_pFPSCounter);

#ifdef IVMP_DEBUG
	// Delete our debug viewer
	SAFE_DELETE(m_pDebugView);
#endif

	// Delete our credits
	SAFE_DELETE(m_pCredits);

	// Delete our main menu
	SAFE_DELETE(m_pMainMenu);

	// Delete our GUI
	SAFE_DELETE(m_pGUI);

	// Delete our streamer class
	SAFE_DELETE(m_pStreamer);

	// Delete our time class
	SAFE_DELETE(m_pTime);

	// Delete our traffic lights
	SAFE_DELETE(m_pTrafficLights);

	// Delete our client script manager
	SAFE_DELETE(m_pClientScriptManager);

	// Delete our client task manager
	SAFE_DELETE(m_pClientTaskManager);

	// Delete our audio manager
	SAFE_DELETE(m_pAudioManager);

	// Delete our events manager
	SAFE_DELETE(m_pEvents);

	// Delete our 3D label manager
	SAFE_DELETE(m_p3DLabelManager);

	// Uninstall the Cursor hook
#ifdef IVMP_DEBUG
	//CCursorHook::Uninstall();
#endif

	// Uninstall the DirectInput hook
	CDirectInputHook::Uninstall();

	// Uninstall the Direct3D hook
	CDirect3DHook::Uninstall();

	// Shutdown our game
	CGame::Shutdown();

	// Close the settings file
	CSettings::Close();

	// Close the log file
	CLogFile::Close();

	// Uninstall the XLive hook
	//CXLiveHook::Uninstall(); // Not needed
}

void CClient::OnD3DEndScene()
{
	// Check for pause menu
	if(m_pNetworkManager)
	{
		if(CGame::IsMenuActive() && CGame::GetState() == GAME_STATE_INGAME && m_pNetworkManager->IsConnected())
			CGame::SetState(GAME_STATE_PAUSE_MENU);
		else if(!CGame::IsMenuActive() && CGame::GetState() == GAME_STATE_PAUSE_MENU && m_pNetworkManager->IsConnected())
			CGame::SetState(GAME_STATE_INGAME);
	}
	// Are we in the main menu?
	if(CGame::GetState() == GAME_STATE_MAIN_MENU || CGame::GetState() == GAME_STATE_LOADING)
	{
		// Is the main menu shown?
		if(CGame::IsMenuActive())
		{
			// Clear the screen
			m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0, 0);
		}
	}
	if(m_pMainMenu && CGame::GetState() == GAME_STATE_MAIN_MENU || CGame::GetState() == GAME_STATE_IVMP_PAUSE_MENU)
	{
		if(!m_pMainMenu->IsVisible())
		{
			if(m_pNetworkManager && m_pNetworkManager->IsConnected())
			{
				m_pMainMenu->SetDisconnectButtonVisible(true);
				m_pMainMenu->SetVisible(true);
			}
			else
			{
				m_pMainMenu->SetDisconnectButtonVisible(false);
				m_pMainMenu->SetVisible(true);
			}

			if(CGame::GetState() == GAME_STATE_MAIN_MENU)
			{
				String strPostPath("/getlatestversion.php");
				m_pHttpClient->Get(strPostPath);
			}
		}
	}
	else
	{
		if(m_pMainMenu->IsVisible())
			m_pMainMenu->SetVisible(false);
	}

	if(m_pClientScriptManager && m_pClientScriptManager->GetGUIManager())
	{
		if(CGame::GetState() != GAME_STATE_INGAME)
		{
			if(!m_pClientScriptManager->GetGUIManager()->IsHidden())
				m_pClientScriptManager->GetGUIManager()->Hide();
		}
		else
		{
			if(m_pClientScriptManager->GetGUIManager()->IsHidden())
				m_pClientScriptManager->GetGUIManager()->Show();
		}
	}

	// If our GUI class exists render it
	if(m_pGUI)
		m_pGUI->Render();

	// If our main menu exists process it
	if(m_pMainMenu)
		m_pMainMenu->Process();

	// If our chat window exist draw it
	if(m_pChatWindow && !CGame::IsMenuActive())
		m_pChatWindow->Draw();

	// If our fps class exists update it
	if(m_pFPSCounter)
		m_pFPSCounter->Pulse();

	// If our credits class exists process it
	if(m_pCredits)
		m_pCredits->Process();

	if(m_p3DLabelManager)
		m_p3DLabelManager->Render();

	// If our scripting manager exists, call the frame event
	if(m_pEvents && !m_pMainMenu->IsVisible())
		m_pEvents->Call("frameRender");

#ifdef IVMP_DEBUG
	if(m_pDebugView && m_pGUI && m_pLocalPlayer)
		m_pDebugView->Draw();
#endif

	// Check if our screen shot write failed
	if(CScreenShot::IsDone())
	{
		if(CScreenShot::HasSucceeded())
			m_pChatWindow->AddInfoMessage("Screen shot written (%s).", CScreenShot::GetWriteName().Get());
		else
			m_pChatWindow->AddInfoMessage("Screen shot write failed (%s).", CScreenShot::GetError().Get());

		CScreenShot::Reset();
	}
	
	if(CGame::GetState() == GAME_STATE_MAIN_MENU)
	{
		if(m_pHttpClient->IsBusy())
			m_pHttpClient->Process();

		if(m_pHttpClient->GotData())
		{
			// Get the data
			String * strData = m_pHttpClient->GetData();

			// Did we not get any data?
			if(strData->IsEmpty())
				return;

#ifndef IVMP_DEBUG
			if(strcmp(strData->Get(), MOD_VERSION_STRING))
			{
				if(m_pGUI)
					m_pGUI->ShowMessageBox(String("A new version of IV:MP is available (Version %s)", strData->Get()).Get(), "New Version available");
			}
#endif
		}
	}

	// Are we in game?
	if(CGame::GetState() == GAME_STATE_INGAME)
	{
		// Is F4 held down and do we have a network manager?
		if(GetAsyncKeyState(VK_F4) && m_pNetworkManager && m_pActorManager)
		{
			// Get the network statistics
			CNetStats * pNetStats = m_pNetworkManager->GetNetClient()->GetNetStats();

			// Convert the network statistics to a string
			char szNetworkStats[10000];
			pNetStats->ToString(szNetworkStats, 2);

			// Create the statistics string
			String strStats(szNetworkStats);

			// Append loaded and unloaded model counts to the stats
			// jenksta: too performance heavy to be done every frame
			//strStats.AppendF("Models (Loaded/Unload): %d/%d\n", CGame::GetLoadedModelCount(), CGame::GetUnloadedModelCount());

			// Append streamed in/out entity counts and streamed in limits to the stats
			strStats.AppendF("Players (StreamedIn/StreamedInLimit): %d/%d\n", m_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_PLAYER)+1, m_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_PLAYER));
			strStats.AppendF("Vehicles (StreamedIn/StreamedInLimit): %d/%d\n", m_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_VEHICLE), m_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_VEHICLE));
			strStats.AppendF("Actors (Actor-Count/Max-Actors): %d/%d\n", m_pActorManager->GetActorCount(), MAX_ACTORS);
			strStats.AppendF("Pickups (StreamedIn/StreamedInLimit): %d/%d\n", m_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_PICKUP), m_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_PICKUP));
			strStats.AppendF("Objects (StreamedIn/StreamedInLimit): %d/%d\n", m_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_OBJECT), m_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_OBJECT));
			strStats.AppendF("Checkpoints (StreamedIn/StreamedInLimit): %d/%d\n", m_pStreamer->GetStreamedInEntityCountOfType(STREAM_ENTITY_CHECKPOINT), m_pStreamer->GetStreamedInLimitOfType(STREAM_ENTITY_CHECKPOINT));

			// Draw the string
			m_pGUI->DrawText(strStats, CEGUI::Vector2(26, 30), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), m_pGUI->GetFont("tahoma-bold", 10));
			m_pChatWindow->SetEnabled(false);
			m_bNetworkStatsDisplayed = true;
		}
		else
		{
			// If our network stats were displayed
			if(m_bNetworkStatsDisplayed)
			{
				if(m_pChatWindow && !m_pChatWindow->IsEnabled())
					m_pChatWindow->SetEnabled(true);
				
				m_bNetworkStatsDisplayed = false;
			}

			// If our input window exists draw it
			if(m_pInputWindow)
				m_pInputWindow->Draw();
		}

		// Update the time
		if(m_pTime)
		{
			// jenksta: Setting this every frame causes the weather to change
			// rapidly
			unsigned char ucHour = 0, ucMinute = 0;
			m_pTime->GetTime(&ucHour, &ucMinute);
			CGame::SetTime(ucHour, ucMinute);
			CGame::SetDayOfWeek(m_pTime->GetDayOfWeek());
		}

#ifdef IVMP_DEBUG
		if(m_pVehicleManager)
		{
			CVector3 vecWorldPosition;
			Vector2 vecScreenPosition;

			for(EntityId i = 0; i < MAX_VEHICLES; i++)
			{
				CNetworkVehicle * pVehicle = m_pVehicleManager->Get(i);
				if(pVehicle)
				{
					if(!pVehicle->IsStreamedIn())
						continue;

					if(!pVehicle->IsOnScreen())
						continue;
					
					pVehicle->GetPosition(vecWorldPosition); 
					if(!CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition))
						continue;

							
					int iHealth = pVehicle->GetHealth();
					int iModel = pVehicle->GetEngineState();
					float fPetrol = pVehicle->GetPetrolTankHealth();
					CVector3 vecPos; pVehicle->GetPosition(vecPos);
					CVector3 vecRot; pVehicle->GetRotation(vecRot);
					m_pGUI->DrawText(String("VehicleId %d, Enginestate: %d, Health: %d, PetrolTankHealth: %f\nPosition(%.3f,%.3f,%.3f), Rot(%.3f,%.3f,%.3f)", i, iModel, iHealth, fPetrol, vecPos.fX,vecPos.fY,vecPos.fZ,vecRot.fX,vecRot.fY,vecRot.fZ), CEGUI::Vector2(vecScreenPosition.fX, vecScreenPosition.fY));
				}
			}
		}
#endif

		if(m_pNameTags)
			m_pNameTags->Draw();
	}
}

void CClient::OnD3DLostDevice()
{
	// If our gui instance exists inform it of the device loss
	if(m_pGUI)
		m_pGUI->OnLostDevice();

	// If our graphics instance exists inform it of the device loss
	if(m_pGraphics)
		m_pGraphics->OnLostDevice();
}

void CClient::OnD3DResetDevice()
{
	// If our audio manager does not exist create it
	if(!m_pAudioManager)
	{
		m_pAudioManager = new CAudioManager();

		if(!m_pAudioManager->Initialize())
			CLogFile::Printf("CAudioManager::Initialize failed");
	}

	// If our GUI instance does not exist create it
	if(!m_pGUI)
	{
		m_pGUI = new CGUI(m_pDevice);

		if(m_pGUI->Initialize())
		{
			// Version identifier text
			m_pVersionIdentifier = m_pGUI->CreateGUIStaticText();
			m_pVersionIdentifier->setText(VERSION_IDENTIFIER);
			CEGUI::Font * pFont = m_pGUI->GetFont("tahoma-bold");
			float fTextWidth = pFont->getTextExtent(VERSION_IDENTIFIER);
			float fTextHeight = pFont->getFontHeight();
			m_pVersionIdentifier->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
			float fTextX = pFont->getTextExtent("_");
			float fTextY = -(fTextX + fTextHeight);
			m_pVersionIdentifier->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fTextX), CEGUI::UDim(1, fTextY)));
			m_pVersionIdentifier->setProperty("FrameEnabled", "false");
			m_pVersionIdentifier->setProperty("BackgroundEnabled", "false");
			m_pVersionIdentifier->setFont(pFont);
			m_pVersionIdentifier->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
			m_pVersionIdentifier->setAlpha(0.6f);
			m_pVersionIdentifier->setVisible(true);

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
		m_pGUI->OnResetDevice();
	}

	// If our graphics instance does not exist create it
	if(!m_pGraphics && m_pDevice)
		m_pGraphics = new CGraphics(m_pDevice);
	else
		m_pGraphics->OnResetDevice();

	// If our main menu class does not exist create it
	if(!m_pMainMenu)
		m_pMainMenu = new CMainMenu();
	else
		m_pMainMenu->OnResetDevice();


	if(!m_p3DLabelManager)
		m_p3DLabelManager = new C3DLabelManager();

	//// Show loading screen
	//if(!CGame::IsGameLoaded() && !CGame::IsMenuActive())
	//	m_pMainMenu->ShowLoadingScreen();

	// If our credits class does not exist create it
	if(!m_pCredits)
		m_pCredits = new CCredits(m_pGUI);

	// If our fps counter class does not exist create it
	if(!m_pFPSCounter)
		m_pFPSCounter = new CFPSCounter();

	// If our fps counter class does not exist create it
	if(!m_pChatWindow)
		m_pChatWindow = new CChatWindow();

	// If our input window class does not exist create it
	if(!m_pInputWindow)
	{
		m_pInputWindow = new CInputWindow();
		RegisterCommands();
	}

	// If our name tags class does not exist create it
	if(!m_pNameTags)
		m_pNameTags = new CNameTags();
}

void CClient::OnGameLoad()
{
	// Initialize the pools
	CGame::GetPools()->Initialize();
	CLogFile::Printf("Initialized pools");

	// Reset the game
	InternalResetGame(true);
}

void CClient::OnGameProcess()
{
	// Do we need to reset the game?
	if(m_bResetGame)
	{
		// Reset the game
		InternalResetGame(m_bAutoConnect);

		// Flag as default value
		m_bAutoConnect = false;

		// Flag the game as no longer needed to reset
		m_bResetGame = false;
	}

	// If our network manager exists process it
	if(m_pNetworkManager)
		m_pNetworkManager->Process();

	// HACKY!
	// TEMP! TODO: Anywhere in GTA there's a function which checks if the engine is turned on or off...
	//		       ...If the player is in the vehicle, it will turn it automatic on -.-
	if(m_pLocalPlayer)
	{
		if(m_pLocalPlayer->GetVehicle())
		{
			// TEMP! TODO: Anywhere in GTA there's a function which checks if the engine is turned on or off...
			//		 ...If the player is in the vehicle, it will turn it automatic on -.-
			// jenksta: Then find all references to CVehicle::TurnEngineOn and find which call is for when 
			// the player enters the vehicle?
			if(!m_pLocalPlayer->GetVehicle()->GetEngineState())
				m_pLocalPlayer->GetVehicle()->SetEngineState(false);
		}
	}
	for(EntityId playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(m_pPlayerManager->DoesExist(playerId))
		{
			if(m_pPlayerManager->GetAt(playerId)->GetVehicle() != NULL)
			{
				if(!m_pPlayerManager->GetAt(playerId)->GetVehicle()->GetEngineState() != NULL)
					m_pPlayerManager->GetAt(playerId)->GetVehicle()->SetEngineState(false);
			}
		}
	}

	// If we have text to draw draw it
	if(iTextTime > 0)
	{
		if((int)(SharedUtility::GetTime() - dwTextStartTick) < iTextTime)
			Scripting::DisplayTextWithLiteralString(fTextPos[0], fTextPos[1], "STRING", strTextText);
		else
			iTextTime = 0;
	}
}

void CClient::ExceptionHandlerCallback(String& strReportData)
{
	// Get our crash reporter path
	String strApplicationPath = SharedUtility::GetAbsolutePath("Client.CrashReporter" DEBUG_SUFFIX ".exe ");

	// Generate the command line
	String strCommandLine("%s %s", strApplicationPath.Get(), strReportData.Get());

	// Start our crash reporter
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	if(!CreateProcess(strApplicationPath.Get(), (char *)strCommandLine.Get(), NULL, NULL, TRUE, NULL, NULL, SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo))
		CLogFile::Printf("Failed to start crash reporter.");
}

// (John) TODO: Only SAFE_DELETE and re-instantiate things that really need it, otherwise give them a Reset() method.
void CClient::InternalResetGame(bool bAutoConnect)
{
	CLogFile::Printf("Initializing game for multiplayer activities");

	// Remove local player from vehicle
	if(m_pLocalPlayer)
		m_pLocalPlayer->RemoveFromVehicle();

	// Remove all current GUIs
	m_pClientScriptManager->RemoveAll();
	m_pClientScriptManager->GetGUIManager()->DeleteAll();


	SAFE_DELETE(m_pClientScriptManager);
	m_pClientScriptManager = new CClientScriptManager();
	m_pEvents->clear();
	m_pGUI->SetScriptedCursorVisible(false);
	CLogFile::Printf("Reset clientside scripting stuff");

	SAFE_DELETE(m_pModelManager);
	m_pModelManager = new CModelManager();
	CLogFile::Printf("Created model manager instance");

	SAFE_DELETE(m_pPickupManager);
	m_pPickupManager = new CPickupManager();
	CLogFile::Printf("Created pickup manager instance");

	SAFE_DELETE(m_pCheckpointManager);
	m_pCheckpointManager = new CCheckpointManager();
	CLogFile::Printf("Created checkpoint manager instance");

	SAFE_DELETE(m_pBlipManager);
	m_pBlipManager = new CBlipManager();
	CLogFile::Printf("Created blip manager instance");

	SAFE_DELETE(m_pObjectManager);
	m_pObjectManager = new CObjectManager();
	CLogFile::Printf("Created object manager instance");

	SAFE_DELETE(m_pFireManager);
	m_pFireManager = new CFireManager();
	CLogFile::Printf("Created fire manager instance");

	SAFE_DELETE(m_p3DLabelManager);
	m_p3DLabelManager = new C3DLabelManager();
	CLogFile::Printf("Created 3d label manager instance");

	// Set all vehicles to destroyable
	if(m_pVehicleManager)
	{
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			CNetworkVehicle * pVehicle = m_pVehicleManager->Get(i);

			if(pVehicle)
				pVehicle->MarkAsActorVehicle(false);
		}
	}

	SAFE_DELETE(m_pVehicleManager);
	m_pVehicleManager = new CVehicleManager();
	CLogFile::Printf("Created vehicle manager instance");

	SAFE_DELETE(m_pActorManager);
	m_pActorManager = new CActorManager();
	CLogFile::Printf("Created actor manager instance");

	// Reset the streamer
	m_pStreamer->Reset();
	CLogFile::Printf("Reset streamer instance");

	if(!m_pLocalPlayer)
	{
		m_pLocalPlayer = new CLocalPlayer();
		CLogFile::Printf("Created local player instance");
	}

	m_pLocalPlayer->Reset();
	m_pLocalPlayer->SetPlayerId(INVALID_ENTITY_ID);
	m_pLocalPlayer->SetModel(Scripting::MODEL_PLAYER);
	m_pLocalPlayer->Teleport(CVector3());
	m_pLocalPlayer->SetPlayerControlAdvanced(false, false);
	m_pLocalPlayer->RemoveAllWeapons();
	m_pLocalPlayer->ResetMoney();
	m_pLocalPlayer->SetHealth(200);
	m_pLocalPlayer->SetControl(true);
	CLogFile::Printf("Reset local player instance");

	SAFE_DELETE(m_pPlayerManager);
	m_pPlayerManager = new CPlayerManager();
	CLogFile::Printf("Created player manager instance");

	// Create network manager if it doesn't exist
	if(!m_pNetworkManager)
	{
		m_pNetworkManager = new CNetworkManager();
		CLogFile::Printf("Created network manager instance");
	}
	
	// Reset network manager
	m_pNetworkManager->Reset();
	CLogFile::Printf("Reset network manager instance");

	// Clear our file transfer list
	m_pFileTransfer->Clear(true);

	// Reset game world
	m_pTime->SetDayOfWeek(2);
	m_pTime->SetMinuteDuration(0);
	m_pTrafficLights->Reset();
	m_pNameTags->SetEnabled(true);
	CGame::SetWantedLevel(0);
	CGame::SetHudVisible(false);
	CGame::SetRadarVisible(false);
	CGame::SetAreaNamesEnabled(false);
	CGame::ResetScrollBars();
	CGame::SetScrollBarColor();
	CGame::ToggleLazlowStation(true);

	//
	Scripting::SetScenarioPedDensityMultiplier(0, 0);
	Scripting::SetPedDensityMultiplier(0);
	Scripting::SetParkedCarDensityMultiplier(0);
	Scripting::SetRandomCarDensityMultiplier(0);

	// Set the time and weather after the camera set, one of the camera stuff changes the time and the weather
	CGame::GetWeather()->SetWeather(WEATHER_SUNNY);
	m_pTime->SetTime(0, 0);
	CGame::SetTime(0, 0);
	// SetCanBurstCarTyres(bool canburst);
	Scripting::SetMaxWantedLevel(0);
	Scripting::SetCreateRandomCops(false);
	// Test if this is needed (Script is unloaded, so it shouldn't be)
	Scripting::AllowStuntJumpsToTrigger(false);
	CLogFile::Printf("Reset world");

	// Destroy the camera and create it again
	SAFE_DELETE(m_pCamera);
	m_pCamera = new CCamera();

	// Mark the game as loaded.
	if(!m_bGameLoaded)
		m_bGameLoaded = true;

	CGame::SetGameLoaded(m_bGameLoaded);

	// Reset the network stats
	m_pMainMenu->ResetNetworkStats();

	// Auto-connect if needed, otherwise show the main menu.
	if(m_pNetworkManager && bAutoConnect)
		m_pNetworkManager->Connect();
	else
		m_pMainMenu->SetVisible(true);

	CLogFile::Printf("Sucessfully (re)initialized game for multiplayer activities");
}

// jenksta: NOTE: Can only be called with bResetNow = true from the game script thread
void CClient::ResetGame(bool bResetNow, bool bAutoConnect)
{
	// If requested reset the game now
	if(bResetNow)
		InternalResetGame(bAutoConnect);
	else
	{
		m_bResetGame = true;
		m_bAutoConnect = bAutoConnect;
	}
}

void CClient::ResetMainMenuCamera()
{
	SAFE_DELETE(m_pCamera);
	m_pCamera = new CCamera();
	
	CLogFile::Printf("Attempting to reset the Main Menu Camera...");
	if(m_pCamera)
	{
		m_pCamera->ActivateScriptCam();
		m_pCamera->SetPosition(CVector3(HAPPINESS_CAMERA_POS));
		m_pCamera->SetLookAt(CVector3(HAPPINESS_CAMERA_LOOK_AT),false);
		CLogFile::Printf("Successfully Reset the Main Menu Camera!");
	}
	else
		CLogFile::Printf("Failed to Reset the Main Menu Camera!");
}	
