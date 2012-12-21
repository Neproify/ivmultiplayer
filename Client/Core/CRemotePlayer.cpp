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
#include "CChatWindow.h"
#include "CVehicleManager.h"
#include "CPlayerManager.h"
#include "Scripting.h"
#include "CLocalPlayer.h"
#include "CGame.h"
#include "CPlayerManager.h"

extern CChatWindow * g_pChatWindow;
extern CVehicleManager * g_pVehicleManager;
extern CLocalPlayer * g_pLocalPlayer;
extern CPlayerManager * g_pPlayerManager;

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

		if(!Create())
			return false;
		else
			this->m_bIsStreamedIn = true;
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

		if(!CGame::GetNameTags()) {
			char red = (GetColor() & 0xFF000000) >> 24;
			char green = (GetColor() & 0x00FF0000) >> 16;
			char blue = (GetColor() & 0x0000FF00) >> 8;
			char alpha = (GetColor() & 0x000000FF);
			Scripting::GivePedFakeNetworkName(GetScriptingHandle(),(GetName() + String(" (%i)",this->GetPlayerId())).Get(),red, green, blue, alpha);
			//Scripting::GivePedFakeNetworkName(GetScriptingHandle(),GetName().Get(),((BYTE*)GetColor())[0],((BYTE*)GetColor())[1],((BYTE*)GetColor())[2],((BYTE*)GetColor())[3]);
		}
		// These two will be useful for a setPlayerUseModelAnims native
		//Scripting::SetAnimGroupForChar(m_pedIndex, "move_player");
		//Scripting::SetCharGestureGroup(m_pedIndex, "GESTURES@MALE");
		Scripting::BlockCharHeadIk(GetScriptingHandle(), true);

		m_stateType = STATE_TYPE_SPAWN;
	}
}

void CRemotePlayer::StoreOnFootSync(OnFootSyncData * syncPacket)
{
	// Check if the player isn't avaiable(disconnect etc)
	if(!g_pPlayerManager->IsActive(GetPlayerId()))
		return;
	
	// If we are in a vehicle remove ourselves from it
	if(IsInVehicle())
		RemoveFromVehicle();

	// Set our control state
	SetControlState(&syncPacket->controlState);

	// If we have new sync data -> continue otherwise we don't need to update it
	/*if(m_pLastSyncData != syncPacket)
		m_pLastSyncData = syncPacket;
	else
		return;*/

	// Set our position
	SetTargetPosition(syncPacket->vecPos, TICK_RATE*2);

	// Set our heading
	SetCurrentHeading(syncPacket->fHeading);

	// Set our move speed
	SetMoveSpeed(syncPacket->vecMoveSpeed);

	// Set our turn speed
	SetTurnSpeed(syncPacket->vecTurnSpeed);

	// Set our ducking state
	SetDucking(syncPacket->bDuckState);

	// Lock our health
	LockHealth(syncPacket->uHealthArmour >> 16);

	// Lock our armour
	LockArmour((syncPacket->uHealthArmour << 16) >> 16);

	// Set our anim stuff
	//if(syncPacket->bAnim)
	//{
	//	m_strAnimGroup = syncPacket->szAnimGroup;
	//	m_strAnimSpec = syncPacket->szAnimSpecific;

	//	// check if we're not animating and the animation isn't finished(1.0)
	//	if(!m_bAnimating && syncPacket->fAnimTime < 1.0f)
	//	{
	//		m_bAnimating = true;
	//		Scripting::TaskPlayAnim(GetScriptingHandle(),m_strAnimSpec.C_String(),m_strAnimGroup.C_String(),8.0f,0,0,0,0,-1);
	//		Scripting::SetCharAnimCurrentTime(GetScriptingHandle(),m_strAnimGroup.C_String(),m_strAnimSpec.C_String(),syncPacket->fAnimTime);
	//	}
	//	else if(m_bAnimating)
	//		Scripting::SetCharAnimCurrentTime(GetScriptingHandle(),m_strAnimGroup.C_String(),m_strAnimSpec.C_String(),syncPacket->fAnimTime);
	//}
	//else
	//	m_bAnimating = false;

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
	// Check if the player isn't avaiable(disconnect etc)
	if(!g_pPlayerManager->IsActive(GetPlayerId()))
		return;

	// Get the vehicle
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	// Does the vehicle exist and is it spawned?
	if(pVehicle && pVehicle->IsStreamedIn())
	{
		// Store the vehicle id
		m_vehicleId = vehicleId;

		// If they have just warped into the vehicle and the local player
		// is driving it eject the local player
		if(m_stateType != STATE_TYPE_INVEHICLE && pVehicle->GetDriver() == g_pLocalPlayer)
		{
			g_pLocalPlayer->RemoveFromVehicle();
			g_pChatWindow->AddInfoMessage("Car Jacked");
		}

		// If they are not in the vehicle put them in it
		if(GetVehicle() != pVehicle)
		{
			if(g_pLocalPlayer->GetVehicle() == pVehicle && !g_pLocalPlayer->IsAPassenger())
				g_pLocalPlayer->RemoveFromVehicle();

			PutInVehicle(pVehicle, 0);
		}

		// Set their control state
		SetControlState(&syncPacket->controlState);

		// Set their vehicles target position
		pVehicle->SetTargetPosition(syncPacket->vecPos, TICK_RATE/**2*/);

		// Set their vehicles target rotation
		pVehicle->SetTargetRotation(syncPacket->vecRotation, TICK_RATE/**2*/);
		
		// Check if we have no bike(otherwise -> shake shake, shake shake shake IT! :P)
		if(pVehicle->GetModelInfo()->GetIndex() < 105 || pVehicle->GetModelInfo()->GetIndex() > 111)
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
		/*
		// Set their windows
		for(int i = 0; i <= 3; i++)
			pVehicle->SetWindowState(i, syncPacket->bWindow[i]);

		// Set their typres
		for(int i = 0; i <= 5; i++)
		{
			if(syncPacket->bTyre[i])
				Scripting::BurstCarTyre(pVehicle->GetScriptingHandle(),(Scripting::eVehicleTyre)i);
		}*/

		// Set their vehicles dirt level
		if(pVehicle->GetDirtLevel() != syncPacket->fDirtLevel)
			pVehicle->SetDirtLevel(syncPacket->fDirtLevel);

		// Set their vehicles engine status
		if(pVehicle->GetEngineState() != syncPacket->bEngineStatus)
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
	if(!g_pPlayerManager->IsActive(GetPlayerId()))
		return;

	// Get the vehicle
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

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
	if(!g_pPlayerManager->IsActive(GetPlayerId()))
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
