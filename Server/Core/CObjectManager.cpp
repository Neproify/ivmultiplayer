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
extern CEvents         * g_pEvents;
extern CModuleManager  * g_pModuleManager;

CObjectManager::CObjectManager()
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		m_bActive[x] = false;
		m_Objects[x].bAttached = false;
	}

	for(EntityId y = 0; y < MAX_FIRE; y++)
		m_bFireActive[y] = false;
}

CObjectManager::~CObjectManager()
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}

	for(EntityId y = 0; y < MAX_FIRE; y++)
	{
		if(m_bFireActive[y])
			DeleteFire(y);

		m_bFireActive[y] = false;
	}
}

EntityId CObjectManager::Create(DWORD dwModelHash, const CVector3& vecPosition, const CVector3& vecRotation)
{
	for(EntityId x = 0; x < MAX_OBJECTS; x++)
	{
		if(!m_bActive[x])
		{
			m_Objects[x].dwModelHash = dwModelHash;
			m_Objects[x].vecPosition = vecPosition;
			m_Objects[x].vecRotation = vecRotation;
			m_bActive[x] = true;
			m_Objects[x].iBone = -1;
			m_Objects[x].m_iInterior = -1;
			m_Objects[x].bAttached = false;

			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(m_Objects[x].dwModelHash);
			bsSend.Write(m_Objects[x].vecPosition);
			bsSend.Write(m_Objects[x].vecRotation);
			bsSend.Write(m_Objects[x].m_iInterior);
			bsSend.Write(m_Objects[x].bAttached);
			bsSend.Write(m_Objects[x].bVehicleAttached);
			bsSend.Write(m_Objects[x].uiVehiclePlayerId);
			bsSend.Write(m_Objects[x].vecAttachPosition);
			bsSend.Write(m_Objects[x].vecAttachRotation);
				
			if(m_Objects[x].iBone == -1)
				bsSend.Write0();
			else
			{
				bsSend.Write1();
				bsSend.Write(m_Objects[x].iBone);
			}

			g_pNetworkManager->RPC(RPC_NewObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			
			CSquirrelArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("objectCreate", &pArguments);

			SetDimension(x, this->GetDimension(x));	

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
				bsSend.Reset();
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Objects[x].dwModelHash);
				bsSend.Write(m_Objects[x].vecPosition);
				bsSend.Write(m_Objects[x].vecRotation);
				bsSend.Write(m_Objects[x].m_iInterior);
				bsSend.Write(m_Objects[x].bAttached);
				bsSend.Write(m_Objects[x].bVehicleAttached);
				bsSend.Write(m_Objects[x].uiVehiclePlayerId);
				bsSend.Write(m_Objects[x].vecAttachPosition);
				bsSend.Write(m_Objects[x].vecAttachRotation);
				
				if(m_Objects[x].iBone == -1)
					bsSend.Write0();
				else
				{
					bsSend.Write1();
					bsSend.Write(m_Objects[x].iBone);
				}

				g_pNetworkManager->RPC(RPC_NewObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				SetDimension(x, this->GetDimension(x));
			}
		}
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
		return m_Objects[objectId].dwModelHash;

	return 0;
}

bool CObjectManager::SetPosition(EntityId objectId, const CVector3& vecPosition)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].vecPosition = vecPosition;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecPosition);
		g_pNetworkManager->RPC(RPC_ScriptingSetObjectPosition, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}

	return false;
}

bool CObjectManager::GetPosition(EntityId objectId, CVector3& vecPosition)
{
	if(DoesExist(objectId))
	{
		vecPosition = m_Objects[objectId].vecPosition;
		return true;
	}

	return false;
}

bool CObjectManager::SetRotation(EntityId objectId, const CVector3& vecRotation)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].vecRotation = vecRotation;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecRotation);
		g_pNetworkManager->RPC(RPC_ScriptingSetObjectRotation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		return true;
	}

	return false;
}

bool CObjectManager::GetRotation(EntityId objectId, CVector3& vecRotation)
{
	if(DoesExist(objectId))
	{
		vecRotation = m_Objects[objectId].vecRotation;
		return true;
	}

	return false;
}

EntityId CObjectManager::CreateFire(const CVector3& vecPos, float fdensity)
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		if(!m_bFireActive[x])
		{
			CBitStream bsSend;
			bsSend.Write(x);
			bsSend.Write(vecPos);
			bsSend.Write(fdensity);
			g_pNetworkManager->RPC(RPC_ScriptingCreateFire, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			m_FireObject[x].vecPosition = vecPos;
			m_FireObject[x].fdensity = fdensity;
			m_bFireActive[x] = true;
			return x;
		}
	}
	return INVALID_ENTITY_ID;
}

