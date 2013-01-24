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

	struct
	{
		struct
		{
			CVector3      vecStart;
			CVector3      vecTarget;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} pos;

		struct
		{
			CVector3      vecStart;
			CVector3      vecTarget;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} rot;
	}               m_interp;

public:
	CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation);
	~CObject();

	bool         IsSpawned() { return (m_uiObjectHandle != 0); }
	unsigned int GetHandle() { return m_uiObjectHandle; }
	bool         Create();
	void         Destroy();
	void         SetPosition(const CVector3& vecPosition);
	void         GetPosition(CVector3& vecPosition);
	void         SetRotation(const CVector3& vecRotation);
	void         GetRotation(CVector3& vecRotation);
	void         Process();

	bool		 IsAttached() { return bAttached; }
	void		 SetAttached(bool bAttach) { bAttached = bAttach; }

	void		 Move(const CVector3& vecPosition, unsigned int uiTime);
	void         Rotate(const CVector3& vecRotation, unsigned int uiTime);
	bool         IsMoving() { return (m_interp.pos.ulFinishTime != 0); }
	bool         IsRotating() { return (m_interp.rot.ulFinishTime != 0); }

	// Streaming
	void GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void UpdateInterior(unsigned int uiInterior);
	void StreamIn();
	void StreamOut();
};
