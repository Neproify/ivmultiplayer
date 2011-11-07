//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickupManager.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPickupManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;

CPickupManager::CPickupManager()
{
	for(EntityId x = 0; x < MAX_PICKUPS; x++)
		m_bActive[x] = false;
}

CPickupManager::~CPickupManager()
{
	for(EntityId x = 0; x < MAX_PICKUPS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}
}

EntityId CPickupManager::Create(DWORD dwModelHash, unsigned char ucType, unsigned int uiValue, float fX, float fY, float fZ, float fRX, float fRY, float fRZ)
{
	for(EntityId x = 0; x < MAX_PICKUPS; x++)
	{
		if(!m_bActive[x])
		{
			CVector3 vecPos(fX, fY, fZ);
			CVector3 vecRot(fRX, fRY, fRZ);
			m_Pickups[x].dwModelHash = dwModelHash;
			m_Pickups[x].vecPos = vecPos;
			m_Pickups[x].vecRot = vecRot;
			m_Pickups[x].ucType = ucType;
			m_Pickups[x].uiValue = uiValue;
			m_bActive[x] = true;

			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(dwModelHash);
			bsSend.Write(vecPos);
			bsSend.Write(vecRot);
			bsSend.Write(ucType);
			bsSend.Write(uiValue);
			g_pNetworkManager->RPC(RPC_NewPickup, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

			CSquirrelArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("pickupCreate", &pArguments);

			return x;
		}
	}
	return INVALID_ENTITY_ID;
}

void CPickupManager::Delete(EntityId pickupId)
{
	if(!m_bActive[pickupId])
		return;

	CSquirrelArguments pArguments;
	pArguments.push(pickupId);
	g_pEvents->Call("pickupDelete", &pArguments);

	CBitStream bsSend;
	bsSend.WriteCompressed(pickupId);
	g_pNetworkManager->RPC(RPC_DeletePickup, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	m_bActive[pickupId] = false;
}

void CPickupManager::HandleClientJoin(EntityId playerId)
{
	if(GetPickupCount() > 0)
	{
		CBitStream bsSend;

		for(EntityId x = 0; x < MAX_PICKUPS; x++)
		{
			if(m_bActive[x])
			{
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Pickups[x].dwModelHash);
				bsSend.Write(m_Pickups[x].vecPos);
				bsSend.Write(m_Pickups[x].vecRot);
				bsSend.Write(m_Pickups[x].ucType);
				bsSend.Write(m_Pickups[x].uiValue);
			}
		}

		g_pNetworkManager->RPC(RPC_NewPickup, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	}
}

bool CPickupManager::DoesExist(EntityId pickupId)
{
	if(pickupId < 0 || pickupId >= MAX_PICKUPS)
		return false;

	return m_bActive[pickupId];
}

EntityId CPickupManager::GetPickupCount()
{
	EntityId pickupCount = 0;

	for(EntityId x = 0; x < MAX_PICKUPS; x++)
	{
		if(m_bActive[x])
			pickupCount++;
	}

	return pickupCount;
}

DWORD CPickupManager::GetModel(EntityId pickupId)
{
	if(DoesExist(pickupId))
	{
		return m_Pickups[pickupId].dwModelHash;
	}
	return 0;
}


unsigned char CPickupManager::GetType(EntityId pickupId)
{
	if(DoesExist(pickupId))
	{
		return m_Pickups[pickupId].ucType;
	}
	return 0;
}

bool CPickupManager::SetValue(EntityId pickupId, unsigned int pValue)
{
	if(DoesExist(pickupId))
	{
		m_Pickups[pickupId].uiValue = pValue;

		CBitStream bsSend;
		bsSend.WriteCompressed(pickupId);
		bsSend.WriteCompressed(pValue);
		g_pNetworkManager->RPC(RPC_ScriptingSetPickupValue, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}
	return false;
}

unsigned int CPickupManager::GetValue(EntityId pickupId)
{
	if(DoesExist(pickupId))
	{
		return m_Pickups[pickupId].uiValue;
	}
	return 0;
}

bool CPickupManager::SetPosition(EntityId pickupId, CVector3 vecPosition)
{
	if(DoesExist(pickupId))
	{
		m_Pickups[pickupId].vecPos = vecPosition;

		CBitStream bsSend;
		bsSend.WriteCompressed(pickupId);
		bsSend.Write(vecPosition);
		g_pNetworkManager->RPC(RPC_ScriptingSetPickupPosition, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}
	return false;
}

bool CPickupManager::GetPosition(EntityId pickupId, CVector3* vecPosition)
{
	if(DoesExist(pickupId))
	{
		memcpy(vecPosition, &m_Pickups[pickupId].vecPos, sizeof(CVector3));
		return true;
	}
	return false;
}

bool CPickupManager::SetRotation(EntityId pickupId, CVector3 vecRotation)
{
	if(DoesExist(pickupId))
	{
		m_Pickups[pickupId].vecRot = vecRotation;

		CBitStream bsSend;
		bsSend.WriteCompressed(pickupId);
		bsSend.Write(vecRotation);
		g_pNetworkManager->RPC(RPC_ScriptingSetPickupRotation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}
	return false;
}

bool CPickupManager::GetRotation(EntityId pickupId, CVector3* vecRotation)
{
	if(DoesExist(pickupId))
	{
		memcpy(vecRotation, &m_Pickups[pickupId].vecRot, sizeof(CVector3));
		return true;
	}
	return false;
}
