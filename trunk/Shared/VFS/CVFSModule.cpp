//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFSModule.cpp
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//============================================================================== 

#define WIN32_LEAN_AND_MEAN
#include "CVFSModule.h"
#include <SharedUtility.h>

CLibrary *                  CVFSModule::m_pLibrary;
GetVFSInterface_t			CVFSModule::m_pfnGetVFSInterface;
CVFSInterface *				CVFSModule::m_pVFSInterface;

//CVFSModule ___vfs_module;

CVFSModule::CVFSModule()
{
	CVFSModule::Load();
}

bool CVFSModule::Load()
{
	m_pLibrary = new CLibrary();

	// Load the vfs module
	if(!m_pLibrary->Load(SharedUtility::GetAbsolutePath(VFS_MODULE_NAME DEBUG_SUFFIX LIBRARY_EXTENSION)))
	{
		return false;
	}

	VerifyVFSVersion_t pfnVerifyVersion = (VerifyVFSVersion_t)m_pLibrary->GetProcedureAddress("VerifyVersion");
	pfnVerifyVersion(0xFF);

	m_pfnGetVFSInterface = (GetVFSInterface_t)m_pLibrary->GetProcedureAddress("GetVFSInterface");
	
	if(!m_pfnGetVFSInterface)
	{
		return false;
	}

	m_pVFSInterface = m_pfnGetVFSInterface();

	if(!m_pVFSInterface)
	{
		return false;
	}

	return true;
}
