//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Linux.h
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//==============================================================================

#include <stdarg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#define Sleep(ms) usleep(ms * 1000)

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef short unsigned int WORD;

char* itoa(int value, char* str, int radix);

#define THREAD_PRIORITY_NORMAL 4
