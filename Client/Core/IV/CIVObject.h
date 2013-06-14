//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVObject.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVPhysical.h"
#include "CIVWeapon.h"

class IVObject : public IVPhysical
{
public:
	                           // 000-210
	PAD(IVObject, pad0, 0x4C); // 210-25C
	IVWeapon * m_pWeapon;      // 25C-260
	PAD(IVObject, pad1, 0xC0); // 210-320
};

class CIVObject : public CIVPhysical
{
private:
	IVObject  * m_pObject;
	CIVWeapon * m_pWeapon;

public:
	CIVObject(IVObject * pObject);
	~CIVObject();

	void        SetObject(IVObject * pObject) { m_pObject = pObject; }
	IVObject  * GetObject() { return m_pObject; }
	CIVWeapon * GetWeapon();
};