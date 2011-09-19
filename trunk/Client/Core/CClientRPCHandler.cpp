//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientRPCHandler.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNetworkPlayer.h"
#include "CRemotePlayer.h"
#include "CNetworkManager.h"
#include "CChatWindow.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CCheckpointManager.h"
#include "CPickupManager.h"
#include "CModelManager.h"
#include "CMainMenu.h"
#include "CClientScriptManager.h"
#include <CSettings.h>
#include "CFileTransfer.h"
#include <Game/CTime.h>
#include <Game/CTrafficLights.h>
#include "CLocalPlayer.h"
#include "Scripting.h"
#include <Scripting/CScriptingManager.h>
#include <SharedUtility.h>
#include <CEvents.h>
#include "CNameTags.h"
#include "COffsets.h"
#include "CIVWeather.h"

extern String g_strNick;
extern String g_strHost;
extern CLocalPlayer * g_pLocalPlayer;
extern CNetworkManager * g_pNetworkManager;
extern CChatWindow * g_pChatWindow;
extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CObjectManager * g_pObjectManager;
extern CBlipManager * g_pBlipManager;
extern CActorManager * g_pActorManager;
extern CCheckpointManager * g_pCheckpointManager;
extern CPickupManager * g_pPickupManager;
extern CModelManager * g_pModelManager;
extern CMainMenu * g_pMainMenu;
extern CClientScriptManager * g_pClientScriptManager;
extern CFileTransfer * g_pFileTransfer;
extern CTime * g_pTime;
extern CTrafficLights * g_pTrafficLights;
extern CScriptingManager * g_pScriptingManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;
extern CNameTags * g_pNameTags;

bool m_bControlsDisabled = false;

void CClientRPCHandler::JoinedGame(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	String sHostName;
	bool bPayAndSpray;
	bool bAutoAim;
	unsigned int color;
	String sHttpServer;
	unsigned short usHttpPort;
	unsigned char ucWeather;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned int uiMinuteDuration;
	unsigned char ucTrafficLightState;
	unsigned int uiTrafficLightTimePassed, uiGreenDuration, uiYellowDuration, uiRedDuration;

	pBitStream->Read(playerId);
	pBitStream->Read(sHostName);
	pBitStream->Read(bPayAndSpray);
	pBitStream->Read(bAutoAim);
	pBitStream->Read(color);

	pBitStream->Read(sHttpServer);
	pBitStream->Read(usHttpPort);
	pBitStream->Read(ucWeather);

	pBitStream->Read(ucHour);
	pBitStream->Read(ucMinute);
	if(pBitStream->ReadBit())
		pBitStream->Read(uiMinuteDuration);
	else
		uiMinuteDuration = CTime::DEFAULT_MINUTE_DURATION;

	g_pTrafficLights->Reset();
	pBitStream->Read(ucTrafficLightState);
	pBitStream->Read(uiTrafficLightTimePassed);

	if(ucTrafficLightState != CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
	{
		g_pTrafficLights->SetLocked(pBitStream->ReadBit());
		if(pBitStream->ReadBit())
		{
			if(ucTrafficLightState >= CTrafficLights::TRAFFIC_LIGHT_STATE_FLASHING_FLASHING)
			{
				pBitStream->Read(uiYellowDuration);
				g_pTrafficLights->SetGreenDuration(uiYellowDuration);
			}
			else
			{
				pBitStream->Read(uiGreenDuration);
				pBitStream->Read(uiYellowDuration);
				pBitStream->Read(uiRedDuration);
				g_pTrafficLights->SetGreenDuration(uiGreenDuration);
				g_pTrafficLights->SetYellowDuration(uiYellowDuration);
				g_pTrafficLights->SetRedDuration(uiRedDuration);
			}
		}
	}
	g_pTrafficLights->SetState((CTrafficLights::eTrafficLightState)ucTrafficLightState);
	g_pTrafficLights->SetTimeThisCycle(uiTrafficLightTimePassed);

	g_pFileTransfer->SetServerInformation(sHttpServer.IsEmpty() ? g_strHost : sHttpServer, usHttpPort);

	g_pNetworkManager->SetHostName(sHostName);
	g_pPlayerManager->SetLocalPlayer(playerId, g_pLocalPlayer);
	g_pLocalPlayer->SetName(g_strNick);
	g_pLocalPlayer->SetColor(color);
	Scripting::SetNoResprays(!bPayAndSpray);
	Scripting::DisablePlayerLockon(0, !bAutoAim);
	CGame::GetWeather()->SetWeather((eWeather)(ucWeather - 1));
	g_pTime->SetMinuteDuration(uiMinuteDuration);
	g_pTime->SetTime(ucHour, ucMinute);
	g_pNetworkManager->SetJoinedServer(true);

	g_pChatWindow->AddInfoMessage("Successfully joined %s.", sHostName.C_String());
	CGame::SetInputState(true);
}

void CClientRPCHandler::NewPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CLogFile::Printf("NewPlayer - 1");
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	CLogFile::Printf("NewPlayer - 2");

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed(playerId);
	CLogFile::Printf("NewPlayer - 3");

	// Read the color
	unsigned int color;
	pBitStream->Read(color);
	CLogFile::Printf("NewPlayer - 4");

	// Read the player name
	String sPlayerName;
	pBitStream->Read(sPlayerName);
	CLogFile::Printf("NewPlayer - 5");

	// Add the player to the player manager
	g_pPlayerManager->Add(playerId, sPlayerName);
	g_pPlayerManager->GetAt(playerId)->SetColor(color);
	CLogFile::Printf("NewPlayer - 6");
}

void CClientRPCHandler::DeletePlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CLogFile::Printf("DeletePlayer - 1");
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	CLogFile::Printf("DeletePlayer - 2");

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed(playerId);
	CLogFile::Printf("DeletePlayer - 3");

	// Remove the player from the player manager
	g_pPlayerManager->Remove(playerId);
	CLogFile::Printf("DeletePlayer - 4");
}

