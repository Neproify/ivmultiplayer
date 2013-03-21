//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: StaticPlayerNatives.cpp
// Project: Server.Core
// Author(s):	CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include "StaticPlayerNatives.h"

namespace StaticNatives
{
	// Set/Get
	bool SetPlayerCoordinates(EntityId playerId, CVector3 vecPos)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetCoordinates(vecPos);			
			return true;
		}
		return false;
	}
	bool GetPlayerCoordinates(EntityId playerId, CVector3& vecPos)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->GetCoordinates(vecPos);
			return true;
		}
		return false;
	}
	bool SetPlayerName(EntityId playerId, String strPlayerName)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			CSquirrelArguments nameCheckArguments;
			nameCheckArguments.push(playerId);
			nameCheckArguments.push(strPlayerName);

			if(g_pEvents->Call("playerNameCheck", &nameCheckArguments).GetInteger() != 1)
			{
				CLogFile::Printf("Can't change the name from player %d (playerNameCheck block)",playerId);
				return false;
			}

			return pPlayer->SetName(strPlayerName);
		}
		return false;
	}
	bool GetPlayerName(EntityId playerId, String& strPlayerName)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			strPlayerName = pPlayer->GetName();
			return true;
		}
		return false;
	}
	bool SetPlayerWantedLevel(EntityId playerId, unsigned int iWantedLevel)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetWantedLevel(iWantedLevel);
			return true;
		}
		return false;
	}
	bool GetPlayerWantedLevel(EntityId playerId, unsigned int& iWantedLevel)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			iWantedLevel = pPlayer->GetWantedLevel();
			return true;
		}
		return false;
	}
	bool SetPlayerHealth(EntityId playerId, int iHealth)	
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			// CrackHD: I dont understand why we can't set health to 255. It just doesn't work, limited to 100(200 actually in IV).
			// TODO: Increase max health of players. It MUST be more than 100
			/*if(iHealth < -1)
				iHealth = -1;
			else if(iHealth > 255)
				iHealth = 255;*/

			pPlayer->SetHealth(iHealth);
			return true;
		}
		return false;
	}
	bool GetPlayerHealth(EntityId playerId, int& iHealth)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			if(pPlayer->IsDead())
				iHealth = -1;
			else
				iHealth = pPlayer->GetHealth();
			return true;
		}
		return false;
	}
	bool SetPlayerArmour(EntityId playerId, unsigned int uiArmour)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetArmour(uiArmour);
			return true;
		}
		return false;
	}
	bool GetPlayerArmour(EntityId playerId, unsigned int& uiArmour)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			uiArmour = pPlayer->GetArmour();
			return true;
		}
		return false;
	}
	bool SetPlayerGravity(EntityId playerId, float fGravity)
	{
		// TODO: getPlayerGravity scripting native
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetGravity(fGravity);
			return true;
		}
		return false;
	}
	bool SetPlayerSpawnLocation(EntityId playerId, CVector3 vecCoords, float fHeading)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetSpawnLocation(vecCoords, fHeading);			
			return true;
		}
		return false;
	}
	bool GetPlayerSpawnLocation(EntityId playerId, CVector3& vecCoords, float& fHeading)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->GetSpawnLocation(vecCoords, &fHeading);
			return true;
		}
		return false;
	}
	bool SetPlayerModel(EntityId playerId, unsigned short usModelId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetModel(usModelId);
			return true;
		}
		return false;
	}
	bool GetPlayerModel(EntityId playerId, unsigned short& usModelId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			usModelId = pPlayer->GetModel();
			return true;
		}
		return false;
	}
	bool SetPlayerHeading(EntityId playerId, float fHeading)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetHeading(fHeading);
			return true;
		}
		return false;
	}
	bool GetPlayerHeading(EntityId playerId, float& fHeading)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			fHeading = pPlayer->GetHeading();
			return true;
		}
		return false;
	}
	bool SetPlayerVelocity(EntityId playerId, CVector3 vecSpeed)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetVelocity(vecSpeed);
			return true;
		}
		return false;
	}
	bool GetPlayerVelocity(EntityId playerId, CVector3& vecSpeed)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->GetVelocity(vecSpeed);
			return true;
		}
		return false;
	}
	bool SetPlayerDucking(EntityId playerId, bool bDucking)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetDucking(bDucking);
			return true;
		}
		return false;
	}
	bool IsPlayerDucking(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->IsDucking();
		return false;
	}
	bool SetPlayerColor(EntityId playerId, unsigned int uiColor)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)			
		{
			pPlayer->SetColor(uiColor);
			return true;
		}
		return false;
	}
	bool GetPlayerColor(EntityId playerId, unsigned int& uiColor)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			uiColor = pPlayer->GetColor();
			return true;
		}
		return false;
	}
	// clothes
	bool SetPlayerDimension(EntityId playerId, DimensionId dimensionId)
	{		
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)			
		{
			pPlayer->SetDimension(dimensionId);
			return true;
		}
		return false;
	}
	bool GetPlayerDimension(EntityId playerId, DimensionId& dimensionId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)			
		{
			dimensionId = pPlayer->GetDimension();
			return true;
		}
		return false;
	}

	// Weapons stuff
	bool GivePlayerWeapon(EntityId playerId, int iWeaponModelId, int iAmmo)
	{		
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->GiveWeapon(iWeaponModelId, iAmmo);
			return true;
		}
		return false;
	}
	bool RemovePlayerWeapons(EntityId playerId)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			g_pNetworkManager->RPC(RPC_ScriptingRemoveWeapons, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}
	bool GetPlayerWeapon(EntityId playerId, unsigned int& iWeaponModelId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			iWeaponModelId = pPlayer->GetWeapon();
			return true;
		}
		return false;
	}
	bool GetPlayerAmmo(EntityId playerId, unsigned int& iWeaponAmmo)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			iWeaponAmmo = pPlayer->GetAmmo();
			return true;
		}
		return false;
	}

	// Money stuff
	bool GivePlayerMoney(EntityId playerId, int iMoneyAdd)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->GiveMoney(iMoneyAdd);
		return false;
	}
	bool SetPlayerMoney(EntityId playerId, int iMoney)
	{
		// TODO: Checks for range?
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->SetMoney(iMoney);
		return false;
	}
	bool GetPlayerMoney(EntityId playerId, int& iMoney)
	{
		// TODO: Checks for range?
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			iMoney = pPlayer->GetMoney();
			return true;
		}
		return false;
	}
	bool ResetPlayerMoney(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetMoney(0);
			return true;
		}
		return false;
	}
	
	// Toggles
	bool TogglePlayerPayAndSpray(EntityId playerId, bool bToggle)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->TogglePayAndSpray(bToggle);
			return true;
		}
		return false;
	}
	bool TogglePlayerAutoAim(EntityId playerId, bool bToggle)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleAutoAim(bToggle);
			return true;
		}
		return false;
	}
	bool TogglePlayerDrunk(EntityId playerId, bool bToggle)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleDrunk(bToggle);
			return true;
		}
		return false;
	}
	bool TogglePlayerControls(EntityId playerId, bool bControlsEnabled)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleControls(bControlsEnabled);
			return true;
		}
		return false;
		// TODO: GET function
	}
	bool TogglePlayerFrozen(EntityId playerId, bool bFrozen, bool bCameraFrozen)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleFrozen(bFrozen, bCameraFrozen);
			return true;
		}
		return false;
	}
	bool TogglePlayerPhysics(EntityId playerId, bool bEnabled)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->TogglePhysics(bEnabled);
			return true;
		}
		return false;
	}
	bool TogglePlayerHelmet(EntityId playerId, bool bHelmet)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			if(bHelmet)
				pPlayer->GiveHelmet();
			else
				pPlayer->RemoveHelmet();
			return true;
		}
		return false;
	}
	bool TogglePlayerHUD(EntityId playerId, bool bVisible)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleHUD(bVisible);
			return true;
		}
		return false;
	}
	bool TogglePlayerRadar(EntityId playerId, bool bVisible)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleRadar(bVisible);
			return true;
		}
		return false;
	}
	bool TogglePlayerNameTags(EntityId playerId, bool bDraw)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleNameTags(bDraw);
			return true;
		}
		return false;
	}
	bool TogglePlayerAreaNames(EntityId playerId, bool bShowAreaNames)
	{		
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ToggleAreaNames(bShowAreaNames);
			return true;
		}
		return false;
	}

	// Camera
	bool SetPlayerCameraBehind(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetCameraBehind();
			return true;
		}
		return false;
	}
	bool SetPlayerCameraPos(EntityId playerId, CVector3 vecPos)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetCameraPos(vecPos);
			return true;
		}
		return false;
	}
	bool SetPlayerCameraLookAt(EntityId playerId, CVector3 vecLookAt)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetCameraLookAt(vecLookAt);
			return true;
		}
		return false;
	}
	bool ResetPlayerCamera(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->ResetCamera();
			return true;
		}
		return false;
	}

	// Screen text	
	bool DisplayPlayerText(EntityId playerId, float fX, float fY, String strText, unsigned int uiTime)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->DisplayText(fX,fY,strText, uiTime);
			return true;
		}
		return false;
	}
	bool DisplayTextForAll(float fX, float fY, String strText, unsigned int uiTime)
	{
		if(g_pPlayerManager->GetPlayerCount() == 0)
			return false;

		// TODO: replace with broadcasted RPC ?
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			CPlayer* pPlayer = g_pPlayerManager->GetAt(i);
			if(pPlayer)
				pPlayer->DisplayText(fX, fY, strText, uiTime);
		}
		return true;
	}
	bool DisplayPlayerInfoText(EntityId playerId, String strText, unsigned int uiTime)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->DisplayInfoText(strText, uiTime);
			return true;
		}
		return false;
	}
	bool DisplayInfoTextForAll(String strText, unsigned int uiTime)
	{
		// TODO: or replace it with broadcasted rpc ?
		if(g_pPlayerManager->GetPlayerCount() == 0)
			return false;

		// TODO: replace with broadcasted RPC ?
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			CPlayer* pPlayer = g_pPlayerManager->GetAt(i);
			if(pPlayer)
				pPlayer->DisplayInfoText(strText, uiTime);
		}
		return true;
	}

	// Other natives
	bool SetPlayerTime(EntityId playerId, BYTE byteHour, BYTE byteMinute)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer && byteHour < 24 && byteMinute < 60)
		{
			pPlayer->SetTime(byteHour, byteMinute, g_pTime->GetMinuteDuration());
			return true;
		}
		return false;
	}
	bool SetPlayerWeather(EntityId playerId, BYTE byteWeatherId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer && byteWeatherId > 0 && byteWeatherId < 11)
		{
			pPlayer->SetWeather(byteWeatherId);
			return true;
		}
		return false;
	}
	bool SendPlayerMessage(EntityId playerId, String strMessage, DWORD dwColor, bool bAllowFormatting)
	{
		// TODO: Move RPC send code anywhere to some of managers
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(dwColor);
			bsSend.Write(strMessage);
			bsSend.Write(bAllowFormatting);
			g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}
	bool SendMessageToAll(String strMessage, DWORD dwColor, bool bAllowFormatting)
	{
		// TODO: Move RPC send code anywhere to some of managers
		if(g_pPlayerManager->GetPlayerCount() > 0)
		{
			CBitStream bsSend;
			bsSend.Write(dwColor);
			bsSend.Write(strMessage);
			bsSend.Write(bAllowFormatting);
			g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return true;
		}
		return false;
	}
	bool IsPlayerConnected(EntityId playerId)
	{
		return playerId >= 0 && playerId < MAX_PLAYERS
			&& g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->GetAt(playerId)->IsJoined();
	}
	bool GetPlayerSerial(EntityId playerId, String& strSerial)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(!pPlayer)
		{
			strSerial = pPlayer->GetSerial();
			return true;
		}
		return false;
	}
	bool IsPlayerInAnyVehicle(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->IsInVehicle();
		return false;
	}
	bool IsPlayerInVehicle(EntityId playerId, EntityId vehicleId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			CVehicle* pVehicle = pPlayer->GetVehicle();
			return pVehicle && pVehicle->GetVehicleId() == vehicleId;
		}
		return false;
	}
	bool SetPlayerInvincible(EntityId playerId, bool bInvincible)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->SetInvincible(bInvincible);
			return true;
		}
		return false;
	}
	bool GetPlayerVehicleId(EntityId playerId, EntityId& vehicleId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			CVehicle* pVehicle = pPlayer->GetVehicle();
			if(pVehicle)
			{
				vehicleId = pVehicle->GetVehicleId();
				return true;
			}
		}
		return false;
	}
	bool GetPlayerSeatId(EntityId playerId, BYTE& byteSeat)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer && pPlayer->IsInVehicle())
		{
			byteSeat = pPlayer->GetVehicleSeatId();
			return true;			
		}
		return false;
	}
	bool IsPlayerOnFoot(EntityId playerId)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->IsOnFoot();
		return false;
	}
	bool IsPlayerSpawned(EntityId playerId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->IsSpawned();
		return false;
	}
	bool KickPlayer(EntityId playerId, bool bSendKickMessage)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->Kick(bSendKickMessage);
			return true;
		}
		return false;
	}
	bool BanPlayer(EntityId playerId, unsigned int uiSeconds)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->Ban(uiSeconds);
			return true;
		}
		return false;
	}
	bool GetPlayerIp(EntityId playerId, String& strIp)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			strIp = pPlayer->GetIp();
			return true;
		}
		return false;
	}
	bool GetPlayerState(EntityId playerId, eStateType& state)
	{
		if(/*playerId >= 0 &&*/playerId < MAX_PLAYERS)
		{
			CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
			if(pPlayer)
				state = pPlayer->GetState();
			else
				state = STATE_TYPE_DISCONNECT;
			return true;
		}
		return false;
	}
	bool WarpPlayerIntoVehicle(EntityId playerId, EntityId vehicleId, BYTE byteSeatId)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
			return pPlayer->WarpIntoVehicle(vehicleId, byteSeatId);
		return false;
	}
	bool RemovePlayerFromVehicle(EntityId playerId, bool bAnimated)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->RemoveFromVehicle(bAnimated);
			return true;
		}
		return false;
	}
	bool TriggerPlayerClientEvent(EntityId playerId, CSquirrelArguments sqArgs)
	{
		CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->TriggerClientEvent(sqArgs);
			return true;
		}
		return false;
	}
	bool GetPlayerPing(EntityId playerId, unsigned short& usPing)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			usPing = pPlayer->GetPing();
			return true;
		}
		return false;
	}
}