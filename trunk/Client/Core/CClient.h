//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClient.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CChatWindow.h"
#include "CInputWindow.h"
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
#include "CCamera.h"
#include "CGUI.h"
#include "CGraphics.h"
#include <Scripting\CScriptingManager.h>
#include "CClientScriptManager.h"
#include "CMainMenu.h"
#include "CFPSCounter.h"
#include "CDebugView.h"
#include "CFileTransferManager.h"
#include "CStreamer.h"
#include <Game\CTime.h>
#include <CEvents.h>
#include <Game\CTrafficLights.h>
#include "CCredits.h"
#include "CNameTags.h"
#include "CClientTaskManager.h"
#include "CFireManager.h"
#include "C3DLabels.h"
#include "CAudioManager.h"
#ifdef IVMP_WEBKIT
#include "CD3D9Webkit.hpp"
#endif

class CClient
{
private:
	CChatWindow          * m_pChatWindow;
	CInputWindow         * m_pInputWindow;
	CNetworkManager      * m_pNetworkManager;
	CPlayerManager       * m_pPlayerManager;
	CLocalPlayer         * m_pLocalPlayer;
	CVehicleManager      * m_pVehicleManager;
	CObjectManager       * m_pObjectManager;
	CBlipManager         * m_pBlipManager;
	CActorManager        * m_pActorManager;
	CCheckpointManager   * m_pCheckpointManager;
	CPickupManager       * m_pPickupManager;
	CModelManager        * m_pModelManager;
	CCamera              * m_pCamera;
	CGUI                 * m_pGUI;
	CGraphics            * m_pGraphics;
	CScriptingManager    * m_pScriptingManager;
	CClientScriptManager * m_pClientScriptManager;
	CMainMenu            * m_pMainMenu;
	CFPSCounter          * m_pFPSCounter;
	CDebugView			 * m_pDebugView;
	CGUIStaticText       * m_pVersionIdentifier;
	CFileTransferManager * m_pFileTransfer;
	CStreamer            * m_pStreamer;
	CTime                * m_pTime;
	CEvents              * m_pEvents;
	CTrafficLights       * m_pTrafficLights;
	CCredits             * m_pCredits;
	CNameTags            * m_pNameTags;
	CClientTaskManager   * m_pClientTaskManager;
	CFireManager		 * m_pFireManager;
	C3DLabelManager		 * m_p3DLabelManager;
	CAudioManager        * m_pAudioManager;

#ifdef IVMP_WEBKIT
	CD3D9WebKit          * m_pWebkit;
	CD3D9WebView         * m_pWebView;
#endif

	IDirect3DDevice9     * m_pDevice;
	bool		           m_bGameLoaded;
	bool                   m_bWindowedMode;
	bool                   m_bFPSToggle;
	unsigned short         m_usPort;
	String                 m_strHost;
	String                 m_strNick;
	String                 m_strPassword;
	int			           m_iCameraState;
	int			           m_iCameraTime;
	bool		           m_bNetworkStatsDisplayed;
	bool                   m_bResetGame;

	CHttpClient          * m_pHttpClient;

	void                   InternalResetGame(bool bAutoConnect);

public:
	CClient();
	~CClient();

	CChatWindow          * GetChatWindow() { return m_pChatWindow; }
	CInputWindow         * GetInputWindow() { return m_pInputWindow; }
	CNetworkManager      * GetNetworkManager() { return m_pNetworkManager; }
	CPlayerManager       * GetPlayerManager() { return m_pPlayerManager; }
	CLocalPlayer         * GetLocalPlayer() { return m_pLocalPlayer; }
	CVehicleManager      * GetVehicleManager() { return m_pVehicleManager; }
	CObjectManager       * GetObjectManager() { return m_pObjectManager; }
	CBlipManager         * GetBlipManager() { return m_pBlipManager; }
	CActorManager        * GetActorManager() { return m_pActorManager; }
	CCheckpointManager   * GetCheckpointManager() { return m_pCheckpointManager; }
	CPickupManager       * GetPickupManager() { return m_pPickupManager; }
	CModelManager        * GetModelManager() { return m_pModelManager; }
	CCamera              * GetCamera() { return m_pCamera; }
	CGUI                 * GetGUI() { return m_pGUI; }
	CGraphics            * GetGraphics() { return m_pGraphics; }
	CScriptingManager    * GetScriptingManager() { return m_pScriptingManager; }
	CClientScriptManager * GetClientScriptManager() { return m_pClientScriptManager; }
	CMainMenu            * GetMainMenu() { return m_pMainMenu; }
	CFPSCounter          * GetFPSCounter() { return m_pFPSCounter; }
	CDebugView			 * GetDebugView() { return m_pDebugView; }
	CGUIStaticText       * GetVersionIdentifier() { return m_pVersionIdentifier; }
	CFileTransferManager * GetFileTransfer() { return m_pFileTransfer; }
	CStreamer            * GetStreamer() { return m_pStreamer; }
	CTime                * GetTime() { return m_pTime; }
	CEvents              * GetEvents() { return m_pEvents; }
	CTrafficLights       * GetTrafficLights() { return m_pTrafficLights; }
	CCredits             * GetCredits() { return m_pCredits; }
	CNameTags            * GetNameTags() { return m_pNameTags; }
	CClientTaskManager   * GetClientTaskManager() { return m_pClientTaskManager; }
	CFireManager		 * GetFireManager() { return m_pFireManager; }
	C3DLabelManager		 * Get3DLabelManager() { return m_p3DLabelManager; }
	CAudioManager        * GetAudioManager() { return m_pAudioManager; }

#ifdef IVMP_WEBKIT
	//CD3D9WebKit        * GetWebKit() { return m_pWebkit; }
	//CD3D9WebView       * GetWebView() { return m_pWebView; }
#endif

	void                   SetDevice(IDirect3DDevice9 * pDevice) { m_pDevice = pDevice; }
	IDirect3DDevice9     * GetDevice() { return m_pDevice; }
	void                   SetGameLoaded(bool bGameLoaded) { m_bGameLoaded = bGameLoaded; }
	bool                   IsGameLoaded() { return m_bGameLoaded; }
	void                   SetWindowedMode(bool bWindowedMode) { m_bWindowedMode = bWindowedMode; }
	bool                   IsWindowedMode() { return m_bWindowedMode; }
	void                   SetFPSToggle(bool bFPSToggle) { m_bFPSToggle = bFPSToggle; }
	bool                   GetFPSToggle() { return m_bFPSToggle; }
	void                   SetPort(unsigned short usPort) { m_usPort = usPort; }
	unsigned short         GetPort() { return m_usPort; }
	void                   SetHost(String strHost) { m_strHost = strHost; }
	String                 GetHost() { return m_strHost; }
	void                   SetNick(String strNick) { m_strNick = strNick; }
	String                 GetNick() { return m_strNick; }
	void                   SetPassword(String strPassword) { m_strPassword = strPassword; }
	String                 GetPassword() { return m_strPassword; }

	bool                   OnLoad();
	void                   OnUnload();
	void                   OnD3DEndScene();
	void                   OnD3DLostDevice();
	void                   OnD3DResetDevice();
	void                   OnGameLoad();
	void                   OnGameProcess();
	static void            ExceptionHandlerCallback(String& strReportData);

	void                   ResetGame(bool bResetNow = false, bool bAutoConnect = false);
};
