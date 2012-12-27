//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: main.h
// Project: Client.CrashReporter
// Author(s): FRi
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once

#ifndef WIN32
#define WIN32
#endif

#ifndef WIN_32
#define WIN_32
#endif

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <Network\CHttpClient.h>
#include <CString.h>
#include <SharedUtility.h>