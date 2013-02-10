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
#include <CLogFile.h>

CIVPedWeapons::CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed) : m_pPedWeapons(pPedWeapons), m_pPed(pPed)
{

	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		m_pWeaponSlots[i] = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapons[i], this);

	m_pWeapon = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapon, this);
}

CIVPedWeapons::~CIVPedWeapons()
{
	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		SAFE_DELETE(m_pWeaponSlots[i]);

	SAFE_DELETE(m_pWeapon);
}

eWeaponType CIVPedWeapons::GetWeaponInSlot(eWeaponSlot weaponSlot)
{
	if(m_pPedWeapons)
		return m_pWeaponSlots[weaponSlot]->GetType();

	return WEAPON_TYPE_UNARMED;
}

eWeaponSlot CIVPedWeapons::GetCurrentWeaponSlot()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
		return (eWeaponSlot)m_pPedWeapons->m_currentWeaponSlot;

	return WEAPON_SLOT_UNARMED;
}

eWeaponType CIVPedWeapons::GetCurrentWeaponType()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
		return m_pWeaponSlots[m_pPedWeapons->m_currentWeaponSlot]->GetType();

	return WEAPON_TYPE_UNARMED;
}

void CIVPedWeapons::SetCurrentWeaponBySlot(eWeaponSlot weaponSlot)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return;

		int iUnknown = 0;

		if(m_pPed->IsInVehicle())
			iUnknown = 19;

		IVPed * pGamePed = m_pPed->GetPed();
		DWORD dwFunc = (CGame::GetBase() + 0x9AA330); // CPedWeapons::SetCurrentWeapon
		_asm
		{
			push pGamePed
			push 1
			push weaponSlot
			push iUnknown
			mov ecx, pPedWeapons
			call dwFunc
		}
	}
}

void CIVPedWeapons::SetCurrentWeaponByType(eWeaponType weaponType)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(weaponType);

		if(pWeaponInfo)
		{
			eWeaponSlot weaponSlot = pWeaponInfo->GetWeaponInfo()->m_slot;

			if(m_pWeaponSlots[weaponSlot]->GetType() == weaponType)
				SetCurrentWeaponBySlot(weaponSlot);
		}
	}
}

void CIVPedWeapons::SetCurrentWeaponVisible(bool bVisible)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();

		if(bVisible)
		{
			DWORD dwFunc = (CGame::GetBase() + 0x9AC110); // CPedWeapons::ShowWeapon
			_asm
			{
				push 0
				push -1
				push pGamePed
				mov ecx, pPedWeapons
				call dwFunc
			}
		}
		else
		{
			if(pPedWeapons->m2C)
			{
				DWORD dwFunc = (CGame::GetBase() + 0x9ABF00); // CPedWeapons::HideWeapon
				_asm
				{
					push 0
					push 0
					push pGamePed
					mov ecx, pPedWeapons
					call dwFunc
				}
			}
		}
	}
}

void CIVPedWeapons::RemoveWeapon(eWeaponType weaponType, int iUnknown)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();
		_asm
		{
			push iUnknown
			push weaponType
			push pGamePed
			mov ecx, pPedWeapons
			call COffsets::FUNC_CPedWeapons__RemoveWeapon
		}
	}
}

void CIVPedWeapons::RemoveAllWeapons()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();
		_asm
		{
			push pGamePed
			mov ecx, pPedWeapons
			call COffsets::FUNC_CPedWeapons__RemoveAllWeapons
		}
	}
}

DWORD CIVPedWeapons::GetAmmoBySlot(eWeaponSlot weaponSlot)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons && pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return 0;

		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(m_pWeaponSlots[weaponSlot]->GetType());

		if(pWeaponInfo && pWeaponInfo->GetWeaponInfo()->m_slot == weaponSlot)
			m_pWeaponSlots[weaponSlot]->SetAmmo(m_pCurrentWeapon->GetAmmoTotal());

		return m_pWeaponSlots[weaponSlot]->GetAmmo();
	}

	return 0;
}

DWORD CIVPedWeapons::GetAmmoByType(eWeaponType weaponType)
{
	if(m_pPedWeapons)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(weaponType);

		if(pWeaponInfo)
		{
			eWeaponSlot weaponSlot = pWeaponInfo->GetWeaponInfo()->m_slot;

			if(m_pWeaponSlots[weaponSlot]->GetType() == weaponType)
				return GetAmmoBySlot(weaponSlot);
		}
	}

	return 0;
}

void CIVPedWeapons::SetAmmoBySlot(eWeaponSlot weaponSlot, DWORD dwAmmo)
{
	if(m_pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return;

		SetAmmoByType(m_pWeaponSlots[weaponSlot]->GetType(), dwAmmo);
	}
}

void CIVPedWeapons::SetAmmoByType(eWeaponType weaponType, DWORD dwAmmo)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		DWORD dwFunc = (CGame::GetBase() + 0x9A9D60); // CPedWeapons::SetAmmo
		_asm
		{
			push dwAmmo
			push weaponType
			mov ecx, pPedWeapons
			call dwFunc
		}
	}
}

DWORD CIVPedWeapons::GetAmmoInClip()
{
	if(m_pPedWeapons && m_pCurrentWeapon)
		return m_pCurrentWeapon->GetAmmoInClip();

	return 0;
}

void CIVPedWeapons::SetAmmoInClip(DWORD dwAmmoInClip)
{
	if(m_pPedWeapons && m_pCurrentWeapon)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(m_pCurrentWeapon->GetType());

		if(pWeaponInfo)
		{
			WORD wClipSize = pWeaponInfo->GetWeaponInfo()->m_wClipSize;

			if(dwAmmoInClip > wClipSize)
				dwAmmoInClip = wClipSize;

			DWORD dwCurrentAmmoInClip = m_pCurrentWeapon->GetAmmoInClip();
			m_pCurrentWeapon->SetAmmoInClip(dwAmmoInClip);

			DWORD dwCurrentAmmo = m_pCurrentWeapon->GetAmmoTotal();
			DWORD dwDifference = (dwAmmoInClip - dwCurrentAmmoInClip);

			if(dwDifference >= 25000)
				m_pCurrentWeapon->SetAmmoTotal(25000);

			DWORD dwNewAmmo = (dwCurrentAmmo + dwDifference);

			if(dwNewAmmo <= 25000)
				m_pCurrentWeapon->SetAmmoTotal(dwNewAmmo);
			else
				m_pCurrentWeapon->SetAmmoTotal(25000);
		}
	}
}
