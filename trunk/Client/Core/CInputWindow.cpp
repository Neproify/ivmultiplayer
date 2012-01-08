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

// test code
//CEGUI::Editbox * pEditBox = NULL;
// end test code
CInputWindow::CInputWindow()
{
	memset(m_szInput, 0, sizeof(m_szInput));
	m_bEnabled = false;
	m_iCurrentHistory = -1;
	m_iTotalHistory = 0;
	memset(m_szCurrent, 0, sizeof(m_szCurrent));
	memset(m_szHistory, 0, sizeof(m_szHistory));
	ClearInput();
	// test code
	// TODO
	/*pEditBox = (CEGUI::Editbox *)g_pGUI->CreateGUIEditBox();
	pEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 26.0f), CEGUI::UDim(0.0f, ((20.0f * MAX_DISPLAYED_MESSAGES) + 30.0f))));
	pEditBox->setVisible(true);
	pEditBox->setEnabled(true);
	pEditBox->activate();*/
	// end test code
}

CInputWindow::~CInputWindow()
{

}

void CInputWindow::Draw()
{
	if(m_bEnabled && g_pGUI)
	{
		CEGUI::Font * pFont = g_pGUI->GetFont("tahoma-bold", 10);

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
			g_pGraphics->DrawRect(5 , 35 + MAX_DISPLAYED_MESSAGES * 20 , 500 , 25 , m_ulChatLineBgColor);
			//Draw Text on top of the background
			size_t sLen = strlen(m_szInput);
			char * szInput = (char *)malloc(sLen + 4);
			sprintf(szInput, "> %s_", m_szInput);

			g_pGUI->DrawText(szInput, CEGUI::Vector2(26.0f, ((20 * MAX_DISPLAYED_MESSAGES) + 38)), MESSAGE_BACKGROUND_COLOR, pFont, false);
			g_pGUI->DrawText(szInput, CEGUI::Vector2(25.0f, ((20 * MAX_DISPLAYED_MESSAGES) + 37)), INPUT_COLOR, pFont, false);

			free(szInput);
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
			if(dwChar == VK_BACK)
			{
				Backspace();
				return true;
			}

			AddChar((char)dwChar);
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
	if(strlen(m_szInput) > 0)
	{
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
	if(g_pGUI->IsCursorVisible(true)) return;
	CGame::SetInputState(false);
	m_bEnabled = true;
}

void CInputWindow::Disable()
{
	if(!m_bControlsDisabled)
		CGame::SetInputState(true);

	m_bEnabled = false;
}

void CInputWindow::Backspace()
{
	size_t sLen = strlen(m_szInput);

	if(sLen == 0)
		return;

	if(m_szInput[sLen - 1] == '%')
		m_szInput[sLen - 2] = '\0';

	m_szInput[sLen - 1] = '\0';
}

void CInputWindow::AddChar(unsigned char ucChar)
{
	if(ucChar < ' ')
		return;

	size_t sLen = strlen(m_szInput);

	if(sLen < (MAX_INPUT_LENGTH - 2))
	{
		if(ucChar == '%')
		{
			m_szInput[sLen] = ucChar;
			sLen = strlen(m_szInput);
			m_szInput[sLen] = ucChar;
		}
		else
			m_szInput[sLen] = ucChar;
		m_szInput[sLen + 1] = '\0';
	}
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
		if(m_iCurrentHistory == -1)
			strcpy(m_szCurrent, m_szInput);

		m_iCurrentHistory++;
		strcpy(m_szInput, m_szHistory[m_iCurrentHistory]);
	}
}

void CInputWindow::RecallDown()
{
	if(m_iCurrentHistory > -1)
	{
		m_iCurrentHistory--;

		if(m_iCurrentHistory == -1)
			strcpy(m_szInput, m_szCurrent);
		else
			strcpy(m_szInput, m_szHistory[m_iCurrentHistory]);
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
}

void CInputWindow::CursorLeft()
{
	
}

void CInputWindow::CursorRight()
{
	
}