void CObjectManager::DeleteFire(EntityId fireId)
{
	if(m_bFireActive[fireId])
	{
		CBitStream bsSend;
		bsSend.Write(fireId);
		g_pNetworkManager->RPC(RPC_ScriptingDeleteFire, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		m_FireObject[fireId].vecPosition = CVector3(0.0f,0.0f,0.0f);
		m_bFireActive[fireId] = false;
	}
}

void CObjectManager::HandleClientJoinFire(EntityId playerId)
{
	CBitStream bsSend;
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		if(m_bFireActive[x])
		{
			bsSend.Write(x);
			bsSend.Write(m_FireObject[x].vecPosition);
			bsSend.Write(m_FireObject[x].fdensity);
			g_pNetworkManager->RPC(RPC_ScriptingCreateFire, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		}
	}
}

void CObjectManager::CreateExplosion(const CVector3& vecPos, float fdensity)
{
	CBitStream bsSend;
	bsSend.Write(vecPos);
	bsSend.Write(fdensity);
	g_pNetworkManager->RPC(RPC_ScriptingCreateExplosion,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CObjectManager::AttachToPlayer(EntityId objectId, EntityId playerId, const CVector3& vecPos, const CVector3& vecRot, int iBone)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].bAttached = true;
		m_Objects[objectId].bVehicleAttached = false;
		m_Objects[objectId].uiVehiclePlayerId = playerId;
		m_Objects[objectId].vecAttachPosition = vecPos;
		m_Objects[objectId].vecAttachRotation = vecRot;
		m_Objects[objectId].iBone = iBone;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(m_Objects[objectId].bAttached);
		bsSend.Write(m_Objects[objectId].bVehicleAttached);
		bsSend.Write(m_Objects[objectId].uiVehiclePlayerId);
		bsSend.Write(m_Objects[objectId].vecAttachPosition);
		bsSend.Write(m_Objects[objectId].vecAttachRotation);
		if(iBone != -1)
		{
			bsSend.Write1();
			bsSend.Write(m_Objects[objectId].iBone);
		}
		else
			bsSend.Write0();
		g_pNetworkManager->RPC(RPC_ScriptingAttachObject,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CObjectManager::AttachToVehicle(EntityId objectId, EntityId vehicleId,const CVector3& vecPos, const CVector3& vecRot)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].bAttached = true;
		m_Objects[objectId].bVehicleAttached = true;
		m_Objects[objectId].uiVehiclePlayerId = vehicleId;
		m_Objects[objectId].vecAttachPosition = vecPos;
		m_Objects[objectId].vecAttachRotation = vecRot;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(m_Objects[objectId].bAttached);
		bsSend.Write(m_Objects[objectId].bVehicleAttached);
		bsSend.Write(m_Objects[objectId].uiVehiclePlayerId);
		bsSend.Write(m_Objects[objectId].vecAttachPosition);
		bsSend.Write(m_Objects[objectId].vecAttachRotation);
		bsSend.Write0();
		g_pNetworkManager->RPC(RPC_ScriptingAttachObject,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CObjectManager::Detach(EntityId objectId)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].bAttached = false;
		m_Objects[objectId].bVehicleAttached = false;
		m_Objects[objectId].uiVehiclePlayerId = INVALID_ENTITY_ID;
		m_Objects[objectId].vecAttachPosition = CVector3();
		m_Objects[objectId].vecAttachRotation = CVector3();
		m_Objects[objectId].iBone = -1;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		g_pNetworkManager->RPC(RPC_ScriptingDetachObject,&bsSend,PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CObjectManager::MoveObject(EntityId objectId, const CVector3& vecMoveTarget, int iTime, bool bHasRot, const CVector3& vecMoveRot)
{
	if(DoesExist(objectId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecMoveTarget);
		bsSend.Write(iTime);
		m_Objects[objectId].vecPosition = vecMoveTarget;

		if(bHasRot)
		{
			bsSend.Write1();
			bsSend.Write(vecMoveRot);
			m_Objects[objectId].vecRotation = vecMoveRot;
		}
		else
			bsSend.Write0();

		g_pNetworkManager->RPC(RPC_ScriptingMoveObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CObjectManager::RotateObject(EntityId objectId, const CVector3& vecMoveRot, int iTime)
{
	if(DoesExist(objectId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(vecMoveRot);
		bsSend.Write(iTime);
		m_Objects[objectId].vecRotation = vecMoveRot;

		g_pNetworkManager->RPC(RPC_ScriptingRotateObject, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}


void CObjectManager::SetDimension(EntityId objectId, DimensionId ucDimension)
{
	if(DoesExist(objectId))
	{
		m_Objects[objectId].ucDimension = ucDimension;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(ucDimension);

		g_pNetworkManager->RPC(RPC_ScriptingSetObjectDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}


void CObjectManager::SetInterior(EntityId objectId, int iInterior)
{
	if(DoesExist(objectId)) {
		m_Objects[objectId].m_iInterior = iInterior;

		CBitStream bsSend;
		bsSend.WriteCompressed(objectId);
		bsSend.Write(iInterior);
		
		g_pNetworkManager->RPC(RPC_ScriptingSetObjectInterior, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}