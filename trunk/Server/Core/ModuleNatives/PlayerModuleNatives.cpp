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

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "Squirrel/sqstring.h"
#include "../CVehicleManager.h"
#include "../CPlayerManager.h"
#include "../CNetworkManager.h"
#include <Game/CTime.h>
#include "CEvents.h"
#include <stdarg.h>

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;

// Player functions
namespace Modules
{

	// isPlayerConnected(playerid)
	bool CPlayerModuleNatives::IsConnected(EntityId playerId)
	{
		return g_pPlayerManager->DoesExist(playerId);
		return 1;
	}

	// getPlayerName(playerid)
	const char * CPlayerModuleNatives::GetName(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetName().CopyData();
		}

		return NULL;
	}

	bool CPlayerModuleNatives::SetName(EntityId playerId, const char * szName)
	{
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
				return false;
			} 

			return pPlayer->SetName(szName);
		}

		return false;
	}

	// givePlayerWeapon(playerid, weaponid, ammo)
	bool CPlayerModuleNatives::GiveWeapon(EntityId playerId, int iWeaponId, int iAmmo)
	{
		if(g_pPlayerManager->DoesExist(playerId) && iWeaponId > 0 && iWeaponId < 21 && iWeaponId != 6)
		{
			CBitStream bsSend;
			bsSend.Write(iWeaponId);
			bsSend.Write(iAmmo);
			g_pNetworkManager->RPC(RPC_ScriptingGivePlayerWeapon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// removePlayerWeapons(playerid)
	bool CPlayerModuleNatives::RemoveWeapons(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			g_pNetworkManager->RPC(RPC_ScriptingRemoveWeapons, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	//setPlayerWantedLevel(playerid, wantedlevel)
	bool CPlayerModuleNatives::SetWantedLevel(EntityId playerId, int iWantedLevel)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iWantedLevel);
			g_pNetworkManager->RPC(RPC_ScriptingSetWantedLevel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return true;
	}

	// setPlayerHealth(playerid, health)
	bool CPlayerModuleNatives::SetHealth(EntityId playerId, int iHealth)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iHealth);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerHealth, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerHealth(playerid)
	int CPlayerModuleNatives::GetHealth(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return (pPlayer->GetHealth() - 100);
		}

		return -1;
	}

	// setPlayerArmour(playerid)
	bool CPlayerModuleNatives::SetArmour(EntityId playerId, int iArmour)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iArmour);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerArmour, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerArmour(playerid)
	int CPlayerModuleNatives::GetArmour(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetArmour();
		}

		return -1;
	}

	// setPlayerCoordinates(playerid, x, y, z)
	bool CPlayerModuleNatives::SetCoordinates(EntityId playerId, CVector3 vecPos)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(vecPos);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerCoordinates(playerid)
	CVector3 CPlayerModuleNatives::GetCoordinates(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			CVector3 vecPosition;
			pPlayer->GetPosition(vecPosition);
			return vecPosition;
		}

		return CVector3();
	}

	// setPlayerTime(playerid, hour, minute)
	bool CPlayerModuleNatives::SetTime(EntityId playerId, unsigned char iHour, unsigned char iMinute)
	{
		if(g_pPlayerManager->DoesExist(playerId) && (iHour >= 0 && iHour < 24) && (iMinute >= 0 && iMinute < 60))
		{
			CBitStream bsSend;
			bsSend.Write((unsigned char)iHour);
			bsSend.Write((unsigned char)iMinute);
			if(g_pTime->GetMinuteDuration() != CTime::DEFAULT_MINUTE_DURATION)
				bsSend.Write(g_pTime->GetMinuteDuration());

			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerTime, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// setPlayerTime(playerid, weather)
	bool CPlayerModuleNatives::SetWeather(EntityId playerId, unsigned char iWeather)
	{
		if(g_pPlayerManager->DoesExist(playerId) && (iWeather >= 1 && iWeather < 10))
		{
			CBitStream bsSend;
			bsSend.Write((unsigned char)iWeather);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// setPlayerGravity(playerid, gravity)
	bool CPlayerModuleNatives::SetGravity(EntityId playerId, float fGravity)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(fGravity);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerGravity, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// sendPlayerMessage(playerid, message [, color = 0xFFFFFFAA, allowformatting = true])
	bool CPlayerModuleNatives::SendMessage(EntityId playerId, const char * szMessage, int iColor, bool sqbAllowFormatting)
	{

		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write((DWORD)iColor);
			bsSend.Write(String(szMessage));
			bool bAllowFormatting = (sqbAllowFormatting != 0);
			bsSend.Write(bAllowFormatting);
			g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// sendMessageToAll(message [, color = 0xFFFFFFAA, allowformatting = true])
	bool CPlayerModuleNatives::SendMessageToAll(const char * szMessage, int iColor, bool sqbAllowFormatting)
	{
		if(g_pPlayerManager->GetPlayerCount() > 0)
		{
			CBitStream bsSend;
			bsSend.Write((DWORD)iColor);
			bsSend.Write(String(szMessage));
			bool bAllowFormatting = (sqbAllowFormatting != 0);
			bsSend.Write(bAllowFormatting);
			g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return true;
		}

		return false;
	}

	// isPlayerInAnyVehicle(playerid)
	bool CPlayerModuleNatives::IsInAnyVehicle(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->IsInVehicle();
		}

		return false;
	}

	// isPlayerInVehicle(playerid, vehicleid)
	bool CPlayerModuleNatives::IsInVehicle(EntityId playerId, EntityId vehicleId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			if(pPlayer->IsInVehicle())
			{
				return (pPlayer->GetVehicle()->GetVehicleId() == vehicleId);
			}
		}

		return false;
	}

	// getPlayerVehicleId(playerid)
	EntityId CPlayerModuleNatives::GetVehicleId(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			if(pPlayer->IsInVehicle())
			{
				return pPlayer->GetVehicle()->GetVehicleId();
				return 1;
			}
		}

		return -1;
	}

	// getPlayerSeatId(playerid)
	int CPlayerModuleNatives::GetSeatId(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer && pPlayer->IsInVehicle())
		{
			return pPlayer->GetVehicleSeatId();
		}

		return -1;
	}

	// isPlayerOnFoot(playerid)
	bool CPlayerModuleNatives::IsOnFoot(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->IsOnFoot();
		}

		return false;
	}

	// togglePlayerPayAndSpray(playerid, toggle)
	bool CPlayerModuleNatives::TogglePayAndSpray(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
			return true;
		}

		return false;
	}

	// togglePlayerAutoAim(playerid, toggle)
	bool CPlayerModuleNatives::ToggleAutoAim(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
			return true;
		}

		return false;
	}

	// setPlayerSpawnLocation(playerid, x, y, z, r)
	bool CPlayerModuleNatives::SetSpawnLocation(EntityId playerId, CVector3 vecPos, float fRotation)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->SetSpawnLocation(vecPos, fRotation);
			return true;
		}

		return false;
	}

	// setPlayerModel(playerid, model)
	bool CPlayerModuleNatives::SetModel(EntityId playerId, int iModelId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->SetModel(iModelId);
		}

		return false;
	}

	// getPlayerModel(playerid)
	int CPlayerModuleNatives::GetModel(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetModel();
		}

		return -1;
	}

	// togglePlayerControls(playerid, toggle)
	bool CPlayerModuleNatives::ToggleControls(EntityId playerId, bool sqbControls)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bControls = (sqbControls != 0);
			bsSend.Write(bControls);
			g_pNetworkManager->RPC(RPC_ScriptingToggleControls, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// isPlayerSpawned(playerid)
	bool CPlayerModuleNatives::IsSpawned(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->IsSpawned();
		}

		return false;
	}

	// setPlayerHeading(playerid, heading)
	bool CPlayerModuleNatives::SetHeading(EntityId playerId, float fHeading)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(fHeading);
			g_pNetworkManager->RPC(RPC_ScriptingSetHeading, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerHeading(playerid)
	float CPlayerModuleNatives::GetHeading(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetCurrentHeading();
		}

		return -1.0f;
	}

	// togglePlayerPhysics(playerid, toggle)
	bool CPlayerModuleNatives::TogglePhysics(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleRagdoll, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// kickPlayer(playerid, sendkickmessage)
	bool CPlayerModuleNatives::Kick(EntityId playerId, bool sqbKickMessage)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->Kick(sqbKickMessage != 0);
			return true;
		}

		return false;
	}

	// banPlayer(playerid, milliseconds)
	bool CPlayerModuleNatives::Ban(EntityId playerId, unsigned int uiSeconds)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->Ban(uiSeconds);
			return true;
		}

		return false;
	}

	// getPlayerIp(playerid)
	const char * CPlayerModuleNatives::GetIp(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetIp().CopyData();
		}

		return NULL;
	}

	// givePlayerMoney(playerid, money)
	bool CPlayerModuleNatives::GiveMoney(EntityId playerId, int iMoney)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GiveMoney(iMoney);
		}

		return false;
	}

	// setPlayerMoney(playerid, money)
	bool CPlayerModuleNatives::SetMoney(EntityId playerId, int iMoney)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->SetMoney(iMoney);
		}

		return false;
	}

	// resetPlayerMoney(playerid)
	bool CPlayerModuleNatives::ResetMoney(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->SetMoney(0);
		}

		return false;
	}

	// getPlayerMoney(playerid)
	int CPlayerModuleNatives::GetMoney(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetMoney();
		}

		return -1;
	}


	// togglePlayerFrozen(playerid, playerfrozen, camerafrozen)
	bool CPlayerModuleNatives::ToggleFrozen(EntityId playerId, bool sqbPlayerFrozen, bool sqbCameraFrozen)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(sqbPlayerFrozen);
			bsSend.Write(sqbCameraFrozen);
			g_pNetworkManager->RPC(RPC_ScriptingToggleFrozen, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerState(playerid)
	int CPlayerModuleNatives::GetState(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

			if(pPlayer)
			{
				return pPlayer->GetState();
			}
		}
		else if(playerId >= 0 && playerId < MAX_PLAYERS)
		{
			return STATE_TYPE_DISCONNECT;
		}

		return -1;
	}

	// displayPlayerText(playerid, x, y, text, time)
	bool CPlayerModuleNatives::DisplayText(EntityId playerId, float fPosX, float fPosY, const char * szText, int iTime)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(fPosX);
			bsSend.Write(fPosY);
			bsSend.Write(String(szText));
			bsSend.Write(iTime);
			g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// displayTextForAll(x, y, text, time)
	bool CPlayerModuleNatives::DisplayTextForAll(float fPosX, float fPosY, const char * szText, int iTime)
	{
		CBitStream bsSend;
		bsSend.Write(fPosX);
		bsSend.Write(fPosY);
		bsSend.Write(String(szText));
		bsSend.Write(iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		return true;
	}

	// displayInfoText(playerid, text, time)
	bool CPlayerModuleNatives::DisplayInfoText(EntityId playerId, const char * szText, int iTime)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(String(szText));
			bsSend.Write(iTime);
			g_pNetworkManager->RPC(RPC_ScriptingDisplayInfoText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// displayInfoTextForAll(playerid, text, time)
	bool CPlayerModuleNatives::DisplayInfoTextForAll(const char * szText, int iTime)
	{
		CBitStream bsSend;
		bsSend.Write(String(szText));
		bsSend.Write(iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		return true;
	}

	// setPlayerVelocity(playerid, x, y, z)
	bool CPlayerModuleNatives::SetVelocity(EntityId playerId, CVector3 vecMoveSpeed)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(vecMoveSpeed);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerMoveSpeed, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getPlayerVelocity(playerid)
	CVector3 CPlayerModuleNatives::GetVelocity(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			CVector3 vecMoveSpeed;
			pPlayer->GetMoveSpeed(vecMoveSpeed);
			return vecMoveSpeed;
		}

		return CVector3();
	}

	// warpPlayerIntoVehicle(playerid, vehicleid, seatid = 0)
	bool CPlayerModuleNatives::WarpIntoVehicle(EntityId playerId, EntityId vehicleId, int iSeatId)
	{
		if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(vehicleId))
		{
			CBitStream bsSend;
			bsSend.Write((EntityId)vehicleId);
			bsSend.Write((BYTE)iSeatId);
			g_pNetworkManager->RPC(RPC_ScriptingWarpPlayerIntoVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// removePlayerFromVehicle(playerid)
	bool CPlayerModuleNatives::RemoveFromVehicle(EntityId playerId, bool bGraceful)
	{
		if(!g_pPlayerManager->DoesExist(playerId))
		{
			return false;
		}

		CBitStream bsSend;
		bsSend.WriteBit(bGraceful);
		g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerFromVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

		return true;
	}

	// getPlayerWeapon(playerid)
	int CPlayerModuleNatives::GetWeapon(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetWeapon();
		}

		return -1;
	}

	// getPlayerAmmo(playerid)
	int CPlayerModuleNatives::GetAmmo(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetAmmo();
		}

		return -1;
	}

	// getPlayerSerial(playerid)
	const char * CPlayerModuleNatives::GetSerial(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(!pPlayer)
		{
			return NULL;
		}

		return pPlayer->GetSerial().CopyData();
	}

	// setCameraBehindPlayer(playerid)
	bool CPlayerModuleNatives::SetCameraBehind(EntityId playerId)
	{
		if(!g_pPlayerManager->DoesExist(playerId))
		{
			return false;
		}

		CBitStream bsSend;
		g_pNetworkManager->RPC(RPC_ScriptingSetCameraBehindPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

		return true;
	}

	// setPlayerDucking(playerid, ducking)
	bool CPlayerModuleNatives::SetDucking(EntityId playerId, bool sqbDucking)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(sqbDucking != 0);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDucking, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// isPlayerDucking(playerid)
	bool CPlayerModuleNatives::IsDucking(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->IsDucking();
		}

		return false;
	}

	// setPlayerInvincible(playerid, invincible)
	bool CPlayerModuleNatives::SetInvincible(EntityId playerId, bool sqbInvincible)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(sqbInvincible != 0);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerInvincible, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// togglePlayerHUD(playerid, toggle)
	bool CPlayerModuleNatives::ToggleHUD(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(sqbToggle != 0);
			g_pNetworkManager->RPC(RPC_ScriptingToggleHUD, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// togglePlayerRadar(playerid, toggle)
	bool CPlayerModuleNatives::ToggleRadar(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleRadar, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// togglePlayerNames(playerid, toggle)
	bool CPlayerModuleNatives::ToggleNames(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// togglePlayerAreaNames(playerid, toggle)
	bool CPlayerModuleNatives::ToggleAreaNames(EntityId playerId, bool sqbToggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bool bToggle = (sqbToggle != 0);
			bsSend.Write(bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleAreaNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	
	// getEmptyPlayerControlState()
	CControlState CPlayerModuleNatives::GetEmptyControlState()
	{
		// Create a new control state
		CControlState controlState;
		return controlState;
	}

	// getPlayerPreviousControlState(playerid)
	CControlState CPlayerModuleNatives::GetPreviousControlState(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			// Get the player control state
			CControlState controlState;
			pPlayer->GetPreviousControlState(&controlState);
			return controlState;
		}

		return CControlState();
	}

	// getPlayerControlState(playerid)
	CControlState CPlayerModuleNatives::GetControlState(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			// Get the player control state
			CControlState controlState;
			pPlayer->GetControlState(&controlState);
			return controlState;
		}

		return CControlState();
	}
	
	// triggerClientEvent(playerid, eventname, ...)
	bool CPlayerModuleNatives::TriggerEvent(EntityId playerid, const char * szEventName, const char * szFormat, SquirrelArgumentsInterface* args, ...)
	{
		if(args != NULL)
		{
			// If we have arguments already, we add event name to the front of list and send them to client.
			CSquirrelArguments* arguments = reinterpret_cast<CSquirrelArguments*>(args);
			arguments->push_front((new CSquirrelArgument(String(szEventName))));
			CBitStream bsSend;
			arguments->serialize(&bsSend);
			g_pNetworkManager->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerid, false, PACKET_CHANNEL_SCRIPT);
			return true;
		}

		CBitStream bsSend;
		CSquirrelArguments* arguments = new CSquirrelArguments();
		arguments->push(String(szEventName));
		int argcount = 0;
		const char* p = szFormat;
		if(p)
		{
			va_list ap;
			va_start(ap, szFormat);
			while(*p)
			{
				switch(*p)
				{
					case 'b':
					{
						arguments->push(va_arg(ap, int) != 0);
					}
					break;
					case 'i':
					{
						arguments->push((int)va_arg(ap, int));
						break;
					}
					case 'f':
					{
						arguments->push((float)va_arg(ap, double));
						break;
					}
					break;
					case 's':
					{
						char* sz = va_arg(ap, char*);
						arguments->push(String(sz));
						break;
					}
				}
				argcount++;
				++p;
			}
			va_end(ap);
		}
		
		arguments->serialize(&bsSend);
		g_pNetworkManager->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerid, false, PACKET_CHANNEL_SCRIPT);
		return true;
	}
	
	// setPlayerColor(playerid, rgba)
	bool CPlayerModuleNatives::SetColor(EntityId playerId, int color)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->SetColor(color);
			return true;
		}

		return false;
	}

	int CPlayerModuleNatives::GetColor(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetColor();
		}

		return false;
	}

	int CPlayerModuleNatives::GetPing(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			return pPlayer->GetPing();
		}

		return -1;
	}

	bool CPlayerModuleNatives::SetClothes(EntityId iPlayerId, int iBodyPart, int iClothes)
	{
		if(g_pPlayerManager->DoesExist(iPlayerId) && iBodyPart >= 0 && iBodyPart <= 10 && iClothes >= 0 && iClothes <= 255)
		{
			g_pPlayerManager->GetAt(iPlayerId)->SetClothes((unsigned char)iBodyPart, (unsigned char)iClothes);
			return true;
		}
	
		return false;
	}

	int * CPlayerModuleNatives::GetClothes(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		int * iClothes = new int[12];

		if(pPlayer)
		{
			for(unsigned char uc = 0; uc < 11; uc ++ )
			{
				iClothes[uc] = pPlayer->GetClothes(uc);
			}

			return iClothes;
		}
	
		return iClothes;
	}

	bool CPlayerModuleNatives::ResetClothes(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->ResetClothes();
			return true;
		}

		return false;
	}

	bool CPlayerModuleNatives::Respawn(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			CBitStream bitStream;
			bitStream.Write(playerId);
			g_pNetworkManager->RPC(RPC_PlayerSpawn, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, pPlayer->GetPlayerId(), false);
			return true;
		}

		return false;
	}

	bool CPlayerModuleNatives::GiveHelmet(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->GiveHelmet();
			return true;
		}
	
		return false;
	}

	bool CPlayerModuleNatives::RemoveHelmet(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			pPlayer->RemoveHelmet();
			return true;
		}
	
		return false;
	}

	// togglePlayerHelmet(playerid, toggle)
	bool CPlayerModuleNatives::ToggleHelmet(EntityId playerId, bool sqbToggle)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			if(sqbToggle)
				pPlayer->GiveHelmet();
			else
				pPlayer->RemoveHelmet();
			return true;
		}

		return false;
	}

	// setPlayerCameraPos(playerid, x, y, z)
	bool CPlayerModuleNatives::SetCameraPos(EntityId playerId, CVector3 vecPos)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(vecPos);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraPos, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// setPlayerCameraLookAt(playerid, x, y, z)
	bool CPlayerModuleNatives::SetCameraLookAt(EntityId playerId, CVector3 vecPos)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(vecPos);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraLookAt, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// resetPlayerCamera(playerid)
	bool CPlayerModuleNatives::ResetCamera(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			g_pNetworkManager->RPC(RPC_ScriptingResetPlayerCamera, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}


	bool CPlayerModuleNatives::forceAnim(EntityId playerId, const char * szGroup, const char * szAnim)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(String(szGroup));
			bsSend.Write(String(szAnim));
			g_pNetworkManager->RPC(RPC_ScriptingForcePlayerAnim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	bool CPlayerModuleNatives::requestAnim(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			g_pNetworkManager->RPC(RPC_ScriptingRequestAnims, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::triggerAudioEvent(EntityId playerId, const char * szAudio)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(String(szAudio));
			g_pNetworkManager->RPC(RPC_ScriptingPlayGameAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::triggerMissionCompleteAudio(EntityId playerId, int iMission)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iMission);
			g_pNetworkManager->RPC(RPC_ScriptingPlayMissionCompleteAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::triggerPoliceReport(EntityId playerId, const char * szAudio)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(String(szAudio));
			g_pNetworkManager->RPC(RPC_ScriptingPlayPoliceReport, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::fadeScreenIn(EntityId playerId, int iDuration)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iDuration);
			g_pNetworkManager->RPC(RPC_ScriptingFadeScreenIn, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::fadeScreenOut(EntityId playerId, int iDuration)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(iDuration);
			g_pNetworkManager->RPC(RPC_ScriptingFadeScreenOut, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::blockWeaponChange(EntityId playerId, bool btoggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(btoggle);
			g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponChange, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	bool CPlayerModuleNatives::blockWeaponDrop(EntityId playerId, bool btoggle)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(btoggle);
			g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponDrop, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}

	// CrackHD: whatta fuck? Why here all functions use RPC() instead of way i did ???? It is shit. TODO fix
	bool CPlayerModuleNatives::SetDimension(EntityId playerId, unsigned char ucDimension)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			g_pPlayerManager->GetAt(playerId)->SetDimension(ucDimension);
			return true;
		}
		return false;
	}

	int CPlayerModuleNatives::GetDimension(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			return g_pPlayerManager->GetAt(playerId)->GetDimension();
		}
		return false;
	}
}
