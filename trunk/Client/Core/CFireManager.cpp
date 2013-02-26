//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): FRi
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFireManager.h"
#include "Scripting.h"

CFireManager::CFireManager()
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		m_bActive[x] = false;
		m_Fire[x].uiHandle = -1;
	}
}

CFireManager::~CFireManager()
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		if(m_bActive[x])
			Delete(x);
	}
}

void CFireManager::Create(EntityId fireId, CVector3 vecPos, float fdensity)
{
	// Do we have created the fire?
	if(m_bActive[fireId])
		Delete(fireId);

	// Set the parameters
	m_bActive[fireId] = true;
	m_Fire[fireId].vecPos = vecPos;
	m_Fire[fireId].fdensity = fdensity;
	m_Fire[fireId].uiHandle = CGame::CreateFire(m_Fire[fireId].vecPos,(unsigned int)m_Fire[fireId].fdensity);
}

void CFireManager::Delete(EntityId fireId)
{
	// Do we have created the fire?
	if(m_bActive[fireId]){
		m_bActive[fireId] = false;
		m_Fire[fireId].vecPos = CVector3(0.0f,0.0f,0.0f);

		// Check if we have a fire handle
		if(m_Fire[fireId].uiHandle != -1)
			CGame::DeleteFire(m_Fire[fireId].uiHandle);

		// Reset fire handle
		m_Fire[fireId].uiHandle = -1;
	}
}

void CFireManager::ReCreateAllFire()
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		if(m_bActive[x])
		{
			// Check if we have a fire handle
			if(m_Fire[x].uiHandle != -1)
				CGame::DeleteFire(m_Fire[x].uiHandle);

			// Reset fire handle
			m_Fire[x].uiHandle = -1;

			// Recreate
			m_Fire[x].uiHandle = CGame::CreateFire(m_Fire[x].vecPos,(unsigned int)m_Fire[x].fdensity);
		}
	}
}
