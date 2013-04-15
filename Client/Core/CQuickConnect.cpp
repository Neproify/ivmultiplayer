//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CQuickConnect.cpp
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#include "CQuickConnect.h"
#include "CClient.h"
#include <CSettings.h>

extern CClient * g_pClient;
CQuickConnect * CQuickConnect::m_pSingleton = NULL;

CQuickConnect::CQuickConnect(void)
{
        assert(!m_pSingleton);

        // Set our singleton
        m_pSingleton = this;

        // Set up the GUI
        CGUI * pGUI = g_pClient->GetGUI();

        float fWidth = (float)pGUI->GetDisplayWidth();
        float fHeight = (float)pGUI->GetDisplayHeight();

        m_GUIElements.pWindow = pGUI->CreateGUIFrameWindow();
        m_GUIElements.pWindow->setText("Quick Connect");
        m_GUIElements.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 300), CEGUI::UDim(0, 240)));
        m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-150.0f), CEGUI::UDim(0, fHeight/2-120.0f)));
        m_GUIElements.pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CQuickConnect::OnCloseClick, this));
        m_GUIElements.pWindow->setProperty("FrameEnabled", "false");
        m_GUIElements.pWindow->setVisible(false);

        m_GUIElements.pIPStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pWindow);
        m_GUIElements.pIPStaticText->setText("IP Address:");
        m_GUIElements.pIPStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0, 260), CEGUI::UDim(0, 20)));
        m_GUIElements.pIPStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 20), CEGUI::UDim(0, 20)));
        m_GUIElements.pIPStaticText->setProperty("FrameEnabled", "false");
        m_GUIElements.pIPStaticText->setProperty("BackgroundEnabled", "false");
        m_GUIElements.pIPStaticText->setFont(pGUI->GetFont("tahoma-bold"));

        m_GUIElements.pIPEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pWindow);
        m_GUIElements.pIPEditBox->setText(String("%s:%d", g_pClient->GetHost().Get(), g_pClient->GetPort()).Get());
        m_GUIElements.pIPEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0, 260), CEGUI::UDim(0, 30)));
        m_GUIElements.pIPEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 20), CEGUI::UDim(0, 50)));
        m_GUIElements.pIPEditBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&CQuickConnect::OnIPEditBoxKeyUp, this));

        m_GUIElements.pPasswordStaticText = pGUI->CreateGUIStaticText(m_GUIElements.pWindow);
        m_GUIElements.pPasswordStaticText->setText("Password (blank for none):");
        m_GUIElements.pPasswordStaticText->setSize(CEGUI::UVector2(CEGUI::UDim(0, 260), CEGUI::UDim(0, 20)));
        m_GUIElements.pPasswordStaticText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 20), CEGUI::UDim(0, 90)));
        m_GUIElements.pPasswordStaticText->setProperty("FrameEnabled", "false");
        m_GUIElements.pPasswordStaticText->setProperty("BackgroundEnabled", "false");
        m_GUIElements.pPasswordStaticText->setFont(pGUI->GetFont("tahoma-bold"));

        m_GUIElements.pPasswordEditBox = pGUI->CreateGUIEditBox(m_GUIElements.pWindow);
        m_GUIElements.pPasswordEditBox->setText(g_pClient->GetPassword().Get());
        m_GUIElements.pPasswordEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0, 260), CEGUI::UDim(0, 30)));
        m_GUIElements.pPasswordEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 20), CEGUI::UDim(0, 120)));
        m_GUIElements.pPasswordEditBox->subscribeEvent(CEGUI::Editbox::EventKeyUp, CEGUI::Event::Subscriber(&CQuickConnect::OnIPEditBoxKeyUp, this));
        m_GUIElements.pPasswordEditBox->setProperty("MaskText", "true");

        m_GUIElements.pConnectButton = pGUI->CreateGUIButton(m_GUIElements.pWindow);
        m_GUIElements.pConnectButton->setText("Connect");
        m_GUIElements.pConnectButton->setSize(CEGUI::UVector2(CEGUI::UDim(0, 200), CEGUI::UDim(0, 40)));
        m_GUIElements.pConnectButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 50), CEGUI::UDim(0, 160)));
        m_GUIElements.pConnectButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CQuickConnect::OnConnectButtonClick, this));

        m_bVisible = false;
}


CQuickConnect::~CQuickConnect(void)
{
        // Delete the GUI elements
        CGUI * pGUI = g_pClient->GetGUI();
        pGUI->RemoveGUIWindow(m_GUIElements.pWindow);
}

// (John): Do we really need to re-position the GUI every time the D3D device resets?
void CQuickConnect::OnResetDevice()
{
        CGUI * pGUI = g_pClient->GetGUI();
        
        float fWidth = (float)pGUI->GetDisplayWidth();
        float fHeight = (float)pGUI->GetDisplayHeight();
        
        m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-150.0f), CEGUI::UDim(0, fHeight/2-120.0f)));
}

void CQuickConnect::SetVisible(bool bVisible)
{
        m_GUIElements.pWindow->setVisible(bVisible);
        m_GUIElements.pWindow->setProperty("AlwaysOnTop", "true");
    m_GUIElements.pWindow->activate();

        m_bVisible = bVisible;
}

bool CQuickConnect::OnCloseClick(const CEGUI::EventArgs &eventArgs)
{
        SetVisible(false);

        // Clear the password edit box, for security
        m_GUIElements.pPasswordEditBox->setText("");

        return true;
}

bool CQuickConnect::OnIPEditBoxKeyUp(const CEGUI::EventArgs &eventArgs)
{
        const CEGUI::KeyEventArgs& key_args = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
        if(key_args.scancode == CEGUI::Key::Return) {
                OnSubmit();
                return true;
        }
        return true;
}

bool CQuickConnect::OnConnectButtonClick(const CEGUI::EventArgs &eventArgs)
{
        OnSubmit();
        return true;
}

void CQuickConnect::OnSubmit()
{
        String strHost;
        unsigned short usPort;

        if(!CMainMenu::GetHostAndPort(m_GUIElements.pIPEditBox->getText().c_str(), strHost, usPort))
        {
                g_pClient->GetGUI()->ShowMessageBox("You must enter a valid host and port.", "Error");
                return;
        }

        // Get the password
        String strPassword(m_GUIElements.pPasswordEditBox->getText().c_str());

        // Set the ip, port and password
        CVAR_SET_STRING("ip", strHost);
        CVAR_SET_INTEGER("port", usPort);
        CVAR_SET_STRING("pass", strPassword);

        // Hide the quick connect window
        SetVisible(false);

        // Call the connect function
        CMainMenu::GetSingleton()->OnConnect(strHost, usPort, strPassword);
}