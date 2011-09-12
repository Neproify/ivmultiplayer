//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSquirrelInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once
#include "InterfaceCommon.h"

class CSquirrelInterface
{
public:
	virtual SQVM * GetVM() = 0;
};