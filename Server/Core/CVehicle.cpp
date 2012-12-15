//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicle.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CVehicle.h"
#include "CNetworkManager.h"
#include "CPlayerManager.h"
#include <CLogFile.h>
#include "CEvents.h"

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;
extern CEvents * g_pEvents;


CVehicle::CVehicle(EntityId vehicleId, int iModelId, CVector3 vecSpawnPosition, CVector3 vecSpawnRotation, BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4)
{
	m_vehicleId = vehicleId;
	m_iModelId = iModelId;
	m_vecSpawnPosition = vecSpawnPosition;
	m_vecSpawnRotation = vecSpawnRotation;
	m_byteSpawnColors[0] = byteColor1;
	m_byteSpawnColors[1] = byteColor2;
	m_byteSpawnColors[2] = byteColor3;
	m_byteSpawnColors[3] = byteColor4;
	m_iRespawnDelay = -1;
	m_bActorVehicle = false;
	m_ucDimension = 254;
	Reset();
	SpawnForWorld();
}

CVehicle::~CVehicle()
{
	DestroyForWorld();
}

void CVehicle::Reset()
{
	m_pDriver = NULL;
	memset(m_pPassengers, 0, sizeof(m_pPassengers));
	m_uiHealth = 1000;
	m_fPetrolTankHealth = 1000.0f;
	m_vecPosition = m_vecSpawnPosition;
	m_vecRotation = m_vecSpawnRotation;
	memset(&m_vecTurnSpeed, 0, sizeof(CVector3));
	memset(&m_vecMoveSpeed, 0, sizeof(CVector3));
	memcpy(m_byteColors, m_byteSpawnColors, sizeof(m_byteColors));
	memset(&m_bIndicatorState, 0, sizeof(m_bIndicatorState));
	ResetComponents(false);
	m_fDirtLevel = 0.0f;
	m_iHornDuration = 0;
	m_bSirenState = false;
	m_ucVariation = 0;
	m_iLocked = 0;
	m_bEngineStatus = false;
	m_bLights = false;
	m_fDoor[0] = 0.0f;
	m_fDoor[1] = 0.0f;
	m_fDoor[2] = 0.0f;
	m_fDoor[3] = 0.0f;
	m_fDoor[4] = 0.0f;
	m_fDoor[5] = 0.0f;
	m_bWindow[0] = false;
	m_bWindow[1] = false;
	m_bWindow[2] = false;
	m_bWindow[3] = false;
	m_bTaxiLight = true;
	m_bTyre[0] = false;
	m_bTyre[1] = false;
	m_bTyre[2] = false;
	m_bTyre[3] = false;
	m_bTyre[4] = false;
	m_bTyre[5] = false;
	m_bGpsState = false;
}

void CVehicle::SpawnForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.WriteCompressed(m_vehicleId);
	bsSend.Write(m_iModelId);
	bsSend.Write(m_uiHealth);
	bsSend.Write(m_fPetrolTankHealth);
	bsSend.Write(m_vecPosition);

	if(!m_vecRotation.IsEmpty())
	{
		bsSend.Write1();
		bsSend.Write(m_vecRotation);
	}
	else
		bsSend.Write0();

	if(!m_vecTurnSpeed.IsEmpty())
	{
		bsSend.Write1();
		bsSend.Write(m_vecTurnSpeed);
	}
	else
		bsSend.Write0();

	if(!m_vecMoveSpeed.IsEmpty())
	{
		bsSend.Write1();
		bsSend.Write(m_vecMoveSpeed);
	}
	else
		bsSend.Write0();

	bsSend.Write(m_iRespawnDelay);

	bsSend.Write((char *)m_byteColors, sizeof(m_byteColors));

	bsSend.Write(m_fDirtLevel);

	for(int i = 0; i < 4; ++ i)
		bsSend.WriteBit(m_bIndicatorState[i]);

	for(int i = 0; i < 9; ++ i)
		bsSend.WriteBit(m_bComponents[i]);

	bsSend.Write(m_iHornDuration);
	bsSend.Write(m_bSirenState);
	bsSend.Write(m_iLocked);
	bsSend.WriteBit(m_bEngineStatus);
	bsSend.Write(m_bLights);
	bsSend.Write(m_fDoor[0]);
	bsSend.Write(m_fDoor[1]);
	bsSend.Write(m_fDoor[2]);
	bsSend.Write(m_fDoor[3]);
	bsSend.Write(m_fDoor[4]);
	bsSend.Write(m_fDoor[5]);
	bsSend.Write(m_bWindow[0]);
	bsSend.Write(m_bWindow[1]);
	bsSend.Write(m_bWindow[2]);
	bsSend.Write(m_bWindow[3]);
	bsSend.Write(m_bTaxiLight);
	bsSend.Write(m_bGpsState);

	if(m_ucVariation != 0)
	{
		bsSend.Write1();
		bsSend.Write(m_ucVariation);
	}
	else
		bsSend.Write0();

	g_pNetworkManager->RPC(RPC_NewVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

	// Mark vehicle as actor vehicle
	bsSend.Reset();
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_bActorVehicle);
	g_pNetworkManager->RPC(RPC_ScriptingMarkVehicleAsActorVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);


	SetColors(m_byteColors[0],m_byteColors[1],m_byteColors[2],m_byteColors[3]);
}