void CClientRPCHandler::NewVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	// Read the model id
	int iModelId;
	pBitStream->Read(iModelId);

	// Read the vehicle health
	unsigned int uiHealth;
	pBitStream->Read(uiHealth);

	// Read the position
	CVector3 vecPosition;
	pBitStream->Read(vecPosition);

	// Read the rotation (if we have it)
	CVector3 vecRotation;

	if(pBitStream->ReadBit())
		pBitStream->Read(vecRotation);

	// Read the turn speed vector (if we have it)
	CVector3 vecTurnSpeed;

	if(pBitStream->ReadBit())
		pBitStream->Read(vecTurnSpeed);

	// Read the move speed vector (if we have it)
	CVector3 vecMoveSpeed;

	if(pBitStream->ReadBit())
		pBitStream->Read(vecMoveSpeed);

	// Read the colors
	BYTE byteColors[4];
	pBitStream->Read((char *)byteColors, sizeof(byteColors));

	// Read the dirt level
	float fDirtLevel;
	pBitStream->Read(fDirtLevel);

	// Read the indicator states
	bool bIndicatorStateFrontLeft  = pBitStream->ReadBit();
	bool bIndicatorStateFrontRight = pBitStream->ReadBit();
	bool bIndicatorStateBackLeft   = pBitStream->ReadBit();
	bool bIndicatorStateBackRight  = pBitStream->ReadBit();

	// Read the components
	bool bComponents[9] = {0};
	for(int i = 0; i < 9; ++ i)
		bComponents[i] = pBitStream->ReadBit();

	// Read the horn duration
	int iHornDuration;
	pBitStream->Read(iHornDuration);

	// Read the siren state flag
	bool bSirenState = pBitStream->ReadBit();

	// Read the lock state
	unsigned char ucLocked;
	pBitStream->Read(ucLocked);

	// Read the variation
	unsigned char ucVariation = 0;
	if(pBitStream->ReadBit())
		pBitStream->Read(ucVariation);

	// Create the new vehicle
	CNetworkVehicle * pVehicle = new CNetworkVehicle(g_pModelManager->VehicleIdToModelHash(iModelId));

	// Add the vehicle to the vehicle manager
	g_pVehicleManager->Add(vehicleId, pVehicle);

	// Set the vehicle id
	pVehicle->SetVehicleId(vehicleId);

	// Set the vehicle position
	pVehicle->SetPosition(vecPosition);

	// Set the vehicle rotation
	pVehicle->SetRotation(vecRotation);

	// Set the vehicle colors
	pVehicle->SetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);

	// Set the vehicle health
	pVehicle->SetHealth(uiHealth);

	// Set the vehicle turn speed vector
	pVehicle->SetTurnSpeed(&vecTurnSpeed);

	// Set the vehicle move speed vector
	pVehicle->SetMoveSpeed(&vecMoveSpeed);

	// Set the vehicle dirt level
	pVehicle->SetDirtLevel(fDirtLevel);

	// Set the vehicle indicators
	pVehicle->SetIndicatorState(bIndicatorStateFrontLeft, bIndicatorStateFrontRight, bIndicatorStateBackLeft, bIndicatorStateBackRight);

	// Set the components
	for(unsigned char i = 0; i < 9; ++ i)
		pVehicle->SetComponentState(i, bComponents[i]);

	// Set the vehicle horn duration (this isn't right as the server will
	// send us the initial horn duration value and not the actual horn
	// duration value (it will decrease as the horn sounds))
	//pVehicle->SoundHorn(iHornDuration);

	// Set the vehicle siren state
	pVehicle->SetSirenState(bSirenState);

	// set the locked state
	pVehicle->SetDoorLockState(ucLocked);

	// set the variation
	pVehicle->SetVariation(ucVariation);

	// Flag the vehicle as can be streamed in
	pVehicle->SetCanBeStreamedIn(true);
}

void CClientRPCHandler::DeleteVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	// If the vehicle exists remove it from the vehicle manager
	if(g_pVehicleManager->Exists(vehicleId))
		g_pVehicleManager->Delete(vehicleId);
}

void CClientRPCHandler::NewObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	DWORD dwModelHash;
	CVector3 vecPos;
	CVector3 vecRot;

	// Read the object id
	EntityId objectId;

	while(pBitStream->ReadCompressed(objectId))
	{
		// Read the model hash
		pBitStream->Read(dwModelHash);

		// Read the position
		pBitStream->Read(vecPos);

		// Read the rotation
		pBitStream->Read(vecRot);

		// Create the object
		CObject * pObject = new CObject(dwModelHash, vecPos, vecRot);

		// Add the object to the object manager
		g_pObjectManager->Add(objectId, pObject);

		// Flag the object as can be streamed in
		pObject->SetCanBeStreamedIn(true);
	}
}

void CClientRPCHandler::DeleteObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the object id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	// Delete the object from the object manager
	g_pObjectManager->Delete(objectId);
}

void CClientRPCHandler::NewPickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	DWORD dwModelHash;
	CVector3 vecPos;
	CVector3 vecRot;
	unsigned char ucType;
	unsigned int uiValue;

	// Read the pickup id
	EntityId pickupId;

	while(pBitStream->ReadCompressed(pickupId))
	{
		// Read the model hash
		pBitStream->Read(dwModelHash);

		// Read the position
		pBitStream->Read(vecPos);

		// Read the rotation
		pBitStream->Read(vecRot);

		// Read the type
		pBitStream->Read(ucType);

		// Read the value
		pBitStream->Read(uiValue);

		// Create the pickup
		CPickup * pPickup = new CPickup(dwModelHash, ucType, uiValue, vecPos, vecRot);

		// Add the pickup to the pickup manager
		g_pPickupManager->Add(pickupId, pPickup);

		// Flag the pickup as can be streamed in
		pPickup->SetCanBeStreamedIn(true);
	}
}

void CClientRPCHandler::DeletePickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the object id
	EntityId pickupId;
	pBitStream->ReadCompressed(pickupId);

	// Delete the object from the object manager
	g_pPickupManager->Delete(pickupId);
}


void CClientRPCHandler::NewBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int iSpriteId;
	CVector3 vecPosition;
	unsigned int uiColor;
	float fSize;
	bool bShortRange;
	bool bRouteBlip;
	String strName;

	// Read the blip id
	EntityId blipId;

	while(pBitStream->ReadCompressed(blipId))
	{
		// Read the sprite id
		pBitStream->Read(iSpriteId);

		// Read the position
		pBitStream->Read(vecPosition);

		// Read the color
		pBitStream->Read(uiColor);

		// Read the size
		pBitStream->Read(fSize);

		//Read the shortRangeTag
		pBitStream->Read(bShortRange);

		//Read if it's a route Blip
		pBitStream->Read(bRouteBlip);

		// Read the name
		pBitStream->Read(strName);

		// Create the blip in the blip manager
		g_pBlipManager->Create(blipId, iSpriteId, vecPosition);
		g_pBlipManager->SetColor(blipId, uiColor); 
		g_pBlipManager->SetSize(blipId, fSize);
		g_pBlipManager->ToggleShortRange(blipId, bShortRange);
		g_pBlipManager->ToggleRouteBlip(blipId, bRouteBlip);
		g_pBlipManager->SetName(blipId, strName);
	}
}

void CClientRPCHandler::DeleteBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->ReadCompressed(blipId);

	// Delete the blip from the blip manager
	g_pBlipManager->Delete(blipId);
}

void CClientRPCHandler::NewActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	CVector3 vecPosition;
	int iModelId;
	float fHeading;

	while(pBitStream->ReadCompressed(actorId))
	{
		pBitStream->Read(iModelId);
		pBitStream->Read(vecPosition);
		pBitStream->Read(fHeading);
		g_pActorManager->Create(actorId, iModelId, vecPosition, fHeading);
	}
}

void CClientRPCHandler::DeleteActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->ReadCompressed(actorId);
	g_pActorManager->Delete(actorId);
}

void CClientRPCHandler::NewCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId checkpointId;
	WORD wType;
	CVector3 vecPosition;
	CVector3 vecTargetPosition;
	float fRadius;

	while(pBitStream->ReadCompressed(checkpointId))
	{
		pBitStream->Read(wType);
		pBitStream->Read(vecPosition);
		pBitStream->Read(vecTargetPosition);
		pBitStream->Read(fRadius);

		// Create the checkpoint
		CCheckpoint * pCheckpoint = new CCheckpoint(checkpointId, (eCheckpointType)wType, vecPosition, vecTargetPosition, fRadius);

		// Add the checkpoint to the checkpoint manager
		g_pCheckpointManager->Add(checkpointId, pCheckpoint);

		// Flag the checkpoint as can be streamed in
		pCheckpoint->SetCanBeStreamedIn(true);
	}
}

void CClientRPCHandler::DeleteCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId checkpointId;
	pBitStream->ReadCompressed(checkpointId);
	g_pCheckpointManager->Delete(checkpointId);
}

