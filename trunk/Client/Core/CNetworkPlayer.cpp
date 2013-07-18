//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkPlayer.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNetworkPlayer.h"
#include "CClient.h"
#include <Patcher/CPatcher.h>
#include "KeySync.h"
#include "CLocalPlayer.h"
#include <SharedUtility.h>
#include "COffsets.h"
#include "CPools.h"

extern CClient * g_pClient;
extern bool      g_bControlsDisabled;

IVPlayerPed * g_pCreatedPlayerPeds[32];
bool          g_bPlayerPedsUsed[32];
CIVPlayerInfo * g_pPlayerInfos[32];

#define THIS_CHECK if(!this) { if(g_pClient->GetChatWindow()) { g_pClient->GetChatWindow()->AddErrorMessage("[WARNING] Internal error occured in %s[ Func %s() ]",__FILE__,__FUNCTION__); } return; }
#define THIS_CHECK_R(x) if(!this) { if(g_pClient->GetChatWindow()) { g_pClient->GetChatWindow()->AddErrorMessage("[WARNING] Internal error occured in %s [ Func %s() ]",__FILE__,__FUNCTION__); } return x; }

void InitNetworkPlayerPedStuff()
{
	for(int i = 0; i < 32; i++)
	{
		g_pCreatedPlayerPeds[i] = NULL;
		g_bPlayerPedsUsed[i] = false;
		g_pPlayerInfos[i] = NULL;
	}
}


CNetworkPlayer::CNetworkPlayer(bool bIsLocalPlayer)
	: CStreamableEntity(STREAM_ENTITY_PLAYER, -1),
	m_bIsLocalPlayer(bIsLocalPlayer),
	m_playerId(INVALID_ENTITY_ID),
	m_pContextData(NULL),
	m_byteGamePlayerNumber(0),
	m_pPlayerInfo(NULL),
	m_pModelInfo(CGame::GetModelInfo(MODEL_PLAYER_INDEX)),
	m_bSpawned(false),
	m_uiColor(0xFFFFFFFF),
	m_fArmHeading(0.0f),
	m_fArmUpDown(0.0f),
	m_usPing(0),
	m_pVehicle(NULL),
	m_byteVehicleSeatId(0),
	m_bHealthLocked(false),
	m_bArmourLocked(false),
	m_bPlayerBlipCreated(false),
	m_uiPlayerBlipHandle(NULL),
	m_bHelmet(false),
	m_bUseMobilePhone(false),
	m_bUseCustomClothesOnSpawn(false)
	
{
	m_interp.pos.ulFinishTime = 0;
	m_interp.rot.ulFinishTime = 0;
	memset(&m_ucClothes, 0, sizeof(m_ucClothes));

	memset(&m_previousControlState, 0, sizeof(CControlState));
	memset(&m_currentControlState, 0, sizeof(CControlState));
	ResetVehicleEnterExit();

	
	if(IsLocalPlayer())
	{

		// Places all peds vars/variables to false/null
		InitNetworkPlayerPedStuff();

		// Create a new player ped instance with the local player ped
		m_pPlayerPed = new CIVPlayerPed(CGame::GetPools()->GetPlayerInfoFromIndex(0)->m_pPlayerPed);

		// Get the local player info pointer
		m_pPlayerInfo = new CIVPlayerInfo(CGame::GetPools()->GetPlayerInfoFromIndex(0));

		// Create a new context data instance with the local player info
		m_pContextData = CContextDataManager::CreateContextData(m_pPlayerInfo);

		// Set the context data player ped pointer
		m_pContextData->SetPlayerPed(m_pPlayerPed);

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

	if(!IsLocalPlayer())
		SetCanBeStreamedIn(true);

}

CNetworkPlayer::~CNetworkPlayer()
{
	CLogFile::Printf("Destroy NetworkPlayer");
	if(!IsLocalPlayer()) {
		// Destroy ourselves
		OnDelete();
	}
}
#include "IV/CIVPedMoveBlend.h"


/* Does not work atm */
void SetupPed(Matrix34 * pMatrix, IVPed * pPed, int iModelIndex)
{
	IVPedMoveBlendOnFoot * pMoveBlendOnFoot = (IVPedMoveBlendOnFoot*)CGame::GetPools()->GetPedMoveBlendPool()->Allocate();

	if(pMoveBlendOnFoot)
	{
		_asm
		{
			push pPed
			mov ecx, pMoveBlendOnFoot
			call COffsets::FUNC_CPedMoveBlendOnFoot__Constructor
			mov pMoveBlendOnFoot, eax
		}
		pPed->m_pPedMoveBlendOnFoot = pMoveBlendOnFoot;
	}

	//if(pMatrix)
		//CMatrix::CopyFromMatrix(pPed->m_pMatrix, pMatrix);

	pPed->SetModelIndex(iModelIndex);

	IVPedData * pPedData;
	DWORD dwFunc = CGame::GetBase() + 0x43A960;
	_asm
	{
		call dwFunc
		mov pPedData, eax
	}
	if(pPedData)
	{
		pPedData->dword4 = *(DWORD*)(pPed->m_pPedBase + 0x12C);
		((int**)pPedData)[0] = (int*)COffsets::VAR_IPedDataNY;
		pPed->m_pPedData = pPedData;
	}
}

bool CNetworkPlayer::Create()
{
	THIS_CHECK_R(false);

	// Are we already spawned or are we the local player?
	if(IsSpawned() || IsLocalPlayer())
		return false;

	// jenksta: hack
	// do we have any unused player peds created already?
	IVPlayerPed * pOldPlayerPed = NULL;
	CIVPlayerInfo * pOldPlayerInfo = NULL;

	for(int i = 0; i < 32; i++)
	{
		if(g_pCreatedPlayerPeds[i] && g_bPlayerPedsUsed[i] == false)
		{
			pOldPlayerPed = g_pCreatedPlayerPeds[i];
			g_bPlayerPedsUsed[i] = true;
			pOldPlayerInfo = g_pPlayerInfos[i];
			break;
		}
	}
	
	// Add our model info reference
	m_pModelInfo->AddReference(true);

	if(pOldPlayerPed)
	{
		// Set the player info instance
		m_pPlayerInfo = pOldPlayerInfo;

		// Get the player number
		m_byteGamePlayerNumber = m_pPlayerInfo->GetPlayerNumber();

		// Create player ped instance
		m_pPlayerPed = new CIVPlayerPed(pOldPlayerPed);

		// Create a context data instance for this player
		m_pContextData = CContextDataManager::CreateContextData(m_pPlayerInfo);

		// Set the context data player ped pointer
		m_pContextData->SetPlayerPed(m_pPlayerPed);
	}
	else
	{

		//ViruZz: Ignore the tab size here k?

		// Find a free player number
		m_byteGamePlayerNumber = (BYTE)CGame::GetPools()->FindFreePlayerInfoIndex();

		// Invalid player number?
		if(m_byteGamePlayerNumber == INVALID_PLAYER_PED)
			return false;

		// Get our model index
		int iModelIndex = m_pModelInfo->GetIndex();

		// Create player info instance
		m_pPlayerInfo = new CIVPlayerInfo(m_byteGamePlayerNumber);

		// Create a context data instance for this player
		m_pContextData = CContextDataManager::CreateContextData(m_pPlayerInfo);

		// Set game player info pointer
		CGame::GetPools()->SetPlayerInfoAtIndex((unsigned int)m_byteGamePlayerNumber, m_pPlayerInfo->GetPlayerInfo());

		// Allocate the player ped
		IVPlayerPed * pPlayerPed = (IVPlayerPed *)CGame::GetPools()->GetPedPool()->Allocate();

		// Ensure the player ped pointer is valid
		if(!pPlayerPed)
		{
			Destroy();
			return false;
		}

		// Call the CPlayerPed constructor
		unsigned int uiPlayerIndex = (unsigned int)m_byteGamePlayerNumber;
		WORD wPlayerData = MAKEWORD(0, 1);
		WORD * pwPlayerData = &wPlayerData;
		_asm
		{
			push uiPlayerIndex
			push iModelIndex
			push pwPlayerData
			mov ecx, pPlayerPed
			call COffsets::FUNC_CPlayerPed__Constructor
		}

		if(!pPlayerPed)
			return false;

		Matrix34 * pMatrix = NULL;
		_asm
		{
			push iModelIndex
			push COffsets::VAR_Ped_Factory
			mov edi, pMatrix
			mov esi, pPlayerPed
			call COffsets::FUNC_Setup_Ped
		}
		//SetupPed(pMatrix, pPlayerPed, iModelIndex);

		// Set our player info ped pointer
		m_pPlayerInfo->SetPlayerPed(pPlayerPed);

		// Set our player info state
		m_pPlayerInfo->GetPlayerInfo()->m_dwState = 2;

		SET_BIT(*(DWORD *)(pPlayerPed + 0x260), 1);

		// Set our player peds player info pointer
		pPlayerPed->m_pPlayerInfo = m_pPlayerInfo->GetPlayerInfo();

		// Create player ped instance
		m_pPlayerPed = new CIVPlayerPed(pPlayerPed);

		// Set the context data player ped pointer
		m_pContextData->SetPlayerPed(m_pPlayerPed);

		/*
		pPlayerPed->m_byteCreatedBy = 2;
		pPlayerPed->SetDefaultDecisionMaker();
		pPlayerPed->GetPedIntelligence()->SetSenseRange1(30.0f);
		pPlayerPed->GetPedIntelligence()->SetSenseRange0(30.0f);
		sub_B2A8F0(pPlayerPed->m_pPortalTracker, 1);
		sub_B29E50(pPlayerPed->m_pPortalTracker);
		*/
		_asm
		{
			push 2
			mov ecx, pPlayerPed
			call COffsets::FUNC_SetupPedIntelligence
		}

		// jenksta: hack
		g_pCreatedPlayerPeds[m_byteGamePlayerNumber] = pPlayerPed;
		g_bPlayerPedsUsed[m_byteGamePlayerNumber] = true;
		g_pPlayerInfos[m_byteGamePlayerNumber] = m_pPlayerInfo;
	}

	// Add to world
	m_pPlayerPed->AddToWorld();

	// jenksta: wtf is this doing here??
	// Delete player helmet
	m_bHelmet = false;
	SetHelmet(m_bHelmet);

	// Flag as spawned
	m_bSpawned = true;

	// Set health
	SetHealth(200);

	// Set the interior
	SetInterior(g_pClient->GetLocalPlayer()->GetInterior());

	// Assign our default task
	// (Part of player ped creation)
	CIVTaskComplexPlayerOnFoot * pTask = new CIVTaskComplexPlayerOnFoot();
	if(pTask)
		pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_DEFAULT);

	// Remember that we might have clothes
	m_bUseCustomClothesOnSpawn = true;

	// Reset interpolation
	ResetInterpolation();
	return true;
}

