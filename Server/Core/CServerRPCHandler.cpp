//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerRPCHandler.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CServerRPCHandler.h"
#include <Network/CPlayerSocket.h>
#include "Scripting/CScriptingManager.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CPickupManager.h"
#include "CCheckpointManager.h"
#include "CClientFileManager.h"
#include <CSettings.h>
#include <Game/CTime.h>
#include <Game/CTrafficLights.h>
#include "CModuleManager.h"
#include "CEvents.h"
#include "CNetworkManager.h"
#include "CVehicle.h"

extern CNetworkManager * g_pNetworkManager;
extern CScriptingManager * g_pScriptingManager;
extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CObjectManager * g_pObjectManager;
extern CBlipManager * g_pBlipManager;
extern CActorManager * g_pActorManager;
extern CCheckpointManager * g_pCheckpointManager;
extern CPickupManager * g_pPickupManager;
extern CClientFileManager * g_pClientScriptFileManager;
extern CClientFileManager * g_pClientResourceFileManager;
extern CTime * g_pTime;
extern CTrafficLights * g_pTrafficLights;
extern CModuleManager * g_pModuleManager;
extern CEvents * g_pEvents;
extern CVehicle * g_pVehicle;

void CServerRPCHandler::PlayerJoin(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CBitStream bsSend;

	// Read their network version
	int iVersion;

	if(!pBitStream->Read(iVersion))
		return;

	if(iVersion != NETWORK_VERSION)
	{
		bsSend.Write(REFUSE_REASON_INVALIDVERSION);
		g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		CLogFile::Printf("Authorization for %s failed (Invalid version (Client: %x, Server: %x)).", pSenderSocket->GetAddress(true).Get(), iVersion, NETWORK_VERSION);
		return;
	}

	// Read their name
	String strName;

	if(!pBitStream->Read(strName))
		return;

	if(strName.IsEmpty() || strName.GetLength() < 2)
	{
		bsSend.Write(REFUSE_REASON_TOOSHORT);
		g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		CLogFile::Printf("Authorization for %s failed (Name too short).", pSenderSocket->GetAddress(true).Get());
		return;
	}
	else if(strName.GetLength() > MAX_NAME)
	{
		bsSend.Write(REFUSE_REASON_TOOLONG);
		g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		CLogFile::Printf("Authorization for %s failed (Name too long).", pSenderSocket->GetAddress(true).Get());
		return;
	}

	if(g_pPlayerManager->IsNameInUse(strName))
	{
		bool bForceDisconnect = false;
		EntityId otherPlayerId = g_pPlayerManager->GetPlayerFromName(strName);

		// Check if this is even meant to be supported
		if(CVAR_GET_BOOL("kickoldplayers"))
		{
			// Check if the ip matches
			if(!g_pNetworkManager->GetPlayerIp(otherPlayerId).Compare(pSenderSocket->GetAddress()))
			{
				// Check if the HDD Serial matches
				if(g_pPlayerManager->GetAt(otherPlayerId)->GetSerial() == pSenderSocket->GetSerial())
					bForceDisconnect = true;
			}
		}

		if(bForceDisconnect)
			g_pNetworkManager->GetNetServer()->KickPlayer(otherPlayerId);
		else
		{
			bsSend.Write(REFUSE_REASON_INUSE);
			g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
			CLogFile::Printf("Authorization for %s failed (Name in use).", pSenderSocket->GetAddress(true).Get());
		}

		return;
	}

	// Check the player's name
	CSquirrelArguments nameCheckArguments;
	nameCheckArguments.push(playerId);
	nameCheckArguments.push(strName);

	if(g_pEvents->Call("playerNameCheck", &nameCheckArguments).GetInteger() != 1)
	{
		bsSend.Write(REFUSE_REASON_INVALIDNAME);
		g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		CLogFile::Printf("Authorization for %s failed (Invalid name).", pSenderSocket->GetAddress(true).Get());
		return;
	}

	// Read if their game files are modified
	bool bGameFilesModded = false;

	if(!pBitStream->Read(bGameFilesModded))
		return;

	// Are their game files modified?
	if(bGameFilesModded)
	{
		// Check if this server allows game file modifications
		CSquirrelArguments modCheckArguments;
		nameCheckArguments.push(playerId);

		if(g_pEvents->Call("playerModifiedGameFiles", &modCheckArguments).GetInteger() == 1)
		{
			bsSend.Write(REFUSE_REASON_FILES_MODIFIED);
			g_pNetworkManager->RPC(RPC_ConnectionRefused, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
			CLogFile::Printf("Authorization for %s failed (Game files are modified).", pSenderSocket->GetAddress(true).Get());
			return;
		}
	}

	g_pPlayerManager->Add(playerId, strName);
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CBitStream bsNametags;
		bsNametags.Write(CVAR_GET_BOOL("guinametags"));
		g_pNetworkManager->RPC(RPC_ScriptingSetNametags, &bsNametags, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

		// Let the vehicle manager handle the client join
		g_pVehicleManager->HandleClientJoin(playerId);

		// Let the player manager handle the client join
		g_pPlayerManager->HandleClientJoin(playerId);

		// Let the object manager handle the client join
		g_pObjectManager->HandleClientJoin(playerId);

		// Let the fire manager handle the client join
		g_pObjectManager->HandleClientJoinFire(playerId);

		// Let the blip manager handle the client join
		g_pBlipManager->HandleClientJoin(playerId);

		// Let the actor manager handle the client join
		g_pActorManager->HandleClientJoin(playerId);

		// Let the checkpoint manager handle the client join
		g_pCheckpointManager->HandleClientJoin(playerId);

		// Let the pickup manager handle the client join
		g_pPickupManager->HandleClientJoin(playerId);

		// Construct the reply bit stream
		bsSend.Write(playerId);
		bsSend.Write(CVAR_GET_STRING("hostname"));
		bsSend.Write(CVAR_GET_BOOL("paynspray"));
		bsSend.Write(CVAR_GET_BOOL("autoaim"));
		bsSend.Write(pPlayer->GetColor());
		bsSend.Write(CVAR_GET_STRING("httpserver"));
		bsSend.Write((unsigned short)CVAR_GET_INTEGER("httpport"));
		bsSend.Write((unsigned char)CVAR_GET_INTEGER("weather"));
		bsSend.Write(CVAR_GET_BOOL("guinametags"));
		bsSend.Write(CVAR_GET_BOOL("headmovement"));
		bsSend.Write(CVAR_GET_INTEGER("maxplayers"));

		// Time
		unsigned char ucHour = 0, ucMinute = 0;
		g_pTime->GetTime(&ucHour, &ucMinute);
		bsSend.Write((unsigned char)(ucHour + (24 * (1 + g_pTime->GetDayOfWeek()))));
		bsSend.Write(ucMinute);
		if(g_pTime->GetMinuteDuration() != CTime::DEFAULT_MINUTE_DURATION)
		{
			bsSend.Write1();
			bsSend.Write(g_pTime->GetMinuteDuration());
		}
		else
			bsSend.Write0();

		// Traffic Lights
		bsSend.Write((BYTE)g_pTrafficLights->GetSetState());
		bsSend.Write(g_pTrafficLights->GetTimeThisCylce());

		if(g_pTrafficLights->GetSetState() != CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
		{
			if(g_pTrafficLights->IsLocked())
				bsSend.Write1();
			else
				bsSend.Write0();

			if(!g_pTrafficLights->IsUsingDefaultDurations())
			{
				bsSend.Write1();
				if(g_pTrafficLights->GetSetState() >= CTrafficLights::TRAFFIC_LIGHT_STATE_FLASHING_FLASHING)
					bsSend.Write(g_pTrafficLights->GetYellowDuration());
				else
				{
					bsSend.Write(g_pTrafficLights->GetGreenDuration());
					bsSend.Write(g_pTrafficLights->GetYellowDuration());
					bsSend.Write(g_pTrafficLights->GetRedDuration());
				}
			}
			else
				bsSend.Write0();
		}

		// Send the joined game RPC
		g_pNetworkManager->RPC(RPC_JoinedGame, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

		// Inform the resource file manager of the client join
		g_pClientResourceFileManager->HandleClientJoin(playerId);

		// Inform the script file manager of the client join
		g_pClientScriptFileManager->HandleClientJoin(playerId);

		// Call the playerConnect scripting function
		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		g_pEvents->Call("playerConnect", &pArguments);

		CLogFile::Printf("[Join] %s (%d) has successfully joined the server.", strName.Get(), playerId);
	}
}

void CServerRPCHandler::Chat(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String strChat;

		if(!pBitStream->Read(strChat))
			return;

		strChat.Truncate(128);

		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pArguments.push(strChat);

		if(g_pEvents->Call("playerText", &pArguments).GetInteger() == 1)
		{
			CLogFile::Printf("[Chat] %s: %s", pPlayer->GetName().C_String(), strChat.Get());

			CBitStream bsSend;
			bsSend.WriteCompressed(playerId);
			bsSend.Write(strChat);
			g_pNetworkManager->RPC(RPC_Chat, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
	}
}

void CServerRPCHandler::Command(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String strCommand;

		if(!pBitStream->Read(strCommand))
			return;


		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pArguments.push(strCommand.Get());
		g_pEvents->Call("playerCommand", &pArguments);
	}
}

void CServerRPCHandler::PlayerSpawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		int iSkinId;

		if(!pBitStream->Read(iSkinId))
			return;

		pPlayer->SetModel(iSkinId);
		pPlayer->SpawnForWorld();
		CLogFile::Printf("[Spawn] %s spawned.", pPlayer->GetName().C_String());

		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		g_pEvents->Call("playerSpawn", &pArguments);
	}
}

void CServerRPCHandler::Death(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		EntityId killerPlayerId;
		EntityId killerVehicleId;
		EntityId killerWeaponId;

		if(!pBitStream->ReadCompressed(killerPlayerId))
			return;

		if(!pBitStream->ReadCompressed(killerVehicleId))
			return;

		if(!pBitStream->ReadCompressed(killerWeaponId))
			return;

		if(killerPlayerId != INVALID_ENTITY_ID)
		{
			CPlayer * pKiller = g_pPlayerManager->GetAt(killerPlayerId);

			if(pKiller)
				CLogFile::Printf("[Death] %s was killed by %s(Weapon %d).", pPlayer->GetName().C_String(), pKiller->GetName().C_String(), killerWeaponId);
			else
				killerPlayerId = INVALID_ENTITY_ID;
		}

		if(killerVehicleId != INVALID_ENTITY_ID)
		{
			CVehicle * pKillerVehicle = g_pVehicleManager->GetAt(killerVehicleId);

			if(pKillerVehicle)
				CLogFile::Printf("[Death] %s was killed by vehicle %d.", pPlayer->GetName().C_String(), killerVehicleId);
			else
				killerVehicleId = INVALID_ENTITY_ID;
		}

		if(killerPlayerId == INVALID_ENTITY_ID && killerVehicleId == INVALID_ENTITY_ID)
			CLogFile::Printf("[Death] %s died.", pPlayer->GetName().C_String());

		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pArguments.push(killerPlayerId);
		pArguments.push(killerWeaponId);
		pArguments.push(killerVehicleId);
		g_pEvents->Call("playerDeath", &pArguments);

		pPlayer->KillForWorld();
	}
}

