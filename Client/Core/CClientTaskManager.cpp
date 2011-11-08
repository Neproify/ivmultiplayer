//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientTaskManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClientTaskManager.h"

CClientTaskManager::CClientTaskManager()
{

}

CClientTaskManager::~CClientTaskManager()
{
	for(std::list<ClientTaskPair *>::iterator iter = m_taskList.begin(); iter != m_taskList.end(); iter++)
	{
		// Delete the client task pair
		delete (*iter);
	}
}

bool CClientTaskManager::AddTask(CIVTask * pClientTask)
{
	// Do we have an invalid client task pointer?
	if(!pClientTask)
		return false;

	// Create the client task pair
	ClientTaskPair * pClientTaskPair = new ClientTaskPair;

	// Do we have an invalid task pair?
	if(!pClientTaskPair)
		return false;

	// Set the client task pair pointers
	pClientTaskPair->pGameTask = pClientTask->GetTask();
	pClientTaskPair->pClientTask = pClientTask;

	// Add the client task pair to the client task pair list
	m_taskList.push_back(pClientTaskPair);
	return true;
}

bool CClientTaskManager::RemoveTask(CIVTask * pClientTask)
{
	// Do we have an invalid client task pointer?
	if(!pClientTask)
		return NULL;

	for(std::list<ClientTaskPair *>::iterator iter = m_taskList.begin(); iter != m_taskList.end(); iter++)
	{
		// Is this the client task pair we are looking for?
		if((*iter)->pClientTask == pClientTask)
		{
			// Delete the client task pair
			delete (*iter);
			return true;
		}
	}

	// Client task not in task list
	return false;
}

IVTask * CClientTaskManager::GetGameTaskFromClientTask(CIVTask * pClientTask)
{
	// Do we have an invalid client task pointer?
	if(!pClientTask)
		return NULL;

	for(std::list<ClientTaskPair *>::iterator iter = m_taskList.begin(); iter != m_taskList.end(); iter++)
	{
		// Is this the client task pair we are looking for?
		if((*iter)->pClientTask == pClientTask)
			return (*iter)->pGameTask;
	}

	// No game task found
	return NULL;
}

CIVTask * CClientTaskManager::GetClientTaskFromGameTask(IVTask * pGameTask, bool bCreateIfNotExist)
{
	// Do we have an invalid game task pointer?
	if(!pGameTask)
		return NULL;

	for(std::list<ClientTaskPair *>::iterator iter = m_taskList.begin(); iter != m_taskList.end(); iter++)
	{
		// Is this the client task pair we are looking for?
		if((*iter)->pGameTask == pGameTask)
			return (*iter)->pClientTask;
	}

	// Create the client task if requested
	if(bCreateIfNotExist)
	{
		// Create the client task instance
		CIVTask * pClientTask = new CIVTask(pGameTask);

		// Add the task
		AddTask(pClientTask);

		// Return the new client task instance
		return pClientTask;
	}

	// No client task found
	return NULL;
}

bool CClientTaskManager::HandleTaskDelete(IVTask * pGameTask)
{
	// Do we have an invalid game task pointer?
	if(!pGameTask)
		return false;

	// Try and get the client task pointer for this game task
	CIVTask * pClientTask = GetClientTaskFromGameTask(pGameTask, false);

	// Do we have a valid client task pointer?
	if(pClientTask)
	{
		// Remove the task
		RemoveTask(pClientTask);

		// Delete the client task
		delete pClientTask;
		return true;
	}

	// No valid client task pointer
	return false;
}
