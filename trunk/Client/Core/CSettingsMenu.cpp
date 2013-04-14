//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSettingsMenu.cpp
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#include "CSettingsMenu.h"
#include "CClient.h"
#include <CSettings.h>

extern CClient * g_pClient;
CSettingsMenu * CSettingsMenu::m_pSingleton = NULL;

CSettingsMenu::CSettingsMenu(void)
{
	assert(!m_pSingleton);

	// Set our singleton
	m_pSingleton = this;

	// Set up the GUI
	CGUI * pGUI = g_pClient->GetGUI();

	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();

	m_GUIElements.pWindow = pGUI->CreateGUIFrameWindow();
	m_GUIElements.pWindow->setText("Settings");
	m_GUIElements.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 520), CEGUI::UDim(0, 390)));
	m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-260), CEGUI::UDim(0, fHeight/2-190)));
	m_GUIElements.pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CSettingsMenu::OnCloseClick, this));
	m_GUIElements.pWindow->setVisible(false);

	m_GUIElements.pTabControl = pGUI->CreateGUITabControl();
	m_GUIElements.pWindow->addChildWindow( m_GUIElements.pTabControl );
	m_GUIElements.pTabControl->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.8f, 0)));

	m_GUIElements.pGeneralPane = pGUI->CreateGUITabContentPane();
	m_GUIElements.pGeneralPane->setText("General");
	m_GUIElements.pTabControl->addChildWindow( m_GUIElements.pGeneralPane );
	m_GUIElements.pGeneralPane->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	m_GUIElements.pChatPane = pGUI->CreateGUITabContentPane();
	m_GUIElements.pChatPane->setText("Chat");
	m_GUIElements.pTabControl->addChildWindow( m_GUIElements.pChatPane );
	m_GUIElements.pChatPane->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	m_GUIElements.pNickStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pGeneralPane);
	m_GUIElements.pNickStaticText->setText("Name");
	m_GUIElements.pNickStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pNickStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	m_GUIElements.pNickStaticText->setProperty("FrameEnabled", "false");
	m_GUIElements.pNickStaticText->setProperty("BackgroundEnabled", "false");
	m_GUIElements.pNickStaticText->setFont(pGUI->GetFont("tahoma-bold"));

	m_GUIElements.pNickEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pGeneralPane);
	m_GUIElements.pNickEditBox->setText(CGUI::AnsiToCeguiFriendlyString(g_pClient->GetNick()));
	m_GUIElements.pNickEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pNickEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));

	m_GUIElements.pWindowedStaticText= pGUI->CreateGUIStaticText(m_GUIElements.pGeneralPane);
	m_GUIElements.pWindowedStaticText->setText("Windowed");
	m_GUIElements.pWindowedStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pWindowedStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));
	m_GUIElements.pWindowedStaticText->setProperty("FrameEnabled", "false");
	m_GUIElements.pWindowedStaticText->setProperty("BackgroundEnabled", "false");
	m_GUIElements.pWindowedStaticText->setFont(pGUI->GetFont("tahoma-bold"));

	m_GUIElements.pWindowedCheckBox = pGUI->CreateGUICheckBox(m_GUIElements.pGeneralPane);
	CEGUI::Checkbox * pCheckBox = (CEGUI::Checkbox *)m_GUIElements.pWindowedCheckBox;
	pCheckBox->setSelected(g_pClient->IsWindowedMode());
	m_GUIElements.pWindowedCheckBox->setText("");
	m_GUIElements.pWindowedCheckBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pWindowedCheckBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.5f, 0)));

	m_GUIElements.pFPSStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pGeneralPane);
	m_GUIElements.pFPSStaticText->setText("Show FPS");
	m_GUIElements.pFPSStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pFPSStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.6f, 0)));
	m_GUIElements.pFPSStaticText->setProperty("FrameEnabled", "false");
	m_GUIElements.pFPSStaticText->setProperty("BackgroundEnabled", "false");
	m_GUIElements.pFPSStaticText->setFont(pGUI->GetFont("tahoma-bold"));

	m_GUIElements.pFPSCheckBox = pGUI->CreateGUICheckBox(m_GUIElements.pGeneralPane);
	CEGUI::Checkbox * pCheckBox2 = (CEGUI::Checkbox *)m_GUIElements.pFPSCheckBox;
	pCheckBox2->setSelected(g_pClient->GetFPSToggle());
	m_GUIElements.pFPSCheckBox->setText("");
	m_GUIElements.pFPSCheckBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pFPSCheckBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.6f, 0)));


	m_GUIElements.pChatFontSizeStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pChatPane);
	m_GUIElements.pChatFontSizeStaticText->setText("Chatfont/Fontsize");
	m_GUIElements.pChatFontSizeStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatFontSizeStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
	m_GUIElements.pChatFontSizeStaticText->setProperty("FrameEnabled", "false");
	m_GUIElements.pChatFontSizeStaticText->setProperty("BackgroundEnabled", "false");
	m_GUIElements.pChatFontSizeStaticText->setFont(pGUI->GetFont("tahoma-bold"));

	m_GUIElements.pChatFontEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatFontEditBox->setText(CVAR_GET_STRING("chatfont").Get());
	m_GUIElements.pChatFontEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatFontEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));

	m_GUIElements.pChatFontSizeEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatFontSizeEditBox->setText(CVAR_GET_EX("chatsize").Get());
	m_GUIElements.pChatFontSizeEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatFontSizeEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.3f, 0)));

	m_GUIElements.pChatBackgroundStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pChatPane);
	m_GUIElements.pChatBackgroundStaticText->setText("Background (A,R,G,B)");
	m_GUIElements.pChatBackgroundStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatBackgroundStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.4f, 0)));
	m_GUIElements.pChatBackgroundStaticText->setProperty("FrameEnabled", "false");
	m_GUIElements.pChatBackgroundStaticText->setProperty("BackgroundEnabled", "false");
	m_GUIElements.pChatBackgroundStaticText->setFont(pGUI->GetFont("tahoma-bold"));

	m_GUIElements.pChatBGColorAEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatBGColorAEditBox->setText(CVAR_GET_EX("chatbga").Get());
	m_GUIElements.pChatBGColorAEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatBGColorAEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));

	m_GUIElements.pChatBGColorREditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatBGColorREditBox->setText(CVAR_GET_EX("chatbgr").Get());
	m_GUIElements.pChatBGColorREditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatBGColorREditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.5f, 0)));

	m_GUIElements.pChatBGColorGEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatBGColorGEditBox->setText(CVAR_GET_EX("chatbgg").Get());
	m_GUIElements.pChatBGColorGEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatBGColorGEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.5f, 0)));

	m_GUIElements.pChatBGColorBEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pChatPane);
	m_GUIElements.pChatBGColorBEditBox->setText(CVAR_GET_EX("chatbgb").Get());
	m_GUIElements.pChatBGColorBEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pChatBGColorBEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.5f, 0)));

	m_GUIElements.pSaveButton = pGUI->CreateGUIButton(m_GUIElements.pWindow);
	m_GUIElements.pSaveButton->setText("Save");
	m_GUIElements.pSaveButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.1f, 0)));
	m_GUIElements.pSaveButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.8f, 0)));
	m_GUIElements.pSaveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CSettingsMenu::OnSaveButtonClick, this));

	m_bVisible = false;
}

