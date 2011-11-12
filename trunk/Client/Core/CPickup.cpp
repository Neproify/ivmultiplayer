//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickup.cpp
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPickup.h"
#include "CLocalPlayer.h"

extern CLocalPlayer * g_pLocalPlayer;
extern CStreamer * g_pStreamer;

CPickup::CPickup(DWORD dwModelHash, unsigned char ucType, unsigned int uiValue, CVector3 vecPosition, CVector3 vecRotation)
: CStreamableEntity(STREAM_ENTITY_PICKUP, 70.0f)
{
	m_uiPickupHandle = 0;
	m_dwModelHash = dwModelHash;
	m_ucType = ucType;
	m_uiValue = uiValue;
	m_vecPosition = vecPosition;
	m_vecRotation = vecRotation;
}

CPickup::~CPickup()
{
	// Tell the streamer to delete us
	OnDelete();
}

bool CPickup::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		// Create the pickup
		Scripting::CreatePickupRotate((Scripting::eModel)m_dwModelHash, (Scripting::ePickupType)m_ucType, m_uiValue, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ, m_vecRotation.fX,  m_vecRotation.fY, m_vecRotation.fZ, &m_uiPickupHandle);
		Scripting::AddPickupToInteriorRoomByKey(m_uiPickupHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
		return true;
	}

	return false;
}

void CPickup::Destroy()
{
	// Are we spawned?
	if(IsSpawned())
	{
		Scripting::RemovePickup(m_uiPickupHandle);
		m_uiPickupHandle = 0;
	}
}

void CPickup::SetPosition(const CVector3& vecPosition)
{
	m_vecPosition = vecPosition;

	// Are we streamed in?
	if(IsStreamedIn())
	{
		// Stream ourselves out and back in
		StreamOut();
		StreamIn();
	}
}

void CPickup::GetPosition(CVector3& vecPosition)
{
	vecPosition = m_vecPosition;
}

void CPickup::SetRotation(const CVector3& vecRotation)
{
	m_vecRotation = vecRotation;

	// Are we streamed in?
	if(IsStreamedIn())
	{
		// Stream ourselves out and back in
		StreamOut();
		StreamIn();
	}
}

void CPickup::GetRotation(CVector3& vecRotation)
{
	vecRotation = m_vecRotation;
}

void CPickup::SetValue(unsigned int uiValue)
{
	m_uiValue = uiValue;

	// Are we streamed in?
	if(IsStreamedIn())
	{
		// Stream ourselves out and back in
		StreamOut();
		StreamIn();
	}
}

unsigned int CPickup::GetValue()
{
	return m_uiValue;
}

void CPickup::UpdateInterior(unsigned int uiInterior)
{
	// Are we spawned?
	if(IsSpawned())
		Scripting::AddPickupToInteriorRoomByKey(m_uiPickupHandle, (Scripting::eInteriorRoomKey)uiInterior);
}

void CPickup::StreamIn()
{
	Create();
}

void CPickup::StreamOut()
{
	Destroy();
}
