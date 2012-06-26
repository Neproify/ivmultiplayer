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
#include "..\Shared\Scripting\CScriptingManager.h"
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
	CClientScriptGUIManager * m_pGUIManager;

public:
	CClientScriptManager();
	~CClientScriptManager();

	CScriptingManager       * GetScriptingManager() { return m_pScripting; }
	CClientScriptGUIManager * GetGUIManager() { return m_pGUIManager; }
	void                      AddScript(String strName, String strPath);
	void                      RemoveScript(String strName);
	void                      Load(String strName);
	void                      Unload(String strName);
	bool                      Exists(String strName);
	void                      LoadAll();
	void					  RemoveAll();
};
