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
#include "CClient.h"
#include <SharedUtility.h>

extern CClient * g_pClient;

CFPSCounter::CFPSCounter()
	: m_ulLastTime(0),
	m_uiCurrentFrames(0),
	m_uiTotalFramesPerSecond(0)
{
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

	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	// Do we have a valid GUI instance?
	if(pGUI)
	{
		// Get the font
		CEGUI::Font * pFont = pGUI->GetFont("tahoma-bold");

		// Draw the current fps if needed
		if(pFont && g_pClient->GetFPSToggle())
			pGUI->DrawText(String("FPS: %d", m_uiTotalFramesPerSecond), CEGUI::Vector2(5, 5), CEGUI::colour(0xFFFFFFFF), pFont);

		// workaround for not drawing rect on Windows 8
		if(pFont)
			pGUI->DrawText(String(""), CEGUI::Vector2(5, 5), CEGUI::colour(0xFFFFFFFF), pFont);
	}
}
