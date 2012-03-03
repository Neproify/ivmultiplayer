//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CRemotePlayer.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetworkPlayer.h"
#include "Scripting.h"

class CRemotePlayer : public CNetworkPlayer
{
private:
	EntityId m_vehicleId;
	unsigned int m_uiBlipId;
	eStateType m_stateType;
	bool m_bPassenger;
	BYTE m_byteSeatId;
	bool m_helmet;

public:
	CRemotePlayer();
	~CRemotePlayer();	

	bool         Spawn(CVector3 vecSpawnPos, float fSpawnHeading, bool bDontRecreate = false);
	void         Destroy();
	void         Kill();
	void         Init();

	void         StoreOnFootSync(OnFootSyncData * syncPacket);
	void         StoreInVehicleSync(EntityId vehicleId, InVehicleSyncData * syncPacket);
	void         StorePassengerSync(EntityId vehicleId, PassengerSyncData * syncPacket);
	void         StoreSmallSync(SmallSyncData * syncPacket);

	void         SetColor(unsigned int color);
	void         SetStateType(eStateType stateType) { m_stateType = stateType; };
	void		 SetHelmetState(bool on) { m_helmet = on; };
	eStateType   GetStateType() { return m_stateType; };
};
