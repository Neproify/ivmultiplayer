//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickupManager.cpp
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPickupManager.h"
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

void CPickup::GetStreamPosition(CVector3 * vecCoordinates)
{
	memcpy(vecCoordinates, &m_vecPosition, sizeof(CVector3));
}

void CPickup::StreamIn()
{
	Scripting::CreatePickupRotate((Scripting::eModel)m_dwModelHash, (Scripting::ePickupType)m_ucType, m_uiValue, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ, m_vecRotation.fX,  m_vecRotation.fY, m_vecRotation.fZ, &m_uiPickupHandle);
	Scripting::AddPickupToInteriorRoomByKey(m_uiPickupHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
}

void CPickup::StreamOut()
{
	Scripting::RemovePickup(m_uiPickupHandle);
	m_uiPickupHandle = 0;
}

void CPickup::UpdateInterior(unsigned int uiInterior)
{
	Scripting::AddPickupToInteriorRoomByKey(m_uiPickupHandle, (Scripting::eInteriorRoomKey)uiInterior);
}

void CPickup::SetPosition(CVector3 vecPosition)
{
	m_vecPosition = vecPosition;

	// stream out (will be streamed in eventually)
	if(IsStreamedIn())
		g_pStreamer->ForceStreamOut(this);
}

void CPickup::SetRotation(CVector3 vecRotation)
{
	m_vecRotation = vecRotation;

	// stream out (will be streamed in eventually)
	if(IsStreamedIn())
		g_pStreamer->ForceStreamOut(this);
}

void CPickup::SetValue(unsigned int uiValue)
{
	m_uiValue = uiValue;

	// stream out (will be streamed in eventually)
	if(IsStreamedIn())
		g_pStreamer->ForceStreamOut(this);
}
