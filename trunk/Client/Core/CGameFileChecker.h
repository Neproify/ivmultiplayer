//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGameFileChecker.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

class CGameFileChecker
{
public:
	static bool CheckFiles();

	static unsigned int CheckGameFile(int iFile);
	static bool IsGameFileChanged(int iFile);
};
