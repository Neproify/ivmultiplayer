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
#include "CIVPedWeaponSlot.h"

#pragma pack(1)
class IVPedWeapons
{
public:
	PAD(IVPedWeapons, pad0, 0x38); // 000-038
	DWORD m_pWeapon;               // 038-03C (CWeapon * m_pWeapon)
	IVPedWeaponSlot m_weapons[10]; // 03C-0B4
	IVPedWeaponSlot m_weapon;      // 0B4-0C0
	PAD(IVPedWeapons, pad1, 0x18); // 0C0-0D8
	BYTE m_byteShootRate;          // 0D8-0D9
	BYTE m_byteAccuracy;           // 0D9-0DA
	// 0xF8 - Target Entity?
	PAD(IVPedWeapons, pad2, 0x3E); // 0DA-118
	BYTE m_byteCurrentWeaponSlot;  // 118-119 (See eWeaponSlot)
	PAD(IVPedWeapons, pad3, 0x1);  // 119-11A
};
#pragma pack()

class CIVPed;

class CIVPedWeapons
{
private:
	IVPedWeapons     * m_pPedWeapons;
	CIVPed           * m_pPed;
	CIVPedWeaponSlot * m_pWeapons[WEAPON_SLOT_MAX];
	CIVPedWeaponSlot * m_pWeapon;

public:
	CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed);
	~CIVPedWeapons();

	void               SetPedWeapons(IVPedWeapons * pPedWeapons) { m_pPedWeapons = pPedWeapons; }
	IVPedWeapons     * GetPedWeapons() { return m_pPedWeapons; }
	void               SetPed(CIVPed * pPed) { m_pPed = pPed; }
	CIVPed           * GetPed() { return m_pPed; }
	CIVPedWeaponSlot * GetWeapon(eWeaponSlot slot) { return m_pWeapons[slot]; }
	CIVPedWeaponSlot * GetWeapon() { return m_pWeapon; }

	eWeaponSlot        GetCurrentWeaponSlot();
	void               SetCurrentWeapon(eWeaponType weapon, bool bUnknown = true);
	CIVPedWeaponSlot * GetCurrentWeapon();
	eWeaponType        GetCurrentWeaponType();;
	void               RemoveWeapon(eWeaponType weapon, int iUnknown = 1);
	void               RemoveAllWeapons();
};
