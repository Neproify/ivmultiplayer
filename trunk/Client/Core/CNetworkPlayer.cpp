//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkPlayer.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNetworkManager.h"
#include "CNetworkPlayer.h"
#include "CVehicleManager.h"
#include <Patcher/CPatcher.h>
#include "KeySync.h"
#include "CPlayerManager.h"
#include "CLocalPlayer.h"
#include <SharedUtility.h>
#include "COffsets.h"
#include "CIVTask.h"
#include "CPools.h"
#include "IVTasks.h"
#include "CCamera.h"
#include "CChatWindow.h"

extern CNetworkManager * g_pNetworkManager;
extern CChatWindow     * g_pChatWindow;
extern CVehicleManager * g_pVehicleManager;
extern CPlayerManager  * g_pPlayerManager;
extern CLocalPlayer    * g_pLocalPlayer;
extern CCamera         * g_pCamera;
extern CStreamer       * g_pStreamer;
extern bool              m_bControlsDisabled;

CNetworkPlayer::CNetworkPlayer(bool bIsLocalPlayer)
{
	m_bIsLocalPlayer = bIsLocalPlayer;
	m_playerId = INVALID_ENTITY_ID;
	m_pContextData = NULL;
	m_byteGamePlayerNumber = 0;
	m_pPlayerInfo = NULL;
	m_pModelInfo = CGame::GetModelInfo(MODEL_PLAYER_INDEX);
	m_bSpawned = false;
	m_uiColor = 0xFFFFFFFF;
	m_interp.pos.ulFinishTime = 0;
	memset(&m_ucClothes, 0, sizeof(m_ucClothes));
	m_bUseCustomClothesOnSpawn = false;
	memset(&m_previousPadState, 0, sizeof(CPadState));
	memset(&m_currentPadState, 0, sizeof(CPadState));
	m_usPing = 0;
	m_pVehicle = NULL;
	m_byteVehicleSeatId = 0;
	ResetVehicleEnterExit();
	m_bHealthLocked = false;
	m_bArmourLocked = false;

	if(IsLocalPlayer())
	{
		// Create a new player ped instance with the local player ped
		m_pPlayerPed = new CIVPlayerPed(CGame::GetPools()->GetPlayerInfoFromIndex(0)->m_pPlayerPed);

		// Get the local player info pointer
		m_pPlayerInfo = new CIVPlayerInfo(CGame::GetPools()->GetPlayerInfoFromIndex(0));

		// Add our model info reference
		m_pModelInfo->AddReference(false);

		// Flag ourselves as spawned
		m_bSpawned = true;
	}
	else
	{
		// Invalidate the player number
		m_byteGamePlayerNumber = INVALID_PLAYER_PED;

		// Set the player ped instance to NULL
		m_pPlayerPed = NULL;

		// Set the player info instance to NULL
		m_pPlayerInfo = NULL;
	}
}

CNetworkPlayer::~CNetworkPlayer()
{
	// Are we not the local player?
	if(!IsLocalPlayer())
	{
		// Destroy the player ped
		Destroy();
	}
}

bool CNetworkPlayer::Create()
{
	// Are we already spawned or are we the local player?
	if(IsSpawned() || IsLocalPlayer())
		return false;

	// Find a free player number
	m_byteGamePlayerNumber = (BYTE)CGame::GetPools()->FindFreePlayerInfoIndex();

	// Invalid player number?
	if(m_byteGamePlayerNumber == INVALID_PLAYER_PED)
		return false;

	// Add our model info reference
	m_pModelInfo->AddReference(true);

	// Get our model index
	int iModelIndex = m_pModelInfo->GetIndex();

	// Begin new creation code
	// Create player info instance
	m_pPlayerInfo = new CIVPlayerInfo(m_byteGamePlayerNumber);

	// Create a context data instance for this player
	m_pContextData = CContextDataManager::CreateContextData(m_pPlayerInfo);

	CLogFile::Printf("Create 1");

	// Allocate the player ped
	IVPlayerPed * pPlayerPed = (IVPlayerPed *)CGame::GetPools()->GetPedPool()->Allocate();

	CLogFile::Printf("Create 2");

	// Ensure the player ped pointer is valid
	if(!pPlayerPed)
	{
		CLogFile::Printf("Create 2 FAIL");
		Destroy();
		return false;
	}

	CLogFile::Printf("Create 3");

	// Call the CPlayerPed constructor
#define FUNC_CPlayerPed__Constructor 0x9C1910
	DWORD dwFunc = (CGame::GetBase() + FUNC_CPlayerPed__Constructor);
	unsigned int uiPlayerIndex = (unsigned int)m_byteGamePlayerNumber;
	WORD wPlayerData = MAKEWORD(0, 1);
	WORD * pwPlayerData = &wPlayerData;
	_asm
	{
		push uiPlayerIndex
		push iModelIndex
		push pwPlayerData
		mov ecx, pPlayerPed
		call dwFunc
	}

	CLogFile::Printf("Create 4");

	// Setup the player ped
	// jenksta: crash here
	// perhaps some sort of memory leak?
	// maybe a pool limit is passed?
	// maybe theres a function to destroy what this creates when the player ped is destroyed and im not calling it?
	// crash is in some func called from CPlayerPed::SetModelIndex which allocates something to do with ped props
#define VAR_PedFactory 0x15E35A0
#define FUNC_SetupPed 0x43A6A0
	DWORD dwPedFactory = (CGame::GetBase() + VAR_PedFactory);
	Matrix34 * pMatrix = NULL;
	dwFunc = (CGame::GetBase() + FUNC_SetupPed);
	_asm
	{
		push iModelIndex
		push dwPedFactory
		mov edi, pMatrix
		mov esi, pPlayerPed
		call dwFunc
	}

	CLogFile::Printf("Create 5");

	*(DWORD *)(pPlayerPed + 0x260) |= 1u;

	CLogFile::Printf("Create 6");

	// Setup the player ped intelligence
#define FUNC_SetupPedIntelligence 0x89EC20
	dwFunc = (CGame::GetBase() + FUNC_SetupPedIntelligence);
	_asm
	{
		push 2
		mov ecx, pPlayerPed
		call dwFunc
	}

	CLogFile::Printf("Create 7");

	//*(DWORD *)(pPlayerInfo + 0x4DC) = 2;

	CLogFile::Printf("Create 8");

	// Set our player info ped pointer
	m_pPlayerInfo->SetPlayerPed(pPlayerPed);

	CLogFile::Printf("Create 9");

	// Set our player peds player info pointer
	pPlayerPed->m_pPlayerInfo = m_pPlayerInfo->GetPlayerInfo();

	CLogFile::Printf("Create 10");

	// Set game player info pointer
	CGame::GetPools()->SetPlayerInfoAtIndex((unsigned int)m_byteGamePlayerNumber, m_pPlayerInfo->GetPlayerInfo());

	CLogFile::Printf("Create 11");

	// Create player ped instance
	m_pPlayerPed = new CIVPlayerPed(pPlayerPed);

	CLogFile::Printf("Create 12");

	// Set the context data player ped pointer
	m_pContextData->SetPlayerPed(m_pPlayerPed);

	CLogFile::Printf("Create 13");

	// Add to world
	m_pPlayerPed->AddToWorld();
	CLogFile::Printf("Create 14");
	// End new creation code

#if 0
	// Save local player id
	unsigned int uiLocalPlayerId = GetLocalPlayerId();

	// Create player ped
	DWORD dwFunc = COffsets::FUNC_CreatePlayerPed;
	unsigned int uiPlayerId = m_byteGamePlayerNumber;
	WORD wPlayerData = MAKEWORD(0, 1);
	WORD * pwPlayerData = &wPlayerData;
	IVPlayerPed * pPlayerPed = NULL;
	_asm
	{
		push 0 ; unknown
		push iModelIndex ; model index
		push uiPlayerId
		push pwPlayerData
		call dwFunc
		mov pPlayerPed, eax
		add esp, 10h
	}

	// Restore local player id
	SetLocalPlayerId(uiLocalPlayerId);

	// Invalid player ped?
	if(!pPlayerPed)
		return false;

	// Setup ped intelligence
	dwFunc = COffsets::FUNC_SetupPedIntelligence;
	_asm
	{
		push 2 ; unknown
		mov ecx, pPlayerPed
		call dwFunc
	}

	// Get the player ped pointer
	m_pPlayerPed = new CIVPlayerPed(pPlayerPed);

	// Add the player ped to the world
	CGame::AddEntityToWorld(m_pPlayerPed->GetEntity());

	// Get player info pointer
	m_pPlayerInfo = new CIVPlayerInfo(CGame::GetPools()->GetPlayerInfoFromIndex(m_byteGamePlayerNumber));

	// Set player info slot to our new player info
	CGame::GetPools()->SetPlayerInfoAtIndex(m_byteGamePlayerNumber, m_pPlayerInfo->GetPlayerInfo());
#endif

	// Flag as spawned
	m_bSpawned = true;

	// Set health
	SetHealth(200);

	// Set the interior
	SetInterior(g_pLocalPlayer->GetInterior());

	// Remember that we might have clothes
	m_bUseCustomClothesOnSpawn = true;

	// Reset interpolation
	ResetInterpolation();

	CLogFile::Printf("Done: PlayerNumber: %d, ScriptingHandle: %d", m_byteGamePlayerNumber, GetScriptingHandle());
	return true;
}

