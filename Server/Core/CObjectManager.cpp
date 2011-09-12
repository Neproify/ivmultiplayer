//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CObjectManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"
#include "CModuleManager.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;
extern CModuleManager * g_pModuleManager;

CObjectManager::CObjectManager()
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
		m_bActive[x] = false;
}

CObjectManager::~CObjectManager()
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}
}

EntityId CObjectManager::Create(DWORD dwModelHash, float fX, float fY, float fZ, float fRX, float fRY, float fRZ)
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		if(!m_bActive[x])
		{
			CBitStream bsSend;
			CVector3 vecPos(fX, fY, fZ);
			CVector3 vecRot(fRX, fRY, fRZ);
			bsSend.WriteCompressed(x);
			bsSend.Write(dwModelHash);
			bsSend.Write(vecPos);
			bsSend.Write(vecRot);
			g_pNetworkManager->RPC(RPC_NewObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			m_Objects[x].dwModelHash = dwModelHash;
			m_Objects[x].vecSpawnPos = vecPos;
			m_Objects[x].vecSpawnRot = vecRot;
			m_bActive[x] = true;
			
			CSquirrelArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("objectCreate", &pArguments);

			return x;
		}
	}
	return INVALID_ENTITY_ID;
}

void CObjectManager::Delete(EntityId objectId)
{
	if(!m_bActive[objectId])
		return;

	CSquirrelArguments pArguments;
	pArguments.push(objectId);
	g_pEvents->Call("objectDelete", &pArguments);

	CBitStream bsSend;
	bsSend.WriteCompressed(objectId);
	g_pNetworkManager->RPC(RPC_DeleteObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	m_bActive[objectId] = false;
}

void CObjectManager::HandleClientJoin(EntityId playerId)
{
	if(GetObjectCount() > 0)
	{
		CBitStream bsSend;

		for(EntityId x = 0; x < MAX_OBJECTS; x++)
		{
			if(m_bActive[x])
			{
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Objects[x].dwModelHash);
				bsSend.Write(m_Objects[x].vecSpawnPos);
				bsSend.Write(m_Objects[x].vecSpawnRot);
			}
		}

		g_pNetworkManager->RPC(RPC_NewObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	}
}

bool CObjectManager::DoesExist(EntityId objectId)
{
	if(objectId < 0 || objectId >= MAX_OBJECTS)
		return false;

	return m_bActive[objectId];
}

EntityId CObjectManager::GetObjectCount()
{
	EntityId objectCount = 0;

	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		if(m_bActive[x])
			objectCount++;
	}

	return objectCount;
}

DWORD CObjectManager::GetModel(EntityId objectId)
{
	if(DoesExist(objectId))
	{
		return m_Objects[objectId].dwModelHash;
	}
	return 0;
}

bool CObjectManager::SetPosition(EntityId objectId, CVector3 vecPosition)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].vecSpawnPos = vecPosition;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecPosition);
		g_pNetworkManager->RPC(RPC_ScriptingSetObjectPosition, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}
	return false;
}

bool CObjectManager::GetPosition(EntityId objectId, CVector3* vecPosition)
{
	if(DoesExist(objectId))
	{
		memcpy(vecPosition, &m_Objects[objectId].vecSpawnPos, sizeof(CVector3));
		return true;
	}
	return false;
}

bool CObjectManager::SetRotation(EntityId objectId, CVector3 vecRotation)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].vecSpawnRot = vecRotation;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecRotation);
		g_pNetworkManager->RPC(RPC_ScriptingSetObjectRotation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}
	return false;
}

bool CObjectManager::GetRotation(EntityId objectId, CVector3* vecRotation)
{
	if(DoesExist(objectId))
	{
		memcpy(vecRotation, &m_Objects[objectId].vecSpawnRot, sizeof(CVector3));
		return true;
	}
	return false;
}
