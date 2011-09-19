//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFPSCounter.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

// TODO: SetVisible(SetEnabled?) function

#include "CFPSCounter.h"
#include <SharedUtility.h>

extern CGUI * g_pGUI;
extern bool g_bFPSToggle;

CFPSCounter::CFPSCounter()
{
	m_ulLastTime = 0;
	m_uiCurrentFrames = 0;
	m_uiTotalFramesPerSecond = 0;
}

CFPSCounter::~CFPSCounter()
{

}

void CFPSCounter::Pulse()
{
	// Increment the current frame count
	m_uiCurrentFrames++;

	// Get the current time
	unsigned long ulCurrentTime = SharedUtility::GetTime();

	// Has one second passed?
	if((ulCurrentTime - m_ulLastTime) >= 1000)
	{
		// Set the total frames per second
		m_uiTotalFramesPerSecond = m_uiCurrentFrames;

		// Reset the current frame count
		m_uiCurrentFrames = 0;

		// Set the last time
		m_ulLastTime = ulCurrentTime;
	}

	// Do we have a valid GUI instance?
	if(g_pGUI)
	{
		// Get the font
		CEGUI::Font * pFont = g_pGUI->GetFont("tahoma-bold", 10);

		// Draw the current fps if needed
		if(pFont && g_bFPSToggle)
			g_pGUI->DrawText(String("FPS: %d", m_uiTotalFramesPerSecond), CEGUI::Vector2(5, 5), CEGUI::colour(0xFFFFFFFF), pFont);
	}
}