void CNetworkPlayer::Init()
{
}

void CNetworkPlayer::Destroy()
{
	// Are we the local player?
	if(IsLocalPlayer())
		return;

	// Are we spawned and not the local player?
	if(IsSpawned())
	{
		// Remove from world
		/*CGame::RemoveEntityFromWorld(m_pPlayerPed->GetEntity());

		// Call destructor
		DWORD dwFunc = m_pPlayerPed->GetEntity()->m_VFTable->ScalarDeletingDestructor;
		IVPed * pPlayerPed = m_pPlayerPed->GetPed();
		_asm
		{
			push 1
			mov ecx, pPlayerPed
			call dwFunc
		}

		// Remove our model info reference
		m_pModelInfo->RemoveReference();

		// Delete our player ped instance
		SAFE_DELETE(m_pPlayerPed);

		// Delete our player info instance
		SAFE_DELETE(m_pPlayerInfo);

		// Do we have a valid player number?
		if(m_byteGamePlayerNumber != INVALID_PLAYER_PED)
		{
			// Reset game player info pointer
			CGame::GetPools()->SetPlayerInfoAtIndex((unsigned int)m_byteGamePlayerNumber, NULL);

			// Invalidate the player number
			m_byteGamePlayerNumber = INVALID_PLAYER_PED;
		}*/
		CLogFile::Printf("Destroy 1");
		// Get the player ped pointer
		IVPlayerPed * pPlayerPed = m_pPlayerPed->GetPlayerPed();
		CLogFile::Printf("Destroy 2");

		IVPedIntelligence * pPedIntelligence = pPlayerPed->m_pPedIntelligence;
#define FUNC_ShutdownPedIntelligence 0x9C4DF0
		DWORD dwFunc = (CGame::GetBase() + FUNC_ShutdownPedIntelligence);
		_asm
		{
			push 0
			mov ecx, pPedIntelligence
			call dwFunc
		}
		CLogFile::Printf("Destroy 3");

		*(DWORD *)(pPlayerPed + 0x260) &= 0xFFFFFFFE;
		CLogFile::Printf("Destroy 4");

		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();
		CLogFile::Printf("Destroy 5");

		// Delete the player ped
		// We use the CPed destructor and not the CPlayerPed destructor because the CPlayerPed destructor
		// messes with our player info (which we handle manually)
		//dwFunc = m_pPlayerPed->GetPlayerPed()->m_VFTable->ScalarDeletingDestructor;
#define FUNC_CPed__ScalarDeletingDestructor 0x8ACAC0
		dwFunc = (CGame::GetBase() + FUNC_CPed__ScalarDeletingDestructor);
		_asm
		{
			push 1
			mov ecx, pPlayerPed
			call dwFunc
		}
		CLogFile::Printf("Destroy 6");

		// Remove our model info reference
		m_pModelInfo->RemoveReference();
		CLogFile::Printf("Destroy 7");
	}

	CLogFile::Printf("Destroy 8");

	// Do we have a context data instance
	if(m_pContextData)
	{
		CLogFile::Printf("Destroy 9");
		// Delete the context data instance
		CContextDataManager::DestroyContextData(m_pContextData);
		CLogFile::Printf("Destroy 10");

		// Set the context data pointer to NULL
		m_pContextData = NULL;
		CLogFile::Printf("Destroy 11");
	}
	CLogFile::Printf("Destroy 12");

	// Delete the player ped instance
	SAFE_DELETE(m_pPlayerPed);
	CLogFile::Printf("Destroy 13");

	// Delete our player info instance
	SAFE_DELETE(m_pPlayerInfo);
	CLogFile::Printf("Destroy 14");

	// Do we have a valid player number?
	if(m_byteGamePlayerNumber != INVALID_PLAYER_PED)
	{
		CLogFile::Printf("Destroy 15");
		// Reset game player info pointer
		CGame::GetPools()->SetPlayerInfoAtIndex((unsigned int)m_byteGamePlayerNumber, NULL);
		CLogFile::Printf("Destroy 16");

		// Invalidate the player number
		m_byteGamePlayerNumber = INVALID_PLAYER_PED;
		CLogFile::Printf("Destroy 17");
	}
	CLogFile::Printf("Destroy 18");

	// Flag ourselves as despawned
	m_bSpawned = false;
	CLogFile::Printf("Destroy 19");
}

void CNetworkPlayer::Kill(bool bInstantly)
{
	// Are we spawned and not already dead?
	if(IsSpawned() && !IsDead())
	{
		// Are we getting killed instantly?
		if(bInstantly)
		{
			// Create the dead task
			// if this doesn't work vary last 2 params (1, 0 : 0, 1 : 1, 1 : 0, 0)
			CIVTaskSimpleDead * pTask = new CIVTaskSimpleDead(CGame::GetTime(), 1, 0);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_DEFAULT);
			}
		}
		else // We are not getting killed instantly
		{
			// Are we already dying?
			if(IsDying())
				return;

			// Create the death task
			// guess from sa (thx mta)
			// wep type, body part, anim group, anim id, unknown?
			CIVTaskComplexDie * pTask = new CIVTaskComplexDie(0, 0, 44, 190, 4.0f, 0.0f, 1);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);
			}
		}

		// Set the health and armour to 0
		SetHealth(0);
		SetArmour(0);

		// Reset the pad state
		CPadState padState;
		SetPadState(&padState);

		// Reset vehicle entry/exit flags
		ResetVehicleEnterExit();

		// Reset interpolation
		ResetInterpolation();
	}
}

bool CNetworkPlayer::IsDying()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_DIE)
				return true;
		}
	}

	return false;
}

bool CNetworkPlayer::IsDead()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_DEAD)
				return true;
		}
	}

	return false;
}

IVEntity * CNetworkPlayer::GetLastDamageEntity()
{
	if(IsSpawned())
		return m_pPlayerPed->GetLastDamageEntity();

	return NULL;
}

