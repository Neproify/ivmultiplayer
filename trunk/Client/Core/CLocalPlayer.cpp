//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CLocalPlayer.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Leopard
//            MaVe
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CLocalPlayer.h"
#include "CPlayerManager.h"
#include "CNetworkManager.h"
#include "CVehicleManager.h"
#include "CStreamer.h"
#include "CChatWindow.h"
#include "CInputWindow.h"
#include "KeySync.h"
#include "Patcher/CPatcher.h"
#include "CCutsceneInteriors.h"
#include "COffsets.h"
#include "CCamera.h"

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager  * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CStreamer       * g_pStreamer;
extern CChatWindow     * g_pChatWindow;
extern CInputWindow    * g_pInputWindow;
extern CCamera         * g_pCamera;
extern bool              m_bControlsDisabled;

void * pAddress = NULL;
void * pReturnAddress = NULL;

extern CLocalPlayer * g_pLocalPlayer;

void GetLocalPlayerSpawnPosition(int, CVector3 * vecSpawnPosition, float * fAngle)
{
	_asm
	{
		pushad
	}

	// Get the saved spawn position
	g_pLocalPlayer->GetSpawnPosition(vecSpawnPosition);

	// Get the desired angle
	*fAngle = g_pLocalPlayer->GetSpawnRotation();

	_asm
	{
		popad
	}
}

// Params seem to be IVPed * ped, Vector3 position, float rotation
// (could prolly skip the GetLocalPlayerSpawnPosition but I didn't get it to work)
void __declspec(naked) HandleLocalPlayerSpawn()
{
	_asm
	{
		pushad
	}

	g_pLocalPlayer->HandleSpawn();

	_asm
	{
		popad
		jmp COffsets::FUNC_SpawnPlayer
	}
}

CLocalPlayer::CLocalPlayer() : CNetworkPlayer(true)
{
	m_bIsDead = false;
	m_bToggleControl = true;
	memset(&m_vecSpawnPosition, 0, sizeof(CVector3));
	m_fSpawnAngle = 0;
	m_ulLastPureSyncTime = 0;
	m_uiLastInterior = 0;
	m_bDisableVehicleInfo = false;
	m_bAnimating = false;
	memset(&m_lastControlStateSent, 0, sizeof(CControlState));
	
	// Patch to override spawn position and let the game call HandleSpawn
	CPatcher::InstallCallPatch(COffsets::FUNC_GetLocalPlayerSpawnPosition, (DWORD)GetLocalPlayerSpawnPosition, 5);
	CPatcher::InstallCallPatch(COffsets::CALL_SpawnLocalPlayer, (DWORD)HandleLocalPlayerSpawn, 5);
}

CLocalPlayer::~CLocalPlayer()
{

}

void CLocalPlayer::Respawn()
{
	// Set the local player state to respawning
	*(DWORD *)COffsets::VAR_LocalPlayerState = 5;
}

void CLocalPlayer::HandleSpawn()
{
	CLogFile::Printf("HandleSpawn(LocalPlayer)");

	// Enable input if needed
	if(!g_pInputWindow->IsEnabled() && !m_bControlsDisabled)
		CGame::SetInputState(true);

	// Reset vehicle entry/exit flags
	ResetVehicleEnterExit();

	// Enable our controls
	SetPlayerControlAdvanced(true, true);

	// Enable the HUD
	CGame::SetHudVisible(true);

	// Enable the radar
	CGame::SetRadarVisible(true);

	// Enable the area names
	CGame::SetAreaNamesEnabled(true);

	// Reset the camera
	g_pCamera->Reset();

	// Send the spawn notification to the server
	CBitStream bsSend;
	bsSend.Write(ModelHashToSkinId(GetModelInfo()->GetHash()));
	g_pNetworkManager->RPC(RPC_PlayerSpawn, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);

	// Flag us as alive
	m_bIsDead = false;
}

