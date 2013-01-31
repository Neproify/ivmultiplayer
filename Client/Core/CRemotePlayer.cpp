//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CRemotePlayer.cpp
// Project: Client.Core
// Author(s): jenksta
//            TrojaA
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CRemotePlayer.h"
#include "CClient.h"
#include "Scripting.h"
#include "CGame.h"
#include "CPlayerManager.h"

extern CClient * g_pClient;

CRemotePlayer::CRemotePlayer()
	: CNetworkPlayer(),
	m_vehicleId(INVALID_ENTITY_ID),
	m_stateType(STATE_TYPE_DISCONNECT),
	m_bPassenger(false),
	m_pLastSyncData(NULL)
{
}

CRemotePlayer::~CRemotePlayer()
{
	Destroy();
}

bool CRemotePlayer::Spawn(int iModelId, CVector3 vecSpawnPos, float fSpawnHeading, bool bDontRecreate)
{
	if(!bDontRecreate)
	{
		if(IsSpawned())
			return false;
		
		g_pClient->GetStreamer()->ForceStreamIn(this);
		SetCanBeStreamedIn(true);

		if(!IsSpawned())
		{
			g_pClient->GetChatWindow()->AddNetworkMessage("[NETWORK] Failed to spawn remote player %s.", GetName().Get());
			return false;
		}

		/*if(!Create())
			return false;
		else
			m_bIsStreamedIn = true;*/
	}

	Teleport(vecSpawnPos);
	SetCurrentHeading(fSpawnHeading);
	Init();
	return true;
}

void CRemotePlayer::Destroy()
{
	CNetworkPlayer::Destroy();
}

void CRemotePlayer::Kill()
{
	CNetworkPlayer::Kill();

	if(IsSpawned())
		m_stateType = STATE_TYPE_DEATH;
}

void CRemotePlayer::Init()
{
	if(IsSpawned())
	{
		ToggleRagdoll(false);
		SetColor(GetColor());
		Scripting::SetCharWillFlyThroughWindscreen(GetScriptingHandle(), false);
		Scripting::SetPedDiesWhenInjured(GetScriptingHandle(), false);
		Scripting::SetCharInvincible(GetScriptingHandle(), true);
		// These two will be useful for a setPlayerUseModelAnims native
		//Scripting::SetAnimGroupForChar(m_pedIndex, "move_player");
		//Scripting::SetCharGestureGroup(m_pedIndex, "GESTURES@MALE");
		Scripting::BlockCharHeadIk(GetScriptingHandle(), true);

		m_stateType = STATE_TYPE_SPAWN;
	}
}