void CServerRPCHandler::OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(CVAR_GET_BOOL("frequentevents"))
		{
			CSquirrelArguments pArguments;
			pArguments.push(playerId);

			if(g_pEvents->Call("playerOnFootSyncReceived", &pArguments).GetInteger() != 1 || g_pEvents->Call("playerSyncReceived", &pArguments).GetInteger() != 1)
				return;
		}

		OnFootSyncData syncPacket;
		AimSyncData aimSyncData;

		if(!pBitStream->Read((char *)&syncPacket, sizeof(OnFootSyncData)))
			return;

		bool bHasAimSyncData = pBitStream->ReadBit();

		if(bHasAimSyncData)
		{
			if(!pBitStream->Read((char *)&aimSyncData, sizeof(AimSyncData)))
				return;
		}

		pPlayer->StoreOnFootSync(&syncPacket, bHasAimSyncData, &aimSyncData);
	}
}

void CServerRPCHandler::InVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(CVAR_GET_BOOL("frequentevents"))
		{
			CSquirrelArguments pArguments;
			pArguments.push(playerId);

			if(g_pEvents->Call("playerInVehicleSyncReceived", &pArguments).GetInteger() != 1 || g_pEvents->Call("playerSyncReceived", &pArguments).GetInteger() != 1)
				return;
		}

		EntityId vehicleId;
		InVehicleSyncData syncPacket;
		AimSyncData aimSyncData;

		if(!pBitStream->ReadCompressed(vehicleId))
			return;

		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			if(!pBitStream->Read((char *)&syncPacket, sizeof(InVehicleSyncData)))
				return;

			bool bHasAimSyncData = pBitStream->ReadBit();

			if(bHasAimSyncData)
			{
				if(!pBitStream->Read((char *)&aimSyncData, sizeof(AimSyncData)))
					return;
			}

			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

			if(pVehicle)
			{
				pPlayer->StoreInVehicleSync(pVehicle, &syncPacket, bHasAimSyncData, &aimSyncData);
				pVehicle->StoreInVehicleSync(&syncPacket);
			}
		}
	}
}

