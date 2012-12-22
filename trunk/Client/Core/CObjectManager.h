//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.h
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetworkEntityManager.h"
#include "CObject.h"
#include <SharedUtility.h>

class CObjectManager : public CNetworkEntityManager<CObject, MAX_OBJECTS>
{
public:
	void Process();
};
