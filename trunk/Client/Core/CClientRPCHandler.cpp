//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientRPCHandler.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClient.h"
#include "CNetworkPlayer.h"
#include "CRemotePlayer.h"
#include <CSettings.h>
#include "Scripting.h"
#include <SharedUtility.h>
#include "COffsets.h"
#include "CIVWeather.h"
#include "CFireManager.h"
#include "CGame.h"

extern CClient * g_pClient;

bool m_bControlsDisabled = false;

void CClientRPCHandler::JoinedGame(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	String sHostName, sHttpServer;
	bool bPayAndSpray, bAutoAim, bGUINametags, bHeadMovement, bSpecialData1, bSpecialData2;
	unsigned short usHttpPort;
	unsigned char ucWeather, ucTrafficLightState, ucHour, ucMinute;
	unsigned int uiColor, uiMaxPlayers, uiMinuteDuration, uiTrafficLightTimePassed, uiGreenDuration, uiYellowDuration, uiRedDuration;

	pBitStream->Read(playerId);
	pBitStream->Read(sHostName);
	bPayAndSpray = pBitStream->ReadBit();
	bAutoAim = pBitStream->ReadBit();
	pBitStream->Read(uiColor);

	pBitStream->Read(sHttpServer);
	pBitStream->Read(usHttpPort);
	pBitStream->Read(ucWeather);
	bGUINametags = pBitStream->ReadBit();
	bSpecialData1 = pBitStream->ReadBit();
	bSpecialData2 = pBitStream->ReadBit();
	bHeadMovement = pBitStream->ReadBit();
	pBitStream->Read(uiMaxPlayers);

	pBitStream->Read(ucHour);
	pBitStream->Read(ucMinute);

	if(pBitStream->ReadBit())
		pBitStream->Read(uiMinuteDuration);
	else
		uiMinuteDuration = CTime::DEFAULT_MINUTE_DURATION;

	CTrafficLights * pTrafficLights = g_pClient->GetTrafficLights();
	pTrafficLights->Reset();
	pBitStream->Read(ucTrafficLightState);
	pBitStream->Read(uiTrafficLightTimePassed);

	if(ucTrafficLightState != CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
	{
		pTrafficLights->SetLocked(pBitStream->ReadBit());

		if(pBitStream->ReadBit())
		{
			if(ucTrafficLightState >= CTrafficLights::TRAFFIC_LIGHT_STATE_FLASHING_FLASHING)
			{
				pBitStream->Read(uiYellowDuration);
				pTrafficLights->SetGreenDuration(uiYellowDuration);
			}
			else
			{
				pBitStream->Read(uiGreenDuration);
				pBitStream->Read(uiYellowDuration);
				pBitStream->Read(uiRedDuration);
				pTrafficLights->SetGreenDuration(uiGreenDuration);
				pTrafficLights->SetYellowDuration(uiYellowDuration);
				pTrafficLights->SetRedDuration(uiRedDuration);
			}
		}
	}

	pTrafficLights->SetState((CTrafficLights::eTrafficLightState)ucTrafficLightState);
	pTrafficLights->SetTimeThisCycle(uiTrafficLightTimePassed);

	// Get our file transfer manager
	CFileTransferManager * pFileTransferManager = g_pClient->GetFileTransfer();
	pFileTransferManager->SetHost(sHttpServer.IsEmpty() ? g_pClient->GetHost() : sHttpServer);
	pFileTransferManager->SetPort(usHttpPort);

	CGame::SetNameTags(bGUINametags);
	CGame::SetSpecialData(bSpecialData1, bSpecialData2);
	CGame::SetHeadMovement(bHeadMovement);
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->SetHostName(sHostName);
	pNetworkManager->SetMaxPlayers(uiMaxPlayers);
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();
	g_pClient->GetPlayerManager()->SetLocalPlayer(playerId, pLocalPlayer);
	pLocalPlayer->SetName(g_pClient->GetNick());
	pLocalPlayer->SetColor(uiColor);
	Scripting::SetNoResprays(!bPayAndSpray);
	Scripting::DisablePlayerLockon(0, !bAutoAim);
	CGame::GetWeather()->SetWeather((eWeather)(ucWeather - 1));

	// Get our time
	CTime * pTime = g_pClient->GetTime();
	pTime->SetMinuteDuration(uiMinuteDuration);
	pTime->SetTime(ucHour, ucMinute);

	g_pClient->GetNetworkManager()->SetJoinedServer(true);

	// Set the disconnect button visible
	g_pClient->GetMainMenu()->SetDisconnectButtonVisible(true);

	CGame::SetInputState(true);
	CGame::SetKickedFromServer(false);
	CGame::SetState(GAME_STATE_INGAME);
	g_pClient->GetChatWindow()->AddInfoMessage("Successfully joined %s.", sHostName.C_String());
}

void CClientRPCHandler::NewPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed(playerId);

	// Read the color
	unsigned int color;
	pBitStream->Read(color);

	// Read the player name
	String sPlayerName;
	pBitStream->Read(sPlayerName);

	// Get our player manager
	CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();

	// Add the player to the player manager
	pPlayerManager->Add(playerId, sPlayerName);
	pPlayerManager->GetAt(playerId)->SetColor(color);
}

void CClientRPCHandler::DeletePlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed(playerId);

	// Remove the player from the player manager
	g_pClient->GetPlayerManager()->Remove(playerId);
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

	// Read the petroltank health
	float fPetrolTank;
	pBitStream->Read(fPetrolTank);

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
	int iLocked;
	pBitStream->Read(iLocked);

	// Convert int to dword
	DWORD dwLockState = static_cast<DWORD>(iLocked);

	// Read the engine state
	bool bEngineStatus; pBitStream->Read(bEngineStatus);

	// Read if our lights are on
	bool bLights; pBitStream->Read(bLights);

	// Read the door angles
	float fDoor[6];
	for(int i = 0; i <= 5; i++)
		pBitStream->Read(fDoor[i]);

	// Read the window states
	bool bWindow[4];
	for(int i = 0; i <= 3; i++)
		pBitStream->Read(bWindow[i]);
	
	// Read if taxilight is turned on
	bool bTaxiLight;
	pBitStream->Read(bTaxiLight);

	// Read if gps state is turned on
	bool bGPS;
	pBitStream->Read(bGPS);

	// Read the variation
	unsigned char ucVariation = 0;
	if(pBitStream->ReadBit())
		pBitStream->Read(ucVariation);

	// Create the new vehicle
	CNetworkVehicle * pVehicle = new CNetworkVehicle(g_pClient->GetModelManager()->VehicleIdToModelHash(iModelId), iModelId);
	
	// Set the vehicle spawn position
	pVehicle->SetSpawnPosition(vecPosition);

	// Set the vehicle spawn rotation
	pVehicle->SetSpawnRotation(vecRotation);

	// Add the vehicle to the vehicle manager
	g_pClient->GetVehicleManager()->Add(vehicleId, pVehicle);

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
	pVehicle->SetTurnSpeed(vecTurnSpeed);

	// Set the vehicle move speed vector
	pVehicle->SetMoveSpeed(vecMoveSpeed);

	// Set the vehicle dirt level
	pVehicle->SetDirtLevel(fDirtLevel);

	// Set the vehicle indicators
	pVehicle->SetIndicatorState(bIndicatorStateFrontLeft, bIndicatorStateFrontRight, bIndicatorStateBackLeft, bIndicatorStateBackRight);

	// Set the components
	for(int i = 0; i < 9; ++ i)
		pVehicle->SetComponentState(i, bComponents[i]);

	// Sound horn if needed
	pVehicle->SoundHorn(iHornDuration);

	// Set the locked state
	pVehicle->SetDoorLockState(dwLockState);

	// Set the vehicle siren state
	pVehicle->SetSirenState(bSirenState);

	// Set the variation
	pVehicle->SetVariation(ucVariation);

	// Set the engine status
	pVehicle->SetEngineState(bEngineStatus);
	
	// Set the lights
	pVehicle->SetLightsState(bLights);

	// Set the taxilights
	pVehicle->SetTaxiLightsState(bTaxiLight);

	// Set the petrol tank health
	pVehicle->SetPetrolTankHealth(fPetrolTank);

	// Set the door angle
	for(int i = 0; i <= 5; i++)
		pVehicle->SetCarDoorAngle(i,false,fDoor[i]);

	// Set the window states(broken etc)
	for(int i = 0; i <= 3; i++)
		pVehicle->SetWindowState(i,bWindow[i]);

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

	// Get our vehicle manager
	CVehicleManager * pVehicleManager = g_pClient->GetVehicleManager();

	// If the vehicle exists remove it from the vehicle manager
	if(pVehicleManager->Exists(vehicleId))
		pVehicleManager->Delete(vehicleId);
}