void CNetworkPlayer::Init()
{
	THIS_CHECK;
	// Set again model
	//SetModel(m_pModelInfo->GetHash());
}

void CNetworkPlayer::Destroy()
{
	THIS_CHECK;
	// Are we not the local player?
	if(!IsLocalPlayer())
	{
		// Are we spawned?
		if(IsSpawned())
		{
			// jenksta: hacky hack hack
			// jenksta: keep player ped for future use...
			g_bPlayerPedsUsed[m_byteGamePlayerNumber] = false;
			m_pPlayerPed->RemoveFromWorld();
			m_pPlayerPed->GetPlayerPed()->m_pLivery = 0;

			// Remove our model info reference
			m_pModelInfo->RemoveReference();
#if 0
		ViruZz: Don't worry about the tab size here k?
		// Get the player ped pointer
		IVPlayerPed * pPlayerPed = m_pPlayerPed->GetPlayerPed();

		IVPedIntelligence * pPedIntelligence = pPlayerPed->m_pPedIntelligence;
		_asm
		{
			push 0
			mov ecx, pPedIntelligence
			call COffsets::FUNC_CPedIntelligence__Reset
		}

		UNSET_BIT(*(DWORD *)(pPlayerPed + 0x260), 1);

		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();

		_asm
		{
			push 1
			mov ecx, pPlayerPed
			call COffsets::FUNC_CPed__ScalarDeletingDestructor
		}

		// Remove our model info reference
		m_pModelInfo->RemoveReference();
#endif
		}
	}

	// Do we have a context data instance
	if(m_pContextData)
	{
		// Delete the context data instance
		CContextDataManager::DestroyContextData(m_pContextData);

		// Set the context data pointer to NULL
		m_pContextData = NULL;
	}

	// Delete the player ped instance
	SAFE_DELETE(m_pPlayerPed);

	// jenksta: player ped is kept so keep player info too
	// jenksta: invalidate the player number only
	m_byteGamePlayerNumber = INVALID_PLAYER_PED;
#if 0
	// Delete our player info instance
	SAFE_DELETE(m_pPlayerInfo);

	// Are we not the local player ped and do we have a valid player number?
	if(!IsLocalPlayer() && m_byteGamePlayerNumber != INVALID_PLAYER_PED)
	{
		// Reset game player info pointer
		CGame::GetPools()->SetPlayerInfoAtIndex((unsigned int)m_byteGamePlayerNumber, NULL);

		// Invalidate the player number
		m_byteGamePlayerNumber = INVALID_PLAYER_PED;
	}
#endif

	// Flag ourselves as despawned
	m_bSpawned = false;
}

void CNetworkPlayer::StreamIn()
{
	THIS_CHECK;
	if(Create())
	{
		SetPosition(m_vecPos);
		SetHealth(m_uiHealth);
	}
}

