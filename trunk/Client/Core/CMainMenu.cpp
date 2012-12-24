//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMainMenu.cpp
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
//            Matthias
// License: See LICENSE in root directory
//
//==============================================================================

#include "CMainMenu.h"
#include "CNetworkManager.h"
#include <CSettings.h>
#include "CGraphics.h"
#include "CChatWindow.h"
#include <Network/CHttpClient.h>
#include <CLogFile.h>
#include "CCredits.h"
#include <map>
#include "CGame.h"
//#include "CD3D9Webkit.hpp"

extern CGUI            * g_pGUI;
extern CNetworkManager * g_pNetworkManager;
extern CChatWindow     * g_pChatWindow;
extern bool              g_bWindowedMode;
extern bool              g_bFPSToggle;
extern unsigned short    g_usPort;
extern String            g_strHost;
extern String            g_strNick;
extern String            g_strPassword;
extern CCredits        * g_pCredits;
extern bool				 g_bGameLoaded;
//extern CD3D9WebKit     * g_pWebkit;

CMainMenu                         * CMainMenu::m_pSingleton = NULL;
std::map<String, unsigned long>     serverPingStartMap;
extern std::vector<CEGUI::Window *> g_pGUIElements;

void ResetGame(bool now);
void InternalResetGame(bool bToggle);

bool CMainMenu::OnServerBrowserButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pServerBrowserButton->setAlpha(0.5);
	return true;
}

bool CMainMenu::OnServerBrowserButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pServerBrowserButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnServerBrowserButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	SetServerBrowserWindowVisible(true);
	return true;
}

bool CMainMenu::OnQuickConnectButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pQuickConnectButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnQuickConnectButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pQuickConnectButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnQuickConnectButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	SetQuickConnectWindowVisible(true);
	return true;
}

bool CMainMenu::OnDisconnectButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pDisconnectButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnDisconnectButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pDisconnectButton->setAlpha(1);
	return true;
}

void CMainMenu::SetDisconnectButtonVisible(bool bDisconnectButtonVisible)
{
	m_pDisconnectButton->setVisible(bDisconnectButtonVisible);
	m_bDisconnectButtonVisible = bDisconnectButtonVisible;
}

bool CMainMenu::OnDisconnectButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	if(g_pNetworkManager && g_pNetworkManager->IsConnected())
	{
		g_pChatWindow->SetEnabled(true);
		for(int i = 0; i < 10; i++)
			g_pChatWindow->AddMessage(0xFFFFFFAA,false," ");

		ShowMessageBox("Successfully disconnected from server ...","Disconnect",true,false,false);
		InternalResetGame(false);
		Sleep(500);
		SetDisconnectButtonVisible(false);

		//m_pBackground->setAlpha(0.1f);
	}
	return true;
}

bool CMainMenu::OnSettingsButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pSettingsButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnSettingsButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pSettingsButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnSettingsButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	SetSettingsWindowVisible(true);
	return true;
}

bool CMainMenu::OnQuitButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pQuitButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnAboutButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pAboutButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnQuitButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pQuitButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnAboutButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pAboutButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnAboutButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	g_pCredits->Start();
	return true;
}

bool CMainMenu::OnQuitButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	// If we are connected to a server, disconnect
	if(g_pNetworkManager && g_pNetworkManager->IsConnected())
		g_pNetworkManager->Disconnect();
	
	// Delete the network manager interface
	SAFE_DELETE(g_pNetworkManager);

	// Delete the webkit interface
	// SAFE_DELETE(g_pWebkit);

	// Exit
	ExitProcess(0);
	return true;
}

// Server connect handler

void CMainMenu::OnConnectMessageBoxResponse(eGUIMessageBoxResponse type)
{
	// Was the yes button clicked?
	if(type == GUI_MESSAGEBOX_YES)
	{
		// Show the settings window
		CMainMenu::GetSingleton()->SetSettingsWindowVisible(true);
	}
	else if(type == GUI_MESSAGEBOX_NO)
	{
		// Connect anyway
		CMainMenu::GetSingleton()->OnConnect(g_strHost, g_usPort, g_strPassword, true);
	}
}

void CMainMenu::OnConnect(String strHost, unsigned short usPort, String strPassword, bool bIgnoreDefaultName)
{
	// Ensure we have a name set
	if(g_strNick.IsEmpty())
	{
		g_pGUI->ShowMessageBox("You must have a name set!\nPress 'Ok' to open the 'Settings' menu.", "Error", GUI_MESSAGEBOXTYPE_OK, OnConnectMessageBoxResponse);
		return;
	}

	// Set the host
	g_strHost = strHost;

	// Set the port
	g_usPort = usPort;

	// Set the password
	g_strPassword = strPassword;

	// Ensure they have changed from the default name and we are not ignoring default name
	if(CVAR_GET_STRING("nick") == "player" && !bIgnoreDefaultName)
	{
		g_pGUI->ShowMessageBox("Your current name \"player\" is the default name.\nDo you wish to change your name?.", "Warning", GUI_MESSAGEBOXTYPE_YESNO, OnConnectMessageBoxResponse);
		return;
	}

	// Are we already connected?
	if(g_pNetworkManager && g_pNetworkManager->IsConnected())
	{
		// Reset the game
		ResetGame(false);

		// Disable the menu
		CGame::SetState(GAME_STATE_INGAME);
		g_pChatWindow->SetEnabled(true);
	}
	else if(g_pNetworkManager && !g_pNetworkManager->IsConnected() && g_bGameLoaded == true)
	{
		// Just connect to the server
		SAFE_DELETE(g_pNetworkManager);
		g_pNetworkManager = new CNetworkManager();

		g_pNetworkManager->Startup(g_strHost,g_usPort,g_strPassword);
		g_pNetworkManager->Connect();

		// Disable the menu
		CGame::SetState(GAME_STATE_INGAME);
		g_pChatWindow->SetEnabled(true);
	}
	else
	{
		CPatcher::Unprotect((CGame::GetBase() + 0x119DB14), 1);
		*(BYTE*)(CGame::GetBase() + 0x119DB14) = 1;

		// Disable the menu and load the game
		CGame::SetState(GAME_STATE_LOADING);
		ShowLoadingScreen();
	}
}

