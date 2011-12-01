//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedTaskManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

// From Multi Theft Auto
// Priority Task Types
enum eTaskPriority
{
	TASK_PRIORITY_PHYSICAL_RESPONSE,
	TASK_PRIORITY_EVENT_RESPONSE_TEMP,
	TASK_PRIORITY_EVENT_RESPONSE_NONTEMP,
	TASK_PRIORITY_PRIMARY,
	TASK_PRIORITY_DEFAULT,
	TASK_PRIORITY_MAX
};

// From Multi Theft Auto
// Secondary Task Types
enum eTaskSecondary
{
	TASK_SECONDARY_ATTACK,              // want duck to be after attack
	TASK_SECONDARY_DUCK,                // because attack controls ducking movement
	TASK_SECONDARY_SAY,
	TASK_SECONDARY_FACIAL_COMPLEX,
	TASK_SECONDARY_PARTIAL_ANIM,
	TASK_SECONDARY_IK,
	TASK_SECONDARY_MAX
};

class IVTask;
class CIVTask;
class CIVPed;

class IVPedTaskManager
{
public:
	IVTask * m_primaryTasks[TASK_PRIORITY_MAX];    // 00-14
	IVTask * m_secondaryTasks[TASK_SECONDARY_MAX]; // 14-2C
	IVTask * m_unknownTasks[3];                    // 2C-38 (Seems like movement specific tasks)
	// TODO: Find size
};

class CIVPedTaskManager
{
private:
	IVPedTaskManager * m_pPedTaskManager;
	CIVPed           * m_pPed;

public:
	CIVPedTaskManager(IVPedTaskManager * pPedTaskManager, CIVPed * pPed);

	void               SetPedTaskManager(IVPedTaskManager * pPedTaskManager) { m_pPedTaskManager = pPedTaskManager; }
	IVPedTaskManager * GetPedTaskManager() { return m_pPedTaskManager; }
	void               SetPed(CIVPed * pPed) { m_pPed = pPed; }
	CIVPed           * GetPed() { return m_pPed; }

	// Primary tasks
	void               SetTask(CIVTask * pTask, int iTaskPriority, bool bForceNewTask = false);
	void               RemoveTask(int iTaskPriority);
	CIVTask          * GetTask(int iTaskPriority);

	// Secondary tasks
	void               SetTaskSecondary(CIVTask * pTask, int iType);
	void               RemoveTaskSecondary(int iType);
	CIVTask          * GetTaskSecondary(int iType);

	// Unknown tasks
	// TODO

	void               ClearTasks(int iAbortPriority);
};
