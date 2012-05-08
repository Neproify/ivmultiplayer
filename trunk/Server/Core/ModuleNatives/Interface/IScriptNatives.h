//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICScriptModuleNatives
	{
	public:
		virtual std::list<String> * GetScripts() = 0;
		virtual std::list<String> * GetClientScripts() = 0;
		virtual std::list<String> * GetClientResources() = 0;
		virtual const char * GetScriptName(void * pVM) = 0;
		virtual bool LoadScript(const char * szScript) = 0;
		virtual bool UnloadScript(const char * szScript) = 0;
		virtual bool ReloadScript(const char * szScript) = 0;
		virtual bool LoadClientScript(const char * szScript) = 0;
		virtual bool UnloadClientScript(const char * szScript) = 0;
		virtual bool ReloadClientScript(const char * szScript) = 0;
		virtual bool LoadClientResource(const char * szResource) = 0;
		virtual bool UnloadClientResource(const char * szResource) = 0;
		virtual bool ReloadClientResource(const char * szResource) = 0;
	};
}