void CLocalPlayer::DoDeathCheck()
{
	// Have we not yet processed the death and is the local player dead?
	if(!m_bIsDead && IsDead())
	{
		// Get the kill info
		EntityId playerId = INVALID_ENTITY_ID;
		EntityId vehicleId = INVALID_ENTITY_ID;
		GetKillInfo(&playerId, &vehicleId);

		CLogFile::Printf("HandleDeath(LocalPlayer, %d, %d)", playerId, vehicleId);

		// Send the death notification to the server
		CBitStream bsSend;
		bsSend.WriteCompressed(playerId);
		bsSend.WriteCompressed(vehicleId);
		g_pNetworkManager->RPC(RPC_Death, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);

		// Mark ourselves as dead
		m_bIsDead = true;

		// Reset vehicle entry/exit flags
		ResetVehicleEnterExit();
	}
}

void CLocalPlayer::Pulse()
{
	CNetworkPlayer::Pulse();

	if(g_pNetworkManager->IsConnected())
	{
		if(IsSpawned())
		{
			// Do the death check
			DoDeathCheck();

			// Are we not dead?
			if(!m_bIsDead)
			{
				// Is a pure sync needed and are we not getting in/out of a vehicle?
				if(IsPureSyncNeeded() && !HasVehicleEnterExit())
				{
					// Are we not in a vehicle?
					if(!IsInVehicle())
					{
						// Send on foot sync
						SendOnFootSync();
						// TODO: Sync head movement for remote players?
					}
					else
					{
						// Are we not a passenger
						if(!IsAPassenger())
						{
							// Send in vehicle sync
							SendInVehicleSync();
						}
						else
						{
							// Send passenger sync
							SendPassengerSync();
						}
					}
				}
				else
				{
					// Is a small sync needed?
					if(IsSmallSyncNeeded())
					{
						// Send small sync
						SendSmallSync();
					}
				}

				// Have we changed interiors?
				if(GetInterior() != m_uiLastInterior)
				{
					// Set the new interior
					m_uiLastInterior = GetInterior();

					// Check for cut scene interiors
					CCutsceneInteriors::Update(m_uiLastInterior);

					// Update all interiors
					g_pStreamer->UpdateInterior(m_uiLastInterior);
				}
			}
		}
	}
}

void CLocalPlayer::SetSpawnLocation(CVector3 vecPosition, float fHeading)
{
	m_vecSpawnPosition = vecPosition;
	m_fSpawnAngle = fHeading;
}

void CLocalPlayer::SetPlayerControlAdvanced(bool bControl, bool bCamera)
{
	Scripting::SetPlayerControlAdvanced(GetGamePlayerNumber(), bControl, bControl, bControl);
	Scripting::SetCameraControlsDisabledWithPlayerControls(bCamera);
}