void CClientRPCHandler::NewObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	DWORD			dwModelHash;
	CVector3		vecPos;
	CVector3		vecRot;
	int				iInterior;
	bool			bAttached;
	bool			bVehicleAttached;
	unsigned int	uiVehiclePlayerId;
	CVector3		vecAttachPosition;
	CVector3		vecAttachRotation;
	int				iBone = -1;

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

		// Read the interior
		pBitStream->Read(iInterior);

		// Read the attached state
		pBitStream->Read(bAttached);

		// Read the attached type state
		pBitStream->Read(bVehicleAttached);

		// Read the attached id state
		pBitStream->Read(uiVehiclePlayerId);

		// Read the attached pos
		pBitStream->Read(vecAttachPosition);

		// Read the attached rot
		pBitStream->Read(vecAttachRotation);

		// Read the bone id
		if(pBitStream->ReadBit())
			pBitStream->Read(iBone);

		// Create the object
		CObject * pObject = new CObject(dwModelHash, vecPos, vecRot);

		// Add the object to the object manager
		g_pClient->GetObjectManager()->Add(objectId, pObject);

		// Flag the object as can be streamed in
		pObject->SetCanBeStreamedIn(true);

		// If object is attached
		if(bAttached)
		{
			if(bVehicleAttached)
			{
				CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(uiVehiclePlayerId);

				if(pVehicle)
				{
					Scripting::AttachObjectToCar(pObject->GetHandle(),pVehicle->GetScriptingHandle(),0,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ);
					pObject->SetAttached(true);
				}
			}
			else if(!bVehicleAttached)
			{
				// Get the player
				CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(uiVehiclePlayerId);

				if(pPlayer && pPlayer->IsSpawned())
				{
					if(iBone != -1)
						Scripting::AttachObjectToPed(pObject->GetHandle(),pPlayer->GetScriptingHandle(),(Scripting::ePedBone)iBone,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ,0);
					else
						Scripting::AttachObjectToPed(pObject->GetHandle(),pPlayer->GetScriptingHandle(),(Scripting::ePedBone)0,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ,0);

					pObject->SetAttached(true);
				}
			}
		}
		// If object interior is set
		if(iInterior != -1) {
			Scripting::AddObjectToInteriorRoomByKey(pObject->GetHandle(),(Scripting::eInteriorRoomKey)iInterior);
			pObject->SetAttached(true);
		}
	}
}

void CClientRPCHandler::DetachObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId objectId;
	pBitStream->ReadCompressed(objectId);
	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

	if(pObject)
	{
		Scripting::DetachObject(pObject->GetHandle(), true);
		CVector3 vecPos; pObject->GetPosition(vecPos);
		pObject->SetPosition(vecPos);
	}
}

void CClientRPCHandler::AttachObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId		objectId;
	bool			bAttached;
	bool			bVehicleAttached;
	unsigned int	uiVehiclePlayerId;
	CVector3		vecAttachPosition;
	CVector3		vecAttachRotation;
	int				iBone = -1;

	while(pBitStream->ReadCompressed(objectId))
	{
		CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

		if(pObject)
		{
			// Read the attached state
			pBitStream->Read(bAttached);

			// Read the attached type state
			pBitStream->Read(bVehicleAttached);

			// Read the attached id state
			pBitStream->Read(uiVehiclePlayerId);

			// Read the attached pos
			pBitStream->Read(vecAttachPosition);

			// Read the attached rot
			pBitStream->Read(vecAttachRotation);

			// Read the bone id
			if(pBitStream->ReadBit())
				pBitStream->Read(iBone);

			// If object is attached
			if(bAttached)
			{
				if(bVehicleAttached)
				{
					CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(uiVehiclePlayerId);

					if(pVehicle)
					{
						Scripting::AttachObjectToCar(pObject->GetHandle(),pVehicle->GetScriptingHandle(),0,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ);
						pObject->SetAttached(true);
					}
				}
				else if(!bVehicleAttached)
				{
					CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(uiVehiclePlayerId);

					if(pPlayer)
					{
						if(iBone != -1)
							Scripting::AttachObjectToPed(pObject->GetHandle(),pPlayer->GetScriptingHandle(),(Scripting::ePedBone)iBone,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ,0);
						else
							Scripting::AttachObjectToPed(pObject->GetHandle(),pPlayer->GetScriptingHandle(),(Scripting::ePedBone)0,vecAttachPosition.fX,vecAttachPosition.fY,vecAttachPosition.fZ,vecAttachRotation.fX,vecAttachRotation.fY,vecAttachRotation.fZ,0);

						pObject->SetAttached(true);
					}
				}
			}
		}	
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

	CObjectManager * pObjectManager = g_pClient->GetObjectManager();
	CObject * pObject = pObjectManager->Get(objectId);

	// Does the object exist?
	if(pObject)
	{
		// Is the object attached?
		if(pObject->IsAttached()) 
			Scripting::DetachObject(pObject->GetHandle(), true);

		// Delete the object from the object manager
		pObjectManager->Delete(objectId);
	}
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
		g_pClient->GetPickupManager()->Add(pickupId, pPickup);

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
	g_pClient->GetPickupManager()->Delete(pickupId);
}


void CClientRPCHandler::NewBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int				iSpriteId;
	CVector3		vecPosition;
	unsigned int	uiColor;
	float			fSize;
	bool			bShortRange;
	bool			bRouteBlip;
	bool			bShow;
	String			strName;

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

		// Read if it's shown 
		pBitStream->Read(bShow);

		// Read the name
		pBitStream->Read(strName);

		// Get our blip manager
		CBlipManager * pBlipManager = g_pClient->GetBlipManager();

		// Create the blip in the blip manager
		pBlipManager->Create(blipId, iSpriteId, vecPosition);
		pBlipManager->SetSize(blipId, fSize);
		pBlipManager->ToggleShortRange(blipId, bShortRange);
		pBlipManager->ToggleRouteBlip(blipId, bRouteBlip);
		pBlipManager->Show(blipId, bShow);
		pBlipManager->SetName(blipId, strName);
		pBlipManager->SetColor(blipId, uiColor); 
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
	g_pClient->GetBlipManager()->Delete(blipId);
}

void CClientRPCHandler::NewActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId, vehicleId;
	CVector3 vecPosition, vecDrivePosition, vecDriveRotation, vecDriveFinalPosition;
	int iModelId, iSeat;
	float fHeading;
	String name;
	unsigned int color;
	bool togglename, frozen, helmet, bBlip, bDriveStop;

	while(pBitStream->Read(actorId))
	{
		pBitStream->Read(iModelId);
		pBitStream->Read(vecPosition);
		pBitStream->Read(fHeading);
		pBitStream->Read(name);
		pBitStream->Read(togglename);
		pBitStream->Read(color);
		pBitStream->Read(frozen);
		pBitStream->Read(helmet);
		pBitStream->Read(bBlip);
		pBitStream->Read(bDriveStop);
		pBitStream->Read(vecDrivePosition);
		pBitStream->Read(vecDriveRotation);
		pBitStream->Read(vecDriveFinalPosition);
		pBitStream->Read(vehicleId);
		pBitStream->Read(iSeat);

		g_pClient->GetActorManager()->Create(actorId, iModelId, vecPosition, fHeading, name, togglename, color, frozen, helmet, bBlip);
		//g_pActorManager->DriveToPoint(actorId, vehicleId, vecDrivePosition, vecDriveRotation, vecDriveFinalPosition, bDriveStop);
	}
}

void CClientRPCHandler::ScriptingSetActorName(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	String name;
	pBitStream->Read(name);

	g_pClient->GetActorManager()->SetName(actorId, name);
}

void CClientRPCHandler::ScriptingWarpActorIntoVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	int iSeatid;
	pBitStream->Read(iSeatid);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
	{
		if(g_pClient->GetVehicleManager()->Exists(vehicleId))
			pActorManager->WarpIntoVehicle(actorId, vehicleId, iSeatid);
	}
}

void CClientRPCHandler::ScriptingRemoveActorFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
		pActorManager->RemoveFromVehicle(actorId);
}

void CClientRPCHandler::ScriptingToggleActorNametag(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	bool bShow;
	pBitStream->Read(bShow);

	g_pClient->GetActorManager()->ToggleNametag(actorId,bShow);
}

void CClientRPCHandler::ScriptingToggleActorBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	bool bShow;
	pBitStream->Read(bShow);

	g_pClient->GetActorManager()->ToggleBlip(actorId,bShow);
}

void CClientRPCHandler::ScriptingSetActorColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	unsigned int color;
	pBitStream->Read(color);
	//CLogFile::Printf("Got color: 0x%X", color);

	g_pClient->GetActorManager()->SetColor(actorId,color);
}

void CClientRPCHandler::ScriptingToggleActorFrozen(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	bool frozen;
	pBitStream->Read(frozen);

	g_pClient->GetActorManager()->ToggleFrozen(actorId,frozen);
}

void CClientRPCHandler::ScriptingToggleActorHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	bool helmet;
	pBitStream->Read(helmet);

	g_pClient->GetActorManager()->ToggleHelmet(actorId,helmet);
}

void CClientRPCHandler::DeleteActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);
	g_pClient->GetActorManager()->Delete(actorId);
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

	while(pBitStream->Read(checkpointId))
	{
		pBitStream->Read(wType);
		pBitStream->Read(vecPosition);
		pBitStream->Read(vecTargetPosition);
		pBitStream->Read(fRadius);

		// Create the checkpoint
		CCheckpoint * pCheckpoint = new CCheckpoint(checkpointId, (eCheckpointType)wType, vecPosition, vecTargetPosition, fRadius);

		// Add the checkpoint to the checkpoint manager
		g_pClient->GetCheckpointManager()->Add(checkpointId, pCheckpoint);

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
	pBitStream->Read(checkpointId);
	g_pClient->GetCheckpointManager()->Delete(checkpointId);
}