// TODO: Add weapon
bool CNetworkPlayer::GetKillInfo(EntityId * playerId, EntityId * vehicleId)
{
	// TODO: Include kill weapon
	// Are we spawned?
	if(IsSpawned())
	{
		// Reset player id and vehicle id
		*playerId = INVALID_ENTITY_ID;
		*vehicleId = INVALID_ENTITY_ID;

		// Loop through all players
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			// Is this player connected?
			if(g_pPlayerManager->DoesExist(i))
			{
				// Get this players CNetworkPlayer pointer
				CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(i);

				// Is the CNetworkPlayer pointer valid and is this player spawned?
				if(pPlayer && pPlayer->IsSpawned())
				{
					// Is this player the last damage entity?
					if(GetLastDamageEntity() == (IVEntity *)pPlayer->GetGamePlayerPed()->GetPed())
					{
						// This player killed us
						*playerId = i;
						break;
					}
					else
					{
						// Is this players vehicle the last damage entity?
						if(pPlayer->IsInVehicle() && !pPlayer->IsAPassenger() && 
							(GetLastDamageEntity() == (IVEntity *)pPlayer->GetVehicle()))
						{
							// This player killed us with their vehicle
							*playerId = i;
							*vehicleId = i;
							break;
						}
					}
				}
			}
		}

		// Have we not yet found a killer?
		if(*playerId == INVALID_ENTITY_ID && *vehicleId == INVALID_ENTITY_ID)
		{
			// Loop through all streamed in vehicles
			std::list<CStreamableEntity *> * streamedVehicles = g_pStreamer->GetStreamedInEntitiesOfType(STREAM_ENTITY_VEHICLE);

			for(std::list<CStreamableEntity *>::iterator iter = streamedVehicles->begin(); iter != streamedVehicles->end(); ++iter)
			{
				CNetworkVehicle * pVehicle = reinterpret_cast<CNetworkVehicle *>(*iter);

				// Is this vehicle the last damage entity?
				if(GetLastDamageEntity() == pVehicle->GetGameVehicle()->GetEntity())
				{
					// This vehicle killed us
					*vehicleId = pVehicle->GetVehicleId();
					break;
				}
			}
		}

		return true;
	}

	return false;
}

bool CNetworkPlayer::IsMoving()
{
	if(IsSpawned())
	{
		CVector3 vecMoveSpeed;
		GetMoveSpeed(vecMoveSpeed);

		// TODO: This should use code reversed from the IS_CHAR_STOPPED native?
		if(!(vecMoveSpeed.fX == 0 && vecMoveSpeed.fY == 0 && (vecMoveSpeed.fZ >= -0.000020 && vecMoveSpeed.fZ <= 0.000020)))
			return true;
	}

	return false;
}

void CNetworkPlayer::StopMoving()
{
	if(IsSpawned())
		SetMoveSpeed(CVector3());
}

bool CNetworkPlayer::InternalIsInVehicle()
{
	// Are we spawned?
	if(IsSpawned())
		return (m_pPlayerPed->IsInVehicle() && m_pPlayerPed->GetCurrentVehicle());

	return false;
}

CNetworkVehicle * CNetworkPlayer::InternalGetVehicle()
{
	// Are we spawned and in a vehicle?
	if(IsSpawned() && InternalIsInVehicle())
		return g_pStreamer->GetVehicleFromGameVehicle(m_pPlayerPed->GetCurrentVehicle());

	return NULL;
}

void CNetworkPlayer::InternalPutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	// Are we spawned and not in a vehicle?
	if(IsSpawned() && !InternalIsInVehicle())
	{
		// Is the seat the driver seat?
		if(byteSeatId == 0)
			Scripting::WarpCharIntoCar(GetScriptingHandle(), pVehicle->GetScriptingHandle());
		else
		{
			// Is the passenger seat valid?
			if(byteSeatId <= pVehicle->GetMaxPassengers())
				Scripting::WarpCharIntoCarAsPassenger(GetScriptingHandle(), pVehicle->GetScriptingHandle(), (byteSeatId - 1));
		}
	}
}

void CNetworkPlayer::InternalRemoveFromVehicle()
{
	// Are we spawned and in a vehicle?
	if(IsSpawned() && m_pVehicle)
	{
		// Create the car set ped out task
		CIVTaskSimpleCarSetPedOut * pTask = new CIVTaskSimpleCarSetPedOut(m_pVehicle->GetGameVehicle(), 0xF, 0, 1);

		// Apply the task to the player ped
		IVPlayerPed * pPlayerPed = m_pPlayerPed->GetPlayerPed();
		IVTask * pGameTask = pTask->GetTask();
		DWORD dwFunc = (CGame::GetBase() + 0xA9ED10);
		_asm
		{
			push pPlayerPed
			mov ecx, pGameTask
			call dwFunc
		}

		// Destroy the task
		pTask->Destroy();
	}
}

unsigned int CNetworkPlayer::GetScriptingHandle()
{
	if(IsSpawned())
		return CGame::GetPools()->GetPedPool()->HandleOf(m_pPlayerPed->GetPed());

	return 0;
}

void CNetworkPlayer::SetModel(DWORD dwModelHash)
{
	// Get the model index from the model hash
	int iModelIndex = CGame::GetStreaming()->GetModelIndexFromHash(dwModelHash);

	// Do we have an invalid model index?
	if(iModelIndex == -1)
		return;

	// Has the model not changed?
	if(m_pModelInfo->GetIndex() == iModelIndex)
		return;

	// Get the new model info
	CIVModelInfo * pNewModelInfo = CGame::GetModelInfo(iModelIndex);

	// Is the new model info valid?
	if(!pNewModelInfo || !pNewModelInfo->IsValid() || !pNewModelInfo->IsPed())
	{
		CLogFile::Printf("CClientPlayer::SetModel Failed (Invalid model)!");
		return;
	}

	// Remove our model info reference from the old model info
	m_pModelInfo->RemoveReference();

	// Set the new model info
	m_pModelInfo = pNewModelInfo;

	// Are we spawned?
	if(IsSpawned())
	{
		// Add our model info reference
		m_pModelInfo->AddReference(true);

		// Begin hacky code that needs to be changed
		// TODO: Don't use a native for this (Create a CIVEntity::SetModelIndex and use it instead)
		{
			// TODO: Use StreamIn/Out for this for remote players, and for local players use this and restore all info like in StreamIn/Out
			// or perhaps make StreamIn/Out only get/set info and not create/destroy player if its the local player that way we can use it
			// for local and remote players
			unsigned int uiInterior = GetInterior();
			Scripting::ChangePlayerModel(m_byteGamePlayerNumber, (Scripting::eModel)dwModelHash);
			m_pPlayerPed->SetPed(m_pPlayerInfo->GetPlayerPed());
			SetInterior(uiInterior);
		}
		// End hacky code that needs to be changed

		// Do we not have any custom clothes?
		if(!m_bUseCustomClothesOnSpawn)
		{
			// Set the default clothes variation
			Scripting::SetCharDefaultComponentVariation(GetScriptingHandle());

			// Reset our clothes
			memset(&m_ucClothes, 0, sizeof(m_ucClothes));
		}
		else // We have custom clothes
		{
			// Set our clothes
			for(unsigned char uc = 0; uc < 11; ++uc)
				SetClothes(uc, m_ucClothes[uc]);

			// Flag ourselves as not having custom clothes
			// jenksta: why does this reset here, surely if we have custom clothes
			// we only want to reset them if the scripter requests it or if we change
			// our model?
			m_bUseCustomClothesOnSpawn = false;
		}
	}
}

