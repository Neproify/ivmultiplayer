//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCamera.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCamera.h"
#include "CGame.h"
#include "CPools.h"
#include <CLogFile.h>
#include "Scripting.h"
#include "CLocalPlayer.h"

extern CLocalPlayer * g_pLocalPlayer;

CCamera::CCamera()
{
	// Get the game cam pointer
	IVCam * pCam = NULL;
	void * unkn = (void **)(CGame::GetBase() + 0xF21A6C);
	_asm
	{
		mov ecx, unkn
		mov eax, [ecx+0Ch]
		mov pCam, eax
	}

	// Create the game cam instance
	m_pGameCam = new CIVCam(pCam);

	// Create the script cam
	unsigned int uiScriptCam;
	Scripting::CreateCam(14, &uiScriptCam);
	Scripting::SetCamPropagate(uiScriptCam, true);

	// Create the script cam instance
	m_pScriptCam = new CIVCam(CGame::GetPools()->GetCamPool()->AtHandle(uiScriptCam));

	// Flag the script cam as inactive
	m_bScriptCamActive = false;
}

CCamera::~CCamera()
{
	// Deactivate the script cam if needed
	if(m_bScriptCamActive)
		DeactivateScriptCam();

	// Destroy the script cam
	Scripting::DestroyCam(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()));
}

void CCamera::ActivateScriptCam()
{
	// Is the script cam not active?
	if(!m_bScriptCamActive)
	{
		// Activate script cams
		Scripting::ActivateScriptedCams(true, true);

		// Activate the script cam
		Scripting::SetCamActive(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()), true);

		// Flag the script cam as active
		m_bScriptCamActive = true;
	}
}

void CCamera::DeactivateScriptCam()
{
	// Is the script cam active?
	if(m_bScriptCamActive)
	{
		// Deactivate the script cam
		Scripting::SetCamActive(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()), false);

		// Deactivate scripted cams
		Scripting::ActivateScriptedCams(false, false);

		// Flag the script cam as inactive
		m_bScriptCamActive = false;
	}
}

void CCamera::Reset()
{
	// Deactivate the script cam if needed
	if(m_bScriptCamActive)
		DeactivateScriptCam();

	// Set the camera behind the local player
	g_pLocalPlayer->SetCameraBehind();
}

void CCamera::SetBehindPed(CIVPed * pPed)
{
	// Deactivate the script cam if needed
	if(m_bScriptCamActive)
		DeactivateScriptCam();

	// Set the cam behind the specified ped
	Scripting::SetCamBehindPed(CGame::GetPools()->GetPedPool()->HandleOf(pPed->GetPed()));
}

void CCamera::SetPosition(const CVector3& vecPosition)
{
	// Activate the script cam if needed
	if(!m_bScriptCamActive)
		ActivateScriptCam();

	// Set the script cam position
	m_pScriptCam->SetPosition(vecPosition);
}

void CCamera::GetPosition(CVector3& vecPosition)
{
	// Is the script cam active?
	if(m_bScriptCamActive)
	{
		// Get the script cam position
		m_pScriptCam->GetPosition(vecPosition);
	}
	else
	{
		// Get the game cam position
		m_pGameCam->GetPosition(vecPosition);
	}
}

void CCamera::SetLookAt(const CVector3& vecLookAt)
{
	// Activate the script cam if needed
	if(!m_bScriptCamActive)
		ActivateScriptCam();

	// Load the world at the look at
	CGame::GetStreaming()->LoadWorldAtPosition(vecLookAt);

	// Set the script cam look at
	Scripting::PointCamAtCoord(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()), vecLookAt.fX, vecLookAt.fY, vecLookAt.fZ);
}

void CCamera::GetLookAt(CVector3& vecLookAt)
{
	// TODO!
	
}
