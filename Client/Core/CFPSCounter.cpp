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
#include <CLogFile.h>

extern CGUI * g_pGUI;
extern bool g_bFPSToggle;

CFPSCounter::CFPSCounter()
{
	m_dwLastTime = 0;
	m_iFrames = 0;
	m_iValue = 0;
	strcpy(m_szFPSTextBuffer, "FPS: 0");
}

CFPSCounter::~CFPSCounter()
{

}

int CFPSCounter::Get()
{
	return m_iValue;
}

int iCurrent = 0;

void CFPSCounter::Pulse()
{
	m_iFrames++;

	DWORD dwCurrentTime = SharedUtility::GetTime();
	DWORD dwTime = (dwCurrentTime - m_dwLastTime);

	if(dwTime >= 500)
	{
		if(dwTime >= 1000 && iCurrent == 1)
		{
			sprintf(m_szFPSTextBuffer, "FPS: %d", m_iFrames);
			m_iValue = m_iFrames;
			m_iFrames = 0;
			m_dwLastTime = dwCurrentTime;
			iCurrent = 0;
		}
		else if(iCurrent == 0)
		{
			sprintf(m_szFPSTextBuffer, "FPS: %d", (m_iFrames * 2));
			iCurrent = 1;
		}
	}

	if(g_pGUI)
	{
		CEGUI::Font * pFont = g_pGUI->GetFont("tahoma-bold", 10);

		if(pFont && g_bFPSToggle)
			g_pGUI->DrawText(m_szFPSTextBuffer, CEGUI::Vector2(5, 5), CEGUI::colour(0xFFFFFFFF), pFont);
	}
}
