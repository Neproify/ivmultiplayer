//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CClientFileManager.h"
#include "Scripting/CScriptTimerManager.h"

extern CScriptingManager * g_pScriptingManager;
extern CClientFileManager * g_pClientScriptFileManager;
extern CClientFileManager * g_pClientResourceFileManager;
extern CScriptTimerManager * g_pScriptTimerManager;

// Script functions
/*
void RegisterScriptNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getScripts", sq_server_getscripts, 0, NULL);
	pScriptingManager->RegisterFunction("getClientScripts", sq_server_getclientscripts, 0, NULL);
	pScriptingManager->RegisterFunction("getScriptName", sq_server_getscriptname, 0, NULL);
	pScriptingManager->RegisterFunction("loadScript", sq_server_loadscript, 1, "s");
	pScriptingManager->RegisterFunction("unloadScript", sq_server_unloadscript, 1, "s");
	pScriptingManager->RegisterFunction("reloadScript", sq_server_reloadscript, 1, "s");
	pScriptingManager->RegisterFunction("loadClientScript", sq_server_loadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("unloadClientScript", sq_server_unloadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("reloadClientScript", sq_server_reloadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("loadClientResource", sq_server_loadclientresource, 1, "s");
	pScriptingManager->RegisterFunction("unloadClientResource", sq_server_unloadclientresource, 1, "s");
	pScriptingManager->RegisterFunction("reloadClientResource", sq_server_reloadclientresource, 1, "s");
}

// getScripts()
SQInteger sq_server_getscripts(SQVM * pVM)
{
	sq_newarray(pVM, 0);
	std::list<CSquirrel*>* scripts = g_pScriptingManager->GetScriptList();
	for(std::list<CSquirrel*>::iterator iter = scripts->begin(); iter != scripts->end(); ++ iter)
	{
		sq_pushstring(pVM, (*iter)->GetName().C_String(), (*iter)->GetName().GetLength());
		sq_arrayappend(pVM, -2);
	}
	return 1;
}

// getClientScripts()
SQInteger sq_server_getclientscripts(SQVM * pVM)
{
	sq_newarray(pVM, 0);
	for(CClientFileManager::iterator iter = g_pClientScriptFileManager->begin(); iter != g_pClientScriptFileManager->end(); ++ iter)
	{
		sq_pushstring(pVM, (*iter).first.C_String(), (*iter).first.GetLength());
		sq_arrayappend(pVM, -2);
	}
	return 1;
}

// getClientResources()
SQInteger sq_server_getclientresources(SQVM * pVM)
{
	sq_newarray(pVM, 0);
	for(CClientFileManager::iterator iter = g_pClientResourceFileManager->begin(); iter != g_pClientResourceFileManager->end(); ++ iter)
	{
		sq_pushstring(pVM, (*iter).first.C_String(), (*iter).first.GetLength());
		sq_arrayappend(pVM, -2);
	}
	return 1;
}

// getScriptName()
SQInteger sq_server_getscriptname(SQVM * pVM)
{
	String strScriptName = g_pScriptingManager->Get(pVM)->GetName();
	sq_pushstring(pVM, strScriptName.Get(), strScriptName.GetLength());
	return 1;
}

// loadScript(script)
SQInteger sq_server_loadscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	if(!g_pScriptingManager->Get(szScript))
	{
		String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
		CSquirrel * pScript = g_pScriptingManager->Load(szScript, strPath);

		if(!pScript)
			CLogFile::Printf("Failed to load script %s (Script does not exist/Script compilation failed).", szScript);
		else
			CLogFile::Printf("Loaded script %s.", szScript);

		sq_pushbool(pVM, (pScript != NULL));
	}
	else
		sq_pushbool(pVM, false);
	return 1;
}

// unloadScript(script)
SQInteger sq_server_unloadscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
	CSquirrel* pScript = g_pScriptingManager->Get(szScript);
	if(pScript && pScript->GetVM() != pVM)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);
		sq_pushbool(pVM, g_pScriptingManager->Unload(szScript));
		CLogFile::Printf("Unloaded script %s.", szScript);
	}
	else
		sq_pushbool(pVM, false);
	return 1;
}

// reloadScript(script)
SQInteger sq_server_reloadscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
	CSquirrel* pScript = g_pScriptingManager->Get(szScript);

	if(pScript && pScript->GetVM() != pVM)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);
		String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
		sq_pushbool(pVM, g_pScriptingManager->Unload(szScript) && g_pScriptingManager->Load(szScript, strPath));
		CLogFile::Printf("Reloaded script %s.", szScript);
	}
	else
		sq_pushbool(pVM, false);

	return 1;
}

// loadClientScript(script)
SQInteger sq_server_loadclientscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	if(!g_pClientScriptFileManager->Exists(szScript))
	{
		sq_pushbool(pVM, g_pClientScriptFileManager->Start(szScript));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// unloadClientScript(script)
SQInteger sq_server_unloadclientscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	if(g_pClientScriptFileManager->Exists(szScript))
	{
		sq_pushbool(pVM, g_pClientScriptFileManager->Stop(szScript));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// reloadClientScript(script)
SQInteger sq_server_reloadclientscript(SQVM * pVM)
{
	const char* szScript;
	sq_getstring(pVM, 2, &szScript);

	if(g_pClientScriptFileManager->Exists(szScript))
	{
		sq_pushbool(pVM, g_pClientScriptFileManager->Restart(szScript));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// loadClientResource(resource)
SQInteger sq_server_loadclientresource(SQVM * pVM)
{
	const char* szResource;
	sq_getstring(pVM, 2, &szResource);

	if(!g_pClientResourceFileManager->Exists(szResource))
	{
		sq_pushbool(pVM, g_pClientResourceFileManager->Start(szResource));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// unloadClientResource(resource)
SQInteger sq_server_unloadclientresource(SQVM * pVM)
{
	const char* szResource;
	sq_getstring(pVM, 2, &szResource);

	if(g_pClientResourceFileManager->Exists(szResource))
	{
		sq_pushbool(pVM, g_pClientResourceFileManager->Stop(szResource));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// reloadClientResource(resource)
SQInteger sq_server_reloadclientresource(SQVM * pVM)
{
	const char* szResource;
	sq_getstring(pVM, 2, &szResource);

	if(g_pClientResourceFileManager->Exists(szResource))
	{
		sq_pushbool(pVM, g_pClientResourceFileManager->Restart(szResource));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
*/