void CClientRPCHandler::PlayerSpawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	pBitStream->ReadCompressed(playerId);

	// Get our player manager and local player
	CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();
	CNetworkPlayer * pPlayer = pPlayerManager->GetAt(playerId);

	// Is the player valid and active?
	if(pPlayer && pPlayerManager->IsActive(pPlayer->GetPlayerId()))
	{
		// Is it the local player?
		if(pPlayer->IsLocalPlayer())
		{
			// Get our local player
			CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

			if(g_pClient->GetFileTransfer()->IsComplete() && pLocalPlayer->IsConnectFinished())
				pLocalPlayer->Respawn();
		}
		else
		{
			/* Serverside
				bsSend.Write(m_iModelId);
				bsSend.Write(m_bHelmet);
				bsSend.Write(m_vecPosition);
				bsSend.Write(m_fHeading);
			Serverside end */

			int iModelId;
			bool bHelmet;
			CVector3 vecSpawnPos;
			float fHeading;

			EntityId vehicleId;

			pBitStream->Read(iModelId);
			pBitStream->Read(bHelmet);
			pBitStream->Read(vecSpawnPos);
			pBitStream->Read(fHeading);


			// Reset health to 200(IV Health + 100), otherwise player is "dead"
			pPlayer->SetHealth(200);

			// If the player is already spawned(connected) -> clear die task
			pPlayer->ClearDieTask();

			// Spawn player
			pPlayerManager->Spawn(playerId, iModelId, vecSpawnPos, fHeading);

			DWORD dwModelHash = SkinIdToModelHash(iModelId);
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

			// Is there a helmet?
			if(bHelmet)
				pPlayer->GiveHelmet();

			pBitStream->Read(vehicleId);
			CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

			// Is the player driver or passenger?
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
					pPlayerManager->GetAt(playerId)->SetClothes(uc, ucClothes);
				}
			}
		}
	}
}
void CClientRPCHandler::PlayerDeath(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	pBitStream->ReadCompressed(playerId);
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && !pPlayer->IsLocalPlayer())
		pPlayer->Kill();
}

void CClientRPCHandler::ScriptingSetPlayerDucking(CBitStream * pBitStream, CPlayerSocket * senderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bDucking;
	pBitStream->Read(bDucking);
	g_pClient->GetLocalPlayer()->SetDucking(bDucking);
}

void CClientRPCHandler::ScriptingSetPlayerInvincible(CBitStream * pBitStream, CPlayerSocket * senderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bInvincible;
	pBitStream->Read(bInvincible);
	Scripting::SetCharInvincible(g_pClient->GetLocalPlayer()->GetScriptingHandle(), bInvincible);
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

	if(g_pClient->GetPlayerManager()->DoesExist(playerId))
		g_pClient->GetChatWindow()->AddChatMessage(playerId, sChat);
}

void CClientRPCHandler::OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	unsigned short usPing;
	bool bHelmet;
	OnFootSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->ReadCompressed(bHelmet);

	pBitStream->Read((char *)&syncPacket, sizeof(OnFootSyncData));

	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
	{
		if(!pPlayer->IsLocalPlayer())
		{
			pPlayer->SetPing(usPing);

			CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);
			if(pRemotePlayer)
			{
				// Set the Helmet
				pPlayer->SetHelmet(bHelmet);
																
				// Store aimdata before updating foot stuff, otherwise the hitbox has moved
				// jenksta: what??
				if(bHasAimSyncData)
					pRemotePlayer->SetAimSyncData(&aimSyncPacket);

				pRemotePlayer->StoreOnFootSync(&syncPacket, bHasAimSyncData);
			}
		}
		else
		{
			// jenksta: local player shouldn't be receiving full sync data from itself...
			g_pClient->GetLocalPlayer()->SetPing(usPing);
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
	bool m_bHelmet;
	InVehicleSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(vehicleId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->ReadCompressed(m_bHelmet);
	pBitStream->Read((char *)&syncPacket, sizeof(InVehicleSyncData));

	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
	{
		pPlayer->SetPing(usPing);
		if(!pPlayer->IsLocalPlayer()) {
			CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

			if(pRemotePlayer)
			{
				// Set the Helmet
				bool bHelmet = m_bHelmet;
				pPlayer->SetHelmet(bHelmet);

				// Store aimdata before updating foot stuff, otherwise the hitbox has moved
				if(bHasAimSyncData)
					pRemotePlayer->SetAimSyncData(&aimSyncPacket);

				pRemotePlayer->StoreInVehicleSync(vehicleId, &syncPacket);
			}
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
	bool m_bHelmet;
	PassengerSyncData syncPacket;
	AimSyncData aimSyncPacket;
	pBitStream->ReadCompressed(playerId);
	pBitStream->ReadCompressed(vehicleId);
	pBitStream->ReadCompressed(usPing);
	pBitStream->ReadCompressed(m_bHelmet);
	pBitStream->Read((char *)&syncPacket, sizeof(PassengerSyncData));
	bool bHasAimSyncData = pBitStream->ReadBit();

	if(bHasAimSyncData)
		pBitStream->Read((char *)&aimSyncPacket, sizeof(AimSyncData));

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
	{
		
		if(!pPlayer->IsLocalPlayer()) {
			pPlayer->SetPing(usPing);
			CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

			if(pRemotePlayer)
			{
				// Set the Helmet
				bool helmet = m_bHelmet;
				pPlayer->SetHelmet(helmet);

				// Store aimdata before updating foot stuff, otherwise the hitbox has moved
				if(bHasAimSyncData)
					pRemotePlayer->SetAimSyncData(&aimSyncPacket);

				pRemotePlayer->StorePassengerSync(vehicleId, &syncPacket);
			}
		}
		else
			g_pClient->GetLocalPlayer()->SetPing(usPing);
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

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
	{
		// Todo: Local player stuff...
		if(!pPlayer->IsLocalPlayer()) {
			CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer*>(pPlayer);
			if(pRemotePlayer)
			{
				// Store aimdata before updating foot stuff, otherwise the hitbox has moved
				if(bHasAimSyncData)
					pRemotePlayer->SetAimSyncData(&aimSyncPacket);

				pRemotePlayer->StoreSmallSync(&syncPacket);
			}
		}
	}
}

void CClientRPCHandler::EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
//	// Ensure we have a valid bit stream
//	if(!pBitStream)
//		return;
//
//	EMPTYVEHICLESYNCPACKET syncPacket;
//	pBitStream->Read((PCHAR)&syncPacket, sizeof(EMPTYVEHICLESYNCPACKET));
//	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(syncPacket.vehicleId);
//
//	if(pVehicle)
//		pVehicle->StoreEmptySync(&syncPacket);
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
	g_pClient->GetChatWindow()->AddMessage(dwColor, bAllowFormatting, sMessage);
}

void CClientRPCHandler::ConnectionRefused(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the refuse reason
	int iReason;
	pBitStream->Read(iReason);

	// Determine the refuse reason
	String strReason = "Connection refused";
 
 	if(iReason == REFUSE_REASON_INVALID_VERSION)
		strReason = "You are using an invalid version.";
 	else if(iReason == REFUSE_REASON_NAME_INVALID)
		strReason = "You are using an invalid name.";
 	else if(iReason == REFUSE_REASON_NAME_IN_USE)
		strReason = "Your name is already in use.";
 	else if(iReason == REFUSE_REASON_ABORTED_BY_SCRIPT)
		strReason = "Connection aborted by script.";
 
 	// Disconnect from the server & show the message
 	g_pClient->GetNetworkManager()->Disconnect();
	g_pClient->GetMainMenu()->ShowMessageBox(strReason.C_String(),"Connection failed", true, true, false);
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
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	// Is the player pointer invalid?
	if(!pPlayer)
	{
		CLogFile::Printf("Warning: Invalid player for VehicleEnterExit RPC\n");
		return;
	}

	// Set no longer requesting a vehicle entry/exit
	pPlayer->SetRequestingVehicleEnterExit(false);

	// Is the reply ok?
	if(bReply)
	{
		// Get the vehicle pointer
		CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

		// Is the vehicle pointer invalid?
		if(!pVehicle)
		{
			CLogFile::Printf("Warning: Invalid vehicle for VehicleEnterExit RPC\n");
			return;
		}

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
			pPlayer->ResetVehicleEnterExit();
		}
		// Is it an exit reply?
		else if(byteEnterExitVehicleType == VEHICLE_EXIT_RETURN)
		{
			CLogFile::Printf("VehicleExit(%d, %d)", playerId, vehicleId);

			// Exit the vehicle
			pPlayer->ExitVehicle(EXIT_VEHICLE_NORMAL);
		}
	}
}

void CClientRPCHandler::HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	CVector3 vecAim;

	pBitStream->ReadCompressed(playerId);
	pBitStream->Read(vecAim.fX);
	pBitStream->Read(vecAim.fY);
	pBitStream->Read(vecAim.fZ);

	// Get the player
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
		pPlayer->TaskLookAtCoord(vecAim.fX, vecAim.fY, vecAim.fZ);
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

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->SetName(strName);

		if(pPlayer->IsLocalPlayer())
		{
			g_pClient->SetNick(strName);
			g_pClient->GetMainMenu()->m_pSettingsWindowNickEditBox->setText(strName.Get());
			CVAR_SET_STRING("nick", strName);
		}
	}
}

void CClientRPCHandler::NewFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	
	bool bIsScript;
	String strFileName;
	CFileChecksum fileChecksum;

	// Read the file type
	while(pBitStream->Read(bIsScript))
	{
		// Read the file name
		pBitStream->Read(strFileName);

		// Read the file checksum
		pBitStream->Read((char *)&fileChecksum, sizeof(CFileChecksum));

		// Add the file to the file transfer manager
		g_pClient->GetFileTransfer()->Add(!bIsScript, strFileName, fileChecksum);
	}
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

	//CLogFile::Printf(__FILE__,__LINE__,"Delete File: %s", strFileName.C_String());
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();

	if(bIsScript && pClientScriptManager->Exists(strFileName))
	{
		pClientScriptManager->Unload(strFileName);
		pClientScriptManager->RemoveScript(strFileName);
	}
}