void CServerRPCHandler::PassengerSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(CVAR_GET_BOOL("frequentevents"))
		{
			CSquirrelArguments pArguments;
			pArguments.push(playerId);

			if(g_pEvents->Call("playerPassengerSyncReceived", &pArguments).GetInteger() != 1 || g_pEvents->Call("playerSyncReceived", &pArguments).GetInteger() != 1)
				return;
		}

		EntityId vehicleId;
		PassengerSyncData syncPacket;
		AimSyncData aimSyncData;

		if(!pBitStream->ReadCompressed(vehicleId))
			return;

		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			if(!pBitStream->Read((char *)&syncPacket, sizeof(PassengerSyncData)))
				return;

			bool bHasAimSyncData = pBitStream->ReadBit();

			if(bHasAimSyncData)
			{
				if(!pBitStream->Read((char *)&aimSyncData, sizeof(AimSyncData)))
					return;
			}

			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

			if(pVehicle)
			{
				pPlayer->StorePassengerSync(pVehicle, &syncPacket, bHasAimSyncData, &aimSyncData);
				pVehicle->StorePassengerSync(&syncPacket);
			}
		}
	}
}

void CServerRPCHandler::SmallSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(CVAR_GET_BOOL("frequentevents"))
		{
			CSquirrelArguments pArguments;
			pArguments.push(playerId);

			if(g_pEvents->Call("playerSmallSyncReceived", &pArguments).GetInteger() != 1 || g_pEvents->Call("playerSyncReceived", &pArguments).GetInteger() != 1)
				return;
		}

		SmallSyncData syncPacket;
		AimSyncData aimSyncData;

		if(!pBitStream->Read((char *)&syncPacket, sizeof(SmallSyncData)))
			return;

		bool bHasAimSyncData = pBitStream->ReadBit();

		if(bHasAimSyncData)
		{
			if(!pBitStream->Read((char *)&aimSyncData, sizeof(AimSyncData)))
				return;
		}

		pPlayer->StoreSmallSync(&syncPacket, bHasAimSyncData, &aimSyncData);
	}
}