void CNetworkPlayer::Teleport(const CVector3& vecCoordinates, bool bResetInterpolation)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we not in a vehicle?
		if(!IsInVehicle())
		{
			// FIXUPDATE
			// Reverse code from below native and use it here
			Scripting::SetCharCoordinatesNoOffset(GetScriptingHandle(), vecCoordinates.fX, vecCoordinates.fY, vecCoordinates.fZ);

			/*
			// This still causes players to be invisible occasionally
			
			// Remove the ped from the world
			m_pPlayerPed->RemoveFromWorld();

			// Set the position in the matrix
			m_pPlayerPed->Teleport(vecCoordinates);

			// Re add the ped to the world to apply the matrix change
			m_pPlayerPed->AddToWorld();
			*/
		}
		else
			Scripting::WarpCharFromCarToCoord(GetScriptingHandle(), vecCoordinates.fX, vecCoordinates.fY, vecCoordinates.fZ);
	}

	// Reset interpolation if requested
	if(bResetInterpolation)
		RemoveTargetPosition();
}

void CNetworkPlayer::SetPosition(const CVector3& vecCoordinates, bool bResetInterpolation)
{
	// FIXUPDATE
	// This doesn't work for long distances

	// Are we spawned?
	if(IsSpawned())
	{
		// Are we not in a vehicle and not entering a vehicle?
		if(!InternalIsInVehicle() && !HasVehicleEnterExit())
		{
			// Remove the player ped from the world
			m_pPlayerPed->RemoveFromWorld();

			//Set the position in the matrix
			m_pPlayerPed->SetPosition(vecCoordinates);

			// Are we not the local player?
			if(!IsLocalPlayer())
			{
				// Get the local players interior
				unsigned int uiLocalPlayerInterior = g_pLocalPlayer->GetInterior();

				// If our interior is not the same as the local players interior force it
				// to the same as the local players
				if(GetInterior() != uiLocalPlayerInterior)
					SetInterior(uiLocalPlayerInterior);
			}

			// Re add the ped to the world to apply the matrix change
			m_pPlayerPed->AddToWorld();
		}
	}

	// Reset interpolation if requested
	if(bResetInterpolation)
		RemoveTargetPosition();
}

void CNetworkPlayer::GetPosition(CVector3& vecCoordinates)
{
	if(IsSpawned())
	{
		// If we are in a vehicle use our vehicles position
		if(m_pVehicle)
			m_pVehicle->GetPosition(vecCoordinates);
		else
			m_pPlayerPed->GetPosition(vecCoordinates);
	}
	else
		vecCoordinates = CVector3();
}

void CNetworkPlayer::SetCurrentHeading(float fHeading)
{
	if(IsSpawned())
	{
		m_pPlayerPed->SetCurrentHeading(fHeading);
		SetDesiredHeading(fHeading);
	}
}

float CNetworkPlayer::GetCurrentHeading()
{
	if(IsSpawned())
		return m_pPlayerPed->GetCurrentHeading();

	return 0.0f;
}

void CNetworkPlayer::SetDesiredHeading(float fHeading)
{
	if(IsSpawned())
		m_pPlayerPed->SetDesiredHeading(fHeading);
}

float CNetworkPlayer::GetDesiredHeading()
{
	if(IsSpawned())
		return m_pPlayerPed->GetDesiredHeading();

	return 0.0f;
}

void CNetworkPlayer::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->SetMoveSpeed(vecMoveSpeed);
}

void CNetworkPlayer::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->GetMoveSpeed(vecMoveSpeed);
	else
		vecMoveSpeed = CVector3();
}

void CNetworkPlayer::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->SetTurnSpeed(vecTurnSpeed);
}

void CNetworkPlayer::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->GetTurnSpeed(vecTurnSpeed);
	else
		vecTurnSpeed = CVector3();
}

void CNetworkPlayer::SetHealth(unsigned int uiHealth)
{
	// Are we spawned?
	if(IsSpawned())
		Scripting::SetCharHealth(GetScriptingHandle(), uiHealth);

	// Unlock our health
	m_bHealthLocked = false;
}

void CNetworkPlayer::LockHealth(unsigned int uiHealth)
{
	// Set our health
	SetHealth(uiHealth);

	// Set our locked health
	m_uiLockedHealth = uiHealth;

	// Flag our health as locked
	m_bHealthLocked = true;
}

unsigned int CNetworkPlayer::GetHealth()
{
	// If our health is locked return our locked health
	if(m_bHealthLocked)
		return m_uiLockedHealth;

	// Are we spawned?
	if(IsSpawned())
	{
		unsigned int uiHealth;
		Scripting::GetCharHealth(GetScriptingHandle(), &uiHealth);
		return uiHealth;
	}

	// Not spawned
	return 0;
}

void CNetworkPlayer::SetArmour(unsigned int uiArmour)
{
	// Are we spawned?
	if(IsSpawned())
		Scripting::AddArmourToChar(GetScriptingHandle(), (uiArmour - GetArmour()));

	// Unlock our armour
	m_bArmourLocked = false;
}

void CNetworkPlayer::LockArmour(unsigned int uiArmour)
{
	// Set our armour
	SetArmour(uiArmour);

	// Set our locked armour
	m_uiLockedArmour = uiArmour;

	// Flag our armour as locked
	m_bArmourLocked = true;
}

unsigned int CNetworkPlayer::GetArmour()
{
	// If our armour is locked return our locked armour
	if(m_bArmourLocked)
		return m_uiLockedArmour;

	// Are we spawned?
	if(IsSpawned())
	{
		unsigned int uiArmour;
		Scripting::GetCharArmour(GetScriptingHandle(), &uiArmour);
		return uiArmour;
	}

	// Not spawned
	return 0;
}

void CNetworkPlayer::GiveWeapon(unsigned int uiWeaponId, unsigned int uiAmmo)
{
	if(IsSpawned())
		Scripting::GiveWeaponToChar(GetScriptingHandle(), (Scripting::eWeapon)uiWeaponId, uiAmmo, true);
}

void CNetworkPlayer::RemoveWeapon(unsigned int uiWeaponId)
{
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->RemoveWeapon((eWeaponType)uiWeaponId);
}

void CNetworkPlayer::RemoveAllWeapons()
{
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->RemoveAllWeapons();
}

void CNetworkPlayer::SetCurrentWeapon(unsigned int uiWeaponId)
{
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->SetCurrentWeapon((eWeaponType)uiWeaponId);
}

unsigned int CNetworkPlayer::GetCurrentWeapon()
{
	if(IsSpawned())
	{
		// TODO: Fix, IVPedWeapons::m_byteCurrentWeaponSlot isn't right
		/*CIVWeapon * pWeapon = m_pPlayerPed->GetPedWeapons()->GetCurrentWeapon();

		if(pWeapon)
			return pWeapon->GetType();*/
		unsigned int uiWeaponId;
		Scripting::GetCurrentCharWeapon(GetScriptingHandle(), (Scripting::eWeapon *)&uiWeaponId);
		return uiWeaponId;
	}

	return 0;
}

void CNetworkPlayer::SetAmmo(unsigned int uiWeaponId, unsigned int uiAmmo)
{
	if(IsSpawned())
	{
		// TODO: Fix, IVPedWeapons::m_byteCurrentWeaponSlot isn't right
		/*CIVWeapon * pWeapon = m_pPlayerPed->GetPedWeapons()->GetCurrentWeapon();

		if(pWeapon)
			pWeapon->SetAmmo(uiAmmo);*/
		Scripting::SetCharAmmo(GetScriptingHandle(), (Scripting::eWeapon)uiWeaponId, uiAmmo);
	}
}

unsigned int CNetworkPlayer::GetAmmo(unsigned int uiWeaponId)
{
	if(IsSpawned())
	{
		// TODO: Create a function for SetAmmoInClip
		//SetAmmoInClip()
		// TODO: Fix, IVPedWeapons::m_byteCurrentWeaponSlot isn't right
		/*CIVWeapon * pWeapon = m_pPlayerPed->GetPedWeapons()->GetCurrentWeapon();

		if(pWeapon)
			return pWeapon->GetAmmo();*/
		unsigned int uiAmmo;
		Scripting::GetAmmoInCharWeapon(GetScriptingHandle(), (Scripting::eWeapon)uiWeaponId, &uiAmmo);
		return uiAmmo;
	}

	return 0;
}

