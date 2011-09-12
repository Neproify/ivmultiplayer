//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManager.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "CBlipManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"
#include "CModuleManager.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;
extern CModuleManager * g_pModuleManager;

CBlipManager::CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
		m_bActive[x] = false;
}

CBlipManager::~CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}
}

EntityId CBlipManager::Create(int iSprite, CVector3 vecPosition)
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(!m_bActive[x])
		{
			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(iSprite);
			bsSend.Write(vecPosition);
			m_Blips[x].color = 0xFFFFFFFF;
			m_Blips[x].size = 1.0f;
			m_Blips[x].routeBlip = false;
			m_Blips[x].shortRange = false;
			m_Blips[x].strName = "";
			bsSend.Write(m_Blips[x].color);
			bsSend.Write(m_Blips[x].size);
			bsSend.Write(m_Blips[x].shortRange);
			bsSend.Write(m_Blips[x].routeBlip);
			bsSend.Write(m_Blips[x].strName);
			g_pNetworkManager->RPC(RPC_NewBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			m_Blips[x].sprite = iSprite;
			m_Blips[x].vecSpawnPos = vecPosition;
			m_bActive[x] = true;

			CSquirrelArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("blipCreate", &pArguments);

			return x;
		}
	}

	return INVALID_ENTITY_ID;
}

void CBlipManager::Delete(EntityId blipId)
{
	if(!DoesExist(blipId))
		return;

	CSquirrelArguments pArguments;
	pArguments.push(blipId);
	g_pEvents->Call("blipDelete", &pArguments);

	CBitStream bsSend;
	bsSend.WriteCompressed(blipId);
	g_pNetworkManager->RPC(RPC_DeleteBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	m_bActive[blipId] = false;
}

void CBlipManager::SetPosition(EntityId blipId, CVector3 vecPosition)
{
	if(DoesExist(blipId))
	{
		//Update serverside position
		m_Blips[blipId].vecSpawnPos = vecPosition;
		//Set blip position for clients, therefore delete...
		CBitStream bsSend;
		bsSend.WriteCompressed(blipId);
		g_pNetworkManager->RPC(RPC_DeleteBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		//...and create a new one
		CBitStream bsSend2;
		bsSend2.WriteCompressed(blipId);
		bsSend2.Write(m_Blips[blipId].sprite);
		bsSend2.Write(vecPosition);
		bsSend2.Write(m_Blips[blipId].color);
		bsSend2.Write(m_Blips[blipId].size);
		bsSend2.Write(m_Blips[blipId].shortRange);
		bsSend2.Write(m_Blips[blipId].routeBlip);
		bsSend2.Write(m_Blips[blipId].strName);
		g_pNetworkManager->RPC(RPC_NewBlip, &bsSend2, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

CVector3 CBlipManager::GetPosition(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].vecSpawnPos;

	return CVector3(0.0f, 0.0f, 0.0f);
}

void CBlipManager::SetColor(EntityId blipId, unsigned int uiColor) 
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].color = uiColor;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(uiColor);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

unsigned int CBlipManager::GetColor(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].color;

	return 0;
}

void CBlipManager::SetSize(EntityId blipId, float fSize)
{
	if(DoesExist(blipId) && fSize > 0.0f && fSize <= 4.0f)
	{
		m_Blips[blipId].size = fSize;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(fSize);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipSize, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

float CBlipManager::GetSize(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].size;

	return 0.0f;
}

void CBlipManager::ToggleShortRange(EntityId blipId, bool bShortRange)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].shortRange = bShortRange;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(bShortRange);
		g_pNetworkManager->RPC(RPC_ScriptingToggleBlipShortRange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CBlipManager::ToggleRoute(EntityId blipId, bool bRoute)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].routeBlip = bRoute;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(bRoute);
		g_pNetworkManager->RPC(RPC_ScriptingToggleBlipRoute, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CBlipManager::SetName(EntityId blipId, String strName)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].strName = strName;
		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(strName);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

String CBlipManager::GetName(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].strName;

	return "";
}

void CBlipManager::HandleClientJoin(EntityId playerId)
{
	if(GetBlipCount() > 0)
	{
		CBitStream bsSend;

		for(EntityId x = 0; x < MAX_BLIPS; x++)
		{
			if(m_bActive[x])
			{
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Blips[x].sprite);
				bsSend.Write(m_Blips[x].vecSpawnPos);
				bsSend.Write(m_Blips[x].color);
				bsSend.Write(m_Blips[x].size);
				bsSend.Write(m_Blips[x].shortRange);
				bsSend.Write(m_Blips[x].routeBlip);
				bsSend.Write(m_Blips[x].strName);
			}
		}

		g_pNetworkManager->RPC(RPC_NewBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	}
}

bool CBlipManager::DoesExist(EntityId blipId)
{
	if(blipId < 0 || blipId > MAX_BLIPS)
		return false;

	return m_bActive[blipId];
}

EntityId CBlipManager::GetBlipCount()
{
	EntityId blipCount = 0;

	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(m_bActive[x])
			blipCount++;
	}

	return blipCount;
}