void CServerRPCHandler::VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Get the player pointer
	CPlayer * pPlayer = g_pPlayerManager->GetAt(pSenderSocket->playerId);

	// Is the player pointer valid?
	if(pPlayer)
	{
		// Read the vehicle entry/exit type
		BYTE byteVehicleEntryExitType;

		if(!pBitStream->Read(byteVehicleEntryExitType))
			return;

		// Read the vehicle id
		EntityId vehicleId;

		if(!pBitStream->ReadCompressed(vehicleId))
			return;

		// Get the vehicle
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		// Does the vehicle not exist?
		if(!pVehicle)
			return;

		// Is this an entry request?
		if(byteVehicleEntryExitType == VEHICLE_ENTRY_REQUEST)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Is the vehicle fully locked?
			if(pVehicle->GetLocked() == 1)
				return;

			// Get the reply
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			bool bReply = (g_pEvents->Call("vehicleEntryRequest", &arguments).GetInteger() == 1);

			// Reply to the vehicle entry request
			CBitStream bitStream;
			bitStream.WriteCompressed(pSenderSocket->playerId);
			bitStream.WriteBit(bReply);

			// Was the reply ok?
			if(bReply)
			{
				bitStream.Write((BYTE)VEHICLE_ENTRY_RETURN);
				bitStream.Write(vehicleId);
				bitStream.Write(byteSeatId);
				g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);

				// Set the player state
				pPlayer->SetState(STATE_TYPE_ENTERVEHICLE);
			}
		}
		// Is this an entry cancellation?
		if(byteVehicleEntryExitType == VEHICLE_ENTRY_CANCELLED)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Call the event
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			g_pEvents->Call("vehicleEntryCancelled", &arguments);

			CBitStream bitStream;
			bitStream.WriteCompressed(pSenderSocket->playerId);
			bitStream.WriteBit(true);
			bitStream.Write((BYTE)VEHICLE_ENTRY_CANCELLED);
			bitStream.Write(vehicleId);
			g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, pSenderSocket->playerId, true);

			// Set the player state
			pPlayer->SetState(STATE_TYPE_ONFOOT);
		}
		// Is this an entry completion?
		else if(byteVehicleEntryExitType == VEHICLE_ENTRY_COMPLETE)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Call the event
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			g_pEvents->Call("vehicleEntryComplete", &arguments);

			// Set the player vehicle and seat id
			pPlayer->SetVehicle(pVehicle);
			pPlayer->SetVehicleSeatId(byteSeatId);

			// Set the vehicle occupant
			pVehicle->SetOccupant(byteSeatId, pPlayer);

			// Set the player state
			pPlayer->SetState(STATE_TYPE_INVEHICLE);
		}
		// Is this an exit request?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_REQUEST)
		{
			// Get the reply
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(pPlayer->GetVehicleSeatId());
			bool bReply = (g_pEvents->Call("vehicleExitRequest", &arguments).GetInteger() == 1);

			// Reply to the vehicle exit request
			CBitStream bitStream;
			bitStream.WriteCompressed(pSenderSocket->playerId);
			bitStream.WriteBit(bReply);

			// Was the reply ok?
			if(bReply)
			{
				bitStream.Write((BYTE)VEHICLE_EXIT_RETURN);
				bitStream.Write(vehicleId);
				g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);

				// Set the player state
				pPlayer->SetState(STATE_TYPE_EXITVEHICLE);
			}
		}
		// Is this an exit completion?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_COMPLETE)
		{
			// Call the event
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(pPlayer->GetVehicleSeatId());
			g_pEvents->Call("vehicleExitComplete", &arguments);

			// Reset the vehicle occupant
			pVehicle->SetOccupant(pPlayer->GetVehicleSeatId(), NULL);

			// Reset the player vehicle and seat id
			pPlayer->SetVehicle(NULL);
			pPlayer->SetVehicleSeatId(0);

			// Set the player state
			pPlayer->SetState(STATE_TYPE_ONFOOT);
		}
		// Is this a forceful exit?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_FORCEFUL)
		{
			// Call the event
			CSquirrelArguments arguments;
			arguments.push(pSenderSocket->playerId);
			arguments.push(vehicleId);
			arguments.push(pPlayer->GetVehicleSeatId());
			g_pEvents->Call("vehicleForcefulExit", &arguments);

			// Reset the vehicle occupant
			pVehicle->SetOccupant(pPlayer->GetVehicleSeatId(), NULL);

			// Reset the player vehicle and seat id
			pPlayer->SetVehicle(NULL);
			pPlayer->SetVehicleSeatId(0);

			// Set the player state
			pPlayer->SetState(STATE_TYPE_ONFOOT);
		}
	}
}

