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

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CBlipManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CBlipManager * g_pBlipManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Blip functions

void CBlipNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createBlip", Create, 5, "ifffb");
	pScriptingManager->RegisterFunction("deleteBlip", Delete, 1, "i");
	pScriptingManager->RegisterFunction("setBlipCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getBlipCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setBlipPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getBlipPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setBlipColor", SetColor, -1, "ii");
	pScriptingManager->RegisterFunction("getBlipColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("setBlipSize", SetSize, -1, "ii");
	pScriptingManager->RegisterFunction("getBlipSize", GetSize, 1, "i");
	pScriptingManager->RegisterFunction("toggleBlipShortRange", ToggleShortRange, -1, "ib");
	pScriptingManager->RegisterFunction("toggleBlipRoute", ToggleRoute, -1, "ib"); // TODO, fix native clientside
	pScriptingManager->RegisterFunction("setBlipName", SetName, -1, "is");
	pScriptingManager->RegisterFunction("getBlipName", GetName, -1, "is");
	pScriptingManager->RegisterFunction("switchBlipIcon", SwitchIcon, 2, "ib");
	pScriptingManager->RegisterFunction("switchBlipIconForPlayer", SwitchIconPlayer, 3, "iib");

	pScriptingManager->RegisterFunction("createPlayerBlip", CreatePlayerBlip, 2, "ii");
	pScriptingManager->RegisterFunction("deletePlayerBlip", DeletePlayerBlip, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerBlipShortRange", TogglePlayerShortRange, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerBlipDisplay", TogglePlayerBlipDisplay, 2, "ib");
}

// createBlip(spriteID, x, y, z)
SQInteger CBlipNatives::Create(SQVM * pVM)
{
	SQInteger iSprite;
	CVector3 vecPosition;
	SQBool bShow;
	sq_getinteger(pVM, -5, &iSprite);
	sq_getvector3(pVM, -4, &vecPosition);
	sq_getbool(pVM, -1, &bShow);
	bool bToggle = (bShow != 0);
	sq_pushentity(pVM, g_pBlipManager->Create(iSprite, vecPosition, bToggle));
	return 1;
}