void CRemotePlayer::StoreOnFootSync(OnFootSyncData * syncPacket, bool bHasAimSyncData)
{
	// Check if the player isn't available
	if(!g_pClient->GetPlayerManager()->IsActive(GetPlayerId()))
		return;
	
	// If we are in a vehicle remove ourselves from it
	if(IsInVehicle())
		RemoveFromVehicle();

	// If we have new sync data -> continue otherwise we don't need to update it
	/*if(m_pLastSyncData != syncPacket)
		m_pLastSyncData = syncPacket;
	else
		return;*/

#if 0
	if(!bHasAimSyncData)
	{
		if(syncPacket->vecMoveSpeed.Length() < 0.75)
		{
			SetTargetPosition(syncPacket->vecPos,TICK_RATE*2);
			SetCurrentSyncHeading(syncPacket->fHeading);

			if(m_iOldMoveStyle != 0) {
				// Stop wakling, stand still and delete tasks
				unsigned int uiPlayerIndex = GetScriptingHandle();
				DWORD dwAddress = (CGame::GetBase() + 0x8067A0);
				_asm
				{
					push 17
					push 0
					push uiPlayerIndex
					call dwAddress
				}
				/*dwAddress = (CGame::GetBase() + 0xB868E0);
				_asm
				{
					push 1
					push uiPlayerIndex
					call dwAddress
				}*/
			}
			SetMoveSpeed(syncPacket->vecMoveSpeed);
			SetTurnSpeed(syncPacket->vecTurnSpeed);
			m_iOldMoveStyle = 0;
		}
		else if(syncPacket->vecMoveSpeed.Length() < 3.0 && syncPacket->vecMoveSpeed.Length() >= 0.75) {
			SetTargetPosition(syncPacket->vecPos,TICK_RATE);
			SetMoveToDirection(syncPacket->vecPos, syncPacket->vecMoveSpeed, 2);
			m_iOldMoveStyle = 1;
		}
		else if(syncPacket->vecMoveSpeed.Length() < 5.0 && syncPacket->vecMoveSpeed.Length() > 3.0) {
			SetTargetPosition(syncPacket->vecPos,TICK_RATE);
			SetMoveToDirection(syncPacket->vecPos, syncPacket->vecMoveSpeed, 3);
			m_iOldMoveStyle = 2;
		}
		else {
			SetTargetPosition(syncPacket->vecPos, (TICK_RATE/4)*3);
			SetMoveToDirection(syncPacket->vecPos, syncPacket->vecMoveSpeed, 4);
			m_iOldMoveStyle = 3;
		}
	}
	else
	{
		// If we start aiming/shoting, stop walking, set stand still and delete task
		if(!m_bStoreOnFootSwitch) {
			m_bStoreOnFootSwitch = true;
			unsigned int uiPlayerIndex = GetScriptingHandle();
			DWORD dwAddress = (CGame::GetBase() + 0x8067A0);
			_asm
			{
				push 17
				push 0
				push uiPlayerIndex
				call dwAddress
			}
			uiPlayerIndex = GetScriptingHandle();
			dwAddress = (CGame::GetBase() + 0xB868E0);
			_asm
			{
				push 1
				push uiPlayerIndex
				call dwAddress
			}
		}
#endif
		SetTargetPosition(syncPacket->vecPos, TICK_RATE);
		SetTargetRotation(syncPacket->fHeading, TICK_RATE);
		SetMoveSpeed(syncPacket->vecMoveSpeed);
		SetTurnSpeed(syncPacket->vecTurnSpeed);
#if 0
	}

	// Simulate jump(while walking/running)
	if(!m_bStoreOnFootSwitch && syncPacket->vecMoveSpeed.Length() > 1.0 && syncPacket->controlState.IsJumping())
	{
		// Start the jump task
		Jump(true);
	}
#endif

	// Set our control state
	SetControlState(&syncPacket->controlState);

	// Set our ducking state
	SetDucking(syncPacket->bDuckState);

	// Lock our health
	LockHealth(syncPacket->uHealthArmour >> 16);

	// Lock our armour
	LockArmour((syncPacket->uHealthArmour << 16) >> 16);

	// Get our new weapon and ammo
	unsigned int uiWeapon = (syncPacket->uWeaponInfo >> 20);
	unsigned int uiAmmo = ((syncPacket->uWeaponInfo << 12) >> 12);

	// Do we not have the right weapon equipped?
	if(GetCurrentWeapon() != uiWeapon)
	{
		// Set our current weapon
		GiveWeapon(uiWeapon, uiAmmo);
	}

	// Do we not have the right ammo?
	if(GetAmmo(uiWeapon) != uiAmmo)
	{
		// Set our ammo
		SetAmmo(uiWeapon, uiAmmo);
	}

	m_stateType = STATE_TYPE_ONFOOT;
}

