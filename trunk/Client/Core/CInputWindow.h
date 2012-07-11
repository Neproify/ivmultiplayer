//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CInputWindow.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include "CGUI.h"
#include "CChatWindow.h"

#define MAX_INPUT_LENGTH MAX_MESSAGE_LENGTH
#define MAX_RECALLS 10
#define INPUT_COLOR (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255)
#define COMMAND_CHAR '/'
#define MAX_COMMAND_LENGTH 128

typedef void (* Command_t)(char* szParams);

struct ClientCommand
{
	char szCommandName[MAX_COMMAND_LENGTH];
	Command_t pCommandFunc;
};

class CInputWindow
{
private:
	char						m_szInput[MAX_INPUT_LENGTH];
	int							m_iCurrentHistory;
	int							m_iTotalHistory;
	char						m_szCurrent[MAX_INPUT_LENGTH];
	char						m_szHistory[MAX_RECALLS][MAX_INPUT_LENGTH];
	bool						m_bEnabled;
	std::list<ClientCommand *>	m_pCommands;
	unsigned long				m_ulChatLineBgColor;
	CGUIStaticImage				* m_pEditBoxImage;
	CGUIEditBox					* m_pEditBox;

public:
	CInputWindow();
	~CInputWindow();

	void Draw();
	bool HandleInput(UINT uMsg, DWORD dwChar);
	void ProcessInput();
	bool IsEnabled();
	void Enable();
	void Disable();
	void AddToHistory(char * szMessage);
	void RelocateHistory();
	void RecallUp();
	void RecallDown();
	void RegisterCommand(char * szCommandName, Command_t pCommandFunc);
	void UnregisterCommand(char * szCommandName);
	void ClearInput();
	void CursorLeft();
	void CursorRight();
	String GetChatBoxText();
};