void CClientRPCHandler::PlayerSpawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CLogFile::Printf("PlayerSpawn - 1");
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	CLogFile::Printf("PlayerSpawn - 2");

	EntityId playerId;
	int iModelId;
	CVector3 vecSpawnPos;
	float fHeading;
	EntityId vehicleId;
	bool bHelmet;
	pBitStream->ReadCompressed(playerId);
	pBitStream->Read(iModelId);
	pBitStream->Read(bHelmet);
	pBitStream->Read(vecSpawnPos);
	pBitStream->Read(fHeading);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	CLogFile::Printf("PlayerSpawn - 4");

	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		CLogFile::Printf("PlayerSpawn - 5");
		/*if(pPlayer->GetAt()->IsSpawned())
			pPlayer->GetAt()->Destroy();*/
		CLogFile::Printf("PlayerSpawn - 6");

		g_pPlayerManager->Spawn(playerId, iModelId, vecSpawnPos, fHeading);

		if(bHelmet)
			pPlayer->GiveHelmet();

		CLogFile::Printf("PlayerSpawn - 7");
		pBitStream->Read(vehicleId);
		CLogFile::Printf("PlayerSpawn - 8");

		CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

		if(pVehicle)
		{
			BYTE byteVehicleSeatId;
			pBitStream->Read(byteVehicleSeatId);
			pPlayer->PutInVehicle(pVehicle, byteVehicleSeatId);
		}

		// Custom clothing
		if(pBitStream->ReadBit())
		{
			unsigned char ucClothes = 0;

			for(unsigned char uc = 0; uc < 11; ++ uc)
			{
				pBitStream->Read(ucClothes);
				g_pPlayerManager->GetAt(playerId)->SetClothes(uc, ucClothes);
			}
		}

		CLogFile::Printf("PlayerSpawn - 9");
	}
}

void CClientRPCHandler::PlayerDeath(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	pBitStream->ReadCompressed(playerId);
	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer && !pPlayer->IsLocalPlayer())
		pPlayer->Kill();
}

void CClientRPCHandler::ScriptingSetPlayerDucking(CBitStream * pBitStream, CPlayerSocket * senderSocket)
{
	bool bDucking;
	pBitStream->Read(bDucking);

	g_pLocalPlayer->SetDucking(bDucking);
}

void CClientRPCHandler::ScriptingSetPlayerInvincible(CBitStream * pBitStream, CPlayerSocket * senderSocket)
{
	bool bInvincible;
	pBitStream->Read(bInvincible);

	Scripting::SetCharInvincible(g_pLocalPlayer->GetPedHandle(), bInvincible);
}

void CClientRPCHandler::Chat(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	String sChat;
	pBitStream->ReadCompressed(playerId);
	pBitStream->Read(sChat);

	if(g_pPlayerManager->DoesExist(playerId))
		g_pChatWindow->AddChatMessage(playerId, sChat);
}

void CClientRPCHandler::OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	unsigned short usPing;
	OnFootSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->Read((char *)&syncPacket, sizeof(OnFootSyncData));

	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		pPlayer->SetPing(usPing);
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

		if(pRemotePlayer)
		{
			pRemotePlayer->StoreOnFootSync(&syncPacket);

			if(bHasAimSyncData)
				pRemotePlayer->StoreAimSync(&aimSyncPacket);
		}
	}
}

void CClientRPCHandler::InVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	EntityId vehicleId;
	unsigned short usPing;
	InVehicleSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(vehicleId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->Read((char *)&syncPacket, sizeof(InVehicleSyncData));

	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		pPlayer->SetPing(usPing);
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

		if(pRemotePlayer)
		{
			pRemotePlayer->StoreInVehicleSync(vehicleId, &syncPacket);

			if(bHasAimSyncData)
				pRemotePlayer->StoreAimSync(&aimSyncPacket);
		}
	}
}

void CClientRPCHandler::PassengerSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	EntityId vehicleId;
	unsigned short usPing;
	PassengerSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(vehicleId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->Read((char *)&syncPacket, sizeof(PassengerSyncData));
	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		pPlayer->SetPing(usPing);
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

		if(pRemotePlayer)
		{
			pRemotePlayer->StorePassengerSync(vehicleId, &syncPacket);

			if(bHasAimSyncData)
				pRemotePlayer->StoreAimSync(&aimSyncPacket);
		}
	}
}

void CClientRPCHandler::SmallSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	SmallSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->Read((char *)&syncPacket, sizeof(SmallSyncData));
	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer*>(pPlayer);
		if(pRemotePlayer)
		{
			pRemotePlayer->StoreSmallSync(&syncPacket);

			if(bHasAimSyncData)
				pRemotePlayer->StoreAimSync(&aimSyncPacket);
		}
	}
}

void CClientRPCHandler::EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EMPTYVEHICLESYNCPACKET syncPacket;
	pBitStream->Read((PCHAR)&syncPacket, sizeof(EMPTYVEHICLESYNCPACKET));
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(syncPacket.vehicleId);

	if(pVehicle)
		pVehicle->StoreEmptySync(&syncPacket);
}

void CClientRPCHandler::Message(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	DWORD dwColor;
	String sMessage;
	bool bAllowFormatting;
	pBitStream->Read(dwColor);
	pBitStream->Read(sMessage);
	pBitStream->Read(bAllowFormatting);
	g_pChatWindow->AddMessage(dwColor, bAllowFormatting, sMessage);
}

void CClientRPCHandler::ConnectionRefused(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the refuse reason
	int iReason;
	pBitStream->Read(iReason);

	// Add the refuse message and refuse reason to the chat window
	g_pChatWindow->AddInfoMessage("Connection Refused.");

	if(iReason == REFUSE_REASON_INVALIDVERSION)
		g_pChatWindow->AddInfoMessage("You are using an invalid version.");
	else if(iReason == REFUSE_REASON_TOOSHORT)
		g_pChatWindow->AddInfoMessage("Your name is too short.");
	else if(iReason == REFUSE_REASON_TOOLONG)
		g_pChatWindow->AddInfoMessage("Your name is too long.");
	else if(iReason == REFUSE_REASON_INUSE)
		g_pChatWindow->AddInfoMessage("Your name is already in use.");
	else if(iReason == REFUSE_REASON_INVALIDNAME)
		g_pChatWindow->AddInfoMessage("Your name is invalid.");
	else if(iReason == REFUSE_REASON_FILES_MODIFIED)
		g_pChatWindow->AddInfoMessage("Your game files are modified.");

	// Disconnect from the server
	g_pNetworkManager->Disconnect();
}