// Server Browser Window

bool CMainMenu::OnServerBrowserWindowCloseClick(const CEGUI::EventArgs &eventArgs)
{
	SetServerBrowserWindowVisible(false);
	return true;
}

bool CMainMenu::OnServerBrowserWindowRowClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_serverBrowser.pServerMultiColumnList;
	m_serverBrowser.pConnectButton->setEnabled((pMultiColumnList->getSelectedCount() > 0) ? true : false);
	return true;
}

void CMainMenu::ServerQueryHandler(String strHost, unsigned short usPort, String strQuery, CBitStream * pReply)
{
	// Read the query type
	char cQueryType;

	if(!pReply->Read(cQueryType))
		return;

	// Get the server host and port
	String strHostAndPort("%s:%d", strHost.Get(), usPort);

	if(cQueryType == 'p') // Ping
	{
		// Get the start time from the ping map
		unsigned long ulStartTime = serverPingStartMap[strHostAndPort];

		// Do we have a valid start time?
		if(ulStartTime > 0)
		{
			// Calculate the round trip time
			unsigned long ulPing = (SharedUtility::GetTime() - ulStartTime);

			// Set the server ping in the multi column list
			CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)CMainMenu::GetSingleton()->m_serverBrowser.pServerMultiColumnList;

			for(unsigned int i = 0; i < pMultiColumnList->getRowCount(); i++)
			{
				CEGUI::ListboxItem * pHost = pMultiColumnList->getItemAtGridReference(CEGUI::MCLGridRef(i, 1));

				if(!strHostAndPort.Compare(pHost->getText().c_str()))
				{
					CEGUI::ListboxItem * pPing = pMultiColumnList->getItemAtGridReference(CEGUI::MCLGridRef(i, 4));

					if(pPing)
					{
						char szTempBuf[64];
						pPing->setText(itoa(ulPing, szTempBuf, 10));
						pMultiColumnList->invalidate();
						break;
					}
				}
			}
		}
	}
	else 
	{
		// Read the host name
		String strHostName;

		if(cQueryType != 'i')
			return;

		if(!pReply->Read(strHostName))
			return;

		// Read the player count
		int iPlayerCount;

		if(!pReply->Read(iPlayerCount))
			return;

		// Read the max player limit
		int iMaxPlayers;

		if(!pReply->Read(iMaxPlayers))
			return;

		// Read if the server is passworded or not
		bool bPassworded;

		if(!pReply->Read(bPassworded))
			return;

		// Add the server to the multi column list
		CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)CMainMenu::GetSingleton()->m_serverBrowser.pServerMultiColumnList;
		unsigned int iRowIndex = pMultiColumnList->addRow();
		pMultiColumnList->setItem(new ServerBrowserListItem(strHostName.Get()), 0, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem(strHostAndPort.Get()), 1, iRowIndex);
		char szTempBuf[64];
		pMultiColumnList->setItem(new ServerBrowserListItem("-1"), 2, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem(itoa(iMaxPlayers, szTempBuf, 10)), 3, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem("-1"), 4, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem(bPassworded ? "Yes" : "No"), 5, iRowIndex);
		pMultiColumnList->invalidate();

		// Save the current time to the ping map
		serverPingStartMap[strHostAndPort] = SharedUtility::GetTime();

		// Send a ping query to the server
		CMainMenu::GetSingleton()->m_pServerQuery->Query(strHost, usPort, "p");
	}
}

void CMainMenu::MasterListQueryHandler(std::vector<String> serverVector)
{
	// Loop through all servers
	for(std::vector<String>::iterator iter = serverVector.begin(); iter != serverVector.end(); iter++)
	{
		// Get the address
		String strAddress = (*iter);

		// Get the ip and port
		size_t sSplit = strAddress.Find(':', 0);
		String strIp = strAddress.SubStr(0, sSplit++);
		String strPort = strAddress.SubStr(sSplit, (strAddress.GetLength() - sSplit));

		// Do we have a valid ip and port?
		if(strIp.IsEmpty() || strPort.IsEmpty())
			continue;

		// Query the server
		CMainMenu::GetSingleton()->m_pServerQuery->Query(strIp, strPort.ToInteger(), "i");
	}
}

void CMainMenu::OnMasterListQuery(int iType)
{
	// Reset the server browser list
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_serverBrowser.pServerMultiColumnList;
	pMultiColumnList->resetList();

	// Reset the master list query
	m_pMasterListQuery->Reset();

	// Reset the server query
	m_pServerQuery->Reset();

	// Query the master list
	if(!m_pMasterListQuery->Query(iType))
	{
		String strError("Failed to contact the master list (%s).\nPlease check your internet connection.", CMainMenu::GetSingleton()->m_pMasterListQuery->GetHttpClient()->GetLastErrorString().Get());
		g_pGUI->ShowMessageBox(strError.Get(), "Error");
	}
}

bool CMainMenu::OnServerBrowserWindowRefreshButtonClick(const CEGUI::EventArgs &eventArgs)
{
	// Query the master list
	OnMasterListQuery(0);
	return true;
}

bool CMainMenu::OnServerBrowserWindowSponsoredButtonClick(const CEGUI::EventArgs &eventArgs)
{
	// Query the master list
	OnMasterListQuery(1);
	return true;
}

bool CMainMenu::OnServerBrowserWindowFeaturedButtonClick(const CEGUI::EventArgs &eventArgs)
{
	// Query the master list
	OnMasterListQuery(2);
	return true;
}

bool GetHostAndPort(String strHostAndPort, String& strHost, unsigned short& usPort)
{
	// Find the split
	unsigned int uiSplit = strHostAndPort.Find(":");

	// Did we not find the split?
	if(uiSplit == String::nPos)
	{
		// Set the host
		strHost = strHostAndPort;
		
		// Set the default port (9999)
		usPort = 9999;
	}
	else
	{
		// Get and set the host
		strHost = strHostAndPort.SubStr(0, uiSplit);

		// Get the port
		String strPort = strHostAndPort.SubStr(uiSplit + 1);

		// Ensure the port is valid
		if(!strPort.IsNumeric())
			return false;

		// Set the port
		usPort = strPort.ToInteger();
	}
	return true;
}

