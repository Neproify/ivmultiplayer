//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCamera.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// FIXUPDATE
// Either update this (and not use scripting functions) and use it
// or remove it

#pragma once

#include "Scripting.h"

class CCamera
{
private:
	unsigned int m_uiCamIndex;
	bool         m_bActive;
	CVector3     m_vecPosition;
	CVector3     m_vecLookAt;

public:
	CCamera();
	~CCamera();

	void Activate();
	void Reset();
	void SetPostion(CVector3 vecPosition);
	CVector3 GetPostion();
	void SetLookAt(CVector3 vecLookAt);
	CVector3 GetLookAt();
};
