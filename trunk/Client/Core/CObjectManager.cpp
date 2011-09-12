//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CObjectManager.h"
#include "CLocalPlayer.h"

extern CLocalPlayer * g_pLocalPlayer;

CObject::CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation)
	: CStreamableEntity(STREAM_ENTITY_OBJECT, 400.0f)
{
	m_uiObjectHandle = 0;
	m_dwModelHash = dwModelHash;
	m_vecPosition = vecPosition;
	m_vecRotation = vecRotation;
}

CObject::~CObject()
{
	// Tell the streamer to delete us
	OnDelete();
}

void CObject::GetStreamPosition(CVector3 * vecCoordinates)
{
	memcpy(vecCoordinates, &m_vecPosition, sizeof(CVector3));
}

void CObject::StreamIn()
{
	Scripting::CreateObjectNoOffset((Scripting::eModel)m_dwModelHash, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ, &m_uiObjectHandle, true);
	Scripting::SetObjectRotation(m_uiObjectHandle, m_vecRotation.fX, m_vecRotation.fY, m_vecRotation.fZ);
	Scripting::FreezeObjectPosition(m_uiObjectHandle, true);
	Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
}

void CObject::StreamOut()
{
	Scripting::DeleteObject(&m_uiObjectHandle);
	m_uiObjectHandle = 0;
}

void CObject::SetPosition(CVector3 vecPosition)
{
	m_vecPosition = vecPosition;
	if(IsStreamedIn())
	{
		Scripting::SetObjectCoordinates(m_uiObjectHandle, vecPosition.fX, vecPosition.fY, vecPosition.fZ);
		Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
	}
}

void CObject::SetRotation(CVector3 vecRotation)
{
	m_vecRotation = vecRotation;
	if(IsStreamedIn())
		Scripting::SetObjectRotation(m_uiObjectHandle, vecRotation.fX, vecRotation.fY, vecRotation.fZ);
}

void CObject::UpdateInterior(unsigned int uiInterior)
{
	Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)uiInterior);
}