bool CMainMenu::OnServerBrowserWindowConnectButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_serverBrowser.pServerMultiColumnList;
	CEGUI::ListboxItem * pHostname = pMultiColumnList->getFirstSelectedItem();
	CEGUI::ListboxItem * pHost = pMultiColumnList->getNextSelected(pHostname);
	CEGUI::ListboxItem * pPlayerCount = pMultiColumnList->getNextSelected(pHost);
	CEGUI::ListboxItem * pMaxPlayers = pMultiColumnList->getNextSelected(pPlayerCount);
	CEGUI::ListboxItem * pPing = pMultiColumnList->getNextSelected(pMaxPlayers);
	CEGUI::ListboxItem * pPassworded = pMultiColumnList->getNextSelected(pPing);


	// Get the host and port
	String strHost;
	unsigned short usPort;

	if(!GetHostAndPort(pHost->getText().c_str(), strHost, usPort))
	{
		g_pGUI->ShowMessageBox("You must enter a valid host and port!", "Error!");
		return false;
	}

	// Hide the server browser window
	SetServerBrowserWindowVisible(false);

	// Does this server require a password?
	if(pPassworded->getText() == "yes")
	{
		// Show password entry window
	}
	else
	{
		// Call the connect function
		OnConnect(strHost, usPort, "");
	}

	return true;
}

// Quick Connect Window

bool CMainMenu::OnQuickConnectWindowCloseClick(const CEGUI::EventArgs &eventArgs)
{
	SetQuickConnectWindowVisible(false);
	return true;
}


bool CMainMenu::OnQuickConnectIPEditBoxKeyUp(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs& key_args = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	if(key_args.scancode == CEGUI::Key::Return) {
		String strHost;
		unsigned short usPort;

		if(!GetHostAndPort(m_pQuickConnectWindowIPEditBox->getText().c_str(), strHost, usPort))
		{
			g_pGUI->ShowMessageBox("You must enter a valid host and port.", "Error");
			return false;
		}

		// Get the password
		String strPassword(m_pQuickConnectWindowPasswordEditBox->getText().c_str());

		// Set the ip, port and password
		CVAR_SET_STRING("ip", strHost);
		CVAR_SET_INTEGER("port", usPort);
		CVAR_SET_STRING("pass", strPassword);

		// Hide the quick connect window
		SetQuickConnectWindowVisible(false);

		// Call the connect function
		OnConnect(strHost, usPort, strPassword);
		return true;
	}
	return true;
}

bool CMainMenu::OnQuickConnectWindowConnectButtonClick(const CEGUI::EventArgs &eventArgs)
{
	// Get the host and port
	String strHost;
	unsigned short usPort;

	if(!GetHostAndPort(m_pQuickConnectWindowIPEditBox->getText().c_str(), strHost, usPort))
	{
		g_pGUI->ShowMessageBox("You must enter a valid host and port.", "Error");
		return false;
	}

	// Get the password
	String strPassword(m_pQuickConnectWindowPasswordEditBox->getText().c_str());

	// Set the ip, port and password
	CVAR_SET_STRING("ip", strHost);
	CVAR_SET_INTEGER("port", usPort);
	CVAR_SET_STRING("pass", strPassword);

	// Hide the quick connect window
	SetQuickConnectWindowVisible(false);

	// Call the connect function
	OnConnect(strHost, usPort, strPassword);
	return true;
}

// Settings Window

bool CMainMenu::OnSettingsWindowCloseClick(const CEGUI::EventArgs &eventArgs)
{
	SetSettingsWindowVisible(false);
	return true;
}

bool CMainMenu::OnSettingsWindowSaveButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::Checkbox * pCheckBox = (CEGUI::Checkbox *)m_pSettingsWindowWindowedCheckBox;
	bool bWindowed = pCheckBox->isSelected();
	CEGUI::Checkbox * pCheckBox2 = (CEGUI::Checkbox *)m_pSettingsWindowFPSCheckBox;
	g_bFPSToggle = pCheckBox2->isSelected();
	String strFont(m_pSettingsWindowChatFont->getText().c_str());
	String strFontSize(m_pSettingsWindowChatFontSize->getText().c_str());
	String strBackgroundAlpha(m_pSettingsWindowChatBGColorA->getText().c_str());
	String strBackgroundRed(m_pSettingsWindowChatBGColorR->getText().c_str());
	String strBackgroundGreen(m_pSettingsWindowChatBGColorG->getText().c_str());
	String strBackgroundBlue(m_pSettingsWindowChatBGColorB->getText().c_str());

	if(bWindowed != g_bWindowedMode)
		g_pGUI->ShowMessageBox("You must restart IV:MP for the windowed mode option to take effect", "Information.");

	CVAR_SET_BOOL("windowed", bWindowed);
	CVAR_SET_BOOL("fps", g_bFPSToggle);
	CVAR_SET_INTEGER("chatfont", strFont.ToInteger());
	CVAR_SET_INTEGER("chatsize", strFontSize.ToInteger());
	CVAR_SET_INTEGER("chatbga", strBackgroundAlpha.ToInteger());
	CVAR_SET_INTEGER("chatbgr", strBackgroundRed.ToInteger());
	CVAR_SET_INTEGER("chatbgg", strBackgroundGreen.ToInteger());
	CVAR_SET_INTEGER("chatbgb", strBackgroundBlue.ToInteger());

	// Init the chat window font and background
	g_pChatWindow->InitFontAndBackground();

	// Hide the settings window
	SetSettingsWindowVisible(false);

	// Are we connected to a server and have we changed our name?
	bool bNameChanged = (CVAR_GET_STRING("nick").Compare(m_pSettingsWindowNickEditBox->getText().c_str()) != 0);

	if(bNameChanged)
	{
		CVAR_SET_STRING("nick", m_pSettingsWindowNickEditBox->getText().c_str());
		g_strNick.Set(m_pSettingsWindowNickEditBox->getText().c_str());
	}

	if(g_pNetworkManager && g_pNetworkManager->IsConnected() && bNameChanged)
	{
		// Send the name change request
		CBitStream bsSend;
		bsSend.Write(String(m_pSettingsWindowNickEditBox->getText().c_str()));
		g_pNetworkManager->RPC(RPC_NameChange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
	}

	return true;
}

