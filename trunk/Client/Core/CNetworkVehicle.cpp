//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkVehicle.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Save driver/passengers in StreamOut and put them back in vehicle on StreamIn (if
// they are streamed in too (possibly force stream in if not?))

#include "CNetworkVehicle.h"
#include "CPlayerManager.h"
#include "CModelManager.h"
#include "CLocalPlayer.h"
#include "Scripting.h"
#include <SharedUtility.h>
#include "CPools.h"


//extern CChatWindow * g_pChatWindow;
extern CPlayerManager * g_pPlayerManager;
extern CModelManager * g_pModelManager;
extern CLocalPlayer * g_pLocalPlayer;
CNetworkVehicle::CNetworkVehicle(DWORD dwModelHash)
	: CStreamableEntity(STREAM_ENTITY_VEHICLE, 200.0f)
{
	m_pVehicle = NULL;
	m_vehicleId = INVALID_ENTITY_ID;
	m_pDriver = NULL;
	memset(m_pPassengers, 0, sizeof(m_pPassengers));
	m_pModelInfo = CGame::GetModelInfo(MODEL_ADMIRAL_INDEX);
	SetModel(dwModelHash);
	memset(&m_vecPosition, 0, sizeof(CVector3));
	memset(&m_vecRotation, 0, sizeof(CVector3));
	memset(&m_vecMoveSpeed, 0, sizeof(CVector3));
	memset(&m_vecTurnSpeed, 0, sizeof(CVector3));
	memset(m_byteColors, 0, sizeof(m_byteColors));
	m_bSirenState = false;
	memset(m_bIndicatorState, 0, sizeof(m_bIndicatorState));
	memset(m_bComponents, 0, sizeof(m_bComponents));
	m_interp.pos.ulFinishTime = 0;
	m_interp.rot.ulFinishTime = 0;
	m_dwDoorLockState = 0;
	m_ulHornDurationEnd = 0;
	m_uiHealth = 1000;
	m_fPetrolTankHealth = 1000.0f;
	m_fDirtLevel = 1000.0f;
	m_ucVariation = 0;
	m_bEngineStatus = false;
	m_bTaxiLights = false;
	m_fDoor[0] = 0.0f;
	m_fDoor[1] = 0.0f;
	m_fDoor[2] = 0.0f;
	m_fDoor[3] = 0.0f;
	m_fDoor[4] = 0.0f;
	m_fDoor[5] = 0.0f;
	m_bLights = false;
}

CNetworkVehicle::~CNetworkVehicle()
{
	// Remove the driver if we have one
	if(m_pDriver)
		m_pDriver->RemoveFromVehicle();

	// Remove all passengers if we have any
	for(BYTE i = 0; i < MAX_VEHICLE_PASSENGERS; i++)
	{
		if(m_pPassengers[i])
			m_pPassengers[i]->RemoveFromVehicle();
	}

	// Tell the streamer to delete us
	OnDelete();
}

bool CNetworkVehicle::IsOccupied()
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

void CNetworkVehicle::SetPassenger(BYTE bytePassengerId, CNetworkPlayer * pPassenger)
{
	if(bytePassengerId < MAX_VEHICLE_PASSENGERS)
		return;

	m_pPassengers[bytePassengerId] = pPassenger;
}

CNetworkPlayer * CNetworkVehicle::GetPassenger(BYTE bytePassengerId)
{
	if(bytePassengerId >= MAX_VEHICLE_PASSENGERS)
		return NULL;

	return m_pPassengers[bytePassengerId];
}

void CNetworkVehicle::SetOccupant(BYTE byteSeatId, CNetworkPlayer * pOccupant)
{
	if(byteSeatId == 0)
		SetDriver(pOccupant);
	else
		SetPassenger((byteSeatId - 1), pOccupant);
}

CNetworkPlayer * CNetworkVehicle::GetOccupant(BYTE byteSeatId)
{
	if(byteSeatId == 0)
		return GetDriver();

	return GetPassenger(byteSeatId - 1);
}