void CClientRPCHandler::ScriptingSetPlayerHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned int uHealth;
	pBitStream->Read(uHealth);
	g_pClient->GetLocalPlayer()->SetHealth(uHealth + 100);
}

void CClientRPCHandler::ScriptingSetPlayerCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecPosition;
	pBitStream->Read(vecPosition);
	g_pClient->GetLocalPlayer()->Teleport(vecPosition);
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

	// Get our time
	CTime * pTime = g_pClient->GetTime();
	pTime->SetTime(ucHour, ucMinute);

	if(pBitStream->Read(uiMinuteDuration))
		pTime->SetMinuteDuration(uiMinuteDuration);
	else
		pTime->SetMinuteDuration(CTime::DEFAULT_MINUTE_DURATION);
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

	bool bToggle;
	pBitStream->Read(bToggle);
	Scripting::SetNoResprays(!bToggle);
}

void CClientRPCHandler::ScriptingToggleAutoAim(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bToggle;
	pBitStream->Read(bToggle);
	Scripting::DisablePlayerLockon(0, !bToggle);
}

void CClientRPCHandler::ScriptingGivePlayerWeapon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int weapon, ammo;
	pBitStream->Read(weapon);
	pBitStream->Read(ammo);
	g_pClient->GetLocalPlayer()->GiveWeapon(weapon, ammo);
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
	g_pClient->GetLocalPlayer()->SetSpawnLocation(vecPos, r);
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

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);
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

	g_pClient->GetLocalPlayer()->SetControl(bToggle);

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
	g_pClient->GetLocalPlayer()->SetCurrentHeading(heading);
}

void CClientRPCHandler::ScriptingSetPlayerGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	float grav;
	pBitStream->Read(grav);
	Scripting::SetCharGravity(g_pClient->GetLocalPlayer()->GetScriptingHandle(), grav);
}

