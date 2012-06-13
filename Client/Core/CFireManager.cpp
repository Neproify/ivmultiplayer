//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): FRi
// License: See LICENSE in root directory
//
//==============================================================================
#include "CFireManager.h"
#include "CGame.h"
#include "CChatWindow.h"

extern CChatWindow * g_pChatWindow;

extern CGame * g_pGame;

CFireManager::CFireManager()
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
		m_bActive[x] = false;
}

CFireManager::~CFireManager()
{
	for(EntityId x = 0; x < MAX_FIRE; x++)
	{
		if(m_bActive[x])
			Delete(x);
	}
}

void CFireManager::Create(EntityId fireId, CVector3& vecPos, float fdensity)
{
	if(m_bActive[fireId])
		Delete(fireId);
	
	m_bActive[fireId] = true;
	char unkown;
	sprintf(&unkown,"000");
	unsigned int handle = CGame::CreateFire(vecPos,unkown,(unsigned int)fdensity);
	m_Fire[fireId].uiHandle = handle;
	m_Fire[fireId].fdensity = fdensity;
	g_pChatWindow->AddInfoMessage("FIRE READY: X: %f, Y: %f, Z: %f",vecPos.fX,vecPos.fY,vecPos.fZ);
}

void CFireManager::Delete(EntityId fireId)
{
	if(m_bActive[fireId]){
		m_bActive[fireId] = false;
		m_Fire[fireId].vecPos = CVector3(0.0f,0.0f,0.0f);
		CGame::DeleteFire(m_Fire[fireId].uiHandle);
	}
}