void CNetworkPlayer::StreamOut()
{
	THIS_CHECK;

	// Check if the camera is attached to our player
	if(g_pClient->GetLocalPlayer()->IsCameraAttachedToEntity(GetScriptingHandle()))
		return;

	GetPosition(m_vecPos);
	m_uiHealth = GetHealth();
	Destroy();
}

void CNetworkPlayer::Kill(bool bInstantly)
{
	THIS_CHECK;
	// Are we spawned and not already dead?
	if(IsSpawned() && !IsDead())
	{
		// Are we getting killed instantly?
		if(bInstantly)
		{
			/* Only use complex die

			// Create the dead task
			// if this doesn't work vary last 2 params (1, 0 : 0, 1 : 1, 1 : 0, 0)
			CIVTaskSimpleDead * pTask = new CIVTaskSimpleDead(CGame::GetTime(), 1, 0);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_DEFAULT);
			}
			*/
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

		// Reset the control state
		CControlState controlState;
		SetControlState(&controlState);

		// Reset vehicle entry/exit flags
		ResetVehicleEnterExit();

		// Reset interpolation
		ResetInterpolation();
	}
}

bool CNetworkPlayer::IsDying()
{
	THIS_CHECK_R(false);
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);

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
	THIS_CHECK_R(false);
	if(IsSpawned())
	{
		// jenksta: HACK: code below never seems to trigger so use IsDying instead
		return IsDying();
		/*CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_DEAD)
				return true;
		}*/
	}

	return false;
}

IVEntity * CNetworkPlayer::GetLastDamageEntity()
{
	THIS_CHECK_R(NULL);
	if(IsSpawned())
		return m_pPlayerPed->GetLastDamageEntity();

	return NULL;
}

bool CNetworkPlayer::GetKillInfo(EntityId * playerId, EntityId * vehicleId, EntityId * weaponId)
{
	THIS_CHECK_R(false);
	// Are we spawned?
	if(IsSpawned())
	{
		// Reset player id and vehicle id
		*playerId = INVALID_ENTITY_ID;
		*vehicleId = INVALID_ENTITY_ID;
		*weaponId = INVALID_ENTITY_ID;

		// Loop through all players
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			// Is this player connected and spawned?
			CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(i);

			if(pPlayer && pPlayer->IsSpawned())
			{
				// Is this player the last damage entity?
				if(GetLastDamageEntity() == (IVEntity *)pPlayer->GetGamePlayerPed()->GetPed())
				{
					// This player killed us
					*playerId = i;
					*weaponId = pPlayer->GetCurrentWeapon();
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
						*weaponId = pPlayer->GetCurrentWeapon();
						*vehicleId = i;
						break;
					}
				}
			}
		}

		// Have we not yet found a killer?
		if(*playerId == INVALID_ENTITY_ID && *vehicleId == INVALID_ENTITY_ID)
		{
			// Loop through all streamed in vehicles
			std::list<CStreamableEntity *> * streamedVehicles = g_pClient->GetStreamer()->GetStreamedInEntitiesOfType(STREAM_ENTITY_VEHICLE);

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
	THIS_CHECK_R(false);
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
	THIS_CHECK;
	if(IsSpawned())
		SetMoveSpeed(CVector3());
}

bool CNetworkPlayer::InternalIsInVehicle()
{
	THIS_CHECK_R(false);
	// Are we spawned?
	if(IsSpawned())
		return (m_pPlayerPed->IsInVehicle() && m_pPlayerPed->GetCurrentVehicle());

	return false;
}

CNetworkVehicle * CNetworkPlayer::InternalGetVehicle()
{
	THIS_CHECK_R(NULL);
	// Are we spawned and in a vehicle?
	if(IsSpawned() && InternalIsInVehicle())
		return g_pClient->GetStreamer()->GetVehicleFromGameVehicle(m_pPlayerPed->GetCurrentVehicle());

	return NULL;
}

void CNetworkPlayer::InternalPutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	THIS_CHECK;
	// Are we spawned and not in a vehicle?
	if(IsSpawned() && !InternalIsInVehicle())
	{
		// Get the door
		int iDoor = -2;

		if(byteSeatId == 0)
			iDoor = 0;
		else if(byteSeatId == 1)
			iDoor = 2;
		else if(byteSeatId == 2)
			iDoor = 1;
		else if(byteSeatId == 3)
			iDoor = 3;

		// Create the car set ped in vehicle task
		CIVTaskSimpleCarSetPedInVehicle * pTask = new CIVTaskSimpleCarSetPedInVehicle(pVehicle->GetGameVehicle(), iDoor, 0, 0);

		// Did the task create successfully?
		if(pTask)
		{
			// Process the ped
			pTask->ProcessPed(m_pPlayerPed);

			// Destroy the task
			pTask->Destroy();
		}
	}
}

void CNetworkPlayer::InternalRemoveFromVehicle()
{
	THIS_CHECK;
	// Are we spawned and in a vehicle?
	if(IsSpawned() && m_pVehicle)
	{
		// Set the vehicle can be damaged to false before the task out is called, because when the client crashs, the vehicle is still damage able
		m_pVehicle->SetDamageable(false);

		// Create the car set ped out task
		CIVTaskSimpleCarSetPedOut * pTask = new CIVTaskSimpleCarSetPedOut(m_pVehicle->GetGameVehicle(), 0xF, 0, 1);

		// Did the task create successfully?
		if(pTask)
		{
			// Process the ped
			pTask->ProcessPed(m_pPlayerPed);

			// Destroy the task
			pTask->Destroy();
		}
	}
}

unsigned int CNetworkPlayer::GetScriptingHandle()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return CGame::GetPools()->GetPedPool()->HandleOf(m_pPlayerPed->GetPed());

	return 0;
}