/*void CClientRPCHandler::SetPlayerDrunk(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bToggle;
	bToggle = pBitStream->ReadBit();

	Scripting::SetPedIsDrunk(g_pLocalPlayer->GetScriptingHandle(), !bToggle);
}

void CClientRPCHandler::SetVehicleGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
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
		//TODO
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
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

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetSirenState(bState);
}

void CClientRPCHandler::ScriptingSetVehicleEngineState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	bool bState;
	pBitStream->Read(vehicleId);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetEngineState(bState);
}

void CClientRPCHandler::ScriptingTurnTaxiLights(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	bool bState;
	pBitStream->Read(vehicleId);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetTaxiLightsState(bState);
}

void CClientRPCHandler::ScriptingControlCar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	int iDoor;
	bool bState;
	float fAngle;
	pBitStream->Read(vehicleId);
	pBitStream->Read(iDoor);
	pBitStream->Read(bState);
	pBitStream->Read(fAngle);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetCarDoorAngle(iDoor,bState,fAngle);

}

void CClientRPCHandler::ScriptingSetVehicleLights(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	bool bLights;
	pBitStream->Read(vehicleId);
	pBitStream->Read(bLights);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetLightsState(bLights);
}

void CClientRPCHandler::ScriptingRepairVehicleTyres(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)0);
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)1);
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)2);
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)3);
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)4);
		Scripting::FixCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)5);
	}
}

void CClientRPCHandler::ScriptingRepairVehicleWindows(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		//TODO
	}
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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
	g_pClient->GetLocalPlayer()->ToggleRagdoll(bToggle);
}

void CClientRPCHandler::ScriptingGivePlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int amount;
	pBitStream->Read(amount);
	g_pClient->GetLocalPlayer()->GiveMoney(amount);
}

void CClientRPCHandler::ScriptingSetPlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int amount;
	pBitStream->Read(amount);
	g_pClient->GetLocalPlayer()->SetMoney(amount);
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
	g_pClient->GetLocalPlayer()->SetPlayerControlAdvanced(!bFrozen, !bCameraFrozen);
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

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
	g_pClient->GetLocalPlayer()->SetArmour(uArmour);
}

void CClientRPCHandler::ScriptingSetPlayerMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecMoveSpeed;
	pBitStream->Read(vecMoveSpeed);
	g_pClient->GetLocalPlayer()->SetMoveSpeed(vecMoveSpeed);
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetMoveSpeed(vecMoveSpeed);
}

void CClientRPCHandler::ScriptingSetVehicleTurnSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	CVector3 vecTurnSpeed;
	pBitStream->Read(vehicleId);
	pBitStream->Read(vecTurnSpeed);
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetTurnSpeed(vecTurnSpeed);
}

void CClientRPCHandler::ScriptingRemoveWeapons(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetLocalPlayer()->RemoveAllWeapons();
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		g_pClient->GetLocalPlayer()->PutInVehicle(pVehicle, byteSeatId);
}

void CClientRPCHandler::ScriptingRemovePlayerFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bGraceful = pBitStream->ReadBit();

	if(bGraceful)
		g_pClient->GetLocalPlayer()->ExitVehicle(EXIT_VEHICLE_NORMAL);
	else
		g_pClient->GetLocalPlayer()->RemoveFromVehicle();
}

void CClientRPCHandler::ScriptingSetCameraBehindPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetLocalPlayer()->SetCameraBehind();
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
	g_pClient->GetActorManager()->SetPosition(actorId, vecPosition);
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
	g_pClient->GetActorManager()->SetHeading(actorId, fHeading);
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

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
	{
		// Apply the walk task to the actor
		unsigned int ped = pActorManager->GetScriptingHandle(actorId);
		Scripting::TaskGoStraightToCoord(ped, x, y, z, 2, 45000);
	}
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
	g_pClient->GetBlipManager()->SetColor(blipId, uiColor);
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
	g_pClient->GetBlipManager()->SetSize(blipId, fSize);
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
	g_pClient->GetBlipManager()->ToggleShortRange(blipId, bShortRange);
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
	g_pClient->GetBlipManager()->ToggleRouteBlip(blipId, bRouteBlip);
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
	g_pClient->GetBlipManager()->SetName(blipId, strName);
}

void CClientRPCHandler::ScriptingSetBlipIcon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId blipId;
	pBitStream->Read(blipId);
	bool bShow;
	pBitStream->Read(bShow);
	g_pClient->GetBlipManager()->Show(blipId, bShow);

}

void CClientRPCHandler::ScriptingShowCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the checkpoint id
	EntityId checkpointId;
	CVector3 vecPosition;
	CVector3 vecTargetPosition;

	pBitStream->Read(checkpointId);
	pBitStream->Read(vecPosition);
	pBitStream->Read(vecTargetPosition);

	// Set the checkpoint to visible
	CCheckpoint * pCheckpoint = g_pClient->GetCheckpointManager()->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->Show();
		pCheckpoint->SetPosition(vecPosition);
		pCheckpoint->SetTargetPosition(vecTargetPosition);
	}
}

void CClientRPCHandler::ScriptingHideCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the checkpoint id
	EntityId checkpointId;
	pBitStream->Read(checkpointId);

	// Set the checkpoint to not visible
	CCheckpoint * pCheckpoint = g_pClient->GetCheckpointManager()->Get(checkpointId);

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
	Scripting::DisplayHUD(bToggle);
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
	Scripting::DisplayRadar(bToggle);
	g_pClient->GetLocalPlayer()->SetRadarVisible(bToggle);
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
	g_pClient->GetNameTags()->SetEnabled(bToggle);
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
	Scripting::DisplayAreaName(bToggle);
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

		g_pClient->GetEvents()->Call(szEventName, pArgs);
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

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
		pPlayer->SetColor(uiColor);
}

void CClientRPCHandler::ScriptingSetVehicleLocked(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	int iLocked;

	pBitStream->Read(vehicleId);
	pBitStream->Read(iLocked);

	// Convert int to dword
	DWORD dwLockState = static_cast<DWORD>(iLocked);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
	if(pVehicle)
		pVehicle->SetDoorLockState(dwLockState);
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
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

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
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		for(unsigned char uc = 0; uc < 11; ++ uc)
			pPlayer->SetClothes(uc, 0);
	}
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
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

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
	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

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

	CTrafficLights * pTrafficLights = g_pClient->GetTrafficLights();

	if(eState != CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
	{
		// set locked
		pTrafficLights->SetLocked(pBitStream->ReadBit());

		unsigned int uiYellowDuration;
		if(eState >= CTrafficLights::TRAFFIC_LIGHT_STATE_FLASHING_FLASHING && eState < CTrafficLights::TRAFFIC_LIGHT_STATE_DISABLED_DISABLED)
		{
			// yellow flashing duration
			if(pBitStream->Read(uiYellowDuration))
				pTrafficLights->SetYellowDuration(uiYellowDuration);
			else
				pTrafficLights->SetYellowDuration(CTrafficLights::DEFAULT_YELLOW_DURATION);
		}
		else if(eState < CTrafficLights::TRAFFIC_LIGHT_STATE_TO_GREEN_RED)
		{
			// traffic cycle durations
			unsigned int uiGreenDuration, uiRedDuration;
			if(pBitStream->Read(uiGreenDuration))
			{
				pBitStream->Read(uiYellowDuration);
				pBitStream->Read(uiRedDuration);
				pTrafficLights->SetGreenDuration(uiGreenDuration);
				pTrafficLights->SetYellowDuration(uiYellowDuration);
				pTrafficLights->SetRedDuration(uiRedDuration);
			}
			else
			{
				pTrafficLights->SetGreenDuration(CTrafficLights::DEFAULT_GREEN_DURATION);
				pTrafficLights->SetYellowDuration(CTrafficLights::DEFAULT_YELLOW_DURATION);
				pTrafficLights->SetRedDuration(CTrafficLights::DEFAULT_RED_DURATION);
			}
		}
	}

	pTrafficLights->SetState(eState);
}

void CClientRPCHandler::ScriptingSetVehicleComponents(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
	if(pVehicle)
	{
		// Read the component values
		for(unsigned char i = 0; i < 9; ++ i)
		{
			bool bState = pBitStream->ReadBit();
			pVehicle->SetComponentState(i, bState);
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

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
	if(pVehicle)
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
	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

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
	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

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

	CPickup * pPickup = g_pClient->GetPickupManager()->Get(objectId);

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

	CPickup * pPickup = g_pClient->GetPickupManager()->Get(objectId);

	if(pPickup)
	{
		// Read the rotation
		CVector3 vecRotation;
		pBitStream->Read(vecRotation);

		// Set the rotation
		pPickup->SetRotation(vecRotation);
	}
}

void CClientRPCHandler::ScriptingSetPickupValue(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the pickup id
	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	CPickup * pPickup = g_pClient->GetPickupManager()->Get(objectId);

	if(pPickup)
	{
		// Read the value
		unsigned int pValue;
		pBitStream->ReadCompressed(pValue);

		// Set the value
		pPickup->SetValue(pValue);
	}
}

void CClientRPCHandler::ScriptingSetPlayerCameraPos(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the camera position
	CVector3 vecPosition;

	if(!pBitStream->Read(vecPosition))
		return;

	// Set the camera position
	g_pClient->GetCamera()->SetPosition(vecPosition);
}

void CClientRPCHandler::ScriptingSetPlayerCameraLookAt(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	// Read the camera look at
	CVector3 vecLookAt;

	if(!pBitStream->Read(vecLookAt))
		return;

	// Set the camera look at
	g_pClient->GetCamera()->SetLookAt(vecLookAt);
}

void CClientRPCHandler::ScriptingResetPlayerCamera(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Reset the camera
	g_pClient->GetCamera()->Reset();
}

void CClientRPCHandler::ScriptingCreateFire(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId fireId;
	pBitStream->Read(fireId);

	CVector3 vecPos;
	pBitStream->Read(vecPos);

	float fdensity;
	pBitStream->Read(fdensity);

	g_pClient->GetFireManager()->Create(fireId,vecPos,fdensity);
}

void CClientRPCHandler::ScriptingDeleteFire(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId fireId;
	pBitStream->Read(fireId);

	g_pClient->GetFireManager()->Delete(fireId);
}

void CClientRPCHandler::ScriptingCreateExplosion(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	CVector3 vecPos;
	pBitStream->Read(vecPos);

	float fdensity;
	pBitStream->Read(fdensity);
	CGame::CreateExplosion(vecPos, 1, fdensity);
}

void CClientRPCHandler::ScriptingForcePlayerAnimation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	String strGroup;
	String strAnim;
	pBitStream->Read(playerId);
	pBitStream->Read(strGroup);
	pBitStream->Read(strAnim);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
		Scripting::TaskPlayAnim(pPlayer->GetScriptingHandle(), strAnim.Get(), strGroup.Get(), (float)8, 0, 0, 0, 0, -1);
}

void CClientRPCHandler::ScriptingForceActorAnimation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	EntityId actorId;
	String strGroup;
	String strAnim;

	pBitStream->Read(actorId);
	pBitStream->Read(strGroup);
	pBitStream->Read(strAnim);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
		pActorManager->ForceAnimation(actorId, strGroup.Get(), strAnim.Get());
}

void CClientRPCHandler::ScriptingBlockWeaponScroll(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	bool bToggle;
	pBitStream->Read(bToggle);

	Scripting::BlockPedWeaponSwitching(g_pClient->GetLocalPlayer()->GetScriptingHandle(),bToggle);
}

void CClientRPCHandler::ScriptingBlockWeaponDrop(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	bool bToggle;
	pBitStream->Read(bToggle);

	Scripting::SetCharDropsWeaponsWhenDead(g_pClient->GetLocalPlayer()->GetScriptingHandle(),bToggle);
}

void CClientRPCHandler::ScriptingFadeScreenIn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	int iDuration;
	pBitStream->Read(iDuration);

	Scripting::ScreenFadeIn( iDuration );
}

void CClientRPCHandler::ScriptingFadeScreenOut(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	int iDuration;
	pBitStream->Read(iDuration);

	Scripting::ScreenFadeOut( iDuration );
}

void CClientRPCHandler::ScriptingPlayPoliceReport(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	String strReport;
	pBitStream->Read(strReport);

	Scripting::TriggerPoliceReport(strReport.C_String());
}

void CClientRPCHandler::ScriptingPlayMissionCompleteAudio(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	int iMission;
	pBitStream->Read(iMission);

	Scripting::TriggerMissionComplete(iMission);
}

void CClientRPCHandler::ScriptingPlayGameAudio(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;
	
	String strMusic;
	pBitStream->Read(strMusic);

	Scripting::PlayAudioEvent(strMusic.Get());
}

void CClientRPCHandler::ScriptingRequestAnims(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	String strGroup;
	pBitStream->Read(strGroup);

	int iAnimIndex = CGame::GetStreaming()->GetAnimIndexFromName(strGroup);
	CGame::GetStreaming()->RequestResource(RESOURCE_TYPE_WAD, iAnimIndex);
}

void CClientRPCHandler::ScriptingReleaseAnims(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	String strGroup;
	pBitStream->Read(strGroup);
	int iAnimIndex = CGame::GetStreaming()->GetAnimIndexFromName(strGroup);
	CGame::GetStreaming()->ReleaseResource(RESOURCE_TYPE_WAD, iAnimIndex);
}

void CClientRPCHandler::ScriptingForceWind(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	float fWind;
	pBitStream->Read(fWind);

	Scripting::ForceWind(fWind);
}

void CClientRPCHandler::ScriptingSetNametags(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	bool bToggle;
	pBitStream->Read(bToggle);
	CGame::SetNameTags(bToggle);
}

void CClientRPCHandler::ScriptingAttachCam(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId attachId;
	bool bVehicleOrPlayer;
	unsigned int uiHandle = -1;
	int iPointType;
	CVector3 vecOffset;

	pBitStream->ReadCompressed(attachId);
	pBitStream->Read(vecOffset);
	pBitStream->Read(iPointType);
	bVehicleOrPlayer = pBitStream->ReadBit();

	if(bVehicleOrPlayer)
	{
		CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(attachId);

		if(pVehicle)
			uiHandle = pVehicle->GetScriptingHandle();
	}
	else
	{
		CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(attachId);

		if(pPlayer)
			uiHandle = pPlayer->GetScriptingHandle();
	}

	g_pClient->GetCamera()->Attach(uiHandle, bVehicleOrPlayer, iPointType, vecOffset);
}

void CClientRPCHandler::ScriptingDisplayHudNotification(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int iMode;
	pBitStream->Read(iMode);

	String iMessage;
	pBitStream->Read(iMessage);

	Scripting::DisplayHudNotification(iMode,iMessage.C_String(),iMode);
}

void CClientRPCHandler::ScriptingSetVehicleFollowMode(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	int iMode;
	pBitStream->Read(iMode);

	if(g_pClient->GetLocalPlayer()->IsInVehicle())
		Scripting::SetFollowVehicleCamSubmode(iMode);
}

void CClientRPCHandler::ScriptingSetVehicleFollowOffset(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CVector3 vecPos;
	pBitStream->Read(vecPos);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		Scripting::SetFollowVehicleCamOffset(pVehicle->GetScriptingHandle(), vecPos.fX, vecPos.fY, vecPos.fZ);
}

void CClientRPCHandler::ScriptingSetAmmoInClip(CBitStream * pBitStream, CPlayerSocket * senderSocket)	
{	
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned int uiAmmoInClip;
	pBitStream->Read(uiAmmoInClip);

	g_pClient->GetLocalPlayer()->SetAmmoInClip(uiAmmoInClip);
}

void CClientRPCHandler::ScriptingSetAmmo(CBitStream * pBitStream, CPlayerSocket * senderSocket)	
{	
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	unsigned int uiWeapon, uiAmmo;
	pBitStream->Read(uiWeapon);
	pBitStream->Read(uiAmmo);

	g_pClient->GetLocalPlayer()->SetAmmo(uiWeapon, uiAmmo);
}

void CClientRPCHandler::ScriptingCreatePlayerBlip(CBitStream * pBitStream, CPlayerSocket * senderSocket)	
{	
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	int iSprite;
	bool bShortRange;
	bool bShow;

	pBitStream->ReadCompressed(playerId);
	pBitStream->Read(iSprite);
	pBitStream->Read(bShortRange);
	pBitStream->Read(bShow);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		if(!pPlayer->GetBlipActivity() && !pPlayer->IsLocalPlayer())
		{
			unsigned int uiHandle;
			Scripting::AddBlipForChar(pPlayer->GetScriptingHandle(), &uiHandle);
			Scripting::ChangeBlipSprite(uiHandle, (Scripting::eBlipSprite)iSprite);
			Scripting::ChangeBlipScale(uiHandle, 0.5);
			Scripting::ChangeBlipNameFromAscii(uiHandle,  pPlayer->GetName().C_String());
			pPlayer->SetBlip(uiHandle);
			pPlayer->SetBlipActive(true);
		}
	}
}

void CClientRPCHandler::ScriptingRemovePlayerBlip(CBitStream * pBitStream, CPlayerSocket * senderSocket)	
{	
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;

	pBitStream->ReadCompressed(playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		if(pPlayer->GetBlipActivity())
		{
			Scripting::RemoveBlip(pPlayer->GetBlip());
			pPlayer->SetBlipActive(false);
			pPlayer->SetBlip(NULL);
		}
	}
}

void CClientRPCHandler::ScriptingChangePlayerBlip(CBitStream * pBitStream, CPlayerSocket * senderSocket)	
{	
		// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	int iSprite;
	bool bShortRange;
	bool bShow;

	pBitStream->ReadCompressed(playerId);
	pBitStream->Read(iSprite);
	pBitStream->Read(bShortRange);
	pBitStream->Read(bShow);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		if(!pPlayer->GetBlipActivity())
		{
			unsigned int uiHandle = pPlayer->GetBlip();

			// Apply short range
			Scripting::SetBlipAsShortRange(uiHandle, bShow);

			// Apply display
			if(bShow)
				Scripting::ChangeBlipDisplay(uiHandle,Scripting::BLIP_MODE_SHOW);
			else if(!bShow)
				Scripting::ChangeBlipDisplay(uiHandle,Scripting::BLIP_MODE_HIDE);

			// Apply sprite
			Scripting::ChangeBlipSprite(uiHandle, (Scripting::eBlipSprite)iSprite);

			pPlayer->SetBlipActive(true);
		}
	}
}

void CClientRPCHandler::ScriptingSetVehicleGPSState(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	bool bState = false;
	pBitStream->Read(vehicleId);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
	if(pVehicle)
		pVehicle->SetVehicleGPSState(bState);
}

void CClientRPCHandler::ScriptingSetVehicleAlarm(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	int	iDuration;

	pBitStream->Read(vehicleId);
	pBitStream->Read(iDuration);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);
	if(pVehicle)
		pVehicle->GetGameVehicle()->SetAlarmDuration(iDuration);
}

void CClientRPCHandler::ScriptingSetVehiclePetrolTankHealth(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	float fPetrol;

	pBitStream->Read(vehicleId);
	pBitStream->Read(fPetrol);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetPetrolTankHealth(fPetrol);
}

void CClientRPCHandler::ScriptingSetVehicleTyreState(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	int	iTyre;
	bool bState;

	pBitStream->Read(vehicleId);
	pBitStream->Read(iTyre);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		//TODO
	}
}

void CClientRPCHandler::ScriptingSetVehicleWindowState(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	int	iWindow;
	bool bState;

	pBitStream->Read(vehicleId);
	pBitStream->Read(iWindow);
	pBitStream->Read(bState);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		//TODO
	}
}

void CClientRPCHandler::ScriptingSetPlayerUseMobilePhone(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId playerId;
	bool bUse;

	pBitStream->Read(playerId);
	pBitStream->Read(bUse);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	// jenksta: why is this only enabled for the local player?
	if(pPlayer && pPlayer->IsLocalPlayer())
		Scripting::TaskUseMobilePhone(pPlayer->GetScriptingHandle(), bUse);
}

void CClientRPCHandler::ScriptingStopActorDriving(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	g_pClient->GetActorManager()->DriveToPoint(actorId, -1, CVector3(), CVector3(), CVector3(), false);
}

void CClientRPCHandler::ScriptingActorDriveToCoords(CBitStream *pBitStream, CPlayerSocket *pPlayerSocket)
{
	if(!pBitStream)
		return;

	EntityId actorId;
	pBitStream->Read(actorId);

	CVector3 vecPos;
	pBitStream->Read(vecPos);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
		pActorManager->DriveToPoint(actorId, pActorManager->GetVehicleId(actorId), pActorManager->GetPosition(actorId), CVector3(), vecPos, true);
}
void CClientRPCHandler::ScriptingMarkVehicleAsActorVehicle(CBitStream * pBitStream, CPlayerSocket *pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	pBitStream->Read(vehicleId);
	
	bool bToggle;
	pBitStream->Read(bToggle);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->MarkAsActorVehicle(bToggle);
}

void CClientRPCHandler::ScriptingForcePlayerSpeech(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId;
	String strVoice;
	String strText;
	pBitStream->Read(playerId);
	pBitStream->Read(strVoice);
	pBitStream->Read(strText);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
		Scripting::SayAmbientSpeechWithVoice(pPlayer->GetScriptingHandle(), strText.Get(), strVoice.Get(), 1, 1, 0);
}

void CClientRPCHandler::ScriptingForceActorSpeech(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId actorId;
	String strVoice;
	String strText;
	pBitStream->Read(actorId);
	pBitStream->Read(strVoice);
	pBitStream->Read(strText);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
		Scripting::SayAmbientSpeechWithVoice(pActorManager->GetScriptingHandle(actorId), strText.Get(), strVoice.Get(), 1, 1, 0);
}

void CClientRPCHandler::ScriptingLetPlayerDriveAutomatic(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Ensure we have a valid bit stream
	if(!pBitStream)
		return;

	EntityId playerId, vehicleId;
	CVector3 vecPos;
	float fSpeed;
	int iDrivingStyle;

	pBitStream->Read(playerId);
	pBitStream->Read(vehicleId);
	pBitStream->Read(vecPos);
	pBitStream->Read(fSpeed);
	pBitStream->Read(iDrivingStyle);

	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(playerId == pLocalPlayer->GetPlayerId() && pLocalPlayer->GetVehicle()->GetVehicleId() == vehicleId)
		Scripting::TaskCarDriveToCoord(pLocalPlayer->GetScriptingHandle(), pLocalPlayer->GetVehicle()->GetScriptingHandle(), vecPos.fX, vecPos.fY, vecPos.fZ, fSpeed, iDrivingStyle, 1, iDrivingStyle, 5.0f, -1);
}

void CClientRPCHandler::ScriptingSetPlayerDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId playerId;
	SQInteger Dimension;
	
	pBitStream->Read(playerId);
	pBitStream->Read(Dimension);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
		pPlayer->SetDimension(Dimension);

	g_pClient->GetStreamer()->Pulse();
}

void CClientRPCHandler::ScriptingSetVehicleDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId vehicleId;
	SQInteger Dimension;	
	pBitStream->Read(vehicleId);
	pBitStream->Read(Dimension);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		pVehicle->SetDimension(Dimension);

	//g_pStreamer->Pulse();
}


void CClientRPCHandler::ResetVehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId playerId;
	pBitStream->Read(playerId);
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(playerId == pLocalPlayer->GetPlayerId())
		pLocalPlayer->ResetVehicleEnterExit();
}

void CClientRPCHandler::ScriptingTogglePlayerLabelForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId playerId;
	bool bToggle;

	pBitStream->Read(playerId);
	pBitStream->Read(bToggle);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsSpawned())
	{
		// jenksta: TODO: CNetworkPlayer::ToggleNametag, as name tags are now drawn by us and not the game
		/*if(!bToggle)
			Scripting::RemoveFakeNetworkNameFromPed(pPlayer->GetScriptingHandle());
		else
		{
			char red = (pPlayer->GetColor() & 0xFF000000) >> 24;
			char green = (pPlayer->GetColor() & 0x00FF0000) >> 16;
			char blue = (pPlayer->GetColor() & 0x0000FF00) >> 8;
			char alpha = (pPlayer->GetColor() & 0x000000FF);
					
			Scripting::GivePedFakeNetworkName(pPlayer->GetColor(), pPlayer->GetName().C_String(),red,green,blue,alpha);
		}*/
	}
}

