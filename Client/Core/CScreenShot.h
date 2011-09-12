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

class CScreenShot
{
private:
	static unsigned long m_ulLastScreenShotTime;

	static String GetScreenShotPath();
	static bool   WriteImageToFile(unsigned char * ucData, unsigned int uiScreenWidth, unsigned int uiScreenHeight);

public:
	static bool   Take();
};
