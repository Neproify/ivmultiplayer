//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

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
