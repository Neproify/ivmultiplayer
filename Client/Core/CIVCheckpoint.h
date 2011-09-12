//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVCheckpoint.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IVCommon.h"

class IVCheckpoint
{
public:
	BYTE m_byteActive;               // 00-01
	PAD(IVCheckpoint, pad0, 0x1);    // 01-02
	WORD m_wUnknown1;                // 02-04
	WORD m_wType;                    // 04-06
	PAD(IVCheckpoint, pad1, 0x2);    // 06-08
	float m_fRadius;                 // 08-0C
	PAD(IVCheckpoint, pad2, 0x4);    // 0C-10
	CVector3 m_vecPosition;          // 10-1C
	PAD(IVCheckpoint, pad3, 0x4);    // 1C-20
	CVector3 m_vecTargetPosition;    // 20-2C
	PAD(IVCheckpoint, pad4, 0x4);    // 2C-30
};

class CIVCheckpoint
{
private:
	bool           m_bCreatedByUs;
	IVCheckpoint * m_pCheckpoint;

public:
	CIVCheckpoint();
	CIVCheckpoint(IVCheckpoint * pCheckpoint);
	~CIVCheckpoint();

	void           SetCheckpoint(IVCheckpoint * pCheckpoint);
	IVCheckpoint * GetCheckpoint();

	void           SetActiveState(BYTE byteActive);
	BYTE           GetActiveState();
	void           SetType(WORD wType);
	WORD           GetType();
	void           SetRadius(float fRadius);
	float          GetRadius();
	void           SetPosition(CVector3 * vecPosition);
	void           GetPosition(CVector3 * vecPosition);
	void           SetTargetPosition(CVector3 * vecTargetPosition);
	void           GetTargetPosition(CVector3 * vecTargetPosition);
};
