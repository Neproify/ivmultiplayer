//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SQLiteNatives.h
// Project: Shared
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

//class CSQLiteNatives
//{
	void RegisterSQLiteNatives(CScriptingManager * pScriptingManager);

	_MEMBER_FUNCTION_IMPL(db, constructor);
	_MEMBER_FUNCTION_IMPL(db, query);
	_MEMBER_FUNCTION_IMPL(db, close);
//};
