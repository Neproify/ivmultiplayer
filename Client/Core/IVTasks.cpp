//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: IVTasks.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "IVTasks.h"
#include "CClientTaskManager.h"
#include "CGame.h"
#include "COffsets.h"

extern CClientTaskManager * g_pClientTaskManager;

CIVTaskComplexNewGetInVehicle::CIVTaskComplexNewGetInVehicle(CIVVehicle * pVehicle, int a3, int a4, unsigned int a5, float a6) : CIVTaskComplex()
{
	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
	_asm
	{
		push a6
		push a5
		push a4
		push a3
		push pGameVehicle
		mov ecx, pTask
		call COffsets::FUNC_CTaskComplexNewGetInVehicle__Constructor
	}
}

CIVTaskComplexNewExitVehicle::CIVTaskComplexNewExitVehicle(CIVVehicle * pVehicle, int iExitMode, int a4, int a5) : CIVTaskComplex()
{
	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
	_asm
	{
		push a5
		push a4
		push iExitMode
		push pGameVehicle
		mov ecx, pTask
		call COffsets::FUNC_CTaskComplexNewExitVehicle__Constructor
	}
}

CIVTaskComplexDie::CIVTaskComplexDie(int a2, int a3, int a4, int a5, float a6, float a7, char a8) : CIVTaskComplex()
{
	// Create the task
	Create();

	// Call the task constructor
	IVTask * pTask = GetTask();
	_asm
	{
		push a8
		push a7
		push a6
		push a5
		push a4
		push a3
		push a2
		mov ecx, pTask
		call COffsets::FUNC_CTaskComplexDie__Constructor
	}
}

CIVTaskSimpleDead::CIVTaskSimpleDead(DWORD dwDeathTime, char a3, char a4) : CIVTaskSimple()
{
	// Create the task
	Create();

	// Call the task constructor
	IVTask * pTask = GetTask();
	_asm
	{
		push a4
		push a3
		push dwDeathTime
		mov ecx, pTask
		call COffsets::FUNC_CTaskSimpleDead__Constructor
	}
}

CIVTaskSimpleCarSetPedInVehicle::CIVTaskSimpleCarSetPedInVehicle(CIVVehicle * pVehicle, int a3, char a4, char a5) : CIVTaskSimple()
{
	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
	_asm
	{
		push a5
		push a4
		push a3
		push pGameVehicle
		mov ecx, pTask
		call COffsets::FUNC_CTaskSimpleCarSetPedInVehicle__Constructor
	}
}

CIVTaskSimpleCarSetPedOut::CIVTaskSimpleCarSetPedOut(CIVVehicle * pVehicle, int a3, char a4, char a5) : CIVTaskSimple()
{
	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
	_asm
	{
		push a5
		push a4
		push a3
		push pGameVehicle
		mov ecx, pTask
		call COffsets::FUNC_CTaskSimpleCarSetPedOut__Constructor
	}
}