void CRemotePlayer::StoreInVehicleSync(EntityId vehicleId, InVehicleSyncData * syncPacket)
{
	// Check if the player isn't available
	if(!g_pClient->GetPlayerManager()->IsActive(GetPlayerId()))
		return;

	// Get the vehicle
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	// Does the vehicle exist and is it spawned?
	if(pVehicle && pVehicle->IsStreamedIn())
	{
		// Store the vehicle id
		m_vehicleId = vehicleId;

		// Get our local player
		CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

		// If they have just warped into the vehicle and the local player
		// is driving it eject the local player
		if(m_stateType != STATE_TYPE_INVEHICLE && pVehicle->GetDriver() == pLocalPlayer)
		{
			pLocalPlayer->RemoveFromVehicle();
			g_pClient->GetChatWindow()->AddInfoMessage("Car Jacked");
		}

		// If they are not in the vehicle put them in it
		if(GetVehicle() != pVehicle)
		{
			if(pLocalPlayer->GetVehicle() == pVehicle && !pLocalPlayer->IsAPassenger())
				pLocalPlayer->RemoveFromVehicle();

			PutInVehicle(pVehicle, 0);
		}

		// Set their control state
		SetControlState(&syncPacket->controlState);

		// Set their vehicles target position
		pVehicle->SetTargetPosition(syncPacket->vecPos, TICK_RATE/**2*/);

		// Set their vehicles target rotation
		pVehicle->SetTargetRotation(syncPacket->vecRotation, TICK_RATE/**2*/);

		// Set their quaternion
		//pVehicle->SetQuaternion(syncPacket->fQuaternion);
		
		// Check if we have no bike(otherwise -> shake shake, shake shake shake IT! :P)
		if(pVehicle->GetVehicleModelType() < 105 || pVehicle->GetVehicleModelType() > 111)
		{
			// Set their vehicles turn speed
			pVehicle->SetTurnSpeed(syncPacket->vecTurnSpeed);
		}

		// Set their vehicles move speed
		pVehicle->SetMoveSpeed(syncPacket->vecMoveSpeed);

		// Set their vehicles health
		pVehicle->SetHealth(syncPacket->uiHealth);
		pVehicle->SetPetrolTankHealth(syncPacket->fPetrolHealth);

		// Set their door states
		for(int i = 0; i <= 5; i++)
		{
			if(pVehicle->GetCarDoorAngle(i) != syncPacket->fDoor[i])
				pVehicle->SetCarDoorAngle(i,false,syncPacket->fDoor[i]);
		}

		// Check if color set is needed
		BYTE byteColors[4];
		pVehicle->GetColors(byteColors[0],byteColors[1],byteColors[2],byteColors[3]);
		for(int i = 0; i < 4; i++)
		{
			if(syncPacket->byteColors[i] != byteColors[i])
			{
				// Set their vehicles color
				pVehicle->SetColors(syncPacket->byteColors[0], syncPacket->byteColors[1], syncPacket->byteColors[2], syncPacket->byteColors[3]);
				break;
			}
			else
				continue;
		}

		// Set their vehicles siren state
		if(pVehicle->GetSirenState() != syncPacket->bSirenState)
			pVehicle->SetSirenState(syncPacket->bSirenState);
		
		// Set their windows
		for(int i = 0; i <= 3; i++)
			pVehicle->SetWindowState(i, syncPacket->bWindow[i]);

		// Set their typres
		for(int i = 0; i <= 5; i++)
		{
			if(syncPacket->bTyre[i])
				Scripting::BurstCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)i);
		}

		// Set their vehicles dirt level
		if(pVehicle->GetDirtLevel() != syncPacket->fDirtLevel)
			pVehicle->SetDirtLevel(syncPacket->fDirtLevel);

		// Set their vehicles engine status
		pVehicle->SetEngineState(syncPacket->bEngineStatus);

		// Set their lights
		if(pVehicle->GetLightsState() != syncPacket->bLights)
			pVehicle->SetLightsState(syncPacket->bLights);

		// Lock our health
		LockHealth(syncPacket->uPlayerHealthArmour >> 16);

		// Lock our armour
		LockArmour((syncPacket->uPlayerHealthArmour << 16) >> 16);

		// Get our new weapon and ammo
		unsigned int uiWeapon = (syncPacket->uPlayerWeaponInfo >> 20);
		unsigned int uiAmmo = ((syncPacket->uPlayerWeaponInfo << 12) >> 12);

		// Do we not have the right weapon equipped?
		if(GetCurrentWeapon() != uiWeapon)
		{
			// Set our current weapon
			GiveWeapon(uiWeapon, uiAmmo);
		}

		// Do we not have the right ammo?
		if(GetAmmo(uiWeapon) != uiAmmo)
		{
			// Set our ammo
			SetAmmo(uiWeapon, uiAmmo);
		}

	}
	else if(pVehicle && !(pVehicle->IsStreamedIn()))
	{
		//set the vehicle position
		pVehicle->SetPosition(syncPacket->vecPos, false, true);

		//set the player position
		SetPosition(syncPacket->vecPos, true);
	}

	m_stateType = STATE_TYPE_INVEHICLE;
}

