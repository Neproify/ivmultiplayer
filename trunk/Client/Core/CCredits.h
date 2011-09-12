//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCredits.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGUI.h"

struct CreditText
{
	unsigned long ulTime;
	String strFont;
	unsigned int uiFontSize;
	String strText;
};

class CCredits
{
private:
	CGUI                   * m_pGUI;
	unsigned long            m_ulStartTime;
	CEGUI::Font            * m_pCurrentFont;
	CreditText             * m_pCurrentCreditText;

public:
	CCredits(CGUI * pGUI);
	~CCredits();

	String GetCreditsString();
	void   Start();
	void   End();
	void   Process();
};
