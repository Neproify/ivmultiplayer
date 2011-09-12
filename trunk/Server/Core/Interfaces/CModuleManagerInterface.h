//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModuleManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CModuleInterface.h"

class CModuleManagerInterface
{
public:
	virtual CModuleInterface * LoadModule(const char * szName) = 0;
};