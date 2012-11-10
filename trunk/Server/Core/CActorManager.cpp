//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManager.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "CActorManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"
#include "CModuleManager.h"
#include "CVehicle.h"
#include "CVehicleManager.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;
extern CModuleManager * g_pModuleManager;
extern CVehicleManager * g_pVehicleManager;

CActorManager::CActorManager()
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		m_bActive[x] = false;
		m_Actors[x].bDrivingAutomatic = false;
	}
}

CActorManager::~CActorManager()
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}
}

EntityId CActorManager::Create(int iModelId, CVector3 vecPosition, float fHeading)
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(!m_bActive[x])
		{
			m_Actors[x].strName = "Actor";
			m_Actors[x].bTogglename = false;
			m_Actors[x].iColor = 0xFFFFFFAA;
			m_Actors[x].bFrozen = false;
			m_Actors[x].bHelmet = false;
			m_Actors[x].bBlip = true;
			m_Actors[x].bDrivingAutomatic = false;
			m_Actors[x].vecDrivePos = CVector3();
			m_Actors[x].vecDriveFinalPos = CVector3();
			m_Actors[x].vehicleId = -1;
			m_Actors[x].iSeat = -1;

			CBitStream bsSend;
			bsSend.Write(x);
			bsSend.Write(iModelId);
			bsSend.Write(vecPosition);
			bsSend.Write(fHeading);
			bsSend.Write(m_Actors[x].strName);
			bsSend.Write(m_Actors[x].bTogglename);
			bsSend.Write(m_Actors[x].iColor);
			bsSend.Write(m_Actors[x].bFrozen);
			bsSend.Write(m_Actors[x].bHelmet);
			bsSend.Write(m_Actors[x].bBlip);
			bsSend.Write(m_Actors[x].bDrivingAutomatic);
			bsSend.Write(m_Actors[x].vecDrivePos);
			bsSend.Write(m_Actors[x].vecDriveRot);
			bsSend.Write(m_Actors[x].vecDriveFinalPos);
			bsSend.Write(m_Actors[x].vehicleId);
			bsSend.Write(m_Actors[x].iSeat);

			g_pNetworkManager->RPC(RPC_NewActor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			m_Actors[x].iModelId = iModelId;
			memcpy(&m_Actors[x].vecPosition, &vecPosition, sizeof(CVector3));
			m_Actors[x].fHeading = fHeading;
			m_bActive[x] = true;
			CSquirrelArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("actorCreate", &pArguments);

			return x;
		}
	}

	return INVALID_ENTITY_ID;
}

