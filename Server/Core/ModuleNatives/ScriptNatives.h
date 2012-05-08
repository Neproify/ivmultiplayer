//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CScriptModuleNatives : public ICScriptModuleNatives
	{
	public:
		std::list<String> * GetScripts();
		std::list<String> * GetClientScripts();
		std::list<String> * GetClientResources();
		const char * GetScriptName(void * pVM);
		bool LoadScript(const char * szScript);
		bool UnloadScript(const char * szScript);
		bool ReloadScript(const char * szScript);
		bool LoadClientScript(const char * szScript);
		bool UnloadClientScript(const char * szScript);
		bool ReloadClientScript(const char * szScript);
		bool LoadClientResource(const char * szResource);
		bool UnloadClientResource(const char * szResource);
		bool ReloadClientResource(const char * szResource);
	};
}