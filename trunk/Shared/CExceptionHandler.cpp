//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CExceptionHandler.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CExceptionHandler.h"

#ifndef IVMP_DEBUG
#include <time.h>
#include "CString.h"
#include "SharedUtility.h"
#include "CLogFile.h"
#ifdef WIN32
#include <dbghelp.h>
#include <tlhelp32.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <execinfo.h>
#include <signal.h>
#endif

ExceptionHandlerCallback_t CExceptionHandler::m_pfnCallback = NULL;

#ifdef WIN32
const char * CExceptionHandler::ExceptionCodeToString(DWORD dwExceptionCode)
{
	switch(dwExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION: return "Access Violation";
	case EXCEPTION_DATATYPE_MISALIGNMENT: return "Datatype Misalignment";
	case EXCEPTION_BREAKPOINT: return "Breakpoint";
	case EXCEPTION_SINGLE_STEP: return "Single Step";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "Array Bounds Exceeded";
	case EXCEPTION_FLT_DENORMAL_OPERAND: return "Float: Denormal Operand";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "Float: Divide By Zero";
	case EXCEPTION_FLT_INEXACT_RESULT: return "Float: Inexact Result";
	case EXCEPTION_FLT_INVALID_OPERATION: return "Float: Invalid Operation";
	case EXCEPTION_FLT_OVERFLOW: return "Float: Overflow";
	case EXCEPTION_FLT_STACK_CHECK: return "Float: Stack Check";
	case EXCEPTION_FLT_UNDERFLOW: return "Float: Underflow";
	case EXCEPTION_INT_DIVIDE_BY_ZERO: return "Integer: Divide By Zero";
	case EXCEPTION_INT_OVERFLOW: return "Integer: Overflow";
	case EXCEPTION_PRIV_INSTRUCTION: return "Priv Instruction";
	case EXCEPTION_IN_PAGE_ERROR: return "In Page Error";
	case EXCEPTION_ILLEGAL_INSTRUCTION: return "Illegal Instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "Non-continuable Exception";
	case EXCEPTION_STACK_OVERFLOW: return "Stack Overflow";
	case EXCEPTION_INVALID_DISPOSITION: return "Invalid Disposition";
	case EXCEPTION_GUARD_PAGE: return "Guard Page";
	case EXCEPTION_INVALID_HANDLE: return "Invalid Handle";
	case 0xC0000194L: return "Possible Deadlock";
	}

	return "Unknown";
}
#endif

