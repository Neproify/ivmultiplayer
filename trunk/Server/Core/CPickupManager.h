//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPickupManager.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

struct _Pickup
{
	DWORD dwModelHash;
	unsigned char ucType;
	unsigned int uiValue; // totally guessed that but it might be for ammo
	CVector3 vecPos;
	CVector3 vecRot;
};

class CPickupManager : public CPickupManagerInterface
{
private:
	bool m_bActive[MAX_PICKUPS];
	_Pickup m_Pickups[MAX_PICKUPS];

public:
	CPickupManager();
	~CPickupManager();

	EntityId Create(DWORD dwModelHash, unsigned char ucType, unsigned int uiValue, float fX, float fY, float fZ, float fRX, float fRY, float fRZ);
	void Delete(EntityId pickupId);
	void HandleClientJoin(EntityId playerId);
	bool DoesExist(EntityId pickupId);
	EntityId GetPickupCount();

	DWORD GetModel(EntityId objectId);
	unsigned char GetType(EntityId pickupId);
	bool SetValue(EntityId pickupId, unsigned int pValue);
	unsigned int GetValue(EntityId pickupId);
	bool SetPosition(EntityId pickupId, CVector3 vecPosition);
	bool GetPosition(EntityId pickupId, CVector3* vecPosition);
	bool SetRotation(EntityId pickupId, CVector3 vecRotation);
	bool GetRotation(EntityId pickupId, CVector3* vecRotation);
};