void CServerRPCHandler::HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	CVector3 vecAim;

	if(!pBitStream->Read(vecAim.fX))
		return;

	if(!pBitStream->Read(vecAim.fY))
		return;

	if(!pBitStream->Read(vecAim.fZ))
		return;

	// Check if frequentevents and headmovement is enabled
	if(CVAR_GET_BOOL("frequentevents") && CVAR_GET_BOOL("headmovement"))
	{
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(vecAim.fX);
		bsSend.Write(vecAim.fY);
		bsSend.Write(vecAim.fZ);
		g_pNetworkManager->RPC(RPC_HeadMovement, &bsSend, PRIORITY_HIGH, RELIABILITY_UNRELIABLE, INVALID_ENTITY_ID, true);

		// Update our head sync
		if(pPlayer)
			pPlayer->UpdateHeadMoveSync(vecAim);
	}
}

// TODO: Vehicle id checking here
void CServerRPCHandler::EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// FIXUPDATE
	// Ensure we have a valid bit stream
	/*if(!pBitStream)
		return;

	// TODO: Server needs to store this?
	EntityId playerId = pSenderSocket->playerId;

	if(CVAR_GET_BOOL("frequentevents"))
	{
		CSquirrelArguments pArguments;
		pArguments.push(playerId);

		if(g_pEvents->Call("playerEmptyVehicleSyncReceived", &pArguments) != 1 || g_pEvents->Call("playerSyncReceived", &pArguments) != 1)
			return;
	}

	CBitStream bsSend(packet->data, packet->length, false);
	EMPTYVEHICLESYNCPACKET syncPacket;

	if(!pBitStream->Read((PCHAR)&syncPacket, sizeof(EMPTYVEHICLESYNCPACKET)))
		return;

	//g_pVehicleManager->StoreEmptyVehicleSync(&syncPacket);
	g_pNetworkManager->RPC(RPC_EmptyVehicleSync, &bsSend, PRIORITY_HIGH, RELIABILITY_UNRELIABLE_SEQUENCED, playerId, true);*/
}

