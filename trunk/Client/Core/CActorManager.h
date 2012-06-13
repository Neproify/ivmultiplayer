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
	unsigned int    uiActorIndex;
	CIVModelInfo *  pModelInfo;
	String		    name;
	CVector3	    vecPosition;	
	bool		    nametag;
	int			    nametagColor;
	bool			frozen;			
	bool			helmet;
	bool			stateincar;
	int				vehicleid;
	int				seatid;
	unsigned int	uiBlipId;
 };

class CActorManager
{
private:
	bool m_bActive[MAX_ACTORS];
	_Actor m_Actors[MAX_ACTORS];

public:
	CActorManager();
	~CActorManager();

	void			Create(EntityId actorId, int iModelId, CVector3 vecPosition, float fHeading, String name, bool togglename, int color, bool frozen, bool helmet);
	bool			Delete(EntityId actorId);
	void			SetPosition(EntityId actorId, CVector3 vecPosition);
	CVector3		GetPosition(EntityId actorId);
	void			SetHeading(EntityId actorId, float fHeading);
	String			GetName(EntityId actorId) { return m_Actors[actorId].name; }
	bool			IsNameTagEnabled(EntityId actorId) { return m_Actors[actorId].nametag; }
	void			SetName(EntityId actorId, String name);
	int				GetNametagColor(EntityId actorId) { return m_Actors[actorId].nametagColor; }
	bool			ToggleNametag(EntityId actorId, bool show);
	void			SetColor(EntityId actorId, int color);
	bool			ToggleFrozen(EntityId actorId, bool frozen);
	bool			ToggleHelmet(EntityId actorId, bool helmet);
	void			WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid);
	void			RemoveFromVehicle(EntityId actorId);
	bool			IsCharOnScreen(EntityId actorId);
	bool			DoesExist(EntityId actorId) { return m_bActive[actorId]; };
	unsigned int	GetScriptingHandle(EntityId actorId);
	void			ForceAnimation(EntityId actorId, const char * szGroup, const char * szAnim);
	float	        GetHealth(EntityId actorId);
	float			GetArmour(EntityId actorId);
};