void CNetworkPlayer::SetModel(DWORD dwModelHash)
{
	THIS_CHECK;
	CLogFile::PrintDebugf("SETMODEL %p | PlayerId: %d",dwModelHash,m_playerId);

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

			unsigned int uiHealth = GetHealth();
			unsigned int uiArmour = GetArmour();
			float fHeading = GetCurrentHeading();
			unsigned int uiInterior = GetInterior();
			unsigned int uiCurrentWeapon = GetCurrentWeapon();
			unsigned int uiAmmoInClip = GetAmmoInClip();
			unsigned int uiWeaponInfo[WEAPON_SLOT_MAX][2];

			for(unsigned int i = 0; i < WEAPON_SLOT_MAX; i++)
				GetWeaponInSlot(i, uiWeaponInfo[i][0], uiWeaponInfo[i][1]);

			Scripting::ChangePlayerModel(m_byteGamePlayerNumber, (Scripting::eModel)dwModelHash);
			//m_pPlayerPed->SetModelIndex(m_pModelInfo->GetIndex());

			m_pPlayerPed->SetPed(m_pPlayerInfo->GetPlayerPed());
			if(!IsLocalPlayer())
				g_pCreatedPlayerPeds[m_byteGamePlayerNumber] = m_pPlayerInfo->GetPlayerPed();
			SetHealth(uiHealth);
			SetArmour(uiArmour);
			SetCurrentHeading(fHeading);
			SetInterior(uiInterior);

			for(unsigned int i = 0; i < WEAPON_SLOT_MAX; i++)
			{
				if(uiWeaponInfo[i][0] == uiCurrentWeapon)
					GiveWeapon(uiWeaponInfo[i][0], (uiWeaponInfo[i][1] + uiAmmoInClip));
				else
					GiveWeapon(uiWeaponInfo[i][0], uiWeaponInfo[i][1]);
			}

			SetCurrentWeapon(uiCurrentWeapon);
			SetAmmoInClip(uiAmmoInClip);
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

void CNetworkPlayer::Teleport(const CVector3& vecPosition, bool bResetInterpolation)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we not in a vehicle?
		if(!IsInVehicle())
		{
			Scripting::SetCharCoordinatesNoOffset(GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
			//m_pPlayerPed->SetPosition(vecPosition);
		}
		else
			Scripting::WarpCharFromCarToCoord(GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	}

	// Reset interpolation if requested
	if(bResetInterpolation)
		ResetInterpolation();
}

void CNetworkPlayer::SetPosition(const CVector3& vecPosition, bool bResetInterpolation)
{
	THIS_CHECK;

	// Are we spawned?
	if(IsSpawned())
	{
		// Are we not in a vehicle and not entering a vehicle?
		if(!InternalIsInVehicle() && !HasVehicleEnterExit())
		{
			// Set the position in the matrix
			m_pPlayerPed->SetPosition(vecPosition);

			m_pPlayerPed->GetPed()->UpdatePhysicsMatrix(true);

			if(GetHealth() > 0 && IsDying())
				m_pPlayerPed->GetPedTaskManager()->RemoveTask(TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);
		}
	}

	// Reset interpolation if requested
	if(bResetInterpolation)
		ResetInterpolation();
}

void CNetworkPlayer::GetPosition(CVector3& vecPosition)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		// If we are in a vehicle use our vehicles position
		if(m_pVehicle)
			m_pVehicle->GetPosition(vecPosition);
		else
			m_pPlayerPed->GetPosition(vecPosition);
	}
	else
		vecPosition = CVector3();
}

void CNetworkPlayer::SetCurrentHeading(float fHeading)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		float fHeadingRadians = Math::ConvertDegreesToRadians(fHeading);
		m_pPlayerPed->SetCurrentHeading(fHeadingRadians);
		SetDesiredHeading(fHeading);
		m_pPlayerPed->SetHeading(fHeadingRadians);
	}
}

void CNetworkPlayer::SetCurrentSyncHeading(float fHeading)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		// Check if the player has already the same pos
		if(GetCurrentHeading() == fHeading)
			return;

		// Check if the player isn't moving
		CVector3 vecMoveSpeed; m_pPlayerPed->GetMoveSpeed(vecMoveSpeed);
		if(vecMoveSpeed.Length() < 2.0f)
		{
			m_pPlayerPed->SetDesiredHeading(fHeading);
			m_pPlayerPed->SetCurrentHeading(fHeading);
		}
		else if(!m_currentControlState.IsSprinting())
		{
			m_pPlayerPed->SetCurrentHeading(fHeading);
			Sleep(10);
			m_pPlayerPed->SetCurrentHeading(fHeading);
		}
		else
		{
			float fHeadingFinal;
			if(fHeading > GetCurrentHeading())
				fHeadingFinal = fHeading-GetCurrentHeading();

			else if(GetCurrentHeading() > fHeading)
				fHeadingFinal = GetCurrentHeading()-fHeading;

			for(int i = 0; i < 10; i++)
			{
				if(fHeading > GetCurrentHeading())
					m_pPlayerPed->SetCurrentHeading(GetCurrentHeading()+fHeadingFinal/10);
				else if(GetCurrentHeading() > fHeading)
					m_pPlayerPed->SetCurrentHeading(GetCurrentHeading()-fHeadingFinal/10);
			}
		}
	}
}

float CNetworkPlayer::GetCurrentHeading()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return Math::ConvertRadiansToDegrees(m_pPlayerPed->GetCurrentHeading());

	return 0.0f;
}

void CNetworkPlayer::SetDesiredHeading(float fHeading)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->SetDesiredHeading(Math::ConvertDegreesToRadians(fHeading));
}

float CNetworkPlayer::GetDesiredHeading()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return Math::ConvertRadiansToDegrees(m_pPlayerPed->GetDesiredHeading());

	return 0.0f;
}

void CNetworkPlayer::SetBonePosition(CVector3 vecBone)
{
	if(IsSpawned()) 
	{
		//TODO
	}
}

CVector3 CNetworkPlayer::GetBonePosition(int iBone)
{
	if(IsSpawned()) 
	{
		CVector3 vecPos; m_pPlayerPed->GetPosition(vecPos);
		CVector3 vecBone;
		Scripting::GetPedBonePosition(GetScriptingHandle(), (Scripting::ePedBone)iBone, vecPos.fX, vecPos.fY, vecPos.fZ, &vecBone);
		return vecBone;
	}
	return CVector3();
}

void CNetworkPlayer::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->SetMoveSpeed(vecMoveSpeed);
}

void CNetworkPlayer::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetMoveSpeed(vecMoveSpeed);
	else
		vecMoveSpeed = CVector3();
}

void CNetworkPlayer::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->SetTurnSpeed(vecTurnSpeed);
}

void CNetworkPlayer::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetTurnSpeed(vecTurnSpeed);
	else
		vecTurnSpeed = CVector3();
}

void CNetworkPlayer::SetHealth(unsigned int uiHealth)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
		Scripting::SetCharHealth(GetScriptingHandle(), uiHealth);

	// Unlock our health
	m_bHealthLocked = false;
}

void CNetworkPlayer::LockHealth(unsigned int uiHealth)
{
	THIS_CHECK;
	// Set our health
	SetHealth(uiHealth);

	// Set our locked health
	m_uiLockedHealth = uiHealth;

	// Flag our health as locked
	m_bHealthLocked = true;
}

unsigned int CNetworkPlayer::GetHealth()
{
	THIS_CHECK_R(0);
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
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
		Scripting::AddArmourToChar(GetScriptingHandle(), (uiArmour - GetArmour()));

	// Unlock our armour
	m_bArmourLocked = false;
}

void CNetworkPlayer::LockArmour(unsigned int uiArmour)
{
	THIS_CHECK;
	// Set our armour
	SetArmour(uiArmour);

	// Set our locked armour
	m_uiLockedArmour = uiArmour;

	// Flag our armour as locked
	m_bArmourLocked = true;
}

