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

#include <map>
#include <CSettings.h>
#include <CLogFile.h>
#include <Network/CHttpClient.h>
#include "CMainMenu.h"
#include "CClient.h"
#include "CGame.h"

extern CClient * g_pClient;

#define PAUSE_MENU_BACKGROUND 0x00000080

CMainMenu                         * CMainMenu::m_pSingleton = NULL;
extern std::vector<CEGUI::Window *> g_pGUIElements;

CMainMenu::CMainMenu()
{
	// Set our singleton pointer
	m_pSingleton = this;

	// Create the server browser
	m_pServerBrowser = new CServerBrowser();

	// Create the quick connect window
	m_pQuickConnect = new CQuickConnect();

	// Create the settings menu
	m_pSettingsMenu = new CSettingsMenu();

	// Reset all visible and open flags
	m_bVisible = false;
	m_bDisconnectButtonVisible = false;

	// Reset cinematic camera
	m_iCameraState = 1;
	m_iCameraTime = 0;

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	// Load all images
	try
	{
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Background", "IVMPBackgroundNetwork.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("Logo", "IVMPLogoHiRes.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("MenuLogo", "IVMPMainMenuLogo.png");
		CEGUI::ImagesetManager::getSingleton().createFromImageFile("MenuLogoNetwork", "IVMPMainMenuLogoNetwork.png");
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

	// Create the GUI elements
	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();
	float fX = -2.0f;
	float fY = 0.5f;

	//IV:MP Menu Background
	m_pBackground = pGUI->CreateGUIStaticImage(pGUI->GetDefaultWindow());
	m_pBackground->setProperty("FrameEnabled", "false");
	m_pBackground->setProperty("BackgroundEnabled", "false");
	m_pBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	m_pBackground->setProperty("Image", "set:Background image:full_image");
	m_pBackground->setProperty("InheritsAlpha","false");
	m_pBackground->setAlpha(0.975f);
	m_pBackground->setVisible(false);

	//IV:MP Logo
	// (John) Covers up the quit button, maybe use the loading screen logo and place it in the middle?
	/*m_pLogo = pGUI->CreateGUIStaticImage(pGUI->GetDefaultWindow());
    m_pLogo->setProperty("FrameEnabled", "false");
    m_pLogo->setProperty("BackgroundEnabled", "false");
	m_pLogo->setProperty("Image", "set:MenuLogo image:full_image");
	m_pLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-134), CEGUI::UDim(0, fHeight-44)));
	// (John) - TODO: Fix the size of this logo
	m_pLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, 134), CEGUI::UDim(0, 44)));
	m_pBackground->addChildWindow( m_pLogo );*/

	// 0.88

	// TODO: Ability to pass user data to an event subscriber

	fY += 0.40f;
	fX += 0.30f;

	//Set network stuff
	m_pHost = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pHost->setText("-");
	m_pHost->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pHost->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY-10.0f))));
	m_pHost->setProperty("FrameEnabled", "false");
	m_pHost->setProperty("BackgroundEnabled", "false");
	m_pHost->setFont(pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pHost );

	m_pPlayers = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pPlayers->setText("-");
	m_pPlayers->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pPlayers->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY+20.0f))));
	m_pPlayers->setProperty("FrameEnabled", "false");
	m_pPlayers->setProperty("BackgroundEnabled", "false");
	m_pPlayers->setFont(pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pPlayers );

	m_pMyName = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pMyName->setText("-");
	m_pMyName->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pMyName->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)+fWidth/8.75f)),  CEGUI::UDim(0, (fY+50.0f)))); //(fWidth/2)+fWidth/8.5f)
	m_pMyName->setProperty("FrameEnabled", "false");
	m_pMyName->setProperty("BackgroundEnabled", "false");
	m_pMyName->setFont(pGUI->GetFont("electronichighwaysign",21U));
	m_pBackground->addChildWindow( m_pMyName );

	m_pHostDesc = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pHostDesc->setText("CONNECTED TO:");
	m_pHostDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pHostDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, (fY-10.0f))));
	m_pHostDesc->setProperty("FrameEnabled", "false");
	m_pHostDesc->setProperty("BackgroundEnabled", "false");
	m_pHostDesc->setFont(pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pHostDesc );

	m_pPlayersDesc = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pPlayersDesc->setText("PLAYERS ONLINE:");
	m_pPlayersDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pPlayersDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, (fY+20.0f))));
	m_pPlayersDesc->setProperty("FrameEnabled", "false");
	m_pPlayersDesc->setProperty("BackgroundEnabled", "false");
	m_pPlayersDesc->setFont(pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pPlayersDesc );

	m_pMyNameDesc = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
	m_pMyNameDesc->setText("MY NAME:");
	m_pMyNameDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_pMyNameDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.75f)),  CEGUI::UDim(0, fY+50.0f)));
	m_pMyNameDesc->setProperty("FrameEnabled", "false");
	m_pMyNameDesc->setProperty("BackgroundEnabled", "false");
	m_pMyNameDesc->setFont(pGUI->GetFont("electronichighwaysign",20U));
	m_pBackground->addChildWindow( m_pMyNameDesc );

	// Menu buttons
	fX = 0.10f;
	m_pDisconnectButton = CreateButton("X",  CEGUI::UVector2(CEGUI::UDim(/*0.074f*/0.20f, 0), CEGUI::UDim(0.1f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0, ((fWidth/2)-fWidth/4.25f)),  CEGUI::UDim(0, fY+20.0f)));
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

	OnResetDevice();
}

