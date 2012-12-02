//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "CActorManager.h"
#include "CChatWindow.h"
#include "CModelmanager.h"
#include "CVehicleManager.h"
#include "CNetworkManager.h"
#include <CLogfile.h>

extern CModelManager * g_pModelManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;

CActorManager::CActorManager()
	: bGameFocused(true), 
	bMenuFocused(false)
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		m_bActive[x] = false;
		m_Actors[x].bRender = false;
		m_Actors[x].bFailedEnterVehicle = false;
	}
}

CActorManager::~CActorManager()
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(m_bActive[x])
			Delete(x);
	}
}

void CActorManager::Create(EntityId actorId, int iModelId, CVector3 vecPosition, float fHeading, String strName, bool bTogglename, unsigned int iColor, bool bFrozen, bool bHelmet, bool bBlip)
{
	if(m_bActive[actorId])
		Delete(actorId);

	// Get the model hash
	DWORD dwModelHash = SkinIdToModelHash(iModelId);

	// Get the model index
	int iModelIndex = CGame::GetStreaming()->GetModelIndexFromHash(dwModelHash);

	// Get the model info
	CIVModelInfo * pModelInfo = CGame::GetModelInfo(iModelIndex);

	// Do we have a valid model info?
	if(pModelInfo)
	{
		// Add our model info reference
		pModelInfo->AddReference(true);
	}

	// Set the spawn location
	memcpy(&m_Actors[actorId].vecPosition,&vecPosition,sizeof(vecPosition));

	// Set the model info
	m_Actors[actorId].pModelInfo = pModelInfo;

	CreateChar(1, (Scripting::eModel)dwModelHash, vecPosition.fX, vecPosition.fY, vecPosition.fZ, &m_Actors[actorId].uiActorIndex, true);
	Scripting::SetBlockingOfNonTemporaryEvents(m_Actors[actorId].uiActorIndex, true);
	Scripting::SetCharInvincible(m_Actors[actorId].uiActorIndex, true);
	Scripting::SetCharHealth(m_Actors[actorId].uiActorIndex, 200);
	Scripting::AddArmourToChar(m_Actors[actorId].uiActorIndex, 200);
	Scripting::SetCharDefaultComponentVariation(m_Actors[actorId].uiActorIndex);
	//Scripting::AddArmourToChar(m_Actors[actorId].uiActorIndex, 200);
	Scripting::SetCharHeading(m_Actors[actorId].uiActorIndex, fHeading);
	Scripting::AllowReactionAnims(m_Actors[actorId].uiActorIndex,false);

	if(bFrozen)
		Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,true);
	if(bHelmet)
		Scripting::GivePedHelmet(m_Actors[actorId].uiActorIndex);

	if(!CGame::GetNameTags())
	{
		if(bBlip)
		{
			Scripting::AddBlipForChar(m_Actors[actorId].uiActorIndex, &m_Actors[actorId].uiBlipId);
			Scripting::ChangeBlipSprite(m_Actors[actorId].uiBlipId, Scripting::BLIP_OBJECTIVE);
			Scripting::ChangeBlipScale(m_Actors[actorId].uiBlipId, 0.5);
			Scripting::ChangeBlipNameFromAscii(m_Actors[actorId].uiBlipId, m_Actors[actorId].strName.Get());
		}
		Scripting::RemoveFakeNetworkNameFromPed(m_Actors[actorId].uiActorIndex);
		Scripting::GivePedFakeNetworkName(m_Actors[actorId].uiActorIndex,strName.Get(),255,255,255,255);
	}
	else
	{
		m_Actors[actorId].strName = strName;
		m_Actors[actorId].iNametagColor = iColor;
	}
	m_Actors[actorId].bNametag = true;
	m_bActive[actorId] = true;

	// Check position for freeze char
	bool m_bSpawned = false;
	CVector3 spawnControl;

	while(!m_bSpawned)
	{
		Scripting::GetCharCoordinates(m_Actors[actorId].uiActorIndex,&spawnControl.fX,&spawnControl.fY,&spawnControl.fZ);
		if((m_Actors[actorId].vecPosition.fZ - spawnControl.fZ) < 0.1f)
		{
			Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,true);
			Scripting::SetCharCoordinates(m_Actors[actorId].uiActorIndex,vecPosition.fX,vecPosition.fY,(vecPosition.fZ - 1.0f));
			m_bSpawned = true;
		}
	}

}

