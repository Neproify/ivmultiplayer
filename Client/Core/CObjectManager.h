//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.h
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================
// FIXUPDATE
// Don't use natives for this

#pragma once

#include "CStreamer.h"
#include "Scripting.h"
#include "CNetworkEntityManager.h"

class CObject : public CStreamableEntity
{
private:
	unsigned int m_uiObjectHandle;
	DWORD        m_dwModelHash;
	CVector3     m_vecPosition;
	CVector3     m_vecRotation;

public:
	CObject(DWORD dwModelHash, CVector3 vecPosition, CVector3 vecRotation);
	~CObject();

	void GetStreamPosition(CVector3 * vecCoordinates);
	void UpdateInterior(unsigned int uiInterior);
	void SetPosition(CVector3 vecPosition);
	void SetRotation(CVector3 vecRotation);
	void StreamIn();
	void StreamOut();
};

class CObjectManager : public CNetworkEntityManager<CObject, MAX_OBJECTS>
{
};
