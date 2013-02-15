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
#include "CIVPedWeaponSlot.h"

class IVObject;

#pragma pack(1)
class IVPedWeapons
{
public:
	PAD(IVPedWeapons, pad0, 0x18);              // 000-018
	eWeaponSlot m_currentWeaponSlot;            // 018-01C
	PAD(IVPedWeapons, pad1, 0x10);              // 01C-02C
	IVObject * m_pWeaponObject;                 // 02C-030
	PAD(IVPedWeapons, pad2, 0x8);               // 030-038
	IVWeapon * m_pSpecialWeapon;                // 038-03C
	IVPedWeaponSlot m_weapons[WEAPON_SLOT_MAX]; // 03C-0B4
	IVPedWeaponSlot m_weapon;                   // 0B4-0C0
	PAD(IVPedWeapons, pad3, 0x18);              // 0C0-0D8
	BYTE m_byteShootRate;                       // 0D8-0D9
	BYTE m_byteAccuracy;                        // 0D9-0DA
	PAD(IVPedWeapons, pad4, 0x40);              // 0DA-11A
};
#pragma pack()

class CIVObject;
class CIVPed;

class CIVPedWeapons
{
private:
	IVPedWeapons     * m_pPedWeapons;
	CIVPed           * m_pPed;
	CIVObject        * m_pWeaponObject;
	CIVWeapon        * m_pSpecialWeapon;
	CIVPedWeaponSlot * m_pWeaponSlots[WEAPON_SLOT_MAX];
	CIVPedWeaponSlot * m_pWeapon;

public:
	CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed);
	~CIVPedWeapons();

	void               SetPedWeapons(IVPedWeapons * pPedWeapons) { m_pPedWeapons = pPedWeapons; }
	IVPedWeapons     * GetPedWeapons() { return m_pPedWeapons; }
	void               SetPed(CIVPed * pPed) { m_pPed = pPed; }
	CIVPed           * GetPed() { return m_pPed; }
	CIVWeapon        * GetCurrentWeapon();
	CIVPedWeaponSlot * GetWeapon(eWeaponSlot slot) { return m_pWeaponSlots[slot]; }
	CIVPedWeaponSlot * GetWeapon() { return m_pWeapon; }

	void               GiveWeapon(eWeaponType weaponType, DWORD dwAmmo);
	eWeaponType        GetWeaponInSlot(eWeaponSlot weaponSlot);
	eWeaponSlot        GetCurrentWeaponSlot();
	eWeaponType        GetCurrentWeaponType();
	void               SetCurrentWeaponBySlot(eWeaponSlot weaponSlot);
	void               SetCurrentWeaponByType(eWeaponType weaponType);
	void               RemoveWeapon(eWeaponType weaponType, int iUnknown = 1);
	void               RemoveAllWeapons();
	void               SetCurrentWeaponVisible(bool bVisible);
	DWORD              GetAmmoBySlot(eWeaponSlot weaponSlot);
	DWORD              GetAmmoByType(eWeaponType weaponType);
	void               SetAmmoBySlot(eWeaponSlot weaponSlot, DWORD dwAmmo);
	void               SetAmmoByType(eWeaponType weaponType, DWORD dwAmmo);
	DWORD              GetAmmoInClip();
	void               SetAmmoInClip(DWORD dwAmmoInClip);
};