unsigned int CNetworkPlayer::GetArmour()
{
	THIS_CHECK_R(0);
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
	THIS_CHECK;
	if(IsSpawned())
	{
		Scripting::GiveWeaponToChar(GetScriptingHandle(), (Scripting::eWeapon)uiWeaponId, uiAmmo, true);
		/*m_pPlayerPed->GetPedWeapons()->GiveWeapon((eWeaponType)uiWeaponId, uiAmmo);
		m_pPlayerPed->GetPedWeapons()->SetCurrentWeaponVisible(true);*/
	}
}

void CNetworkPlayer::RemoveWeapon(unsigned int uiWeaponId)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->RemoveWeapon((eWeaponType)uiWeaponId);
}

void CNetworkPlayer::RemoveAllWeapons()
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->RemoveAllWeapons();
}

void CNetworkPlayer::SetCurrentWeapon(unsigned int uiWeaponId)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		m_pPlayerPed->GetPedWeapons()->SetCurrentWeaponByType((eWeaponType)uiWeaponId);
		m_pPlayerPed->GetPedWeapons()->SetCurrentWeaponVisible(true);
	}
}

unsigned int CNetworkPlayer::GetCurrentWeapon()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return (unsigned int)m_pPlayerPed->GetPedWeapons()->GetCurrentWeaponType();

	return 0;
}

void CNetworkPlayer::SetAmmo(unsigned int uiWeaponId, unsigned int uiAmmo)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->SetAmmoByType((eWeaponType)uiWeaponId, uiAmmo);
}

unsigned int CNetworkPlayer::GetAmmo(unsigned int uiWeaponId)
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return m_pPlayerPed->GetPedWeapons()->GetAmmoByType((eWeaponType)uiWeaponId);

	return 0;
}

void CNetworkPlayer::GetWeaponInSlot(unsigned int uiWeaponSlot, unsigned int &uiWeaponId, unsigned int &uiAmmo)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		uiWeaponId = (unsigned int)m_pPlayerPed->GetPedWeapons()->GetWeaponInSlot((eWeaponSlot)uiWeaponSlot);
		uiAmmo = m_pPlayerPed->GetPedWeapons()->GetAmmoBySlot((eWeaponSlot)uiWeaponSlot);
	}
}

unsigned int CNetworkPlayer::GetAmmoInClip()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return m_pPlayerPed->GetPedWeapons()->GetAmmoInClip();

	return 0;
}

void CNetworkPlayer::SetAmmoInClip(unsigned int uiAmmoInClip)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->GetPedWeapons()->SetAmmoInClip(uiAmmoInClip);
}

unsigned int CNetworkPlayer::GetMaxAmmoInClip(unsigned int uiWeapon)
{
	THIS_CHECK_R(0);
	if(IsSpawned())
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo((eWeaponType)uiWeapon);

		if(pWeaponInfo)
			return pWeaponInfo->GetWeaponInfo()->m_wClipSize;
	}

	return 0;
}

void CNetworkPlayer::GiveMoney(int iAmount)
{
	THIS_CHECK;
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
	THIS_CHECK;
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
	THIS_CHECK;
	if(IsSpawned())
	{
		m_pPlayerInfo->SetScore(0);
		m_pPlayerInfo->SetDisplayScore(0);
	}
}

int CNetworkPlayer::GetMoney()
{
	THIS_CHECK_R(0);
	if(IsSpawned())
		return m_pPlayerInfo->GetScore();

	return 0;
}

void CNetworkPlayer::SetControlState(CControlState * controlState)
{
	THIS_CHECK;
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

		// Set the last control state
		pPad->SetLastClientControlState(m_currentControlState);

		// Set the current control state
		pPad->SetCurrentClientControlState(*controlState);
	}

	// Copy the current  control state to the previous control state
	memcpy(&m_previousControlState, &m_currentControlState, sizeof(CControlState));

	// Copy the control state to the current control state
	memcpy(&m_currentControlState, controlState, sizeof(CControlState));
}

void CNetworkPlayer::GetPreviousControlState(CControlState * controlState)
{
	THIS_CHECK;
	// Copy the previous control state to the control state
	memcpy(controlState, &m_previousControlState, sizeof(CControlState));
}

void CNetworkPlayer::GetControlState(CControlState * controlState)
{
	THIS_CHECK;
	// Copy the current control state to the control state
	memcpy(controlState, &m_currentControlState, sizeof(CControlState));
}

void CNetworkPlayer::SetArmHeading(const float fArmHeading)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
			m_pContextData->SetArmHeading(fArmHeading);
	}

	m_fArmHeading = fArmHeading;
}

void CNetworkPlayer::GetArmHeading(float& fArmHeading)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
		{
			m_pContextData->GetArmHeading(fArmHeading);
			return;
		}
	}

	fArmHeading = m_fArmHeading;
}

void CNetworkPlayer::SetArmUpDown(const float fArmUpDown)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
			m_pContextData->SetArmUpDown(fArmUpDown);
	}

	m_fArmUpDown = fArmUpDown;
}

void CNetworkPlayer::GetArmUpDown(float& fArmUpDown)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
		{
			m_pContextData->GetArmUpDown(fArmUpDown);
			return;
		}
	}

	fArmUpDown = m_fArmUpDown;
}

void CNetworkPlayer::SetShotSource(const CVector3& vecShotSource)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
			m_pContextData->SetWeaponShotSource(vecShotSource);
	}

	m_vecShotSource = vecShotSource;
}

void CNetworkPlayer::GetShotSource(CVector3& vecShotSource)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
		{
			m_pContextData->GetWeaponShotSource(vecShotSource);
			return;
		}
	}

	vecShotSource = m_vecShotSource;
}

void CNetworkPlayer::SetShotTarget(const CVector3& vecShotTarget)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
			m_pContextData->SetWeaponShotTarget(vecShotTarget);
	}

	m_vecShotTarget = vecShotTarget;
}

void CNetworkPlayer::GetShotTarget(CVector3& vecShotTarget)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Do we have a valid context data pointer?
		if(m_pContextData)
		{
			m_pContextData->GetWeaponShotTarget(vecShotTarget);
			return;
		}
	}

	vecShotTarget = m_vecShotTarget;
}

void CNetworkPlayer::SetAimSyncData(AimSyncData * aimSyncData)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		// Set the arm heading
		SetArmHeading(aimSyncData->fArmHeading);

		// Set the arm up/down
		SetArmUpDown(aimSyncData->fArmUpDown);

		// Set the shot source
		SetShotSource(aimSyncData->vecShotSource);

		// Set the shot target
		SetShotTarget(aimSyncData->vecShotTarget);
	}
}