void CNetworkPlayer::GiveMoney(int iAmount)
{
	if(IsSpawned())
	{
		// this shows +/-$12345
		Scripting::AddScore(m_byteGamePlayerNumber, iAmount);

		// would take forever
		if(iAmount < -1000000 || iAmount > 1000000)
			m_pPlayerInfo->SetDisplayScore(m_pPlayerInfo->GetScore());
	}
}

void CNetworkPlayer::SetMoney(int iAmount)
{
	if(IsSpawned())
	{
		m_pPlayerInfo->SetScore(iAmount);
		
		// would take forever
		int iDiff = (iAmount - m_pPlayerInfo->GetDisplayScore());

		if(iDiff < -1000000 || iDiff > 1000000)
			m_pPlayerInfo->SetDisplayScore(iAmount);
	}
}

void CNetworkPlayer::ResetMoney()
{
	if(IsSpawned())
	{
		m_pPlayerInfo->SetScore(0);
		m_pPlayerInfo->SetDisplayScore(0);
	}
}

int CNetworkPlayer::GetMoney()
{
	if(IsSpawned())
		return m_pPlayerInfo->GetScore();

	return 0;
}

void CNetworkPlayer::SetPadState(CPadState * padState)
{
	// Copy the current  pad state to the previous pad state
	memcpy(&m_previousPadState, &m_currentPadState, sizeof(CPadState));

	// Copy the pad state to the current pad state
	memcpy(&m_currentPadState, padState, sizeof(CPadState));

	// Are we spawned?
	if(IsSpawned())
	{
		// Get the game pad
		CIVPad * pPad = CGame::GetPad();

		// Are we not the local player?
		if(!IsLocalPlayer())
		{
			// Do we have a valid context data pointer?
			if(m_pContextData)
			{
				// Get the context data pad
				pPad = m_pContextData->GetPad();
			}
		}

		// Get the current pad state
		CPadState currentPadState;
		pPad->GetCurrentClientPadState(currentPadState);

		// Set the last pad state
		pPad->SetLastClientPadState(currentPadState);

		// Set the current pad state
		pPad->SetCurrentClientPadState(*padState);
	}
}

void CNetworkPlayer::GetPreviousPadState(CPadState * padState)
{
	// Copy the previous pad state to the pad state
	memcpy(padState, &m_previousPadState, sizeof(CPadState));
}

void CNetworkPlayer::GetPadState(CPadState * padState)
{
	// Copy the current pad state to the pad state
	memcpy(padState, &m_currentPadState, sizeof(CPadState));
}

void CNetworkPlayer::SetAimSyncData(AimSyncData * aimSyncData)
{
	Matrix matAim;
	memcpy(&matAim.vecRight, &aimSyncData->vecRight, sizeof(CVector3));
	memcpy(&matAim.vecForward, &aimSyncData->vecForward, sizeof(CVector3));
	memcpy(&matAim.vecUp, &aimSyncData->vecUp, sizeof(CVector3));
	memcpy(&matAim.vecPosition, &aimSyncData->vecPosition, sizeof(CVector3));

	if(IsLocalPlayer())
		SetGameCameraMatrix(&matAim);
	else
	{
		if(m_pContextData)
			memcpy(m_pContextData->GetCameraMatrix(), &matAim, sizeof(Matrix));
	}
}

void CNetworkPlayer::GetAimSyncData(AimSyncData * aimSyncData)
{
	Matrix matAim;

	if(IsLocalPlayer())
	{
		GetGameCameraMatrix(&matAim);
	}
	else
	{
		if(m_pContextData)
			memcpy(&matAim, m_pContextData->GetCameraMatrix(), sizeof(Matrix));
		else
			matAim.Identity();
	}

	memcpy(&aimSyncData->vecRight, &matAim.vecRight, sizeof(CVector3));
	memcpy(&aimSyncData->vecForward, &matAim.vecForward, sizeof(CVector3));
	memcpy(&aimSyncData->vecUp, &matAim.vecUp, sizeof(CVector3));
	memcpy(&aimSyncData->vecPosition, &matAim.vecPosition, sizeof(CVector3));
}

void CNetworkPlayer::AddToWorld()
{
	if(IsSpawned())
		m_pPlayerPed->AddToWorld();
}

void CNetworkPlayer::RemoveFromWorld(bool bStopMoving)
{
	if(IsSpawned())
	{
		// Stop the player from moving to avoid some weird bugs
		if(bStopMoving)
			StopMoving();

		m_pPlayerPed->RemoveFromWorld();
	}
}

void CNetworkPlayer::GiveHelmet()
{
	if(IsSpawned())
	{
		Scripting::GivePedHelmet(GetScriptingHandle());

		m_bHelmet = true;
	}
}

void CNetworkPlayer::RemoveHelmet()
{
	if(IsSpawned())
	{
		Scripting::RemovePedHelmet(GetScriptingHandle(), true);

		m_bHelmet = false;
	}
}

// TODO: Don't use natives for this
void CNetworkPlayer::SetInterior(unsigned int uiInterior)
{
	if(IsSpawned() && GetInterior() != uiInterior)
		Scripting::SetRoomForCharByKey(GetScriptingHandle(), (Scripting::eInteriorRoomKey)uiInterior);
}

// TODO: Don't use natives for this
unsigned int CNetworkPlayer::GetInterior()
{
	if(IsSpawned())
	{
		unsigned int uiInterior;
		Scripting::GetKeyForCharInRoom(GetScriptingHandle(), (Scripting::eInteriorRoomKey *)&uiInterior);
		return uiInterior;
	}

	return 0;
}

void CNetworkPlayer::UpdateTargetPosition()
{
	if(HasTargetPosition())
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Get our position
		CVector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		// Define CVector3 for new speed.
		CVector3 vecNewMoveSpeed;
		GetMoveSpeed(vecNewMoveSpeed);

		// Get the factor of time spent from the interpolation start
		// to the current time.
		float fAlpha = Math::Unlerp(m_interp.pos.ulStartTime, ulCurrentTime, m_interp.pos.ulFinishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_interp.pos.fLastAlpha);
		m_interp.pos.fLastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, m_interp.pos.vecError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.0f)
			m_interp.pos.ulFinishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = (vecCurrentPosition + vecCompensation);

		// Check if the distance to interpolate is too far
		if((vecCurrentPosition - m_interp.pos.vecTarget).Length() > 5)
		{
			// Abort all interpolation
			m_interp.pos.ulFinishTime = 0;
			vecNewPosition = m_interp.pos.vecTarget;
		}

		// Set our new position
		SetPosition(vecNewPosition, false);

		// Set our new speed 
		SetMoveSpeed ( vecNewMoveSpeed );
	}
}

void CNetworkPlayer::Interpolate()
{
	// Are we not getting in/out of a vehicle?
	if(true)
		UpdateTargetPosition();
}

void CNetworkPlayer::SetTargetPosition(const CVector3 &vecPosition, unsigned long ulDelay)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Update our target position
		UpdateTargetPosition();

		// Get our position
		CVector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		// Set the target position
		m_interp.pos.vecTarget = vecPosition;

		// Calculate the relative error
		m_interp.pos.vecError = (vecPosition - vecCurrentPosition);

		// Get the interpolation interval
		unsigned long ulTime = SharedUtility::GetTime();
		m_interp.pos.ulStartTime = ulTime;
		m_interp.pos.ulFinishTime = (ulTime + ulDelay);

		// Initialize the interpolation
		m_interp.pos.fLastAlpha = 0.0f;
	}
}

void CNetworkPlayer::RemoveTargetPosition()
{
	m_interp.pos.ulFinishTime = 0;
}