#ifdef WIN32
void CExceptionHandler::WriteExceptionReport(_EXCEPTION_POINTERS * ExceptionInfo)
#else
void CExceptionHandler::WriteExceptionReport()
#endif
{
	// Get the current time and date
	time_t t = time(NULL);
	const struct tm * tm = localtime(&t);

	// Get the 'crashinfo' directory path
	String strPath(SharedUtility::GetAbsolutePath("crashinfo"));

	// Create the 'crashinfo' directory if needed
	if(!SharedUtility::Exists(strPath))
		SharedUtility::CreateDirectory(strPath);

	// Append the client or server string to the path
#ifdef _SERVER
	strPath.Append("/Server");
#else
	strPath.Append("/Client");
#endif

	// Append the operating system string to the path
	strPath.Append("-" OS_STRING);

	// Append the version, date and time to the path
	strPath.AppendF("-" MOD_VERSION_STRING "-%04d.%02d.%02d-%02d.%02d.%02d", (tm->tm_year + 1900), (tm->tm_mon + 1), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	// Get the log file path
	String strLogPath("%s.log", strPath.Get());

	// Open the log file
	FILE * fFile = fopen(strLogPath, "w");

	// Did the log file open successfully?
	if(fFile)
	{
		// Write the unhandled exception report start notice to the log file
		fprintf(fFile, "-------------- Unhandled Exception Report Start --------------\n");

#ifdef WIN32
		// Write the exception code and exception code string to the log file
		fprintf(fFile, "Exception code: 0x%p (%s)\n", ExceptionInfo->ExceptionRecord->ExceptionCode, 
			ExceptionCodeToString(ExceptionInfo->ExceptionRecord->ExceptionCode));

		// Write the exception address to the log file
		fprintf(fFile, "Exception address: 0x%p\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);

		// Create a tool help 32 process snapshot
		HANDLE hModuleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

		if(hModuleSnapShot)
		{
			MODULEENTRY32 ModuleEntry;
			ModuleEntry.dwSize = sizeof(ModuleEntry);

			if(Module32First(hModuleSnapShot, &ModuleEntry))
			{
				// Enumerate through all modules
				while(Module32Next(hModuleSnapShot, &ModuleEntry))
				{
					// See if exception was within this module
					if((ExceptionInfo->ContextRecord->Eip >= (DWORD)ModuleEntry.modBaseAddr) && (ExceptionInfo->ContextRecord->Eip <= ((DWORD)ModuleEntry.modBaseAddr + ModuleEntry.modBaseSize)))
					{
						fprintf(fFile, "Exception module: %s (+0x%p)\n", ModuleEntry.szModule, (ExceptionInfo->ContextRecord->Eip - (DWORD)ModuleEntry.modBaseAddr));
						break;
					}
				}
			}
		}

		// Write the registers segment header
		fprintf(fFile, "Exception registers: \n");

		// If we have segments context information then write it to the log file
		if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_SEGMENTS)
		{
			fprintf(fFile, "GS=0x%p FS=0x%p ES=0x%p DS=0x%p\n", ExceptionInfo->ContextRecord->SegGs, 
				ExceptionInfo->ContextRecord->SegFs, ExceptionInfo->ContextRecord->SegEs, 
				ExceptionInfo->ContextRecord->SegDs);
		}

		// If we have integer context information then write it to the log file
		if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_INTEGER)
		{
			fprintf(fFile, "EDI=0x%p ESI=0x%p EBX=0x%p EDX=0x%p\n", ExceptionInfo->ContextRecord->Edi, 
				ExceptionInfo->ContextRecord->Esi, ExceptionInfo->ContextRecord->Ebx,
				ExceptionInfo->ContextRecord->Edx);
			fprintf(fFile, "ECX=0x%p EAX=0x%p\n", ExceptionInfo->ContextRecord->Ecx, 
				ExceptionInfo->ContextRecord->Eax);
		}

		// If we have control context information then write it to the log file
		if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_CONTROL)
		{
			fprintf(fFile, "EBP=0x%p EIP=0x%p CS=0x%p EFLAGS=0x%p\n", ExceptionInfo->ContextRecord->Ebp, 
				ExceptionInfo->ContextRecord->Eip, ExceptionInfo->ContextRecord->SegCs, 
				ExceptionInfo->ContextRecord->EFlags);
			fprintf(fFile, "ESP=0x%p SS=0x%p\n", ExceptionInfo->ContextRecord->Esp, 
				ExceptionInfo->ContextRecord->SegSs);
		}
#else
		void * pArray[50];
		int size = backtrace(pArray, 50);
		char ** szMessages = backtrace_symbols(pArray, size);

		for(int i = 0; i < size && szMessages != NULL; i++)
			fprintf(fFile, "[Backtrace %d]: %s\n", szMessages[i]);
#endif

		// If we have a callback call it
		if(m_pfnCallback)
			m_pfnCallback(fFile);

		// Write the unhandled exception report end notice to the log file
		fprintf(fFile, "--------------- Unhandled Exception Report End ---------------\n");
	}

	// Close the log file
	fclose(fFile);

#ifdef WIN32
	// Get the minidump file path
	String strMiniDumpPath("%s.dmp", strPath.Get());

	// Open the minidump file
	HANDLE hFile = CreateFileA(strMiniDumpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL);

	// Did the minidump file open successfully?
	if(hFile)
	{
		// Create the minidump exception information
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = ExceptionInfo;
		exceptionInfo.ClientPointers = FALSE;

		// Write the minidump to the minidump file
		bool bWritten = (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exceptionInfo, NULL, NULL) != 0);

		// Close the minidump file
		CloseHandle(hFile);
	}
#endif

	// Print a message in the log file
	CLogFile::Printf("IV:MP has crashed. Please see %s for more information.", strLogPath.Get());
}

#ifdef WIN32
long WINAPI CExceptionHandler::ExceptionHandler(_EXCEPTION_POINTERS * ExceptionInfo)
#else
void CExceptionHandler::ExceptionHandler(int iSignal)
#endif
{
#ifdef WIN32
	// Write the exception report
	WriteExceptionReport(ExceptionInfo);
#endif
	// Exit the current process
#ifndef WIN32
	exit(0);
#else
	SharedUtility::_TerminateProcess("GTAIV.exe");

	return EXCEPTION_EXECUTE_HANDLER;	
#endif
}

void CExceptionHandler::Install()
{
	// Set the unhandled exception filter
#ifdef WIN32
	SetUnhandledExceptionFilter(ExceptionHandler);
#else
 	signal(SIGSEGV, ExceptionHandler);
#endif
}
#endif
