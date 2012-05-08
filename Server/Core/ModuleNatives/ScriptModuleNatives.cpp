//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "../CClientFileManager.h"
#include "Scripting/CScriptTimerManager.h"

extern CScriptingManager * g_pScriptingManager;
extern CClientFileManager * g_pClientScriptFileManager;
extern CClientFileManager * g_pClientResourceFileManager;
extern CScriptTimerManager * g_pScriptTimerManager;

// Script functions
namespace Modules
{
	// getScripts()
	std::list<String> * CScriptModuleNatives::GetScripts()
	{
		std::list<CSquirrel*>* scripts = g_pScriptingManager->GetScriptList();
		std::list<String> * list = new std::list<String>;
		for(std::list<CSquirrel*>::iterator iter = scripts->begin(); iter != scripts->end(); ++ iter)
		{
			list->push_back((*iter)->GetName());
		}
		return list;
	}

	// getClientScripts()
	std::list<String> * CScriptModuleNatives::GetClientScripts()
	{
		std::list<String> * list = new std::list<String>;
		for(CClientFileManager::iterator iter = g_pClientScriptFileManager->begin(); iter != g_pClientScriptFileManager->end(); ++ iter)
		{
			list->push_back((*iter).first);
		}
		return list;
	}

	// getClientResources()
	std::list<String> * CScriptModuleNatives::GetClientResources()
	{
		std::list<String> * list = new std::list<String>;
		for(CClientFileManager::iterator iter = g_pClientResourceFileManager->begin(); iter != g_pClientResourceFileManager->end(); ++ iter)
		{
			list->push_back((*iter).first);
		}
		return list;
	}

	// getScriptName()
	const char * CScriptModuleNatives::GetScriptName(void * pVM)
	{
		String strScriptName = g_pScriptingManager->Get((SQVM*)pVM)->GetName();
		return strScriptName.CopyData();
	}

	// loadScript(script)
	bool CScriptModuleNatives::LoadScript(const char * szScript)
	{
		if(!g_pScriptingManager->Get(szScript))
		{
			String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
			CSquirrel * pScript = g_pScriptingManager->Load(szScript, strPath);

			if(!pScript)
				CLogFile::Printf("Failed to load script %s (Script does not exist/Script compilation failed).", szScript);
			else
				CLogFile::Printf("Loaded script %s.", szScript);

			return (pScript != NULL);
		}

		return false;
	}

	// unloadScript(script)
	bool CScriptModuleNatives::UnloadScript(const char * szScript)
	{
		// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
		CSquirrel* pScript = g_pScriptingManager->Get(szScript);
		if(pScript)
		{
			g_pScriptTimerManager->HandleScriptUnload(pScript);
			CLogFile::Printf("Unloaded script %s.", szScript);
			return g_pScriptingManager->Unload(szScript);
		}

		return false;
	}

	// reloadScript(script)
	bool CScriptModuleNatives::ReloadScript(const char * szScript)
	{
		// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
		CSquirrel* pScript = g_pScriptingManager->Get(szScript);

		if(pScript)
		{
			g_pScriptTimerManager->HandleScriptUnload(pScript);
			String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
			CLogFile::Printf("Reloaded script %s.", szScript);
			return (g_pScriptingManager->Unload(szScript) && g_pScriptingManager->Load(szScript, strPath));
		}

		return false;
	}

	// loadClientScript(script)
	bool CScriptModuleNatives::LoadClientScript(const char * szScript)
	{
		if(!g_pClientScriptFileManager->Exists(szScript))
		{
			return g_pClientScriptFileManager->Start(szScript);
		}

		return false;
	}

	// unloadClientScript(script)
	bool CScriptModuleNatives::UnloadClientScript(const char * szScript)
	{
		if(g_pClientScriptFileManager->Exists(szScript))
		{
			return g_pClientScriptFileManager->Stop(szScript);
		}

		return false;
	}

	// reloadClientScript(script)
	bool CScriptModuleNatives::ReloadClientScript(const char * szScript)
	{
		if(g_pClientScriptFileManager->Exists(szScript))
		{
			return g_pClientScriptFileManager->Restart(szScript);
		}

		return false;
	}

	// loadClientResource(resource)
	bool CScriptModuleNatives::LoadClientResource(const char * szResource)
	{
		if(!g_pClientResourceFileManager->Exists(szResource))
		{
			return g_pClientResourceFileManager->Start(szResource);
		}

		return false;
	}

	// unloadClientResource(resource)
	bool CScriptModuleNatives::UnloadClientResource(const char * szResource)
	{
		if(g_pClientResourceFileManager->Exists(szResource))
		{
			return g_pClientResourceFileManager->Stop(szResource);
		}

		return false;
	}

	// reloadClientResource(resource)
	bool CScriptModuleNatives::ReloadClientResource(const char * szResource)
	{
		if(g_pClientResourceFileManager->Exists(szResource))
		{
			return g_pClientResourceFileManager->Restart(szResource);
		}

		return false;
	}
}