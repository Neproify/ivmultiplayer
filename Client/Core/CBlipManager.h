//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManager.h
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"

struct _Blip
{
	unsigned int	uiBlipIndex;
	int				iSprite;
	CVector3		vecPosition;
	unsigned int	uiColor;
	float			fSize;
	bool			bShortRange;
	bool			bRouteBlip;
	bool			bShow;

	EntityId		attachedVehicle;
};

class CBlipManager
{
private:
	bool m_bActive[MAX_BLIPS];
	_Blip m_Blips[MAX_BLIPS];

public:
	CBlipManager();
	~CBlipManager();

	void Create(EntityId blipId, int iSprite, CVector3 vecPosition);
	bool Delete(EntityId blipId);
	void SetColor(EntityId blipId, unsigned int uiColor);
	void SetSize(EntityId blipId, float fSize);
	void Flash(EntityId blipId, bool bFlash, int iFlashType);
	void ToggleShortRange(EntityId blipId, bool bToggle);
	void ToggleRouteBlip(EntityId blipId, bool bToggle);
	void SetName(EntityId blipId, String strName);
	void AttachToVehicle(EntityId blipId, EntityId vehicleId);
	bool DoesExist(EntityId blipId) { return m_bActive[blipId]; };
	void Show(EntityId blipId, bool bShow);
};