void CClientRPCHandler::VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for VehicleEnterExit RPC");
		return;
	}

	// Read the player id
	EntityId playerId = INVALID_ENTITY_ID;

	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Read the reply
	bool bReply = pBitStream->ReadBit();

	// If the reply is ok read the reply type
	BYTE byteEnterExitVehicleType = -1;

	if(bReply && !pBitStream->Read(byteEnterExitVehicleType))
		return;

	// If the reply is ok read the vehicle id
	EntityId vehicleId = INVALID_ENTITY_ID;

	if(bReply && !pBitStream->Read(vehicleId))
		return;

	// If the reply is ok and the reply type is enter return read the seat id
	BYTE byteSeatId = 0;

	if(bReply && (byteEnterExitVehicleType == VEHICLE_ENTRY_RETURN) && !pBitStream->Read(byteSeatId))
		return;

	CLogFile::Printf("VehicleEntryExit(Player %d, Reply %d, Type %d, Vehicle %d, Seat %d)", playerId, bReply, byteEnterExitVehicleType, vehicleId, byteSeatId);

	// Get the player pointer
	CNetworkPlayer * pPlayer = NULL;

	// Is it for us?
	if(g_pLocalPlayer->GetPlayerId() == playerId)
	{
		// Get the local player pointer
		pPlayer = g_pLocalPlayer;
	}
	else
	{
		// Get the remote player pointer
		pPlayer = g_pPlayerManager->GetAt(playerId);
	}

	// Is the player pointer invalid?
	if(!pPlayer)
	{
		CLogFile::Printf("Warning: Invalid player for VehicleEnterExit RPC\n");
		return;
	}

	// Set no longer requesting a vehicle entry/exit
	pPlayer->SetRequestingVehicleEnterExit(false);

	// Get the vehicle pointer
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	// Is the vehicle pointer invalid?
	if(!pVehicle)
	{
		CLogFile::Printf("Warning: Invalid vehicle for VehicleEnterExit RPC\n");
		return;
	}

	// Is the reply ok?
	if(bReply)
	{
		// Is it an entry reply
		if(byteEnterExitVehicleType == VEHICLE_ENTRY_RETURN)
		{
			CLogFile::Printf("VehicleEntry(%d, %d, %d)", playerId, vehicleId, byteSeatId);

			// Enter the vehicle
			pPlayer->EnterVehicle(pVehicle, byteSeatId);
		}
		// Is it an entry cancellation?
		if(byteEnterExitVehicleType == VEHICLE_ENTRY_CANCELLED)
		{
			CLogFile::Printf("VehicleEntryCancelled(%d, %d)", playerId, vehicleId);

			// If they are already in the vehicle, remove them, 
			// if not cancel their vehicle entry task
			if(pPlayer->IsInVehicle())
				pPlayer->RemoveFromVehicle();
			else
				pPlayer->ClearVehicleEntryTask();
		}
		// Is it an exit reply?
		else if(byteEnterExitVehicleType == VEHICLE_EXIT_RETURN)
		{
			CLogFile::Printf("VehicleExit(%d, %d)", playerId, vehicleId);

			// Exit the vehicle
			pPlayer->ExitVehicle();
		}
	}
}

void CClientRPCHandler::HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	/*EntityId playerId;
	Vector3 vecAim;
	bitStream->Read(playerId);
	bitStream->Read(vecAim.X);
	bitStream->Read(vecAim.Y);
	bitStream->Read(vecAim.Z);

	if(DoesCharExist(g_pPlayerManager->GetPedHandle(playerId)))
		TaskLookAtCoord(g_pPlayerManager->GetPedHandle(playerId), vecAim.X, vecAim.Y, vecAim.Z, -2, 0);*/
}

// TODO: Merge with PlayerSpawn rpc
void CClientRPCHandler::Spawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Have we joined the game?
	if(g_pNetworkManager->HasJoinedGame())
	{
		// Respawn the local player
		g_pLocalPlayer->Respawn();
	}
}

void CClientRPCHandler::NameChange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	pBitStream->Read(playerId);

	String strName;
	pBitStream->Read(strName);

	if(playerId == g_pLocalPlayer->GetPlayerId())
	{
		g_strNick = strName;
		g_pLocalPlayer->SetName(strName);
		g_pMainMenu->m_pSettingsWindowNickEditBox->setText(strName.Get());
		CVAR_SET_STRING("nick", strName);
	}
	else
	{
		CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
			pPlayer->SetName(strName);
	}
}

void CClientRPCHandler::NewFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the file type
	bool bIsScript;
	pBitStream->Read(bIsScript);

	// Read the file name
	String strFileName;
	pBitStream->Read(strFileName);

	// Read the file checksum
	CFileChecksum fileChecksum;
	pBitStream->Read((char *)&fileChecksum, sizeof(CFileChecksum));

	// Add the file to the file transfer
	g_pFileTransfer->AddFile(strFileName, fileChecksum, !bIsScript);
}

void CClientRPCHandler::DeleteFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the file type
	bool bIsScript;
	pBitStream->Read(bIsScript);

	// Read the name
	String strFileName;
	pBitStream->Read(strFileName);

	CLogFile::Printf(__FILE__,__LINE__,"Delete File: %s", strFileName.C_String());

	if(bIsScript && g_pClientScriptManager->Exists(strFileName))
	{
		g_pClientScriptManager->Unload(strFileName);
		g_pClientScriptManager->RemoveScript(strFileName);
	}
}

void CClientRPCHandler::ScriptingSetPlayerHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned int uHealth;
	pBitStream->Read(uHealth);
	g_pLocalPlayer->SetHealth(uHealth + 100);
}

void CClientRPCHandler::ScriptingSetPlayerCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecPosition;
	pBitStream->Read(vecPosition);
	g_pLocalPlayer->Teleport(&vecPosition);
}

void CClientRPCHandler::ScriptingSetPlayerTime(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned int uiMinuteDuration;
	pBitStream->Read(ucHour);
	pBitStream->Read(ucMinute);

	g_pTime->SetTime(ucHour, ucMinute);
	if(pBitStream->Read(uiMinuteDuration))
		g_pTime->SetMinuteDuration(uiMinuteDuration);
	else
		g_pTime->SetMinuteDuration(CTime::DEFAULT_MINUTE_DURATION);
}

void CClientRPCHandler::ScriptingSetPlayerWeather(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned char ucWeather;
	pBitStream->Read(ucWeather);
	CGame::GetWeather()->SetWeather((eWeather)(ucWeather - 1));
}

void CClientRPCHandler::ScriptingTogglePayAndSpray(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool toggle;
	pBitStream->Read(toggle);
	Scripting::SetNoResprays(!toggle);
}

void CClientRPCHandler::ScriptingToggleAutoAim(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool toggle;
	pBitStream->Read(toggle);
	Scripting::DisablePlayerLockon(0, !toggle);
}

void CClientRPCHandler::ScriptingGivePlayerWeapon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int weapon, ammo;
	pBitStream->Read(weapon);
	pBitStream->Read(ammo);
	g_pLocalPlayer->GiveWeapon(weapon, ammo);
}

void CClientRPCHandler::ScriptingSetSpawnLocation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecPos;
	float r;
	pBitStream->Read(vecPos);
	pBitStream->Read(r);
	g_pLocalPlayer->SetSpawnLocation(vecPos, r);
}

void CClientRPCHandler::ScriptingSetModel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	int iModelId;
	pBitStream->Read(playerId);
	pBitStream->Read(iModelId);
	DWORD dwModelHash = SkinIdToModelHash(iModelId);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = NULL;
		BYTE byteVehicleSeatId = -1;

		if(pPlayer->IsLocalPlayer() && pPlayer->IsInVehicle())
		{
			pVehicle = pPlayer->GetVehicle();
			byteVehicleSeatId = pPlayer->GetVehicleSeatId();
		}

		pPlayer->SetModel(dwModelHash);

		if(pPlayer->IsLocalPlayer())
		{
			if(pVehicle != NULL)
				pPlayer->PutInVehicle(pVehicle, byteVehicleSeatId);
		}
		else
			pPlayer->Init();
	}
}

void CClientRPCHandler::ScriptingToggleControls(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bToggle;
	pBitStream->Read(bToggle);
	CGame::SetInputState(bToggle);
	m_bControlsDisabled = !bToggle;
}

void CClientRPCHandler::ScriptingSetHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	float heading;
	pBitStream->Read(heading);
	g_pLocalPlayer->SetCurrentHeading(heading);
}

void CClientRPCHandler::ScriptingSetPlayerGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	float grav;
	pBitStream->Read(grav);
	Scripting::SetCharGravity(g_pLocalPlayer->GetScriptingHandle(), grav);
}

// Broken :(
/*void CClientRPCHandler::SetPlayerDrunk(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool toggle;
	pBitStream->Read();
	Scripting::SetPedIsDrunk(g_pLocalPlayer->GetPlayerIndex(), !toggle);
}*/


