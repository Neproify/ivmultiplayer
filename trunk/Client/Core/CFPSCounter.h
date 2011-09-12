//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFPSCounter.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGUI.h"

class CFPSCounter
{
private:
	DWORD m_dwLastTime;
	int m_iFrames;
	int m_iValue;
	char m_szFPSTextBuffer[16];

public:
	CFPSCounter();
	~CFPSCounter();

	int  Get();
	void Pulse();
};
