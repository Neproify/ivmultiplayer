//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "Squirrel/sqstate.h"
#include "Squirrel/sqvm.h"
#include "Squirrel/sqstring.h"
#include "../CNetworkManager.h"
#include "../CVehicleManager.h"
#include "../CPlayerManager.h"
#include "../CRemotePlayer.h"
#include "../CNetworkManager.h"
#include "../CChatWindow.h"
#include "../CLocalPlayer.h"
#include "../CGUI.h"

extern CPlayerManager  * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CChatWindow     * g_pChatWindow;
extern CLocalPlayer    * g_pLocalPlayer;
extern CGUI            * g_pGUI;

// Player functions

void CPlayerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isPlayerConnected", IsConnected, 1, "i");
	pScriptingManager->RegisterFunction("getLocalPlayer", GetLocal, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerName", GetName, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerArmour", GetArmour, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInAnyVehicle", IsInAnyVehicle, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInVehicle", IsInVehicle, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerVehicleId", GetVehicleId, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSeatId", GetSeatId, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerOnFoot", IsOnFoot, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerMoney", GetMoney, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerState", GetState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWeapon", GetWeapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", GetAmmo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerInterior", GetInterior, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerControlState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPreviousControlState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", GetPing, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerDucking", IsDucking, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerJackingAVehicle", IsJackingAVehicle, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWeaponSlot", GetWeaponSlot, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerAmmoInClip", GetAmmoInClip, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerDoorLockState", SetDoorLockState, 7, "isfffbf");
	pScriptingManager->RegisterFunction("switchPlayerPhysics", TogglePhysics, 3, "bbb");
	pScriptingManager->RegisterFunction("getWayPointCoordinates", GetWayPointCoords, 0, NULL);
}

// isPlayerConnected(playerid)
SQInteger CPlayerNatives::IsConnected(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);
	sq_pushbool(pVM, g_pPlayerManager->DoesExist(playerId));
	return 1;
}

// getLocalPlayer()
SQInteger CPlayerNatives::GetLocal(SQVM * pVM)
{
	if(g_pLocalPlayer->IsNetworkPlayer())
		sq_pushentity(pVM, g_pLocalPlayer->GetPlayerId());
	else
		sq_pushinteger(pVM,-1);
	
	return 1;
}

// getPlayerName(playerid)
SQInteger CPlayerNatives::GetName(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushstring(pVM, pPlayer->GetName(), -1);
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, (pPlayer->GetHealth() - 100));
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetArmour());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