bool CNetworkVehicle::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		// Add our model info reference
		m_pModelInfo->AddReference(true);

		// Get our model index
		int iModelIndex = m_pModelInfo->GetIndex();

		CLogFile::Printf("CNetworkVehicle::Create 3 - 3");

		CLogFile::Printf("pModelInfo + 0x70 = %d", *(DWORD *)(m_pModelInfo->GetModelInfo() + 0x70));
		//memset((void *)(CGame::GetBase() + 0x841808), 0x90, 5);
		//memset((void *)(CGame::GetBase() + 0x8419B8), 0x90, 5);

		// Create the vehicle
		DWORD dwFunc = (CGame::GetBase() + 0x8415D0);
		CVector3 * pVecPosition = &m_vecPosition;
		IVVehicle * pVehicle = NULL;
		_asm
		{
			push 1
			push 1
			push pVecPosition
			push iModelIndex
			call dwFunc
			add esp, 10h
			mov pVehicle, eax
		}

		CLogFile::Printf("CNetworkVehicle::Create 4");

		// Invalid vehicle?
		if(!pVehicle)
			return false;

		CLogFile::Printf("CNetworkVehicle::Create 4 - 4");

		dwFunc = (CGame::GetBase() + 0xC6CFC0);
		_asm
		{
			push pVehicle
			call dwFunc
			add esp, 4
		}

		CLogFile::Printf("CNetworkVehicle::Create 5");

		dwFunc = (CGame::GetBase() + 0xB77BB0);
		_asm
		{
			push 0
			push pVehicle
			call dwFunc
			add esp, 8
		}

		CLogFile::Printf("CNetworkVehicle::Create 6");

		// Create the vehicle instance
		m_pVehicle = new CIVVehicle(pVehicle);

		// Invalid vehicle instance?
		if(!m_pVehicle)
			return false;

		// Set initial colors
		SetColors(m_byteColors[0], m_byteColors[1], m_byteColors[2], m_byteColors[3]);

		// Disable automatic lights and enable GPS
		Scripting::ForceCarLights(GetScriptingHandle(),1);

		// Disable visible damage & enable "normal" damage
		SetDamageAble(false);
		m_pVehicle->SetCanBeVisiblyDamaged(false);

		// Add the vehicle to the world
		// Not needed as native does it for us
		//m_pVehicle->AddToWorld();

		// Set the initial health
		SetHealth(m_uiHealth);
		SetPetrolTankHealth(m_fPetrolTankHealth);

		// Set the initial position
		SetPosition(m_vecPosition);

		// Set the initial rotation
		SetRotation(m_vecRotation);

		return true;
	}

	// Already created
	return false;
}

void CNetworkVehicle::Destroy()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Remove the driver if we have one
		if(m_pDriver)
			m_pDriver->InternalRemoveFromVehicle();

		// Remove all passengers if we have any
		for(BYTE i = 0; i < MAX_VEHICLE_PASSENGERS; i++)
		{
			if(m_pPassengers[i])
				m_pPassengers[i]->InternalRemoveFromVehicle();
		}

		// Get the vehicle pointer
		IVVehicle * pVehicle = m_pVehicle->GetVehicle();

		*(BYTE *)(pVehicle + 0xF6D) |= 8;

		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Remove references?
		DWORD dwFunc = (CGame::GetBase() + 0x819190);
		_asm
		{
			push pVehicle
			call dwFunc
			add esp, 4
		}

		*(BYTE *)(pVehicle + 0xF6B) &= 0xDF;

		// Delete the vehicle
		dwFunc = pVehicle->m_VFTable->ScalarDeletingDestructor;
		_asm
		{
			push 1
			mov ecx, pVehicle
			call dwFunc
		}

		// Remove our model info reference
		m_pModelInfo->RemoveReference();

		// Delete the vehicle instance
		SAFE_DELETE(m_pVehicle);
	}
}

