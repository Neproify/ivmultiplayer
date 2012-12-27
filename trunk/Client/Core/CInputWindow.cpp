//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CInputWindow.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CInputWindow.h"
#include <CLogFile.h>
#include "Scripting.h"
#include "CNetworkManager.h"
#include "CChatWindow.h"
#include "CGUI.h"
#include "CDirectInput8Proxy.h"
#include <Scripting\CScriptingManager.h>
#include <CEvents.h>
#include <CSettings.h>
#include "CGraphics.h"

extern CNetworkManager * g_pNetworkManager;
extern CChatWindow * g_pChatWindow;
extern CScriptingManager * g_pScriptingManager;
extern CGUI * g_pGUI;
extern CEvents * g_pEvents;
extern bool m_bControlsDisabled;
extern CGraphics * g_pGraphics;

CInputWindow::CInputWindow()
	: m_bEnabled(false),
	m_iCurrentHistory(-1),
	m_iTotalHistory(0)
{
	memset(m_szInput, 0, sizeof(m_szInput));
	memset(m_szCurrent, 0, sizeof(m_szCurrent));
	memset(m_szHistory, 0, sizeof(m_szHistory));

	if(g_pGUI && g_pGUI->IsInitialized())
	{
		m_pEditBox = g_pGUI->CreateGUIEditBox(g_pGUI->GetDefaultWindow());
		m_pEditBox->setText("");
		m_pEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 75.0f), CEGUI::UDim(0.0f, ((20.0f * MAX_DISPLAYED_MESSAGES) + 30.0f))));
		m_pEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.03375f, 0)));
		m_pEditBox->setFont(g_pGUI->GetFont(CVAR_GET_STRING("chatfont"),CVAR_GET_INTEGER("chatsize")));
		m_pEditBox->setVisible(false);

		m_pEditBoxImage = g_pGUI->CreateGUIStaticImage(g_pGUI->GetDefaultWindow());
		m_pEditBoxImage->setProperty("FrameEnabled", "false");
		m_pEditBoxImage->setProperty("BackgroundEnabled", "false");
		m_pEditBoxImage->setProperty("Image", "set:Chat image:full_image");
		m_pEditBoxImage->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 26.0f), CEGUI::UDim(0.0f, ((20.0f * MAX_DISPLAYED_MESSAGES) + 31.5f))));
		m_pEditBoxImage->setSize(CEGUI::UVector2(CEGUI::UDim(0.025f,0), CEGUI::UDim(0.027f, 0)));
		m_pEditBoxImage->setVisible(false);
	}

	ClearInput();
}

CInputWindow::~CInputWindow()
{

}

void CInputWindow::Draw()
{
	if(m_bEnabled && g_pGUI)
	{
		CEGUI::Font * pFont = g_pGUI->GetFont(CVAR_GET_STRING("chatfont"),CVAR_GET_INTEGER("chatsize"));
		
		if(pFont)
		{
			//Draw InputLine Background
			unsigned short int usAlphaColor;
			if(CVAR_GET_INTEGER("chatbga") == 0)
				usAlphaColor = 0;
			else if(CVAR_GET_INTEGER("chatbga") > 200)
				usAlphaColor = 255;
			else
				usAlphaColor = CVAR_GET_INTEGER("chatbga") + 55;
			m_ulChatLineBgColor = D3DCOLOR_ARGB(usAlphaColor,CVAR_GET_INTEGER("chatbgr"),
												CVAR_GET_INTEGER("chatbgg"),CVAR_GET_INTEGER("chatbgb"));
			g_pGraphics->DrawRect(5 , 35 + MAX_DISPLAYED_MESSAGES * 20 , 450 , 25 , m_ulChatLineBgColor);
		}
	}
}

bool CInputWindow::HandleInput(UINT uMsg, DWORD dwChar)
{
	if(IsEnabled())
	{
		if(uMsg == WM_KEYUP)
		{
			if(dwChar == VK_RETURN)
			{
				ProcessInput();
				return true;
			}
			else if(dwChar == VK_ESCAPE)
			{
				Disable();
				return true;
			}
			else if(dwChar == VK_UP)
			{
				RecallUp();
				return true;
			}
			else if(dwChar == VK_DOWN)
			{
				RecallDown();
				return true;
			}
			else if(dwChar == VK_LEFT)
			{
				CursorLeft();
				return true;
			}
			else if(dwChar == VK_RIGHT)
			{
				CursorRight();
				return true;
			}
		}
		else if(uMsg == WM_CHAR)
		{
			if((unsigned char) dwChar < ' ')
                return false;

			return true;
		}
	}
	else 
	{
		if(uMsg == WM_KEYUP)
		{
			if(dwChar == VK_F6)
			{
				Enable();
				return true;
			}
		}
		else if(uMsg == WM_CHAR)
		{
			switch(dwChar)
			{
			case '`':
			case 't':
			case 'T':
				Enable();
				return true;
			}
		}
	}

	return false;
}

