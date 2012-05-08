//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: HashNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICHashModuleNatives
	{
	public:
		virtual const char * md5(const char * toHash) = 0;
		virtual const char * md5File(const char * szFileName) = 0;
	};
}