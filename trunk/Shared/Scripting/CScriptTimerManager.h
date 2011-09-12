//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptTimerManager.h
// Project: Server.Core
// Author(s): MaVe
//            jenksta
//            Einstein
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CScriptingManager.h"
#include "CScriptTimer.h"
#include <list>

class CScriptTimerManager : public std::list<CScriptTimer *>
{
public:
	void Pulse();
	void HandleScriptUnload(CSquirrel * pScript);
	bool contains(CScriptTimer* pTimer);
};