// getPlayerVelocity(playerid)
SQInteger CPlayerNatives::GetVelocity(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

// isPlayerInAnyVehicle(playerid)
SQInteger CPlayerNatives::IsInAnyVehicle(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = pPlayer->GetVehicle();

		if(pVehicle)
		{
			sq_pushbool(pVM, (pVehicle->GetVehicleId() == vehicleId));
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = pPlayer->GetVehicle();

		if(pVehicle)
			sq_pushentity(pVM, pVehicle->GetVehicleId());
		else
			sq_pushentity(pVM, INVALID_ENTITY_ID);

		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSeatId(playerid)
SQInteger CPlayerNatives::GetSeatId(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsOnFoot());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, ModelHashToSkinId(pPlayer->GetModelInfo()->GetHash()));
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsSpawned());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushfloat(pVM, pPlayer->GetCurrentHeading());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetMoney());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

		if(pRemotePlayer)
		{
			sq_pushinteger(pVM, pRemotePlayer->GetStateType());
			return 1;
		}
	}
	else if(playerId < MAX_PLAYERS)
	{
		sq_pushinteger(pVM, STATE_TYPE_DISCONNECT);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerWeapon(playerid)
SQInteger CPlayerNatives::GetWeapon(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetCurrentWeapon());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//getPlayerAmmoInClip(playerid, weaponId)
SQInteger CPlayerNatives::GetAmmoInClip(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	int iWeaponId;
	sq_getinteger(pVM, -1, &iWeaponId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetAmmoInClip(iWeaponId));
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetAmmo(pPlayer->GetCurrentWeapon()));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerInterior(playerid)
SQInteger CPlayerNatives::GetInterior(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetInterior());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

// getPlayerPreviousControlState(playerid)
SQInteger CPlayerNatives::GetPreviousControlState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

// getPlayerPing(playerid)
SQInteger CPlayerNatives::GetPing(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	if(g_pLocalPlayer->GetPlayerId() == playerId) {
		sq_pushinteger(pVM,g_pLocalPlayer->GetPing());
		return 1;
	}

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetPing());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerColor(playerid)
SQInteger CPlayerNatives::GetColor(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, 2, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushinteger(pVM, pPlayer->GetColor());
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

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsDucking());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerJackingAVehicle(playerid)
SQInteger CPlayerNatives::IsJackingAVehicle(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		sq_pushbool(pVM, pPlayer->IsJackingAVehicle());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerWeaponSlots(playerid, slot)
SQInteger CPlayerNatives::GetWeaponSlot(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -2, &playerId);

	int iSlot;
	sq_getinteger(pVM, -1, &iSlot);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		unsigned int uiWeap, uiAmmo, uiUnknown;
		CSquirrelArguments args;
 		pPlayer->GetWeaponInSlot(iSlot, uiWeap, uiAmmo, uiUnknown);
		args.push((int)uiWeap);
		args.push((int)uiAmmo);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::SetDoorLockState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -7, &playerId);

	const char *szString;
	sq_getstring(pVM, -6, &szString);

	CVector3 vecPos;
	sq_getfloat(pVM, -5, &vecPos.fX);
	sq_getfloat(pVM, -4, &vecPos.fY);
	sq_getfloat(pVM, -3, &vecPos.fZ);

	SQBool bState;
	sq_getbool(pVM, -2, &bState);
	bool bToggle = (bState != 0);

	float fSwing;
	sq_getfloat(pVM, -1, &fSwing);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		DWORD dwHash = Scripting::GetHashKey(szString);
		Scripting::SetStateOfClosestDoorOfType(dwHash, vecPos.fX, vecPos.fY, vecPos.fZ, bToggle, fSwing);
		sq_pushbool(pVM,true);
		return 1;
	}
	sq_pushbool(pVM,false);
	return 1;
}

SQInteger CPlayerNatives::TogglePhysics(SQVM * pVM)
{
	SQBool bInvincible = false;
	SQBool bWindScreen = false;
	SQBool bNockedOffBike = false;

	if(g_pLocalPlayer && g_pLocalPlayer->IsSpawned()) {
		bool bToggle = (bInvincible != 0);
		Scripting::SetCharInvincible(g_pLocalPlayer->GetScriptingHandle(), bToggle);
		bToggle = (bWindScreen != 0);
		Scripting::SetCharWillFlyThroughWindscreen(g_pLocalPlayer->GetScriptingHandle(), bToggle);
		bToggle = (bNockedOffBike != 0);
		Scripting::SetCharCanBeKnockedOffBike(g_pLocalPlayer->GetScriptingHandle(), bToggle);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CPlayerNatives::GetWayPointCoords(SQVM * pVM)
{
	if(g_pLocalPlayer && g_pLocalPlayer->IsSpawned()) {
		unsigned int uiWayPointHandle = -1;
		Scripting::GetFirstBlipInfoId(Scripting::BLIP_WAYPOINT);
		if(uiWayPointHandle != -1) {
			CVector3 vecPos;
			Scripting::GetBlipCoords(uiWayPointHandle, &vecPos);
			if(vecPos.fZ == 0.0)
				Scripting::GetGroundZFor3DCoord(vecPos.fX, vecPos.fY, 1000, &vecPos.fZ);
			CSquirrelArguments args;
			args.push(vecPos.fX);
			args.push(vecPos.fY);
			args.push(vecPos.fZ);
			sq_pusharg(pVM, CSquirrelArgument(args, true));
			return 1;
		}
		sq_pushbool(pVM, false);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}