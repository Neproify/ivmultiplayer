//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScreenShot.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>
#include <Threading/CThread.h>

class CScreenShot
{
private:
	static unsigned long   m_ulLastScreenShotTime;
	static CThread         m_writeThread;
	static unsigned char * m_ucData;
	static unsigned int    m_uiScreenWidth;
	static unsigned int    m_uiScreenHeight;

	static String GetScreenShotPath();
	static void   WriteImageToFile(CThread * pThread);

public:
	static bool   Take();
};
