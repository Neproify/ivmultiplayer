//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVEntity.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Math/CMath.h>
#include "IVCommon.h"

class IVEntityVFTable
{
public:
	DWORD ScalarDeletingDestructor;
	DWORD SetMatrix;
	DWORD SetPosition;
	DWORD SetHeading;
	DWORD m10;
	DWORD m14;
	DWORD m18;
	DWORD AddToInterior;
	DWORD RemoveFromInterior;
	DWORD m24;
	DWORD m28;
	DWORD m2C;
	DWORD m30;
	DWORD m34;
	DWORD SetModelIndex;
	DWORD SetModelByName;
	DWORD m40;
	DWORD m44;
	DWORD m48;
	DWORD m4C;
	DWORD m50;
	DWORD m54;
	DWORD m58;
	DWORD m5C;
	DWORD m60;
	DWORD m64;
	DWORD m68;
	DWORD m6C;
	DWORD m70;
	DWORD m74;
	DWORD m78;
	DWORD m7C;
	DWORD m80;
	DWORD m84;
	DWORD m88;
	DWORD m8C;
	DWORD m90;
	DWORD m94;
	DWORD m98;
	DWORD GetPosition;
	DWORD mA0;
	DWORD mA4;
	DWORD mA8;
	DWORD Add;
	DWORD Remove;
	DWORD mB4;
	DWORD mB8;
	DWORD mBC;
	DWORD mC0;
	DWORD mC4;
	DWORD mC8;
};

class IVEntity
{
public:
	IVEntityVFTable * m_VFTable; // 00-04
	DWORD field_4;               // 04-08
	DWORD field_8;               // 08-0C
	DWORD field_C;               // 0C-10
	CVector3 m_vecPosition;      // 10-1C
	float m_fHeading;            // 1C-20
	Matrix34 * m_pMatrix;        // 20-24
	// Flag 1 - Has collision model (Set to enable col, unset to disable col)

	// Flag 8 - Fixed waiting for world collision (Don't load collision)

	// flags 0 - has col model, 3 - don't load cols, 5 - visible, 12 - draw last
	DWORD m_dwFlags1;            // 24-28 - Bits 0: has collision model, 1: use collision, 3: don't load collisions, 4: fixed, 5: is visible, 8: fixed waiting for collision, 12: draw last, 32: visible
	DWORD m_dwFlags2;            // 28-2C - Bits 2: lights, 21: on fire
	WORD field_2C;               // 2C-2E
	WORD m_wModelIndex;          // 2E-30
	IVEntity * m_pReferences;    // 30-34
	DWORD m_pLivery;             // 34-38 - Not sure about this, all i know is ((ent + 0x34) + 0xD8) = vehicle livery
	DWORD m_pPhysics;            // 38-3C
	DWORD field_3C;              // 3C-40
	BYTE field_40;               // 40-41
	BYTE field_41;               // 41-42
	WORD field_42;               // 42-44
	WORD field_44;               // 48-46
	BYTE field_46;               // 46-47
	BYTE field_47;               // 47-48
	DWORD m_dwInteriorHandle;    // 48-4C - Handle to current interior instance in the interior instance pool
	DWORD field_4C;              // 4C-50
	DWORD field_50;              // 50-54
	DWORD field_54;              // 54-58
	DWORD field_58;              // 58-5C
	WORD field_5C;               // 5C-5E
	WORD field_5E;               // 5E-60
	BYTE field_60;               // 60-61
	BYTE field_61;               // 61-62
	BYTE field_62;               // 62-63
	BYTE m_byteAlpha;            // 63-64
	DWORD field_64;              // 64-68
	DWORD field_68;              // 68-6C
	DWORD m_pNetworkObject;      // 6C-70
	DWORD field_70;              // 70-74
};

class CIVEntity
{
private:
	IVEntity * m_pEntity;

public:
	CIVEntity();
	CIVEntity(IVEntity * pEntity);
	~CIVEntity();

	void       SetEntity(IVEntity * pEntity);
	IVEntity * GetEntity();

	void       SetMatrix(const Matrix& matMatrix);
	void       GetMatrix(Matrix& matMatrix);
	void       SetPosition(const CVector3& vecPosition);
	void       GetPosition(CVector3& vecPosition);
	void       SetHeading(float fHeading);
	void       SetRoll(const CVector3& vecRoll);
	void       GetRoll(CVector3& vecRoll);
	void       SetDirection(const CVector3& vecDirection);
	void       GetDirection(CVector3& vecDirection);
	void       SetModelIndex(WORD wModelIndex);
	WORD       GetModelIndex();
	void       SetAlpha(BYTE byteAlpha);
	BYTE       GetAlpha();
	bool       IsTouchingEntity(CIVEntity * pTouchingEntity);
	void       AddToWorld();
	void       RemoveFromWorld();
};
