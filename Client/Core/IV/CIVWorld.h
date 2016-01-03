//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CIVWorld.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVEntity.h"

class CIVWorld
{
public:
	void AddEntity(CIVEntity * pEntity);
	void RemoveEntity(CIVEntity * pEntity);
};