//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObject.h
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CStreamer.h"

class CObject : public CStreamableEntity
{
private:
	unsigned int	m_uiObjectHandle;
	DWORD			m_dwModelHash;
	CVector3		m_vecPosition;
	CVector3		m_vecRotation;
	bool			bAttached;
	bool			bVehicleAttached;
	unsigned int	uiVehiclePlayerId;
	CVector3		vecAttachPosition;
	CVector3		vecAttachRotation;

public:
	CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation);
	~CObject();

	bool IsSpawned() { return (m_uiObjectHandle != 0); }
	bool Create();
	void Destroy();
	void SetPosition(const CVector3& vecPosition);
	void GetPosition(CVector3& vecPosition);
	void SetRotation(const CVector3& vecRotation);
	void GetRotation(CVector3& vecRotation);
	unsigned int GetHandle() { return m_uiObjectHandle; }

	// Streaming
	void GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void UpdateInterior(unsigned int uiInterior);
	void StreamIn();
	void StreamOut();
};
