//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMutex.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#ifdef WIN32
#include <windows.h>
#endif
#include <CString.h>

#define USE_CRITICAL_SECTION

class CMutex
{
private:
#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	// Faster than a standard mutex (But not shared among processes)
	CRITICAL_SECTION m_criticalSection;
#else
	HANDLE m_hMutex;
#endif
#else
	pthread_mutex_t m_mutex;
#endif

public:
	CMutex();
	~CMutex();

	void Lock();
	bool TryLock(unsigned int uiTimeOutMilliseconds = 0);
	void Unlock();
};