void CNetworkVehicle::StreamIn()
{
	if(GetDriver() == g_pLocalPlayer)
		CLogFile::Printf("STREAM IN ON LOCAL PLAYERS VEHICLE!");

	// Attempt to create the vehicle
	if(Create())
	{
		// Set the rotation
		SetRotation(m_vecRotation);

		// Set the move speed
		SetMoveSpeed(m_vecMoveSpeed);

		// Set the turn speed
		SetTurnSpeed(m_vecTurnSpeed);
		
		// Set the colors
		SetColors(m_byteColors[0], m_byteColors[1], m_byteColors[2], m_byteColors[3]);

		// Disable visible damage & enable "normal" damage
		SetDamageAble(false);
		m_pVehicle->SetCanBeVisiblyDamaged(false);

		// Disable some damage stuff
		Scripting::SetCarProofs(GetScriptingHandle(), false, false, false, false, false);

		// Restore the health
		SetHealth(m_uiHealth);

		// Restore the siren state
		SetSirenState(m_bSirenState);

		// Restore the petrol tank health
		SetPetrolTankHealth(m_fPetrolTankHealth);

		// Restore the dirt level
		SetDirtLevel(m_fDirtLevel);

		// Apply the lock state
		SetDoorLockState(m_dwDoorLockState);

		// Sound horn if needed
		if(m_ulHornDurationEnd > SharedUtility::GetTime())
			SoundHorn((m_ulHornDurationEnd - SharedUtility::GetTime()));

		// Set the extras
		for(int i = 0; i <= 8; ++ i)
			SetComponentState(i, m_bComponents[i]);

		// Restore the variation
		SetVariation(m_ucVariation);

		if(m_bLights)
			Scripting::ForceCarLights(GetScriptingHandle(),2);
		else if(!m_bLights)
			Scripting::ForceCarLights(GetScriptingHandle(),1);
		
		// Restore the engine state
		SetEngineState(m_bEngineStatus);

		// Reset interpolation
		ResetInterpolation();

		// Add the driver if we have one
		if(m_pDriver)
			m_pDriver->InternalPutInVehicle(this, 0);

		// Add all passengers if we have any
		for(BYTE i = 0; i < MAX_VEHICLE_PASSENGERS; i++)
		{
			if(m_pPassengers[i])
				m_pPassengers[i]->InternalPutInVehicle(this, (i + 1));
		}
	}
}

void CNetworkVehicle::StreamOut()
{
	if(GetDriver() == g_pLocalPlayer)
		CLogFile::Printf("STREAM OUT ON LOCAL PLAYERS VEHICLE!");

	// Save the coordinates
	GetPosition(m_vecPosition);

	// Save the rotation
	GetRotation(m_vecRotation);

	// Save the move speed
	GetMoveSpeed(m_vecMoveSpeed);

	// Save the turn speed
	GetTurnSpeed(m_vecTurnSpeed);

	// Save the color
	GetColors(m_byteColors[0], m_byteColors[1], m_byteColors[2], m_byteColors[3]);

	// Save the health
	m_uiHealth = GetHealth();
	m_fPetrolTankHealth = GetPetrolTankHealth();

	// Save the siren state
	m_bSirenState = GetSirenState();

	// Save the dirt level
	m_fDirtLevel = GetDirtLevel();

	// Save the engine state
	m_bEngineStatus = GetEngineState();

	// Destroy the vehicle
	Destroy();

}

bool CNetworkVehicle::IsMoving()
{
	CVector3 vecMoveSpeed;
	GetMoveSpeed(vecMoveSpeed);

	if(!vecMoveSpeed.IsEmpty())
		return true;

	return false;
}

void CNetworkVehicle::StopMoving()
{
	SetMoveSpeed(CVector3());
}

void CNetworkVehicle::SoundHorn(int iDuration)
{
	if(IsSpawned())
		m_pVehicle->SoundHorn(iDuration);

	m_ulHornDurationEnd = (SharedUtility::GetTime() + iDuration);
}

unsigned int CNetworkVehicle::GetScriptingHandle()
{
	if(IsSpawned())
		return CGame::GetPools()->GetVehiclePool()->HandleOf(m_pVehicle->GetVehicle());

	return 0;
}

