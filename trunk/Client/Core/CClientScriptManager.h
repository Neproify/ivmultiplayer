//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <Scripting\CScriptingManager.h>
#include <Scripting\CScriptTimerManager.h>
#include "CClientScriptGUIManager.h"

struct ClientScript
{
	String strName;
	String strPath;
};

class CClientScriptManager
{
private:
	std::list<ClientScript *> m_clientScripts;
	CScriptingManager       * m_pScripting;
	CScriptTimerManager     * m_pScriptTimerManager;
	CClientScriptGUIManager * m_pGUIManager;

public:
	CClientScriptManager();
	~CClientScriptManager();

	CScriptingManager       * GetScriptingManager() { return m_pScripting; }
	CScriptTimerManager     * GetScriptTimerManager() { return m_pScriptTimerManager; }
	CClientScriptGUIManager * GetGUIManager() { return m_pGUIManager; }

	void                      AddScript(String strName, String strPath);
	void                      RemoveScript(String strName);
	void                      Load(String strName);
	void                      Unload(String strName);
	bool                      Exists(String strName);
	void                      LoadAll();
	void					  RemoveAll();
};
