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

extern CModelManager * g_pModelManager;

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
	}
}

void CActorManager::Create(EntityId actorId, int iModelId, CVector3 vecPosition, float fHeading)
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
	Scripting::SetCharDefaultComponentVariation(m_Actors[actorId].uiActorIndex);
	Scripting::AddArmourToChar(m_Actors[actorId].uiActorIndex, 200);
	Scripting::SetCharHeading(m_Actors[actorId].uiActorIndex, fHeading);
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
