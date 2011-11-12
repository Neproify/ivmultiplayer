//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickup.h
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CStreamer.h"

class CPickup : public CStreamableEntity
{
private:
	unsigned int  m_uiPickupHandle;
	DWORD         m_dwModelHash;
	unsigned char m_ucType;
	unsigned int  m_uiValue;
	CVector3      m_vecPosition;
	CVector3      m_vecRotation;

public:
	CPickup(DWORD dwModelHash, unsigned char ucType, unsigned int uiValue, CVector3 vecPosition, CVector3 vecRotation);
	~CPickup();

	bool         IsSpawned() { return (m_uiPickupHandle != 0); }
	bool         Create();
	void         Destroy();
	void         SetPosition(const CVector3& vecPosition);
	void         GetPosition(CVector3& vecPosition);
	void         SetRotation(const CVector3& vecRotation);
	void         GetRotation(CVector3& vecRotation);
	void         SetValue(unsigned int uiValue);
	unsigned int GetValue();

	// Streaming
	void         GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void         UpdateInterior(unsigned int uiInterior);
	void         StreamIn();
	void         StreamOut();
};