void CNetworkPlayer::GetAimSyncData(AimSyncData * aimSyncData)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		// Get the arm heading
		GetArmHeading(aimSyncData->fArmHeading);

		// Get the arm up/down
		GetArmHeading(aimSyncData->fArmUpDown);

		// Get the aim source
		GetShotSource(aimSyncData->vecShotSource);

		// Get the shot target
		GetShotTarget(aimSyncData->vecShotTarget);

		// Get the look at pos
		g_pClient->GetCamera()->GetLookAt(aimSyncData->vecLookAt);
	}
}

void CNetworkPlayer::AddToWorld()
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->AddToWorld();
}

void CNetworkPlayer::RemoveFromWorld(bool bStopMoving)
{
	THIS_CHECK;
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
	THIS_CHECK;
	if(IsSpawned())
	{
		Scripting::GivePedHelmet(GetScriptingHandle());
		m_bHelmet = true;
	}
}

void CNetworkPlayer::RemoveHelmet()
{
	THIS_CHECK;
	if(IsSpawned())
	{
		Scripting::RemovePedHelmet(GetScriptingHandle(),true);
		m_bHelmet = false;
	}
}

// TODO: Don't use natives for this
void CNetworkPlayer::SetInterior(unsigned int uiInterior)
{
	THIS_CHECK;
	if(IsSpawned() && GetInterior() != uiInterior)
		Scripting::SetRoomForCharByKey(GetScriptingHandle(), (Scripting::eInteriorRoomKey)uiInterior);
}

// TODO: Don't use natives for this
unsigned int CNetworkPlayer::GetInterior()
{
	THIS_CHECK_R(0);
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
	THIS_CHECK;
	if(HasTargetPosition())
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();

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

		// Get our position
		CVector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

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
	}
}

void CNetworkPlayer::UpdateTargetRotation()
{
	THIS_CHECK;
	if(HasTargetRotation())
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Get the factor of time spent from the interpolation start
		// to the current time.
		float fAlpha = Math::Unlerp(m_interp.rot.ulStartTime, ulCurrentTime, m_interp.rot.ulFinishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_interp.rot.fLastAlpha);
		m_interp.rot.fLastAlpha = fAlpha;

		// Apply the error compensation
		float fCompensation = Math::Lerp(0.0f, fCurrentAlpha, m_interp.rot.fError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.0f)
			m_interp.rot.ulFinishTime = 0;

		// Get our rotation
		float fCurrentHeading = GetCurrentHeading();

		// Calculate the new rotation
		float fNewRotation = (fCurrentHeading + fCompensation);

		// Set our new rotation
		SetCurrentHeading(fNewRotation);
	}
}

void CNetworkPlayer::Interpolate()
{
	THIS_CHECK;
	// Are we not getting in/out of a vehicle?
	// jenksta: why is this vehicle entry/exit check disabled?
	if(true)
	{
		UpdateTargetPosition();
		UpdateTargetRotation();
	}
}

void CNetworkPlayer::SetTargetPosition(const CVector3 &vecPosition, unsigned long ulDelay)
{
	THIS_CHECK;
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

void CNetworkPlayer::SetMoveToDirection(CVector3 vecPos, CVector3 vecMove, int iMoveType)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		float tX = (vecPos.fX + (vecMove.fX * 10));
		float tY = (vecPos.fY + (vecMove.fY * 10));
		float tZ = (vecPos.fZ + (vecMove.fZ * 10));
		unsigned int uiPlayerIndex = GetScriptingHandle();
		//Create the task
		DWORD dwAddress = (CGame::GetBase() + 0xB87480);
		_asm
		{
			push 1000
			push iMoveType
			push tZ
			push tY
			push tX
			push uiPlayerIndex
			call dwAddress
			add  esp, 18h
		}
	}
}	

void CNetworkPlayer::SetTargetRotation(float fHeading, unsigned long ulDelay)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Update our target rotation
		UpdateTargetRotation();

		// Get our rotation
		float fCurrentHeading = GetCurrentHeading();

		// Set the target heading
		m_interp.rot.fTarget = fHeading;

		// Calculate the relative error
		m_interp.rot.fError = (fHeading - fCurrentHeading);

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
	THIS_CHECK;
	m_interp.pos.ulFinishTime = 0;
}

void CNetworkPlayer::RemoveTargetRotation()
{
	THIS_CHECK;
	m_interp.rot.ulFinishTime = 0;
}

void CNetworkPlayer::ResetInterpolation()
{
	THIS_CHECK;
	RemoveTargetPosition();
	RemoveTargetRotation();
}

void CNetworkPlayer::SetColor(unsigned int uiColor)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerInfo->SetColour(uiColor);

	m_uiColor = uiColor;
}

unsigned int CNetworkPlayer::GetColor()
{
	THIS_CHECK_R(0);
	return m_uiColor;
}

void CNetworkPlayer::SetClothes(unsigned char ucBodyPart, unsigned char ucClothes)
{
	THIS_CHECK;
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
	THIS_CHECK_R(0);
	if(ucBodyPart > 10)
		return 0;

	return m_ucClothes[ucBodyPart];
}

void CNetworkPlayer::SetDucking(bool bDucking)
{
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->SetDucking(bDucking);
}

bool CNetworkPlayer::IsDucking()
{
	THIS_CHECK_R(false);
	if(IsSpawned())
		return m_pPlayerPed->IsDucking();

	return false;
}

void CNetworkPlayer::SetCameraBehind()
{
	THIS_CHECK;
	if(IsSpawned())
		g_pClient->GetCamera()->SetBehindPed(m_pPlayerPed);
}

void CNetworkPlayer::Pulse()
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
		// Is this the local player?
		if(IsLocalPlayer())
		{
			// Copy the current control state to the previous control state
			memcpy(&m_previousControlState, &m_currentControlState, sizeof(CControlState));

			// Update the current control state
			CGame::GetPad()->GetCurrentClientControlState(m_currentControlState);
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

			// Check if our car is death
			if(m_bVehicleDeathCheck)
			{
				if(m_pVehicle)
				{
					if(m_pVehicle->GetDriver() == NULL)
					{
						if(Scripting::IsCarDead(m_pVehicle->GetScriptingHandle()) || (m_pVehicle->GetVehicleModelType() < 116 && Scripting::IsCarInWater(m_pVehicle->GetScriptingHandle()) && CGame::GetSpecialData(1)))
						{
							CBitStream bsDeath;
							bsDeath.Write(m_pVehicle->GetVehicleId());
							g_pClient->GetNetworkManager()->RPC(RPC_ScriptingVehicleDeath, &bsDeath, PRIORITY_HIGH, RELIABILITY_UNRELIABLE_SEQUENCED);
							m_bVehicleDeathCheck = false;
						}
					}
					else
						m_bVehicleDeathCheck = false;
				}
				else
					m_bVehicleDeathCheck = false;
			}
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
	THIS_CHECK;
	m_strName = strName;

	if(!CGame::GetNameTags())
	{
		Scripting::RemoveFakeNetworkNameFromPed(GetScriptingHandle());
		char red = (GetColor() & 0xFF000000) >> 24;
		char green = (GetColor() & 0x00FF0000) >> 16;
		char blue = (GetColor() & 0x0000FF00) >> 8;
		char alpha = (GetColor() & 0x000000FF);
		Scripting::GivePedFakeNetworkName(GetScriptingHandle(),(GetName() + String(" (%i)",this->GetPlayerId())).Get(),red, green, blue, alpha);
		//Scripting::GivePedFakeNetworkName(GetScriptingHandle(),m_strName.C_String(),m_uiColor,m_uiColor,m_uiColor,m_uiColor); 
	}

	if(m_pPlayerInfo)
		m_pPlayerInfo->SetName(strName.GetData());
}

