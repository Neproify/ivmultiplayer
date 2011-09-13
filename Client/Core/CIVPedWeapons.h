//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedWeapons.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IVCommon.h"
#include "CIVWeapon.h"

#pragma pack(1)
class IVPedWeapons
{
public:
	PAD(IVPedWeapons, pad0, 0x3C);    // 000-03C
	IVWeapon m_weapons[10];           // 03C-0B4
	IVWeapon m_weapon;                // 0B4-0C0
	PAD(IVPedWeapons, pad1, 0x58);    // 0C0-118
	BYTE     m_byteCurrentWeaponSlot; // 118-119 (See eWeaponSlot)
	PAD(IVPedWeapons, pad2, 0x1);     // 119-11A
};
#pragma pack()

class CIVPed;

class CIVPedWeapons
{
private:
	IVPedWeapons * m_pPedWeapons;
	CIVPed       * m_pPed;
	CIVWeapon    * m_pWeapons[WEAPON_SLOT_MAX];
	CIVWeapon    * m_pWeapon;

public:
	CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed);
	~CIVPedWeapons();

	void           SetPedWeapons(IVPedWeapons * pPedWeapons) { m_pPedWeapons = pPedWeapons; }
	IVPedWeapons * GetPedWeapons() { return m_pPedWeapons; }
	void           SetPed(CIVPed * pPed) { m_pPed = pPed; }
	CIVPed       * GetPed() { return m_pPed; }
	CIVWeapon    * GetWeapon(eWeaponSlot slot) { return m_pWeapons[slot]; }
	CIVWeapon    * GetWeapon() { return m_pWeapon; }

	eWeaponSlot    GetCurrentWeaponSlot();
	void           SetCurrentWeapon(eWeaponType weapon, bool bUnknown = true);
	CIVWeapon    * GetCurrentWeapon();
	eWeaponType    GetCurrentWeaponType();;
	void           RemoveWeapon(eWeaponType weapon, int iUnknown = 1);
	void           RemoveAllWeapons();
};