bool CActorManager::Delete(EntityId actorId)
{
	if(!m_bActive[actorId])
	{
		CLogFile::Printf("Tried to delete inexistent Actor %d", actorId);
		return false;
	}

	Scripting::DeleteChar(&m_Actors[actorId].uiActorIndex);

	// Do we have a valid model info?
	if(m_Actors[actorId].pModelInfo)
	{
		// Remove our model info reference
		m_Actors[actorId].pModelInfo->RemoveReference();
	}

	m_bActive[actorId] = false;
	m_Actors[actorId].bRender = false;
	return true;
}

void CActorManager::SetPosition(EntityId actorId, CVector3 vecPosition)
{
	if(m_bActive[actorId])
		Scripting::SetCharCoordinates(m_Actors[actorId].uiActorIndex, vecPosition.fX, vecPosition.fY, vecPosition.fZ);
}

void CActorManager::SetHeading(EntityId actorId, float fHeading)
{
	if(m_bActive[actorId])
		Scripting::SetCharHeading(m_Actors[actorId].uiActorIndex, fHeading);
}

CVector3 CActorManager::GetPosition(EntityId actorId)
{	
	float vecPosition1;
	float vecPosition2;
	float vecPosition3;
	
	Scripting::GetCharCoordinates(m_Actors[actorId].uiActorIndex,&vecPosition1,&vecPosition2,&vecPosition3);

	if(m_bActive[actorId])
	{
		m_Actors[actorId].vecPosition = CVector3(vecPosition1,vecPosition2,vecPosition3); 
		return CVector3(vecPosition1,vecPosition2,vecPosition3);
	}

	return CVector3(0.0f, 0.0f, 0.0f);
}

void CActorManager::SetName(EntityId actorId, String strName)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].strName = strName;
		if(!CGame::GetNameTags()) {
			Scripting::ChangeBlipNameFromAscii(m_Actors[actorId].uiBlipId, m_Actors[actorId].strName.C_String());
			Scripting::RemoveFakeNetworkNameFromPed(m_Actors[actorId].uiActorIndex);
			Scripting::GivePedFakeNetworkName(m_Actors[actorId].uiActorIndex,strName.Get(),255,255,255,255);
		}
	}
}

bool CActorManager::ToggleNametag(EntityId actorId, bool bShow)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].bNametag = bShow;
		return true;
	}
	return false;
}

bool CActorManager::ToggleBlip(EntityId actorId, bool bShow)
{
	if(m_bActive[actorId])
	{
		if(bShow)
			Scripting::AddBlipForChar(m_Actors[actorId].uiActorIndex, &m_Actors[actorId].uiBlipId);
		else if(!bShow)
			Scripting::RemoveBlip(m_Actors[actorId].uiBlipId);
		
		m_Actors[actorId].bBlip = bShow;
		return true;
	}
	return false;
}

void CActorManager::SetColor(EntityId actorId, unsigned int iColor)
{
	if(m_bActive[actorId])
		m_Actors[actorId].iNametagColor = iColor;
}

bool CActorManager::ToggleFrozen(EntityId actorId, bool bFrozen)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].bFrozen = bFrozen;
		Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,bFrozen);
		return true;
	}
	return false;
}

bool CActorManager::ToggleHelmet(EntityId actorId, bool bHelmet)
{
	if(m_bActive[actorId])
	{
		Scripting::EnablePedHelmet(m_Actors[actorId].uiActorIndex, bHelmet);
		m_Actors[actorId].bHelmet = bHelmet;
		return true;
	}
	return false;
}

