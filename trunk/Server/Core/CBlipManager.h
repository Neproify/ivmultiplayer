//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManager.h
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

struct _Blip
{
	CVector3		vecSpawnPos;
	int				iSprite;
	unsigned int	uiColor;
	float			fSize;
	bool			bShortRange;
	bool			bRouteBlip;
	bool			bShow;
	String			strName;
};

struct _PlayerBlip
{
	EntityId	playerId;
	int			iSprite;
	bool		bShortRange;
	bool		bShow;
};

class CBlipManager : public CBlipManagerInterface
{
private:
	bool m_bActive[MAX_BLIPS];
	_Blip m_Blips[MAX_BLIPS];

	bool m_bPlayerActive[MAX_PLAYERS];
	_PlayerBlip m_PlayerBlips[MAX_PLAYERS];

public:
	CBlipManager();
	~CBlipManager();

	EntityId     Create(int iSprite, CVector3 vecPosition, bool bShow);
	void         Delete(EntityId blipId);
	void         SetPosition(EntityId blipId, CVector3 vecPosition);
	CVector3      GetPosition(EntityId blipId);
	void         SetColor(EntityId blipId, unsigned int color);
	unsigned int GetColor(EntityId blipId);
	void         SetSize(EntityId blipId, float size);
	float        GetSize(EntityId blipId);
	void		 ToggleShortRange(EntityId blipId, bool bShortRange);
	void		 ToggleRoute(EntityId blipId, bool bRoute);
	void         SetName(EntityId blipId, String strName);
	String       GetName(EntityId blipId);
	void         HandleClientJoin(EntityId playerId);
	bool         DoesExist(EntityId blipId);
	EntityId     GetBlipCount();
	void		 SwitchIcon(EntityId blipId, bool bShow, EntityId playerId);

	void		 CreateForPlayer(EntityId playerId, int iSprite, bool bShow);
	void		 DeleteForPlayer(EntityId playerId);
	void	     ToggleShortRangeForPlayer(EntityId playerId, bool bToggle);
	void		 SetSpriteForPlayer(EntityId playerId, int iSprite);
	void		 ToggleDisplayForPlayer(EntityId playerId, bool bToggle);
	bool		 DoesPlayerBlipExist(EntityId playerId) { return m_bPlayerActive[playerId]; }
};