void CNetworkVehicle::SetModel(DWORD dwModelHash)
{
	// Get the model index from the model hash
	int iModelIndex = CGame::GetStreaming()->GetModelIndexFromHash(dwModelHash);

	// Do we have an invalid model index?
	if(iModelIndex == -1)
	{
		CLogFile::Printf("CClientVehicle::SetModel Failed (Invalid model hash)!");
		return;
	}

	// Has the model not changed?
	if(m_pModelInfo->GetIndex() == iModelIndex)
	{
		CLogFile::Printf("CClientVehicle::SetModel Failed (Model change not needed)!");
		return;
	}

	// Get the new model info
	CIVModelInfo * pNewModelInfo = CGame::GetModelInfo(iModelIndex);

	// Is the new model info valid?
	if(!pNewModelInfo || !pNewModelInfo->IsValid() || !pNewModelInfo->IsVehicle())
	{
		CLogFile::Printf("CClientVehicle::SetModel Failed (Invalid model)!");
		return;
	}

	// Are we spawned?
	bool bSpawned = IsSpawned();

	if(bSpawned)
	{
		// Stream ourselves out
		CLogFile::Printf("CClientVehicle::SetModel Stream Out Begin");
		StreamOut();
		CLogFile::Printf("CClientVehicle::SetModel Stream Out Complete");
	}

	// Set the new model info
	m_pModelInfo = pNewModelInfo;

	// Were we spawned?
	if(bSpawned)
	{
		// Stream ourselves back in
		CLogFile::Printf("CClientVehicle::SetModel Stream In Begin");
		StreamIn();
		CLogFile::Printf("CClientVehicle::SetModel Stream In Complete");
	}
}

void CNetworkVehicle::SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4)
{
	if(IsSpawned())
		m_pVehicle->SetColors(byteColor1, byteColor2, byteColor3, byteColor4);

	m_byteColors[0] = byteColor1;
	m_byteColors[1] = byteColor2;
	m_byteColors[2] = byteColor3;
	m_byteColors[3] = byteColor4;
}

void CNetworkVehicle::GetColors(BYTE &byteColor1, BYTE &byteColor2, BYTE &byteColor3, BYTE &byteColor4)
{
	if(IsSpawned())
		m_pVehicle->GetColors(&byteColor1, &byteColor2, &byteColor3, &byteColor4);
	else
	{
		byteColor1 = m_byteColors[0];
		byteColor2 = m_byteColors[1];
		byteColor3 = m_byteColors[2];
		byteColor4 = m_byteColors[3];
	}
}

void CNetworkVehicle::SetPosition(const CVector3& vecPosition, bool bDontCancelTasks, bool bResetInterpolation)
{
	if(IsSpawned())
	{
		if(!bDontCancelTasks)
			Scripting::SetCarCoordinatesNoOffset(GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
		else
		{
			// Remove the vehicle from the world
			m_pVehicle->RemoveFromWorld();

			// Set the position in the matrix
			m_pVehicle->SetPosition(vecPosition);

			// Re add the vehicle to the world to apply the matrix change
			m_pVehicle->AddToWorld();
		}
	}
	
	m_vecPosition = vecPosition;

	// Reset interpolation if requested
	if(bResetInterpolation)
		RemoveTargetPosition();
}

void CNetworkVehicle::GetPosition(CVector3& vecPosition)
{
	if(IsSpawned())
		m_pVehicle->GetPosition(vecPosition);
	else
		vecPosition = m_vecPosition;
}

void CNetworkVehicle::SetRotation(const CVector3& vecRotation, bool bResetInterpolation)
{
	if(IsSpawned())
	{
		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(matMatrix);

		// Convert the rotation from degrees to radians
		CVector3 vecNewRotation = Math::ConvertDegreesToRadians(vecRotation);

		// Apply the rotation to the vehicle matrix
		CGame::ConvertEulerAnglesToRotationMatrix(vecNewRotation, matMatrix);

		// Set the new vehicle matrix
		m_pVehicle->SetMatrix(matMatrix);

		// Re add the vehicle to the world to apply the matrix change
		m_pVehicle->AddToWorld();
	}
	
	m_vecRotation = vecRotation;

	// Reset interpolation if requested
	if(bResetInterpolation)
		RemoveTargetRotation();
}

void CNetworkVehicle::GetRotation(CVector3& vecRotation)
{
	if(IsSpawned())
	{
		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(matMatrix);

		// Convert the matrix to euler angles
		CVector3 vecNewRotation;
		CGame::ConvertRotationMatrixToEulerAngles(matMatrix, vecNewRotation);

#pragma message("CGame::ConvertRotationMatrixToEulerAngles should flip the rotation itself")
		// Flip the rotation
		vecNewRotation.fX = ((2 * PI) - vecNewRotation.fX);
		vecNewRotation.fY = ((2 * PI) - vecNewRotation.fY);
		vecNewRotation.fZ = ((2 * PI) - vecNewRotation.fZ);

		// Convert the rotation from radians to degrees
		vecRotation = Math::ConvertRadiansToDegrees(vecNewRotation);
	}
	else
		vecRotation = m_vecRotation;
}

void CNetworkVehicle::SetHealth(unsigned int uiHealth)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetEngineHealth((float)uiHealth);

	m_uiHealth = uiHealth;
}

