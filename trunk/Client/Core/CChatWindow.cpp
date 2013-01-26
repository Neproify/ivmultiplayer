//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CChatWindow.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <stdio.h>
#include "CChatWindow.h"
#include "CLocalPlayer.h"
#include "CPlayerManager.h"
#include <CLogFile.h>
#include "CGraphics.h"
#include <CSettings.h>
#include <string.h>

extern CGUI * g_pGUI;
extern CGraphics * g_pGraphics;
extern CLocalPlayer * g_pLocalPlayer;
extern CPlayerManager * g_pPlayerManager;

CChatWindow::CChatWindow()
	: m_bEnabled(true),
	m_iCurrentPage(1)
{
	memset(&m_chatMessages, 0, sizeof(m_chatMessages));
	InitFontAndBackground();
}

CChatWindow::~CChatWindow()
{

}

void CChatWindow::Draw()
{
	// Are we enabled and do we have a valid GUI instance?
	if(m_bEnabled && g_pGUI)
	{
		// Draw the chat background
		g_pGraphics->DrawRect(5, 5, 500, 30 + MAX_DISPLAYED_MESSAGES * 20, m_ulBackgroundColor);

		// Do we have a valid font?
		if(m_pFont)
		{
			int iCurrentMessage = (m_iCurrentPage * MAX_DISPLAYED_MESSAGES) - 1;
			float fY = 30;

			for(int x = 0; x < MAX_DISPLAYED_MESSAGES; x++)
			{
				// Draw the text
				if(m_chatMessages[iCurrentMessage - x].fNameExtent)
				{
					// Draw a name shadow
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szName, CEGUI::Vector2(26.0f, fY + 1), MESSAGE_BACKGROUND_COLOR, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting, false);

					// Draw the name
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szName, CEGUI::Vector2(25.0f, fY), m_chatMessages[iCurrentMessage - x].nameColor, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting);

					// Draw a text shadow
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szMessage, CEGUI::Vector2(26.0f + m_chatMessages[iCurrentMessage - x].fNameExtent, fY + 1), MESSAGE_BACKGROUND_COLOR, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting);

					// Draw the text
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szMessage, CEGUI::Vector2(25.0f + m_chatMessages[iCurrentMessage - x].fNameExtent, fY), m_chatMessages[iCurrentMessage - x].messageColor, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting);
				}
				else
				{
					// Draw a shadow
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szMessage, CEGUI::Vector2(26.0f, fY + 1), MESSAGE_BACKGROUND_COLOR, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting, false);

					// Draw the text
					g_pGUI->DrawText(m_chatMessages[iCurrentMessage - x].szMessage, CEGUI::Vector2(25.0f, fY), m_chatMessages[iCurrentMessage - x].messageColor, m_pFont, m_chatMessages[iCurrentMessage - x].bAllowFormatting);
				}

				fY += 20;
			}
		}
	}
}

void CChatWindow::MoveUp()
{
	for(BYTE x = MAX_MESSAGE - 1; x; x--)
		memcpy(&m_chatMessages[x], &m_chatMessages[x - 1], sizeof(CHAT_MESSAGE));
}

void CChatWindow::AddChatMessage(EntityId playerId, const char * szMessage)
{
	// Ensure we have a valid message string
	if(!szMessage)
		return;
	
	// Check for % character and write %%
	char * pcChar = (char*)memchr(szMessage,'%',strlen(szMessage));
	char szMessageToSet[129];

	if(pcChar != NULL)
	{
		char szMessageA[129];
		int offset1 = 0;
		int offset2 = 0;
		strcpy(szMessageA,szMessage);
		
		while(true)
		{
			if(szMessageA[offset1] == '%')
			{
				szMessageToSet[offset2] = '%';
				szMessageToSet[offset2+1] = '%';
				offset2 += 2;
				offset1++;
				continue;
			}
			else if(szMessageA[offset1] == '\0' || offset1 == 128 || offset2 == 128)
			{
				szMessageToSet[offset2] = '\0';
				break;
			}
			else
			{
				szMessageToSet[offset2] = szMessageA[offset1];
				offset1++;
				offset2++;
			}
		}
	}
	else
		strcpy(szMessageToSet,szMessage);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(!pPlayer)
		return;

	String strName = pPlayer->GetName();
	unsigned int uiColor = pPlayer->GetColor();
	MoveUp();
	strcpy_s(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szMessageToSet);
	sprintf(m_chatMessages[0].szName, "%s: ", strName.Get());
	m_chatMessages[0].messageColor = MESSAGE_CHAT_COLOR;
	m_chatMessages[0].nameColor = CEGUI::colour(0xFF000000 | (uiColor >> 8));
	float fTextExtent = 0;

	if(m_pFont)
		fTextExtent = m_pFont->getTextExtent(m_chatMessages[0].szName);

	m_chatMessages[0].fNameExtent = fTextExtent;
	m_iMessageAmount++;

	//Write messages to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("%s: %s",m_chatMessages[0].szName,szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
}

