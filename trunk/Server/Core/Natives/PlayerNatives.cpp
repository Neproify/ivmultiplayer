//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "Squirrel/sqstring.h"
#include "../CVehicleManager.h"
#include "../CPlayerManager.h"
#include "../CNetworkManager.h"
#include <Game/CTime.h>

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;

// Player functions

void RegisterPlayerNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isPlayerConnected", sq_player_isconnected, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerName", sq_player_getname, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerName", sq_player_setname, 2, "is");
	pScriptingManager->RegisterFunction("setPlayerHealth", sq_player_sethealth, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerHealth", sq_player_gethealth, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerArmour", sq_player_setarmour, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerArmour", sq_player_getarmour, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerCoordinates", sq_player_setcoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerCoordinates", sq_player_getcoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerPosition", sq_player_setcoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerPosition", sq_player_getcoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerTime", sq_player_settime, 3, "iii");
	pScriptingManager->RegisterFunction("setPlayerWeather", sq_player_setweather, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerGravity", sq_player_setgravity, 2, "if");
	pScriptingManager->RegisterFunction("sendPlayerMessage", sq_player_sendmessage, -1, NULL);
	pScriptingManager->RegisterFunction("sendMessageToAll", sq_player_sendmessagetoall, -1, NULL);
	pScriptingManager->RegisterFunction("isPlayerInAnyVehicle", sq_player_isinanyvehicle, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInVehicle", sq_player_isinvehicle, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerVehicleId", sq_player_getvehicleid, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSeatId", sq_player_getseatid, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerOnFoot", sq_player_isonfoot, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerPayAndSpray", sq_player_togglepayandspray, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAutoAim", sq_player_toggleautoaim, 2, "ib");
	//pScriptingManager->RegisterFunction("setPlayerDrunk", sq_player_setplayerdrunk, 2, "ii");
	pScriptingManager->RegisterFunction("givePlayerWeapon", sq_player_giveweapon, 3, "iii");
	pScriptingManager->RegisterFunction("removePlayerWeapons", sq_player_removeweapons, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerSpawnLocation", sq_player_setspawnlocation, 5, "iffff");
	pScriptingManager->RegisterFunction("setPlayerModel", sq_player_setmodel, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerModel", sq_player_getmodel, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerControls", sq_player_togglecontrols, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerSpawned", sq_player_isspawned, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerHeading", sq_player_setheading, 2, "if");
	pScriptingManager->RegisterFunction("getPlayerHeading", sq_player_getheading, 1, "1");
	pScriptingManager->RegisterFunction("togglePlayerPhysics", sq_player_togglephysics, 2, "ib");
	pScriptingManager->RegisterFunction("kickPlayer", sq_player_kick, 2, "ib");
	pScriptingManager->RegisterFunction("banPlayer", sq_player_ban, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerIp", sq_player_getip, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerMoney", sq_player_givemoney, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerMoney", sq_player_setmoney, 2, "ii");
	pScriptingManager->RegisterFunction("resetPlayerMoney", sq_player_resetmoney, 1, "ii");
	pScriptingManager->RegisterFunction("getPlayerMoney", sq_player_getmoney, 1, "i");
	pScriptingManager->RegisterFunction("displayPlayerText", sq_player_displaytext, 5, "iffsi");
	pScriptingManager->RegisterFunction("displayTextForAll", sq_player_displaytextforall, 4, "ffsi");
	pScriptingManager->RegisterFunction("displayPlayerInfoText", sq_player_displayinfotext, 3, "isi");
	pScriptingManager->RegisterFunction("displayInfoTextForAll", sq_player_displayinfotextforall, 2, "si");
	pScriptingManager->RegisterFunction("togglePlayerFrozen", sq_player_togglefrozen, 2, "ib");
	pScriptingManager->RegisterFunction("getPlayerState", sq_player_getstate, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerVelocity", sq_player_setvelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerVelocity", sq_player_getvelocity, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerWantedLevel", sq_player_setwantedlevel, 2, "ii");
	pScriptingManager->RegisterFunction("warpPlayerIntoVehicle", sq_player_warpintovehicle, -1, NULL);
	pScriptingManager->RegisterFunction("removePlayerFromVehicle", sq_player_removefromvehicle, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerWeapon", sq_player_getweapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", sq_player_getammo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSerial", sq_player_getserial, 1, "i");
	pScriptingManager->RegisterFunction("setCameraBehindPlayer", sq_player_setcamerabehind, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerDucking", sq_player_setducking, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerHud", sq_player_togglehud, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerRadar", sq_player_toggleradar, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerNames", sq_player_togglenames, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAreaNames", sq_player_toggleareanames, 2, "ib");
	pScriptingManager->RegisterFunction("getEmptyPlayerPadState", sq_player_getemptypadstate, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", sq_player_getpreviouspadstate, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", sq_player_getpadstate, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerColor", sq_player_setcolor, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerColor", sq_player_getcolor, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", sq_player_getping, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerHelmet", sq_player_givehelmet, 1, "i");
	pScriptingManager->RegisterFunction("removePlayerHelmet", sq_player_removehelmet, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerClothes", sq_player_setclothes, 3, "iii");
	pScriptingManager->RegisterFunction("getPlayerClothes", sq_player_getclothes, 1, "i");
	pScriptingManager->RegisterFunction("resetPlayerClothes", sq_player_resetclothes, 1, "i");
	pScriptingManager->RegisterFunction("respawnPlayer", sq_player_respawn, 1, "i");

	pScriptingManager->RegisterFunction("triggerClientEvent", sq_player_trigger_event, -1, NULL);
}

// isPlayerConnected(playerid)
SQInteger sq_player_isconnected(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
	sq_pushbool(pVM, g_pPlayerManager->DoesExist(playerId));
	return 1;
}

// getPlayerName(playerid)
SQInteger sq_player_getname(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushstring(pVM, pPlayer->GetName(), -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_setname(SQVM * pVM)
{
	EntityId playerId;
	const char* szName;
	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szName);

	CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->SetName(szName));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// givePlayerWeapon(playerid, weaponid, ammo)
SQInteger sq_player_giveweapon(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iWeaponId;
	SQInteger iAmmo;
	sq_getentity(pVM, -3, &playerId);
	sq_getinteger(pVM, -2, &iWeaponId);
	sq_getinteger(pVM, -1, &iAmmo);

	if(g_pPlayerManager->DoesExist(playerId) && iWeaponId > 0 && iWeaponId < 21 && iWeaponId != 6)
	{
		CBitStream bsSend;
		bsSend.Write(iWeaponId);
		bsSend.Write(iAmmo);
		g_pNetworkManager->RPC(RPC_ScriptingGivePlayerWeapon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// removePlayerWeapons(playerid)
SQInteger sq_player_removeweapons(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pNetworkManager->RPC(RPC_ScriptingRemoveWeapons, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//setPlayerWantedLevel(playerid, wantedlevel)
SQInteger sq_player_setwantedlevel(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iWantedLevel;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iWantedLevel);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iWantedLevel);
		g_pNetworkManager->RPC(RPC_ScriptingSetWantedLevel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerHealth(playerid, health)
SQInteger sq_player_sethealth(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iHealth;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iHealth);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iHealth);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerHealth, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHealth(playerid)
SQInteger sq_player_gethealth(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, (pPlayer->GetHealth() - 100));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerArmour(playerid)
SQInteger sq_player_setarmour(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iArmour;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iArmour);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iArmour);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerArmour, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerArmour(playerid)
SQInteger sq_player_getarmour(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetArmour());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCoordinates(playerid, x, y, z)
SQInteger sq_player_setcoordinates(SQVM * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerCoordinates(playerid)
SQInteger sq_player_getcoordinates(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecPosition;
		pPlayer->GetPosition(&vecPosition);
		sq_newtable(pVM);
		sq_pushinteger(pVM, 0);
		sq_pushfloat(pVM, vecPosition.fX);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 1);
		sq_pushfloat(pVM, vecPosition.fY);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 2);
		sq_pushfloat(pVM, vecPosition.fZ);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVelocity(playerid)
SQInteger sq_player_getVelocity(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecMoveSpeed;
		pPlayer->GetMoveSpeed(&vecMoveSpeed);
		sq_newtable(pVM);
		sq_pushinteger(pVM, 0);
		sq_pushfloat(pVM, vecMoveSpeed.fX);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 1);
		sq_pushfloat(pVM, vecMoveSpeed.fY);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 2);
		sq_pushfloat(pVM, vecMoveSpeed.fZ);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTime(playerid, hour, minute)
SQInteger sq_player_settime(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iHour;
	SQInteger iMinute;
	sq_getinteger(pVM, -1, &iMinute);
	sq_getinteger(pVM, -2, &iHour);
	sq_getentity(pVM, -3, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && (iHour >= 0 && iHour < 24) && (iMinute >= 0 && iMinute < 60))
	{
		CBitStream bsSend;
		bsSend.Write((unsigned char)iHour);
		bsSend.Write((unsigned char)iMinute);
		if(g_pTime->GetMinuteDuration() != CTime::DEFAULT_MINUTE_DURATION)
			bsSend.Write(g_pTime->GetMinuteDuration());

		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerTime, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTime(playerid, weather)
SQInteger sq_player_setweather(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iWeather;
	sq_getinteger(pVM, -1, &iWeather);
	sq_getentity(pVM, -2, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && (iWeather >= 1 && iWeather < 10))
	{
		CBitStream bsSend;
		bsSend.Write((unsigned char)iWeather);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerGravity(playerid, gravity)
SQInteger sq_player_setgravity(SQVM * pVM)
{
	EntityId playerId;
	float fGravity;
	sq_getentity(pVM, -2, &playerId);
	sq_getfloat(pVM, -1, &fGravity);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fGravity);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerGravity, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessage(playerid, message [, color = 0xFFFFFFAA, allowformatting = true])
SQInteger sq_player_sendmessage(SQVM * pVM)
{
	SQInteger iTop = (sq_gettop(pVM) - 1);

	if(iTop < 2 || iTop > 4)
	{
		CHECK_PARAMS("sendPlayerMessage", 2);
	}

	EntityId playerId;
	SQInteger iColor = 0xFFFFFFFF;
	const char * szMessage = NULL;
	SQBool sqbAllowFormatting = false;

	if(iTop > 3)
	{
		CHECK_TYPE("sendPlayerMessage", 1, -4, OT_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -3, OT_STRING);
		CHECK_TYPE("sendPlayerMessage", 3, -2, OT_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 4, -1, OT_BOOL);
		sq_getentity(pVM, -4, &playerId);
		sq_getstring(pVM, -3, &szMessage);
		sq_getinteger(pVM, -2, &iColor);
		sq_getbool(pVM, -1, &sqbAllowFormatting);
	}
	else if(iTop > 2)
	{
		CHECK_TYPE("sendPlayerMessage", 1, -3, OT_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -2, OT_STRING);
		CHECK_TYPE("sendPlayerMessage", 3, -1, OT_INTEGER);
		sq_getentity(pVM, -3, &playerId);
		sq_getstring(pVM, -2, &szMessage);
		sq_getinteger(pVM, -1, &iColor);
	}
	else
	{
		CHECK_TYPE("sendPlayerMessage", 1, -2, OT_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -1, OT_STRING);
		sq_getentity(pVM, -2, &playerId);
		sq_getstring(pVM, -1, &szMessage);
	}

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((DWORD)iColor);
		bsSend.Write(String(szMessage));
		bool bAllowFormatting = (sqbAllowFormatting != 0);
		bsSend.Write(bAllowFormatting);
		g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendMessageToAll(message [, color = 0x999999, allowformatting = true])
SQInteger sq_player_sendmessagetoall(SQVM * pVM)
{
	SQInteger vtop = (sq_gettop(pVM) - 1);

	if(vtop > 3 || vtop < 1)
	{
		CHECK_PARAMS("sendMessageToAll", 1);
	}

	SQInteger iColor = 0xFFFFFFAA;
	const char * szMessage = NULL;
	SQBool sqbAllowFormatting = false;

	if(vtop > 2)
	{
		CHECK_TYPE("sendMessageToAll", 1, -3, OT_STRING);
		CHECK_TYPE("sendMessageToAll", 2, -2, OT_INTEGER);
		CHECK_TYPE("sendMessageToAll", 3, -1, OT_BOOL);
		sq_getstring(pVM, -3, &szMessage);
		sq_getinteger(pVM, -2, &iColor);
		sq_getbool(pVM, -1, &sqbAllowFormatting);
	}
	else if(vtop > 1)
	{
		CHECK_TYPE("sendMessageToAll", 1, -2, OT_STRING);
		CHECK_TYPE("sendMessageToAll", 2, -1, OT_INTEGER);
		sq_getstring(pVM, -2, &szMessage);
		sq_getinteger(pVM, -1, &iColor);
	}
	else
	{
		CHECK_TYPE("sendMessageToAll", 1, -1, OT_STRING);
		sq_getstring(pVM, -1, &szMessage);
	}

	if(g_pPlayerManager->GetPlayerCount() > 0)
	{
		CBitStream bsSend;
		bsSend.Write((DWORD)iColor);
		bsSend.Write(String(szMessage));
		bool bAllowFormatting = (sqbAllowFormatting != 0);
		bsSend.Write(bAllowFormatting);
		g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerInAnyVehicle(playerid)
SQInteger sq_player_isinanyvehicle(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsInVehicle());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerInVehicle(playerid, vehicleid)
SQInteger sq_player_isinvehicle(SQVM * pVM)
{
	EntityId playerId;
	EntityId vehicleId;
	sq_getentity(pVM, -2, &playerId);
	sq_getentity(pVM, -1, &vehicleId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(pPlayer->IsInVehicle())
		{
			sq_pushbool(pVM, (pPlayer->GetVehicle()->GetVehicleId() == vehicleId));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVehicleId(playerid)
SQInteger sq_player_getvehicleid(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(pPlayer->IsInVehicle())
		{
			sq_pushinteger(pVM, pPlayer->GetVehicle()->GetVehicleId());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSeatId(playerid)
SQInteger sq_player_getseatid(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer && pPlayer->IsInVehicle())
	{
		sq_pushinteger(pVM, pPlayer->GetVehicleSeatId());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerOnFoot(playerid)
SQInteger sq_player_isonfoot(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, !pPlayer->IsInVehicle());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerPayAndSpray(playerid, toggle)
SQInteger sq_player_togglepayandspray(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerAutoAim(playerid, toggle)
SQInteger sq_player_toggleautoaim(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerDrunk(playerid, toggle)
/*SQInteger sq_player_setplayerdrunk(SQVM * pVM)
{
	SQInteger playerid;
	bool toggle;
	sq_getinteger(vm, -2, &playerid);
	sq_getinteger(vm, -1, (int*)&toggle);
	CBitStream bsSend;
	bsSend.Write(toggle);
	g_pNetworkManager->RPC("SetPlayerDrunk", &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerid, false);
	sq_pushbool(vm, true);
	return 1;
}*/

// setPlayerSpawnLocation(playerid, x, y, z, r)
SQInteger sq_player_setspawnlocation(SQVM * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	float fRotation;
	sq_getentity(pVM, -5, &playerId);
	sq_getvector3(pVM, -4, &vecPos);
	sq_getfloat(pVM, -1, &fRotation);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->SetSpawnLocation(vecPos, fRotation);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerModel(playerid, model)
SQInteger sq_player_setmodel(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iModelId;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iModelId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->SetModel(iModelId));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerModel(playerid)
SQInteger sq_player_getmodel(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetModel());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerControls(playerid, toggle)
SQInteger sq_player_togglecontrols(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbControls;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbControls);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bControls = (sqbControls != 0);
		bsSend.Write(bControls);
		g_pNetworkManager->RPC(RPC_ScriptingToggleControls, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerSpawned(playerid)
SQInteger sq_player_isspawned(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsSpawned());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerHeading(playerid, heading)
SQInteger sq_player_setheading(SQVM * pVM)
{
	EntityId playerId;
	float fHeading;
	sq_getentity(pVM, -2, &playerId);
	sq_getfloat(pVM, -1, &fHeading);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fHeading);
		g_pNetworkManager->RPC(RPC_ScriptingSetHeading, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHeading(playerid)
SQInteger sq_player_getheading(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushfloat(pVM, pPlayer->GetCurrentHeading());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerPhysics(playerid, toggle)
SQInteger sq_player_togglephysics(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleRagdoll, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// kickPlayer(playerid, sendkickmessage)
SQInteger sq_player_kick(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbKickMessage;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbKickMessage);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->Kick(sqbKickMessage != 0);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// banPlayer(playerid, milliseconds)
SQInteger sq_player_ban(SQVM * pVM)
{
	EntityId playerId;
	SQInteger uiSeconds;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &uiSeconds);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->Ban(uiSeconds);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerIp(playerid)
SQInteger sq_player_getip(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushstring(pVM, pPlayer->GetIp(), -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// givePlayerMoney(playerid, money)
SQInteger sq_player_givemoney(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iMoney;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iMoney);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->GiveMoney(iMoney));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerMoney(playerid, money)
SQInteger sq_player_setmoney(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iMoney;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iMoney);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->SetMoney(iMoney));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// resetPlayerMoney(playerid)
SQInteger sq_player_resetmoney(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->SetMoney(0));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerMoney(playerid)
SQInteger sq_player_getmoney(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetMoney());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}


// togglePlayerFrozen(playerid, playerfrozen, camerafrozen)
SQInteger sq_player_togglefrozen(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbPlayerFrozen;
	SQBool sqbCameraFrozen;
	sq_getentity(pVM, -2, &playerId);
	// jenksta: is there a reason these are both the same?
	sq_getbool(pVM, -1, &sqbPlayerFrozen);
	sq_getbool(pVM, -1, &sqbCameraFrozen);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bPlayerFrozen = (sqbPlayerFrozen != 0);
		bsSend.Write(bPlayerFrozen);
		bool bCameraFrozen = (sqbCameraFrozen != 0);
		bsSend.Write(bCameraFrozen);
		g_pNetworkManager->RPC(RPC_ScriptingToggleFrozen, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerState(playerid)
SQInteger sq_player_getstate(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetState());
			return 1;
		}
	}
	else if(playerId >= 0 && playerId < MAX_PLAYERS)
	{
		sq_pushinteger(pVM, STATE_TYPE_DISCONNECT);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// displayPlayerText(playerid, x, y, text, time)
SQInteger sq_player_displaytext(SQVM * pVM)
{
	EntityId playerId;
	float fPos[2];
	const char * szText;
	SQInteger iTime;
	sq_getentity(pVM, -5, &playerId);
	sq_getfloat(pVM, -4, &fPos[0]);
	sq_getfloat(pVM, -3, &fPos[1]);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fPos[0]);
		bsSend.Write(fPos[1]);
		bsSend.Write(String(szText));
		bsSend.Write(iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// displayTextForAll(x, y, text, time)
SQInteger sq_player_displaytextforall(SQVM * pVM)
{
	float fPos[2];
	const char * szText;
	SQInteger iTime;
	sq_getfloat(pVM, -4, &fPos[0]);
	sq_getfloat(pVM, -3, &fPos[1]);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);
	CBitStream bsSend;
	bsSend.Write(fPos[0]);
	bsSend.Write(fPos[1]);
	bsSend.Write(String(szText));
	bsSend.Write(iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	sq_pushbool(pVM, true);
	return 1;
}

// DisplayInfoText(playerid, text, time)
SQInteger sq_player_displayinfotext(SQVM * pVM)
{
	EntityId playerId;
	const char * szText;
	SQInteger iTime;
	sq_getentity(pVM, -3, &playerId);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szText));
		bsSend.Write(iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayInfoText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// DisplayInfoTextForAll(playerid, text, time)
SQInteger sq_player_displayinfotextforall(SQVM * pVM)
{
	const char * szText;
	SQInteger iTime;
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);
	CBitStream bsSend;
	bsSend.Write(String(szText));
	bsSend.Write(iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerVelocity(playerid, x, y, z)
SQInteger sq_player_setvelocity(SQVM * pVM)
{
	EntityId playerId;
	CVector3 vecMoveSpeed;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecMoveSpeed);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecMoveSpeed);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerMoveSpeed, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVelocity(playerid)
SQInteger sq_player_getvelocity(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecMoveSpeed;
		pPlayer->GetMoveSpeed(&vecMoveSpeed);
		sq_newtable(pVM);
		sq_pushinteger(pVM, 0);
		sq_pushfloat(pVM, vecMoveSpeed.fX);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 1);
		sq_pushfloat(pVM, vecMoveSpeed.fY);
		sq_createslot(pVM, -3);
		sq_pushinteger(pVM, 2);
		sq_pushfloat(pVM, vecMoveSpeed.fZ);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// warpPlayerIntoVehicle(playerid, vehicleid, seatid = 0)
SQInteger sq_player_warpintovehicle(SQVM * pVM)
{
	SQInteger iParams = sq_gettop(pVM) - 1;

	CHECK_PARAMS_MIN("warpPlayerIntoVehicle", 2);
	CHECK_TYPE("warpPlayerIntoVehicle", 1, -iParams, OT_INTEGER);
	CHECK_TYPE("warpPlayerIntoVehicle", 2, -iParams + 1, OT_INTEGER);

	SQInteger iSeatId = 0;

	if(iParams >= 3)
	{
		CHECK_TYPE("warpPlayerIntoVehicle", 3, -iParams + 2, OT_INTEGER);
		sq_getinteger(pVM, -iParams + 2, &iSeatId);
	}

	EntityId playerId;
	EntityId vehicleId;
	sq_getentity(pVM, -iParams, &playerId);
	sq_getentity(pVM, -iParams + 1, &vehicleId);

	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(vehicleId))
	{
		CBitStream bsSend;
		bsSend.Write((EntityId)vehicleId);
		bsSend.Write((BYTE)iSeatId);
		g_pNetworkManager->RPC(RPC_ScriptingWarpPlayerIntoVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// removePlayerFromVehicle(playerid)
SQInteger sq_player_removefromvehicle(SQVM * pVM)
{
	CHECK_PARAMS_MIN("removePlayerFromVehicle", 1);
	CHECK_TYPE("removePlayerFromVehicle", 1, 2, OT_INTEGER);

	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	if(!g_pPlayerManager->DoesExist(playerId))
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	bool bGraceful = false;
	if(sq_gettop(pVM) >= 3)
	{
		CHECK_TYPE("removePlayerFromVehicle", 2, 3, OT_BOOL);
		SQBool b;
		sq_getbool(pVM, 3, &b);
		bGraceful = (b != 0);
	}

	CBitStream bsSend;
	bsSend.WriteBit(bGraceful);
	g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerFromVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	sq_pushbool(pVM, true);
	return 1;
}

// getPlayerWeapon(playerid)
SQInteger sq_player_getweapon(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetWeapon());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerAmmo(playerid)
SQInteger sq_player_getammo(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetAmmo());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSerial(playerid)
SQInteger sq_player_getserial(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(!pPlayer)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushstring(pVM, pPlayer->GetSerial().Get(), -1);
	return 1;
}

// setCameraBehindPlayer(playerid)
SQInteger sq_player_setcamerabehind(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(!g_pPlayerManager->DoesExist(playerId))
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	CBitStream bsSend;
	g_pNetworkManager->RPC(RPC_ScriptingSetCameraBehindPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	sq_pushbool(pVM, true);

	return 1;
}

// setPlayerDucking(playerid, ducking)
SQInteger sq_player_setducking(SQVM * pVM)
{
	EntityId playerId;
	SQBool iDucking;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &iDucking);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iDucking);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDucking, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerHUD(playerid, toggle)
SQInteger sq_player_togglehud(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleHUD, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerRadar(playerid, toggle)
SQInteger sq_player_toggleradar(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleRadar, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerNames(playerid, toggle)
SQInteger sq_player_togglenames(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerAreaNames(playerid, toggle)
SQInteger sq_player_toggleareanames(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleAreaNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getEmptyPlayerPadState()
SQInteger sq_player_getemptypadstate(SQVM * pVM)
{
	NetPadState padState;
	sq_newtable(pVM);
	sq_pushstring(pVM, "leftAnalogLR0", -1);
	sq_pushinteger(pVM, padState.byteLeftAnalogLR[0]);
	sq_createslot(pVM, -3);
	sq_pushstring(pVM, "leftAnalogLR1", -1);
	sq_pushinteger(pVM, padState.byteLeftAnalogLR[1]);
	sq_createslot(pVM, -3);
	sq_pushstring(pVM, "leftAnalogUD0", -1);
	sq_pushinteger(pVM, padState.byteLeftAnalogUD[0]);
	sq_createslot(pVM, -3);
	sq_pushstring(pVM, "leftAnalogUD1", -1);
	sq_pushinteger(pVM, padState.byteLeftAnalogUD[1]);
	sq_createslot(pVM, -3);
	sq_pushstring(pVM, "keys", -1);
	sq_pushinteger(pVM, padState.dwKeys);
	sq_createslot(pVM, -3);
	return 1;
}

// getPlayerPreviousPadState(playerid)
SQInteger sq_player_getpreviouspadstate(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		NetPadState padState;
		pPlayer->GetPreviousPadState(&padState);
		sq_newtable(pVM);
		sq_pushstring(pVM, "leftAnalogLR0", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogLR[0]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogLR1", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogLR[1]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogUD0", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogUD[0]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogUD1", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogUD[1]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "keys", -1);
		sq_pushinteger(pVM, padState.dwKeys);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPadState(playerid)
SQInteger sq_player_getpadstate(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		NetPadState padState;
		pPlayer->GetPadState(&padState);
		sq_newtable(pVM);
		sq_pushstring(pVM, "leftAnalogLR0", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogLR[0]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogLR1", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogLR[1]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogUD0", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogUD[0]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "leftAnalogUD1", -1);
		sq_pushinteger(pVM, padState.byteLeftAnalogUD[1]);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "keys", -1);
		sq_pushinteger(pVM, padState.dwKeys);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// triggerClientEvent( playerid, eventname, ... )
SQInteger sq_player_trigger_event(SQVM * pVM)
{
	SQInteger playerid;
	sq_getinteger(pVM, 2, &playerid);

	CSquirrelArguments* pArguments = new CSquirrelArguments();
	for(SQInteger i = 3; i <= sq_gettop( pVM ); ++ i )
	{
		if( !pArguments->pushFromStack( pVM, i ) )
		{
			delete pArguments;
			sq_pushbool(pVM, false);
			return 1;
		}
	}

	CBitStream bsSend;
	pArguments->serialize(&bsSend);
	delete pArguments;

	g_pNetworkManager->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerid, false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerColor(playerid, rgba)
SQInteger sq_player_setcolor(SQVM * pVM)
{
	EntityId playerId;
	SQInteger color;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &color);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->SetColor(color);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_getcolor(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetColor());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_getping(SQVM* pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetPing());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_setclothes(SQVM * pVM)
{
	SQInteger iPlayerId, iBodyPart, iClothes;
	sq_getinteger(pVM, 2, &iPlayerId);
	sq_getinteger(pVM, 3, &iBodyPart);
	sq_getinteger(pVM, 4, &iClothes);

	if(g_pPlayerManager->DoesExist(iPlayerId) && iBodyPart >= 0 && iBodyPart <= 10 && iClothes >= 0 && iClothes <= 255)
	{
		g_pPlayerManager->GetAt(iPlayerId)->SetClothes((unsigned char)iBodyPart, (unsigned char)iClothes);
		sq_pushbool(pVM, true);
		return 1;
	}
	
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_getclothes(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_newarray(pVM, 0);

		for(unsigned char uc = 0; uc < 11; uc ++ )
		{
			sq_pushinteger(pVM, pPlayer->GetClothes(uc));
			sq_arrayappend(pVM, -2);
		}

		return 1;
	}
	
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_resetclothes(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->ResetClothes();
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_respawn(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		g_pNetworkManager->RPC(RPC_Spawn, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, pPlayer->GetPlayerId(), false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_givehelmet(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->GiveHelmet();
		sq_pushbool(pVM, true);
		return 1;
	}
	
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_player_removehelmet(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->RemoveHelmet();
		sq_pushbool(pVM, true);
		return 1;
	}
	
	sq_pushbool(pVM, false);
	return 1;
}