unsigned int CNetworkVehicle::GetHealth()
{
	// Are we spawned?
	if(IsSpawned())
		return (unsigned int)m_pVehicle->GetEngineHealth();

	return m_uiHealth;
}

void CNetworkVehicle::SetPetrolTankHealth(float fHealth)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetPetrolTankHealth((float)fHealth);

	m_fPetrolTankHealth = fHealth;
}

float CNetworkVehicle::GetPetrolTankHealth()
{
	// Are we spawned?
	if(IsSpawned())
		return m_pVehicle->GetPetrolTankHealth();

	return m_fPetrolTankHealth;
}

void CNetworkVehicle::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetMoveSpeed(vecMoveSpeed);

	m_vecMoveSpeed = vecMoveSpeed;
}

void CNetworkVehicle::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->GetMoveSpeed(vecMoveSpeed);
	else
		vecMoveSpeed = m_vecMoveSpeed;
}

void CNetworkVehicle::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetTurnSpeed(vecTurnSpeed);

	m_vecTurnSpeed = vecTurnSpeed;
}

void CNetworkVehicle::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->GetTurnSpeed(vecTurnSpeed);
	else
		vecTurnSpeed = m_vecTurnSpeed;
}

void CNetworkVehicle::SetSirenState(bool bSirenState)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetSirenState(bSirenState);

	m_bSirenState = bSirenState;
}

bool CNetworkVehicle::GetSirenState()
{
	// Are we spawned?
	if(IsSpawned())
		return m_pVehicle->GetSirenState();

	return m_bSirenState;
}

void CNetworkVehicle::SetDirtLevel(float fDirtLevel)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetDirtLevel(fDirtLevel);

	m_fDirtLevel = fDirtLevel;
}

float CNetworkVehicle::GetDirtLevel()
{
	// Are we spawned?
	if(IsSpawned())
		return m_pVehicle->GetDirtLevel();

	return m_fDirtLevel;
}

void CNetworkVehicle::StoreEmptySync(EMPTYVEHICLESYNCPACKET * emptyVehicleSync)
{
	SetTargetPosition(emptyVehicleSync->vecPosition, TICK_RATE);
	SetTargetRotation(emptyVehicleSync->vecRotation, TICK_RATE);
	SetHealth(emptyVehicleSync->uiHealth);
	SetPetrolTankHealth(emptyVehicleSync->fPetrolHealth);
	SetDirtLevel(emptyVehicleSync->fDirtLevel);
	SetWindowBroken(0,emptyVehicleSync->bWindow[0]);
	SetWindowBroken(1,emptyVehicleSync->bWindow[1]);
	SetWindowBroken(2,emptyVehicleSync->bWindow[2]);
	SetWindowBroken(3,emptyVehicleSync->bWindow[3]);
	SetLightsState(emptyVehicleSync->bLights);
	SetTaxiLightsState(emptyVehicleSync->bTaxiLights);
	SetSirenState(emptyVehicleSync->bSirenState);
	SetCarDoorAngle(0,0,emptyVehicleSync->fDoor[0]);
	SetCarDoorAngle(1,0,emptyVehicleSync->fDoor[1]);
	SetCarDoorAngle(2,0,emptyVehicleSync->fDoor[2]);
	SetCarDoorAngle(3,0,emptyVehicleSync->fDoor[3]);
	SetCarDoorAngle(4,0,emptyVehicleSync->fDoor[4]);
	SetCarDoorAngle(5,0,emptyVehicleSync->fDoor[5]);
}

BYTE CNetworkVehicle::GetMaxPassengers()
{
	// Are we spawned?
	if(IsSpawned())
		return m_pVehicle->GetMaxPasssengers();

	return 0;
}

void CNetworkVehicle::AddToWorld()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Add the vehicle to the world
		m_pVehicle->AddToWorld();
	}
}

