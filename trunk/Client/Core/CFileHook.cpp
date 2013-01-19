//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileHook.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileHook.h"
#include <Patcher/CPatcher.h>
#include <SharedUtility.h>
#include <CLogFile.h>

CFileHook::CreateFileA_t         CFileHook::m_pfnCreateFileA;
bool                             CFileHook::m_bHookInstalled;
std::list<CFileHook::HookedFile> CFileHook::m_hookedFileList;

HANDLE WINAPI CFileHook::CreateFileA_Hook(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	String strNewFilePath;

	if(IsFileHooked(SharedUtility::StripPath2(String(lpFileName).GetData()), strNewFilePath))
	{
		CLogFile::Printf("Opening hooked file '%s'", strNewFilePath.Get());
		return m_pfnCreateFileA(strNewFilePath.Get(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	return m_pfnCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

void CFileHook::Install()
{
	if(!m_bHookInstalled)
	{
		m_pfnCreateFileA = (CreateFileA_t)CPatcher::InstallDetourPatch("Kernel32.dll", "CreateFileA", (DWORD)CreateFileA_Hook);
		CLogFile::Printf("Hooked 'CreateFileA' in 'Kernel32.dll' (0x%p)", m_pfnCreateFileA);

		m_bHookInstalled = true;
	}
}

void CFileHook::Uninstall()
{
	if(m_bHookInstalled)
	{
		if(m_pfnCreateFileA)
			CPatcher::UninstallDetourPatch((PBYTE)m_pfnCreateFileA, (DWORD)CreateFileA_Hook);

		m_bHookInstalled = false;
	}
}

void CFileHook::AddFile(String strFileName, String strNewFilePath)
{
	HookedFile hookedFile;
	hookedFile.strFileName = strFileName;
	hookedFile.strNewFilePath = strNewFilePath;
	m_hookedFileList.push_back(hookedFile);
	CLogFile::Printf("Added hook for file '%s' to '%s'", strFileName.Get(), strNewFilePath.Get());
}

bool CFileHook::Remove(String strFileName)
{
	if(m_hookedFileList.size() > 0)
	{
		for(std::list<HookedFile>::iterator i = m_hookedFileList.begin(); i != m_hookedFileList.end(); i++)
		{
			if(i->strFileName == strFileName)
			{
				m_hookedFileList.erase(i);
				return true;
			}
		}
	}

	return false;
}

bool CFileHook::IsFileHooked(String strFileName, String& strNewFilePath)
{
	if(m_hookedFileList.size() > 0)
	{
		for(std::list<HookedFile>::iterator i = m_hookedFileList.begin(); i != m_hookedFileList.end(); i++)
		{
			if(i->strFileName == strFileName)
			{
				strNewFilePath = i->strNewFilePath;
				return true;
			}
		}
	}

	return false;
}
