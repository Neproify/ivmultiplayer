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
#include <Math/CMath.h>
#include "CGraphics.h"

extern CGraphics* g_pGraphics;

extern CLocalPlayer * g_pLocalPlayer;

CCamera::CCamera()
	: m_bScriptCamActive(false)
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

	m_iCameraAttached = 0;
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
	
	m_iCameraAttached = 0;
	m_uiCameraAttachedHandle = -1;
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

void CCamera::SetLookAt(const CVector3& vecLookAt, bool bWorldLoad)
{
	// Activate the script cam if needed
	if(!m_bScriptCamActive)
		ActivateScriptCam();

	// Load the world at the look at
	if(bWorldLoad)
		CGame::GetStreaming()->LoadWorldAtPosition(vecLookAt);

	unsigned int uiCam = CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam());
	// Set the script cam look at | Or we have to use the hook code?(CGame)
	if(uiCam)
		Scripting::PointCamAtCoord(uiCam, vecLookAt.fX, vecLookAt.fY, vecLookAt.fZ);
}

void CCamera::GetLookAt(CVector3& vecLookAt)
{
	// TODO: try to get coords from vec
	// Camera data
	CIVCam * pGameCam = GetGameCam();
	CVector3 vecCamPosition;
	CVector3 vecCamForward;
	CVector3 vecCamLookAt;

	pGameCam->GetPosition(vecCamPosition);
	vecCamForward = pGameCam->GetCam()->m_data1.m_matMatrix.vecForward;
	vecCamLookAt.fX = vecCamPosition.fX + /*floatmul(*/vecCamForward.fX/*, fScale)*/;
	vecCamLookAt.fY = vecCamPosition.fY + /*floatmul(*/vecCamForward.fY/*, fScale)*/;
	vecCamLookAt.fZ = vecCamPosition.fZ + /*floatmul(*/vecCamForward.fZ/*, fScale)*/;

	// Calculate fScale
	CVector3 vecFinalCamLookAt = Math::GetOffsetDegrees(vecCamPosition,vecCamForward);
	float fScale = (vecFinalCamLookAt.Length()/2);

	vecCamLookAt.fX = vecCamPosition.fX + (vecCamForward.fX * fScale);
	vecCamLookAt.fY = vecCamPosition.fY + (vecCamForward.fY * fScale);
	vecCamLookAt.fZ = vecCamPosition.fZ + (vecCamForward.fZ * fScale);

	memcpy(&vecLookAt, &vecCamLookAt, sizeof(CVector3));
}

void CCamera::Attach(unsigned int uiHandle, bool bVehicleOrPlayer, int iPointType, CVector3 vecOffset)
{
	// Check if the scriptcam is activated
	if(!m_bScriptCamActive)
		ActivateScriptCam();

	// Check if we got no handler
	if(uiHandle == -1)
		return;

	if(iPointType == 1) {
		m_iCameraAttached = 1;
		m_uiCameraAttachedHandle = uiHandle;
		// Check if the camera should be attached to a vehicle or player
		if(bVehicleOrPlayer)
			Scripting::AttachCamToVehicle(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()),uiHandle);
		else
			Scripting::AttachCamToPed(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()),uiHandle);

		Scripting::SetCamAttachOffset(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()), vecOffset.fX, vecOffset.fY, vecOffset.fZ);
	}
	else if(iPointType == 2)
	{
		m_iCameraAttached = 2;
		m_uiCameraAttachedHandle = uiHandle;
		// Check if the camera should be attached to a vehicle or player
		if(bVehicleOrPlayer)
			Scripting::PointCamAtVehicle(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()),uiHandle);
		else
			Scripting::PointCamAtPed(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()),uiHandle);

		//Scripting::SetCamAttachOffset(CGame::GetPools()->GetCamPool()->HandleOf(m_pScriptCam->GetCam()), vecOffset.fX, vecOffset.fY, vecOffset.fZ);
	}
}
#include "d3d9.h"
#include "CChatWindow.h"

extern CChatWindow* g_pChatWindow;

bool CCamera::IsOnScreen(const CVector3& vecPosition)
{
#define CVEC_TO_D3DVEC(vec) &D3DXVECTOR3(vec.fX, vec.fY, vec.fZ)
#define D3DVEC_TO_CVEC(vec) &CVector3(vec.x, vec.y, vec.z)
	CVector3 vecCamPos;
	GetGameCam()->GetPosition(vecCamPos);

	CVector3 vecCamLookAt;
	GetLookAt(vecCamLookAt);

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, CVEC_TO_D3DVEC(vecCamPos), CVEC_TO_D3DVEC(vecCamLookAt), &D3DXVECTOR3(0, 0, 1));

	D3DVIEWPORT9 viewport;
	g_pGraphics->GetDevice()->GetViewport(&viewport);

	DWORD dwLenX = viewport.Width;
	DWORD dwLenY = viewport.Height;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, GetGameCam()->GetCam()->m_data1.m_fFOV, (float)dwLenX / (float)dwLenY, GetGameCam()->GetCam()->m_data1.m_fNear, GetGameCam()->GetCam()->m_data1.m_fFar); 

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXVECTOR3 vecSPos;
	D3DXVec3Project(&vecSPos, CVEC_TO_D3DVEC(vecPosition), &viewport, &matProj, &matView, &matWorld);

	//g_pChatWindow->AddInfoMessage("W2S (%f|%f|%f)", vecSPos.x, vecSPos.y, vecSPos.z);

	return ( vecSPos.z < 1.f );
}