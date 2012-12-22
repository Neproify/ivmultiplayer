//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObject.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CObject.h"
#include "CLocalPlayer.h"

extern CLocalPlayer * g_pLocalPlayer;

CObject::CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation)
	: CStreamableEntity(STREAM_ENTITY_OBJECT, 400.0f),
	m_uiObjectHandle(0),
	m_dwModelHash(dwModelHash),
	m_vecPosition(vecPosition),
	m_vecRotation(vecRotation),
	m_bIsMoving(false),
	m_bIsRotating(false)
{
}

CObject::~CObject()
{
	// Tell the streamer to delete us
	OnDelete();
}

bool CObject::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		Scripting::CreateObjectNoOffset((Scripting::eModel)m_dwModelHash, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ, &m_uiObjectHandle, true);
		Scripting::FreezeObjectPosition(m_uiObjectHandle, true);
		Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
		return true;
	}

	return false;
}

void CObject::Destroy()
{
	// Are we spawned?
	if(IsSpawned())
	{
		Scripting::DeleteObject(&m_uiObjectHandle);
		m_uiObjectHandle = 0;
	}
}

void CObject::SetPosition(const CVector3& vecPosition)
{
	// Are we spawned?
	if(IsStreamedIn())
	{
		Scripting::SetObjectCoordinates(m_uiObjectHandle, vecPosition.fX, vecPosition.fY, vecPosition.fZ);
		Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)g_pLocalPlayer->GetInterior());
	}

	m_vecPosition = vecPosition;
}

void CObject::GetPosition(CVector3& vecPosition)
{
	vecPosition = m_vecPosition;
}

void CObject::SetRotation(const CVector3& vecRotation)
{
	// Are we spawned?
	if(IsSpawned())
		Scripting::SetObjectRotation(m_uiObjectHandle, vecRotation.fX, vecRotation.fY, vecRotation.fZ);

	m_vecRotation = vecRotation;
}

void CObject::GetRotation(CVector3& vecRotation)
{
	vecRotation = m_vecRotation;
}

void CObject::StreamIn()
{
	if(Create())
	{
		SetPosition(m_vecPosition);
		SetRotation(m_vecRotation);
	}
}

void CObject::StreamOut()
{
	Destroy();
}

void CObject::UpdateInterior(unsigned int uiInterior)
{
	Scripting::AddObjectToInteriorRoomByKey(m_uiObjectHandle, (Scripting::eInteriorRoomKey)uiInterior);
}
