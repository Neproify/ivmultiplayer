//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CExceptionHandler.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Common.h"
#include <stdio.h>

typedef void (* ExceptionHandlerCallback_t)(String& strReportData);

class CExceptionHandler
{
private:
#ifndef IVMP_DEBUG
	static              ExceptionHandlerCallback_t m_pfnCallback;

#ifdef WIN32
	static const char * ExceptionCodeToString(DWORD dwExceptionCode);
	static void         WriteExceptionReport(_EXCEPTION_POINTERS * ExceptionInfo);
	static long WINAPI  ExceptionHandler(_EXCEPTION_POINTERS * ExceptionInfo);
#else
	static void         WriteExceptionReport();
	static void         ExceptionHandler(int iSignal);
#endif
#endif

public:
#ifndef IVMP_DEBUG
	static void         Install();
	static void         SetCallback(ExceptionHandlerCallback_t pfnCallback) { m_pfnCallback = pfnCallback; }
#else
	static void         Install() { }
	static void         SetCallback(ExceptionHandlerCallback_t pfnCallback) { }
#endif
};
