//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSettingsMenu.h
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

#include "CGUI.h"
#include "CMainMenu.h"

class CSettingsMenu
{
	friend class CMainMenu;
private:
	static CSettingsMenu	* m_pSingleton;

	bool m_bVisible;

	// Struct for GUI elements
	// Struct for GUI elements
	struct
	{
		CGUIFrameWindow		* pWindow;
		CGUITabControl		* pTabControl;
		CGUITabContentPane	* pGeneralPane;
		CGUITabContentPane	* pChatPane;
		CGUIStaticText		* pNickStaticText;
		CGUIEditBox			* pNickEditBox;
		CGUIStaticText		* pWindowedStaticText;
		CGUICheckBox		* pWindowedCheckBox;
		CGUIStaticText		* pFPSStaticText;
		CGUICheckBox		* pFPSCheckBox;
		CGUIStaticText		* pChatFontSizeStaticText;
		CGUIStaticText		* pChatBackgroundStaticText;
		CGUIEditBox			* pChatFontEditBox;
		CGUIEditBox			* pChatFontSizeEditBox;
		CGUIEditBox			* pChatBGColorAEditBox;
		CGUIEditBox			* pChatBGColorREditBox;
		CGUIEditBox			* pChatBGColorGEditBox;
		CGUIEditBox			* pChatBGColorBEditBox;
		CGUIButton			* pSaveButton;
	} m_GUIElements;

	CSettingsMenu(void);
	~CSettingsMenu(void);

	static CSettingsMenu * GetSingleton() { return m_pSingleton; }
	void			OnResetDevice();
	void			SetVisible(bool bVisible);
	bool			IsVisible() { return m_bVisible; };

	bool			OnCloseClick(const CEGUI::EventArgs &eventArgs);
	bool			OnSaveButtonClick(const CEGUI::EventArgs &eventArgs);
public:
	void			SetNick(const char * strNick);
};

