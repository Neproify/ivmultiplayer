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
#include <SharedUtility.h>

extern CLocalPlayer * g_pLocalPlayer;

CObject::CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation)
	: CStreamableEntity(STREAM_ENTITY_OBJECT, 400.0f),
	m_uiObjectHandle(0),
	m_dwModelHash(dwModelHash),
	m_vecPosition(vecPosition),
	m_vecRotation(vecRotation)
{
	m_interp.pos.ulFinishTime = 0;
	m_interp.rot.ulFinishTime = 0;
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

void CObject::Process()
{
	// Do we need to interpolate movement?
	if(m_interp.pos.ulFinishTime != 0)
	{
		// Get the factor of time spend from start time to current time
		float fAlpha = Math::UnlerpClamped(m_interp.pos.ulStartTime, SharedUtility::GetTime(), m_interp.pos.ulFinishTime);

		// Have we finished interpolating?
		if(fAlpha == 1.0f)
			m_interp.pos.ulFinishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = Math::Lerp(m_interp.pos.vecStart, fAlpha, m_interp.pos.vecTarget);

		// Set the new position
		SetPosition(vecNewPosition);
	}

	// Do we need to interpolate rotation?
	if(m_interp.rot.ulFinishTime != 0)
	{
		// Get the factor of time spend from start time to current time
		float fAlpha = Math::UnlerpClamped(m_interp.rot.ulStartTime, SharedUtility::GetTime(), m_interp.rot.ulFinishTime);

		// Have we finished interpolating?
		if(fAlpha == 1.0f)
			m_interp.rot.ulFinishTime = 0;

		// Calculate the new rotation
		CVector3 vecNewRotation = Math::Lerp(m_interp.rot.vecStart, fAlpha, m_interp.rot.vecTarget);

		// Set the new rotation
		SetRotation(vecNewRotation);
	}
}

void CObject::Move(const CVector3& vecPosition, unsigned int uiTime)
{
	GetPosition(m_interp.pos.vecStart);
	m_interp.pos.vecTarget = vecPosition;
	unsigned int ulCurrentTime = SharedUtility::GetTime();
	m_interp.pos.ulStartTime = ulCurrentTime;
	m_interp.pos.ulFinishTime = (ulCurrentTime + uiTime);
}

void CObject::Rotate(const CVector3& vecRotation, unsigned int uiTime)
{
	GetRotation(m_interp.rot.vecStart);
	m_interp.rot.vecTarget = vecRotation;
	unsigned int ulCurrentTime = SharedUtility::GetTime();
	m_interp.rot.ulStartTime = ulCurrentTime;
	m_interp.rot.ulFinishTime = (ulCurrentTime + uiTime);
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
