//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManager.h
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"
#include "CPlayerManager.h"

struct _Actor
{
	unsigned int   uiActorIndex;
	CIVModelInfo * pModelInfo;
	CVector3	   vecPosition;
};

class CActorManager
{
private:
	bool m_bActive[MAX_ACTORS];
	_Actor m_Actors[MAX_ACTORS];

public:
	CActorManager();
	~CActorManager();

	void Create(EntityId actorId, int iModelId, CVector3 vecPosition, float fHeading);
	bool Delete(EntityId actorId);
	void SetPosition(EntityId actorId, CVector3 vecPosition);
	void SetHeading(EntityId actorId, float fHeading);
	bool DoesExist(EntityId actorId) { return m_bActive[actorId]; };
	unsigned int GetScriptingHandle(EntityId actorId);
};