void CVehicle::DestroyForPlayer(EntityId playerId)
{
	CBitStream bsSend;
	bsSend.WriteCompressed(m_vehicleId);
	g_pNetworkManager->RPC(RPC_DeleteVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CVehicle::SpawnForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			SpawnForPlayer(i);
	}
}

void CVehicle::DestroyForWorld()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
			DestroyForPlayer(i);
	}
}

bool CVehicle::IsOccupied()
{
	// Do we have a driver?
	if(m_pDriver)
		return true;
	else
	{
		// Loop through all passenger seats
		for(BYTE i = 0; i < MAX_VEHICLE_PASSENGERS; i++)
		{
			// Do we have a passenger in this seat?
			if(m_pPassengers[i])
				return true;
		}
	}

	// Vehicle is not occupied
	return false;
}

void CVehicle::SetPassenger(BYTE byteSeatId, CPlayer * pPassenger)
{
	if(byteSeatId < MAX_VEHICLE_PASSENGERS)
		return;

	m_pPassengers[byteSeatId] = pPassenger;
}

CPlayer * CVehicle::GetPassenger(BYTE byteSeatId)
{
	if(byteSeatId >= MAX_VEHICLE_PASSENGERS)
		return NULL;

	return m_pPassengers[byteSeatId];
}

void CVehicle::SetOccupant(BYTE byteSeatId, CPlayer * pOccupant)
{
	if(byteSeatId == 0)
		SetDriver(pOccupant);
	else
		SetPassenger((byteSeatId - 1), pOccupant);
}

CPlayer * CVehicle::GetOccupant(BYTE byteSeatId)
{
	if(byteSeatId == 0)
		return GetDriver();

	return GetPassenger(byteSeatId - 1);
}

void CVehicle::Respawn()
{
	DestroyForWorld();
	Reset();
	SpawnForWorld();
}

void CVehicle::StoreInVehicleSync(InVehicleSyncData * syncPacket)
{
	m_vecPosition = syncPacket->vecPos;
	m_vecRotation = syncPacket->vecRotation;
	if(m_uiHealth != syncPacket->uiHealth || m_fPetrolTankHealth != syncPacket->fPetrolHealth)
	{
		CSquirrelArguments pArguments;
		pArguments.push(m_vehicleId);
		pArguments.push((int)m_uiHealth);
		pArguments.push((int)syncPacket->uiHealth);
		pArguments.push(m_fPetrolTankHealth);
		pArguments.push(syncPacket->fPetrolHealth);

		g_pEvents->Call("vehicleDamage", &pArguments);
	}
	m_uiHealth = syncPacket->uiHealth;
	m_fPetrolTankHealth = syncPacket->fPetrolHealth;
	memcpy(m_byteColors, syncPacket->byteColors, sizeof(m_byteColors));
	memcpy(&m_vecTurnSpeed, &syncPacket->vecTurnSpeed, sizeof(CVector3));
	memcpy(&m_vecMoveSpeed, &syncPacket->vecMoveSpeed, sizeof(CVector3));
	m_fDirtLevel = syncPacket->fDirtLevel;
	m_bSirenState = syncPacket->bSirenState;
	m_bEngineStatus = syncPacket->bEngineStatus;
	m_bLights = syncPacket->bLights;
	m_fDoor[0] = syncPacket->fDoor[0];
	m_fDoor[1] = syncPacket->fDoor[1];
	m_fDoor[2] = syncPacket->fDoor[2];
	m_fDoor[3] = syncPacket->fDoor[3];
	m_fDoor[4] = syncPacket->fDoor[4];
	m_fDoor[5] = syncPacket->fDoor[5];
	m_bWindow[0] = syncPacket->bWindow[0];
	m_bWindow[1] = syncPacket->bWindow[1];
	m_bWindow[2] = syncPacket->bWindow[2];
	m_bWindow[3] = syncPacket->bWindow[3];
	m_bTaxiLight = syncPacket->bTaxiLights;
	m_bTyre[0] = syncPacket->bTyre[0];
	m_bTyre[1] = syncPacket->bTyre[1];
	m_bTyre[2] = syncPacket->bTyre[2];
	m_bTyre[3] = syncPacket->bTyre[3];
	m_bTyre[4] = syncPacket->bTyre[4];
	m_bTyre[5] = syncPacket->bTyre[5];
	m_bGpsState = syncPacket->bGpsState;
}

