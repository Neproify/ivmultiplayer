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
}

// isPlayerConnected(playerid)
SQInteger CPlayerNatives::IsConnected(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);
	sq_pushbool(pVM, g_pPlayerManager->DoesExist(iPlayerId));
	return 1;
}

// getLocalPlayer()
SQInteger CPlayerNatives::GetLocal(SQVM * pVM)
{
	sq_pushinteger(pVM, g_pLocalPlayer->GetPlayerId());
	return 1;
}

// getPlayerName(playerid)
SQInteger CPlayerNatives::GetName(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushstring(pVM, pPlayer->GetName(), -1);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHealth(playerid)
SQInteger CPlayerNatives::GetHealth(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, (pPlayer->GetHealth() - 100));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerArmour(playerid)
SQInteger CPlayerNatives::GetArmour(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetArmour());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerCoordinates(playerid)
SQInteger CPlayerNatives::GetCoordinates(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

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
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVelocity(playerid)
SQInteger CPlayerNatives::GetVelocity(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

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
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerInAnyVehicle(playerid)
SQInteger CPlayerNatives::IsInAnyVehicle(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushbool(pVM, pPlayer->IsInVehicle());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerInVehicle(playerid, vehicleid)
SQInteger CPlayerNatives::IsInVehicle(SQVM * pVM)
{
	int iPlayerId;
	int iVehicleId;
	sq_getinteger(pVM, -2, &iPlayerId);
	sq_getinteger(pVM, -1, &iVehicleId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushbool(pVM, (pPlayer->GetVehicle()->GetVehicleId() == iVehicleId));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVehicleId(playerid)
SQInteger CPlayerNatives::GetVehicleId(SQVM * pVM)
{
	SQInteger iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = pPlayer->GetVehicle();

		if(pVehicle)
		{
			sq_pushinteger(pVM, pVehicle->GetVehicleId());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSeatId(playerid)
SQInteger CPlayerNatives::GetSeatId(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetVehicleSeatId());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerOnFoot(playerid)
SQInteger CPlayerNatives::IsOnFoot(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushbool(pVM, !pPlayer->IsInVehicle());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerModel(playerid)
SQInteger CPlayerNatives::GetModel(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, ModelHashToSkinId(pPlayer->GetModelInfo()->GetHash()));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isPlayerSpawned(playerid)
SQInteger CPlayerNatives::IsSpawned(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushbool(pVM, pPlayer->IsSpawned());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHeading(playerid)
SQInteger CPlayerNatives::GetHeading(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushfloat(pVM, pPlayer->GetCurrentHeading());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerMoney(playerid)
SQInteger CPlayerNatives::GetMoney(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetMoney());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerState(playerid)
SQInteger CPlayerNatives::GetState(SQVM * pVM)
{
	// TODO: Re-enable this
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer && !pPlayer->IsLocalPlayer())
		{
			CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer*>(pPlayer);
			if(pRemotePlayer)
			{
				sq_pushinteger(pVM, pRemotePlayer->GetStateType());
				return 1;
			}
		}
	}
	else if(iPlayerId >= 0 && iPlayerId < MAX_PLAYERS)
	{
		sq_pushinteger(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerWeapon(playerid)
SQInteger CPlayerNatives::GetWeapon(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetCurrentWeapon());
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerAmmo(playerid)
SQInteger CPlayerNatives::GetAmmo(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetAmmo(pPlayer->GetCurrentWeapon()));
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerInterior(playerid)
SQInteger CPlayerNatives::GetInterior(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, -1, &iPlayerId);

	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(iPlayerId);

		if(pPlayer)
		{
			sq_pushinteger(pVM, pPlayer->GetInterior());
			return 1;
		}
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

		// Create the table
		sq_newtable(pVM);

		// Create the 'onFootMove' array
		sq_pushstring(pVM, "onFootMove", -1);
		sq_newarray(pVM, 0);

		for(int i = 0; i < 4; i++)
		{
			sq_pushinteger(pVM, padState.ucOnFootMove[i]);
			sq_arrayappend(pVM, -2);
		}

		sq_createslot(pVM, -3);

		// Create the 'inVehicleMove' array
		sq_pushstring(pVM, "inVehicleMove", -1);
		sq_newarray(pVM, 0);

		for(int i = 0; i < 4; i++)
		{
			sq_pushinteger(pVM, padState.ucInVehicleMove[i]);
			sq_arrayappend(pVM, -2);
		}

		sq_createslot(pVM, -3);

		// Create the on foot keys slots
		sq_pushstring(pVM, "enterExitVehicle", -1);
		sq_pushbool(pVM, padState.keys.bEnterExitVehicle);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "sprint", -1);
		sq_pushbool(pVM, padState.keys.bSprint);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "jump", -1);
		sq_pushbool(pVM, padState.keys.bJump);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "attack", -1);
		sq_pushbool(pVM, padState.keys.bAttack);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "attack2", -1);
		sq_pushbool(pVM, padState.keys.bAttack2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "aim", -1);
		sq_pushbool(pVM, padState.keys.bAim);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "freeAim", -1);
		sq_pushbool(pVM, padState.keys.bFreeAim);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeAttack1", -1);
		sq_pushbool(pVM, padState.keys.bMeleeAttack1);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeAttack2", -1);
		sq_pushbool(pVM, padState.keys.bMeleeAttack2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeKick", -1);
		sq_pushbool(pVM, padState.keys.bMeleeKick);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeBlock", -1);
		sq_pushbool(pVM, padState.keys.bMeleeBlock);
		sq_createslot(pVM, -3);

		// Create the in vehicle keys slots
		sq_pushstring(pVM, "accelerate", -1);
		sq_pushbool(pVM, padState.keys.bAccelerate);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "brake", -1);
		sq_pushbool(pVM, padState.keys.bBrake);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "handbrake", -1);
		sq_pushbool(pVM, padState.keys.bHandbrake);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "handbrake2", -1);
		sq_pushbool(pVM, padState.keys.bHandbrake2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "horn", -1);
		sq_pushbool(pVM, padState.keys.bHorn);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "driveBy", -1);
		sq_pushbool(pVM, padState.keys.bDriveBy);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "heliPrimaryFire", -1);
		sq_pushbool(pVM, padState.keys.bHeliPrimaryFire);
		sq_createslot(pVM, -3);
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

		// Create the table
		sq_newtable(pVM);

		// Create the 'onFootMove' array
		sq_pushstring(pVM, "onFootMove", -1);
		sq_newarray(pVM, 0);

		for(int i = 0; i < 4; i++)
		{
			sq_pushinteger(pVM, padState.ucOnFootMove[i]);
			sq_arrayappend(pVM, -2);
		}

		sq_createslot(pVM, -3);

		// Create the 'inVehicleMove' array
		sq_pushstring(pVM, "inVehicleMove", -1);
		sq_newarray(pVM, 0);

		for(int i = 0; i < 4; i++)
		{
			sq_pushinteger(pVM, padState.ucInVehicleMove[i]);
			sq_arrayappend(pVM, -2);
		}

		sq_createslot(pVM, -3);

		// Create the on foot keys slots
		sq_pushstring(pVM, "enterExitVehicle", -1);
		sq_pushbool(pVM, padState.keys.bEnterExitVehicle);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "sprint", -1);
		sq_pushbool(pVM, padState.keys.bSprint);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "jump", -1);
		sq_pushbool(pVM, padState.keys.bJump);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "attack", -1);
		sq_pushbool(pVM, padState.keys.bAttack);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "attack2", -1);
		sq_pushbool(pVM, padState.keys.bAttack2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "aim", -1);
		sq_pushbool(pVM, padState.keys.bAim);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "freeAim", -1);
		sq_pushbool(pVM, padState.keys.bFreeAim);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeAttack1", -1);
		sq_pushbool(pVM, padState.keys.bMeleeAttack1);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeAttack2", -1);
		sq_pushbool(pVM, padState.keys.bMeleeAttack2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeKick", -1);
		sq_pushbool(pVM, padState.keys.bMeleeKick);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "meleeBlock", -1);
		sq_pushbool(pVM, padState.keys.bMeleeBlock);
		sq_createslot(pVM, -3);

		// Create the in vehicle keys slots
		sq_pushstring(pVM, "accelerate", -1);
		sq_pushbool(pVM, padState.keys.bAccelerate);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "brake", -1);
		sq_pushbool(pVM, padState.keys.bBrake);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "handbrake", -1);
		sq_pushbool(pVM, padState.keys.bHandbrake);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "handbrake2", -1);
		sq_pushbool(pVM, padState.keys.bHandbrake2);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "horn", -1);
		sq_pushbool(pVM, padState.keys.bHorn);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "driveBy", -1);
		sq_pushbool(pVM, padState.keys.bDriveBy);
		sq_createslot(pVM, -3);
		sq_pushstring(pVM, "heliPrimaryFire", -1);
		sq_pushbool(pVM, padState.keys.bHeliPrimaryFire);
		sq_createslot(pVM, -3);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPing(playerid)
SQInteger CPlayerNatives::GetPing(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, 2, &iPlayerId);
	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		sq_pushinteger(pVM, g_pPlayerManager->GetAt(iPlayerId)->GetPing());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerColor(playerid)
SQInteger CPlayerNatives::GetColor(SQVM * pVM)
{
	int iPlayerId;
	sq_getinteger(pVM, 2, &iPlayerId);
	if(g_pPlayerManager->DoesExist(iPlayerId))
	{
		sq_pushinteger(pVM, g_pPlayerManager->GetAt(iPlayerId)->GetColor());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
