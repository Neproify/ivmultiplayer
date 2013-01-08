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
#include "CEvents.h"

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;

// Player functions

void CPlayerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isPlayerConnected", IsConnected, 1, "i");

	pScriptingManager->RegisterFunction("getPlayerName", GetName, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerName", SetName, 2, "is");

	pScriptingManager->RegisterFunction("setPlayerHealth", SetHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerHealth", GetHealth, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerArmour", SetArmour, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerArmour", GetArmour, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerCoordinates", GetCoordinates, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerPosition", GetCoordinates, 1, "i");

	// World stuffs
	pScriptingManager->RegisterFunction("setPlayerTime", SetTime, 3, "iii");
	pScriptingManager->RegisterFunction("setPlayerWeather", SetWeather, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerGravity", SetGravity, 2, "if");

	pScriptingManager->RegisterFunction("sendPlayerMessage", SendMessage, -1, NULL);
	pScriptingManager->RegisterFunction("sendMessageToAll", SendMessageToAll, -1, NULL);
	pScriptingManager->RegisterFunction("isPlayerInAnyVehicle", IsInAnyVehicle, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInVehicle", IsInVehicle, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerVehicleId", GetVehicleId, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSeatId", GetSeatId, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerOnFoot", IsOnFoot, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerPayAndSpray", TogglePayAndSpray, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAutoAim", ToggleAutoAim, 2, "ib");
	//pScriptingManager->RegisterFunction("setPlayerDrunk", SetPlayerDrunk, 2, "ii");
	pScriptingManager->RegisterFunction("givePlayerWeapon", GiveWeapon, 3, "iii");
	pScriptingManager->RegisterFunction("removePlayerWeapons", RemoveWeapons, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerSpawnLocation", SetSpawnLocation, 5, "iffff");
	pScriptingManager->RegisterFunction("setPlayerModel", SetModel, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerControls", ToggleControls, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "1");
	pScriptingManager->RegisterFunction("togglePlayerPhysics", TogglePhysics, 2, "ib");
	pScriptingManager->RegisterFunction("kickPlayer", Kick, 2, "ib");
	pScriptingManager->RegisterFunction("banPlayer", Ban, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerIp", GetIp, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerMoney", GiveMoney, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerMoney", SetMoney, 2, "ii");
	pScriptingManager->RegisterFunction("resetPlayerMoney", ResetMoney, 1, "ii");
	pScriptingManager->RegisterFunction("getPlayerMoney", GetMoney, 1, "i");
	pScriptingManager->RegisterFunction("displayPlayerText", DisplayText, 5, "iffsi");
	pScriptingManager->RegisterFunction("displayTextForAll", DisplayTextForAll, 4, "ffsi");
	pScriptingManager->RegisterFunction("displayPlayerInfoText", DisplayInfoText, 3, "isi");
	pScriptingManager->RegisterFunction("displayInfoTextForAll", DisplayInfoTextForAll, 2, "si");
	pScriptingManager->RegisterFunction("togglePlayerFrozen", ToggleFrozen, 2, "ib");
	pScriptingManager->RegisterFunction("getPlayerState", GetState, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerVelocity", SetVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWantedLevel", GetWantedLevel, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerWantedLevel", SetWantedLevel, 2, "ii");
	pScriptingManager->RegisterFunction("warpPlayerIntoVehicle", WarpIntoVehicle, -1, NULL);
	pScriptingManager->RegisterFunction("removePlayerFromVehicle", RemoveFromVehicle, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerWeapon", GetWeapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", GetAmmo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSerial", GetSerial, 1, "i");
	pScriptingManager->RegisterFunction("setCameraBehindPlayer", SetCameraBehind, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerDucking", SetDucking, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerDucking", IsDucking, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerInvincible", SetInvincible, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerHud", ToggleHUD, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerRadar", ToggleRadar, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerNames", ToggleNames, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAreaNames", ToggleAreaNames, 2, "ib");
	pScriptingManager->RegisterFunction("getEmptyPlayerPadState", GetEmptyControlState, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getEmptyPlayerControlState", GetEmptyControlState, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerPreviousControlState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerControlState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerColor", SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", GetPing, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerHelmet", GiveHelmet, 1, "i");
	pScriptingManager->RegisterFunction("removePlayerHelmet", RemoveHelmet, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerHelmet", ToggleHelmet, 2, "ib");
	pScriptingManager->RegisterFunction("setPlayerClothes", SetClothes, 3, "iii");
	pScriptingManager->RegisterFunction("getPlayerClothes", GetClothes, 1, "i");
	pScriptingManager->RegisterFunction("resetPlayerClothes", ResetClothes, 1, "i");
	pScriptingManager->RegisterFunction("respawnPlayer", Respawn, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerCameraPos", SetCameraPos, 4, "ifff");
	pScriptingManager->RegisterFunction("setPlayerCameraLookAt", SetCameraLookAt, 4, "ifff");
	pScriptingManager->RegisterFunction("resetPlayerCamera", ResetCamera, 1, "i");
	pScriptingManager->RegisterFunction("forcePlayerPlayAnimation", forceAnim, 3, "iss");
	pScriptingManager->RegisterFunction("triggerPlayerPoliceReport", triggerPoliceReport, 2, "is");
	pScriptingManager->RegisterFunction("triggerPlayerGameSound", triggerAudioEvent, 2, "is");
	pScriptingManager->RegisterFunction("triggerPlayerMissionSound", triggerMissionCompleteAudio, 2, "ii");
	pScriptingManager->RegisterFunction("fadePlayerScreenIn",fadeScreenIn, 2, "ii");
	pScriptingManager->RegisterFunction("fadePlayerScreenOut", fadeScreenOut, 2, "ii");
	pScriptingManager->RegisterFunction("blockPlayerDropWeaponsAtDeath",blockWeaponDrop, 2, "ib");
	pScriptingManager->RegisterFunction("blockPlayerWeaponScroll", blockWeaponChange, 2, "ib");
	pScriptingManager->RegisterFunction("requestPlayerAnimations", requestAnim, 2, "is");
	pScriptingManager->RegisterFunction("releasePlayerAnimations", releaseAnim, 2, "is");
	pScriptingManager->RegisterFunction("attachPlayerCameraToPlayer", AttachCamToPlayer, 6, "iiifff");
	pScriptingManager->RegisterFunction("attachPlayerCameraToVehicle", AttachCamToVehicle, 6, "iiifff");
	pScriptingManager->RegisterFunction("displayHudNotification", DisplayHudNotification, 3, "iis");
	pScriptingManager->RegisterFunction("setPlayerFollowVehicleMode", FollowVehicleMode, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerFollowVehicleOffset", FollowVehicleOffset, 5, "iifff");
	pScriptingManager->RegisterFunction("setPlayerAmmoInClip", SetAmmoInClip, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerAmmo", SetAmmo, 3, "iii");
	pScriptingManager->RegisterFunction("setPlayerUseMobilePhone", SetMobilePhone, 2, "ib");
	pScriptingManager->RegisterFunction("sayPlayerSpeech", SaySpeech, 3, "iss");
	pScriptingManager->RegisterFunction("letPlayerDriveAutomaticAtCoords", DriveAutomatic, 7, "iiffffi");
	pScriptingManager->RegisterFunction("togglePlayerNametagForPlayer",ToggleNametagForPlayer, 3, "iib");
	pScriptingManager->RegisterFunction("triggerClientEvent", TriggerEvent, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerFileChecksum", GetFileChecksum, 2, "ii");
	
	pScriptingManager->RegisterFunction("setPlayerDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerDimension", GetDimension, 1, "i");
}

// isPlayerConnected(playerid)
SQInteger CPlayerNatives::IsConnected(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->GetAt(playerId)->IsJoined())
		sq_pushbool(pVM, true);
	else
		sq_pushbool(pVM, false);
	return 1;
}

// getPlayerName(playerid)
SQInteger CPlayerNatives::GetName(SQVM * pVM)
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

SQInteger CPlayerNatives::SetName(SQVM * pVM)
{
	EntityId playerId;
	const char* szName;
	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szName);

	CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String Checkstring = szName;
		CSquirrelArguments nameCheckArguments;
		nameCheckArguments.push(playerId);
		nameCheckArguments.push(Checkstring);

		if(g_pEvents->Call("playerNameCheck", &nameCheckArguments).GetInteger() != 1)
		{
			CLogFile::Printf("Can't change the name from player %d (Invalid Characters)",playerId);

			sq_pushbool(pVM, false);
			return 0;
		} 

		sq_pushbool(pVM, pPlayer->SetName(szName));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// givePlayerWeapon(playerid, weaponid, ammo)
SQInteger CPlayerNatives::GiveWeapon(SQVM * pVM)
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
SQInteger CPlayerNatives::RemoveWeapons(SQVM * pVM)
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

//getPlayerWantedLevel(playerid, wantedlevel)
SQInteger CPlayerNatives::GetWantedLevel(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		sq_pushinteger(pVM, g_pPlayerManager->GetAt(playerId)->GetWantedLevel());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//setPlayerWantedLevel(playerid, wantedlevel)
SQInteger CPlayerNatives::SetWantedLevel(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iWantedLevel;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iWantedLevel);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pPlayerManager->GetAt(playerId)->SetWantedLevel(iWantedLevel);
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
SQInteger CPlayerNatives::SetHealth(SQVM * pVM)
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
SQInteger CPlayerNatives::GetHealth(SQVM * pVM)
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
SQInteger CPlayerNatives::SetArmour(SQVM * pVM)
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
SQInteger CPlayerNatives::GetArmour(SQVM * pVM)
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
SQInteger CPlayerNatives::SetCoordinates(SQVM * pVM)
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
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerCoordinates(playerid)
SQInteger CPlayerNatives::GetCoordinates(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecPosition;
		pPlayer->GetPosition(vecPosition);
		CSquirrelArguments args;
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTime(playerid, hour, minute)
SQInteger CPlayerNatives::SetTime(SQVM * pVM)
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
SQInteger CPlayerNatives::SetWeather(SQVM * pVM)
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
SQInteger CPlayerNatives::SetGravity(SQVM * pVM)
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
SQInteger CPlayerNatives::SendMessage(SQVM * pVM)
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

// sendMessageToAll(message [, color = 0xFFFFFFAA, allowformatting = true])
SQInteger CPlayerNatives::SendMessageToAll(SQVM * pVM)
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
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerInAnyVehicle(playerid)
SQInteger CPlayerNatives::IsInAnyVehicle(SQVM * pVM)
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
SQInteger CPlayerNatives::IsInVehicle(SQVM * pVM)
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
SQInteger CPlayerNatives::GetVehicleId(SQVM * pVM)
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
SQInteger CPlayerNatives::GetSeatId(SQVM * pVM)
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
SQInteger CPlayerNatives::IsOnFoot(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsOnFoot());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerPayAndSpray(playerid, toggle)
SQInteger CPlayerNatives::TogglePayAndSpray(SQVM * pVM)
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
SQInteger CPlayerNatives::ToggleAutoAim(SQVM * pVM)
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
/*SQInteger CPlayerNatives::SetPlayerDrunk(SQVM * pVM)
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
SQInteger CPlayerNatives::SetSpawnLocation(SQVM * pVM)
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
SQInteger CPlayerNatives::SetModel(SQVM * pVM)
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
SQInteger CPlayerNatives::GetModel(SQVM * pVM)
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
SQInteger CPlayerNatives::ToggleControls(SQVM * pVM)
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
SQInteger CPlayerNatives::IsSpawned(SQVM * pVM)
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
SQInteger CPlayerNatives::SetHeading(SQVM * pVM)
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
SQInteger CPlayerNatives::GetHeading(SQVM * pVM)
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
SQInteger CPlayerNatives::TogglePhysics(SQVM * pVM)
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
SQInteger CPlayerNatives::Kick(SQVM * pVM)
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
SQInteger CPlayerNatives::Ban(SQVM * pVM)
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
SQInteger CPlayerNatives::GetIp(SQVM * pVM)
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
SQInteger CPlayerNatives::GiveMoney(SQVM * pVM)
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
SQInteger CPlayerNatives::SetMoney(SQVM * pVM)
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
SQInteger CPlayerNatives::ResetMoney(SQVM * pVM)
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
SQInteger CPlayerNatives::GetMoney(SQVM * pVM)
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
SQInteger CPlayerNatives::ToggleFrozen(SQVM * pVM)
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
SQInteger CPlayerNatives::GetState(SQVM * pVM)
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
SQInteger CPlayerNatives::DisplayText(SQVM * pVM)
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
		bsSend.Write((int)iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// displayTextForAll(x, y, text, time)
SQInteger CPlayerNatives::DisplayTextForAll(SQVM * pVM)
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
	bsSend.Write((int)iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	sq_pushbool(pVM, true);
	return 1;
}

// displayInfoText(playerid, text, time)
SQInteger CPlayerNatives::DisplayInfoText(SQVM * pVM)
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
		bsSend.Write((int)iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayInfoText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// displayInfoTextForAll(playerid, text, time)
SQInteger CPlayerNatives::DisplayInfoTextForAll(SQVM * pVM)
{
	const char * szText;
	SQInteger iTime;
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);
	CBitStream bsSend;
	bsSend.Write(String(szText));
	bsSend.Write((int)iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerVelocity(playerid, x, y, z)
SQInteger CPlayerNatives::SetVelocity(SQVM * pVM)
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
SQInteger CPlayerNatives::GetVelocity(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecMoveSpeed;
		pPlayer->GetMoveSpeed(vecMoveSpeed);
		CSquirrelArguments args;
		args.push(vecMoveSpeed.fX);
		args.push(vecMoveSpeed.fY);
		args.push(vecMoveSpeed.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// warpPlayerIntoVehicle(playerid, vehicleid, seatid = 0)
SQInteger CPlayerNatives::WarpIntoVehicle(SQVM * pVM)
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
SQInteger CPlayerNatives::RemoveFromVehicle(SQVM * pVM)
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
SQInteger CPlayerNatives::GetWeapon(SQVM * pVM)
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
SQInteger CPlayerNatives::GetAmmo(SQVM * pVM)
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
SQInteger CPlayerNatives::GetSerial(SQVM * pVM)
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
SQInteger CPlayerNatives::SetCameraBehind(SQVM * pVM)
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
SQInteger CPlayerNatives::SetDucking(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbDucking;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbDucking);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbDucking != 0);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDucking, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerDucking(playerid)
SQInteger CPlayerNatives::IsDucking(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsDucking());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerInvincible(playerid, invincible)
SQInteger CPlayerNatives::SetInvincible(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbInvincible;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbInvincible);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbInvincible != 0);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerInvincible, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerHUD(playerid, toggle)
SQInteger CPlayerNatives::ToggleHUD(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbToggle != 0);
		g_pNetworkManager->RPC(RPC_ScriptingToggleHUD, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerRadar(playerid, toggle)
SQInteger CPlayerNatives::ToggleRadar(SQVM * pVM)
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
SQInteger CPlayerNatives::ToggleNames(SQVM * pVM)
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
SQInteger CPlayerNatives::ToggleAreaNames(SQVM * pVM)
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

// getEmptyPlayerControlState()
SQInteger CPlayerNatives::GetEmptyControlState(SQVM * pVM)
{
	// Create a new control state
	CControlState controlState;

	// Create the table and array
	CSquirrelArguments table;
	CSquirrelArguments array;

	// Create the 'onFootMove' array
	table.push("onFootMove");
	array.reset();

	for(int i = 0; i < 4; i++)
		array.push(controlState.ucOnFootMove[i]);

	table.push(array, true);

	// Create the 'inVehicleMove' array
	table.push("inVehicleMove");
	array.reset();

	for(int i = 0; i < 4; i++)
		array.push(controlState.ucInVehicleMove[i]);

	table.push(array, true);

	// Create the 'inVehicleTriggers' array
	table.push("inVehicleTriggers");
	array.reset();

	for(int i = 0; i < 2; i++)
		array.push(controlState.ucInVehicleTriggers[i]);

	table.push(array, true);

	// Create the on foot keys slots
	table.push("enterExitVehicle");
	table.push(controlState.keys.bEnterExitVehicle);
	table.push("sprint");
	table.push(controlState.keys.bSprint);
	table.push("jump");
	table.push(controlState.keys.bJump);
	table.push("attack");
	table.push(controlState.keys.bAttack);
	table.push("attack2");
	table.push(controlState.keys.bAttack2);
	table.push("aim");
	table.push(controlState.keys.bAim);
	table.push("freeAim");
	table.push(controlState.keys.bFreeAim);
	table.push("meleeAttack1");
	table.push(controlState.keys.bMeleeAttack1);
	table.push("meleeAttack2");
	table.push(controlState.keys.bMeleeAttack2);
	table.push("meleeKick");
	table.push(controlState.keys.bMeleeKick);
	table.push("meleeBlock");
	table.push(controlState.keys.bMeleeBlock);

	// Create the in vehicle keys slots
	table.push("handbrake");
	table.push(controlState.keys.bHandbrake);
	table.push("handbrake2");
	table.push(controlState.keys.bHandbrake2);
	table.push("horn");
	table.push(controlState.keys.bHorn);
	table.push("driveBy");
	table.push(controlState.keys.bDriveBy);
	table.push("heliPrimaryFire");
	table.push(controlState.keys.bHeliPrimaryFire);

	// Push the table to the VM
	sq_pusharg(pVM, CSquirrelArgument(table, false));
	return 1;
}

// getPlayerPreviousControlState(playerid)
SQInteger CPlayerNatives::GetPreviousControlState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player control state
		CControlState controlState;
		pPlayer->GetPreviousControlState(&controlState);

		// Create the table and array
		CSquirrelArguments table;
		CSquirrelArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(controlState.ucOnFootMove[i]);

		table.push(array, true);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(controlState.ucInVehicleMove[i]);

		table.push(array, true);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
			array.push(controlState.ucInVehicleTriggers[i]);

		table.push(array, true);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(controlState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(controlState.keys.bSprint);
		table.push("jump");
		table.push(controlState.keys.bJump);
		table.push("attack");
		table.push(controlState.keys.bAttack);
		table.push("attack2");
		table.push(controlState.keys.bAttack2);
		table.push("aim");
		table.push(controlState.keys.bAim);
		table.push("freeAim");
		table.push(controlState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(controlState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(controlState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(controlState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(controlState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(controlState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(controlState.keys.bHandbrake2);
		table.push("horn");
		table.push(controlState.keys.bHorn);
		table.push("driveBy");
		table.push(controlState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(controlState.keys.bHeliPrimaryFire);

		// Push the table to the VM
		sq_pusharg(pVM, CSquirrelArgument(table, false));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerControlState(playerid)
SQInteger CPlayerNatives::GetControlState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player control state
		CControlState controlState;
		pPlayer->GetControlState(&controlState);

		// Create the table and array
		CSquirrelArguments table;
		CSquirrelArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(controlState.ucOnFootMove[i]);

		table.push(array, true);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(controlState.ucInVehicleMove[i]);

		table.push(array, true);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
			array.push(controlState.ucInVehicleTriggers[i]);

		table.push(array, true);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(controlState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(controlState.keys.bSprint);
		table.push("jump");
		table.push(controlState.keys.bJump);
		table.push("attack");
		table.push(controlState.keys.bAttack);
		table.push("attack2");
		table.push(controlState.keys.bAttack2);
		table.push("aim");
		table.push(controlState.keys.bAim);
		table.push("freeAim");
		table.push(controlState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(controlState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(controlState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(controlState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(controlState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(controlState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(controlState.keys.bHandbrake2);
		table.push("horn");
		table.push(controlState.keys.bHorn);
		table.push("driveBy");
		table.push(controlState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(controlState.keys.bHeliPrimaryFire);

		// Push the table to the VM
		sq_pusharg(pVM, CSquirrelArgument(table, false));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// triggerClientEvent(playerid, eventname, ...)
SQInteger CPlayerNatives::TriggerEvent(SQVM * pVM)
{
	CHECK_PARAMS_MIN("triggerClientEvent", 2);
	CHECK_TYPE("triggerClientEvent", 1, 2, OT_INTEGER);
	CHECK_TYPE("triggerClientEvent", 2, 3, OT_STRING);

	SQInteger playerId;
	sq_getinteger(pVM, 2, &playerId);
	CSquirrelArguments arguments(pVM, 3);

	CBitStream bsSend;
	arguments.serialize(&bsSend);
	g_pNetworkManager->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerColor(playerid, rgba)
SQInteger CPlayerNatives::SetColor(SQVM * pVM)
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

SQInteger CPlayerNatives::GetColor(SQVM * pVM)
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

SQInteger CPlayerNatives::GetPing(SQVM* pVM)
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

SQInteger CPlayerNatives::SetClothes(SQVM * pVM)
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

SQInteger CPlayerNatives::GetClothes(SQVM * pVM)
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

SQInteger CPlayerNatives::ResetClothes(SQVM * pVM)
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

SQInteger CPlayerNatives::Respawn(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CBitStream bitStream;
		bitStream.WriteCompressed(playerId);
		g_pNetworkManager->RPC(RPC_PlayerSpawn, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, pPlayer->GetPlayerId(), false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::GiveHelmet(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

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

SQInteger CPlayerNatives::RemoveHelmet(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

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

// togglePlayerHelmet(playerid, toggle)
SQInteger CPlayerNatives::ToggleHelmet(SQVM * pVM)
{
	EntityId playerId;
	SQBool sqbToggle;
	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		bool bToggle = (sqbToggle != 0);
		if(bToggle)
			pPlayer->GiveHelmet();
		else
			pPlayer->RemoveHelmet();

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraPos(playerid, x, y, z)
SQInteger CPlayerNatives::SetCameraPos(SQVM * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraPos, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraLookAt(playerid, x, y, z)
SQInteger CPlayerNatives::SetCameraLookAt(SQVM * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraLookAt, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// resetPlayerCamera(playerid)
SQInteger CPlayerNatives::ResetCamera(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pNetworkManager->RPC(RPC_ScriptingResetPlayerCamera, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::forceAnim(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -3, &playerId);

	const char *szGroup = NULL;
	sq_getstring(pVM,-2,&szGroup);

	const char *szAnim = NULL;
	sq_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(String(szGroup));
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingForcePlayerAnim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::requestAnim(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);
	
	const char *szAnim = NULL;
	sq_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingRequestAnims, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::releaseAnim(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);
	
	const char *szAnim = NULL;
	sq_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingReleaseAnims, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::triggerAudioEvent(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	const char *szAudio = NULL;
	sq_getstring(pVM,-1,&szAudio);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAudio));
		g_pNetworkManager->RPC(RPC_ScriptingPlayGameAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::triggerMissionCompleteAudio(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQInteger szMission;
	sq_getinteger(pVM,-1,&szMission);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)szMission);
		g_pNetworkManager->RPC(RPC_ScriptingPlayMissionCompleteAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::triggerPoliceReport(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	const char *szAudio = NULL;
	sq_getstring(pVM,-1,&szAudio);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAudio));
		g_pNetworkManager->RPC(RPC_ScriptingPlayPoliceReport, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::fadeScreenIn(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQInteger iDuration;
	sq_getinteger(pVM,-1,&iDuration);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iDuration);
		g_pNetworkManager->RPC(RPC_ScriptingFadeScreenIn, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::fadeScreenOut(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQInteger iDuration;
	sq_getinteger(pVM,-1,&iDuration);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iDuration);
		g_pNetworkManager->RPC(RPC_ScriptingFadeScreenOut, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::blockWeaponChange(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQBool bToggle;
	sq_getbool(pVM,-1,&bToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		bool bSwitch = (bToggle != 0);
		CBitStream bsSend;
		bsSend.Write(bSwitch);
		g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponChange, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::blockWeaponDrop(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQBool bToggle;
	sq_getbool(pVM,-1,&bToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		bool bSwitch = (bToggle != 0);
		CBitStream bsSend;
		bsSend.Write(bSwitch);
		g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponDrop, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::AttachCamToPlayer(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -6, &playerId);

	EntityId toPlayerId;
	sq_getentity(pVM, -5, &toPlayerId);

	int iPointType;
	sq_getinteger(pVM, -4, &iPointType);

	CVector3 vecPos;
	sq_getfloat(pVM, -3, &vecPos.fX);
	sq_getfloat(pVM, -2, &vecPos.fY);
	sq_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(toPlayerId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(toPlayerId);
		bsSend.Write(vecPos);
		bsSend.Write(iPointType);
		bsSend.Write0();
		g_pNetworkManager->RPC(RPC_ScriptingAttachCam, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::AttachCamToVehicle(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -6, &playerId);

	EntityId toVehicleId;
	sq_getentity(pVM, -5, &toVehicleId);

	int iPointType;
	sq_getinteger(pVM, -4, &iPointType);

	CVector3 vecPos;
	sq_getfloat(pVM, -3, &vecPos.fX);
	sq_getfloat(pVM, -2, &vecPos.fY);
	sq_getfloat(pVM, -1, &vecPos.fZ);


	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(toVehicleId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(toVehicleId);
		bsSend.Write(vecPos);
		bsSend.Write(iPointType);
		bsSend.Write1();
		g_pNetworkManager->RPC(RPC_ScriptingAttachCam, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::DisplayHudNotification(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -3, &playerId);
	
	SQInteger iMode;
	sq_getinteger(pVM, -2, &iMode);

	const char * szMessage;
	sq_getstring(pVM, -1, &szMessage);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iMode);
		bsSend.Write(String(szMessage));
		g_pNetworkManager->RPC(RPC_ScriptingDisplayHudNotification, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::FollowVehicleMode(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);
	
	SQInteger iMode;
	sq_getinteger(pVM, -1, &iMode);

	if(iMode < 0 || iMode > 5)
	{
		CLogFile::Print("Vehicle follow modes are only supported from 0 to 5!");
		sq_pushbool(pVM,false);
		return 1;
	}

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iMode);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleFollowMode, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::FollowVehicleOffset(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -5, &playerId);

	EntityId vehicleId;
	sq_getentity(pVM, -4, &vehicleId);

	CVector3 vecPos;
	sq_getfloat(pVM, -3, &vecPos.fX);
	sq_getfloat(pVM, -2, &vecPos.fY);
	sq_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(vehicleId))
	{
		CBitStream bsSend;
		bsSend.Write(vehicleId);
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleFollowOffset, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SetAmmoInClip(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	SQInteger iAmmoInClip;
	sq_getinteger(pVM, -1, &iAmmoInClip);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iAmmoInClip);
		g_pNetworkManager->RPC(RPC_ScriptingSetAmmoInClip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SetAmmo(SQVM * pVM)
{
	EntityId playerId;
	SQInteger iWeaponId;
	SQInteger iAmmo;

	sq_getentity(pVM, -3, &playerId);
	sq_getinteger(pVM, -2, &iWeaponId);
	sq_getinteger(pVM, -1, &iAmmo);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iWeaponId);
		bsSend.Write((int)iAmmo);
		g_pNetworkManager->RPC(RPC_ScriptingSetAmmo, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SetMobilePhone(SQVM * pVM)
{
	EntityId playerId;
	SQBool bUse;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &bUse);

	bool bToggle = (bUse != 0);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pPlayerManager->GetAt(playerId)->UseMobilePhone(bToggle);
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerUseMobilePhone, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SaySpeech(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -3, &playerId);
	
	const char * szVoice;
	sq_getstring(pVM, -2, &szVoice);

	const char * szText;
	sq_getstring(pVM, -1, &szText);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(String(szVoice));
		bsSend.Write(String(szText));
		g_pNetworkManager->RPC(RPC_ScriptingPlayerSaySpeech, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::ToggleNametagForPlayer(SQVM * pVM)
{
	EntityId playerId;
	EntityId forPlayerId;
	SQBool bToggle;

	sq_getentity(pVM, -3, &playerId);
	sq_getentity(pVM, -2, &forPlayerId);
	sq_getbool(pVM, -1, &bToggle);

	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(forPlayerId))
	{
		bool bShow = (bToggle != 0);

		CBitStream bsSend;
		bsSend.Write(forPlayerId);
		bsSend.Write(bShow);
		g_pNetworkManager->RPC(RPC_ScriptingTogglePlayerLabelForPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
	
}

SQInteger CPlayerNatives::DriveAutomatic(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM,-7,&playerId);

	EntityId vehicleId;
	sq_getentity(pVM,-6,&vehicleId);

	CVector3 vecPos;
	sq_getfloat(pVM,-5,&vecPos.fX);
	sq_getfloat(pVM,-4,&vecPos.fY);
	sq_getfloat(pVM,-3,&vecPos.fZ);

	float fSpeed;
	sq_getfloat(pVM,-2,&fSpeed);

	SQInteger iDrivingStyle;
	sq_getinteger(pVM,-1,&iDrivingStyle);

	// Check if we have a valid drivingstyle
	if(iDrivingStyle < 0 || iDrivingStyle > 3)
	{
		CLogFile::Print("Failed to activate automatic vehicle drive(wrong drivingstyle(supported from 0 to 3))");
		sq_pushbool(pVM,false);
		return false;
	}

	// Check if we have a valid vehicle
	if(!g_pVehicleManager->DoesExist(vehicleId))
	{
		CLogFile::Print("Failed to activate automatic vehicle drive(wrong vehicle(not valid))");
		sq_pushbool(pVM,false);
		return false;
	}

	// Check if we are in your vehicle
	if(g_pVehicleManager->GetAt(vehicleId)->GetDriver() != NULL)
	{
		if(g_pVehicleManager->GetAt(vehicleId)->GetDriver()->GetPlayerId() != playerId)
		{
			CLogFile::Print("Failed to activate automatic vehicle drive(given player is not as driver in given vehicle)");
			sq_pushbool(pVM,false);
			return false;
		}
		else {
			CBitStream bsSend;
			bsSend.Write(playerId);
			bsSend.Write(vehicleId);
			bsSend.Write(vecPos);
			bsSend.Write(fSpeed);
			bsSend.Write((int)iDrivingStyle);
			g_pNetworkManager->RPC(RPC_ScriptingLetPlayerDriveAutomatic, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			sq_pushbool(pVM, true);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SetDimension(SQVM * pVM)
{
	SQInteger iDimension;
	EntityId playerId;

	sq_getinteger(pVM, -1, &iDimension);
	sq_getentity(pVM, -2, &playerId);
	
	CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer) {
		pPlayer->SetDimension(iDimension);
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(iDimension);

		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		sq_pushbool(pVM, true);
		return true;
	}
	sq_pushbool(pVM, false);
	return false;
}

SQInteger CPlayerNatives::GetDimension(SQVM * pVM)
{ 
	EntityId playerId;

	sq_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetDimension());
		return 1;
	}

	sq_pushinteger(pVM, -1);
	return 1;
}

SQInteger CPlayerNatives::GetFileChecksum(SQVM * pVM)
{ 
	EntityId playerId;
	int iFile;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iFile);
	if(iFile >= 0 && iFile < 2) {
		if(g_pPlayerManager->DoesExist(playerId)) {
			sq_pushinteger(pVM, g_pPlayerManager->GetAt(playerId)->GetFileChecksum(iFile));
			return 1;
		}
		else {
			sq_pushbool(pVM, false);
			return 1;
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}