void CRemotePlayer::StorePassengerSync(EntityId vehicleId, PassengerSyncData * syncPacket)
{
	// Check if the player isn't avaiable(disconnect etc)
	if(!g_pClient->GetPlayerManager()->IsActive(GetPlayerId()))
		return;

	// Get the vehicle
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	// Does the vehicle exist and is it spawned?
	if(pVehicle && pVehicle->IsStreamedIn())
	{
		// Store the vehicle id
		m_vehicleId = vehicleId;

		// If they are not in the vehicle put them in it
		if(GetVehicle() != pVehicle || GetVehicleSeatId() != syncPacket->byteSeatId)
			PutInVehicle(pVehicle, syncPacket->byteSeatId);

		// Set their control state
		SetControlState(&syncPacket->controlState);

		// Lock our health
		LockHealth(syncPacket->uPlayerHealthArmour >> 16);

		// Lock our armour
		LockArmour((syncPacket->uPlayerHealthArmour << 16) >> 16);

		// Get our new weapon and ammo
		unsigned int uiWeapon = (syncPacket->uPlayerWeaponInfo >> 20);
		unsigned int uiAmmo = ((syncPacket->uPlayerWeaponInfo << 12) >> 12);

		// Do we not have the right weapon equipped?
		if(GetCurrentWeapon() != uiWeapon)
		{
			// Set our current weapon
			GiveWeapon(uiWeapon, uiAmmo);
		}

		// Do we not have the right ammo?
		if(GetAmmo(uiWeapon) != uiAmmo)
		{
			// Set our ammo
			SetAmmo(uiWeapon, uiAmmo);
		}
	}

	m_stateType = STATE_TYPE_PASSENGER;
}

void CRemotePlayer::StoreSmallSync(SmallSyncData * syncPacket)
{
	// Check if the player isn't avaiable(disconnect etc)
	if(!g_pClient->GetPlayerManager()->IsActive(GetPlayerId()))
		return;

	// Set their control state
	SetControlState(&syncPacket->controlState);

	// Set their ducking state
	SetDucking(syncPacket->bDuckState);

	// Get our new weapon and ammo
	unsigned int uiWeapon = (syncPacket->uWeaponInfo >> 20);
	unsigned int uiAmmo = ((syncPacket->uWeaponInfo << 12) >> 12);

	// Do we not have the right weapon equipped?
	if(GetCurrentWeapon() != uiWeapon)
	{
		// Set our current weapon
		GiveWeapon(uiWeapon, uiAmmo);
	}

	// Do we not have the right ammo?
	if(GetAmmo(uiWeapon) != uiAmmo)
	{
		// Set our ammo
		SetAmmo(uiWeapon, uiAmmo);
	}
}

void CRemotePlayer::SetColor(unsigned int uiColor)
{
	CNetworkPlayer::SetColor(uiColor);

	if(!CGame::GetNameTags())
	{
		Scripting::RemoveFakeNetworkNameFromPed(GetScriptingHandle());

		char red = (uiColor & 0xFF000000) >> 24;
		char green = (uiColor & 0x00FF0000) >> 16;
		char blue = (uiColor & 0x0000FF00) >> 8;
		char alpha = (uiColor & 0x000000FF);
		Scripting::GivePedFakeNetworkName(GetScriptingHandle(),(GetName() + String(" (%i)",this->GetPlayerId())).Get(),red, green, blue, alpha);
	}

	if(GetBlipActivity())
		Scripting::ChangeBlipColour(GetBlip(), uiColor);
}