void CServerRPCHandler::NameChange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String strName;

		if(!pBitStream->Read(strName))
			return;

		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pArguments.push(strName);

		if(g_pEvents->Call("playerRequestNameChange", &pArguments).GetInteger() == 1)
			pPlayer->SetName(strName);
	}
}

void CServerRPCHandler::CheckpointEntered(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	EntityId checkpointId;

	if(!pBitStream->Read(checkpointId))
		return;

	CSquirrelArguments pArguments;
	pArguments.push(playerId);
	pArguments.push(checkpointId);
	g_pEvents->Call("playerEnterCheckpoint", &pArguments);
}

void CServerRPCHandler::CheckpointLeft(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId = pSenderSocket->playerId;
	EntityId checkpointId;

	if(!pBitStream->Read(checkpointId))
		return;

	CSquirrelArguments pArguments;
	pArguments.push(playerId);
	pArguments.push(checkpointId);
	g_pEvents->Call("playerLeaveCheckpoint", &pArguments);
}

void CServerRPCHandler::EventCall(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CSquirrelArguments* pArgs = new CSquirrelArguments(pBitStream);
	CSquirrelArgument* pEventName = pArgs->size() > 0 ? pArgs->front() : 0;

	if(pEventName && pEventName->GetType() == OT_STRING)
	{
		String strEventName = pEventName->data.str->C_String();
		pArgs->pop_front();
		pArgs->push_front(new CSquirrelArgument(pSenderSocket->playerId));

		g_pEvents->Call(strEventName, pArgs);

		delete pEventName;
	}

	delete pArgs;
}

