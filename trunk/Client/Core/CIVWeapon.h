//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWeapon.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IVCommon.h"
#include "CIVWeaponInfo.h"
#include "CXLiveHook.h"

class IVWeapon
{
public:
	eWeaponType           m_type;   // 00-04
	FakeProtectedBuffer * m_pAmmo;  // 04-08
	BYTE                  bytePad1; // 08-09
	PAD(IVWeapon, pad0, 3);         // 09-0C
};

class CIVPedWeapons;

class CIVWeapon
{
private:
	IVWeapon      * m_pWeapon;
	CIVPedWeapons * m_pPedWeapons;

public:
	CIVWeapon(IVWeapon * pWeapon, CIVPedWeapons * pPedWeapons);

	void            SetWeapon(IVWeapon * pWeapon) { m_pWeapon = pWeapon; }
	IVWeapon      * GetWeapon() { return m_pWeapon; }
	void            SetPedWeapons(CIVPedWeapons * pPedWeapons) { m_pPedWeapons = pPedWeapons; }
	CIVPedWeapons * GetPedWeapons() { return m_pPedWeapons; }

	eWeaponType     GetType();
	void            SetAmmo(DWORD dwAmmo);
	DWORD           GetAmmo();
	void            Remove();
	void            SetCurrent();
};
