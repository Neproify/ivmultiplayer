//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileHook.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <windows.h>
#include <list>
#include <CString.h>

class CFileHook
{
private:
	struct HookedFile
	{
		String strFileName;
		String strNewFilePath;
	};

	typedef HANDLE               (WINAPI * CreateFileA_t)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

	static CreateFileA_t         m_pfnCreateFileA;
	static bool                  m_bHookInstalled;
	static std::list<HookedFile> m_hookedFileList;

	static HANDLE WINAPI         CreateFileA_Hook(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

public:
	static void                  Install();
	static void                  Uninstall();
	static void                  AddFile(String strFileName, String strNewFilePath);
	static bool                  Remove(String strFileName);
	static bool                  IsFileHooked(String strFileName, String& strNewFilePath);
};
