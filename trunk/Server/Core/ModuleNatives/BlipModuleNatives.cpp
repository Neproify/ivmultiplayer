//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: BlipNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "../CBlipManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CBlipManager * g_pBlipManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Blip functions
namespace Modules
{
	// createBlip(spriteID, x, y, z)
	EntityId CBlipModuleNatives::Create(int iSprite, CVector3 vecPosition, bool bShow)
	{
		return g_pBlipManager->Create(iSprite, vecPosition, bShow);
	}

	// deleteBlip(blipid)
	bool CBlipModuleNatives::Delete(EntityId blipId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			g_pBlipManager->Delete(blipId);
			return true;
		}

		return false;
	}

	// setBlipCoordinates(blipid, x, y, z)
	bool CBlipModuleNatives::SetCoordinates(EntityId blipId, CVector3 vecPosition)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			g_pBlipManager->SetPosition(blipId, vecPosition);
			return true;
		}
		return false;
	}

	// getBlipCoordinates(blipid)
	CVector3 CBlipModuleNatives::GetCoordinates(EntityId blipId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			return g_pBlipManager->GetPosition(blipId);
		}

		return CVector3();
	}

	// setBlipColor(blipid, color[, playerID = INVALID_ENTITY_ID])
	bool CBlipModuleNatives::SetColor(EntityId blipId, int iColor, EntityId playerId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			if(playerId != INVALID_ENTITY_ID)
			{
				if(g_pPlayerManager->DoesExist(playerId))
				{
					CBitStream bsSend;
					bsSend.Write(blipId);
					bsSend.Write(iColor);
					g_pNetworkManager->RPC(RPC_ScriptingSetBlipColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
					return true;
				}
			}
			else
			{
				g_pBlipManager->SetColor(blipId, iColor);
				return true;
			}
		}

		return false;
	}

	// getBlipColor(blipid)
	int CBlipModuleNatives::GetColor(EntityId blipId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			return g_pBlipManager->GetColor(blipId);
		}

		return -1;
	}

	// setBlipSize(blipid, size[, playerID = -1 ])
	bool CBlipModuleNatives::SetSize(EntityId blipId, float fSize, EntityId playerId)
	{
		if(fSize > 0.0f && fSize <= 4.0f && g_pBlipManager->DoesExist(blipId))
		{
			if(playerId != INVALID_ENTITY_ID)
			{
				if(g_pPlayerManager->DoesExist(playerId))
				{
					CBitStream bsSend;
					bsSend.Write(blipId);
					bsSend.Write(fSize);
					g_pNetworkManager->RPC(RPC_ScriptingSetBlipSize, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
					return true;
				}
			}
			else
			{
				g_pBlipManager->SetSize(blipId, fSize);
				return true;
			}
		}

		return false;
	}

	// getBlipSize(blipid)
	float CBlipModuleNatives::GetSize(EntityId blipId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			return g_pBlipManager->GetSize(blipId);
		}

		return -1.0f;
	}

	// toggleBlipShortRange(blipid, toggle [, playerid = -1])
	bool CBlipModuleNatives::ToggleShortRange(EntityId blipId, bool bToggle, EntityId playerId)
	{
		if(playerId != INVALID_ENTITY_ID)
		{
			if(g_pPlayerManager->DoesExist(playerId))
			{
				CBitStream bsSend;
				bsSend.Write(blipId);
				bsSend.Write(bToggle);
				g_pNetworkManager->RPC(RPC_ScriptingToggleBlipShortRange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				return true;
			}
		}
		else
		{
			g_pBlipManager->ToggleShortRange(blipId, (bToggle != 0));
			return true;
		}

		return false;
	}

	// toggleBlipRoute(blipid, toggle [, playerid = -1])
	bool CBlipModuleNatives::ToggleRoute(EntityId blipId, bool bToggle, EntityId playerId)
	{
		if(playerId != INVALID_ENTITY_ID)
		{
			if(g_pPlayerManager->DoesExist(playerId))
			{
				CBitStream bsSend;
				bsSend.Write(blipId);
				bsSend.Write(bToggle);
				g_pNetworkManager->RPC(RPC_ScriptingToggleBlipRoute, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				return true;
			}
		}
		else
		{
			g_pBlipManager->ToggleRoute(blipId, (bToggle != 0));
			return true;
		}

		return false;
	}

	// setBlipName(blipid, name[, playerid = -1])
	bool CBlipModuleNatives::SetName(EntityId blipId, const char * szName, EntityId playerId)
	{
		if(playerId != INVALID_ENTITY_ID)
		{
			if(g_pPlayerManager->DoesExist(playerId))
			{
				CBitStream bsSend;
				bsSend.Write(blipId);
				bsSend.Write(String(szName));
				g_pNetworkManager->RPC(RPC_ScriptingSetBlipName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				return true;
			}
		}
		else
		{
			g_pBlipManager->SetName(blipId, szName);
			return true;
		}

		return false;
	}

	// getBlipName(blipid)
	const char * CBlipModuleNatives::GetName(EntityId blipId)
	{
		if(g_pBlipManager->DoesExist(blipId))
		{
			return g_pBlipManager->GetName(blipId).C_String();
		}

		return NULL;
	}

	bool CBlipModuleNatives::CreatePlayerBlip(EntityId playerId, int iSprite)
	{

		if(g_pPlayerManager->DoesExist(playerId) && !g_pBlipManager->DoesPlayerBlipExist(playerId))
		{
			g_pBlipManager->CreateForPlayer(playerId, iSprite, true);
			return true;
		}

		return false;
	}

	bool CBlipModuleNatives::DeletePlayerBlip(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
		{
			g_pBlipManager->DeleteForPlayer(playerId);
			return true;
		}

		return false;
	}

	bool CBlipModuleNatives::TogglePlayerBlipDisplay(EntityId playerId, bool bToggle)
	{
		bool bShow = (bToggle != 0);
		if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
		{
			g_pBlipManager->ToggleDisplayForPlayer(playerId, bShow);
			return true;
		}

		return false;
	}

	bool CBlipModuleNatives::TogglePlayerShortRange(EntityId playerId, bool bToggle)
	{
		bool bShow = (bToggle != 0);
		if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
		{
			g_pBlipManager->ToggleShortRangeForPlayer(playerId, bShow);
			return true;
		}

		return false;
	}
}