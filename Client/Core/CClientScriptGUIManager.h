//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptGUIManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGUI.h"
#include <list>
#include <Scripting/CSquirrel.h>

struct GUIElement
{
	CEGUI::Window * pWindow;
	CSquirrel     * pScript;
	bool            bState;
};

class CClientScriptGUIManager
{
private:
	std::list<GUIElement *> m_elements;
	bool m_bHidden;

public:
	CClientScriptGUIManager();
	~CClientScriptGUIManager();

	bool        IsHidden() { return m_bHidden; } 
	void        Add(CEGUI::Window * pWindow, CSquirrel * pScript);
	void        Delete(CEGUI::Window * pWindow);
	void        DeleteAll();
	bool        Exists(CEGUI::Window * pWindow);
	void        Show();
	void        Hide();
	CSquirrel * GetScript(CEGUI::Window * pWindow);
};