void CLocalPlayer::SendOnFootSync()
{
	CBitStream bsSend;
	OnFootSyncData syncPacket;

	// Get our control state
	GetControlState(&syncPacket.controlState);

	// Update the last sent control state
	memcpy(&m_lastControlStateSent, &syncPacket.controlState, sizeof(CControlState));

	// Get their position
	GetPosition(syncPacket.vecPos);

	// Get their heading
	syncPacket.fHeading = GetCurrentHeading();

	// Get their move speed
	GetMoveSpeed(syncPacket.vecMoveSpeed);

	// Get their ducking state
	syncPacket.bDuckState = IsDucking();

	// Get their health and armour
	syncPacket.uHealthArmour = ((GetHealth() << 16) | GetArmour());

	// Set default animation stuff
	syncPacket.bAnim = false;

	// Check for anims
	if(m_bAnimating)
	{
		if(!Scripting::HasCharAnimFinished(g_pLocalPlayer->GetScriptingHandle(),m_strAnimGroup))
		{
			float fTime;
			syncPacket.bAnim = true;
			syncPacket.szAnimGroup = m_strAnimGroup;
			syncPacket.szAnimSpecific = m_strAnimSpec;
			Scripting::GetCharAnimCurrentTime(g_pLocalPlayer->GetScriptingHandle(),m_strAnimGroup,m_strAnimSpec,&fTime);
			syncPacket.fAnimTime = fTime;
		}
		else
			m_bAnimating = false;
	}

	// Get their current weapon and ammo
	unsigned int uiCurrentWeapon = GetCurrentWeapon();
	syncPacket.uWeaponInfo = ((uiCurrentWeapon << 20) | GetAmmo(uiCurrentWeapon));

	// Write the on foot sync data to the bit stream
	bsSend.Write((char *)&syncPacket, sizeof(OnFootSyncData));

	// Check if they are aiming or firing
	// NOTE: Do i need to sync aim for combat too?
	if(syncPacket.controlState.IsAiming() || syncPacket.controlState.IsFiring())
	{
		// Write a 1 bit to say we have aim sync
		bsSend.Write1();

		// Get their aim sync data
		AimSyncData aimSyncPacket;
		GetAimSyncData(&aimSyncPacket);

		// Write the aim sync data to the bit stream
		bsSend.Write((char *)&aimSyncPacket, sizeof(AimSyncData));
	}
	else
	{
		// Write a 0 bit to say we don't have aim sync
		bsSend.Write0();
	}

	g_pNetworkManager->RPC(RPC_OnFootSync, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
}

void CLocalPlayer::SendInVehicleSync()
{
	CNetworkVehicle * pVehicle = GetVehicle();

	if(pVehicle && pVehicle->IsStreamedIn())
	{
		CBitStream bsSend;
		InVehicleSyncData syncPacket;

		// Write the vehicle id
		bsSend.WriteCompressed(pVehicle->GetVehicleId());

		// Get our control state
		GetControlState(&syncPacket.controlState);

		// Update the last sent control state
		//memcpy(&m_lastControlStateSent, &syncPacket.controlState, sizeof(CControlState)); // Don't activate, will destroy the control state sync ;)

		// Get their vehicles position
		pVehicle->GetPosition(syncPacket.vecPos);

		// Get their vehicles rotation
		pVehicle->GetRotation(syncPacket.vecRotation);

		// Get their vehicles colors
		pVehicle->GetColors(syncPacket.byteColors[0], syncPacket.byteColors[1], syncPacket.byteColors[2], syncPacket.byteColors[3]);

		// Get their vehicles siren state
		syncPacket.bSirenState = pVehicle->GetSirenState();

		// Get their vehicles turn speed
		pVehicle->GetTurnSpeed(syncPacket.vecTurnSpeed);

		// Get their vehicles move speed
		pVehicle->GetMoveSpeed(syncPacket.vecMoveSpeed);

		// Get their vehicles engine health & petroltank health
		syncPacket.uiHealth = pVehicle->GetHealth();
		syncPacket.fPetrolHealth = (float)pVehicle->GetPetrolTankHealth();

		// Get their vehicles dirt level
		syncPacket.fDirtLevel = pVehicle->GetDirtLevel();

		// Get their lights
		syncPacket.bLights = pVehicle->GetLights();

		// Get the door stuff
		syncPacket.fDoor[0] = pVehicle->GetCarDoor(0);
		syncPacket.fDoor[1] = pVehicle->GetCarDoor(1);
		syncPacket.fDoor[2] = pVehicle->GetCarDoor(2);
		syncPacket.fDoor[3] = pVehicle->GetCarDoor(3);
		syncPacket.fDoor[4] = pVehicle->GetCarDoor(4);
		syncPacket.fDoor[5] = pVehicle->GetCarDoor(5);

		// Get their health and armour
		syncPacket.uPlayerHealthArmour = ((GetHealth() << 16) | GetArmour());

		// Get their current weapon and ammo
		unsigned int uCurrentWeapon = GetCurrentWeapon();
		syncPacket.uPlayerWeaponInfo = ((uCurrentWeapon << 20) | GetAmmo(uCurrentWeapon));

		// Get their vehicles engine status (untested)
		syncPacket.bEngineStatus = pVehicle->GetEngineState();
		
		// Set default window and typres values
		syncPacket.bWindow[0] = false;
		syncPacket.bWindow[1] = false;
		syncPacket.bWindow[2] = false;
		syncPacket.bWindow[3] = false;
		syncPacket.bTyre[0] = false;
		syncPacket.bTyre[1] = false;
		syncPacket.bTyre[2] = false;
		syncPacket.bTyre[3] = false;
		syncPacket.bTyre[4] = false;
		syncPacket.bTyre[5] = false;

		// Check windows
		if(!Scripting::IsVehWindowIntact(pVehicle->GetScriptingHandle(),(Scripting::eVehicleWindow)0))
		{
			pVehicle->SetWindow(0,true);
			syncPacket.bWindow[1] = true;
		}
		if(!Scripting::IsVehWindowIntact(pVehicle->GetScriptingHandle(),(Scripting::eVehicleWindow)1))
		{
			pVehicle->SetWindow(1,true);
			syncPacket.bWindow[2] = true;
		}
		if(!Scripting::IsVehWindowIntact(pVehicle->GetScriptingHandle(),(Scripting::eVehicleWindow)2))
		{
			pVehicle->SetWindow(2,true);
			syncPacket.bWindow[3] = true;
		}
		if(!Scripting::IsVehWindowIntact(pVehicle->GetScriptingHandle(),(Scripting::eVehicleWindow)3))
		{
			pVehicle->SetWindow(3,true);
			syncPacket.bWindow[4] = true;
		}

		// Check tyres
		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)0))
			syncPacket.bTyre[0] = true;

		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)1))
			syncPacket.bTyre[1] = true;

		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)2))
			syncPacket.bTyre[2] = true;

		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)3))
			syncPacket.bTyre[3] = true;

		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)4))
			syncPacket.bTyre[4] = true;

		if(!Scripting::IsCarTyreBurst(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)5))
			syncPacket.bTyre[5] = true;

		// Write the in vehicle sync data to the bit stream
		bsSend.Write((char *)&syncPacket, sizeof(InVehicleSyncData));

		// Check if they are doing a drive by
		// TODO: Also sync aim for turret rotation on firetrucks?
		// (Or find turret rotation var in vehicle structure)
		if(syncPacket.controlState.IsDoingDriveBy())
		{
			// Write a 1 bit to say we have aim sync
			bsSend.Write1();

			// Get their aim sync data
			AimSyncData aimSyncPacket;
			GetAimSyncData(&aimSyncPacket);

			// Write the aim sync data to the bit stream
			bsSend.Write((char *)&aimSyncPacket, sizeof(AimSyncData));
		}
		else
		{
			// Write a 0 bit to say we don't have aim sync
			bsSend.Write0();
		}

		g_pNetworkManager->RPC(RPC_InVehicleSync, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);

		if(Scripting::IsCarDead(pVehicle->GetScriptingHandle()))
		{
			g_pChatWindow->AddInfoMessage("VEHICLE DEATH");
			CBitStream bsDeath;
			bsDeath.Write(pVehicle->GetVehicleId());
			g_pNetworkManager->RPC(RPC_ScriptingVehicleDeath, &bsDeath, PRIORITY_HIGH, RELIABILITY_UNRELIABLE_SEQUENCED);
		}
	}
}