bool CNetworkPlayer::IsGettingInToAVehicle()
{
	THIS_CHECK_R(false);
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
	THIS_CHECK_R(false);
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
	THIS_CHECK_R(false);
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
	THIS_CHECK_R(false);
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
	THIS_CHECK_R(false);
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
	THIS_CHECK_R(false);
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

bool CNetworkPlayer::ClearDieTask()
{
	THIS_CHECK_R(false);
	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_DIE)
			{
				m_pPlayerPed->GetPedTaskManager()->RemoveTask(TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);
				return true;
			}
		}
	}

	return false;
}


bool CNetworkPlayer::GetClosestVehicle(bool bPassenger, CNetworkVehicle ** pVehicle, BYTE &byteSeatId)
{
	THIS_CHECK_R(false);
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
		std::list<CStreamableEntity *> * streamedVehicles = g_pClient->GetStreamer()->GetStreamedInEntitiesOfType(STREAM_ENTITY_VEHICLE);

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
				//CLogFile::Printf("GetClosestVehicleSet %d(%d)",i,pClosestVehicle->GetMaxPassengers());
				if(pClosestVehicle->GetPassenger(i) == NULL)
				{
					//CLogFile::Printf("SEAT FOUND!!(%d)",i);
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

void CNetworkPlayer::EnterVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
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
				g_pClient->GetStreamer()->ForceStreamIn(pVehicle);
			}
		}

		// Are we already in a vehicle?
		if(IsInVehicle())
			return;

		// Is the vehicle streamed in?
		//CLogFile::Printf("[DEBUG] Try to enter vehicle %d with door lock state %d",pVehicle->GetVehicleId(),pVehicle->GetDoorLockState());
		if(pVehicle->IsStreamedIn() && pVehicle->GetDoorLockState() == 0)
		{
			// Create the enter vehicle task
			int iUnknown = -4;

			if(byteSeatId == 0)
				iUnknown = -7;
			else if(byteSeatId == 1)
				iUnknown = 2;
			else if(byteSeatId == 2)
				iUnknown = 1;
			else if(byteSeatId == 3)
				iUnknown = 3;

			unsigned int uiUnknown = 0;

			if(byteSeatId > 0)
				uiUnknown |= 0x2000000u;

			CIVTaskComplexNewGetInVehicle * pTask = new CIVTaskComplexNewGetInVehicle(pVehicle->GetGameVehicle(), iUnknown, 27, uiUnknown, 0.0f);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
			}

			// Restore engine
			if(byteSeatId == 0)
				pVehicle->SetEngineState(pVehicle->GetEngineState());

			// Mark ourselves as entering a vehicle and store our vehicle and seat
			m_vehicleEnterExit.bEntering = true;
			m_vehicleEnterExit.pVehicle = pVehicle;
			m_vehicleEnterExit.byteSeatId = byteSeatId;

			// Reset interpolation
			ResetInterpolation();
		}
	}
}

void CNetworkPlayer::ExitVehicle(eExitVehicleMode exitmode)
{
	THIS_CHECK;

	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle?
		if(m_pVehicle)
		{
			/* iExitMode values - 0xF - Get out animation (used when exiting a non-moving vehicle)
								- 0x9C4 - Get out animation (used when smb jacks your vehicle).
								- 0x40B - Dive out animation (used in trucks).
								- 0x100E - Dive out animation (used in the other vehicles). */

			// Simulate the way GTA IV handles vehicle exits.
			CVector3 vecMoveSpeed;
			int modelId;
			int iExitMode = 0xF; 

			m_pVehicle->GetMoveSpeed(vecMoveSpeed);
			modelId = g_pClient->GetModelManager()->ModelHashToVehicleId(m_pVehicle->GetModelInfo()->GetHash());

			if(exitmode == EXIT_VEHICLE_NORMAL)
			{
				if(vecMoveSpeed.fX < -10 || vecMoveSpeed.fX > 10 || vecMoveSpeed.fY < -10 || vecMoveSpeed.fY > 10)
				{
					switch(modelId)
					{
					case 2: case 4: case 5: case 7: case 8: case 10: case 11:
					case 31: case 32: case 49: case 50: case 51: case 52:
					case 53: case 55: case 56: case 60: case 66: case 73:
					case 85: case 86: case 94: case 104:
						iExitMode = 0x40B;
						break;

					default:
						{
							if(modelId != 12 && modelId < 166)
								iExitMode = 0x100E;
						}
					}
				}
			}
			else
				iExitMode = 0x9C4;

			// Create the vehicle exit task.
			CIVTaskComplexNewExitVehicle * pTask = new CIVTaskComplexNewExitVehicle(m_pVehicle->GetGameVehicle(), iExitMode, 0, 0);

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

		if(m_pVehicle && (m_pVehicle->GetHealth() < 0 || m_pVehicle->GetPetrolTankHealth() < 0.0f))
		{
			m_bVehicleDeathCheck = true;
			if(Scripting::IsCarDead(m_pVehicle->GetScriptingHandle()) || (Scripting::IsCarInWater(m_pVehicle->GetScriptingHandle()) && CGame::GetSpecialData(1)))
			{
				CBitStream bsDeath;
				bsDeath.Write(m_pVehicle->GetVehicleId());
				g_pClient->GetNetworkManager()->RPC(RPC_ScriptingVehicleDeath, &bsDeath, PRIORITY_HIGH, RELIABILITY_UNRELIABLE_SEQUENCED);
				m_bVehicleDeathCheck = false;
			}
		}

		// Reset Driver
		THIS_CHECK;
		if(m_pVehicle)
			m_pVehicle->SetDriver(NULL);

		// Reset interpolation
		ResetInterpolation();
	}
}