void CNetworkPlayer::ResetInterpolation()
{
	RemoveTargetPosition();
}

void CNetworkPlayer::SetColor(unsigned int uiColor)
{
	if(IsSpawned())
		m_pPlayerInfo->SetColour(uiColor);

	m_uiColor = uiColor;
}

unsigned int CNetworkPlayer::GetColor()
{
	return m_uiColor;
}

void CNetworkPlayer::SetClothes(unsigned char ucBodyPart, unsigned char ucClothes)
{
	if(ucBodyPart > 10)
		return;

	if(IsSpawned())
	{
		// TODO: Array of this, then just check if valid, then set on char and variable
		unsigned char ucClothesIdx = 0;
		unsigned int uiDrawableVariations = Scripting::GetNumberOfCharDrawableVariations(GetScriptingHandle(), (Scripting::ePedComponent)ucBodyPart);

		for(unsigned int uiDrawable = 0; uiDrawable < uiDrawableVariations; ++uiDrawable)
		{
			unsigned int uiTextureVariations = Scripting::GetNumberOfCharTextureVariations(GetScriptingHandle(), (Scripting::ePedComponent)ucBodyPart, uiDrawable);

			for(unsigned int uiTexture = 0; uiTexture < uiTextureVariations; ++uiTexture)
			{
				if(ucClothesIdx == ucClothes)
				{
					//CLogFile::Printf(__FILE__,__LINE__,"CNetworkPlayer::SetClothes body: %d variat: %d text: %d", ucBodyPart, uiDrawable, uiTexture);
					Scripting::SetCharComponentVariation(GetScriptingHandle(), (Scripting::ePedComponent)ucBodyPart, uiDrawable, uiTexture);
					m_ucClothes[ucBodyPart] = ucClothes;
					return;
				}

				++ucClothesIdx;
			}
		}

		// No clothes available - use default clothes
		Scripting::SetCharComponentVariation(GetScriptingHandle(), (Scripting::ePedComponent)ucBodyPart, 0, 0);
		m_ucClothes[ucBodyPart] = 0;
	}
	else
		m_ucClothes[ucBodyPart] = ucClothes;
}

unsigned char CNetworkPlayer::GetClothes(unsigned char ucBodyPart)
{
	if(ucBodyPart > 10)
		return 0;

	return m_ucClothes[ucBodyPart];
}

void CNetworkPlayer::SetDucking(bool bDucking)
{
	if(IsSpawned())
		m_pPlayerPed->SetDucking(bDucking);
}

bool CNetworkPlayer::IsDucking()
{
	if(IsSpawned())
		return m_pPlayerPed->IsDucking();

	return false;
}

void CNetworkPlayer::SetCameraBehind()
{
	if(IsSpawned())
		g_pCamera->SetBehindPed(m_pPlayerPed);
}

void CNetworkPlayer::Pulse()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Is this the local player?
		if(IsLocalPlayer())
		{
			// Copy the current pad state to the previous pad state
			memcpy(&m_previousPadState, &m_currentPadState, sizeof(CPadState));

			// Update the current pad state
			CGame::GetPad()->GetCurrentClientPadState(m_currentPadState);
		}

		// If our health is locked set our health
		if(m_bHealthLocked)
			SetHealth(m_uiLockedHealth);

		// If our armour is locked set our armour
		if(m_bArmourLocked)
			SetArmour(m_uiLockedArmour);

		// Process vehicle entry/exit
		ProcessVehicleEntryExit();

		// Is this the local player?
		if(IsLocalPlayer())
		{
			// Check vehicle entry/exit key
			CheckVehicleEntryExitKey();
		}
		else
		{
			// Are we not in a vehicle?
			if(!IsInVehicle())
			{
				// Process interpolation
				Interpolate();
			}
		}
	}
}

void CNetworkPlayer::SetName(String strName)
{
	m_strName = strName;

	if(m_pPlayerInfo)
		m_pPlayerInfo->SetName(strName.GetData());
}

bool CNetworkPlayer::IsGettingInToAVehicle()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_GET_IN_VEHICLE)
				return true;
		}
	}

	return false;
}

bool CNetworkPlayer::IsGettingOutOfAVehicle()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_EXIT_VEHICLE)
				return true;
		}
	}

	return false;
}

bool CNetworkPlayer::IsJackingAVehicle()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_CAR_SLOW_DRAG_OUT_PED)
				return true;
		}
	}

	return false;
}

bool CNetworkPlayer::IsGettingJackedFromVehicle()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_CAR_SLOW_BE_DRAGGED_OUT)
				return true;
		}
	}

	return false;
}

bool CNetworkPlayer::ClearVehicleEntryTask()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_GET_IN_VEHICLE)
			{
				m_pPlayerPed->GetPedTaskManager()->RemoveTask(TASK_PRIORITY_PRIMARY);
				return true;
			}
		}
	}

	return false;
}

bool CNetworkPlayer::ClearVehicleExitTask()
{
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_EXIT_VEHICLE)
			{
				m_pPlayerPed->GetPedTaskManager()->RemoveTask(TASK_PRIORITY_PRIMARY);
				return true;
			}
		}
	}

	return false;
}

bool CNetworkPlayer::GetClosestVehicle(bool bPassenger, CNetworkVehicle ** pVehicle, BYTE &byteSeatId)
{
	// TODO: Get closest vehicle door not vehicle and add door parameter
	// Are we spawned?
	if(IsSpawned())
	{
		float fCurrent = 6.0f; // Maximum distance 6.0f
		CVector3 vecVehiclePos;
		CNetworkVehicle * pClosestVehicle = NULL;

		// Get our position
		CVector3 vecPlayerPos;
		GetPosition(vecPlayerPos);

		// Loop through all streamed in vehicles
		std::list<CStreamableEntity *> * streamedVehicles = g_pStreamer->GetStreamedInEntitiesOfType(STREAM_ENTITY_VEHICLE);

		for(std::list<CStreamableEntity *>::iterator iter = streamedVehicles->begin(); iter != streamedVehicles->end(); ++iter)
		{
			CNetworkVehicle * pTestVehicle = reinterpret_cast<CNetworkVehicle *>(*iter);

			// Get the vehicle position
			pTestVehicle->GetPosition(vecVehiclePos);

			// Get the distance between us and the vehicle
			float fDistance = Math::GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecVehiclePos.fX, vecVehiclePos.fY, vecVehiclePos.fZ);

			// Is the distance less than the current distance?
			if(fDistance < fCurrent)
			{
				// Set the current distance
				fCurrent = fDistance;

				// Set the closest vehicle pointer
				pClosestVehicle = pTestVehicle;
			}
		}

		// Do we have a valid closest vehicle pointer?
		if(pClosestVehicle == NULL)
			return false;

		// Are we looking for a passenger seat?
		if(bPassenger)
		{
			// Loop through all passenger seats
			BYTE byteTestSeatId = 0;

			for(BYTE i = 0; i < pClosestVehicle->GetMaxPassengers(); i++)
			{
				// Does this passenger seat contain a passenger?
				if(!pClosestVehicle->GetPassenger(i))
				{
					byteTestSeatId = (i + 1);
					break;
				}
			}

			// Do we have a valid test seat id?
			if(byteTestSeatId == 0)
				return false;

			// Set the seat id
			byteSeatId = byteTestSeatId;
		}
		else
		{
			// Set the seat id to the driver seat
			byteSeatId = 0;
		}

		// Set the vehicle pointer
		*pVehicle = pClosestVehicle;
		return true;
	}

	return false;
}

// From 0x9C6DB0
// Looks like vehicles never have more than 4 doors?
BYTE GetDoorFromSeat(BYTE byteSeatId)
{
	BYTE byteDoorId;

	switch(byteSeatId)
	{
	case 0:
		byteDoorId = 2;
		break;
	case 1:
		byteDoorId = 1;
		break;
	case 2:
		byteDoorId = 3;
		break;
	default:
		byteDoorId = 0;
		break;
	}

	return byteDoorId;
}

