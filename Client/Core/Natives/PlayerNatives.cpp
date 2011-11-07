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
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "1");
	pScriptingManager->RegisterFunction("getPlayerMoney", GetMoney, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerState", GetState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWeapon", GetWeapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", GetAmmo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerInterior", GetInterior, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", GetPadState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", GetPreviousPadState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", GetPing, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerDucking", IsDucking, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerJackingAVehicle", IsJackingAVehicle, 1, "i");
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
	sq_pushentity(pVM, g_pLocalPlayer->GetPlayerId());
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

// getPlayerPadState(playerid)
SQInteger CPlayerNatives::GetPadState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player pad state
		CPadState padState;
		pPlayer->GetPadState(&padState);

		// Create the table and array
		CSquirrelArguments table;
		CSquirrelArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(padState.ucOnFootMove[i]);

		table.push(array, true);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(padState.ucInVehicleMove[i]);

		table.push(array, true);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
			array.push(padState.ucInVehicleTriggers[i]);

		table.push(array, true);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(padState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(padState.keys.bSprint);
		table.push("jump");
		table.push(padState.keys.bJump);
		table.push("attack");
		table.push(padState.keys.bAttack);
		table.push("attack2");
		table.push(padState.keys.bAttack2);
		table.push("aim");
		table.push(padState.keys.bAim);
		table.push("freeAim");
		table.push(padState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(padState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(padState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(padState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(padState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(padState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(padState.keys.bHandbrake2);
		table.push("horn");
		table.push(padState.keys.bHorn);
		table.push("driveBy");
		table.push(padState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(padState.keys.bHeliPrimaryFire);

		// Push the table to the VM
		sq_pusharg(pVM, CSquirrelArgument(table, false));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPreviousPadState(playerid)
SQInteger CPlayerNatives::GetPreviousPadState(SQVM * pVM)
{
	EntityId playerId;
	sq_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player pad state
		CPadState padState;
		pPlayer->GetPreviousPadState(&padState);

		// Create the table and array
		CSquirrelArguments table;
		CSquirrelArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(padState.ucOnFootMove[i]);

		table.push(array, true);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
			array.push(padState.ucInVehicleMove[i]);

		table.push(array, true);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
			array.push(padState.ucInVehicleTriggers[i]);

		table.push(array, true);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(padState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(padState.keys.bSprint);
		table.push("jump");
		table.push(padState.keys.bJump);
		table.push("attack");
		table.push(padState.keys.bAttack);
		table.push("attack2");
		table.push(padState.keys.bAttack2);
		table.push("aim");
		table.push(padState.keys.bAim);
		table.push("freeAim");
		table.push(padState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(padState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(padState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(padState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(padState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(padState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(padState.keys.bHandbrake2);
		table.push("horn");
		table.push(padState.keys.bHorn);
		table.push("driveBy");
		table.push(padState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(padState.keys.bHeliPrimaryFire);

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
