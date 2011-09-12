//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWeapon.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVWeapon.h"
#include "CIVPedWeapons.h"
#include "CGame.h"

CIVWeapon::CIVWeapon(IVWeapon * pWeapon, CIVPedWeapons * pPedWeapons)
{
	m_pWeapon = pWeapon;
	m_pPedWeapons = pPedWeapons;
}

eWeaponType CIVWeapon::GetType()
{
	if(m_pWeapon)
		return m_pWeapon->m_type;

	return WEAPON_TYPE_UNARMED;
}

void CIVWeapon::SetAmmo(DWORD dwAmmo)
{
	if(m_pWeapon)
	{
		if(dwAmmo > 25000)
			dwAmmo = 25000;

		XLivePBufferSetDWORD(m_pWeapon->m_pAmmo, 0, dwAmmo);
	}
}

DWORD CIVWeapon::GetAmmo()
{
	if(m_pWeapon)
	{
		DWORD dwAmmo;
		XLivePBufferGetDWORD(m_pWeapon->m_pAmmo, 0, &dwAmmo);
		return dwAmmo;
	}

	return 0;
}

void CIVWeapon::Remove()
{
	if(m_pWeapon)
	{
		if(m_pWeapon->bytePad1)
		{
			CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(m_pWeapon->m_type);

			if(pWeaponInfo)
			{
				int iModelIndex = CGame::GetStreaming()->GetModelIndexFromHash(pWeaponInfo->GetWeaponInfo()->m_dwModelHash);

				if(iModelIndex != -1)
				{
					CIVModelInfo * pModelInfo = CGame::GetModelInfo(iModelIndex);

					if(pModelInfo)
						pModelInfo->GetModelInfo()->dwReferenceCount--;
				}
			}
		}

		m_pWeapon->bytePad1 = 0;
		m_pWeapon->m_type = WEAPON_TYPE_UNARMED;
		SetAmmo(0);
	}
}

void CIVWeapon::SetCurrent()
{
	if(m_pWeapon)
		m_pPedWeapons->SetCurrentWeapon(m_pWeapon->m_type);
}
