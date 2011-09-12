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
			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(iModelId);
			bsSend.Write(vecPosition);
			bsSend.Write(fHeading);
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