// deleteBlip(blipid)
SQInteger CBlipNatives::Delete(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, -1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		g_pBlipManager->Delete(blipId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setBlipCoordinates(blipid, x, y, z)
SQInteger CBlipNatives::SetCoordinates(SQVM * pVM)
{
	EntityId blipId;
	CVector3 vecPosition;
	sq_getentity(pVM, -4, &blipId);
	sq_getvector3(pVM, -3, &vecPosition);

	if(g_pBlipManager->DoesExist(blipId))
	{
		g_pBlipManager->SetPosition(blipId, vecPosition);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getBlipCoordinates(blipid)
SQInteger CBlipNatives::GetCoordinates(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, 2, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		CVector3 vecPosition = g_pBlipManager->GetPosition(blipId);
		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, vecPosition.fX);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, vecPosition.fY);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, vecPosition.fZ);
		sq_arrayappend(pVM, -2);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setBlipColor(blipid, color[, playerID = INVALID_ENTITY_ID])
SQInteger CBlipNatives::SetColor(SQVM * pVM)
{
	CHECK_PARAMS_MIN_MAX("setBlipColor", 2, 3);
	CHECK_TYPE("setBlipColor", 1, 2, OT_INTEGER);
	CHECK_TYPE("setBlipColor", 2, 3, OT_INTEGER);

	EntityId blipId;
	SQInteger iColor;
	EntityId playerId = INVALID_ENTITY_ID;

	sq_getentity(pVM, 2, &blipId);
	sq_getinteger(pVM, 3, &iColor);

	if(sq_gettop(pVM) >= 4 && sq_gettype(pVM,4) == OT_INTEGER)
		sq_getentity(pVM, 4, &playerId);
	
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
				sq_pushbool(pVM, true);
				return 1;
			}
		}
		else
		{
			g_pBlipManager->SetColor(blipId, iColor);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getBlipColor(blipid)
SQInteger CBlipNatives::GetColor(SQVM * pVM)
{
	EntityId blipId;
	//SQInteger blipId;
	sq_getentity(pVM, 2, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		sq_pushinteger(pVM, g_pBlipManager->GetColor(blipId));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setBlipSize(blipid, size[, playerID = -1 ])
SQInteger CBlipNatives::SetSize(SQVM * pVM)
{
	CHECK_PARAMS_MIN_MAX("setBlipSize", 2, 3);
	CHECK_TYPE("setBlipSize", 1, 2, OT_INTEGER);
	CHECK_TYPE("setBlipSize", 2, 3, OT_FLOAT);

	EntityId blipId;
	float fSize;
	EntityId playerId = INVALID_ENTITY_ID;

	sq_getentity(pVM, 2, &blipId);
	sq_getfloat(pVM, 3, &fSize);

	if(sq_gettop(pVM) >= 4 && sq_gettype(pVM,4) == OT_INTEGER)
		sq_getentity(pVM, 4, &playerId);
	
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
				sq_pushbool(pVM, true);
				return 1;
			}
		}
		else
		{
			g_pBlipManager->SetSize(blipId, fSize);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getBlipSize(blipid)
SQInteger CBlipNatives::GetSize(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, 2, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		sq_pushfloat(pVM, g_pBlipManager->GetSize(blipId));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// toggleBlipShortRange(blipid, toggle [, playerid = -1])
SQInteger CBlipNatives::ToggleShortRange(SQVM * pVM)
{
	CHECK_PARAMS_MIN_MAX("toggleBlipShortRange", 2, 3);
	CHECK_TYPE("toggleBlipShortRange", 1, 2, OT_INTEGER);
	CHECK_TYPE("toggleBlipShortRange", 2, 3, OT_BOOL);

	EntityId blipId;
	SQBool bToggle;
	EntityId playerId = INVALID_ENTITY_ID;

	sq_getentity(pVM, 2, &blipId);
	sq_getbool(pVM, 3, &bToggle);

	if(sq_gettop(pVM) >= 4 && sq_gettype(pVM,4) == OT_INTEGER)
		sq_getentity(pVM, 4, &playerId);

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleBlipShortRange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->ToggleShortRange(blipId, (bToggle != 0));
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// toggleBlipRoute(blipid, toggle [, playerid = -1])
SQInteger CBlipNatives::ToggleRoute(SQVM * pVM)
{
	CHECK_PARAMS_MIN_MAX("toggleBlipRoute", 2, 3);
	CHECK_TYPE("toggleBlipRoute", 1, 2, OT_INTEGER);
	CHECK_TYPE("toggleBlipRoute", 2, 3, OT_BOOL);

	EntityId blipId;
	SQBool bToggle;
	EntityId playerId = INVALID_ENTITY_ID;

	sq_getentity(pVM, 2, &blipId);
	sq_getbool(pVM, 3, &bToggle);

	if(sq_gettop(pVM) >= 4 && sq_gettype(pVM,4) == OT_INTEGER)
		sq_getentity(pVM, 4, &playerId);

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleBlipRoute, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->ToggleRoute(blipId, (bToggle != 0));
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setBlipName(blipid, name[, playerid = -1])
SQInteger CBlipNatives::SetName(SQVM * pVM)
{
	CHECK_PARAMS_MIN_MAX("setBlipName", 2, 3);
	CHECK_TYPE("setBlipName", 1, 2, OT_INTEGER);
	CHECK_TYPE("setBlipName", 2, 3, OT_STRING);

	EntityId blipId;
	const char * szName;
	EntityId playerId = INVALID_ENTITY_ID;

	sq_getentity(pVM, 2, &blipId);
	sq_getstring(pVM, 3, &szName);

	if(sq_gettop(pVM) >= 4 && sq_gettype(pVM,4) == OT_INTEGER)
		sq_getentity(pVM, 4, &playerId);

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write(String(szName));
			g_pNetworkManager->RPC(RPC_ScriptingSetBlipName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->SetName(blipId, szName);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getBlipName(blipid)
SQInteger CBlipNatives::GetName(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, 2, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		sq_pushstring(pVM, g_pBlipManager->GetName(blipId).C_String(), -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// switchBlipIcon(blipId,switch)
SQInteger CBlipNatives::SwitchIcon(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, -2, &blipId);
	
	SQBool bShow;
	sq_getbool(pVM,-1,&bShow);

	if(g_pBlipManager->DoesExist(blipId))
	{
		bool bToggle = (bShow != 0);
		g_pBlipManager->SwitchIcon(blipId,bToggle,INVALID_ENTITY_ID);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// switchBlipIcon(blipId, playerid, switch)
SQInteger CBlipNatives::SwitchIconPlayer(SQVM * pVM)
{
	EntityId blipId;
	sq_getentity(pVM, -3, &blipId);

	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);
	
	SQBool bShow;
	sq_getbool(pVM,-1,&bShow);

	if(g_pBlipManager->DoesExist(blipId) && g_pPlayerManager->DoesExist(playerId))
	{
		bool bToggle = (bShow != 0);
		g_pBlipManager->SwitchIcon(blipId,bToggle,playerId);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CBlipNatives::CreatePlayerBlip(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	int iSprite;
	sq_getinteger(pVM, -1, &iSprite);

	if(g_pPlayerManager->DoesExist(playerId) && !g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->CreateForPlayer(playerId, iSprite, true);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CBlipNatives::DeletePlayerBlip(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->DeleteForPlayer(playerId);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CBlipNatives::TogglePlayerBlipDisplay(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQBool bToggle;
	sq_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->ToggleDisplayForPlayer(playerId, bShow);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CBlipNatives::TogglePlayerShortRange(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQBool bToggle;
	sq_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->ToggleShortRangeForPlayer(playerId, bShow);
		sq_pushbool(pVM,true);
		return 1;
	}

	sq_pushbool(pVM,false);
	return 1;
}