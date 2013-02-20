//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServer.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Server\CServerInterface.h>
#include <Threading\CMutex.h>
#include <queue>

// move start
#ifndef WIN32
#include <string.h>
#include <sys/time.h>

typedef bool BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char *PCHAR;
typedef void *PVOID;
typedef unsigned char BYTE;

#define TRUE (1)
#define FALSE (0)

#define stricmp strcasecmp
#define Sleep(ms) usleep(ms * 1000)
#endif

typedef unsigned char DimensionId;
// move end

class CServer : public CServerInterface
{
private:
	bool               m_bActive;
	bool               m_bShowFPS;
	unsigned long      m_ulLastFPSUpdateTime;
	unsigned long      m_ulFrameCount;
	unsigned long      m_ulFramesPerSecond;
	CMutex             m_inputQueueMutex; // Mutex for m_inputQueue
	std::queue<String> m_inputQueue;

	void        ProcessInputQueue();

public:
	CServer();
	~CServer();

	bool        OnLoad(int argc, char ** argv);
	void        Process();
	void        OnUnload();
	void        SetActive(bool bActive) { m_bActive = bActive; }
	bool        IsActive() { return m_bActive; }
	void        SendInput(String strInput); // Not threadsafe
	void        AddToInputQueue(String strString); // Threadsafe
	void        SetTitle(String strTitle);
};
