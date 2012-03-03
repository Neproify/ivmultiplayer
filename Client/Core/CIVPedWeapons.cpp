//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedWeapons.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPedWeapons.h"
#include "CIVPed.h"
#include "CGame.h"
#include "COffsets.h"

CIVPedWeapons::CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed)
{
	m_pPedWeapons = pPedWeapons;
	m_pPed = pPed;

	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		m_pWeapons[i] = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapons[i], this);

	m_pWeapon = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapon, this);
}

CIVPedWeapons::~CIVPedWeapons()
{
	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		SAFE_DELETE(m_pWeapons[i]);

	SAFE_DELETE(m_pWeapon);
}

eWeaponSlot CIVPedWeapons::GetCurrentWeaponSlot()
{
	if(m_pPedWeapons)
		return (eWeaponSlot)m_pPedWeapons->m_byteCurrentWeaponSlot;

	return WEAPON_SLOT_UNARMED;
}

void CIVPedWeapons::SetCurrentWeapon(eWeaponType weapon, bool bUnknown)
{
	if(m_pPedWeapons)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(weapon);

		if(pWeaponInfo)
		{
			eWeaponSlot slot = pWeaponInfo->GetWeaponInfo()->m_slot;

			if(m_pWeapons[slot]->GetType() == weapon)
			{
				int iUnknown = 0;

				if(m_pPed->IsInVehicle())
					iUnknown = 19;

				IVPed * pGamePed = m_pPed->GetPed();
				IVPedWeapons * pPedWeapons = m_pPedWeapons;
				DWORD dwFunc = (CGame::GetBase() + 0x9AA330);
				_asm
				{
					push pGamePed
					push 1
					push slot
					push iUnknown
					mov ecx, pPedWeapons
					call dwFunc
				}

				if(bUnknown)
				{
					DWORD dwFunc = (CGame::GetBase() + 0x9AC110);
					_asm
					{
						push 0
						push -1
						push pGamePed
						mov ecx, pPedWeapons
						call dwFunc
					}
				}
			}
		}
	}
}

CIVPedWeaponSlot * CIVPedWeapons::GetCurrentWeapon()
{
	if(m_pPedWeapons)
		return m_pWeapons[m_pPedWeapons->m_byteCurrentWeaponSlot];

	return NULL;
}

eWeaponType CIVPedWeapons::GetCurrentWeaponType()
{
	// NOTE: Current weapon type is
	// *(DWORD *)(pPedWeapons + 12 * (*(DWORD *)(pPed + 0x2C8) + 5))
	if(m_pPedWeapons)
		return m_pWeapons[m_pPedWeapons->m_byteCurrentWeaponSlot]->GetType();

	return WEAPON_TYPE_UNARMED;
}

void CIVPedWeapons::RemoveWeapon(eWeaponType weapon, int iUnknown)
{
	IVPed * pGamePed = m_pPed->GetPed();
	IVPedWeapons * pPedWeapons = m_pPedWeapons;
	_asm
	{
		push iUnknown
		push weapon
		push pGamePed
		mov ecx, pPedWeapons
		call COffsets::FUNC_CPedWeapons__RemoveWeapon
	}
}

void CIVPedWeapons::RemoveAllWeapons()
{
	IVPed * pGamePed = m_pPed->GetPed();
	IVPedWeapons * pPedWeapons = m_pPedWeapons;
	_asm
	{
		push pGamePed
		mov ecx, pPedWeapons
		call COffsets::FUNC_CPedWeapons__RemoveAllWeapons
	}
}
