//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
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