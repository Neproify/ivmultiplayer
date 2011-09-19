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
	unsigned long m_ulLastTime;
	unsigned int  m_uiCurrentFrames;
	unsigned int  m_uiTotalFramesPerSecond;

public:
	CFPSCounter();
	~CFPSCounter();

	unsigned int Get() { return m_uiTotalFramesPerSecond; }
	void         Pulse();
};