void CClientRPCHandler::ScriptingFixVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
			return;
	
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		Scripting::FixCar(pVehicle->GetScriptingHandle());
}


void CClientRPCHandler::ScriptingMoveObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId objectId;
	CVector3 vecMoveTarget;
	int iTime;
	bool bHasRotation = false;
	CVector3 vecMoveRotation;

	pBitStream->ReadCompressed(objectId);
	pBitStream->Read(vecMoveTarget);
	pBitStream->Read(iTime);

	if(bHasRotation = pBitStream->ReadBit())
		pBitStream->Read(vecMoveRotation);

	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

	if(pObject)
	{
		pObject->Move(vecMoveTarget, (unsigned int)iTime);

		if(bHasRotation)
			pObject->Rotate(vecMoveRotation, (unsigned int)iTime);
	}
}

void CClientRPCHandler::ScriptingRotateObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId objectId;
	CVector3 vecMoveRotation;
	int iTime;

	pBitStream->ReadCompressed(objectId);	
	pBitStream->Read(vecMoveRotation);
	pBitStream->Read(iTime);

	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

	if(pObject)
		pObject->Rotate(vecMoveRotation, (unsigned int)iTime);
}

void CClientRPCHandler::ScriptingSetObjectDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	unsigned char ucDimension;
	pBitStream->Read(ucDimension);

	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

	if(pObject)
		pObject->SetDimension(ucDimension);
}


void CClientRPCHandler::ScriptingSetCheckpointDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId checkpointId;
	pBitStream->ReadCompressed(checkpointId);

	unsigned char ucDimension;
	pBitStream->Read(ucDimension);

	CCheckpoint * pCheckpoint = g_pClient->GetCheckpointManager()->Get(checkpointId);

	if(pCheckpoint)
		pCheckpoint->SetDimension(ucDimension);
}

void CClientRPCHandler::ScriptingSetObjectInterior(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	EntityId objectId;
	pBitStream->ReadCompressed(objectId);

	int iInterior;
	pBitStream->Read(iInterior);

	CObject * pObject = g_pClient->GetObjectManager()->Get(objectId);

	if(pObject)
		Scripting::AddObjectToInteriorRoomByKey(pObject->GetHandle(), (Scripting::eInteriorRoomKey)iInterior);
}

