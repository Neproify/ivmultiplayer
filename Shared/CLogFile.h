//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CLogFile.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <stdio.h>
#include "CString.h"

typedef void (* LogFileCallback_t)(const char * szBuffer);

class CLogFile
{
private:
	static FILE *            m_fLogFile;
	static bool              m_bUseCallback;
	static LogFileCallback_t m_pfnCallback;
	static bool              m_bUseTimeStamp;

public: 
	static void              SetUseCallback(bool bUseCallback) { m_bUseCallback = bUseCallback; }
	static bool              GetUseCallback() { return m_bUseCallback; }
	static void              SetCallback(LogFileCallback_t pfnCallback) { m_pfnCallback = pfnCallback; }
	static LogFileCallback_t GetCallback() { return m_pfnCallback; }
	static void				 SetTimeStamp(bool bTimeStamp) { m_bUseTimeStamp = bTimeStamp; }
	static void              Open(String strLogFile, bool bAppend = false);
	static void              Print(const char * szString);
	static void              Printf(const char * szFormat, ...);
	static void              PrintToFile(const char * szString);
	static void              PrintfToFile(const char * szFormat, ...);
	static void              Close();
};