void CActorManager::Delete(EntityId actorId)
{
	if(!m_bActive[actorId])
		return;

	CSquirrelArguments pArguments;
	pArguments.push(actorId);
	g_pEvents->Call("actorDelete", &pArguments);

	//TODO remove the player
	//if(m_Actors[actorId].bDrivingAutomatic)
		

	CBitStream bsSend;
	bsSend.Write(actorId);
	g_pNetworkManager->RPC(RPC_DeleteActor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	m_bActive[actorId] = false;
}

void CActorManager::SetPosition(EntityId actorId, CVector3 vecPosition)
{
	if(m_bActive[actorId])
	{
		memcpy(&m_Actors[actorId].vecPosition, &vecPosition, sizeof(CVector3));
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(vecPosition);
		g_pNetworkManager->RPC(RPC_ScriptingSetActorCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CActorManager::SetColor(EntityId actorId, unsigned int iColor)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].iColor = iColor;
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write((DWORD)iColor);
		g_pNetworkManager->RPC(RPC_ScriptingSetActorColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

CVector3 CActorManager::GetPosition(EntityId actorId)
{	
	if(m_bActive[actorId])
		return m_Actors[actorId].vecPosition;

	return CVector3(0.0f, 0.0f, 0.0f);
}

void CActorManager::SetHeading(EntityId actorId, float fHeading)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].fHeading = fHeading;
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(fHeading);
		g_pNetworkManager->RPC(RPC_ScriptingSetActorHeading, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CActorManager::SetActorName(EntityId actorId, String strName)
{
	if(m_bActive[actorId])
	{
		//Check if we have an valid Name
		if(strName.GetLength() > 2)
		{
			m_Actors[actorId].strName = strName;
			CBitStream bsSend;
			bsSend.Write(actorId);
			bsSend.Write(strName);
			g_pNetworkManager->RPC(RPC_ScriptingSetActorName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
		else 
			CLogFile::Print("The name of an actor requires min. 3 chars!");
	}
}

String CActorManager::GetActorName(EntityId actorId)
{
	if(m_bActive[actorId])
		return m_Actors[actorId].strName;
	
	return false;
}

void CActorManager::HandleClientJoin(EntityId playerId)
{
	if(GetActorCount() > 0)
	{
		CBitStream bsSend;

		for(EntityId x = 0; x < MAX_ACTORS; x++)
		{
			if(m_bActive[x])
			{
				bsSend.Write(x);
				bsSend.Write(m_Actors[x].iModelId);
				bsSend.Write(m_Actors[x].vecPosition);
				bsSend.Write(m_Actors[x].fHeading);
				bsSend.Write(m_Actors[x].strName);
				bsSend.Write(m_Actors[x].bTogglename);
				bsSend.Write(m_Actors[x].iColor);
				bsSend.Write(m_Actors[x].bFrozen);
				bsSend.Write(m_Actors[x].bHelmet);
				bsSend.Write(m_Actors[x].bBlip);
				bsSend.Write(m_Actors[x].bDrivingAutomatic);
				bsSend.Write(m_Actors[x].vecDrivePos);
				bsSend.Write(m_Actors[x].vecDriveRot);
				bsSend.Write(m_Actors[x].vecDriveFinalPos);
				bsSend.Write(m_Actors[x].vehicleId);
				bsSend.Write(m_Actors[x].iSeat);
			}
		}
		g_pNetworkManager->RPC(RPC_NewActor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

		for(EntityId x = 0; x < MAX_ACTORS; x++)
		{
			if(m_bActive[x] && m_Actors[x].bDrivingAutomatic)
			{
				bsSend.Reset();

				bsSend.Write(x);
				bsSend.Write(m_Actors[x].vecDriveFinalPos);
				g_pNetworkManager->RPC(RPC_ScriptingActorDriveToCoords, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			}
		}
	}
}

bool CActorManager::DoesExist(EntityId actorId)
{
	if(actorId < 0 || actorId >= MAX_ACTORS)
		return false;

	return m_bActive[actorId];
}

bool CActorManager::ToggleNametag(EntityId actorId, bool bShow)
{
	m_Actors[actorId].bTogglename = bShow;
	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(bShow);
	g_pNetworkManager->RPC(RPC_ScriptingToggleActorNametag, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return 1;
}

bool CActorManager::ToggleBlip(EntityId actorId, bool bShow)
{
	if(m_Actors[actorId].bBlip != bShow)
	{
		m_Actors[actorId].bBlip = bShow;
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(bShow);
		g_pNetworkManager->RPC(RPC_ScriptingToggleActorBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
	return 1;
}

bool CActorManager::ToggleFrozen(EntityId actorId, bool bFrozen)
{
	m_Actors[actorId].bFrozen = bFrozen;
	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(bFrozen);
	g_pNetworkManager->RPC(RPC_ScriptingToggleActorFrozen, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return 1;
}

bool CActorManager::ToggleHelmet(EntityId actorId, bool bHelmet)
{
	m_Actors[actorId].bHelmet = bHelmet;
	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(bHelmet);
	g_pNetworkManager->RPC(RPC_ScriptingToggleActorHelmet, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return 1;
}

void CActorManager::WarpIntoVehicle(EntityId actorId, EntityId vehicleId, int iSeatid)
{
	if(m_bActive[actorId])
	{
		// Check if we have a valid vehicle
		if(!g_pVehicleManager->DoesExist(vehicleId))
			return;

		// Check if we have a valid seat
		if(iSeatid < 0 || iSeatid > 3)
			return;

		m_Actors[actorId].bStateincar = true;
		m_Actors[actorId].vehicleId = vehicleId;
		m_Actors[actorId].iSeat = iSeatid;

		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(vehicleId);
		bsSend.Write(iSeatid);
		g_pNetworkManager->RPC(RPC_ScriptingWarpActorIntoVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CActorManager::RemoveFromVehicle(EntityId actorId)
{
	if(m_bActive[actorId])
	{
		//Check if he is in a car
		if(m_Actors[actorId].bStateincar)
		{
			m_Actors[actorId].bStateincar = false;
			CBitStream bsSend;
			bsSend.Write(actorId);
			g_pNetworkManager->RPC(RPC_ScriptingRemoveActorFromVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
	}
}

bool CActorManager::DriveToCoordinates(EntityId actorId, CVector3 vecDriveTo, CVector3 vecDriveRot, bool bStop)
{
	if(m_Actors[actorId].bDrivingAutomatic == true && !UpdateDrivePos(actorId, vecDriveTo, vecDriveRot, true))
		return false;

	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(vecDriveTo);
	g_pNetworkManager->RPC(RPC_ScriptingActorDriveToCoords, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

	// TODO :
	return false;
}

bool CActorManager::UpdateDrivePos(EntityId actorId, CVector3 vecDrivePos,CVector3 vecDriveRot, bool bStop)
{
	if(m_bActive[actorId])
	{
		if(!bStop)
		{
			m_Actors[actorId].vecDrivePos = vecDrivePos;
			m_Actors[actorId].vecDriveRot = vecDriveRot;
			CVehicle * pVehicle = g_pVehicleManager->GetAt(m_Actors[actorId].vehicleId);
			if(pVehicle)
			{
				pVehicle->SetPositionSave(vecDrivePos);
				pVehicle->SetRotationSave(vecDriveRot);
			}
			m_Actors[actorId].bDrivingAutomatic = true;
			return true;
		}
		else if(bStop)
		{
			m_Actors[actorId].bDrivingAutomatic = false;
			CBitStream bsSend;
			bsSend.Write(actorId);
			g_pNetworkManager->RPC(RPC_ScriptingStopActorDriving, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return true;
		}
		return true;
	}
	else
		return false;
}

EntityId CActorManager::GetActorCount()
{
	EntityId actorCount = 0;

	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(m_bActive[x])
			actorCount++;
	}

	return actorCount;
}