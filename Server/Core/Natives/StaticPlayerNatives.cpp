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
	// Position, heading, model etc...
	bool SetPlayerCoordinates(EntityId playerId, CVector3 vecPos)
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
	bool GetPlayerCoordinates(EntityId playerId, CVector3& vecPos)
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
		if(pPlayer)
		{
			pPlayer->GetPosition(vecPos);
			return true;
		}
		return false;
	}

	// Name stuff
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

	// Wanted level
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

	// Weapons stuff
	bool GivePlayerWeapon(EntityId playerId, int iWeaponModelId, int iAmmo)
	{
		if(g_pPlayerManager->DoesExist(playerId) && iWeaponModelId > 0 && iWeaponModelId < 21 && iWeaponModelId != 6)
		{
			CBitStream bsSend;
			bsSend.Write(iWeaponModelId);
			bsSend.Write(iAmmo);
			g_pNetworkManager->RPC(RPC_ScriptingGivePlayerWeapon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
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
	
	// Other natives
	bool SetPlayerTime(EntityId playerId, BYTE ucHour, BYTE ucMinute)
	{
		if(ucHour < 24 && ucMinute < 60 && g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(ucHour);
			bsSend.Write(ucMinute);
			if(g_pTime->GetMinuteDuration() != CTime::DEFAULT_MINUTE_DURATION)
				bsSend.Write(g_pTime->GetMinuteDuration());

			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerTime, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}
	bool SetPlayerWeather(EntityId playerId, BYTE ucWeatherId)
	{
		if(ucWeatherId > 0 && ucWeatherId < 11 && g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(ucWeatherId);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}
		return false;
	}
	bool SetPlayerGravity(EntityId playerId, float fGravity)
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
	bool SendPlayerMessage(EntityId playerId, String strMessage, DWORD dwColor, bool bAllowFormatting)
	{
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
	bool IsPlayerConnected(EntityId playerId)
	{
		return playerId >= 0 && playerId <= MAX_PLAYERS
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
}