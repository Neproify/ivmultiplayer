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
	bool			m_bIsMoving;
	bool			m_bIsRotating;
	float			m_fMoveSpeed;
	CVector3		m_vecMoveTarget;
	CVector3		m_vecMoveStart;
	CVector3		m_vecMoveTargetRot;
	CVector3		m_vecMoveStartRot;

	float			m_fMoveDuration;
	unsigned long	m_ulMoveStartTime;

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

	bool		IsAttached() { return bAttached; }
	void		SetAttached(bool bAttach) { bAttached = bAttach; }

	bool		IsMoving() { return m_bIsMoving; }
	void		SetIsMoving(bool bMoving) { m_bIsMoving = bMoving; }
	CVector3	GetMoveTarget() { return m_vecMoveTarget; }
	void		SetMoveTarget(const CVector3 &vec) { m_vecMoveTarget = vec; }
	CVector3	GetStartPosititon() { return m_vecMoveStart; }
	void		SetStartPosition(const CVector3 &vec) { m_vecMoveStart = vec; }

	bool		IsRotating() { return m_bIsMoving; }
	void		SetIsRotating(bool bRotating) { m_bIsRotating = bRotating; }
	CVector3	GetMoveTargetRot() { return m_vecMoveTargetRot; }
	void		SetMoveTargetRot(const CVector3 &vecRot) { m_vecMoveTargetRot = vecRot; }
	CVector3	GetStartRotation() { return m_vecMoveStartRot; }
	void		SetStartRotation(const CVector3 &vec) { m_vecMoveStartRot = vec; }

	float		GetMoveSpeed() { return m_fMoveSpeed; }
	float		GetMoveDuration() { return m_fMoveDuration; }
	unsigned long GetMoveStartTime() { return m_ulMoveStartTime; }
	void		SetMoveDuration(float duration) { m_fMoveDuration = duration; }
	void		SetMoveStartTime(unsigned long time) { m_ulMoveStartTime = time; }
	void		SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }
	void		MoveObject(const CVector3& vecTarget, const CVector3& vecRot, float fSpeed);

	// Streaming
	void GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void UpdateInterior(unsigned int uiInterior);
	void StreamIn();
	void StreamOut();
};