CMainMenu::CMainMenu()
{
	// Set our singleton pointer
	m_pSingleton = this;

	// Reset all visible and open flags
	m_bVisible = false;
	m_bServerBrowserWindowVisible = false;
	m_bQuickConnectWindowVisible = false;
	m_bSettingsWindowVisible = false;
	m_bServerBrowserWindowOpen = false;
	m_bQuickConnectWindowOpen = false;
	m_bSettingsWindowOpen = false;
	m_bDisconnectButtonVisible = false;

	// Create the master list query instance
	m_pMasterListQuery = new CMasterListQuery(MASTERLIST_ADDRESS, MASTERLIST_VERSION);

	// Set the master list query handler
	m_pMasterListQuery->SetMasterListQueryHandler(MasterListQueryHandler);

	// Create the server query instance
	m_pServerQuery = new CServerQuery();

	// Set the server query handler
	m_pServerQuery->SetServerQueryHandler(ServerQueryHandler);

	// jenksta: why was this put here?
	g_pGUI->GetDefaultWindow()->setAlpha(1.0f);

	// Reset the show stuff
	m_bCameraState = 1;
	m_bCameraStateTime = 1;

	// Main Menu
	float fWidth = (float)g_pGUI->GetDisplayWidth();
	float fHeight = (float)g_pGUI->GetDisplayHeight();
	float fX = -2.0f;
	float fY = 0.5f;

	// try to load all images
	try
	{
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Background", "IVMPBackgroundNetwork.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("BackgroundLoading", "IVMPBackground.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Logo", "IVMPLogoHiRes.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("LoadingLogo1", "IVMPLoadingLogoHiRes.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("MenuLogo", "IVMPMainMenuLogo.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("MenuLogoNetwork", "IVMPMainMenuLogoNetwork.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("RakNet", "RakNet.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Download", "Download.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Chat", "Chat.png");
	}
	catch(CEGUI::InvalidRequestException e)
	{
		String strFile = e.getMessage().c_str();
		strFile = strFile.SubStr(strFile.Find(" - ")+3, (unsigned int)-1);
		String str("IV:MP failed to load. (%s)", strFile.Get());
		MessageBox(NULL, str.C_String(), "IV:MP Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
	catch(CEGUI::Exception e)
	{
		MessageBox(NULL, "IV:MP failed to load. Check CEGUI.log for details.", "IV:MP Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	//IV:MP Menu Background
	m_pBackground = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
	m_pBackground->setProperty("FrameEnabled", "false");
	m_pBackground->setProperty("BackgroundEnabled", "false");
	m_pBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pBackground->setProperty("Image", "set:Background image:full_image");
	m_pBackground->setProperty("InheritsAlpha","false");
	m_pBackground->setAlpha(0.975f);
	m_pBackground->setVisible(false);

	//IV:MP Loading Background
	m_pLoadingBackground = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
	m_pLoadingBackground->setProperty("FrameEnabled", "false");
	m_pLoadingBackground->setProperty("BackgroundEnabled", "false");
	m_pLoadingBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pLoadingBackground->setProperty("Image", "set:BackgroundLoading image:full_image");
	m_pLoadingBackground->setProperty("InheritsAlpha","false");
	m_pLoadingBackground->setAlpha(0.90f);
	m_pLoadingBackground->setVisible(false);

	// Loadingscreen stuff
	float height = fWidth/7;
	float width = fWidth/(float)1.6; // (float) disable warning (double into ?)

	//IV:MP Loading Logo
	m_pLoadingLogo = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
	m_pLoadingLogo->setProperty("FrameEnabled", "false");
	m_pLoadingLogo->setProperty("BackgroundEnabled", "false");
	m_pLoadingLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pLoadingLogo->setProperty("Image", "set:LoadingLogo1 image:full_image");
	m_pLoadingLogo->setVisible(false);
	m_pLoadingLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/3)), CEGUI::UDim(0, ((fHeight/2)-fHeight/3))));
	m_pLoadingLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));

	//RakNet Loading Logo
	height = 44;
	width = 134;
	m_pRaknetLogo = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
	m_pRaknetLogo->setProperty("FrameEnabled", "false");
	m_pRaknetLogo->setProperty("BackgroundEnabled", "false");
	m_pRaknetLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pRaknetLogo->setProperty("Image", "set:RakNet image:full_image");
	m_pRaknetLogo->setVisible(false);
	m_pRaknetLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth-width), CEGUI::UDim(0, fHeight-height)));
	m_pRaknetLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));

	//Loading label
	m_pLoadingText = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pLoadingText->setText("IV:MP is loading, please wait ...");
	m_pLoadingText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pLoadingText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/8)), CEGUI::UDim(0, (fHeight/2))));
	m_pLoadingText->setProperty("FrameEnabled", "false");
	m_pLoadingText->setProperty("BackgroundEnabled", "false");
	m_pLoadingText->setFont(g_pGUI->GetFont("pricedown",24U));
	m_pLoadingText->setVisible(false);

	//IV:MP Logo
	m_pLogo = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
	m_pLogo->setProperty("FrameEnabled", "false");
	m_pLogo->setProperty("BackgroundEnabled", "false");
	m_pLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pLogo->setProperty("Image", "set:MenuLogo image:full_image");
	m_pLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4)), CEGUI::UDim(0, ((fHeight/2)-fHeight/4))));
	m_pLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));
	m_pBackground->addChildWindow( m_pLogo );

	// 0.88

	// TODO: Ability to pass user data to an event subscriber

	fY += 0.40f;
	fX += 0.30f;

	//Set network stuff
	m_pHost = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pHost->setText("-");
	m_pHost->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pHost->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY-10.0f))));
	m_pHost->setProperty("FrameEnabled", "false");
	m_pHost->setProperty("BackgroundEnabled", "false");
	m_pHost->setFont(g_pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pHost );

	m_pPlayers = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pPlayers->setText("-");
	m_pPlayers->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pPlayers->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY+20.0f))));
	m_pPlayers->setProperty("FrameEnabled", "false");
	m_pPlayers->setProperty("BackgroundEnabled", "false");
	m_pPlayers->setFont(g_pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pPlayers );

	m_pMyName = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pMyName->setText("-");
	m_pMyName->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pMyName->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY+50.0f)))); //(fWidth/2)+fWidth/8.5f)
	m_pMyName->setProperty("FrameEnabled", "false");
	m_pMyName->setProperty("BackgroundEnabled", "false");
	m_pMyName->setFont(g_pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pMyName );

	m_pHostDesc = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pHostDesc->setText("CONNECTED TO:");
	m_pHostDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pHostDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, (fY-10.0f))));
	m_pHostDesc->setProperty("FrameEnabled", "false");
	m_pHostDesc->setProperty("BackgroundEnabled", "false");
	m_pHostDesc->setFont(g_pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pHostDesc );

	m_pPlayersDesc = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pPlayersDesc->setText("PLAYERS ONLINE:");
	m_pPlayersDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pPlayersDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, (fY+20.0f))));
	m_pPlayersDesc->setProperty("FrameEnabled", "false");
	m_pPlayersDesc->setProperty("BackgroundEnabled", "false");
	m_pPlayersDesc->setFont(g_pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pPlayersDesc );

	m_pMyNameDesc = g_pGUI->CreateGUIStaticText(g_pGUI->GetDefaultWindow());
	m_pMyNameDesc->setText("MY NAME:");
	m_pMyNameDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pMyNameDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, fY+50.0f)));
	m_pMyNameDesc->setProperty("FrameEnabled", "false");
	m_pMyNameDesc->setProperty("BackgroundEnabled", "false");
	m_pMyNameDesc->setFont(g_pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pMyNameDesc );

	fX = 0.10f;
	m_pDisconnectButton = CreateButton("Disconnect",  CEGUI::UVector2(CEGUI::UDim(/*0.074f*/0.20f, 0), CEGUI::UDim(0.030f, 0)),
		CEGUI::UVector2(CEGUI::UDim(fX-0.0175f, 0), CEGUI::UDim(fY-0.25f, 0)));
	m_pBackground->addChildWindow( m_pDisconnectButton );
	m_pDisconnectButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnDisconnectButtonMouseEnter, this));
	m_pDisconnectButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnDisconnectButtonMouseLeave, this));
	m_pDisconnectButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnDisconnectButtonMouseClick, this));
	SetDisconnectButtonVisible(false);

	m_pServerBrowserButton = CreateButton("Server Browser", CEGUI::UVector2(CEGUI::UDim(/*0.074f*/0.20f, 0), CEGUI::UDim(0.030f, 0)),
		CEGUI::UVector2(CEGUI::UDim(fX-0.075f, 0), CEGUI::UDim(fY, 0)));
	m_pServerBrowserButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserButtonMouseEnter, this));
	m_pServerBrowserButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserButtonMouseLeave, this));
	m_pServerBrowserButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserButtonMouseClick, this));
	m_pBackground->addChildWindow( m_pServerBrowserButton );

	m_pQuickConnectButton = CreateButton("Quick Connect", CEGUI::UVector2(CEGUI::UDim(/*0.07f*/0.20f, 0), CEGUI::UDim(0.030f, 0)),
		CEGUI::UVector2(CEGUI::UDim(fX+0.175f, 0), CEGUI::UDim(fY, 0)));
	m_pQuickConnectButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectButtonMouseEnter, this));
	m_pQuickConnectButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectButtonMouseLeave, this));
	m_pQuickConnectButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectButtonMouseClick, this));
	m_pBackground->addChildWindow( m_pQuickConnectButton );

	fX = 0.10f;

	m_pSettingsButton = CreateButton("Settings", CEGUI::UVector2(CEGUI::UDim(/*0.042f*/0.162f, 0), CEGUI::UDim(0.030f, 0)),
		CEGUI::UVector2(CEGUI::UDim(fX+0.425f, 0), CEGUI::UDim(fY, 0)));
	m_pSettingsButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnSettingsButtonMouseEnter, this));
	m_pSettingsButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnSettingsButtonMouseLeave, this));
	m_pSettingsButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnSettingsButtonMouseClick, this));
	m_pBackground->addChildWindow( m_pSettingsButton );

	fX = 0.10f;

	m_pAboutButton = CreateButton("About", CEGUI::UVector2(CEGUI::UDim(/*0.02f*/0.08f, 0), CEGUI::UDim(0.030f, 0)), // 0.02f
		CEGUI::UVector2(CEGUI::UDim(fX+0.65f, 0), CEGUI::UDim(fY, 0)));
	m_pAboutButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnAboutButtonMouseEnter, this));
	m_pAboutButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnAboutButtonMouseLeave, this));
	m_pAboutButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnAboutButtonMouseClick, this));
	m_pBackground->addChildWindow( m_pAboutButton );

	fX = 0.10f;

	m_pQuitButton = CreateButton("Quit", CEGUI::UVector2(CEGUI::UDim(/*0.02f*/0.08f, 0), CEGUI::UDim(0.030f, 0)), // 0.02f
		CEGUI::UVector2(CEGUI::UDim(fX+0.825f, 0), CEGUI::UDim(fY, 0)));
	m_pQuitButton->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&CMainMenu::OnQuitButtonMouseEnter, this));
	m_pQuitButton->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&CMainMenu::OnQuitButtonMouseLeave, this));
	m_pQuitButton->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnQuitButtonMouseClick, this));
	m_pBackground->addChildWindow( m_pQuitButton );

	// Server Browser Window
	m_serverBrowser.pWindow = g_pGUI->CreateGUIFrameWindow();
	m_pBackground->addChildWindow(m_serverBrowser.pWindow);
	m_serverBrowser.pWindow->setText("Server Browser");
	m_serverBrowser.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 780), CEGUI::UDim(0, 585)));
	m_serverBrowser.pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowCloseClick, this));
	m_serverBrowser.pWindow->setVisible(false);

	m_serverBrowser.pServerMultiColumnList = g_pGUI->CreateGUIMultiColumnList(m_serverBrowser.pWindow);
	m_serverBrowser.pServerMultiColumnList->setText("Server Browser");
	m_serverBrowser.pServerMultiColumnList->setSize(CEGUI::UVector2(CEGUI::UDim(0.9f, 0), CEGUI::UDim(0.8f, 0)));
	m_serverBrowser.pServerMultiColumnList->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.08f, 0)));
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Name width:{0.26,0} id:0");
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Host width:{0.2,0} id:1");
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Players width:{0.12,0} id:2");
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Max Players width:{0.16,0} id:3");
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Ping width:{0.1,0} id:4");
	// jenksta: width of below header is set to 0.159 due to the fact if all column widths add up to 1.0
	// then an automaic scroll bar is added
	m_serverBrowser.pServerMultiColumnList->setProperty("ColumnHeader", "text:Passworded width:{0.159,0} id:5");
	m_serverBrowser.pServerMultiColumnList->setFont(g_pGUI->GetFont("tahoma-bold", 10));
	//m_pServerBrowserWindowServerMultiColumnList->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	m_serverBrowser.pServerMultiColumnList->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowRowClick, this));

	m_serverBrowser.pRefreshButton = g_pGUI->CreateGUIButton(m_serverBrowser.pWindow);
	m_serverBrowser.pRefreshButton->setText("Internet");
	m_serverBrowser.pRefreshButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_serverBrowser.pRefreshButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.12f, 0), CEGUI::UDim(0.9f, 0)));
	m_serverBrowser.pRefreshButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowRefreshButtonClick, this));

	m_serverBrowser.pSponsoredButton = g_pGUI->CreateGUIButton(m_serverBrowser.pWindow);
	m_serverBrowser.pSponsoredButton->setText("Sponsored");
	m_serverBrowser.pSponsoredButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_serverBrowser.pSponsoredButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.32f, 0), CEGUI::UDim(0.9f, 0)));
	m_serverBrowser.pSponsoredButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowSponsoredButtonClick, this));

	m_serverBrowser.pFeaturedButton = g_pGUI->CreateGUIButton(m_serverBrowser.pWindow);
	m_serverBrowser.pFeaturedButton->setText("Featured");
	m_serverBrowser.pFeaturedButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_serverBrowser.pFeaturedButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.52f, 0), CEGUI::UDim(0.9f, 0)));
	m_serverBrowser.pFeaturedButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowFeaturedButtonClick, this));

	m_serverBrowser.pConnectButton = g_pGUI->CreateGUIButton(m_serverBrowser.pWindow);
	m_serverBrowser.pConnectButton->setText("Connect");
	m_serverBrowser.pConnectButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	m_serverBrowser.pConnectButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.9f, 0)));
	m_serverBrowser.pConnectButton->setEnabled(false);
	m_serverBrowser.pConnectButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnServerBrowserWindowConnectButtonClick, this));

	// Quick Connect Window
	m_pQuickConnectWindow = g_pGUI->CreateGUIFrameWindow();
	m_pBackground->addChildWindow( m_pQuickConnectWindow );
	m_pQuickConnectWindow->setText("Quick Connect");
	m_pQuickConnectWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 520), CEGUI::UDim(0, 390)));
	m_pQuickConnectWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectWindowCloseClick, this));
	m_pQuickConnectWindow->setVisible(false);

	m_pQuickConnectWindowIPStaticText = g_pGUI->CreateGUIStaticText(m_pQuickConnectWindow);
	m_pQuickConnectWindowIPStaticText->setText("IP Address");
	m_pQuickConnectWindowIPStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pQuickConnectWindowIPStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	m_pQuickConnectWindowIPStaticText->setProperty("FrameEnabled", "false");
	m_pQuickConnectWindowIPStaticText->setProperty("BackgroundEnabled", "false");
	m_pQuickConnectWindowIPStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pQuickConnectWindowIPEditBox = g_pGUI->CreateGUIEditBox(m_pQuickConnectWindow);
	char szHost[256];
	sprintf_s(szHost, 256, "%s:%d", g_strHost.C_String(), g_usPort);
	m_pQuickConnectWindowIPEditBox->setText(szHost);
	m_pQuickConnectWindowIPEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pQuickConnectWindowIPEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));
	m_pQuickConnectWindowIPEditBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectIPEditBoxKeyUp, this));

	m_pQuickConnectWindowPasswordStaticText = g_pGUI->CreateGUIStaticText(m_pQuickConnectWindow);
	m_pQuickConnectWindowPasswordStaticText->setText("Password (Blank for none)");
	m_pQuickConnectWindowPasswordStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pQuickConnectWindowPasswordStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));
	m_pQuickConnectWindowPasswordStaticText->setProperty("FrameEnabled", "false");
	m_pQuickConnectWindowPasswordStaticText->setProperty("BackgroundEnabled", "false");
	m_pQuickConnectWindowPasswordStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pQuickConnectWindowPasswordEditBox = g_pGUI->CreateGUIEditBox(m_pQuickConnectWindow);
	m_pQuickConnectWindowPasswordEditBox->setText(g_strPassword.C_String());
	m_pQuickConnectWindowPasswordEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pQuickConnectWindowPasswordEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.6f, 0)));
	m_pQuickConnectWindowPasswordEditBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectIPEditBoxKeyUp, this));

	m_pQuickConnectWindowConnectButton = g_pGUI->CreateGUIButton(m_pQuickConnectWindow);
	m_pQuickConnectWindowConnectButton->setText("Connect");
	m_pQuickConnectWindowConnectButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_pQuickConnectWindowConnectButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.8f, 0)));
	m_pQuickConnectWindowConnectButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnQuickConnectWindowConnectButtonClick, this));

	// Settings Window
	m_pSettingsWindow = g_pGUI->CreateGUIFrameWindow();
	m_pSettingsWindow->setText("Settings");
	m_pBackground->addChildWindow( m_pSettingsWindow );
	m_pSettingsWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 520), CEGUI::UDim(0, 390)));
	m_pSettingsWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CMainMenu::OnSettingsWindowCloseClick, this));
	m_pSettingsWindow->setVisible(false);

	m_pSettingsTabControl = g_pGUI->CreateGUITabControl();
	m_pSettingsWindow->addChildWindow( m_pSettingsTabControl );
	m_pSettingsTabControl->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.8f, 0)));

	m_pSettingsWindowGeneral = g_pGUI->CreateGUITabContentPane();
	m_pSettingsWindowGeneral->setText("General");
	m_pSettingsTabControl->addChildWindow( m_pSettingsWindowGeneral );
	m_pSettingsWindowGeneral->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	m_pSettingsWindowChat = g_pGUI->CreateGUITabContentPane();
	m_pSettingsWindowChat->setText("Chat");
	m_pSettingsTabControl->addChildWindow( m_pSettingsWindowChat );
	m_pSettingsWindowChat->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	m_pSettingsWindowNickStaticText = g_pGUI->CreateGUIStaticText(m_pSettingsWindowGeneral);
	m_pSettingsWindowNickStaticText->setText("Name");
	m_pSettingsWindowNickStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowNickStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	m_pSettingsWindowNickStaticText->setProperty("FrameEnabled", "false");
	m_pSettingsWindowNickStaticText->setProperty("BackgroundEnabled", "false");
	m_pSettingsWindowNickStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pSettingsWindowNickEditBox = g_pGUI->CreateGUIEditBox(m_pSettingsWindowGeneral);
	m_pSettingsWindowNickEditBox->setText(g_strNick.C_String());
	m_pSettingsWindowNickEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowNickEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));

	m_pSettingsWindowWindowedStaticText = g_pGUI->CreateGUIStaticText(m_pSettingsWindowGeneral);
	m_pSettingsWindowWindowedStaticText->setText("Windowed");
	m_pSettingsWindowWindowedStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowWindowedStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));
	m_pSettingsWindowWindowedStaticText->setProperty("FrameEnabled", "false");
	m_pSettingsWindowWindowedStaticText->setProperty("BackgroundEnabled", "false");
	m_pSettingsWindowWindowedStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pSettingsWindowWindowedCheckBox = g_pGUI->CreateGUICheckBox(m_pSettingsWindowGeneral);
	CEGUI::Checkbox * pCheckBox = (CEGUI::Checkbox *)m_pSettingsWindowWindowedCheckBox;
	pCheckBox->setSelected(g_bWindowedMode);
	m_pSettingsWindowWindowedCheckBox->setText("");
	m_pSettingsWindowWindowedCheckBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowWindowedCheckBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.5f, 0)));

	m_pSettingsWindowFPSStaticText = g_pGUI->CreateGUIStaticText(m_pSettingsWindowGeneral);
	m_pSettingsWindowFPSStaticText->setText("Show FPS");
	m_pSettingsWindowFPSStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowFPSStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.6f, 0)));
	m_pSettingsWindowFPSStaticText->setProperty("FrameEnabled", "false");
	m_pSettingsWindowFPSStaticText->setProperty("BackgroundEnabled", "false");
	m_pSettingsWindowFPSStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pSettingsWindowFPSCheckBox = g_pGUI->CreateGUICheckBox(m_pSettingsWindowGeneral);
	CEGUI::Checkbox * pCheckBox2 = (CEGUI::Checkbox *)m_pSettingsWindowFPSCheckBox;
	pCheckBox2->setSelected(g_bFPSToggle);
	m_pSettingsWindowFPSCheckBox->setText("");
	m_pSettingsWindowFPSCheckBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowFPSCheckBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.6f, 0)));


	m_pSettingsWindowChatFontSizeStaticText = g_pGUI->CreateGUIStaticText(m_pSettingsWindowChat);
	m_pSettingsWindowChatFontSizeStaticText->setText("Chatfont/Fontsize");
	m_pSettingsWindowChatFontSizeStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatFontSizeStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	m_pSettingsWindowChatFontSizeStaticText->setProperty("FrameEnabled", "false");
	m_pSettingsWindowChatFontSizeStaticText->setProperty("BackgroundEnabled", "false");
	m_pSettingsWindowChatFontSizeStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pSettingsWindowChatFont = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatFont->setText(CVAR_GET_STRING("chatfont").Get());
	m_pSettingsWindowChatFont->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatFont->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));

	m_pSettingsWindowChatFontSize = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatFontSize->setText(CVAR_GET_EX("chatsize").Get());
	m_pSettingsWindowChatFontSize->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatFontSize->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.3f, 0)));

	m_pSettingsWindowChatBackgroundStaticText = g_pGUI->CreateGUIStaticText(m_pSettingsWindowChat);
	m_pSettingsWindowChatBackgroundStaticText->setText("Background (A,R,G,B)");
	m_pSettingsWindowChatBackgroundStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatBackgroundStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.4f, 0)));
	m_pSettingsWindowChatBackgroundStaticText->setProperty("FrameEnabled", "false");
	m_pSettingsWindowChatBackgroundStaticText->setProperty("BackgroundEnabled", "false");
	m_pSettingsWindowChatBackgroundStaticText->setFont(g_pGUI->GetFont("tahoma-bold"));

	m_pSettingsWindowChatBGColorA = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatBGColorA->setText(CVAR_GET_EX("chatbga").Get());
	m_pSettingsWindowChatBGColorA->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatBGColorA->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));

	m_pSettingsWindowChatBGColorR = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatBGColorR->setText(CVAR_GET_EX("chatbgr").Get());
	m_pSettingsWindowChatBGColorR->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatBGColorR->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.5f, 0)));

	m_pSettingsWindowChatBGColorG = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatBGColorG->setText(CVAR_GET_EX("chatbgg").Get());
	m_pSettingsWindowChatBGColorG->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatBGColorG->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.5f, 0)));

	m_pSettingsWindowChatBGColorB = g_pGUI->CreateGUIEditBox(m_pSettingsWindowChat);
	m_pSettingsWindowChatBGColorB->setText(CVAR_GET_EX("chatbgb").Get());
	m_pSettingsWindowChatBGColorB->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowChatBGColorB->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.5f, 0)));

	m_pSettingsWindowSaveButton = g_pGUI->CreateGUIButton(m_pSettingsWindow);
	m_pSettingsWindowSaveButton->setText("Save");
	m_pSettingsWindowSaveButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_pSettingsWindowSaveButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.8f, 0)));
	m_pSettingsWindowSaveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMainMenu::OnSettingsWindowSaveButtonClick, this));

	OnResetDevice();
}

