//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpoint.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

class CCheckpoint
{
private:
	EntityId m_checkpointId;
	WORD     m_wType;
	CVector3 m_vecPosition;
	CVector3 m_vecTargetPosition;
	float    m_fRadius;
	bool	 m_bShow;

public:
	CCheckpoint(EntityId checkpointId, WORD wType, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius);
	~CCheckpoint();

	EntityId GetCheckpointId() { return m_checkpointId; }
	void     AddForPlayer(EntityId playerId);
	void     AddForWorld();
	void     DeleteForPlayer(EntityId playerId);
	void     DeleteForWorld();
	void     ShowForPlayer(EntityId playerId);
	void     ShowForWorld();
	void     HideForPlayer(EntityId playerId);
	void     HideForWorld();
	void     SetType(WORD wType);
	WORD     GetType() { return m_wType; }
	void     SetPosition(CVector3 vecPosition);
	void     GetPosition(CVector3& vecPosition) { vecPosition = m_vecPosition; }
	void     SetTargetPosition(CVector3 vecTargetPosition);
	void     GetTargetPosition(CVector3& vecTargetPosition) { vecTargetPosition = m_vecPosition; }
	void     SetRadius(float fRadius);
	float    GetRadius() { return m_fRadius; }
};
