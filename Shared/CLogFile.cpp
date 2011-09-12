//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CLogFile.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CLogFile.h"
#include "SharedUtility.h"

#ifdef _LINUX
#include <stdarg.h>
#endif

FILE *            CLogFile::m_fLogFile = NULL;
bool              CLogFile::m_bUseCallback = false;
LogFileCallback_t CLogFile::m_pfnCallback = NULL;
bool              CLogFile::m_bUseTimeStamp = true;

void CLogFile::Open(String strLogFile, bool bAppend)
{
	// Open the log file
	m_fLogFile = fopen(SharedUtility::GetAbsolutePath(strLogFile).Get(), bAppend ? "a" : "w");

	// Did the log file open successfully?
	if(m_fLogFile)
	{
		// Log the log file started message
		PrintToFile("Log file started");
	}
}

void CLogFile::Print(const char * szString)
{
	// Print the message
	printf("%s\n", szString);

	// Print the message to the log file
	PrintToFile(szString);
}

void CLogFile::Printf(const char * szFormat, ...)
{
	// Collect the arguments
	va_list vaArgs;
	char szBuffer[2048];
	va_start(vaArgs, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
	va_end(vaArgs);

	// Print the message
	Print(szBuffer);
}

void CLogFile::PrintToFile(const char * szString)
{
	// If we have a callback and it is enabled call it
	if(m_bUseCallback && m_pfnCallback)
		m_pfnCallback(szString);

	// Is the log file open?
	if(m_fLogFile)
	{
		// Log the message
		if(m_bUseTimeStamp)
			fprintf(m_fLogFile, "[%s] %s\n", SharedUtility::GetTimeString(), szString);
		else
			fprintf(m_fLogFile, "%s\n", szString);

		// Flush the log file
		fflush(m_fLogFile);
	}
}

void CLogFile::PrintfToFile(const char * szFormat, ...)
{
	// Collect the arguments
	va_list vaArgs;
	char szBuffer[2048];
	va_start(vaArgs, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
	va_end(vaArgs);

	// Print the message to the log file
	PrintToFile(szBuffer);
}

void CLogFile::Close()
{
	// Is the log file open?
	if(m_fLogFile)
	{
		// Log the log file finished message
		PrintToFile("Log file finished");

		// Close the log file
		fclose(m_fLogFile);
		m_fLogFile = NULL;
	}
}