void CNetworkPlayer::PutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId)
{
	THIS_CHECK;
	// Are we spawned?
	if(IsSpawned())
	{
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
				g_pClient->GetStreamer()->ForceStreamIn(pVehicle);
			}
			else
				return;
		}

		// Are we already in a vehicle?
		if(IsInVehicle())
		{
			// Remove ourselves from our current vehicle
			RemoveFromVehicle();
		}

		// Internally put ourselves into the vehicle
		if(pVehicle->IsStreamedIn())
			InternalPutInVehicle(pVehicle, byteSeatId);

		// Reset vehicle entry/exit
		ResetVehicleEnterExit();
		m_pVehicle = pVehicle;
		m_pVehicle->SetDamageable(true);
		m_byteVehicleSeatId = byteSeatId;
		pVehicle->SetOccupant(byteSeatId, this);

		// Is this a network vehicle?
		if(m_pVehicle->IsNetworkVehicle())
		{
			// Send the network rpc
			CBitStream bitStream;
			bitStream.WriteCompressed(GetPlayerId());
			bitStream.Write((BYTE)VEHICLE_ENTRY_COMPLETE);
			bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
			bitStream.Write(m_byteVehicleSeatId);
			g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
		}
	}
}

void CNetworkPlayer::RemoveFromVehicle()
{
	THIS_CHECK;
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
	THIS_CHECK;
	// Are we spawned and is input enabled and are our controls not disabled?
	if(IsSpawned() && CGame::GetInputState() && !g_bControlsDisabled)
	{
		// Has the enter/exit vehicle key just been pressed?
		if(m_currentControlState.IsUsingEnterExitVehicle() && !m_previousControlState.IsUsingEnterExitVehicle())
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
						bitStream.WriteCompressed(GetPlayerId());
						bitStream.Write((BYTE)VEHICLE_EXIT_REQUEST);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
						m_vehicleEnterExit.bRequesting = true;
					}
					else
					{
						// Exit the vehicle
						ExitVehicle(EXIT_VEHICLE_NORMAL);
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

			if(m_previousControlState.IsUsingEnterExitVehicle() && !m_currentControlState.IsUsingEnterExitVehicle())
				bEnterExitVehicleKeyReleased = true;

			// Has the horn key just been released?
			bool bHornKeyReleased = false;

			if(m_previousControlState.IsUsingHorn() && !m_currentControlState.IsUsingHorn())
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
						if(bFound && pVehicle && pVehicle->IsSpawned())
						{
							if(IsLocalPlayer())
								CLogFile::Printf("HandleVehicleEntry(LocalPlayer, %d, %d, %d)", pVehicle->GetVehicleId(), byteSeatId, pVehicle->GetDoorLockState());
							else
								CLogFile::Printf("HandleVehicleEntry(%d, %d, %d, %d)", m_playerId, pVehicle->GetVehicleId(), byteSeatId, pVehicle->GetDoorLockState());

							if(pVehicle->GetDoorLockState() > 0)
							{
								m_vehicleEnterExit.bRequesting = false;
								m_vehicleEnterExit.bEntering = false;
								return;
							}

							// Is this a network vehicle?
							if(pVehicle->IsNetworkVehicle())
							{
								// Request the vehicle entry
								CBitStream bsSend;
								bsSend.WriteCompressed(GetPlayerId());
								bsSend.Write((BYTE)VEHICLE_ENTRY_REQUEST);
								bsSend.WriteCompressed(pVehicle->GetVehicleId());
								bsSend.Write(byteSeatId);
								g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE);
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
	THIS_CHECK;
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
					m_pVehicle->SetDamageable(true);
					m_byteVehicleSeatId = m_vehicleEnterExit.byteSeatId;
					m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, this);
					m_vehicleEnterExit.pVehicle = NULL;

					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.WriteCompressed(GetPlayerId());
						bitStream.Write((BYTE)VEHICLE_ENTRY_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						bitStream.Write(m_byteVehicleSeatId);
						g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Restore engine state
					if(m_pVehicle->GetEngineState())
						m_pVehicle->SetEngineState(true);

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
							m_vehicleEnterExit.pVehicle->SetDamageable(false);

							// Send the network rpc
							CBitStream bitStream;
							bitStream.WriteCompressed(GetPlayerId());
							bitStream.Write((BYTE)VEHICLE_ENTRY_CANCELLED);
							bitStream.WriteCompressed(m_vehicleEnterExit.pVehicle->GetVehicleId());
							bitStream.Write(m_byteVehicleSeatId);
							g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);

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
						bitStream.WriteCompressed(GetPlayerId());
						bitStream.Write((BYTE)VEHICLE_EXIT_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Vehicle exit is complete
					m_vehicleEnterExit.bExiting = false;
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle->SetDamageable(false);
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
						bitStream.WriteCompressed(GetPlayerId());
						bitStream.Write((BYTE)VEHICLE_EXIT_FORCEFUL);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pClient->GetNetworkManager()->RPC(RPC_VehicleEnterExit, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Player has forcefully exited the vehicle (out of windscreen, e.t.c.)
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle->SetDamageable(false);
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
	THIS_CHECK;
	// Reset the vehicle enter/exit flags
	RemoveFromVehicle();

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
	THIS_CHECK;
	if(IsSpawned())
		m_pPlayerPed->SetRagdoll(bToggle);
}

bool CNetworkPlayer::IsOnScreen()
{
	THIS_CHECK_R(false);
	// Are we spawned?
	if(IsSpawned()) 
	{
		return true;
	}
	return false;
}

void CNetworkPlayer::SetHelmet(bool bHelmet)
{
	THIS_CHECK;
	if(bHelmet)
		Scripting::GivePedHelmet(GetScriptingHandle());
	if(!bHelmet)
		Scripting::RemovePedHelmet(GetScriptingHandle(),true); // no animation

	m_bHelmet = bHelmet;
}

void CNetworkPlayer::TaskLookAtCoord(float fX, float fY, float fZ)
{
	THIS_CHECK;
	if(IsSpawned())
	{
		CVector3 vecLookAt(fX, fY, fZ);
		CIVTaskSimpleTriggerLookAt * pTask = new CIVTaskSimpleTriggerLookAt(NULL, TICK_RATE, -1, &vecLookAt);

		// Did the task create successfully?
		if(pTask)
		{
			// Set it as the ped task
			pTask->SetAsPedTaskSecondary(m_pPlayerPed, TASK_SECONDARY_PARTIAL_ANIM);
		}
	}
}

void CNetworkPlayer::UseMobilePhone(bool bUse)
{
	THIS_CHECK;
	if(IsSpawned())
		Scripting::TaskUseMobilePhone(GetScriptingHandle(),bUse);

	m_bUseMobilePhone = bUse;
}

void CNetworkPlayer::Jump(bool bMoving)
{
	if(IsSpawned())
	{
		// Create the jump task
		CIVTaskComplexJump * pTask = new CIVTaskComplexJump((bMoving ? 0x40 : 0), 0);

		// Did the task create successfully?
		if(pTask)
		{
			// Set it as the ped task
			pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
		}
	}
}