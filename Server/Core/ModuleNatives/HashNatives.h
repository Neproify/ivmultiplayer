//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: HashNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CHashModuleNatives : public ICHashModuleNatives
	{
	public:
		const char * md5(const char * toHash);
		const char * md5File(const char * szFileName);
	};
}