CMainMenu::~CMainMenu()
{
	// Settings Window
	if(m_pSettingsWindow)
		g_pGUI->RemoveGUIWindow(m_pSettingsWindow);

	// Quick Connect Window
	if(m_pQuickConnectWindow)
		g_pGUI->RemoveGUIWindow(m_pQuickConnectWindow);

	// Server Browser Window
	if(m_serverBrowser.pWindow)
		g_pGUI->RemoveGUIWindow(m_serverBrowser.pWindow);

	// Main Menu
	if(m_pBackground)
		g_pGUI->RemoveGUIWindow(m_pBackground);

	// Delete the server query instance
	SAFE_DELETE(m_pServerQuery);

	// Delete the master list query instance
	SAFE_DELETE(m_pMasterListQuery);
}

void CMainMenu::OnResetDevice()
{
	// jenksta: Now this is why we should use relative positions...
	float fWidth = (float)g_pGUI->GetDisplayWidth();
	float fHeight = (float)g_pGUI->GetDisplayHeight();

	m_pBackground->setSize(CEGUI::UVector2(CEGUI::UDim(0, fWidth), CEGUI::UDim(0, fHeight)));

	float height = 90;
	float width = 390;
	m_pLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth-width), CEGUI::UDim(0, fHeight-height)));
	m_pLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));

	m_serverBrowser.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-300), CEGUI::UDim(0, fHeight/2-225)));
	m_pQuickConnectWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-260), CEGUI::UDim(0, fHeight/2-195)));
	m_pSettingsWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-260), CEGUI::UDim(0, fHeight/2-190)));
	//SetServerBrowserWindowVisible(false);
	//SetQuickConnectWindowVisible(false);
	//SetSettingsWindowVisible(false);
	SetVisible(true);
}

