//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMainMenu.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGUI.h"
#include "CServerBrowser.h"
#include "CQuickConnect.h"
#include "CSettingsMenu.h"

class CMainMenu
{
	friend class CServerBrowser;
	friend class CQuickConnect;
	friend class CSettingsMenu;

private:
	static CMainMenu    * m_pSingleton;
	CServerBrowser		* m_pServerBrowser;
	CQuickConnect		* m_pQuickConnect;
	CSettingsMenu		* m_pSettingsMenu;
	bool				m_bVisible;
	int					m_iCameraState;
	int					m_iCameraTime;
	bool				m_bDisconnectButtonVisible;

	// Main Menu Buttons
	CGUIStaticImage     * m_pLogo;
	CGUIStaticImage     * m_pBackground;
	CGUIStaticText      * m_pDisconnectButton;
	CGUIStaticText      * m_pServerBrowserButton;
	CGUIStaticText      * m_pQuickConnectButton;
	CGUIStaticText      * m_pSettingsButton;
	CGUIStaticText      * m_pAboutButton;
	CGUIStaticText      * m_pQuitButton;

	// Network stuff
	CGUIStaticText		* m_pHost;
	CGUIStaticText		* m_pPlayers;
	CGUIStaticText		* m_pMyName;
	CGUIStaticText		* m_pHostDesc;
	CGUIStaticText		* m_pPlayersDesc;
	CGUIStaticText		* m_pMyNameDesc;

	// Server connect handler
	void             OnConnect(String strHost, unsigned short usPort, String strPassword, bool bIgnoreDefaultName = false);
	static void      OnConnectMessageBoxResponse(eGUIMessageBoxResponse type);
	
	// Helpers
	static bool		 GetHostAndPort(String strHostAndPort, String& strHost, unsigned short& usPort);
	CGUIStaticText * CreateButton(char * szText, CEGUI::UVector2 vecSize, CEGUI::UVector2 vecPosition);

	// Main Menu events
	bool			 OnServerBrowserButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool			 OnServerBrowserButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool			 OnServerBrowserButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnDisconnectButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnDisconnectButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnDisconnectButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnSettingsButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnSettingsButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnSettingsButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnQuitButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnQuitButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnQuitButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnAboutButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnAboutButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnAboutButtonMouseClick(const CEGUI::EventArgs &eventArgs);

	
public:
	CMainMenu();
	~CMainMenu();

	static CMainMenu * GetSingleton() { return m_pSingleton; }
	void             Process();
	void             OnResetDevice();
	void             SetVisible(bool bVisible);
	bool             IsVisible() { return m_bVisible; }
	bool             IsDisconnectButtonVisible() { return m_bDisconnectButtonVisible; }
	void             SetDisconnectButtonVisible(bool bDisconnectButtonVisible);
	void			 SetNetworkStats(String strHost, int players, int maxplayers, String strName);
	void			 ResetNetworkStats();
	static void	     OnDirectConnectMessageBoxResponse(eGUIMessageBoxResponse type);
	void			 OnDirectConnect(String strHost, unsigned short usPort);

	// ShowMessageBox?
	void			 ShowMessageBox(const char * szMessage, const char * szHeader, bool bGameMenu, bool bResetGame, bool bAllowReconnect);
	CSettingsMenu *	GetSettingsMenu() { return m_pSettingsMenu; }
};