// TODO!!!! - Wont work that way (Error @compiling)
/*void CClientRPCHandler::SetVehicleGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	float grav;
	pBitStream->Read(vehicleId);
	pBitStream->Read(grav);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		Scripting::SetCharGravity(pVehicle, grav);
}*/

void CClientRPCHandler::ScriptingSetVehicleIndicators(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	// Read the indicator states
	unsigned char ucState;
	pBitStream->Read(ucState);
	bool bIndicatorStateFrontLeft  = ( ucState & 0x1 ) != 0;
	bool bIndicatorStateFrontRight = ( ucState & 0x2 ) != 0;
	bool bIndicatorStateBackLeft   = ( ucState & 0x4 ) != 0;
	bool bIndicatorStateBackRight  = ( ucState & 0x8 ) != 0;

	// Set the vehicle indicators
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);
	if(pVehicle)
		pVehicle->SetIndicatorState(bIndicatorStateFrontLeft, bIndicatorStateFrontRight, bIndicatorStateBackLeft, bIndicatorStateBackRight);
}

void CClientRPCHandler::ScriptingSoundVehicleHorn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;

	unsigned int iDuration;

	pBitStream->Read(vehicleId);

	pBitStream->Read(iDuration);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SoundHorn(iDuration);
}

void CClientRPCHandler::ScriptingSetVehicleDirtLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;

	float fLevel;

	pBitStream->Read(vehicleId);

	pBitStream->Read(fLevel);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetDirtLevel(fLevel);
}

void CClientRPCHandler::ScriptingSetVehicleSirenState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	bool bState;
	pBitStream->Read(vehicleId);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetSirenState(bState);
}

void CClientRPCHandler::ScriptingSetVehicleCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	CVector3 vecPos;
	pBitStream->Read(vehicleId);
	pBitStream->Read(vecPos);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetPosition(vecPos);
}

void CClientRPCHandler::ScriptingSetVehicleRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	CVector3 vecRotation;
	pBitStream->Read(vehicleId);
	pBitStream->Read(vecRotation);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetRotation(vecRotation);
}

void CClientRPCHandler::ScriptingSetVehicleColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	BYTE byteColors[4];
	pBitStream->Read(vehicleId);
	pBitStream->Read((char *)byteColors, sizeof(byteColors));
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
}

void CClientRPCHandler::ScriptingToggleRagdoll(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bToggle;
	pBitStream->Read(bToggle);
	g_pLocalPlayer->ToggleRagdoll(bToggle);
}

void CClientRPCHandler::ScriptingGivePlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int amount;
	pBitStream->Read(amount);
	g_pLocalPlayer->GiveMoney(amount);
}

void CClientRPCHandler::ScriptingSetPlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int amount;
	pBitStream->Read(amount);
	g_pLocalPlayer->SetMoney(amount);
}

float fTextPos[2];
String strTextText;
int iTextTime = 0;
DWORD dwTextStartTick = 0;

void CClientRPCHandler::ScriptingDisplayText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// TODO: Text pool
	pBitStream->Read(fTextPos[0]);
	pBitStream->Read(fTextPos[1]);
	pBitStream->Read(strTextText);
	pBitStream->Read(iTextTime);
	dwTextStartTick = SharedUtility::GetTime();
}

void CClientRPCHandler::ScriptingDisplayInfoText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// TODO: Text pool
	String strInfoTextText;
	int iInfoTextTime = 0;
	pBitStream->Read(strInfoTextText);
	pBitStream->Read(iInfoTextTime);
	Scripting::PrintStringWithLiteralStringNow("STRING", strInfoTextText, iInfoTextTime, 1);
}

void CClientRPCHandler::ScriptingToggleFrozen(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bFrozen;
	bool bCameraFrozen;
	pBitStream->Read(bFrozen);
	pBitStream->Read(bCameraFrozen);
	g_pLocalPlayer->SetPlayerControlAdvanced(!bFrozen, !bCameraFrozen);
}

void CClientRPCHandler::ScriptingSetVehicleHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	unsigned int uHealth;
	pBitStream->ReadCompressed(vehicleId);
	pBitStream->Read(uHealth);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetHealth(uHealth);
}

void CClientRPCHandler::ScriptingSetPlayerArmour(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned int uArmour;
	pBitStream->Read(uArmour);
	g_pLocalPlayer->SetArmour(uArmour);
}

void CClientRPCHandler::ScriptingSetPlayerMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecMoveSpeed;
	pBitStream->Read(vecMoveSpeed);
	g_pLocalPlayer->SetMoveSpeed(&vecMoveSpeed);
}

void CClientRPCHandler::ScriptingSetVehicleMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	CVector3 vecMoveSpeed;
	pBitStream->Read(vehicleId);
	pBitStream->Read(vecMoveSpeed);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetMoveSpeed(&vecMoveSpeed);
}

void CClientRPCHandler::ScriptingRemoveWeapons(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pLocalPlayer->RemoveAllWeapons();
}

void CClientRPCHandler::ScriptingSetWantedLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int wantedLevel;
	pBitStream->Read(wantedLevel);
	CGame::SetWantedLevel(wantedLevel);
}

void CClientRPCHandler::ScriptingWarpPlayerIntoVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	BYTE byteSeatId;
	pBitStream->Read(vehicleId);
	pBitStream->Read(byteSeatId);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		g_pLocalPlayer->PutInVehicle(pVehicle, byteSeatId);
}

void CClientRPCHandler::ScriptingRemovePlayerFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bGraceful = pBitStream->ReadBit();

	if(bGraceful)
		g_pLocalPlayer->ExitVehicle();
	else
		g_pLocalPlayer->RemoveFromVehicle();
}

void CClientRPCHandler::ScriptingSetCameraBehindPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pLocalPlayer->SetCameraBehind();
}

void CClientRPCHandler::ScriptingSetActorCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the actor id
	EntityId actorId;
	pBitStream->Read(actorId);

	// Read the position
	CVector3 vecPosition;
	pBitStream->Read(vecPosition);

	// Set the actor position
	g_pActorManager->SetPosition(actorId, vecPosition);
}

void CClientRPCHandler::ScriptingSetActorHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the actor id
	EntityId actorId;
	pBitStream->Read(actorId);

	// Read the heading
	float fHeading;
	pBitStream->Read(fHeading);

	// Set the actor heading
	g_pActorManager->SetHeading(actorId, fHeading);
}

void CClientRPCHandler::ScriptingActorWalkToCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the actor id
	EntityId actorId;
	pBitStream->Read(actorId);

	// Read the position
	float x, y, z;
	pBitStream->Read(x);
	pBitStream->Read(y);
	pBitStream->Read(z);

	// Read the type
	int iType;
	pBitStream->Read(iType);

	// Apply the walk task to the actor
	Scripting::TaskGoStraightToCoord(actorId, x, y, z, 2, 45000);
}

void CClientRPCHandler::ScriptingSetBlipColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->Read(blipId);

	// Read the color
	unsigned int uiColor;
	pBitStream->Read(uiColor);

	// Set the blip color
	g_pBlipManager->SetColor(blipId, uiColor);
}

void CClientRPCHandler::ScriptingSetBlipSize(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->Read(blipId);

	// Read the size
	float fSize;
	pBitStream->Read(fSize);

	// Set the blip size
	g_pBlipManager->SetSize(blipId, fSize);
}

void CClientRPCHandler::ScriptingToggleBlipShortRange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->Read(blipId);

	//Read if it's shortRange
	bool bShortRange;
	pBitStream->Read(bShortRange);

	//Set shortRange
	g_pBlipManager->ToggleShortRange(blipId, bShortRange);
}

