//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CContextDataManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CContextDataManager.h"

std::list<CContextData *> CContextDataManager::m_contextDataList;

CContextDataManager::CContextDataManager()
{

}

CContextDataManager::~CContextDataManager()
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Delete this context info
		delete *iter;
	}
}

CContextData * CContextDataManager::CreateContextData(CIVPlayerInfo * pPlayerInfo)
{
	CContextData * pContextInfo = new CContextData(pPlayerInfo);
	m_contextDataList.push_back(pContextInfo);
	return pContextInfo;
}

void CContextDataManager::DestroyContextData(CContextData * pContextInfo)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Is this the context info we are looking for?
		if(*iter == pContextInfo)
		{
			// Remove it from the context info list
			m_contextDataList.remove(pContextInfo);
			break;
		}
	}

	// Delete the context info
	delete pContextInfo;
}

CContextData * CContextDataManager::GetContextData(BYTE bytePlayerNumber)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Get the context info pointer
		CContextData * pContextInfo = *iter;

		// Is this the context info we are looking for?
		if(pContextInfo->GetPlayerInfo()->GetPlayerNumber() == bytePlayerNumber)
			return pContextInfo;
	}

	return NULL;
}

CContextData * CContextDataManager::GetContextData(CIVPlayerInfo * pPlayerInfo)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Get the context info pointer
		CContextData * pContextInfo = *iter;

		// Is this the context info we are looking for?
		if(pContextInfo->GetPlayerInfo() == pPlayerInfo)
			return pContextInfo;
	}

	return NULL;
}

CContextData * CContextDataManager::GetContextData(IVPlayerInfo * pPlayerInfo)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Get the context info pointer
		CContextData * pContextInfo = *iter;

		// Is this the context info we are looking for?
		if(pContextInfo->GetPlayerInfo()->GetPlayerInfo() == pPlayerInfo)
			return pContextInfo;
	}

	return NULL;
}

CContextData * CContextDataManager::GetContextData(CIVPlayerPed * pPlayerPed)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Get the context info pointer
		CContextData * pContextInfo = *iter;

		// Is this the context info we are looking for?
		if(pContextInfo->GetPlayerPed() == pPlayerPed)
			return pContextInfo;
	}

	return NULL;
}

CContextData * CContextDataManager::GetContextData(IVPlayerPed * pPlayerPed)
{
	// Loop through the context info list
	for(std::list<CContextData *>::iterator iter = m_contextDataList.begin(); iter != m_contextDataList.end(); iter++)
	{
		// Get the context info pointer
		CContextData * pContextInfo = *iter;

		// Is this the context info we are looking for?
		if(pContextInfo->GetPlayerPed()->GetPlayerPed() == pPlayerPed)
			return pContextInfo;
	}

	return NULL;
}