void CNetworkVehicle::RemoveFromWorld(bool bStopMoving)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Stop the vehicle from moving to avoid some weird bugs
		if(bStopMoving)
			StopMoving();

		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();
	}
}

void CNetworkVehicle::SetDoorLockState(DWORD dwDoorLockState)
{
	// Get the actual lock state
	DWORD dwState = 0;

	switch(dwDoorLockState)
	{
		case 0:
			dwState = 0;
			break;
		case 1:
			dwState = 3;
			break;
		case 2:
			dwState = 7;
			break;
		default:
			return;
	}

	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetDoorLockState(dwState);

	m_dwDoorLockState = dwState;
}

DWORD CNetworkVehicle::GetDoorLockState()
{
	DWORD dwState = m_dwDoorLockState;

	// Are we spawned?
	if(IsSpawned())
		dwState = m_pVehicle->GetDoorLockState();
		
	switch(dwState)
	{
		case 0:
			return 0;
		case 3:
			return 1;
		case 7:
			return 2;
		default:
			CLogFile::Printf("Unknown vehicle %d door state %d", m_vehicleId, m_pVehicle->GetDoorLockState()); 
	}

	return 0;
}

void CNetworkVehicle::Pulse()
{
	Interpolate();
}

void CNetworkVehicle::UpdateTargetPosition()
{
	// Do we have a target position?
	if(HasTargetPosition())
	{
		// Get our position
		CVector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		// Get the factor of time spent from the interpolation start
		// to the current time.
		unsigned long ulCurrentTime = SharedUtility::GetTime();
		float fAlpha = Math::Unlerp(m_interp.pos.ulStartTime, ulCurrentTime, m_interp.pos.ulFinishTime);

		// Don't let it overcompensate the error too much
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.5f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_interp.pos.fLastAlpha);
		m_interp.pos.fLastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, m_interp.pos.vecError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.5f)
			m_interp.pos.ulFinishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = (vecCurrentPosition + vecCompensation);

		// Check if the distance to interpolate is too far
		if((vecCurrentPosition - m_interp.pos.vecTarget).Length() > 20)
		{
			// Abort position interpolation
			m_interp.pos.ulFinishTime = 0;
			vecNewPosition = m_interp.pos.vecTarget;

			// Abort target interpolation
			if(HasTargetRotation())
			{
				SetRotation(m_interp.rot.vecTarget);
				m_interp.rot.ulFinishTime = 0;
			}
		}

		// Set our new position
		SetPosition(vecNewPosition, true, false);
	}
}

void CNetworkVehicle::UpdateTargetRotation()
{
	// Do we have a target rotation?
	if(HasTargetRotation())
	{
		// Get our current rotation
		CVector3 vecCurrentRotation;
		GetRotation(vecCurrentRotation);

		// Get the factor of time spent from the interpolation start
		// to the current time.
		unsigned long ulCurrentTime = SharedUtility::GetTime();
		float fAlpha = Math::Unlerp(m_interp.rot.ulStartTime, ulCurrentTime, m_interp.rot.ulFinishTime);

		// Don't let it to overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_interp.rot.fLastAlpha);
		m_interp.rot.fLastAlpha = fAlpha;

		// Compensate the error
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, m_interp.rot.vecError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.0f)
			m_interp.rot.ulFinishTime = 0;

		// Set the new rotation
		SetRotation((vecCurrentRotation + vecCompensation), false);
	}
}

void CNetworkVehicle::Interpolate()
{
	// Do we have a driver?
	if(GetDriver())
	{
		// Update our target position
		UpdateTargetPosition();

		// Update our target rotation
		UpdateTargetRotation();
	}
	else
	{
		// Remove our target position
		RemoveTargetPosition();

		// Remove our target rotation
		RemoveTargetRotation();
	}
}

void CNetworkVehicle::SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Update our target position
		UpdateTargetPosition();

		// Get the current time
		unsigned long ulTime = SharedUtility::GetTime();

		// Get our local position
		CVector3 vecLocalPosition;
		GetPosition(vecLocalPosition);

		// Set the target position
		m_interp.pos.vecTarget = vecPosition;

		// Calculate the relative error
		m_interp.pos.vecError = (vecPosition - vecLocalPosition);

		// Apply the error over 400ms (i.e. 1/4 per 100ms)
		m_interp.pos.vecError *= Math::Lerp<const float>(0.25f, Math::UnlerpClamped(100, ulDelay, 400), 1.0f);

		// Get the interpolation interval
		m_interp.pos.ulStartTime = ulTime;
		m_interp.pos.ulFinishTime = (ulTime + ulDelay);

		// Initialize the interpolation
		m_interp.pos.fLastAlpha = 0.0f;
	}

	// Set our position straight
	m_vecPosition = vecPosition;
}

