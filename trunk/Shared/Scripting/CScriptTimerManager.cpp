//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptTimerManager.cpp
// Project: Shared
// Author(s): MaVe
//            jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CScriptTimerManager.h"

CScriptTimerManager * CScriptTimerManager::m_pInstance = NULL;

CScriptTimerManager::CScriptTimerManager()
{
	// Set our instance
	m_pInstance = this;
}

CScriptTimerManager::~CScriptTimerManager()
{
	// Reset our instance
	m_pInstance = NULL;
}

void CScriptTimerManager::Pulse()
{
	for(iterator iter = begin(); iter != end(); )
	{
		if(!(*iter)->Pulse())
		{
			delete *iter;
			iter = erase(iter);
		}
		else
			++ iter;
	}
}

void CScriptTimerManager::HandleScriptUnload(CSquirrel * pScript)
{
	for(iterator iter = begin(); iter != end(); )
	{
		if((*iter)->GetScript() == pScript)
		{
			delete *iter;
			iter = erase(iter);
		}
		else
			++ iter;
	}
}

// this basically means the CScriptTimer* pointer is valid as well
bool CScriptTimerManager::contains(CScriptTimer* pTimer)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
		if((*iter) == pTimer)
			return true;

	return false;
}
