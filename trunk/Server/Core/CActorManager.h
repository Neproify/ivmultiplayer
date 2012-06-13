//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManager.h
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

struct _Actor
{
	int			iModelId;
	CVector3	vecPosition;
	float		fHeading;
	String		name;		
	bool		togglename;
	int			color;
	bool		frozen;
	bool		helmet;
	bool		stateincar;
};

class CActorManager : public CActorManagerInterface
{
private:
	bool m_bActive[MAX_ACTORS];
	_Actor m_Actors[MAX_ACTORS];

public:
	CActorManager();
	~CActorManager();

	EntityId	Create(int iModelId, CVector3 vecPosition, float fHeading);
	void		Delete(EntityId actorId);
	void		SetPosition(EntityId actorId, CVector3 vecPosition);
	CVector3	GetPosition(EntityId actorId);
	void		SetHeading(EntityId actorId, float fHeading);
	float		GetHeading(EntityId actorId) { return m_Actors[actorId].fHeading; }
	int			GetModel(EntityId actorId) { return m_Actors[actorId].iModelId; }
	void		HandleClientJoin(EntityId playerId);
	void		SetActorName(EntityId actorId, String name);
	String		GetActorName(EntityId actorId);
	void		SetColor(EntityId actorId, int color);
	int			GetColor(EntityId actorId) { return m_Actors[actorId].color; }
	bool		ToggleNametag(EntityId actorId, bool show);
	bool		ToggleFrozen(EntityId actorId, bool frozen);
	bool		ToggleHelmet(EntityId actorId, bool helmet);
	void		WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid);
	void		RemoveFromVehicle(EntityId actorId);
	bool		DoesExist(EntityId actorId);
	EntityId	GetActorCount();
};
