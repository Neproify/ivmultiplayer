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
	int iModelId;
	CVector3 vecPosition;
	float fHeading;
};

class CActorManager : public CActorManagerInterface
{
private:
	bool m_bActive[MAX_ACTORS];
	_Actor m_Actors[MAX_ACTORS];

public:
	CActorManager();
	~CActorManager();

	EntityId Create(int iModelId, CVector3 vecPosition, float fHeading);
	void    Delete(EntityId actorId);
	void    SetPosition(EntityId actorId, CVector3 vecPosition);
	void    SetHeading(EntityId actorId, float fHeading);
	void    HandleClientJoin(EntityId playerId);
	bool    DoesExist(EntityId actorId);
	EntityId GetActorCount();
};