void CNetworkVehicle::SetTargetRotation(const CVector3& vecRotation, unsigned long ulDelay)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Update our target rotation
		UpdateTargetRotation();

		// Get the current time
		unsigned long ulTime = SharedUtility::GetTime();

		// Get our local rotation
		CVector3 vecLocalRotation;
		GetRotation(vecLocalRotation);

		// Set the target rotation
		m_interp.rot.vecTarget = vecRotation;

		// Get the error
		m_interp.rot.vecError = Math::GetOffsetDegrees(vecLocalRotation, vecRotation);

		// Get the interpolation interval
		m_interp.rot.ulStartTime = ulTime;
		m_interp.rot.ulFinishTime = (ulTime + ulDelay);

		// Initialize the interpolation
		m_interp.rot.fLastAlpha = 0.0f;
	}

	// Set our rotation straight
	m_vecRotation = vecRotation;
}

void CNetworkVehicle::RemoveTargetPosition()
{
	m_interp.pos.ulFinishTime = 0;
}

void CNetworkVehicle::RemoveTargetRotation()
{
	m_interp.rot.ulFinishTime = 0;
}

void CNetworkVehicle::ResetInterpolation()
{
	if(HasTargetPosition())
		SetPosition(m_interp.pos.vecTarget, true);

	if(HasTargetRotation())
		SetRotation(m_interp.rot.vecTarget);

	RemoveTargetPosition();
	RemoveTargetRotation();
}

void CNetworkVehicle::SetInterior(unsigned int uiInterior)
{
	// TODO: Fix this (disables physics for cars when you enter & leave an interior - you can't drive nor close the door/push them) 
	/*if(IsSpawned() && uiInterior != GetInterior() && g_pLocalPlayer->GetVehicle()->GetVehicleId() != m_vehicleId)
		Scripting::SetRoomForCarByKey(GetScriptingHandle(), (Scripting::eInteriorRoomKey)uiInterior);*/
}

unsigned int CNetworkVehicle::GetInterior()
{
	// Are we spawned?
	if(IsSpawned())
	{
		unsigned int uiInterior;
		Scripting::GetKeyForCarInRoom(GetScriptingHandle(), (Scripting::eInteriorRoomKey *)&uiInterior);
		return uiInterior;
	}

	return 0;
}

void CNetworkVehicle::SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight)
{
	m_bIndicatorState[0] = bFrontLeft;
	m_bIndicatorState[1] = bFrontRight;
	m_bIndicatorState[2] = bBackLeft;
	m_bIndicatorState[3] = bBackRight;
}

bool CNetworkVehicle::GetIndicatorState(unsigned char ucSlot)
{
	if(ucSlot >= 0 && ucSlot <= 3)
	{
		return m_bIndicatorState[ucSlot];
	}

	return false;
}

void CNetworkVehicle::SetComponentState(unsigned char ucSlot, bool bOn)
{
	if(ucSlot >= 0 && ucSlot <= 8)
	{
		m_bComponents[ucSlot] = bOn;

		// Are we spawned?
		if(IsSpawned())
			m_pVehicle->SetComponentState((ucSlot + 1), bOn);
	}
}

bool CNetworkVehicle::GetComponentState(unsigned char ucSlot)
{
	if(ucSlot >= 0 && ucSlot <= 8)
		return m_bComponents[ucSlot];

	return false;
}

void CNetworkVehicle::SetVariation(unsigned char ucVariation)
{
	// Are we spawned?
	if(IsSpawned())
	{
		if(m_pVehicle->GetTextureVariationCount() != 255)
		{
			//m_pVehicle->SetTextureVariation(ucVariation);
			Scripting::SetCarLivery(GetScriptingHandle(), (int)ucVariation);
			m_ucVariation = ucVariation;
		}
		else
			m_ucVariation = 0;
	}
	else
		m_ucVariation = ucVariation;
}

