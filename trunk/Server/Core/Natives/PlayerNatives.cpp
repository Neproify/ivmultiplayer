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
#include "StaticPlayerNatives.h"

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;

// Player scripting natives for Squirrel
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
	pScriptingManager->RegisterFunction("setPlayerDrunk", ToggleDrunk, 2, "ii");
	pScriptingManager->RegisterFunction("givePlayerWeapon", GiveWeapon, 3, "iii");
	pScriptingManager->RegisterFunction("removePlayerWeapons", RemoveWeapons, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerSpawnLocation", SetSpawnLocation, 5, "iffff");
	pScriptingManager->RegisterFunction("setPlayerModel", SetModel, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerControls", ToggleControls, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "i");
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
	pScriptingManager->RegisterFunction("triggerClientEvent", TriggerClientEvent, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerFileChecksum", GetFileChecksum, 2, "ii");
	
	pScriptingManager->RegisterFunction("setPlayerDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerDimension", GetDimension, 1, "i");
}

// Set/Get
SQInteger CPlayerNatives::SetName(SQVM * pVM)
{
	// setPlayerName(playerid, name) : boolean
	EntityId playerId;
	const char* szName;

	sq_getentity(pVM, -2, &playerId);
	sq_getstring(pVM, -1, &szName);

	sq_pushbool(pVM, StaticNatives::SetPlayerName(playerId, String(szName)));
	return 1;
}
SQInteger CPlayerNatives::GetName(SQVM * pVM)
{
	// getPlayerName(playerid) : string
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	String strPlayerName;
	if(StaticNatives::GetPlayerName(playerId, strPlayerName))
		sq_pushstring(pVM, strPlayerName, -1);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetWantedLevel(SQVM * pVM)
{
	// setPlayerWantedLevel(playerid, wantedlevel) : boolean
	EntityId playerId;
	SQInteger iWantedLevel;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iWantedLevel);

	if(StaticNatives::SetPlayerWantedLevel(playerId, (unsigned int)iWantedLevel))
		sq_pushbool(pVM, true);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::GetWantedLevel(SQVM * pVM)
{
	// getPlayerWantedLevel(playerid, wantedlevel) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	unsigned int uiWantedLevel = 0;
	if(StaticNatives::GetPlayerWantedLevel(playerId, uiWantedLevel))
		sq_pushinteger(pVM, uiWantedLevel);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetHealth(SQVM * pVM)
{
	// setPlayerHealth(playerid, health) : boolean
	EntityId playerId;
	SQInteger iHealth;
	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iHealth);

	sq_pushbool(pVM, StaticNatives::SetPlayerHealth(playerId, iHealth));
	return 1;
}
SQInteger CPlayerNatives::GetHealth(SQVM * pVM)
{
	// getPlayerHealth(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	int iHealth = -1;
	if(StaticNatives::GetPlayerHealth(playerId, iHealth))
		sq_pushinteger(pVM, iHealth);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetArmour(SQVM * pVM)
{
	// setPlayerArmour(playerid) : boolean
	EntityId playerId;
	SQInteger iArmour;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iArmour);

	sq_pushbool(pVM, StaticNatives::SetPlayerArmour(playerId, iArmour));	
	return 1;
}
SQInteger CPlayerNatives::GetArmour(SQVM * pVM)
{
	// getPlayerArmour(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	unsigned int uiArmour = 0;
	if(StaticNatives::GetPlayerArmour(playerId, uiArmour))
		sq_pushinteger(pVM, uiArmour);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetCoordinates(SQVM * pVM)
{
	// setPlayerCoordinates(playerid, x, y, z) : boolean
	EntityId playerId;
	CVector3 vecPos;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecPos);

	sq_pushbool(pVM, StaticNatives::SetPlayerCoordinates(playerId, vecPos));
	return 1;
}
SQInteger CPlayerNatives::GetCoordinates(SQVM * pVM)
{
	// getPlayerCoordinates(playerid) : array
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CVector3 vecPos;
	if(StaticNatives::GetPlayerCoordinates(playerId, vecPos))
	{
		CSquirrelArguments args;
		args.push(vecPos.fX);
		args.push(vecPos.fY);
		args.push(vecPos.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
	}
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetModel(SQVM * pVM)
{
	// setPlayerModel(playerid, model) : boolean
	EntityId playerId;
	SQInteger iModelId;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iModelId);

	sq_pushbool(pVM, StaticNatives::SetPlayerModel(playerId, iModelId));
	return 1;
}
SQInteger CPlayerNatives::GetModel(SQVM * pVM)
{
	// getPlayerModel(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	unsigned short usModelId;
	if(StaticNatives::GetPlayerModel(playerId, usModelId))
		sq_pushinteger(pVM, usModelId);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetHeading(SQVM * pVM)
{
	// setPlayerHeading(playerid, heading) : boolean
	EntityId playerId;
	float fHeading;
	sq_getentity(pVM, -2, &playerId);
	sq_getfloat(pVM, -1, &fHeading);	

	sq_pushbool(pVM, StaticNatives::SetPlayerHeading(playerId, fHeading));
	return 1;
}
SQInteger CPlayerNatives::GetHeading(SQVM * pVM)
{
	// getPlayerHeading(playerid) : float
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	float fHeading;
	if(StaticNatives::GetPlayerHeading(playerId, fHeading))
		sq_pushfloat(pVM, fHeading);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetVelocity(SQVM * pVM)
{
	// setPlayerVelocity(playerId, x,y,z) : boolean
	EntityId playerId;
	CVector3 vecSpeed;
	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecSpeed);

	sq_pushbool(pVM, StaticNatives::SetPlayerVelocity(playerId, vecSpeed));
	return 1;
}
SQInteger CPlayerNatives::GetVelocity(SQVM * pVM)
{
	// getPlayerVelocity(playerId) : array
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CVector3 vecSpeed;
	if(StaticNatives::SetPlayerVelocity(playerId, vecSpeed))
	{
		CSquirrelArguments args;
		args.push(vecSpeed.fX);
		args.push(vecSpeed.fY);
		args.push(vecSpeed.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
	}
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetDucking(SQVM * pVM)
{
	// setPlayerDucking(playerid, ducking) : boolean
	EntityId playerId;
	SQBool sqbDucking;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbDucking);

	sq_pushbool(pVM, StaticNatives::SetPlayerDucking(playerId, sqbDucking != 0));
	return 1;
}
SQInteger CPlayerNatives::IsDucking(SQVM * pVM)
{
	// isPlayerDucking(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM,  StaticNatives::IsPlayerDucking(playerId));
	return 1;
}
SQInteger CPlayerNatives::SetColor(SQVM * pVM)
{
	// setPlayerColor(playerid, rgba) : boolean
	EntityId playerId;
	SQInteger iColor;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iColor);

	sq_pushbool(pVM, StaticNatives::SetPlayerColor(playerId, iColor));
	return 1;
}
SQInteger CPlayerNatives::GetColor(SQVM * pVM)
{
	// getPlayerColor(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	unsigned int uiColor;
	if(StaticNatives::GetPlayerColor(playerId, uiColor))
		sq_pushinteger(pVM, uiColor);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetClothes(SQVM * pVM)
{
	// setPlayerClothes(playerId, iBodyPart, iClothes) : boolean
	SQInteger playerId, iBodyPart, iClothes;
	sq_getinteger(pVM, 2, &playerId);
	sq_getinteger(pVM, 3, &iBodyPart);
	sq_getinteger(pVM, 4, &iClothes);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer && (iBodyPart >= 0 && iBodyPart <= 10) && (iClothes >= 0 && iClothes <= 255))
	{
		pPlayer->SetClothes((unsigned char)iBodyPart, (unsigned char)iClothes);
		sq_pushbool(pVM, true);
		return 1;
	}
	
	sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::GetClothes(SQVM * pVM)
{
	// setPlayerClothes(playerId) : array
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
SQInteger CPlayerNatives::SetDimension(SQVM * pVM)
{
	// setPlayerDimension(playerid, dimension) : boolean
	SQInteger iDimension;
	EntityId playerId;

	sq_getinteger(pVM, -1, &iDimension);
	sq_getentity(pVM, -2, &playerId);
	
	sq_pushbool(pVM, StaticNatives::SetPlayerDimension(playerId, iDimension));
	return false;
}
SQInteger CPlayerNatives::GetDimension(SQVM * pVM)
{ 
	// getPlayerDimension(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	DimensionId ucDimension;
	if(StaticNatives::GetPlayerDimension(playerId, ucDimension))
		sq_pushinteger(pVM, ucDimension);
	else
		sq_pushbool(pVM, false);
	return 1;
}

// Weapons
SQInteger CPlayerNatives::GiveWeapon(SQVM * pVM)
{
	// givePlayerWeapon(playerid, weaponid, ammo) : boolean
	EntityId playerId;
	SQInteger iWeaponId;
	SQInteger iAmmo;

	sq_getentity(pVM, -3, &playerId);
	sq_getinteger(pVM, -2, &iWeaponId);
	sq_getinteger(pVM, -1, &iAmmo);

	sq_pushbool(pVM, StaticNatives::GivePlayerWeapon(playerId, iWeaponId, iAmmo));
	return 1;
}
SQInteger CPlayerNatives::RemoveWeapons(SQVM * pVM)
{
	// removePlayerWeapons(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM, StaticNatives::RemovePlayerWeapons(playerId));
	return 1;
}
SQInteger CPlayerNatives::GetWeapon(SQVM * pVM)
{
	// getPlayerWeapon(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	unsigned int uiWeaponModelId = 0;
	if(StaticNatives::GetPlayerWeapon(playerId, uiWeaponModelId))
		sq_pushinteger(pVM, uiWeaponModelId);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::GetAmmo(SQVM * pVM)
{
	// getPlayerAmmo(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	unsigned int iWeaponAmmo = 0;
	if(StaticNatives::GetPlayerAmmo(playerId, iWeaponAmmo))
		sq_pushinteger(pVM, iWeaponAmmo);
	else
		sq_pushbool(pVM, false);
	return 1;
}

// Money
SQInteger CPlayerNatives::GiveMoney(SQVM * pVM)
{
	// givePlayerMoney(playerid, iAddMoney) : boolean
	EntityId playerId;
	SQInteger iAddMoney;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iAddMoney);

	sq_pushbool(pVM, StaticNatives::GivePlayerMoney(playerId, iAddMoney));
	return 1;
}
SQInteger CPlayerNatives::SetMoney(SQVM * pVM)
{
	// setPlayerMoney(playerid, iMoney) : boolean
	EntityId playerId;
	SQInteger iMoney;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iMoney);

	sq_pushbool(pVM, StaticNatives::SetPlayerMoney(playerId, iMoney));
	return 1;
}
SQInteger CPlayerNatives::GetMoney(SQVM * pVM)
{
	// getPlayerMoney(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	int iMoney;
	if(StaticNatives::GetPlayerMoney(playerId, iMoney))
		sq_pushinteger(pVM, iMoney);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::ResetMoney(SQVM * pVM)
{
	// resetPlayerMoney(playerid) : boolean
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

// Toggles
SQInteger CPlayerNatives::ToggleDrunk(SQVM * pVM)
{
	// togglePlayerDrunk(playerId, bDrunkCamera, bDrunk) : boolean TODO
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);
	
	sq_pushbool(pVM, StaticNatives::TogglePlayerDrunk(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::TogglePayAndSpray(SQVM * pVM)
{
	// togglePlayerPayAndSpray(playerid, bAllowedToPayAndSpray) : boolean
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerPayAndSpray(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleAutoAim(SQVM * pVM)
{
	// togglePlayerAutoAim(playerid, bEnabled) : boolean
	// TODO: Check this function
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerAutoAim(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleControls(SQVM * pVM)
{
	// togglePlayerControls(playerid, toggle) : boolean
	EntityId playerId;
	SQBool sqbControls;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbControls);	

	sq_pushbool(pVM, StaticNatives::TogglePlayerControls(playerId, sqbControls != 0));
	return 1;
}
SQInteger CPlayerNatives::TogglePhysics(SQVM * pVM)
{
	// TODO: Rename to 'togglePlayerRagdoll'  If it works i mean.
	// togglePlayerPhysics(playerid, bEabled) : bool
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);	

	sq_pushbool(pVM, StaticNatives::TogglePlayerPhysics(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleFrozen(SQVM * pVM)
{
	// togglePlayerFrozen(playerid, bPlayerFrozen, bCameraFrozen) : boolean
	EntityId playerId;
	SQBool sqbPlayerFrozen;
	SQBool sqbCameraFrozen;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbPlayerFrozen);
	sq_getbool(pVM, -1, &sqbCameraFrozen);

	sq_pushbool(pVM, StaticNatives::TogglePlayerFrozen(playerId, sqbPlayerFrozen != 0, sqbCameraFrozen != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleHUD(SQVM * pVM)
{
	// togglePlayerHUD(playerid, toggle) : boolean
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerHUD(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleRadar(SQVM * pVM)
{
	// togglePlayerRadar(playerid, toggle) : boolean
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerRadar(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleNames(SQVM * pVM)
{
	// togglePlayerNames(playerid, toggle) : boolean
	// TODO: rename to 'togglePlayerNameTags'
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerNameTags(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleAreaNames(SQVM * pVM)
{
	// togglePlayerAreaNames(playerid, toggle) : boolean
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerAreaNames(playerId, sqbToggle != 0));
	return 1;
}
SQInteger CPlayerNatives::ToggleHelmet(SQVM * pVM)
{
	// togglePlayerHelmet(playerid, toggle) : boolean
	EntityId playerId;
	SQBool sqbToggle;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbToggle);

	sq_pushbool(pVM, StaticNatives::TogglePlayerHelmet(playerId, sqbToggle != 0));
	return 1;
}

// Camera
SQInteger CPlayerNatives::SetCameraBehind(SQVM * pVM)
{
	// setCameraBehindPlayer(playerid) : boolean
	// TODO: remove it, use ResetPlayerCamera! same effect ???
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
		
	sq_pushbool(pVM, StaticNatives::SetPlayerCameraBehind(playerId));
	return 1;
}
SQInteger CPlayerNatives::SetCameraPos(SQVM * pVM)
{
	// setPlayerCameraPos(playerid, x, y, z) : boolean
	EntityId playerId;
	CVector3 vecPos;

	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecPos);

	sq_pushbool(pVM, StaticNatives::SetPlayerCameraPos(playerId, vecPos));
	return 1;
}
SQInteger CPlayerNatives::SetCameraLookAt(SQVM * pVM)
{
	// setPlayerCameraLookAt(playerid, x, y, z) : boolean
	EntityId playerId;
	CVector3 vecLookAt;

	sq_getentity(pVM, -4, &playerId);
	sq_getvector3(pVM, -3, &vecLookAt);

	sq_pushbool(pVM, StaticNatives::SetPlayerCameraLookAt(playerId, vecLookAt));
	return 1;
}
SQInteger CPlayerNatives::ResetCamera(SQVM * pVM)
{
	// resetPlayerCamera(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM, StaticNatives::ResetPlayerCamera(playerId));
	return 1;
}

// Screen text
SQInteger CPlayerNatives::DisplayText(SQVM * pVM)
{
	// displayPlayerText(playerid, x, y, text, time) : boolean
	EntityId playerId;
	float fX, fY;
	const char * szText;
	SQInteger iTime;

	sq_getentity(pVM, -5, &playerId);
	sq_getfloat(pVM, -4, &fX);
	sq_getfloat(pVM, -3, &fY);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	sq_pushbool(pVM, StaticNatives::DisplayPlayerText(playerId, fX, fY, String(szText), iTime));
	return 1;
}
SQInteger CPlayerNatives::DisplayTextForAll(SQVM * pVM)
{
	// displayTextForAll(x, y, text, time) : boolean
	float fX, fY;
	const char * szText;
	SQInteger iTime;

	sq_getfloat(pVM, -4, &fX);
	sq_getfloat(pVM, -3, &fY);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	sq_pushbool(pVM, StaticNatives::DisplayTextForAll(fX, fY, String(szText), iTime));
	return 1;
}
SQInteger CPlayerNatives::DisplayInfoText(SQVM * pVM)
{
	// DisplayPlayerInfoText(playerId, text, time) : boolean
	EntityId playerId;
	const char * szText;
	SQInteger iTime;

	sq_getentity(pVM, -3, &playerId);
	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	sq_pushbool(pVM, StaticNatives::DisplayPlayerInfoText(playerId, String(szText), iTime));
	return 1;
}
SQInteger CPlayerNatives::DisplayInfoTextForAll(SQVM * pVM)
{
	// displayInfoTextForAll(text, time)
	const char * szText;
	SQInteger iTime;

	sq_getstring(pVM, -2, &szText);
	sq_getinteger(pVM, -1, &iTime);

	sq_pushbool(pVM, StaticNatives::DisplayInfoTextForAll(String(szText), iTime));
	return 1;
}

// Other functions
SQInteger CPlayerNatives::SetTime(SQVM * pVM)
{
	// setPlayerTime(playerid, hour, minute) : boolean
	EntityId playerId;
	SQInteger iHour;
	SQInteger iMinute;

	sq_getinteger(pVM, -1, &iMinute);
	sq_getinteger(pVM, -2, &iHour);
	sq_getentity(pVM, -3, &playerId);

	sq_pushbool(pVM, StaticNatives::SetPlayerTime(playerId, (BYTE)iHour, (BYTE)iMinute));
	return 1;
}
SQInteger CPlayerNatives::SetWeather(SQVM * pVM)
{
	// setPlayerWeather(playerid, weatherid) : boolean
	EntityId playerId;
	SQInteger iWeather;

	sq_getinteger(pVM, -1, &iWeather);
	sq_getentity(pVM, -2, &playerId);

	sq_pushbool(pVM, StaticNatives::SetPlayerWeather(playerId, (BYTE)iWeather));
	return 1;
}
SQInteger CPlayerNatives::SetGravity(SQVM * pVM)
{
	// setPlayerGravity(playerid, gravity) : boolean
	// TODO: Doesn't work check rpc handling
	EntityId playerId;
	float fGravity;

	sq_getentity(pVM, -2, &playerId);
	sq_getfloat(pVM, -1, &fGravity);	

	sq_pushbool(pVM, StaticNatives::SetPlayerGravity(playerId, fGravity));
	return 1;
}
SQInteger CPlayerNatives::IsConnected(SQVM * pVM)
{
	// isPlayerConnected(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
	
	sq_pushbool(pVM, StaticNatives::IsPlayerConnected(playerId));
	return 1;
}
SQInteger CPlayerNatives::IsOnFoot(SQVM * pVM)
{
	// isPlayerOnFoot(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM, StaticNatives::IsPlayerOnFoot(playerId));
	return 1;
}
SQInteger CPlayerNatives::IsInVehicle(SQVM * pVM)
{
	// isPlayerInVehicle(playerid, vehicleid) : boolean
	EntityId playerId;
	EntityId vehicleId;

	sq_getentity(pVM, -2, &playerId);
	sq_getentity(pVM, -1, &vehicleId);
	
	sq_pushbool(pVM, StaticNatives::IsPlayerInVehicle(playerId, vehicleId));
	return 1;
}
SQInteger CPlayerNatives::GetVehicleId(SQVM * pVM)
{
	// getPlayerVehicleId(playerid) : entityid
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	EntityId vehicleId = 0;
	if(StaticNatives::GetPlayerVehicleId(playerId, vehicleId))
		sq_pushentity(pVM, vehicleId);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SetSpawnLocation(SQVM * pVM)
{
	// setPlayerSpawnLocation(playerid, x, y, z, fHeading) : boolean
	EntityId playerId;
	CVector3 vecCoords;
	float fHeading;

	sq_getentity(pVM, -5, &playerId);
	sq_getvector3(pVM, -4, &vecCoords);
	sq_getfloat(pVM, -1, &fHeading);	

	sq_pushbool(pVM, StaticNatives::SetPlayerSpawnLocation(playerId, vecCoords, fHeading));
	return 1;
}
SQInteger CPlayerNatives::SetInvincible(SQVM * pVM)
{
	// setPlayerInvincible(playerid, invincible) : boolean
	EntityId playerId;
	SQBool sqbInvincible;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbInvincible);

	sq_pushbool(pVM, StaticNatives::SetPlayerInvincible(playerId, sqbInvincible != 0));
	return 1;
}
SQInteger CPlayerNatives::GetSeatId(SQVM * pVM)
{
	// getPlayerSeatId(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	BYTE byteSeat = 0;
	if(StaticNatives::GetPlayerSeatId(playerId, byteSeat))
		sq_pushinteger(pVM, byteSeat);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::SendMessage(SQVM * pVM)
{
	// sendPlayerMessage(playerid, message [, color = 0xFFFFFFAA, allowformatting = true]) : boolean
	// TODO: clean up. this code can be shorter.
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

	bool bAllowFormatting = (sqbAllowFormatting != 0);
	sq_pushbool(pVM, StaticNatives::SendPlayerMessage(playerId, String(szMessage), (DWORD)iColor, bAllowFormatting));
	return 1;
}
SQInteger CPlayerNatives::SendMessageToAll(SQVM * pVM)
{
	// sendMessageToAll(message [, color = 0xFFFFFFAA, allowformatting = true]) : boolean
	// TODO: clean up. this code can be shorter
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

	sq_pushbool(pVM, StaticNatives::SendMessageToAll(String(szMessage), (DWORD)iColor, sqbAllowFormatting != 0));
	return 1;
}
SQInteger CPlayerNatives::IsInAnyVehicle(SQVM * pVM)
{
	// isPlayerInAnyVehicle(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM, StaticNatives::IsPlayerInAnyVehicle(playerId));
	return 1;
}
SQInteger CPlayerNatives::IsSpawned(SQVM * pVM)
{
	// isPlayerSpawned(playerid) : boolean
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	sq_pushbool(pVM, StaticNatives::IsPlayerSpawned(playerId));
	return 1;
}
SQInteger CPlayerNatives::Kick(SQVM * pVM)
{
	// kickPlayer(playerid, bSendKickMessage) : boolean
	// TODO: Support for 1-argument only syntax
	EntityId playerId;
	SQBool sqbKickMessage;

	sq_getentity(pVM, -2, &playerId);
	sq_getbool(pVM, -1, &sqbKickMessage);

	sq_pushbool(pVM, StaticNatives::KickPlayer(playerId, sqbKickMessage != 0));
	return 1;
}
SQInteger CPlayerNatives::Ban(SQVM * pVM)
{
	// banPlayer(playerid, uiMilliseconds) : boolean
	EntityId playerId;
	SQInteger iSeconds;

	sq_getentity(pVM, -2, &playerId);
	sq_getinteger(pVM, -1, &iSeconds);

	sq_pushbool(pVM, StaticNatives::BanPlayer(playerId, iSeconds));
	return 1;
}
SQInteger CPlayerNatives::GetIp(SQVM * pVM)
{
	// getPlayerIp(playerid) : string
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	String strIp;
	if(StaticNatives::GetPlayerIp(playerId, strIp))
		sq_pushstring(pVM, strIp.Get(), strIp.GetLength());
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::GetState(SQVM * pVM)
{
	// getPlayerState(playerid) : integer
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	eStateType state;
	if(StaticNatives::GetPlayerState(playerId, state))
		sq_pushinteger(pVM, state);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::WarpIntoVehicle(SQVM * pVM)
{
	// warpPlayerIntoVehicle(playerId, vehicleId [, seatId = 0]) : boolean
	// TODO: clean up this code. it can be shorter
	// TODO: 'animated' parameter (and reverse it)
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

	sq_pushbool(pVM, StaticNatives::WarpPlayerIntoVehicle(playerId, vehicleId, (BYTE)iSeatId));
	return 1;
}
SQInteger CPlayerNatives::RemoveFromVehicle(SQVM * pVM)
{
	// removePlayerFromVehicle(playerid)
	// TODO: Clean up this code (can be shorter)
	CHECK_PARAMS_MIN("removePlayerFromVehicle", 1);
	CHECK_TYPE("removePlayerFromVehicle", 1, 2, OT_INTEGER);

	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	if(!g_pPlayerManager->DoesExist(playerId))
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	bool bAnimated = false;
	if(sq_gettop(pVM) >= 3)
	{
		CHECK_TYPE("removePlayerFromVehicle", 2, 3, OT_BOOL);
		SQBool sqbAnimated;
		sq_getbool(pVM, 3, &sqbAnimated);
		bAnimated = (sqbAnimated != 0);
	}
		
	sq_pushbool(pVM, StaticNatives::RemovePlayerFromVehicle(playerId, bAnimated));
	return 1;
}
SQInteger CPlayerNatives::GetSerial(SQVM * pVM)
{
	// getPlayerSerial(playerid) : string
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	String strSerial;
	if(StaticNatives::GetPlayerSerial(playerId, strSerial))
		sq_pushstring(pVM, strSerial.Get(), -1);
	else
		sq_pushbool(pVM, false);
	return 1;
}
SQInteger CPlayerNatives::TriggerClientEvent(SQVM * pVM)
{
	// triggerClientEvent(playerid, eventname [, ...]) : boolean
	CHECK_PARAMS_MIN("triggerClientEvent", 2);
	CHECK_TYPE("triggerClientEvent", 1, 2, OT_INTEGER);
	CHECK_TYPE("triggerClientEvent", 2, 3, OT_STRING);

	SQInteger playerId;
	sq_getinteger(pVM, 2, &playerId);
	CSquirrelArguments sqArgs(pVM, 3);
	
	sq_pushbool(pVM, StaticNatives::TriggerPlayerClientEvent(playerId, sqArgs));
	return 1;
}
SQInteger CPlayerNatives::GetPing(SQVM* pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	unsigned short usPing;
	if(StaticNatives::GetPlayerPing(playerId, usPing))
		sq_pushinteger(pVM, usPing);
	else
		sq_pushbool(pVM, false);
	return 1;
}

// TODO: Cleanup!
SQInteger CPlayerNatives::GetEmptyControlState(SQVM * pVM)
{
	// getEmptyPlayerControlState()
	// TODO: Clean up/remove this messed code.
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
SQInteger CPlayerNatives::GetPreviousControlState(SQVM * pVM)
{
	// getPlayerPreviousControlState(playerid)
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
SQInteger CPlayerNatives::GetControlState(SQVM * pVM)
{
	// getPlayerControlState(playerid)
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