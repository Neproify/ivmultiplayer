//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AimSync.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "AimSync.h"
#include "COffsets.h"
#include <Patcher/CPatcher.h>
#include "CIVPed.h"
#include "CIVPlayerPed.h"
#include "CContextDataManager.h"
#include <CLogFile.h>

DWORD      dwFunc = NULL;
IVPed    * g_pIKPed = NULL;
CVector3 * g_vecLookAtTarget = NULL;
CVector3 * g_vecWeaponAimTarget = NULL;
IVPed    * g_pWeaponPed = NULL;
CVector3 * g_vecWeaponShotSource = NULL;
CVector3 * g_vecWeaponShotTarget = NULL;

#include "CChatWindow.h"
extern CChatWindow * g_pChatWindow;

void StoreAimTarget(IVPed * pPed, CVector3 * vecWeaponTarget)
{
	// Do we have a valid ped pointer and target pointer?
	if(pPed && vecWeaponTarget)
	{
		// Get the remote players context data
		CContextData * pContextData = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

		// Do we have a valid context data?
		if(pContextData)
		{
			// Is this the local player?
			if(pContextData->GetPlayerInfo()->GetPlayerNumber() == 0)
			{
				pContextData->SetWeaponAimTarget(*vecWeaponTarget);
				g_pChatWindow->AddInfoDebugMessage("StoreAimTargetLocal(0x%x, (%f, %f, %f))", pPed, vecWeaponTarget->fX, vecWeaponTarget->fY, vecWeaponTarget->fZ);
			}
			else
			{
				pContextData->GetWeaponAimTarget(*vecWeaponTarget);
				g_pChatWindow->AddInfoDebugMessage("StoreAimTarget(0x%x, (%f, %f, %f))", pPed, vecWeaponTarget->fX, vecWeaponTarget->fY, vecWeaponTarget->fZ);
			}
		}
		else
			CLogFile::PrintDebugf("StoreAimTarget Warning: Invalid Player Ped");
	}
}

void StoreShotSourceTarget(IVPed * pPed, CVector3 * pWeaponSource, CVector3 * pWeaponTarget)
{
	// Do we have a valid ped pointer, source pointer and target pointer?
	if(pPed && pWeaponSource && pWeaponTarget)
	{
		// Get the remote players context data
		CContextData * pContextData = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

		// Do we have a valid context data?
		if(pContextData)
		{
			// Is this the local player?
			if(pContextData->GetPlayerInfo()->GetPlayerNumber() == 0)
			{
				pContextData->SetWeaponShotSource(*pWeaponSource);
				pContextData->SetWeaponShotTarget(*pWeaponTarget);
			}
			else
			{
				pContextData->GetWeaponShotSource(*pWeaponSource);
				pContextData->GetWeaponShotTarget(*pWeaponTarget);
			}
		}
		else
			CLogFile::PrintDebugf("StoreShotSourceTarget Warning: Invalid Player Ped");
	}
}

void _declspec(naked) CIKManager__LookAt()
{
	_asm
	{
		push ebp
		mov ebp, esp
		mov eax, [ecx+40h] // CIKManager + 0x40 = CPed * pPed
		mov g_pIKPed, eax
		mov eax, [ebp+1Ch]
		mov g_vecLookAtTarget, eax
		pop ebp
		pushad
	}

	StoreAimTarget(g_pIKPed, g_vecLookAtTarget);
	dwFunc = (CGame::GetBase() + 0x959CC6);

	_asm
	{
		popad
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		jmp dwFunc
	}
}

void _declspec(naked) CIKManager__AimWeapon()
{
	_asm
	{
		push ebp
		mov ebp, esp
		mov eax, [ecx+40h] // CIKManager + 0x40 = CPed * pPed
		mov g_pIKPed, eax
		mov eax, [ebp+8]
		mov g_vecWeaponAimTarget, eax
		pop ebp
		pushad
	}

	StoreAimTarget(g_pIKPed, g_vecWeaponAimTarget);
	dwFunc = (CGame::GetBase() + 0x950D66);

	_asm
	{
		popad
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		jmp dwFunc
	}
}

void _declspec(naked) CWeapon__Fire_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
		mov eax, [ebp+8h]
		mov g_pWeaponPed, eax
		// [ebp+0Ch] = pSourceMatrix (Matrix34 *)
		mov eax, [ebp+10h]
		mov g_vecWeaponShotSource, eax
		mov eax, [ebp+14h]
		mov g_vecWeaponShotTarget, eax
		pop ebp
		pushad
	}

	StoreShotSourceTarget(g_pWeaponPed, g_vecWeaponShotSource, g_vecWeaponShotTarget);
	dwFunc = (CGame::GetBase() + 0x97D7C6);

	_asm
	{
		popad
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		jmp dwFunc
	}
}

void InstallAimSyncHooks()
{
	// For some reason when the aim/fire tasks are triggered
	// for remote players instead of the local player the 
	// CIKManager::AimWeapon function isn't called, tried 
	// disabling player ped checks below but still doesn't call
	// Disable local player checks for weapon aiming

	// Hook for the CIKManager::LookAt function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x959CC0), (DWORD)CIKManager__LookAt);

	// Hook for the CIKManager::AimWeapon function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x950D60), (DWORD)CIKManager__AimWeapon);

	// Hook for the CWeapon::Fire function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x97D7C0), (DWORD)CWeapon__Fire_Hook);
}