void CClientRPCHandler::ScriptingToggleBlipRoute(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->Read(blipId);

	//Read if it's shortRange
	bool bRouteBlip;
	pBitStream->Read(bRouteBlip);

	//Set shortRange
	g_pBlipManager->ToggleRouteBlip(blipId, bRouteBlip);
}

void CClientRPCHandler::ScriptingSetBlipName(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the blip id
	EntityId blipId;
	pBitStream->Read(blipId);

	//Read the name
	String strName;
	pBitStream->Read(strName);

	//Set the blip name
	g_pBlipManager->SetName(blipId, strName);

}

void CClientRPCHandler::ScriptingShowCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the checkpoint id
	EntityId checkpointId;
	pBitStream->ReadCompressed(checkpointId);

	// Set the checkpoint to visible
	CCheckpoint* pCheckpoint = g_pCheckpointManager->Get(checkpointId);
	if(pCheckpoint)
		pCheckpoint->Show();
}

void CClientRPCHandler::ScriptingHideCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the checkpoint id
	EntityId checkpointId;
	pBitStream->ReadCompressed(checkpointId);

	// Set the checkpoint to not visible
	CCheckpoint* pCheckpoint = g_pCheckpointManager->Get(checkpointId);
	if(pCheckpoint)
		pCheckpoint->Hide();
}


void CClientRPCHandler::ScriptingToggleHUD(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the hud toggle
	bool bToggle;
	pBitStream->Read(bToggle);

	// Set the hud visibility
	CGame::SetHudVisible(bToggle);
}

void CClientRPCHandler::ScriptingToggleRadar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the radar toggle
	bool bToggle;
	pBitStream->Read(bToggle);

	// Set the radar visibility
	CGame::SetRadarVisible(bToggle);
}

void CClientRPCHandler::ScriptingToggleNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player names toggle
	bool bToggle;
	pBitStream->Read(bToggle);

	// Set the player names visibility
	g_pNameTags->SetEnabled(bToggle);
}

void CClientRPCHandler::ScriptingToggleAreaNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the area names toggle
	bool bToggle;
	pBitStream->Read(bToggle);

	// Set the area names state
	CGame::SetAreaNamesEnabled(bToggle);
}

void CClientRPCHandler::ScriptingEventCall(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CSquirrelArguments* pArgs = new CSquirrelArguments(pBitStream);
	CSquirrelArgument* pEventName = pArgs->size() > 0 ? pArgs->front() : 0;

	if(pEventName && pEventName->GetType() == OT_STRING)
	{
		const char* szEventName = pEventName->data.str->C_String();
		pArgs->pop_front();

		g_pEvents->Call(szEventName, pArgs);
	}

	delete pEventName;
	delete pArgs;
}

void CClientRPCHandler::ScriptingSetPlayerColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->Read(playerId);

	// Read the color
	unsigned int uiColor;
	pBitStream->Read(uiColor);

	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
		pPlayer->SetColor(uiColor);
}

void CClientRPCHandler::ScriptingSetVehicleLocked(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	unsigned char ucLocked;
	pBitStream->Read(vehicleId);
	pBitStream->Read(ucLocked);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetDoorLockState(ucLocked);
}

void CClientRPCHandler::ScriptingSetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->Read(playerId);

	// Read the clothes set
	unsigned char ucBodyPart, ucClothes;
	pBitStream->Read(ucBodyPart);
	pBitStream->Read(ucClothes);

	// Set the clothes
	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
		pPlayer->SetClothes(ucBodyPart, ucClothes);
}

void CClientRPCHandler::ScriptingResetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->Read(playerId);

	// Remove the clothes
	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
		for(unsigned char uc = 0; uc < 11; ++ uc)
			pPlayer->SetClothes(uc, 0);
}

void CClientRPCHandler::ScriptingGiveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->Read(playerId);

	// Give the helmet
	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
			pPlayer->GiveHelmet();
}

void CClientRPCHandler::ScriptingRemoveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->Read(playerId);

	// Remove the helmet
	CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
			pPlayer->RemoveHelmet();
}

void CClientRPCHandler::ScriptingSetTrafficLightState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned char ucState;
	CTrafficLights::eTrafficLightState eState;

	// read the state
	pBitStream->Read(ucState);
	eState = (CTrafficLights::eTrafficLightState)ucState;

	if(eState != CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
	{
		// set locked
		g_pTrafficLights->SetLocked(pBitStream->ReadBit());

		unsigned int uiYellowDuration;
		if(eState >= CTrafficLights::TRAFFIC_LIGHT_STATE_FLASHING_FLASHING && eState < CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
		{
			// yellow flashing duration
			if(pBitStream->Read(uiYellowDuration))
				g_pTrafficLights->SetYellowDuration(uiYellowDuration);
			else
				g_pTrafficLights->SetYellowDuration(CTrafficLights::DEFAULT_YELLOW_DURATION);
		}
		else if(eState < CTrafficLights::TRAFFIC_LIGHT_STATE_TO_GREEN_RED)
		{
			// traffic cycle durations
			unsigned int uiGreenDuration, uiRedDuration;
			if(pBitStream->Read(uiGreenDuration))
			{
				pBitStream->Read(uiYellowDuration);
				pBitStream->Read(uiRedDuration);
				g_pTrafficLights->SetGreenDuration(uiGreenDuration);
				g_pTrafficLights->SetYellowDuration(uiYellowDuration);
				g_pTrafficLights->SetRedDuration(uiRedDuration);
			}
			else
			{
				g_pTrafficLights->SetGreenDuration(CTrafficLights::DEFAULT_GREEN_DURATION);
				g_pTrafficLights->SetYellowDuration(CTrafficLights::DEFAULT_YELLOW_DURATION);
				g_pTrafficLights->SetRedDuration(CTrafficLights::DEFAULT_RED_DURATION);
			}
		}
	}
	g_pTrafficLights->SetState(eState);
}

void CClientRPCHandler::ScriptingSetVehicleComponents(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);
	if(pVehicle != NULL)
	{
		// Read the component values
		for(unsigned char i = 0; i < 9; ++ i)
		{
			bool bOn = pBitStream->ReadBit();
			pVehicle->SetComponentState(i, bOn);
		}
	}
}

void CClientRPCHandler::ScriptingSetVehicleVariation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);
	if(pVehicle != NULL)
	{
		// Read the variation
		unsigned char ucVariation;
		pBitStream->Read(ucVariation);
		pVehicle->SetVariation(ucVariation);
	}
}

void CClientRPCHandler::ScriptingSetObjectPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the object id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	CObject* pObject = g_pObjectManager->Get(objectId);
	if(pObject)
	{
		// Read the position
		CVector3 vecPosition;
		pBitStream->Read(vecPosition);

		// Set the position
		pObject->SetPosition(vecPosition);
	}
}

void CClientRPCHandler::ScriptingSetObjectRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the object id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	CObject* pObject = g_pObjectManager->Get(objectId);
	if(pObject)
	{
		// Read the rotation
		CVector3 vecRotation;
		pBitStream->Read(vecRotation);

		// Set the rotation
		pObject->SetRotation(vecRotation);
	}
}

void CClientRPCHandler::ScriptingSetPickupPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the pickup id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	CPickup* pPickup = g_pPickupManager->Get(objectId);
	if(pPickup)
	{
		// Read the position
		CVector3 vecPosition;
		pBitStream->Read(vecPosition);

		// Set the position
		pPickup->SetPosition(vecPosition);
	}
}

