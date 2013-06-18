//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedTaskManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../IVCommon.h"

// From Multi Theft Auto
// Priority task types
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
// Secondary task types
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

// Movement task types
enum eTaskMovement
{
	TASK_MOVEMENT_UNKNOWN0, // seems to never be used
	TASK_MOVEMENT_UNKNOWN1,
	TASK_MOVEMENT_UNKNOWN2,
	TASK_MOVEMENT_MAX
};

class IVTask;
class IVPed;
class CIVTask;
class CIVPed;

class IVPedTaskManager
{
public:
	IVTask * m_pPrimaryTasks[TASK_PRIORITY_MAX];    // 00-14
	IVTask * m_pSecondaryTasks[TASK_SECONDARY_MAX]; // 14-2C
	IVTask * m_pMovementTasks[TASK_MOVEMENT_MAX];   // 2C-38
	IVPed * m_pPed;                                 // 38-3C
	PAD(IVPedTaskManager, pad0, 0x4);               // 3C-40
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
	void               SetTask(CIVTask * pTask, int iType, bool bForceNewTask = false);
	void               RemoveTask(int iType);
	CIVTask          * GetTask(int iType);

	// Secondary tasks
	void               SetTaskSecondary(CIVTask * pTask, int iType);
	void               RemoveTaskSecondary(int iType);
	CIVTask          * GetTaskSecondary(int iType);

	// Movement tasks
	void               SetTaskMovement(CIVTask * pTask, int iType);
	void               RemoveTaskMovment(int iType);
	CIVTask          * GetTaskMovement(int iType);

	void               ClearTasks(int iAbortPriority);
};