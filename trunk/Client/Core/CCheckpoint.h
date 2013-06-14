//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpoint.h
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IV/CIVCheckpoint.h"
#include "CStreamer.h"

class CCheckpoint : public CStreamableEntity
{
private:
	CIVCheckpoint      * m_pCheckpoint;
	EntityId             m_checkpointId;
	eCheckpointType      m_eType;
	CVector3             m_vecPosition;
	CVector3             m_vecTargetPosition;
	float                m_fRadius;
	bool                 m_bInCheckpoint;
	bool                 m_bIsVisible;

public:
	CCheckpoint(EntityId checkpointId, eCheckpointType type, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius);
	~CCheckpoint();

	CIVCheckpoint * GetCheckpoint();
	void            Show();
	void            Hide();
	bool            IsVisible();
	void            SetType(eCheckpointType type);
	eCheckpointType GetType();
	void            SetPosition(const CVector3& vecPosition);
	void            GetPosition(CVector3& vecPosition);
	void            SetTargetPosition(const CVector3& vecTargetPosition);
	void            GetTargetPosition(CVector3& vecTargetPosition);
	void            SetRadius(float fRadius);
	float           GetRadius();
	void            Pulse();

	// Streaming
	void            GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void            StreamIn();
	void            StreamOut();
};