unsigned char CNetworkVehicle::GetVariation()
{
	// Are we spawned?
	if(IsSpawned())
	{
		if(m_pVehicle->GetTextureVariationCount() != 255)
		{
			//return (unsigned char)m_pVehicle->GetTextureVariation();
			int iVariation = 0;
			Scripting::GetCarLivery(GetScriptingHandle(), &iVariation);
			return iVariation;
		}
		else
			return 0;
	}
	else
		return m_ucVariation;
}

bool CNetworkVehicle::IsOnScreen()
{
	// Are we spawned?
	if(IsSpawned())
		return /*Scripting::IsCarOnScreen(GetScriptingHandle())*/ true;

	return false;
}

void CNetworkVehicle::SetTaxiLightsState(bool bState)
{
	// Are we spawned?
	if(IsSpawned())
		Scripting::SetTaxiLights(GetScriptingHandle(),bState);

	m_bTaxiLights = bState;
}

bool CNetworkVehicle::GetTaxiLightsState()
{
	// Are we spawned?
	if(IsSpawned())
		return m_bTaxiLights;

	return false;
}

void CNetworkVehicle::ControlCar(int idoor,bool open, float fAngle)
{
	// Are we spawned?
	if(IsSpawned())
	{
		if(fAngle > 1.9f && fAngle < 350.0f)
			Scripting::ControlCarDoor(GetScriptingHandle(),(Scripting::eVehicleDoor)idoor,0,fAngle);
		else if(fAngle >= 350.0f)
			Scripting::OpenCarDoor(GetScriptingHandle(),(Scripting::eVehicleDoor)idoor);
		else if(fAngle < 2.0f)
			Scripting::ShutCarDoor(GetScriptingHandle(),(Scripting::eVehicleDoor)idoor);

		//g_pChatWindow->AddInfoMessage("%d, %f",idoor,fAngle);
	}
	
	// Apply changes
	m_fDoor[idoor] = fAngle;
}

float CNetworkVehicle::GetCarDoor(int iDoor)
{
	// Are we spawned?
	if(IsSpawned())
		return m_fDoor[iDoor];

	return false;
}

void CNetworkVehicle::SetLightsState(bool bLights)
{
	// Are we spawned?
	if(IsSpawned())
	{
		if(bLights)
			Scripting::ForceCarLights(GetScriptingHandle(),2);
		else if(!bLights)
			Scripting::ForceCarLights(GetScriptingHandle(),1);

		m_bLights = bLights;
	}
}

bool CNetworkVehicle::GetLightsState()
{
	// Are we spawned?
	if(IsSpawned())
		return m_bLights;

	return false;
}

bool CNetworkVehicle::GetWindowBrokenState(int iWindow)
{
	// Are we spawned?
	if(IsSpawned())
		return m_bWindow[iWindow];

	return false;
}

void CNetworkVehicle::SetWindowBroken(int iWindow, bool bBroken)
{
	// Are we spawned?
	if(IsSpawned())
	{
		if(iWindow >= 0 && iWindow <= 4)
		{
			if(bBroken)
				Scripting::RemoveVehicleWindow(GetScriptingHandle(),(Scripting::eVehicleWindow)iWindow);
			else if(!bBroken)
				//TODO

			m_bWindow[iWindow] = bBroken;
		}
	}
}

void CNetworkVehicle::SetDamageAble(bool bToggle)
{
	// Are we spawned?
	if(IsSpawned())
	{
		Scripting::SetCarCanBeDamaged(GetScriptingHandle(),bToggle);
		Scripting::SetCarCanBurstTyres(GetScriptingHandle(),bToggle);
	}
}

void CNetworkVehicle::SetSteeringAngle(float fSteeringAngle)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetSteeringAngle(fSteeringAngle);
}

float CNetworkVehicle::GetSteeringAngle()
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->GetSteeringAngle();

	return 0.0f;
}

void CNetworkVehicle::SetEngineState(bool bState)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetEngineStatus(bState ? 1:0, bState ? 1:0);

	m_bEngineStatus = bState;
}

bool CNetworkVehicle::GetEngineState()
{
	// Are we spawned?
	if(IsSpawned())
		return m_bEngineStatus;//m_pVehicle->GetEngineStatus();

	return false;
}