CGUIStaticText * CMainMenu::CreateButton(char * szText, CEGUI::UVector2 vecSize, CEGUI::UVector2 vecPosition)
{
	CGUIStaticText * pButton = g_pGUI->CreateGUIStaticText();
	pButton->setText(szText);
	pButton->setSize(vecSize);
	pButton->setPosition(vecPosition);
	pButton->setProperty("FrameEnabled", "false");
	pButton->setProperty("BackgroundEnabled", "false");
	pButton->setFont(g_pGUI->GetFont("pricedown", 20));
	pButton->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	return pButton;
}

void CMainMenu::SetVisible(bool bVisible)
{
	if(!bVisible)
	{
		m_bServerBrowserWindowOpen = m_bServerBrowserWindowVisible;
		m_bQuickConnectWindowOpen = m_bQuickConnectWindowVisible;
		m_bSettingsWindowOpen = m_bSettingsWindowVisible;
		SetServerBrowserWindowVisible(false);
		SetQuickConnectWindowVisible(false);
		SetSettingsWindowVisible(false);
	}
	else
	{
		SetServerBrowserWindowVisible(m_bServerBrowserWindowOpen);
		SetQuickConnectWindowVisible(m_bQuickConnectWindowOpen);
		SetSettingsWindowVisible(m_bSettingsWindowOpen);
	}
	m_pHost->setVisible(true);
	m_bVisible = bVisible;
	g_pGUI->SetCursorVisible(bVisible);
	m_pBackground->setVisible(bVisible);
	m_pBackground->setAlpha(0.95f);
}

