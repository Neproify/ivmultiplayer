//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: StdInc.h
// Project: Server.Launcher
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

// Default
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <stdio.h>
#include <Linux.h>
#endif

#include <string.h>

// Shared
#include <Common.h>
#include <CString.h>
#include <SharedUtility.h>
#include <CLibrary.h>
#include <Threading/CMutex.h>
#include <Threading/CThread.h>
#include <Server/CServerInterface.h>
