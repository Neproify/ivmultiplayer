//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: BlipNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICBlipModuleNatives
	{
	public:
		virtual EntityId Create(int iSprite, CVector3 vecPosition, bool bShow) = 0;
		virtual bool Delete(EntityId blipId) = 0;
		virtual bool SetCoordinates(EntityId blipId, CVector3 vecPosition) = 0;
		virtual CVector3 GetCoordinates(EntityId blipId) = 0;
		virtual bool SetColor(EntityId blipId, int iColor, EntityId playerId) = 0;
		virtual int GetColor(EntityId blipId) = 0;
		virtual bool SetSize(EntityId blipId, float fSize, EntityId playerId) = 0;
		virtual float GetSize(EntityId blipId) = 0;
		virtual bool ToggleShortRange(EntityId blipId, bool bToggle, EntityId playerId) = 0;
		virtual bool ToggleRoute(EntityId blipId, bool bToggle, EntityId playerId) = 0;
		virtual bool SetName(EntityId blipId, const char * szName, EntityId playerId) = 0;
		virtual const char * GetName(EntityId blipId) = 0;
		virtual bool CreatePlayerBlip(EntityId playerId, int iSprite) = 0;
		virtual bool DeletePlayerBlip(EntityId playerId) = 0;
		virtual bool TogglePlayerBlipDisplay(EntityId playerId, bool bToggle) = 0;
		virtual bool TogglePlayerShortRange(EntityId playerId, bool bToggle) = 0;
	};
}