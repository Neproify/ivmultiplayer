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
	virtual EntityId	Create(int iModelId, CVector3 vecPosition, float fHeading) = 0;
	virtual void		Delete(EntityId actorId) = 0;
	virtual void		SetPosition(EntityId actorId, CVector3 vecPosition) = 0;
	virtual CVector3	GetPosition(EntityId actorId) = 0;
	virtual void		SetHeading(EntityId actorId, float fHeading) = 0;
	virtual float		GetHeading(EntityId actorId) = 0;
	virtual int			GetModel(EntityId actorId) = 0;
	virtual void		SetActorName(EntityId actorId, String strName) = 0;
	virtual String		GetActorName(EntityId actorId) = 0;
	virtual void		SetColor(EntityId actorId, unsigned int iCcolor) = 0;
	virtual unsigned int GetColor(EntityId actorId) = 0;
	virtual bool		ToggleNametag(EntityId actorId, bool bShow) = 0;
	virtual bool		ToggleBlip(EntityId actorId, bool bShow) = 0;
	virtual bool		ToggleFrozen(EntityId actorId, bool bFrozen) = 0;
	virtual bool		ToggleHelmet(EntityId actorId, bool bHelmet) = 0;
	virtual void		WarpIntoVehicle(EntityId actorId, EntityId vehicleid, int iSeatid) = 0;
	virtual void		RemoveFromVehicle(EntityId actorId) = 0;
	virtual bool		DoesExist(EntityId actorId) = 0;
	virtual EntityId	GetActorCount() = 0;
};
