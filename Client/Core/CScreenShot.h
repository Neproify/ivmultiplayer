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
#include "DXSDK/Include/d3d9.h"
#include <Threading/CMutex.h>
#include <Threading/CThread.h>

class CScreenShot
{
public:
	struct ThreadData
	{
		bool   bWriting;
		bool   bSucceeded;
		String strWriteName;
		String strError;
	};

private:
	static unsigned long   m_ulLastScreenShotTime;
	static CThread         m_writeThread;
	static unsigned char * m_ucData;
	static unsigned int    m_uiScreenWidth;
	static unsigned int    m_uiScreenHeight;
	static ThreadData      m_threadData;

	static String			GetScreenShotPath();
	static void				GetFrontBufferPixels(UINT uiSizeX, UINT uiSizeY,unsigned char* buffer);
	static void				WriteImageToFile(CThread * pThread);

public:
	CScreenShot();

	static bool   Take();
	static bool   IsDone();
	static bool   HasSucceeded();
	
	static String GetWriteName();
	static String GetError();
	static void   Reset();
};