CMainMenu::~CMainMenu()
{
	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	// Settings Window
	SAFE_DELETE(m_pSettingsMenu);

	// Quick Connect Window
	SAFE_DELETE(m_pQuickConnect);

	// Server Browser
	SAFE_DELETE(m_pServerBrowser);

	// Main Menu
	if(m_pBackground)
		pGUI->RemoveGUIWindow(m_pBackground);
}

void CMainMenu::Process()
{
	// Process the server browser queries
	if(m_pServerBrowser)
		m_pServerBrowser->Process();

	// Get our graphics
	CGraphics * pGraphics = g_pClient->GetGraphics();

	// Draw the pause menu
	if(CGame::IsGameLoaded() && pGraphics)
	{
		if(m_pBackground->isVisible())
		{
			CGUI * pGUI = g_pClient->GetGUI();

			float boxHeight, fWidth, fHeight;
			fWidth = (float)pGUI->GetDisplayWidth();
			fHeight = (float)(pGUI->GetDisplayHeight());
			boxHeight = fHeight / 8;

			pGraphics->DrawRect(0.0, 0.0, fWidth, boxHeight, ( PAUSE_MENU_BACKGROUND >> 8 ) + ( ( PAUSE_MENU_BACKGROUND & 0xFF ) << 24 ));
			pGraphics->DrawRect(0.0, fHeight-boxHeight, fWidth, boxHeight, ( PAUSE_MENU_BACKGROUND >> 8 ) + ( ( PAUSE_MENU_BACKGROUND & 0xFF ) << 24 ));
		}
		
		// Process the cinematic main menu view, as long as we arent connected and the game is loaded.
		CCamera * pCamera = g_pClient->GetCamera();
		if(!g_pClient->GetNetworkManager()->IsConnected() && pCamera)
		{
			if(m_iCameraTime > 1000)
			{
				m_iCameraState++;
				if(m_iCameraState >= 2)
					m_iCameraState = 1;

				switch(m_iCameraState) 
				{
					case 1:
						pCamera->SetPosition(CVector3(HAPPINESS_CAMERA_POS));
						pCamera->SetLookAt(CVector3(HAPPINESS_CAMERA_LOOK_AT), false);
					case 2:
						pCamera->SetPosition(CVector3(TRIANGLE_CAMERA_POS));
						pCamera->SetLookAt(CVector3(TRIANGLE_CAMERA_LOOK_AT), false);
				}

				m_iCameraTime = 0;
			}
			else
			{
				m_iCameraTime++;

				CVector3 vecPosition;
				pCamera->GetPosition(vecPosition);

				vecPosition.fX += 0.03f;
				vecPosition.fY += 0.03f;

				pCamera->SetPosition(vecPosition);
			}
		}
	}
}