void CChatWindow::AddErrorMessage(const char * szFormat, ...)
{
	// Ensure we have a valid format string
	if(!szFormat)
		return;

	MoveUp();
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szFormat, vaArgs);
	va_end(vaArgs);
	m_chatMessages[0].szMessage[MAX_MESSAGE_LENGTH] = '\0';
	m_chatMessages[0].messageColor = MESSAGE_ERROR_COLOR;
	m_chatMessages[0].fNameExtent = 0;
	m_chatMessages[0].bAllowFormatting = true;
	m_iMessageAmount++;

	// Write info to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("ERROR: %s",m_chatMessages[0].szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
}

void CChatWindow::AddNetworkMessage(const char * szFormat, ...)
{
	// Ensure we have a valid format string
	if(!szFormat)
		return;

	MoveUp();
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szFormat, vaArgs);
	va_end(vaArgs);
	m_chatMessages[0].szMessage[MAX_MESSAGE_LENGTH] = '\0';
	m_chatMessages[0].messageColor = MESSAGE_NETWORK_COLOR;
	m_chatMessages[0].fNameExtent = 0;
	m_chatMessages[0].bAllowFormatting = true;
	m_iMessageAmount++;

	// Write info to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("NETWORK: %s",m_chatMessages[0].szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
}

void CChatWindow::AddInfoMessage(const char * szFormat, ...)
{
	// Ensure we have a valid format string
	if(!szFormat)
		return;

	MoveUp();
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szFormat, vaArgs);
	va_end(vaArgs);
	m_chatMessages[0].szMessage[MAX_MESSAGE_LENGTH] = '\0';
	m_chatMessages[0].messageColor = MESSAGE_INFO_COLOR;
	m_chatMessages[0].fNameExtent = 0;
	m_chatMessages[0].bAllowFormatting = true;
	m_iMessageAmount++;

	// Write info to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("INFO: %s",m_chatMessages[0].szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
}

void CChatWindow::AddInfoDebugMessage(const char * szFormat, ...)
{
	// Ensure we have a valid format string
	if(!szFormat)
		return;
	
#ifdef _DEBUG
	MoveUp();
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szFormat, vaArgs);
	va_end(vaArgs);
	m_chatMessages[0].szMessage[MAX_MESSAGE_LENGTH] = '\0';
	m_chatMessages[0].messageColor = MESSAGE_INFO_COLOR;
	m_chatMessages[0].fNameExtent = 0;
	m_chatMessages[0].bAllowFormatting = true;
	m_iMessageAmount++;

	// Write info to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("DEBUG: %s",m_chatMessages[0].szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
#endif
}

void CChatWindow::AddMessage(DWORD dwColor, bool bAllowFormatting, const char * szFormat, ...)
{
	// Ensure we have a valid format string
	if(!szFormat)
		return;

	MoveUp();
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsnprintf(m_chatMessages[0].szMessage, MAX_MESSAGE_LENGTH, szFormat, vaArgs);
	va_end(vaArgs);
	m_chatMessages[0].messageColor = ((dwColor >> 8) | 0xFF000000);
	m_chatMessages[0].fNameExtent = 0;
	m_chatMessages[0].bAllowFormatting = bAllowFormatting;
	m_iMessageAmount++;

	// Write info to log
	CLogFile::Open("Chatlog.log",true);
	CLogFile::Printf("%s",m_chatMessages[0].szMessage);
	CLogFile::Close();
	CLogFile::Open("Client.log",true);//Reopen client.log
}

void CChatWindow::PageUp()
{
	if(m_iCurrentPage < MAX_PAGES)
	{
		if(m_iMessageAmount > (MAX_DISPLAYED_MESSAGES * m_iCurrentPage))
			m_iCurrentPage++;
	}
}

void CChatWindow::PageDown()
{
	if(m_iCurrentPage > 1)
		m_iCurrentPage--;
}

void CChatWindow::InitFontAndBackground()
{
	// Load our chat font
	m_pFont = g_pGUI->GetFont(CVAR_GET_STRING("chatfont").Get(), CVAR_GET_INTEGER("chatsize"));

	// Do we not have a valid font?
	if(!m_pFont)
		g_pGUI->ShowMessageBox("Invalid chat font.\nPlease set a valid font in the Chat tab of the Settings menu.", "Warning");

	// Set our background colors
	m_ulBackgroundColor = D3DCOLOR_ARGB(CVAR_GET_INTEGER("chatbga"), 
										CVAR_GET_INTEGER("chatbgr"), 
										CVAR_GET_INTEGER("chatbgg"), 
										CVAR_GET_INTEGER("chatbgb"));
}