void CMainMenu::SetServerBrowserWindowVisible(bool bVisible)
{
	if(bVisible != m_bServerBrowserWindowVisible)
	{
		m_serverBrowser.pWindow->setVisible(bVisible);
		m_serverBrowser.pWindow->activate();
		m_bServerBrowserWindowVisible = bVisible;
	}
}

void CMainMenu::SetQuickConnectWindowVisible(bool bVisible)
{
	if(bVisible != m_bQuickConnectWindowVisible)
	{
		m_pQuickConnectWindow->setVisible(bVisible);
		if(bVisible)
			m_pQuickConnectWindow->activate();

		if(!bVisible)
			m_pQuickConnectWindow->deactivate();

		m_bQuickConnectWindowVisible = bVisible;
	}
}

void CMainMenu::SetSettingsWindowVisible(bool bVisible)
{
	if(bVisible != m_bSettingsWindowVisible)
	{
		m_pSettingsWindow->setVisible(bVisible);
		m_pSettingsWindow->activate();
		m_bSettingsWindowVisible = bVisible;
	}
}

void CMainMenu::Process()
{
	// Process the master list query
	m_pMasterListQuery->Process();

	// Process the server query
	m_pServerQuery->Process();
}

void CMainMenu::ShowLoadingScreen()
{
	m_pLoadingLogo->setVisible(true);
	m_pRaknetLogo->setVisible(true);
	m_pLoadingText->setVisible(true);
	m_pLoadingBackground->setVisible(true);
	m_bLoadingScreenActive = true;
}
void CMainMenu::HideLoadingScreen()
{
	m_pLoadingLogo->setVisible(false);
	m_pRaknetLogo->setVisible(false);
	m_pLoadingText->setVisible(false);
	m_pLoadingBackground->setVisible(false);
	m_bLoadingScreenActive = false;
}

void CMainMenu::SetNetworkStats(String strHost,int players, int maxplayers, String strName)
{
	String szPlayers;
	szPlayers.AppendF("%d/%d",players,maxplayers);

	m_pHost->setText(strHost.C_String());
	m_pPlayers->setText(szPlayers.C_String());
	m_pMyName->setText(strName.C_String());
}

void CMainMenu::ResetNetworkStats()
{
	m_pHost->setText("-");
	m_pPlayers->setText("-");
	m_pMyName->setText("-");
}

void CMainMenu::ShowMessageBox(const char * szMessage, const char * szHeader, bool bGameMenu, bool bResetGame, bool bAllowReconnect)
{
	if(bGameMenu)
		CGame::SetState(GAME_STATE_MAIN_MENU);

	if(bResetGame)
		InternalResetGame(bAllowReconnect);

	g_pGUI->ShowMessageBox(szMessage,szHeader);
}