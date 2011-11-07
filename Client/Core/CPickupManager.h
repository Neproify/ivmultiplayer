//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickupManager.h
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CStreamer.h"
#include "Scripting.h"
#include "CNetworkEntityManager.h"

class CPickup : public CStreamableEntity
{
private:
	unsigned int m_uiPickupHandle;
	DWORD m_dwModelHash;
	unsigned char m_ucType;
	unsigned int m_uiValue;
	CVector3 m_vecPosition;
	CVector3 m_vecRotation;

public:
	CPickup(DWORD dwModelHash, unsigned char ucType, unsigned int uiValue, CVector3 vecPosition, CVector3 vecRotation);
	~CPickup();

	void GetStreamPosition(CVector3 * vecCoordinates);
	void UpdateInterior(unsigned int uiInterior);
	void StreamIn();
	void StreamOut();
	void SetPosition(CVector3 vecPosition);
	void SetRotation(CVector3 vecRotation);
	void SetValue(unsigned int uiValue);

};

class CPickupManager : public CNetworkEntityManager<CPickup, MAX_PICKUPS>
{
};