void CActorManager::WarpIntoVehicle(EntityId actorId, EntityId vehicleId, int iSeatId)
{
	if(m_bActive[actorId] && g_pVehicleManager->Exists(vehicleId))
	{
		CVector3 vecPos, vecCarPos; 

		Scripting::GetCarCoordinates(g_pVehicleManager->Get(vehicleId)->GetScriptingHandle(),&vecCarPos.fX,&vecCarPos.fY,&vecCarPos.fZ);

		Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,false);
		Scripting::SetCharCoordinates(m_Actors[actorId].uiActorIndex,vecCarPos.fX,(vecCarPos.fY+2.0f), vecCarPos.fZ);

		if(iSeatId > 0 && iSeatId <= 3 && vehicleId != -1)
		{
			Scripting::WarpCharIntoCarAsPassenger(m_Actors[actorId].uiActorIndex, g_pVehicleManager->Get(vehicleId)->GetScriptingHandle(), (iSeatId - 1));
			m_Actors[actorId].vehicleId = vehicleId;
			m_Actors[actorId].iSeatid = iSeatId;
			m_Actors[actorId].bStateincar = true;
		}
		else if(iSeatId == 0 && vehicleId != -1)
		{
			Scripting::WarpCharIntoCar(m_Actors[actorId].uiActorIndex, g_pVehicleManager->Get(vehicleId)->GetScriptingHandle());
			m_Actors[actorId].vehicleId = vehicleId;
			m_Actors[actorId].iSeatid = iSeatId;
			m_Actors[actorId].bStateincar = true;
		}
	}
}

void CActorManager::RemoveFromVehicle(EntityId actorId)
{
	if(m_bActive[actorId] && g_pVehicleManager->Exists(m_Actors[actorId].vehicleId))
	{
		Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,true);
		if(m_Actors[actorId].bStateincar)
		{
			Scripting::TaskLeaveCar(m_Actors[actorId].uiActorIndex, g_pVehicleManager->Get(m_Actors[actorId].vehicleId)->GetScriptingHandle());
			m_Actors[actorId].vehicleId = -1;
			m_Actors[actorId].iSeatid = -1;
			m_Actors[actorId].bStateincar = false;
		}
	}
}

bool CActorManager::IsCharOnScreen(EntityId actorId)
{
	if(m_bActive[actorId])
	{
		if(Scripting::IsCharOnScreen(m_Actors[actorId].uiActorIndex))
			return true;

		return false;
	}
	return false;
}

unsigned int CActorManager::GetScriptingHandle(EntityId actorId)
{
	if(m_bActive[actorId])
		return m_Actors[actorId].uiActorIndex;

 	return -1;
}

void CActorManager::ForceAnimation(EntityId actorId, const char * szGroup, const char * szAnim)
{
	if(m_bActive[actorId])
		Scripting::TaskPlayAnim(GetScriptingHandle(actorId),szAnim,szGroup,float(8),0,0,0,0,-1);
}

float CActorManager::GetHealth(EntityId actorId)
{
	if(m_bActive[actorId])
	{
		unsigned int health;
		Scripting::GetCharHealth(m_Actors[actorId].uiActorIndex,&health);
		return (float)health;
	}

	return 0.0f;
}

float CActorManager::GetArmour(EntityId actorId)
{
	if(m_bActive[actorId])
	{
		unsigned int armour;
		Scripting::GetCharArmour(m_Actors[actorId].uiActorIndex,&armour);
		return (float)armour;
	}

	return 0.0f;
}

