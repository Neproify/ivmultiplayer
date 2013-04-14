//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerBrowser.h
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

#include "CGUI.h"
#include "CMainMenu.h"
#include "CMasterListQuery.h"
#include "CServerQuery.h"

class CServerBrowser
{
	friend class CMainMenu;

private:
	static CServerBrowser	* m_pSingleton;
	CMasterListQuery		* m_pMasterListQuery;
	CServerQuery			* m_pServerQuery;

	bool					m_bVisible;

	// Struct for GUI elements
	struct
	{
		CGUIFrameWindow     * pWindow;
		CGUIMultiColumnList * pServerMultiColumnList;
		CGUIButton          * pRefreshButton;
		CGUIButton          * pConnectButton;
	} m_GUIElements;

	CServerBrowser(void);
	~CServerBrowser(void);

	static CServerBrowser * GetSingleton() { return m_pSingleton; }
	void			 Process();
	void			 OnResetDevice();
	void			 SetVisible(bool bVisible);
	bool			 IsVisible() { return m_bVisible; };

	static void      ServerQueryHandler(String strHost, unsigned short usPort, String strQuery, CBitStream * pReply);
	static void      MasterListQueryHandler(std::vector<String> serverVector);
	void             OnMasterListQuery(int iType);

	bool			 OnCloseClick(const CEGUI::EventArgs &eventArgs);
	bool			 OnRowClick(const CEGUI::EventArgs &eventArgs);
	bool			 OnRefreshButtonClick(const CEGUI::EventArgs &eventArgs);
	bool			 OnConnectButtonClick(const CEGUI::EventArgs &eventArgs);	
};