void CMainMenu::OnResetDevice()
{
	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();

	m_pBackground->setSize(CEGUI::UVector2(CEGUI::UDim(0, fWidth), CEGUI::UDim(0, fHeight)));

	//m_pLogo->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth-90), CEGUI::UDim(0, fHeight-390)));
	//m_pLogo->setSize(CEGUI::UVector2(CEGUI::UDim(0, 90), CEGUI::UDim(0, 390)));

	m_pServerBrowser->OnResetDevice();
	m_pQuickConnect->OnResetDevice();
	m_pSettingsMenu->OnResetDevice();

	SetVisible(true);
}

void CMainMenu::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;

	m_pHost->setVisible(true);
	g_pClient->GetGUI()->SetCursorVisible(bVisible);
	m_pBackground->setVisible(bVisible);

	m_pServerBrowser->SetVisible(false);
	m_pQuickConnect->SetVisible(false);
	m_pSettingsMenu->SetVisible(false);

	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	if(pNetworkManager)
		SetDisconnectButtonVisible(pNetworkManager->IsConnected());

	CChatWindow * pChatWindow = g_pClient->GetChatWindow();
	if(bVisible && pChatWindow && pChatWindow->IsEnabled())
		pChatWindow->SetEnabled(false);

	if(CGame::IsGameLoaded())
	{
		m_pBackground->removeProperty("Image");

		if(pNetworkManager && !pNetworkManager->IsConnected())
		{
			CCamera * pCamera = g_pClient->GetCamera();
			pCamera->SetPosition(CVector3(HAPPINESS_CAMERA_POS));
			pCamera->SetLookAt(CVector3(HAPPINESS_CAMERA_LOOK_AT), false);
		}
	}
	else
	{
		m_pBackground->setProperty("Image", "set:Background image:full_image");
		m_pBackground->setAlpha(0.95f);
	}
}

void CMainMenu::SetDisconnectButtonVisible(bool bDisconnectButtonVisible)
{
	m_pDisconnectButton->setVisible(bDisconnectButtonVisible);
	m_bDisconnectButtonVisible = bDisconnectButtonVisible;
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
		g_pClient->ResetGame(true, bAllowReconnect);

	g_pClient->GetGUI()->ShowMessageBox(szMessage,szHeader);
}


void CMainMenu::OnConnect(String strHost, unsigned short usPort, String strPassword, bool bIgnoreDefaultName)
{
	// Ensure we have a name set
	if(g_pClient->GetNick().IsEmpty())
	{
		g_pClient->GetGUI()->ShowMessageBox("You must have a name set!\nPress 'Ok' to open the 'Settings' menu.", "Error", GUI_MESSAGEBOXTYPE_OK, OnConnectMessageBoxResponse);
		return;
	}

	// Set the host and port
	g_pClient->SetHost(strHost);
	g_pClient->SetPort(usPort);

	// Set the password
	g_pClient->SetPassword(strPassword);

	// Ensure they have changed from the default name and we are not ignoring default name
	if(CVAR_GET_STRING("nick") == "player" && !bIgnoreDefaultName)
	{
		g_pClient->GetGUI()->ShowMessageBox("Your current name \"player\" is the default name.\nDo you wish to change your name?.", "Warning", GUI_MESSAGEBOXTYPE_YESNO, OnConnectMessageBoxResponse);
		return;
	}

	// Are we already connected?
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();

	if(pNetworkManager && pNetworkManager->IsConnected())
	{
		// Reset the game
		g_pClient->ResetGame();

		// Disable the menu
		CGame::SetState(GAME_STATE_INGAME);
		g_pClient->GetChatWindow()->SetEnabled(true);
	}
	else if(pNetworkManager && !pNetworkManager->IsConnected() && g_pClient->IsGameLoaded())
	{
		// Just connect to the server
		SAFE_DELETE(pNetworkManager);
		pNetworkManager = new CNetworkManager();

		pNetworkManager->Startup(g_pClient->GetHost(), g_pClient->GetPort(), g_pClient->GetPassword());
		pNetworkManager->Connect();

		// Disable the menu
		CGame::SetState(GAME_STATE_INGAME);
		g_pClient->GetChatWindow()->SetEnabled(true);
	}
	else
	{
		// Disable the menu and load the game
		CGame::SetState(GAME_STATE_LOADING);
	}
}

