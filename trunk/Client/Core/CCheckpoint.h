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

#include "CIVCheckpoint.h"
#include "CStreamer.h"

enum eCheckpointType
{
	CHECKPOINT_TYPE_NONE,
	CHECKPOINT_TYPE_ARROW_TUBE = 2,
	CHECKPOINT_TYPE_FLAG_TUBE,
	CHECKPOINT_TYPE_ARROW_CORONA,
	CHECKPOINT_TYPE_FLAG_CORONA,
	CHECKPOINT_TYPE_ARROW,
	CHECKPOINT_TYPE_FLAG,
	CHECKPOINT_TYPE_CORONA
};

class CCheckpoint : public CStreamableEntity
{
private:
	CIVCheckpoint * m_pCheckpoint;
	EntityId m_checkpointId;
	eCheckpointType m_eType;
	CVector3 m_vecPosition;
	CVector3 m_vecTargetPosition;
	float m_fRadius;
	bool m_bInCheckpoint;
	bool m_bIsVisible;

	static CIVCheckpoint m_checkpointPool[INTERNAL_CHECKPOINT_LIMIT];

public:
	CCheckpoint(EntityId checkpointId, eCheckpointType type, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius);
	~CCheckpoint();

	CIVCheckpoint * GetCheckpoint();
	void Show();
	void Hide();
	bool IsVisible();
	void SetType(eCheckpointType type);
	eCheckpointType GetType();
	void SetPosition(CVector3 vecPosition);
	void GetPosition(CVector3 * vecPosition);
	void SetTargetPosition(CVector3 vecTargetPosition);
	void GetTargetPosition(CVector3 * vecTargetPosition);
	void SetRadius(float fRadius);
	float GetRadius();
	static void Init();

	void GetStreamPosition(CVector3 * vecCoordinates) { GetPosition(vecCoordinates); }
	void StreamIn();
	void StreamOut();
	void Pulse();
};
