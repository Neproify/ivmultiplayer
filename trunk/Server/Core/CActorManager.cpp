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

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;
extern CModuleManager * g_pModuleManager;

CActorManager::CActorManager()
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
		m_bActive[x] = false;
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
			m_Actors[x].name = "Actor";
			m_Actors[x].togglename = false;
			m_Actors[x].color = 0xFFFFFFAA;
			m_Actors[x].frozen = false;
			m_Actors[x].helmet = false;
			m_Actors[x].bBlip = true;

			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(iModelId);
			bsSend.Write(vecPosition);
			bsSend.Write(fHeading);
			bsSend.Write(m_Actors[x].name);
			bsSend.Write(m_Actors[x].togglename);
			bsSend.Write(m_Actors[x].color);
			bsSend.Write(m_Actors[x].frozen);
			bsSend.Write(m_Actors[x].helmet);
			bsSend.Write(m_Actors[x].bBlip);

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
	CBitStream bsSend;
	bsSend.WriteCompressed(actorId);
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

void CActorManager::SetColor(EntityId actorId, int color)
{
	if(m_bActive[actorId])
	{
		m_Actors[actorId].color = color;
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(color);
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

void CActorManager::SetActorName(EntityId actorId, String name)
{
	if(m_bActive[actorId])
	{
		//Check if we have an valid Name
		if(name != "")
		{
			m_Actors[actorId].name = name;
			CBitStream bsSend;
			bsSend.Write(actorId);
			bsSend.Write(name);
			g_pNetworkManager->RPC(RPC_ScriptingSetActorName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
	}
}

String CActorManager::GetActorName(EntityId actorId)
{
	if(m_bActive[actorId])
		return m_Actors[actorId].name;
	
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
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Actors[x].iModelId);
				bsSend.Write(m_Actors[x].vecPosition);
				bsSend.Write(m_Actors[x].fHeading);
				bsSend.Write(m_Actors[x].name);
				bsSend.Write(m_Actors[x].togglename);
				bsSend.Write(m_Actors[x].color);
				bsSend.Write(m_Actors[x].frozen);
				bsSend.Write(m_Actors[x].helmet);
				bsSend.Write(m_Actors[x].bBlip);
			}
		}

		g_pNetworkManager->RPC(RPC_NewActor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
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
	m_Actors[actorId].togglename = bShow;
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

bool CActorManager::ToggleFrozen(EntityId actorId, bool frozen)
{
	m_Actors[actorId].frozen = frozen;
	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(frozen);
	g_pNetworkManager->RPC(RPC_ScriptingToggleActorFrozen, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return 1;
}

bool CActorManager::ToggleHelmet(EntityId actorId, bool helmet)
{
	m_Actors[actorId].helmet = helmet;
	CBitStream bsSend;
	bsSend.Write(actorId);
	bsSend.Write(helmet);
	g_pNetworkManager->RPC(RPC_ScriptingToggleActorHelmet, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return 1;
}

void CActorManager::WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid)
{
	if(m_bActive[actorId])
	{
		//chek if we have an valid car
		//chek if we have an valid seat id
		m_Actors[actorId].stateincar = true;
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(vehicleid);
		bsSend.Write(seatid);
		g_pNetworkManager->RPC(RPC_ScriptingWarpActorIntoVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CActorManager::RemoveFromVehicle(EntityId actorId)
{
	if(m_bActive[actorId])
	{
		//Check if he is in a car
		if(m_Actors[actorId].stateincar)
		{
			m_Actors[actorId].stateincar = false;
			CBitStream bsSend;
			bsSend.Write(actorId);
			g_pNetworkManager->RPC(RPC_ScriptingRemoveActorFromVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
	}
}