void CMainMenu::OnConnectMessageBoxResponse(eGUIMessageBoxResponse type)
{
	// Was the yes button clicked?
	if(type == GUI_MESSAGEBOX_YES)
	{
		// Show the settings window
		CMainMenu::GetSingleton()->m_pSettingsMenu->SetVisible(true);
	}
	else if(type == GUI_MESSAGEBOX_NO)
	{
		// Connect anyway
		CMainMenu::GetSingleton()->OnConnect(g_pClient->GetHost(), g_pClient->GetPort(), g_pClient->GetPassword(), true);
	}
}

bool CMainMenu::GetHostAndPort(String strHostAndPort, String& strHost, unsigned short& usPort)
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

CGUIStaticText * CMainMenu::CreateButton(char * szText, CEGUI::UVector2 vecSize, CEGUI::UVector2 vecPosition)
{
	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	CGUIStaticText * pButton = pGUI->CreateGUIStaticText();
	pButton->setText(CGUI::AnsiToCeguiFriendlyString(szText, strlen(szText)));
	pButton->setSize(vecSize);
	pButton->setPosition(vecPosition);
	pButton->setProperty("FrameEnabled", "false");
	pButton->setProperty("BackgroundEnabled", "false");
	pButton->setFont(pGUI->GetFont("pricedown", 20));
	pButton->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	return pButton;
}

/*
	Main menu button callbacks
*/
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
	m_pServerBrowser->SetVisible(!m_pServerBrowser->IsVisible());
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
	m_pQuickConnect->SetVisible(!m_pQuickConnect->IsVisible());
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

bool CMainMenu::OnDisconnectButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	// Disconnect from the server now, and reset the game (to avoid waiting to be disconnected via game reset)
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	if(pNetworkManager && pNetworkManager->IsConnected())
	{
		pNetworkManager->Disconnect();
		g_pClient->ResetGame(true);
		SetDisconnectButtonVisible(false);
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
	m_pSettingsMenu->SetVisible(!m_pSettingsMenu->IsVisible());
	return true;
}

bool CMainMenu::OnQuitButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pQuitButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnQuitButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pQuitButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnQuitButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	// Disconnect and terminate (no need to wait)
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	if(pNetworkManager && pNetworkManager->IsConnected())
		pNetworkManager->Disconnect();

	TerminateProcess(GetCurrentProcess(), 0);
	return true;
}

bool CMainMenu::OnAboutButtonMouseEnter(const CEGUI::EventArgs &eventArgs)
{
	m_pAboutButton->setAlpha(0.5f);
	return true;
}

bool CMainMenu::OnAboutButtonMouseLeave(const CEGUI::EventArgs &eventArgs)
{
	m_pAboutButton->setAlpha(1);
	return true;
}

bool CMainMenu::OnAboutButtonMouseClick(const CEGUI::EventArgs &eventArgs)
{
	g_pClient->GetCredits()->Start();
	return true;
}