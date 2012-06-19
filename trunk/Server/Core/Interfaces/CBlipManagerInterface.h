//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CBlipManagerInterface
{
public:
	virtual EntityId     Create(int iSprite, CVector3 vecPosition, bool bShow) = 0;
	virtual void         Delete(EntityId blipId) = 0;
	virtual void         SetPosition(EntityId blipId, CVector3 vecPosition) = 0;
	virtual CVector3     GetPosition(EntityId blipId) = 0;
	virtual void         SetColor(EntityId blipId, unsigned int color) = 0;
	virtual unsigned int GetColor(EntityId blipId) = 0;
	virtual void         SetSize(EntityId blipId, float size) = 0;
	virtual float        GetSize(EntityId blipId) = 0;
	virtual void	     ToggleShortRange(EntityId blipId, bool bShortRange) = 0;
	virtual void	     ToggleRoute(EntityId blipId, bool bRoute) = 0;
	virtual bool         DoesExist(EntityId blipId) = 0;
	virtual EntityId     GetBlipCount() = 0;
	virtual void		 SwitchIcon(EntityId blipId, bool bShow, EntityId playerId) = 0;
};
