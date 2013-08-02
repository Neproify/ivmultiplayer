//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpoint.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCheckpoint.h"
#include "COffsets.h"
#include "CClient.h"
#include "CPools.h"

extern CClient * g_pClient;

CCheckpoint::CCheckpoint(EntityId checkpointId, eCheckpointType type, CVector3 vecPosition, CVector3 vecTargetPosition, float fRadius)
	: CStreamableEntity(STREAM_ENTITY_CHECKPOINT, 5.0f * fRadius + 250.0f),
	m_pCheckpoint(NULL),
	m_checkpointId(checkpointId),
	m_eType(type),
	m_vecPosition(vecPosition),
	m_vecTargetPosition(vecTargetPosition),
	m_fRadius(fRadius),
	m_bInCheckpoint(false),
	m_bIsVisible(true)
{
}

CCheckpoint::~CCheckpoint()
{
	// Tell the streamer to delete us
	OnDelete();
}

CIVCheckpoint * CCheckpoint::GetCheckpoint()
{
	return m_pCheckpoint;
}

void CCheckpoint::Show()
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetActiveState(1);

	m_bIsVisible = true;
}

void CCheckpoint::Hide()
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetActiveState(0);

	m_bIsVisible = false;
}

bool CCheckpoint::IsVisible()
{
	if(m_pCheckpoint)
		return (m_pCheckpoint->GetActiveState() != 0);

	return m_bIsVisible;
}

void CCheckpoint::SetType(eCheckpointType type)
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetType(type);

	m_eType = type;
}

eCheckpointType CCheckpoint::GetType()
{
	return m_eType;
}

void CCheckpoint::SetPosition(const CVector3& vecPosition)
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetPosition(vecPosition);

	m_vecPosition = vecPosition;
}

void CCheckpoint::GetPosition(CVector3& vecPosition)
{
	if(m_pCheckpoint)
		m_pCheckpoint->GetPosition(vecPosition);
	else
		vecPosition = m_vecPosition;
}

void CCheckpoint::SetTargetPosition(const CVector3& vecTargetPosition)
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetTargetPosition(vecTargetPosition);

	m_vecTargetPosition = vecTargetPosition;
}

void CCheckpoint::GetTargetPosition(CVector3& vecTargetPosition)
{
	if(m_pCheckpoint)
		m_pCheckpoint->GetTargetPosition(vecTargetPosition);
	else
		vecTargetPosition = m_vecTargetPosition;
}

void CCheckpoint::SetRadius(float fRadius)
{
	if(m_pCheckpoint)
		m_pCheckpoint->SetRadius(fRadius);

	m_fRadius = fRadius;
}

float CCheckpoint::GetRadius()
{
	return m_fRadius;
}

void CCheckpoint::Pulse()
{
	// Are we not visible?
	if(!m_bIsVisible)
		return;

	CVector3 vecPosition;
	g_pClient->GetLocalPlayer()->GetPosition(vecPosition);

	CVector3 vecDistance((m_vecPosition.fX - vecPosition.fX), (m_vecPosition.fY - vecPosition.fY), (m_vecPosition.fZ - vecPosition.fZ));

	// The actual radius of a checkpoint is bigger than its visible entity so we'll add a factor of 5.0
	if(sqrt((vecDistance.fX * vecDistance.fX) + (vecDistance.fY * vecDistance.fY) + (vecDistance.fZ * vecDistance.fZ)) <= (m_fRadius * 5.0f))
	{
		if(!m_bInCheckpoint)
		{
			CBitStream  bsSend;
			bsSend.Write(m_checkpointId);
			g_pClient->GetNetworkManager()->RPC(RPC_CheckpointEntered, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
			m_bInCheckpoint = true;
		}
	}
	else
	{
		if(m_bInCheckpoint)
		{
			CBitStream  bsSend;
			bsSend.Write(m_checkpointId);
			g_pClient->GetNetworkManager()->RPC(RPC_CheckpointLeft, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
			m_bInCheckpoint = false;
		}
	}
}

void CCheckpoint::StreamIn()
{
	// Find a free checkpoint index
	unsigned int uiCheckpoint = CGame::GetPools()->FindFreeCheckpointIndex();

	// Invalid checkpoint index?
	if(uiCheckpoint == INVALID_CHECKPOINT)
		return;

	// Create checkpoint instance
	m_pCheckpoint = new CIVCheckpoint(CGame::GetPools()->GetCheckpointFromIndex(uiCheckpoint));

	// Set checkpoint info
	m_pCheckpoint->SetType(m_eType);
	m_pCheckpoint->SetPosition(m_vecPosition);
	m_pCheckpoint->SetTargetPosition(m_vecTargetPosition);
	m_pCheckpoint->SetRadius(m_fRadius);
	m_pCheckpoint->SetActiveState(m_bIsVisible);
}

void CCheckpoint::StreamOut()
{
	// Reset the checkpoint
	memset(m_pCheckpoint->GetCheckpoint(), 0, sizeof(IVCheckpoint));

	// Delete our checkpoint instance
	SAFE_DELETE(m_pCheckpoint);
}
