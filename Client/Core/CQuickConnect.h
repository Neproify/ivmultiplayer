//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CQuickConnect.h
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

#include "CGUI.h"
#include "CMainMenu.h"

class CQuickConnect
{
	friend class CMainMenu;
private:
	static CQuickConnect	* m_pSingleton;
	
	bool m_bVisible;

	// Struct for GUI elements
	struct
	{
		CGUIFrameWindow		* pWindow;
		CGUIStaticText		* pIPStaticText;
		CGUIEditBox			* pIPEditBox;
		CGUIStaticText		* pPasswordStaticText;
		CGUIEditBox			* pPasswordEditBox;
		CGUIButton			* pConnectButton;
	} m_GUIElements;

	CQuickConnect(void);
	~CQuickConnect(void);

	static CQuickConnect * GetSingleton() { return m_pSingleton; }
	void			OnResetDevice();
	void			SetVisible(bool bVisible);
	bool			IsVisible() { return m_bVisible; };

	bool			OnCloseClick(const CEGUI::EventArgs &eventArgs);
	bool			OnIPEditBoxKeyUp(const CEGUI::EventArgs &eventArgs);
	bool			OnConnectButtonClick(const CEGUI::EventArgs &eventArgs);
	void			OnSubmit();
};