void CInputWindow::ProcessInput()
{
	if(strlen(GetChatBoxText().C_String()) > 0 && IsEnabled())
	{
		if(strlen(GetChatBoxText()) < 125)
			sprintf(m_szInput,"%s",GetChatBoxText().C_String());
		else
		{
			g_pChatWindow->AddInfoMessage("INFO: Text input too long(Got %d(max. 128))",strlen(GetChatBoxText()));
			return;
		}
		if(m_szInput[0] == COMMAND_CHAR)
		{
			// Command entered
			if(strlen(m_szInput) > 1)
			{
				// Add the command to the history
				AddToHistory(m_szInput);

				// Save the original command
				char szInput[MAX_COMMAND_LENGTH];
				strcpy(szInput, m_szInput);

				// Get the command name and params
				char * szCmd = strtok((szInput + 1), " ");
				char * szParams = strtok(NULL, "");

				// Check if theres a command registered for it
				if(m_pCommands.size() > 0)
				{
					std::list<ClientCommand *>::iterator iter;
					for(iter = m_pCommands.begin(); iter != m_pCommands.end(); iter++)
					{
						if(!_stricmp((*iter)->szCommandName, szCmd))
						{
							// A command has been registered for it
							(*iter)->pCommandFunc(szParams);
							ClearInput();
							Disable();
							return;
						}
					}
				}

				// Check if we are connected
				if(g_pNetworkManager && g_pNetworkManager->HasJoinedGame())
				{
					// Event for client-side commands
					if(g_pEvents)
					{
						CSquirrelArguments pArguments;
						pArguments.push(szInput);
						g_pEvents->Call("playerCommand", &pArguments);
					}

					// Send the command to the server
					CBitStream bsSend;
					bsSend.Write(String(m_szInput));
					g_pNetworkManager->RPC(RPC_Command, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE);
				}
			}
		}
		else
		{
			// Add the text to the history
			AddToHistory(m_szInput);

			if(g_pNetworkManager && g_pNetworkManager->HasJoinedGame())
			{
				CBitStream bsSend;
				bsSend.Write(String(m_szInput));
				g_pNetworkManager->RPC(RPC_Chat, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE);
			}
		}

		ClearInput();
	}

	Disable();
}

bool CInputWindow::IsEnabled()
{
	return m_bEnabled;
}

void CInputWindow::Enable()
{
	g_pGUI->SetCursorVisible(true);
	CGame::SetInputState(false);

	// Disable it again
	m_pEditBox->setVisible(false);
	m_pEditBox->setEnabled(false);
	m_pEditBoxImage->setVisible(false);
	m_pEditBox->deactivate();

	// Enable it now
	m_pEditBox->setVisible(true);
	m_pEditBox->setEnabled(true);
	m_pEditBoxImage->setVisible(true);
	m_pEditBox->activate();

	m_bEnabled = true;
}

void CInputWindow::Disable()
{
	if(!m_bControlsDisabled)
		CGame::SetInputState(true);

	m_pEditBox->setVisible(false);
	m_pEditBoxImage->setVisible(false);
	m_pEditBox->deactivate();
	g_pGUI->SetCursorVisible(false);

	m_bEnabled = false;
}

void CInputWindow::AddToHistory(char * szMessage)
{
	// Move all recalls up
	RelocateHistory();

	// Add current buffer to recall
	strcpy(m_szHistory[0], szMessage);
	memset(m_szCurrent, 0, sizeof(m_szCurrent));

	// Reset the current history
	m_iCurrentHistory = -1;

	// Increment the total recalls count
	if(m_iTotalHistory < MAX_RECALLS)
		m_iTotalHistory++;
}

void CInputWindow::RelocateHistory()
{
	for(BYTE x = MAX_RECALLS - 1; x; x--)
		strcpy(m_szHistory[x], m_szHistory[x - 1]);
}

void CInputWindow::RecallUp()
{
	if(m_iCurrentHistory < MAX_RECALLS && ((m_iTotalHistory - 1) > m_iCurrentHistory))
	{
		m_pEditBox->setEnabled(true);
		m_iCurrentHistory++;

		int len = strlen(m_szHistory[m_iCurrentHistory]);
		m_pEditBox->setText(CGUI::AnsiToCeguiFriendlyString(m_szHistory[m_iCurrentHistory], len));
		m_pEditBox->activate();
		((CEGUI::Editbox*)m_pEditBox)->setCaratIndex(len);
	}
}

void CInputWindow::RecallDown()
{
	if(m_iCurrentHistory > -1)
	{
		m_iCurrentHistory--;

		m_pEditBox->setEnabled(true);

		if(m_iCurrentHistory == -1)
		{
			int len = strlen(m_szCurrent);
			m_pEditBox->setText(CGUI::AnsiToCeguiFriendlyString(m_szCurrent, len));
			m_pEditBox->activate();
		}
		else
		{
			int len = strlen(m_szHistory[m_iCurrentHistory]);
			m_pEditBox->setText(CGUI::AnsiToCeguiFriendlyString(m_szHistory[m_iCurrentHistory], len));
			m_pEditBox->activate();
		}
	}
}

void CInputWindow::RegisterCommand(char * szCommandName, Command_t pCommandFunc)
{
	ClientCommand * pCommand = new ClientCommand();
	strcpy(pCommand->szCommandName, szCommandName);
	pCommand->pCommandFunc = pCommandFunc;
	m_pCommands.push_back(pCommand);
}

void CInputWindow::UnregisterCommand(char * szCommandName)
{
	if(m_pCommands.size() > 0)
	{
		std::list<ClientCommand *>::iterator iter;
		for(iter = m_pCommands.begin(); iter != m_pCommands.end(); iter++)
		{
			if(!strcmp((*iter)->szCommandName, szCommandName))
			{
				ClientCommand * pCommand = *iter;
				m_pCommands.remove(pCommand);
				delete pCommand;
				break;
			}
		}
	}
}

void CInputWindow::ClearInput()
{
	memset(m_szInput, 0, sizeof(m_szInput));
	if(m_pEditBox && g_pGUI && g_pGUI->IsInitialized())
		m_pEditBox->setText("");
}

void CInputWindow::CursorLeft()
{
	
}

void CInputWindow::CursorRight()
{
	
}

String CInputWindow::GetChatBoxText()
{
	if(m_pEditBox)
	{
		String strMessage;
		strMessage.AppendF("%s",m_pEditBox->getText().c_str());
		return strMessage;
	}
	return String("Failed to get message input!");
}