CSettingsMenu::~CSettingsMenu(void)
{
	// Delete the GUI elements
	CGUI * pGUI = g_pClient->GetGUI();
	pGUI->RemoveGUIWindow(m_GUIElements.pWindow);
}

// (John): Do we really need to re-position the GUI every time the D3D device resets?
void CSettingsMenu::OnResetDevice()
{
	CGUI * pGUI = g_pClient->GetGUI();
	
	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();
	
	m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-260), CEGUI::UDim(0, fHeight/2-190)));
}

void CSettingsMenu::SetVisible(bool bVisible)
{
	m_GUIElements.pWindow->setVisible(bVisible);
	m_GUIElements.pWindow->setProperty("AlwaysOnTop", "true");
    m_GUIElements.pWindow->activate();

	m_bVisible = bVisible;
}

void CSettingsMenu::SetNick(const char * strNick)
{
	m_GUIElements.pNickEditBox->setText(strNick);
}

bool CSettingsMenu::OnCloseClick(const CEGUI::EventArgs &eventArgs)
{
	SetVisible(false);
	return true;
}

bool CSettingsMenu::OnSaveButtonClick(const CEGUI::EventArgs &eventArgs)
{
	bool bWindowed = ((CEGUI::Checkbox *)m_GUIElements.pWindowedCheckBox)->isSelected();
	bool bShowFPS = ((CEGUI::Checkbox *)m_GUIElements.pFPSCheckBox)->isSelected();
	String strNick(m_GUIElements.pNickEditBox->getText().c_str());
	String strFont(m_GUIElements.pChatFontEditBox->getText().c_str());
	String strFontSize(m_GUIElements.pChatFontSizeEditBox->getText().c_str());
	String strBackgroundAlpha(m_GUIElements.pChatBGColorAEditBox->getText().c_str());
	String strBackgroundRed(m_GUIElements.pChatBGColorREditBox->getText().c_str());
	String strBackgroundGreen(m_GUIElements.pChatBGColorGEditBox->getText().c_str());
	String strBackgroundBlue(m_GUIElements.pChatBGColorBEditBox->getText().c_str());

	CVAR_SET_BOOL("windowed", bWindowed);
	CVAR_SET_BOOL("fps", g_pClient->GetFPSToggle());
	g_pClient->SetFPSToggle(bShowFPS);
	CVAR_SET_INTEGER("chatfont", strFont.ToInteger());
	CVAR_SET_INTEGER("chatsize", strFontSize.ToInteger());
	CVAR_SET_INTEGER("chatbga", strBackgroundAlpha.ToInteger());
	CVAR_SET_INTEGER("chatbgr", strBackgroundRed.ToInteger());
	CVAR_SET_INTEGER("chatbgg", strBackgroundGreen.ToInteger());
	CVAR_SET_INTEGER("chatbgb", strBackgroundBlue.ToInteger());

	// Init the chat window font and background
	g_pClient->GetChatWindow()->InitFontAndBackground();

	if(CVAR_GET_STRING("nick").Compare(strNick) != 0) {
		CVAR_SET_STRING("nick", strNick);
		g_pClient->SetNick(strNick);

		CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
		if(pNetworkManager && pNetworkManager->IsConnected()) {
			CBitStream bsSend;
			bsSend.Write(strNick);
			pNetworkManager->RPC(RPC_NameChange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
		}
	}

	if(bWindowed != g_pClient->IsWindowedMode())
		g_pClient->GetGUI()->ShowMessageBox("You must restart IV:MP for the windowed mode option to take effect", "Information.");
	
	SetVisible(false);

	return true;
}