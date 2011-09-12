//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNameTags.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGUI.h"

class CNameTags
{
private:
	bool          m_bEnabled;
	CEGUI::Font * m_pFont;

public:
	CNameTags();
	~CNameTags();

	void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
	bool IsEnabled() { return m_bEnabled; }
	void Draw();
};