void CClientRPCHandler::ScriptingExplodeCar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;
	
	EntityId vehicleId;
	pBitStream->Read(vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
		Scripting::ExplodeCar(pVehicle->GetScriptingHandle(), true, false);
}

void CClientRPCHandler::New3DLabel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	/*  Server structure
		bsSend.WriteCompressed(i);
		bsSend.Write(m_Labels[i]->GetText());
		bsSend.Write(m_Labels[i]->GetPosition());
		bsSend.Write(m_Labels[i]->GetColor());
		bsSend.Write(m_Labels[i]->GetStreamingDistance());
		bsSend.Write(m_Labels[i]->GetDimension());
		bsSend.WriteBit(m_Labels[i]->IsVisible()); 
	*/
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	String text;
	pBitStream->Read(text);

	CVector3 vecPosition;
	pBitStream->Read(vecPosition);

	DWORD dwColor;
	pBitStream->Read(dwColor);

	float fStreamingDistance;
	pBitStream->Read(fStreamingDistance);

	DimensionId dimensionId;
	pBitStream->Read(dimensionId);

	bool bVisible = pBitStream->ReadBit();

	g_pClient->GetChatWindow()->AddInfoMessage("New Label(%s, %i, %f)", text.Get(), bVisible, fStreamingDistance);
	LabelId id = g_pClient->Get3DLabelManager()->Add(text.Get(), vecPosition, dwColor, bVisible, fStreamingDistance);
	g_pClient->Get3DLabelManager()->GetAt(id)->SetDimension(dimensionId);
}

void CClientRPCHandler::Delete3DLabel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);
	g_pClient->Get3DLabelManager()->Remove(labelId);
}

void CClientRPCHandler::ScriptingSet3DLabelPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	CVector3 vecPosition;
	pBitStream->Read(vecPosition);

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);

	if(pLabel)
		pLabel->SetPosition(vecPosition);
}

void CClientRPCHandler::ScriptingSet3DLabelText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	String text;
	pBitStream->Read(text);

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);

	if(pLabel)
		pLabel->SetText(text);
}

void CClientRPCHandler::ScriptingSet3DLabelColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	DWORD dwColor;
	pBitStream->Read(dwColor);

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);
	
	if(pLabel)
		pLabel->SetColor(dwColor);
}

void CClientRPCHandler::ScriptingSet3DLabelVisible(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	bool bVisible = pBitStream->ReadBit();

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);

	if(pLabel)
		pLabel->SetVisible(bVisible);
}

void CClientRPCHandler::ScriptingSet3DLabelStreamingDistance(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	float fStreamingDistance;
	pBitStream->Read(fStreamingDistance);

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);

	if(pLabel)
		pLabel->SetStreamingDistance(fStreamingDistance);
}

