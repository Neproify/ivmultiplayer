//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CActorManagerInterface
{
public:
	virtual EntityId Create(int iModelId, CVector3 vecPosition, float fHeading) = 0;
	virtual void    Delete(EntityId actorId) = 0;
	virtual void    SetPosition(EntityId actorId, CVector3 vecPosition) = 0;
	virtual void    SetHeading(EntityId actorId, float fHeading) = 0;
	virtual bool    DoesExist(EntityId actorId) = 0;
	virtual EntityId GetActorCount() = 0;
};