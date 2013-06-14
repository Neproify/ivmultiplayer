//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPed.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPed.h"
#include "../CGame.h"
#include "../COffsets.h"
#include <CLogFile.h>

CIVPed::CIVPed() : CIVPhysical(),
	m_pPedTaskManager(NULL)
{
}

CIVPed::CIVPed(IVPed * pPed) : CIVPhysical(pPed),
	m_pPedTaskManager(NULL),
	m_pPedWeapons(NULL)
{
	SetPed(pPed);
}

CIVPed::~CIVPed()
{
	SetPed(NULL);
}

void CIVPed::SetPed(IVPed * pPed)
{
	SetPhysical(pPed);

	// Delete the ped task manager if we have one
	SAFE_DELETE(m_pPedTaskManager);

	// Delete the ped weapons if we have one
	SAFE_DELETE(m_pPedWeapons);

	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Create the ped task manager
		m_pPedTaskManager = new CIVPedTaskManager(&pPed->m_pPedIntelligence->m_pedTaskManager, this);

		// Create the ped weapons
		m_pPedWeapons = new CIVPedWeapons(&pPed->m_weapons, this);
	}
}

IVPed * CIVPed::GetPed()
{
	return (IVPed *)GetPhysical();
}

void CIVPed::SetType(ePedType type)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pPedBase->m_type = type;
}

ePedType CIVPed::GetType()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pPedBase->m_type;

	return (ePedType)0/*PED_TYPE_PLAYER*/;
}

void CIVPed::SetPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pPlayerInfo = pPlayerInfo;
}

IVPlayerInfo * CIVPed::GetPlayerInfo()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pPlayerInfo;

	return NULL;
}

bool CIVPed::IsInVehicle()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return ((pPed->m_byteUnknown & 4) != 0);

	return false;
}

void CIVPed::SetCurrentHeading(float fCurrentHeading)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_fCurrentHeading = fCurrentHeading;
}

float CIVPed::GetCurrentHeading()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_fCurrentHeading;

	return false;
}

void CIVPed::SetDesiredHeading(float fDesiredHeading)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_fDesiredHeading = fDesiredHeading;
}

float CIVPed::GetDesiredHeading()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_fDesiredHeading;

	return false;
}

void CIVPed::SetCurrentVehicle(IVVehicle * pVehicle)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pCurrentVehicle = pVehicle;
}

IVVehicle * CIVPed::GetCurrentVehicle()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pCurrentVehicle;

	return NULL;
}

void CIVPed::SetDucking(bool bDucking, int iUnknown)
{
	IVPed * pPed = GetPed();

	if(pPed)
	{
		_asm
		{
			push iUnknown
			push bDucking
			mov ecx, pPed
			call COffsets::FUNC_CPed__SetDucking
		}
	}
}

bool CIVPed::IsDucking()
{
	IVPed * pPed = GetPed();

	if(pPed)
	{
		bool bDucking = false;
		_asm
		{
			mov ecx, pPed
			call COffsets::FUNC_CPed__IsDucking
			mov bDucking, al
		}
		return bDucking;
	}

	return false;
}

void CIVPed::SetRagdoll(bool bRagdoll)
{
	IVPed * pPed = GetPed();

	if(pPed)
	{
		if(bRagdoll)
		{
			if(!pPed->m_dwRagdollStatus)
				pPed->m_dwRagdollStatus = 2;
		}
		else
		{
			if(pPed->m_dwRagdollStatus > 2)
			{
				_asm
				{
					push 1
					push 1
					push 1
					mov ecx, pPed
					call COffsets::FUNC_CPed__DisableRagdoll
				}
			}

			pPed->m_dwRagdollStatus = 0;
		}

		pPed->m_dwRagdollTime = CGame::GetTime();
	}
}

bool CIVPed::IsRagdoll()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return (pPed->m_dwRagdollStatus == 0);

	return true;
}

unsigned int CIVPed::GetAmmo(eWeaponType weapon)
{
	return 0;
}