void CClientRPCHandler::ScriptingSet3DLabelDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(!pBitStream)
		return;

	LabelId labelId;
	pBitStream->ReadCompressed(labelId);

	DimensionId dimensionId;
	pBitStream->Read(dimensionId);

	C3DLabel * pLabel = g_pClient->Get3DLabelManager()->GetAt(labelId);

	if(pLabel)
		pLabel->SetDimension(dimensionId);
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
	AddFunction(RPC_NameChange, NameChange);
	AddFunction(RPC_NewFile, NewFile);
	AddFunction(RPC_DeleteFile, DeleteFile);
	AddFunction(RPC_NewPickup, NewPickup);
	AddFunction(RPC_DeletePickup, DeletePickup);
	AddFunction(RPC_New3DLabel, New3DLabel);
	AddFunction(RPC_Delete3DLabel, Delete3DLabel);

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
	AddFunction(RPC_ScriptingSetVehicleEngineState, ScriptingSetVehicleEngineState);
	AddFunction(RPC_ScriptingTurnTaxiLights, ScriptingTurnTaxiLights);
	AddFunction(RPC_ScriptingControlCar, ScriptingControlCar);
	AddFunction(RPC_ScriptingSetCarLights,ScriptingSetVehicleLights);
	AddFunction(RPC_ScriptingRepairCarTyres, ScriptingRepairVehicleTyres);
	AddFunction(RPC_ScriptingRepairCarWindows, ScriptingRepairVehicleWindows);
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
	AddFunction(RPC_ScriptingSetVehiclePetrolTankHealth, ScriptingSetVehiclePetrolTankHealth);
	AddFunction(RPC_ScriptingSetPlayerArmour, ScriptingSetPlayerArmour);
	AddFunction(RPC_ScriptingSetPlayerMoveSpeed, ScriptingSetPlayerMoveSpeed);
	AddFunction(RPC_ScriptingSetVehicleMoveSpeed, ScriptingSetVehicleMoveSpeed);
	AddFunction(RPC_ScriptingSetVehicleTurnSpeed, ScriptingSetVehicleTurnSpeed);
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
	AddFunction(RPC_ScriptingSetActorName, ScriptingSetActorName);
	AddFunction(RPC_ScriptingToggleActorNametag, ScriptingToggleActorNametag);
	AddFunction(RPC_ScriptingToggleActorBlip, ScriptingToggleActorBlip);
	AddFunction(RPC_ScriptingSetActorColor, ScriptingSetActorColor);
	AddFunction(RPC_ScriptingToggleActorFrozen, ScriptingToggleActorFrozen);
	AddFunction(RPC_ScriptingToggleActorHelmet, ScriptingToggleActorHelmet);
	AddFunction(RPC_ScriptingWarpActorIntoVehicle, ScriptingWarpActorIntoVehicle);
	AddFunction(RPC_ScriptingRemoveActorFromVehicle, ScriptingRemoveActorFromVehicle);
	AddFunction(RPC_ScriptingSetBlipColor, ScriptingSetBlipColor);
	AddFunction(RPC_ScriptingSetBlipSize, ScriptingSetBlipSize);
	AddFunction(RPC_ScriptingToggleBlipShortRange, ScriptingToggleBlipShortRange);
	AddFunction(RPC_ScriptingToggleBlipRoute, ScriptingToggleBlipRoute);
	AddFunction(RPC_ScriptingSetBlipName, ScriptingSetBlipName);
	AddFunction(RPC_ScriptingSetBlipIcon, ScriptingSetBlipIcon);
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
	AddFunction(RPC_ScriptingSetPickupValue, ScriptingSetPickupValue);
	AddFunction(RPC_ScriptingSetPlayerCameraPos, ScriptingSetPlayerCameraPos);
	AddFunction(RPC_ScriptingSetPlayerCameraLookAt, ScriptingSetPlayerCameraLookAt);
	AddFunction(RPC_ScriptingResetPlayerCamera, ScriptingResetPlayerCamera);
	AddFunction(RPC_ScriptingCreateFire, ScriptingCreateFire);
	AddFunction(RPC_ScriptingDeleteFire, ScriptingDeleteFire);
	AddFunction(RPC_ScriptingCreateExplosion, ScriptingCreateExplosion);
	AddFunction(RPC_ScriptingForcePlayerAnim, ScriptingForcePlayerAnimation);
	AddFunction(RPC_ScriptingForceActorAnimation, ScriptingForceActorAnimation);
	AddFunction(RPC_ScriptingBlockWeaponChange, ScriptingBlockWeaponScroll);
	AddFunction(RPC_ScriptingBlockWeaponDrop, ScriptingBlockWeaponDrop);
	AddFunction(RPC_ScriptingFadeScreenOut, ScriptingFadeScreenOut);
	AddFunction(RPC_ScriptingFadeScreenIn, ScriptingFadeScreenIn);
	AddFunction(RPC_ScriptingPlayPoliceReport, ScriptingPlayPoliceReport);
	AddFunction(RPC_ScriptingPlayMissionCompleteAudio, ScriptingPlayMissionCompleteAudio);
	AddFunction(RPC_ScriptingPlayGameAudio, ScriptingPlayGameAudio);
	AddFunction(RPC_ScriptingRequestAnims, ScriptingRequestAnims);
	AddFunction(RPC_ScriptingReleaseAnims, ScriptingReleaseAnims);
	AddFunction(RPC_ScriptingForceWind, ScriptingForceWind);
	AddFunction(RPC_ScriptingSetNametags, ScriptingSetNametags);
	AddFunction(RPC_ScriptingAttachCam, ScriptingAttachCam);
	AddFunction(RPC_ScriptingDisplayHudNotification, ScriptingDisplayHudNotification);
	AddFunction(RPC_ScriptingSetVehicleFollowMode, ScriptingSetVehicleFollowMode);
	AddFunction(RPC_ScriptingSetVehicleFollowOffset, ScriptingSetVehicleFollowOffset);
	AddFunction(RPC_ScriptingSetAmmoInClip, ScriptingSetAmmoInClip);
	AddFunction(RPC_ScriptingSetAmmo, ScriptingSetAmmo);
	AddFunction(RPC_ScriptingCreatePlayerBlip, ScriptingCreatePlayerBlip);
	AddFunction(RPC_ScriptingRemovePlayerBlip, ScriptingRemovePlayerBlip);
	AddFunction(RPC_ScriptingChangePlayerBlip, ScriptingChangePlayerBlip);
	AddFunction(RPC_ScriptingSetVehicleGPSState, ScriptingSetVehicleGPSState);
	AddFunction(RPC_ScriptingSetVehicleAlarm, ScriptingSetVehicleAlarm);
	AddFunction(RPC_ScriptingSetVehicleTryeState, ScriptingSetVehicleTyreState);
	AddFunction(RPC_ScriptingSetVehicleWindowState, ScriptingSetVehicleWindowState);
	AddFunction(RPC_ScriptingSetPlayerUseMobilePhone, ScriptingSetPlayerUseMobilePhone);
	AddFunction(RPC_ScriptingStopActorDriving, ScriptingStopActorDriving);
	AddFunction(RPC_ScriptingActorDriveToCoords, ScriptingActorDriveToCoords);
	AddFunction(RPC_ScriptingMarkVehicleAsActorVehicle, ScriptingMarkVehicleAsActorVehicle);
	AddFunction(RPC_ScriptingActorSaySpeech, ScriptingForceActorSpeech);
	AddFunction(RPC_ScriptingPlayerSaySpeech, ScriptingForcePlayerSpeech);
	AddFunction(RPC_ScriptingLetPlayerDriveAutomatic, ScriptingLetPlayerDriveAutomatic);
	AddFunction(RPC_ScriptingSetPlayerDimension, ScriptingSetPlayerDimension);
	AddFunction(RPC_ScriptingSetVehicleDimension, ScriptingSetVehicleDimension);
	AddFunction(RPC_ResetVehicleEnterExit, ResetVehicleEnterExit);
	AddFunction(RPC_ScriptingTogglePlayerLabelForPlayer, ScriptingTogglePlayerLabelForPlayer);
	AddFunction(RPC_ScriptingFixVehicle, ScriptingFixVehicle);
	AddFunction(RPC_ScriptingAttachObject, AttachObject);
	AddFunction(RPC_ScriptingDetachObject, DetachObject);
	AddFunction(RPC_ScriptingMoveObject, ScriptingMoveObject);
	AddFunction(RPC_ScriptingRotateObject, ScriptingRotateObject);
	AddFunction(RPC_ScriptingSetObjectDimension, ScriptingSetObjectDimension);
	AddFunction(RPC_ScriptingSetObjectInterior, ScriptingSetObjectInterior);
	AddFunction(RPC_ScriptingSetCheckpointDimension, ScriptingSetCheckpointDimension);
	AddFunction(RPC_ScriptingExplodeCar, ScriptingExplodeCar);
	AddFunction(RPC_ScriptingSet3DLabelPosition, ScriptingSet3DLabelPosition);
	AddFunction(RPC_ScriptingSet3DLabelText, ScriptingSet3DLabelText);
	AddFunction(RPC_ScriptingSet3DLabelColor, ScriptingSet3DLabelColor);
	AddFunction(RPC_ScriptingSet3DLabelVisible, ScriptingSet3DLabelVisible);
	AddFunction(RPC_ScriptingSet3DLabelStreamingDistance, ScriptingSet3DLabelStreamingDistance);
	AddFunction(RPC_ScriptingSet3DLabelDimension, ScriptingSet3DLabelDimension);
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
	RemoveFunction(RPC_ScriptingGivePlayerWeapon);
	RemoveFunction(RPC_ScriptingSetPlayerGravity);
	RemoveFunction(RPC_ScriptingSetSpawnLocation);
	RemoveFunction(RPC_ScriptingSetVehicleIndicators);
	RemoveFunction(RPC_ScriptingSoundVehicleHorn);
	RemoveFunction(RPC_ScriptingGiveHelmet);
	RemoveFunction(RPC_ScriptingRemoveHelmet);
	RemoveFunction(RPC_ScriptingSetModel);
	RemoveFunction(RPC_ScriptingToggleControls);
	RemoveFunction(RPC_ScriptingSetHeading);
	RemoveFunction(RPC_ScriptingSetVehicleDirtLevel);
	RemoveFunction(RPC_ScriptingSetVehicleSirenState);
	RemoveFunction(RPC_ScriptingSetVehicleEngineState);
	RemoveFunction(RPC_ScriptingTurnTaxiLights);
	RemoveFunction(RPC_ScriptingControlCar);
	RemoveFunction(RPC_ScriptingSetCarLights);
	RemoveFunction(RPC_ScriptingRepairCarTyres);
	RemoveFunction(RPC_ScriptingRepairCarWindows);
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
	RemoveFunction(RPC_ScriptingSetVehicleTurnSpeed);
	RemoveFunction(RPC_ScriptingRemoveWeapons);
	RemoveFunction(RPC_ScriptingSetWantedLevel);
	RemoveFunction(RPC_ScriptingWarpPlayerIntoVehicle);
	RemoveFunction(RPC_ScriptingRemovePlayerFromVehicle);
	RemoveFunction(RPC_ScriptingSetCameraBehindPlayer);
	RemoveFunction(RPC_ScriptingSetPlayerDucking);
	RemoveFunction(RPC_ScriptingSetPlayerInvincible);
	RemoveFunction(RPC_ScriptingSetActorCoordinates);
	RemoveFunction(RPC_ScriptingSetActorHeading);
	RemoveFunction(RPC_ScriptingActorWalkToCoordinates);
	RemoveFunction(RPC_ScriptingSetActorName);
	RemoveFunction(RPC_ScriptingToggleActorNametag);
	RemoveFunction(RPC_ScriptingToggleActorBlip);
	RemoveFunction(RPC_ScriptingSetActorColor);
	RemoveFunction(RPC_ScriptingToggleActorFrozen);
	RemoveFunction(RPC_ScriptingToggleActorFrozen);
	RemoveFunction(RPC_ScriptingWarpActorIntoVehicle);
	RemoveFunction(RPC_ScriptingRemoveActorFromVehicle);
	RemoveFunction(RPC_ScriptingSetBlipColor);
	RemoveFunction(RPC_ScriptingSetBlipSize);
	RemoveFunction(RPC_ScriptingToggleBlipShortRange);
	RemoveFunction(RPC_ScriptingToggleBlipRoute);
	RemoveFunction(RPC_ScriptingSetBlipName);
	RemoveFunction(RPC_ScriptingSetBlipIcon);
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
	RemoveFunction(RPC_ScriptingSetPickupValue);
	RemoveFunction(RPC_ScriptingSetPlayerCameraPos);
	RemoveFunction(RPC_ScriptingSetPlayerCameraLookAt);
	RemoveFunction(RPC_ScriptingResetPlayerCamera);
	RemoveFunction(RPC_ScriptingCreateFire);
	RemoveFunction(RPC_ScriptingDeleteFire);
	RemoveFunction(RPC_ScriptingCreateExplosion);
	RemoveFunction(RPC_ScriptingForcePlayerAnim);
	RemoveFunction(RPC_ScriptingForceActorAnimation);
	RemoveFunction(RPC_ScriptingBlockWeaponChange);
	RemoveFunction(RPC_ScriptingBlockWeaponDrop);
	RemoveFunction(RPC_ScriptingFadeScreenOut);
	RemoveFunction(RPC_ScriptingFadeScreenIn);
	RemoveFunction(RPC_ScriptingPlayPoliceReport);
	RemoveFunction(RPC_ScriptingPlayMissionCompleteAudio);
	RemoveFunction(RPC_ScriptingPlayGameAudio);
	RemoveFunction(RPC_ScriptingRequestAnims);
	RemoveFunction(RPC_ScriptingReleaseAnims);
	RemoveFunction(RPC_ScriptingForceWind);
	RemoveFunction(RPC_ScriptingSetNametags);
	RemoveFunction(RPC_ScriptingAttachCam);
	RemoveFunction(RPC_ScriptingDisplayHudNotification);
	RemoveFunction(RPC_ScriptingSetVehicleFollowMode);
	RemoveFunction(RPC_ScriptingSetVehicleFollowOffset);
	RemoveFunction(RPC_ScriptingSetAmmoInClip);
	RemoveFunction(RPC_ScriptingSetAmmo);
	RemoveFunction(RPC_ScriptingCreatePlayerBlip);
	RemoveFunction(RPC_ScriptingRemovePlayerBlip);
	RemoveFunction(RPC_ScriptingChangePlayerBlip);
	RemoveFunction(RPC_ScriptingSetVehicleGPSState);
	RemoveFunction(RPC_ScriptingSetVehicleWindowState);
	RemoveFunction(RPC_ScriptingSetVehicleTryeState);
	RemoveFunction(RPC_ScriptingSetVehicleAlarm);
	RemoveFunction(RPC_ScriptingSetPlayerUseMobilePhone);
	RemoveFunction(RPC_ScriptingStopActorDriving);
	RemoveFunction(RPC_ScriptingActorDriveToCoords);
	RemoveFunction(RPC_ScriptingMarkVehicleAsActorVehicle);
	RemoveFunction(RPC_ScriptingPlayerSaySpeech);
	RemoveFunction(RPC_ScriptingActorSaySpeech);
	RemoveFunction(RPC_ScriptingLetPlayerDriveAutomatic);
	RemoveFunction(RPC_ScriptingSetPlayerDimension);
	RemoveFunction(RPC_ResetVehicleEnterExit);
	RemoveFunction(RPC_ScriptingTogglePlayerLabelForPlayer);
	RemoveFunction(RPC_ScriptingFixVehicle);
	RemoveFunction(RPC_ScriptingAttachObject);
	RemoveFunction(RPC_ScriptingDetachObject);
	RemoveFunction(RPC_ScriptingSetObjectInterior);
	RemoveFunction(RPC_ScriptingSet3DLabelColor);
	RemoveFunction(RPC_ScriptingSet3DLabelDimension);
	RemoveFunction(RPC_ScriptingSet3DLabelPosition);
	RemoveFunction(RPC_ScriptingSet3DLabelStreamingDistance);
	RemoveFunction(RPC_ScriptingSet3DLabelText);
	RemoveFunction(RPC_ScriptingSet3DLabelVisible);
	RemoveFunction(RPC_New3DLabel);
	RemoveFunction(RPC_Delete3DLabel);
}
