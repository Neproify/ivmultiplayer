//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedTaskManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPedTaskManager.h"
#include "CIVTask.h"
#include "CClientTaskManager.h"
#include "CGame.h"
#include "COffsets.h"

extern CClientTaskManager * g_pClientTaskManager;

CIVPedTaskManager::CIVPedTaskManager(IVPedTaskManager * pPedTaskManager, CIVPed * pPed)
{
	m_pPedTaskManager = pPedTaskManager;
	m_pPed = pPed;
}

void CIVPedTaskManager::SetTask(CIVTask * pTask, int iTaskPriority, bool bForceNewTask)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		IVPedTaskManager * pPedTaskManager = m_pPedTaskManager;
		IVTask * pGameTask = (pTask ? pTask->GetTask() : NULL);
		_asm
		{
			push bForceNewTask
			push iTaskPriority
			push pGameTask
			mov ecx, pPedTaskManager
			call COffsets::FUNC_CPedTaskManager__SetTask
		}
	}
}

void CIVPedTaskManager::RemoveTask(int iTaskPriority)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		// Make sure its not the default task (We will crash without it)
		if(iTaskPriority != TASK_PRIORITY_DEFAULT)
			SetTask(NULL, iTaskPriority);
	}
}

CIVTask * CIVPedTaskManager::GetTask(int iTaskPriority)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		if(iTaskPriority < TASK_PRIORITY_MAX)
			return g_pClientTaskManager->GetClientTaskFromGameTask(m_pPedTaskManager->m_primaryTasks[iTaskPriority]);
	}

	return NULL;
}

void CIVPedTaskManager::SetTaskSecondary(CIVTask * pTask, int iType)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		IVPedTaskManager * pPedTaskManager = m_pPedTaskManager;
		IVTask * pGameTask = (pTask ? pTask->GetTask() : NULL);
		_asm
		{
			push iType
			push pGameTask
			mov ecx, pPedTaskManager
			call COffsets::FUNC_CPedTaskManager__SetTaskSecondary
		}
	}
}

void CIVPedTaskManager::RemoveTaskSecondary(int iType)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
		SetTaskSecondary(NULL, iType);
}

CIVTask * CIVPedTaskManager::GetTaskSecondary(int iType)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		if(iType < TASK_SECONDARY_MAX)
			return g_pClientTaskManager->GetClientTaskFromGameTask(m_pPedTaskManager->m_secondaryTasks[iType]);
	}

	return NULL;
}

void CIVPedTaskManager::ClearTasks(int iAbortPriority)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		for(int i = 0; i < TASK_PRIORITY_MAX; i++)
		{
			CIVTask * pTask = GetTask(i);

			if(pTask)
				pTask->MakeAbortable(m_pPed, iAbortPriority, NULL);
		}

		for(int i = 0; i < TASK_SECONDARY_MAX; i++)
		{
			CIVTask * pTask = GetTaskSecondary(i);

			if(pTask)
				pTask->MakeAbortable(m_pPed, iAbortPriority, NULL);
		}
	}
}