int iProcessFrames = 0;

void CNetworkPlayer::EnterVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	// Are we spawned?
	if(IsSpawned())
	{
		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d)", pVehicle, byteSeatId);

		// Is the vehicle invalid?
		if(!pVehicle)
			return;

		// Is the vehicle not spawned?
		if(!pVehicle->IsStreamedIn())
		{
			// Are we the local player?
			if(IsLocalPlayer())
			{
				// Force the vehicle to stream in
				g_pStreamer->ForceStreamIn(pVehicle);
			}
		}

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 2", pVehicle, byteSeatId);

		// Are we already in a vehicle?
		if(IsInVehicle())
			return;

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 3", pVehicle, byteSeatId);

		/*BYTE byteDoorId = GetDoorFromSeat(byteSeatId);
		int iUnknown2 = 0;

		//if(byteSeatId != 0)
		//	iUnknown2 = 0x200000;

		// Create the enter vehicle task
		CIVTaskComplexNewGetInVehicle * pTask = new CIVTaskComplexNewGetInVehicle(m_pVehicle->GetGameVehicle(), byteDoorId, 27, iUnknown2, 0.0f);

		// Did the task create successfully?
		if(pTask)
		{
			// Set it as the ped task
			pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
		}*/

		// Is the vehicle streamed in?
		if(pVehicle->IsStreamedIn())
		{
			// Are we entering as the driver?
			if(byteSeatId == 0)
			{
				// Start the enter car as driver task
				Scripting::TaskEnterCarAsDriver(GetScriptingHandle(), pVehicle->GetScriptingHandle(), -2);
			}
			else
			{
				// Start the enter car as passenger task
				Scripting::TaskEnterCarAsPassenger(GetScriptingHandle(), pVehicle->GetScriptingHandle(), -2, (byteSeatId - 1));
			}

			CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 4", pVehicle, byteSeatId);

			// Mark ourselves as entering a vehicle and store our vehicle and seat
			m_vehicleEnterExit.bEntering = true;
			m_vehicleEnterExit.pVehicle = pVehicle;
			m_vehicleEnterExit.byteSeatId = byteSeatId;

			CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 5", pVehicle, byteSeatId);

			// Reset interpolation
			ResetInterpolation();

			// see ProcessVehicleEntryExit
			if(IsLocalPlayer())
				iProcessFrames = 1;
		}
	}
}

void CNetworkPlayer::ExitVehicle()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle?
		if(m_pVehicle)
		{
			// Create the exit vehicle task
			CIVTaskComplexNewExitVehicle * pTask = new CIVTaskComplexNewExitVehicle(m_pVehicle->GetGameVehicle(), 0xF, 0, 0);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
			}

			// Mark ourselves as exiting a vehicle
			m_vehicleEnterExit.bExiting = true;
		}
		else
		{
			// Are we entering a vehicle?
			if(HasVehicleEnterExit())
			{
				// Clear the vehicle entry task
				ClearVehicleEntryTask();
			}
		}

		// Reset interpolation
		ResetInterpolation();
	}
}

void CNetworkPlayer::PutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	CLogFile::Printf("CNetworkPlayer::PutInVehicle - 1");
	// Are we spawned?
	if(IsSpawned())
	{
		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 2");
		// Is the vehicle invalid?
		if(!pVehicle)
			return;
		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 3");

		// Is the vehicle not spawned?
		if(!pVehicle->IsStreamedIn())
		{
			// Are we the local player?
			if(IsLocalPlayer())
			{
				CLogFile::Printf("CNetworkPlayer::PutInVehicle - 4");
				// Force the vehicle to stream in
				g_pStreamer->ForceStreamIn(pVehicle);
				CLogFile::Printf("CNetworkPlayer::PutInVehicle - 5");
			}
			else
				return;
		}

		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 8");

		// Are we already in a vehicle?
		if(IsInVehicle())
		{
			CLogFile::Printf("CNetworkPlayer::PutInVehicle - 9");
			// Remove ourselves from our current vehicle
			RemoveFromVehicle();
			CLogFile::Printf("CNetworkPlayer::PutInVehicle - 10");
		}

		/*
		v3 = CPool__AtHandle(g_pPedPool, a1);
		v2 = CPool__AtHandle(g_pVehiclePool, a2);
		if ( *(_DWORD *)(v3 + 0x24) & 0x8000000 )
		{
		CWorld__RemoveEntity(v3, 0);
		sub_9E6830(v3, v2, 0);
		CWorld__AddEntity(v3, 0);
		}
		result = *(_DWORD *)(v3 + 0x6C);
		if ( !result || !*(_BYTE *)(result + 0xE) )
		{
		ShutdownPedIntelligence(*(void **)(v3 + 0x224), 0);
		v5 = GetDoorFromSeat(-1);
		CTaskSimpleCarSetPedInVehicle__CTaskSimpleCarSetPedInVehicle(&v6, v2, v5, 0, 0);
		sub_AA07C0(v3);
		result = CTaskSimpleCarSetPedInVehicle___CTaskSimpleCarSetPedInVehicle(&v6);
		}
		return result;
		*/

		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 11");

		// Internally put ourselves into the vehicle
		if(pVehicle->IsStreamedIn())
			InternalPutInVehicle(pVehicle, byteSeatId);

		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 12");

		// Reset vehicle entry/exit
		ResetVehicleEnterExit();
		m_pVehicle = pVehicle;
		m_byteVehicleSeatId = byteSeatId;
		pVehicle->SetOccupant(byteSeatId, this);
		CLogFile::Printf("CNetworkPlayer::PutInVehicle - 13");
	}
}

void CNetworkPlayer::RemoveFromVehicle()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle?
		if(m_pVehicle)
		{
			// Internally remove ourselves from the vehicle
			InternalRemoveFromVehicle();

			// Reset the vehicle occupant for our seat
			m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, NULL);

			// Reset our current vehicle pointer
			m_pVehicle = NULL;

			// Reset our vehicle seat id
			m_byteVehicleSeatId = 0;

			// Reset vehicle entry/exit flags
			ResetVehicleEnterExit();
		}
	}
}

