//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCamera.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVCam.h"
#include "CIVPed.h"

class CCamera
{
private:
	CIVCam * m_pGameCam;
	CIVCam * m_pScriptCam;
	bool     m_bScriptCamActive;

public:
	CCamera();
	~CCamera();

	CIVCam * GetGameCam() { return m_pGameCam; }
	CIVCam * GetScriptCam() { return m_pScriptCam; }
	void     ActivateScriptCam();
	void     DeactivateScriptCam();
	void     Reset();
	void     SetBehindPed(CIVPed * pPed);
	void     SetPosition(const CVector3& vecPosition);
	void     GetPosition(CVector3& vecPosition);
	void     SetLookAt(const CVector3& vecLookAt);
	void     GetLookAt(CVector3& vecLookAt);
	void	 Attach(unsigned int uiHandle, bool bVehicleOrPlayer);
};