void CActorManager::DriveToPoint(EntityId actorId, EntityId vehicleId, CVector3 vecPos, CVector3 vecRot, CVector3 vecFinalPos, bool bDrive)
{
	if(m_bActive[actorId])
	{
		if(bDrive)
		{
			// Apply coords
			m_Actors[actorId].vecDriveFinalPos = vecFinalPos;
			g_pVehicleManager->Get(m_Actors[actorId].vehicleId)->GetPosition(vecPos);
			m_Actors[actorId].vecDrivePos = vecPos;
			m_Actors[actorId].vehicleId = vehicleId;

			if(!g_pVehicleManager->Exists(vehicleId) || !g_pVehicleManager->Get(vehicleId)->IsSpawned() || !g_pVehicleManager->Get(vehicleId)->IsStreamedIn())
			{	
				m_Actors[actorId].bFailedEnterVehicle = true;
			}
			else
			{
				Scripting::FreezeCharPosition(m_Actors[actorId].uiActorIndex,false);
				WarpIntoVehicle(actorId,m_Actors[actorId].vehicleId, m_Actors[actorId].iSeatid);
				
				g_pVehicleManager->Get(vehicleId)->SetPosition(vecPos);

				g_pVehicleManager->Get(vehicleId)->GetRotation(vecRot);
				g_pVehicleManager->Get(vehicleId)->SetRotation(vecRot);

				Scripting::TaskCarDriveToCoord(m_Actors[actorId].uiActorIndex,g_pVehicleManager->Get(vehicleId)->GetScriptingHandle(),vecFinalPos.fX,vecFinalPos.fY,vecFinalPos.fZ, 10.0f, 0, 0, 0, 5.0f, -1);
				m_Actors[actorId].bRender = true;
			}
		}
		else
			m_Actors[actorId].bRender = false;
	}
}
CBitStream		bsSend;
float			fHeading;
unsigned int	uiVehicleHandle;
void CActorManager::Process()
{
	// Check if we were in the menu or on desk and if, get the latest coords
	if(!bGameFocused)
	{
		if(CGame::IsFocused())
		{
			for(EntityId i = 0; i < MAX_ACTORS; i++)
			{
				if(m_bActive[i] && m_Actors[i].bRender)
				{
					bsSend.Reset();

					bsSend.Write(i);
					g_pNetworkManager->RPC(RPC_RequestActorUpdate, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
				}
			}
		}
	}
	if(bMenuFocused)
	{
		if(!CGame::IsMenuActive())
		{
			for(EntityId i = 0; i < MAX_ACTORS; i++)
			{
				if(m_bActive[i] && m_Actors[i].bRender)
				{
					bsSend.Reset();

					bsSend.Write(i);
					g_pNetworkManager->RPC(RPC_RequestActorUpdate, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
				}
			}
		}
	}

	if(CGame::IsMenuActive())
		bMenuFocused = true;

	for(EntityId i = 0; i < MAX_ACTORS; i++)
	{
		if(m_bActive[i] && m_Actors[i].bRender)
		{
			if(g_pVehicleManager->Exists(m_Actors[i].vehicleId))
			{
				// Reset stuff
				bsSend.Reset();
				fHeading = 0.0f;
				uiVehicleHandle = -1;

				CNetworkVehicle * pVehicle = g_pVehicleManager->Get(m_Actors[i].vehicleId);
				if(!pVehicle)
					return;

				if(!pVehicle->IsSpawned() || !pVehicle->IsStreamedIn())
					return;

				ActorSyncData actorSync;
				uiVehicleHandle = g_pVehicleManager->Get(m_Actors[i].vehicleId)->GetScriptingHandle();

				actorSync.actorId = i;
				actorSync.vehicleId = m_Actors[i].vehicleId;
				pVehicle->GetPosition(actorSync.vecPos);
				pVehicle->GetRotation(actorSync.vecRot);

				if((actorSync.vecPos-m_Actors[i].vecDriveFinalPos).Length() < 5.0)
					actorSync.bDriving = true;
				else
					actorSync.bDriving = false;

				bsSend.Write((char *)&actorSync, sizeof(ActorSyncData));
				g_pNetworkManager->RPC(RPC_SyncActor, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
			}
			else
			{
				// Reset stuff
				bsSend.Reset();
				fHeading = 0.0f;
				ActorSyncData actorSync;

				actorSync.bDriving = false;
				bsSend.Write((char *)&actorSync, sizeof(ActorSyncData));
				g_pNetworkManager->RPC(RPC_SyncActor, &bsSend, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
			}
		}
		else if(m_bActive[i] && m_Actors[i].bFailedEnterVehicle)
		{
			if(g_pVehicleManager->Exists(m_Actors[i].vehicleId) && g_pVehicleManager->Get(m_Actors[i].vehicleId)->IsSpawned() && g_pVehicleManager->Get(m_Actors[i].vehicleId)->IsStreamedIn())
			{
				m_Actors[i].bFailedEnterVehicle = false;
				DriveToPoint(i, m_Actors[i].vehicleId, m_Actors[i].vecDrivePos, CVector3(), m_Actors[i].vecDriveFinalPos, true);
			}
		}
	}
}