void CClientRPCHandler::ScriptingSetPickupRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the pickup id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	CPickup* pPickup = g_pPickupManager->Get(objectId);
	if(pPickup)
	{
		// Read the rotation
		CVector3 vecRotation;
		pBitStream->Read(vecRotation);

		// Set the rotation
		pPickup->SetRotation(vecRotation);
	}
}

void CClientRPCHandler::Register()
{
	// Network
	AddFunction(RPC_JoinedGame, JoinedGame);
	AddFunction(RPC_NewPlayer, NewPlayer);
	AddFunction(RPC_DeletePlayer, DeletePlayer);
	AddFunction(RPC_NewVehicle, NewVehicle);
	AddFunction(RPC_DeleteVehicle, DeleteVehicle);
	AddFunction(RPC_NewObject, NewObject);
	AddFunction(RPC_DeleteObject, DeleteObject);
	AddFunction(RPC_NewBlip, NewBlip);
	AddFunction(RPC_DeleteBlip, DeleteBlip);
	AddFunction(RPC_NewActor, NewActor);
	AddFunction(RPC_DeleteActor, DeleteActor);
	AddFunction(RPC_NewCheckpoint, NewCheckpoint);
	AddFunction(RPC_DeleteCheckpoint, DeleteCheckpoint);
	AddFunction(RPC_PlayerSpawn, PlayerSpawn);
	AddFunction(RPC_PlayerDeath, PlayerDeath);
	AddFunction(RPC_Chat, Chat);
	AddFunction(RPC_OnFootSync, OnFootSync);
	AddFunction(RPC_InVehicleSync, InVehicleSync);
	AddFunction(RPC_PassengerSync, PassengerSync);
	AddFunction(RPC_SmallSync, SmallSync);
	AddFunction(RPC_EmptyVehicleSync, EmptyVehicleSync);
	AddFunction(RPC_Message, Message);
	AddFunction(RPC_ConnectionRefused, ConnectionRefused);
	AddFunction(RPC_VehicleEnterExit, VehicleEnterExit);
	AddFunction(RPC_HeadMovement, HeadMovement);
	AddFunction(RPC_Spawn, Spawn);
	AddFunction(RPC_NameChange, NameChange);
	AddFunction(RPC_NewFile, NewFile);
	AddFunction(RPC_DeleteFile, DeleteFile);
	AddFunction(RPC_NewPickup, NewPickup);
	AddFunction(RPC_DeletePickup, DeletePickup);

	// Scripting
	AddFunction(RPC_ScriptingSetPlayerHealth, ScriptingSetPlayerHealth);
	AddFunction(RPC_ScriptingSetPlayerCoordinates, ScriptingSetPlayerCoordinates);
	AddFunction(RPC_ScriptingSetPlayerTime, ScriptingSetPlayerTime);
	AddFunction(RPC_ScriptingSetPlayerWeather, ScriptingSetPlayerWeather);
	AddFunction(RPC_ScriptingTogglePayAndSpray, ScriptingTogglePayAndSpray);
	AddFunction(RPC_ScriptingToggleAutoAim, ScriptingToggleAutoAim);
	//AddFunction(RPC_SetPlayerDrunk, SetPlayerDrunk);
	AddFunction(RPC_ScriptingGivePlayerWeapon, ScriptingGivePlayerWeapon);
	AddFunction(RPC_ScriptingSetPlayerGravity, ScriptingSetPlayerGravity);
	AddFunction(RPC_ScriptingSetSpawnLocation, ScriptingSetSpawnLocation);

	AddFunction(RPC_ScriptingSetVehicleIndicators, ScriptingSetVehicleIndicators);
	AddFunction(RPC_ScriptingSoundVehicleHorn, ScriptingSoundVehicleHorn);

	AddFunction(RPC_ScriptingGiveHelmet, ScriptingGiveHelmet);
	AddFunction(RPC_ScriptingRemoveHelmet, ScriptingRemoveHelmet);

	AddFunction(RPC_ScriptingSetModel, ScriptingSetModel);
	AddFunction(RPC_ScriptingToggleControls, ScriptingToggleControls);
	AddFunction(RPC_ScriptingSetHeading, ScriptingSetHeading);
	AddFunction(RPC_ScriptingSetVehicleDirtLevel, ScriptingSetVehicleDirtLevel);
	AddFunction(RPC_ScriptingSetVehicleSirenState, ScriptingSetVehicleSirenState);
	AddFunction(RPC_ScriptingSetVehicleCoordinates, ScriptingSetVehicleCoordinates);
	AddFunction(RPC_ScriptingSetVehicleRotation, ScriptingSetVehicleRotation);
	AddFunction(RPC_ScriptingSetVehicleColor, ScriptingSetVehicleColor);
	AddFunction(RPC_ScriptingToggleRagdoll, ScriptingToggleRagdoll);
	AddFunction(RPC_ScriptingGivePlayerMoney, ScriptingGivePlayerMoney);
	AddFunction(RPC_ScriptingSetPlayerMoney, ScriptingSetPlayerMoney);
	AddFunction(RPC_ScriptingDisplayText, ScriptingDisplayText);
	AddFunction(RPC_ScriptingDisplayInfoText, ScriptingDisplayInfoText);
	AddFunction(RPC_ScriptingToggleFrozen, ScriptingToggleFrozen);
	AddFunction(RPC_ScriptingSetVehicleHealth, ScriptingSetVehicleHealth);
	AddFunction(RPC_ScriptingSetPlayerArmour, ScriptingSetPlayerArmour);
	AddFunction(RPC_ScriptingSetPlayerMoveSpeed, ScriptingSetPlayerMoveSpeed);
	AddFunction(RPC_ScriptingSetVehicleMoveSpeed, ScriptingSetVehicleMoveSpeed);
	AddFunction(RPC_ScriptingRemoveWeapons, ScriptingRemoveWeapons);
	AddFunction(RPC_ScriptingSetWantedLevel, ScriptingSetWantedLevel);
	AddFunction(RPC_ScriptingWarpPlayerIntoVehicle, ScriptingWarpPlayerIntoVehicle);
	AddFunction(RPC_ScriptingRemovePlayerFromVehicle, ScriptingRemovePlayerFromVehicle);
	AddFunction(RPC_ScriptingSetCameraBehindPlayer, ScriptingSetCameraBehindPlayer);
	AddFunction(RPC_ScriptingSetPlayerDucking, ScriptingSetPlayerDucking);
	AddFunction(RPC_ScriptingSetPlayerInvincible, ScriptingSetPlayerInvincible);
	AddFunction(RPC_ScriptingSetActorCoordinates, ScriptingSetActorCoordinates);
	AddFunction(RPC_ScriptingSetActorHeading, ScriptingSetActorHeading);
	AddFunction(RPC_ScriptingActorWalkToCoordinates, ScriptingActorWalkToCoordinates);
	AddFunction(RPC_ScriptingSetBlipColor, ScriptingSetBlipColor);
	AddFunction(RPC_ScriptingSetBlipSize, ScriptingSetBlipSize);
	AddFunction(RPC_ScriptingToggleBlipShortRange, ScriptingToggleBlipShortRange);
	AddFunction(RPC_ScriptingToggleBlipRoute, ScriptingToggleBlipRoute);
	AddFunction(RPC_ScriptingSetBlipName, ScriptingSetBlipName);
	AddFunction(RPC_ScriptingShowCheckpointForPlayer, ScriptingShowCheckpointForPlayer);
	AddFunction(RPC_ScriptingHideCheckpointForPlayer, ScriptingHideCheckpointForPlayer);
	AddFunction(RPC_ScriptingToggleHUD, ScriptingToggleHUD);
	AddFunction(RPC_ScriptingToggleRadar, ScriptingToggleRadar);
	AddFunction(RPC_ScriptingToggleNames, ScriptingToggleNames);
	AddFunction(RPC_ScriptingToggleAreaNames, ScriptingToggleAreaNames);
	AddFunction(RPC_ScriptingEventCall, ScriptingEventCall);
	AddFunction(RPC_ScriptingSetPlayerColor, ScriptingSetPlayerColor);
	AddFunction(RPC_ScriptingSetVehicleLocked, ScriptingSetVehicleLocked);
	AddFunction(RPC_ScriptingSetPlayerClothes, ScriptingSetPlayerClothes);
	AddFunction(RPC_ScriptingResetPlayerClothes, ScriptingResetPlayerClothes);
	AddFunction(RPC_ScriptingSetTrafficLightState, ScriptingSetTrafficLightState);
	AddFunction(RPC_ScriptingSetVehicleComponents, ScriptingSetVehicleComponents);
	AddFunction(RPC_ScriptingSetVehicleVariation, ScriptingSetVehicleVariation);
	AddFunction(RPC_ScriptingSetObjectPosition, ScriptingSetObjectPosition);
	AddFunction(RPC_ScriptingSetObjectRotation, ScriptingSetObjectRotation);
	AddFunction(RPC_ScriptingSetPickupPosition, ScriptingSetPickupPosition);
	AddFunction(RPC_ScriptingSetPickupRotation, ScriptingSetPickupRotation);
}