void CServerRPCHandler::VehicleDeath(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(!pVehicle)
		return;

	CSquirrelArguments pArguments;
	pArguments.push(vehicleId);
	g_pEvents->Call("vehicleDeath",&pArguments);

	// Sure that we have no player in vehicle(otherwise crash >.<)
	if(pVehicle->GetDriver())
	{
		EntityId playerId = pVehicle->GetDriver()->GetPlayerId();
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write0();
		g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerFromVehicle,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	}
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
		{
			if(!g_pPlayerManager->GetAt(i)->IsOnFoot())
			{
				if(g_pPlayerManager->GetAt(i)->GetVehicle()->GetVehicleId() == pVehicle->GetVehicleId())
				{
					CBitStream bsSend;
					bsSend.Write(i);
					bsSend.Write0();
					g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerFromVehicle,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, i, false);
				}
			}
		}

	}

	// If everyone is kickedout of the vehicle, respawn it
	pVehicle->Respawn();
	g_pEvents->Call("vehicleRespawn", &pArguments);
}

void CServerRPCHandler::Register()
{
	AddFunction(RPC_PlayerJoin, PlayerJoin);
	AddFunction(RPC_Chat, Chat);
	AddFunction(RPC_Command, Command);
	AddFunction(RPC_PlayerSpawn, PlayerSpawn);
	AddFunction(RPC_Death, Death);
	AddFunction(RPC_OnFootSync, OnFootSync);
	AddFunction(RPC_InVehicleSync, InVehicleSync);
	AddFunction(RPC_PassengerSync, PassengerSync);
	AddFunction(RPC_SmallSync, SmallSync);
	AddFunction(RPC_VehicleEnterExit, VehicleEnterExit);
	AddFunction(RPC_HeadMovement, HeadMovement);
	AddFunction(RPC_EmptyVehicleSync, EmptyVehicleSync);
	AddFunction(RPC_NameChange, NameChange);
	AddFunction(RPC_CheckpointEntered, CheckpointEntered);
	AddFunction(RPC_CheckpointLeft, CheckpointLeft);
	AddFunction(RPC_ScriptingEventCall, EventCall);
	AddFunction(RPC_ScriptingVehicleDeath, VehicleDeath);
}

void CServerRPCHandler::Unregister()
{
	RemoveFunction(RPC_PlayerJoin);
	RemoveFunction(RPC_Chat);
	RemoveFunction(RPC_Command);
	RemoveFunction(RPC_PlayerSpawn);
	RemoveFunction(RPC_Death);
	RemoveFunction(RPC_OnFootSync);
	RemoveFunction(RPC_InVehicleSync);
	RemoveFunction(RPC_PassengerSync);
	RemoveFunction(RPC_SmallSync);
	RemoveFunction(RPC_VehicleEnterExit);
	RemoveFunction(RPC_HeadMovement);
	RemoveFunction(RPC_EmptyVehicleSync);
	RemoveFunction(RPC_NameChange);
	RemoveFunction(RPC_CheckpointEntered);
	RemoveFunction(RPC_CheckpointLeft);
	RemoveFunction(RPC_ScriptingEventCall);
	RemoveFunction(RPC_ScriptingVehicleDeath);
}
