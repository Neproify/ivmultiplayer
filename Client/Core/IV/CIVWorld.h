//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
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