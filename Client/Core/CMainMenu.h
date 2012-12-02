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
#include "CMasterListQuery.h"
#include "CServerQuery.h"

// Custom ListboxTextItem class to automatically set the selection brush image on creation
class ServerBrowserListItem : public CEGUI::ListboxTextItem
{
public:
	ServerBrowserListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionBrushImage("WindowsLook", "MultiListSelectionBrush");
	}
};

class CMainMenu
{
private:
	// 
	static CMainMenu    * m_pSingleton;
	bool                  m_bVisible;
	int					  m_bCameraState;
	int					  m_bCameraStateTime;
	bool                  m_bDisconnectButtonVisible;
	bool                  m_bQuickConnectWindowVisible;
	bool                  m_bSettingsWindowVisible;
	bool                  m_bServerBrowserWindowVisible;
	bool                  m_bServerBrowserWindowOpen;
	bool                  m_bQuickConnectWindowOpen;
	bool                  m_bSettingsWindowOpen;
	bool				  m_bLoadingScreenActive;
	CMasterListQuery    * m_pMasterListQuery;
	CServerQuery        * m_pServerQuery;

	// Main Menu
	CGUIStaticImage     * m_pLogo;
	CGUIStaticImage     * m_pBackground;
	CGUIStaticText      * m_pDisconnectButton;
	CGUIStaticText      * m_pServerBrowserButton;
	CGUIStaticText      * m_pQuickConnectButton;
	CGUIStaticText      * m_pSettingsButton;
	CGUIStaticText      * m_pQuitButton;
	CGUIStaticText      * m_pAboutButton;
	CGUIStaticText		* m_pHost;
	CGUIStaticText		* m_pPlayers;
	CGUIStaticText		* m_pMyName;
	CGUIStaticText		* m_pHostDesc;
	CGUIStaticText		* m_pPlayersDesc;
	CGUIStaticText		* m_pMyNameDesc;

	//Loading Stuff
	CGUIStaticImage		* m_pLoadingLogo;
	CGUIStaticImage		* m_pRaknetLogo;
	CGUIStaticText		* m_pLoadingText;
	CGUIStaticImage		* m_pLoadingBackground;

	// Server browser window
	struct
	{
		CGUIFrameWindow     * pWindow;
		CGUIMultiColumnList * pServerMultiColumnList;
		CGUIButton          * pRefreshButton;
		CGUIButton          * pFeaturedButton;
		CGUIButton          * pSponsoredButton;
		CGUIButton          * pConnectButton;
	}                     m_serverBrowser;

	// Quick connect window
	CGUIFrameWindow * m_pQuickConnectWindow;
	CGUIStaticText * m_pQuickConnectWindowIPStaticText;
	CGUIEditBox * m_pQuickConnectWindowIPEditBox;
	CGUIStaticText * m_pQuickConnectWindowPasswordStaticText;
	CGUIEditBox * m_pQuickConnectWindowPasswordEditBox;
	CGUIButton * m_pQuickConnectWindowConnectButton;

	// Settings window
	CGUIFrameWindow * m_pSettingsWindow;
	CGUITabControl * m_pSettingsTabControl;
	CGUITabContentPane * m_pSettingsWindowGeneral;
	CGUITabContentPane * m_pSettingsWindowChat;
	CGUIStaticText * m_pSettingsWindowNickStaticText;
public: // Below member is public (For disallowed nick changes)
	CGUIEditBox * m_pSettingsWindowNickEditBox;
private:
	CGUIStaticText * m_pSettingsWindowWindowedStaticText;
	CGUICheckBox * m_pSettingsWindowWindowedCheckBox;
	CGUIStaticText * m_pSettingsWindowFPSStaticText;
	CGUICheckBox * m_pSettingsWindowFPSCheckBox;
	CGUIStaticText * m_pSettingsWindowChatFontSizeStaticText;
	CGUIStaticText * m_pSettingsWindowChatBackgroundStaticText;
	CGUIEditBox * m_pSettingsWindowChatFont;
	CGUIEditBox * m_pSettingsWindowChatFontSize;
	CGUIEditBox * m_pSettingsWindowChatBGColorA;
	CGUIEditBox * m_pSettingsWindowChatBGColorR;
	CGUIEditBox * m_pSettingsWindowChatBGColorG;
	CGUIEditBox * m_pSettingsWindowChatBGColorB;
	CGUIButton * m_pSettingsWindowSaveButton;

	// Main Menu events
	bool             OnServerBrowserButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseEnter(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseLeave(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectButtonMouseClick(const CEGUI::EventArgs &eventArgs);
	bool			 OnQuickConnectIPEditBoxKeyUp(const CEGUI::EventArgs &eventArgs);
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

	// Server connect handler
	static void      OnConnectMessageBoxResponse(eGUIMessageBoxResponse type);
	void             OnConnect(String strHost, unsigned short usPort, String strPassword, bool bIgnoreDefaultName = false);

	// Server browser window events
	bool             OnServerBrowserWindowCloseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserWindowRowClick(const CEGUI::EventArgs &eventArgs);
	static void      ServerQueryHandler(String strHost, unsigned short usPort, String strQuery, CBitStream * pReply);
	static void      MasterListQueryHandler(std::vector<String> serverVector);
	void             OnMasterListQuery(int iType);
	bool             OnServerBrowserWindowRefreshButtonClick(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserWindowFeaturedButtonClick(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserWindowSponsoredButtonClick(const CEGUI::EventArgs &eventArgs);
	bool             OnServerBrowserWindowConnectButtonClick(const CEGUI::EventArgs &eventArgs);


	// Quick connect window events
	bool             OnQuickConnectWindowCloseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnQuickConnectWindowConnectButtonClick(const CEGUI::EventArgs &eventArgs);


	// Settings window events
	bool             OnSettingsWindowCloseClick(const CEGUI::EventArgs &eventArgs);
	bool             OnSettingsWindowSaveButtonClick(const CEGUI::EventArgs &eventArgs);

	// 
	CGUIStaticText * CreateButton(char * szText, CEGUI::UVector2 vecSize, CEGUI::UVector2 vecPosition);

public:
	CMainMenu();
	~CMainMenu();

	static CMainMenu * GetSingleton() { return m_pSingleton; }
	bool             IsVisible() { return m_bVisible; }
	bool             IsDisconnectButtonVisible() { return m_bDisconnectButtonVisible; }
	void             SetVisible(bool bVisible);
	void             SetDisconnectButtonVisible(bool bDisconnectButtonVisible);
	void             SetServerBrowserWindowVisible(bool bVisible);
	bool             IsServerBrowserWindowVisible() { return m_bServerBrowserWindowVisible; }
	void             SetQuickConnectWindowVisible(bool bVisible);
	bool             IsQuickConnectWindowVisible() { return m_bQuickConnectWindowVisible; }
	void             SetSettingsWindowVisible(bool bVisible);
	bool             IsSettingsWindowVisible() { return m_bSettingsWindowVisible; }
	void             OnResetDevice();
	void             Process();
	void			 ShowLoadingScreen();
	void			 HideLoadingScreen();
	bool			 IsLoadingScreenActive() { return m_bLoadingScreenActive; };
	void			 SetNetworkStats(String strHost, int players, int maxplayers, String strName);
	void			 ResetNetworkStats();
	void			 ShowMessageBox(const char * szMessage, const char * szHeader, bool bGameMenu, bool bResetGame, bool bAllowReconnect);
};
