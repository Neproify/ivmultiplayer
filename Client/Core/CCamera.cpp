//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCamera.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCamera.h"

CCamera::CCamera()
{
	Scripting::CreateCam(14, &m_uiCamIndex);
	Scripting::SetCamActive(m_uiCamIndex, false);
	m_bActive = false;
}

CCamera::~CCamera()
{
	if(m_bActive)
		Scripting::SetCamActive(m_uiCamIndex, false);

	Scripting::DestroyCam(m_uiCamIndex);
}

void CCamera::Activate()
{
	Scripting::ActivateScriptedCams(true, true);
	Scripting::SetCamActive(m_uiCamIndex, true);
	m_bActive = true;
}

void CCamera::Reset()
{
	if(m_bActive)
	{
		Scripting::SetCamActive(m_uiCamIndex, false);
		Scripting::ActivateScriptedCams(false, false);
		m_vecPosition = CVector3();
		m_vecLookAt = CVector3();
	}
}

void CCamera::SetPostion(CVector3 vecPosition)
{
	if(m_bActive)
	{
		Scripting::SetCamPos(m_uiCamIndex, vecPosition.fX, vecPosition.fY, vecPosition.fZ);
		m_vecPosition = vecPosition;
	}
}

CVector3 CCamera::GetPostion()
{
	return m_vecPosition;
}

void CCamera::SetLookAt(CVector3 vecLookAt)
{
	Scripting::PointCamAtCoord(m_uiCamIndex, vecLookAt.fX, vecLookAt.fY, vecLookAt.fZ);
	m_vecLookAt = vecLookAt;
}

CVector3 CCamera::GetLookAt()
{
	return m_vecLookAt;
}