void CClientRPCHandler::Unregister()
{
	// Network
	RemoveFunction(RPC_JoinedGame);
	RemoveFunction(RPC_NewPlayer);
	RemoveFunction(RPC_DeletePlayer);
	RemoveFunction(RPC_NewVehicle);
	RemoveFunction(RPC_DeleteVehicle);
	RemoveFunction(RPC_NewObject);
	RemoveFunction(RPC_DeleteObject);
	RemoveFunction(RPC_NewBlip);
	RemoveFunction(RPC_DeleteBlip);
	RemoveFunction(RPC_NewActor);
	RemoveFunction(RPC_DeleteActor);
	RemoveFunction(RPC_NewCheckpoint);
	RemoveFunction(RPC_DeleteCheckpoint);
	RemoveFunction(RPC_PlayerSpawn);
	RemoveFunction(RPC_PlayerDeath);
	RemoveFunction(RPC_Chat);
	RemoveFunction(RPC_OnFootSync);
	RemoveFunction(RPC_InVehicleSync);
	RemoveFunction(RPC_PassengerSync);
	RemoveFunction(RPC_SmallSync);
	RemoveFunction(RPC_EmptyVehicleSync);
	RemoveFunction(RPC_Message);
	RemoveFunction(RPC_ConnectionRefused);
	RemoveFunction(RPC_VehicleEnterExit);
	RemoveFunction(RPC_HeadMovement);
	RemoveFunction(RPC_Spawn);
	RemoveFunction(RPC_NameChange);
	RemoveFunction(RPC_NewFile);
	RemoveFunction(RPC_DeleteFile);
	RemoveFunction(RPC_DeletePickup);
	RemoveFunction(RPC_NewPickup);

	// Scripting
	RemoveFunction(RPC_ScriptingSetPlayerHealth);
	RemoveFunction(RPC_ScriptingSetPlayerCoordinates);
	RemoveFunction(RPC_ScriptingSetPlayerTime);
	RemoveFunction(RPC_ScriptingSetPlayerWeather);
	RemoveFunction(RPC_ScriptingTogglePayAndSpray);
	RemoveFunction(RPC_ScriptingToggleAutoAim);
	//RemoveFunction(RPC_SetPlayerDrunk);
	RemoveFunction(RPC_ScriptingSetPlayerGravity);
	RemoveFunction(RPC_ScriptingGivePlayerWeapon);
	RemoveFunction(RPC_ScriptingSetSpawnLocation);

	RemoveFunction(RPC_ScriptingSetVehicleIndicators);
	RemoveFunction(RPC_ScriptingSoundVehicleHorn);

	RemoveFunction(RPC_ScriptingGiveHelmet);
	RemoveFunction(RPC_ScriptingRemoveHelmet);

	RemoveFunction(RPC_ScriptingSetModel);
	RemoveFunction(RPC_ScriptingToggleControls);
	RemoveFunction(RPC_ScriptingSetHeading);
	RemoveFunction(RPC_ScriptingSetVehicleDirtLevel);
	RemoveFunction(RPC_ScriptingSetVehicleCoordinates);
	RemoveFunction(RPC_ScriptingSetVehicleRotation);
	RemoveFunction(RPC_ScriptingSetVehicleColor);
	RemoveFunction(RPC_ScriptingToggleRagdoll);
	RemoveFunction(RPC_ScriptingGivePlayerMoney);
	RemoveFunction(RPC_ScriptingSetPlayerMoney);
	RemoveFunction(RPC_ScriptingDisplayText);
	RemoveFunction(RPC_ScriptingDisplayInfoText);
	RemoveFunction(RPC_ScriptingToggleFrozen);
	RemoveFunction(RPC_ScriptingSetVehicleHealth);
	RemoveFunction(RPC_ScriptingSetPlayerArmour);
	RemoveFunction(RPC_ScriptingSetPlayerMoveSpeed);
	RemoveFunction(RPC_ScriptingSetVehicleMoveSpeed);
	RemoveFunction(RPC_ScriptingRemoveWeapons);
	RemoveFunction(RPC_ScriptingSetWantedLevel);
	RemoveFunction(RPC_ScriptingWarpPlayerIntoVehicle);
	RemoveFunction(RPC_ScriptingRemovePlayerFromVehicle);
	RemoveFunction(RPC_ScriptingSetCameraBehindPlayer);
	RemoveFunction(RPC_ScriptingSetActorCoordinates);
	RemoveFunction(RPC_ScriptingSetActorHeading);
	RemoveFunction(RPC_ScriptingActorWalkToCoordinates);
	RemoveFunction(RPC_ScriptingSetBlipColor);
	RemoveFunction(RPC_ScriptingSetBlipSize);
	RemoveFunction(RPC_ScriptingToggleBlipShortRange);
	RemoveFunction(RPC_ScriptingToggleBlipRoute);
	RemoveFunction(RPC_ScriptingSetBlipName);
	RemoveFunction(RPC_ScriptingShowCheckpointForPlayer);
	RemoveFunction(RPC_ScriptingHideCheckpointForPlayer);
	RemoveFunction(RPC_ScriptingToggleHUD);
	RemoveFunction(RPC_ScriptingToggleRadar);
	RemoveFunction(RPC_ScriptingToggleNames);
	RemoveFunction(RPC_ScriptingToggleAreaNames);
	RemoveFunction(RPC_ScriptingEventCall);
	RemoveFunction(RPC_ScriptingSetPlayerColor);
	RemoveFunction(RPC_ScriptingSetVehicleLocked);
	RemoveFunction(RPC_ScriptingSetPlayerClothes);
	RemoveFunction(RPC_ScriptingResetPlayerClothes);
	RemoveFunction(RPC_ScriptingSetTrafficLightState);
	RemoveFunction(RPC_ScriptingSetVehicleComponents);
	RemoveFunction(RPC_ScriptingSetVehicleVariation);
	RemoveFunction(RPC_ScriptingSetObjectPosition);
	RemoveFunction(RPC_ScriptingSetObjectRotation);
	RemoveFunction(RPC_ScriptingSetPickupPosition);
	RemoveFunction(RPC_ScriptingSetPickupRotation);
}