void CLocalPlayer::SendPassengerSync()
{
	CNetworkVehicle * pVehicle = GetVehicle();

	if(pVehicle && pVehicle->IsStreamedIn())
	{
		// TODO: Add position/rotation/move speed to passenger sync data? (only if we have no driver?)
		CBitStream bsSend;
		PassengerSyncData syncPacket;

		// Write the vehicle id
		bsSend.WriteCompressed(pVehicle->GetVehicleId());

		// Get our control state
		GetControlState(&syncPacket.controlState);

		// Update the last sent control state
		memcpy(&m_lastControlStateSent, &syncPacket.controlState, sizeof(CControlState));

		// Get their seat id
		syncPacket.byteSeatId = GetVehicleSeatId();

		// Get their health and armour
		syncPacket.uPlayerHealthArmour = ((GetHealth() << 16) | GetArmour());

		// Get their current weapon and ammo
		unsigned int uCurrentWeapon = GetCurrentWeapon();
		syncPacket.uPlayerWeaponInfo = ((uCurrentWeapon << 20) | GetAmmo(uCurrentWeapon));

		// Write the passenger sync data to the bit stream
		bsSend.Write((char *)&syncPacket, sizeof(PassengerSyncData));

		// Check if they are doing a drive by
		// NOTE: I think certain vehicles (e.g. helicoptors) allow 3rd person
		// shooting from them which involves the on foot aim/fire keys so add
		// them to this check if needed
		if(syncPacket.controlState.IsDoingDriveBy())
		{
			// Write a 1 bit to say we have aim sync
			bsSend.Write1();

			// Get their aim sync data
			AimSyncData aimSyncPacket;
			GetAimSyncData(&aimSyncPacket);

			// Write the aim sync data to the bit stream
			bsSend.Write((char *)&aimSyncPacket, sizeof(AimSyncData));
		}
		else
		{
			// Write a 0 bit to say we don't have aim sync
			bsSend.Write0();
		}

		g_pNetworkManager->RPC(RPC_PassengerSync, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
	}
}

void CLocalPlayer::SendSmallSync()
{
	CBitStream bsSend;
	SmallSyncData syncPacket;

	// Get our control state
	GetControlState(&syncPacket.controlState);

	// Update the last sent control state
	memcpy(&m_lastControlStateSent, &syncPacket.controlState, sizeof(CControlState));

	// Get their ducking state
	syncPacket.bDuckState = IsDucking();

	// Get their current weapon and ammo
	unsigned int uCurrentWeapon = GetCurrentWeapon();
	syncPacket.uWeaponInfo = ((uCurrentWeapon << 20) | GetAmmo(uCurrentWeapon));

	// Write the small key sync data to the bit stream
	bsSend.Write((char *)&syncPacket, sizeof(SmallSyncData));

	// Check if they are aiming or firing
	if(syncPacket.controlState.IsAiming() || syncPacket.controlState.IsFiring())
	{
		// Write a 1 bit to say we have aim sync
		bsSend.Write1();

		// Get their aim sync data
		AimSyncData aimSyncPacket;
		GetAimSyncData(&aimSyncPacket);

		// Write the aim sync data to the bit stream
		bsSend.Write((char *)&aimSyncPacket, sizeof(AimSyncData));
	}
	else
	{
		// Write a 0 bit to say we don't have aim sync
		bsSend.Write0();
	}

	g_pNetworkManager->RPC(RPC_SmallSync, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
}

bool CLocalPlayer::IsPureSyncNeeded()
{
	// Get the current time
	unsigned long ulCurrentTime = SharedUtility::GetTime();

	// Has it been TICK_RATE or more ms since our last pure sync?
	if(ulCurrentTime >= (m_ulLastPureSyncTime + TICK_RATE))
	{
		// Update the last pure sync time
		m_ulLastPureSyncTime = ulCurrentTime;
		return true;
	}

	return false;
}

bool CLocalPlayer::IsSmallSyncNeeded()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Get the current control state
		CControlState controlState;
		GetControlState(&controlState);

		// Is the current control state different to the last sent control state?
		if(controlState != m_lastControlStateSent)
			return true;
	}

	return false;
}

unsigned short CLocalPlayer::GetPing()
{
	return (unsigned short)g_pNetworkManager->GetNetClient()->GetLastPing();
}

void CLocalPlayer::SetControl(bool control)
{
	m_bToggleControl = control;
}

bool CLocalPlayer::GetControl()
{
	return m_bToggleControl;
}

void CLocalPlayer::SetAnimation(const char * strGroup, const char * strAnim)
{
	m_bAnimating = true;
	Scripting::TaskPlayAnim(GetScriptingHandle(),strAnim, strGroup,(float)8,0,0,0,0,-1);
	m_strAnimGroup = strGroup;
	m_strAnimSpec = strAnim;
}
