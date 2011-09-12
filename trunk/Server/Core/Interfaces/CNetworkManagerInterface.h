//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CNetworkManagerInterface
{
public:
	virtual bool AddBan(String strIp, unsigned int uiSeconds) = 0;
};
