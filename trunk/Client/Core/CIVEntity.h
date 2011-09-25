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

#define FUNC_CEntity__IsTouchingEntity_7 0xA47B30

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
	// 0x8 = DWORD
	PAD(IVEntity, pad0, 0xC);    // 04-10
	CVector3 m_vecPosition;      // 10-1C
	float m_fHeading;            // 1C-20
	Matrix34 * m_pMatrix;        // 20-24
	DWORD m_dwFlags1;            // 24-28 - Bits 0: has collision model, 1: use collision, 3: don't load collisions, 4: fixed, 5: is visible, 8: fixed waiting for collision, 12: draw last, 32: visible
	DWORD m_dwFlags2;            // 28-2C - Bits 2: lights, 21: on fire
	WORD m_w2C;                  // 2C-2E
	WORD m_wModelIndex;          // 2E-30
	IVEntity * m_pReferences;    // 30-34
	DWORD m_pLivery;             // 34-38 - Not sure about this, all i know is ((ent + 0x34) + 0xD8) = vehicle livery
	DWORD m_pPhysics;            // 38-3C
	// 0x3C = DWORD
	// 0x40 = BYTE
	// 0x41 = BYTE
	// 0x42 = WORD
	// 0x44 = WORD
	PAD(IVEntity, pad3, 0xC);    // 3C-48
	DWORD m_hInterior;           // 48-4C
	PAD(IVEntity, pad4, 0x17);   // 4C-63
	// 0x4C = DWORD
	// 0x58 = DWORD
	// 0x5C = WORD
	// 0x5E = WORD
	// 0x60 = BYTE
	// 0x61 = BYTE
	// 0x62 = BYTE
	BYTE m_byteAlpha;            // 63-64
	PAD(IVEntity, pad5, 0x8);    // 64-6C
	DWORD m_dwNetObject;         // 6C-70
	PAD(IVEntity, pad6, 0x4);    // 70-74
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
	void       SetRoll(const CVector3& vecRoll);
	void       GetRoll(CVector3& vecRoll);
	void       SetDirection(const CVector3& vecDirection);
	void       GetDirection(CVector3& vecDirection);
	void       SetModelIndex(WORD wModelIndex);
	WORD       GetModelIndex();
	void       SetAlpha(BYTE byteAlpha);
	BYTE       GetAlpha();
	bool       IsTouchingEntity(IVEntity * pTouchingEntity);
	void       AddToWorld();
	void       RemoveFromWorld();
};
