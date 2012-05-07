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

#include "ModuleNatives.h"

namespace Modules
{
	class CBlipModuleNatives
	{
	public:
		EntityId Create(int iSprite, CVector3 vecPosition);
		bool Delete(EntityId blipId);
		bool SetCoordinates(EntityId blipId, CVector3 vecPosition);
		CVector3 GetCoordinates(EntityId blipId);
		bool SetColor(EntityId blipId, int iColor, EntityId playerId);
		int GetColor(EntityId blipId);
		bool SetSize(EntityId blipId, float fSize, EntityId playerId);
		float GetSize(EntityId blipId);
		bool ToggleShortRange(EntityId blipId, bool bToggle, EntityId playerId);
		bool ToggleRoute(EntityId blipId, bool bToggle, EntityId playerId);
		bool SetName(EntityId blipId, const char * szName, EntityId playerId);
		const char * GetName(EntityId blipId);
	};
}