void CVehicle::StorePassengerSync(PassengerSyncData * syncPacket)
{
	
}


void CVehicle::SetModel(int iModelId)
{
	m_iModelId = iModelId;
	ResetComponents();
	// TODO: RPC
}

int CVehicle::GetModel()
{
	return m_iModelId;
}

void CVehicle::SetHealth(unsigned int uHealth)
{
	m_uiHealth = uHealth;

	CBitStream bsSend;
	bsSend.WriteCompressed(m_vehicleId);
	bsSend.Write(uHealth);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleHealth, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

unsigned int CVehicle::GetHealth()
{
	return m_uiHealth;
}

void CVehicle::SetPosition(const CVector3& vecPosition)
{
	m_vecPosition = vecPosition;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(vecPosition);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetPositionSave(CVector3 vecPosition)
{
	m_vecPosition = vecPosition;
}

void CVehicle::GetPosition(CVector3& vecPosition)
{
	vecPosition = m_vecPosition;
}

void CVehicle::SetRotation(const CVector3& vecRotation)
{
	m_vecRotation = vecRotation;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(vecRotation);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleRotation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetRotationSave(CVector3 vecRotation)
{
	m_vecRotation = vecRotation;
}

void CVehicle::GetRotation(CVector3& vecRotation)
{
	vecRotation = m_vecRotation;
}

void CVehicle::SetDirtLevel(float fDirtLevel)
{
	m_fDirtLevel = fDirtLevel;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(fDirtLevel);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleDirtLevel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

float CVehicle::GetDirtLevel()
{
	return m_fDirtLevel;
}

void CVehicle::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	m_vecTurnSpeed = vecTurnSpeed;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(vecTurnSpeed);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleTurnSpeed, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	vecTurnSpeed = m_vecTurnSpeed;
}

void CVehicle::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	m_vecMoveSpeed = vecMoveSpeed;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(vecMoveSpeed);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleMoveSpeed, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	vecMoveSpeed = m_vecMoveSpeed;
}

void CVehicle::SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4)
{
	m_byteColors[0] = byteColor1;
	m_byteColors[1] = byteColor2;
	m_byteColors[2] = byteColor3;
	m_byteColors[3] = byteColor4;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write((char *)m_byteColors, sizeof(m_byteColors));
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4)
{
	byteColor1 = m_byteColors[0];
	byteColor2 = m_byteColors[1];
	byteColor3 = m_byteColors[2];
	byteColor4 = m_byteColors[3];
}

void CVehicle::SoundHorn(unsigned int iDuration)
{
	m_iHornDuration = iDuration;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(iDuration);
	g_pNetworkManager->RPC(RPC_ScriptingSoundVehicleHorn, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetSirenState(bool bSirenState)
{
	m_bSirenState = bSirenState;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(bSirenState);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleSirenState, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}


bool CVehicle::GetSirenState()
{
	return m_bSirenState;
}

bool CVehicle::SetLocked(unsigned int iLocked)
{
	if(iLocked >= 0 && iLocked < 3)
	{
		m_iLocked = iLocked;

		CBitStream bsSend;
		bsSend.Write(m_vehicleId);
		bsSend.Write(iLocked);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleLocked, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		return true;
	}
	else
	{
		CLogFile::Printf("Unkown vehicle doorlock state %d (only supported from 0 to 2)!",iLocked);
		return false;
	}

	return false;
}

unsigned int CVehicle::GetLocked()
{
	return m_iLocked;
}

void CVehicle::SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight)
{
	m_bIndicatorState[0] = bFrontLeft;
	m_bIndicatorState[1] = bFrontRight;
	m_bIndicatorState[2] = bBackLeft;
	m_bIndicatorState[3] = bBackRight;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write((unsigned char)(bFrontLeft + 2*bFrontRight + 4*bBackLeft + 8*bBackRight));
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleIndicators, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

bool CVehicle::GetIndicatorState(unsigned char ucSlot)
{
	if(ucSlot >= 0 && ucSlot <= 3)
		return m_bIndicatorState[ucSlot];
	
	return false;
}

void CVehicle::SetComponentState(unsigned char ucSlot, bool bOn)
{
	if(ucSlot >= 0 && ucSlot <= 8)
	{
		if(m_bComponents[ucSlot] != bOn)
		{
			m_bComponents[ucSlot] = bOn;

			CBitStream bsSend;
			bsSend.Write(m_vehicleId);

			for(int i = 0; i < 9; ++ i)
				bsSend.WriteBit(m_bComponents[i]);

			g_pNetworkManager->RPC(RPC_ScriptingSetVehicleComponents, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
	}
}

void CVehicle::ResetComponents(bool bNotify)
{
	memset(&m_bComponents, 0, sizeof(m_bComponents));

	// Make sure some default components are added (they can be removed)
	switch(m_iModelId)
	{
		case 4: // Benson
		case 51: // Mule
		case 85: // Steed
		case 104: // Yankee
			m_bComponents[0] = true; // missing half of it otherwise, could chose anything from 0 on
			break;
		case 6: // Blista
		case 26: // Faction
		case 76: // Ruiner
		case 87: // Stratum
			m_bComponents[1] = true; // Roof window
			break;
		case 13: // Cabby
			m_bComponents[5] = true; // Taxi sign
			break;
		case 35: // Futo
			m_bComponents[0] = true; // driver seat
			break;
		case 52: // Noose
		case 63: // Police
		case 64: // Police 2
			m_bComponents[0] = true; // Sirens
			break;
		case 89: // Sultan
			m_bComponents[1] = true; // Air vent
			break;
		case 92: // Taxi
		case 93: // Taxi 2
			m_bComponents[4] = true; // Taxi sign
			break;
		case 98: // Vigero (trashed)
			m_bComponents[2] = true;
			break;
		case 105: // Freeway
		case 109: // PCJ
			m_bComponents[0] = true; // Tank
			m_bComponents[6] = true; // Exhaust
			break;
		case 108: // NRG
			m_bComponents[3] = true; // Body
			m_bComponents[7] = true; // Exhaust
			break;
		case 112: // Annihilator
		case 113: // Maverick
		case 114: // Police Maverick
		case 115: // Heli-Tours Maverick
			m_bComponents[0] = true; // Rotor blades
			break;
		case 123: // Tropic
			m_bComponents[4] = true;
			break;
	}

	if(bNotify)
	{
		CBitStream bsSend;
		bsSend.Write(m_vehicleId);

		for(int i = 0; i < 9; ++ i)
			bsSend.WriteBit(m_bComponents[i]);

		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleComponents, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

bool CVehicle::GetComponentState(unsigned char ucSlot)
{
	if(ucSlot >= 0 && ucSlot <= 8)
		return m_bComponents[ucSlot];

	return false;
}

void CVehicle::SetVariation(unsigned char ucVariation)
{
	if(m_ucVariation != ucVariation)
	{
		m_ucVariation = ucVariation;

		CBitStream bsSend;
		bsSend.Write(m_vehicleId);
		bsSend.Write(ucVariation);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleVariation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

unsigned char CVehicle::GetVariation()
{
	return m_ucVariation;
}

void CVehicle::SetEngineStatus(bool bEngineStatus)
{
	m_bEngineStatus = bEngineStatus;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(bEngineStatus);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleEngineState, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}


bool CVehicle::GetEngineStatus()
{
	return m_bEngineStatus;
}

void CVehicle::TurnTaxiLights(bool bTaxilights)
{
	m_bTaxiLight = bTaxilights;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_bTaxiLight);
	g_pNetworkManager->RPC(RPC_ScriptingTurnTaxiLights, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetCarDoorAngle(unsigned int uiDoor, bool bClosed, float fAngle)
{
	m_fDoor[uiDoor] = fAngle;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(uiDoor);
	bsSend.Write(bClosed);
	bsSend.Write(fAngle);
	g_pNetworkManager->RPC(RPC_ScriptingControlCar, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetLights(bool bLights)
{
	m_bLights = bLights;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(bLights);
	g_pNetworkManager->RPC(RPC_ScriptingSetCarLights, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

bool CVehicle::GetLights()
{
	return m_bLights;
}

bool CVehicle::GetTaxiLights()
{
	return m_bTaxiLight;
}

void CVehicle::RepairWheels()
{
	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	g_pNetworkManager->RPC(RPC_ScriptingRepairCarTyres, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::RepairWindows()
{
	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	g_pNetworkManager->RPC(RPC_ScriptingRepairCarWindows, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::StoreEmptyVehicle(EMPTYVEHICLESYNCPACKET * syncPacket)
{
	// Check stuff
	if(syncPacket->bLights != GetLights())
		SetLights(syncPacket->bLights);

	if(syncPacket->bSirenState != GetSirenState())
		SetSirenState(syncPacket->bSirenState);

	if(syncPacket->bTaxiLights != GetTaxiLights())
		TurnTaxiLights(syncPacket->bTaxiLights);

	if(syncPacket->fDirtLevel != GetDirtLevel())
		SetDirtLevel(syncPacket->fDirtLevel);

	if(syncPacket->fPetrolHealth != GetPetrolTankHealth())
		SetPetrolTankHealth(syncPacket->fPetrolHealth);

	if(syncPacket->uiHealth != GetHealth())
		SetHealth(syncPacket->uiHealth);

	// Only when the vehicle stands still -> update stuff
	/*if(syncPacket->vecMoveSpeed.Length() == 0)
	{
		CVector3 vecPos; GetPosition(vecPos);
		if((vecPos-syncPacket->vecPosition).Length() > 5.0f || (vecPos-syncPacket->vecPosition).Length() < -5.0f)
			SetPosition(syncPacket->vecPosition);

		CVector3 vecRot; GetRotation(vecRot);
		if((vecRot-syncPacket->vecRotation).Length() > 5.0f || (vecRot-syncPacket->vecRotation).Length() < -5.0f)
			SetRotation(syncPacket->vecRotation);

		CVector3 vecTurnSpeed; GetTurnSpeed(vecTurnSpeed);
		if((vecTurnSpeed-syncPacket->vecTurnSpeed).Length() > 5.0f || (vecTurnSpeed-syncPacket->vecTurnSpeed).Length() < -5.0f)
			SetTurnSpeed(syncPacket->vecTurnSpeed);

		CVector3 vecMoveSpeed; GetMoveSpeed(vecMoveSpeed);
		if((vecMoveSpeed-syncPacket->vecMoveSpeed).Length() > 5.0f || (vecMoveSpeed-syncPacket->vecMoveSpeed).Length() < -5.0f)
			SetMoveSpeed(syncPacket->vecMoveSpeed);
	}*/

	/*for(unsigned int ui = 0; ui <= 3; ui++)
	{
		if(syncPacket->bWindow[ui] != m_bWindow[ui])
			SetWindowState(ui, syncPacket->bWindow[ui]);
	}

	for(unsigned int ui = 0; ui <= 5; ui++)
	{
		if(syncPacket->bTyre[ui] != m_bTyre[ui])
			SetTyreState(ui, syncPacket->bTyre[ui]);
	}*/
}

bool CVehicle::GetWindowState(unsigned int uiWindow)
{
	return m_bWindow[uiWindow];
}

void CVehicle::SetWindowState(unsigned int uiWindow, bool bState)
{
	m_bWindow[uiWindow] = bState;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_bWindow[uiWindow]);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleWindowState, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

bool CVehicle::GetTyreState(unsigned int uiTyre)
{
	return m_bTyre[uiTyre];
}

void CVehicle::SetTyreState(unsigned int uiTyre, bool bState)
{
	m_bTyre[uiTyre] = bState;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_bTyre[uiTyre]);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleTryeState, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

float CVehicle::GetPetrolTankHealth()
{
	return m_fPetrolTankHealth;
}

void CVehicle::SetPetrolTankHealth(float fHealth)
{
	m_fPetrolTankHealth = fHealth;
	
	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_fPetrolTankHealth);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehiclePetrolTankHealth, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::SetVehicleGPSState(bool bState)
{
	m_bGpsState = bState;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(bState);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleGPSState, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

bool CVehicle::GetVehicleGPSState()
{
	return m_bGpsState;
}

void CVehicle::SetAlarm(int iDuration)
{
	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(iDuration);
	g_pNetworkManager->RPC(RPC_ScriptingSetVehicleAlarm, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CVehicle::MarkVehicle(bool bToggle)
{
	m_bActorVehicle = bToggle;

	CBitStream bsSend;
	bsSend.Write(m_vehicleId);
	bsSend.Write(m_bActorVehicle);
	g_pNetworkManager->RPC(RPC_ScriptingMarkVehicleAsActorVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}