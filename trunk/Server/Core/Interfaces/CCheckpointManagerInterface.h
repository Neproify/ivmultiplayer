//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpointManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CCheckpointManagerInterface
{
public:
	virtual EntityId Add(WORD wType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius) = 0;
	virtual bool     Delete(EntityId checkpointId) = 0;
	virtual bool     DoesExist(EntityId checkpointId) = 0;
	virtual EntityId GetCheckpointCount() = 0;
};