void CNetworkPlayer::CheckVehicleEntryExitKey()
{
	// Are we spawned and is input enabled and are our controls not disabled?
	if(IsSpawned() && CGame::GetInputState() && !m_bControlsDisabled)
	{
		// Has the enter/exit vehicle key just been pressed?
		if(m_currentPadState.IsUsingEnterExitVehicle() && !m_previousPadState.IsUsingEnterExitVehicle())
		{
			if(!m_vehicleEnterExit.bRequesting && IsInVehicle() && !m_vehicleEnterExit.bExiting)
			{
				if(IsLocalPlayer())
					CLogFile::Printf("HandleVehicleExitKey(LocalPlayer)");
				else
					CLogFile::Printf("HandleVehicleExitKey(%d)", m_playerId);

				// Are we not already requesting a vehicle entry or exit?
				if(!m_vehicleEnterExit.bRequesting)
				{
					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Request the vehicle exit
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_EXIT_REQUEST);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
						m_vehicleEnterExit.bRequesting = true;
					}
					else
					{
						// Exit the vehicle
						ExitVehicle();
					}
				}
				else
				{
					CLogFile::Printf("Already requesting vehicle entry/exit!");
				}
			}
		}
		else
		{
			// Has the enter/exit vehicle key just been released?
			bool bEnterExitVehicleKeyReleased = false;

			if(m_previousPadState.IsUsingEnterExitVehicle() && !m_currentPadState.IsUsingEnterExitVehicle())
				bEnterExitVehicleKeyReleased = true;

			// Has the horn key just been released?
			bool bHornKeyReleased = false;

			if(m_previousPadState.IsUsingHorn() && !m_currentPadState.IsUsingHorn())
				bHornKeyReleased = true;

			// Has the enter/exit vehicle key or the horn key just been released?
			if(bEnterExitVehicleKeyReleased || bHornKeyReleased)
			{
				if(IsLocalPlayer())
					CLogFile::Printf("HandleVehicleEntryKey(LocalPlayer)");
				else
					CLogFile::Printf("HandleVehicleEntryKey(%d)", m_playerId);

				// Are we not already requesting a vehicle entry or exit?
				if(!m_vehicleEnterExit.bRequesting)
				{
					if(!IsInVehicle() && !m_vehicleEnterExit.bEntering)
					{
						CNetworkVehicle * pVehicle = NULL;
						BYTE byteSeatId = 0;
						bool bFound = false;

						// Has the horn key just been released?
						if(bHornKeyReleased)
						{
							// Do we have a close vehicle?
							bFound = GetClosestVehicle(true, &pVehicle, byteSeatId);
						}
						else
						{
							// Enter/exit vehicle key has just been released
							bFound = GetClosestVehicle(false, &pVehicle, byteSeatId);
						}

						// Have we found a close vehicle?
						if(bFound)
						{
							if(IsLocalPlayer())
								CLogFile::Printf("HandleVehicleEntry(LocalPlayer, %d, %d)", pVehicle->GetVehicleId(), byteSeatId);
							else
								CLogFile::Printf("HandleVehicleEntry(%d, %d, %d)", m_playerId, pVehicle->GetVehicleId(), byteSeatId);

							// Is this a network vehicle?
							if(pVehicle->IsNetworkVehicle())
							{
								// Request the vehicle entry
								CBitStream bitStream;
								bitStream.Write((BYTE)VEHICLE_ENTRY_REQUEST);
								bitStream.WriteCompressed(pVehicle->GetVehicleId());
								bitStream.Write(byteSeatId);
								g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
								m_vehicleEnterExit.bRequesting = true;
							}
							else
							{
								// Enter the vehicle
								EnterVehicle(pVehicle, byteSeatId);
							}
						}
					}
				}
				else
				{
					CLogFile::Printf("Already requesting vehicle entry/exit!");
				}
			}
		}
	}
}

void CNetworkPlayer::ProcessVehicleEntryExit()
{
	// this hack is needed to make vehicle entry work for network vehicles
	// (due to the fact when you call task natives the task doesn't apply till 
	// next frame) once we apply vehicle entry/exit tasks properly, this can 
	// be removed
	// note: not safe for more than one player at a time
	if(IsLocalPlayer())
	{
		if(iProcessFrames > 0)
		{
			iProcessFrames++;

			if(iProcessFrames == 4)
				iProcessFrames = 0;
			else
				return;
		}
	}

	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle internally?
		if(InternalIsInVehicle())
		{
			// Are we flagged as entering a vehicle?
			if(m_vehicleEnterExit.bEntering)
			{
				// Have we finished our enter vehicle task?
				if(!IsGettingInToAVehicle())
				{
					// Vehicle entry is complete
					m_vehicleEnterExit.bEntering = false;
					m_pVehicle = m_vehicleEnterExit.pVehicle;
					m_byteVehicleSeatId = m_vehicleEnterExit.byteSeatId;
					m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, this);
					m_vehicleEnterExit.pVehicle = NULL;

					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_ENTRY_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						bitStream.Write(m_byteVehicleSeatId);
						g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleEntryComplete(LocalPlayer)");
					else
						CLogFile::Printf("VehicleEntryComplete(%d)", m_playerId);
				}
			}
		}
		else
		{
			// Are we flagged as entering a vehicle?
			if(m_vehicleEnterExit.bEntering)
			{
				// Do we no longer have our enter vehicle task?
				if(!IsGettingInToAVehicle())
				{
					// Are we the local player?
					if(IsLocalPlayer())
					{
						// NOTE: Isn't there some exit vehicle task abort event?
						// Is our enter/exit vehicle a network vehicle?
						if(m_vehicleEnterExit.pVehicle->IsNetworkVehicle())
						{
							// Get our position
							CVector3 vecPosition;
							GetPosition(vecPosition);

							// Send the network rpc
							CBitStream bitStream;
							bitStream.Write((BYTE)VEHICLE_ENTRY_CANCELLED);
							bitStream.WriteCompressed(m_vehicleEnterExit.pVehicle->GetVehicleId());
							bitStream.Write(m_byteVehicleSeatId);
							g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);

							CLogFile::Printf("VehicleEntryCancelled(LocalPlayer)");
						}

						// Vehicle entry has been canceled
						m_vehicleEnterExit.bEntering = false;
						m_vehicleEnterExit.pVehicle = NULL;
					}
					else
					{
						// Force ourselves to enter the vehicle
						EnterVehicle(m_vehicleEnterExit.pVehicle, m_vehicleEnterExit.byteSeatId);
						CLogFile::Printf("VehicleEntryRestarted(%d)", m_playerId);
					}
				}
			}
			else
			{
				// Do we have an enter vehicle task?
				if(IsGettingInToAVehicle())
				{
					// Clear our vehicle entry task
					ClearVehicleEntryTask();

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleEntryRemoved(LocalPlayer)");
					else
						CLogFile::Printf("VehicleEntryRemoved(%d)", m_playerId);
				}
			}

			// Are we flagged as exiting a vehicle?
			if(m_vehicleEnterExit.bExiting)
			{
				// Have we finished our exit vehicle task?
				if(!IsGettingOutOfAVehicle())
				{
					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_EXIT_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Vehicle exit is complete
					m_vehicleEnterExit.bExiting = false;
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle = NULL;
					m_byteVehicleSeatId = 0;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleExitComplete(LocalPlayer)");
					else
						CLogFile::Printf("VehicleExitComplete(%d)", m_playerId);
				}
			}
			else
			{
				// Do we have an exit vehicle task?
				if(IsGettingOutOfAVehicle())
				{
					// Clear our vehicle exit task
					ClearVehicleExitTask();

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleExitRemoved(LocalPlayer)");
					else
						CLogFile::Printf("VehicleExitRemoved(%d)", m_playerId);
				}

				// Are we flagged as in a vehicle?
				if(m_pVehicle)
				{
					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_EXIT_FORCEFUL);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pNetworkManager->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Player has forcefully exited the vehicle (out of windscreen, e.t.c.)
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle = NULL;
					m_byteVehicleSeatId = 0;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleForcefulExit(LocalPlayer)");
					else
						CLogFile::Printf("VehicleForcefulExit(%d)", m_playerId);
				}
			}
		}
	}
}

void CNetworkPlayer::ResetVehicleEnterExit()
{
	// Reset the vehicle enter/exit flags
	m_vehicleEnterExit.bEntering = false;
	m_vehicleEnterExit.pVehicle = NULL;
	m_vehicleEnterExit.byteSeatId = 0;
	m_vehicleEnterExit.bExiting = false;
	m_vehicleEnterExit.bRequesting = false;

	// Clear the vehicle entry task
	ClearVehicleEntryTask();

	// Clear the vehicle exit task
	ClearVehicleExitTask();
}

void CNetworkPlayer::ToggleRagdoll(bool bToggle)
{
	if(IsSpawned())
		m_pPlayerPed->SetRagdoll(bToggle);
}

bool CNetworkPlayer::IsOnScreen()
{
	// Are we spawned?
	if(IsSpawned())
		return /*Scripting::IsCharOnScreen(GetScriptingHandle())*/true;

	return false;
}

void CNetworkPlayer::SetWorldDimensions ( int iWorldDimension )
{ 
	m_iWorldDimension = iWorldDimension; 
}
