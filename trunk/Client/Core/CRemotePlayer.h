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
	EntityId			m_vehicleId;
	eStateType			m_stateType;
	int					m_iModelId;
	bool				m_bPassenger;
	BYTE				m_byteSeatId;
	bool				m_helmet;
	bool				m_bAnimating;
	String				m_strAnimGroup;
	String				m_strAnimSpec;
	OnFootSyncData	   *m_pLastSyncData;
	bool				m_bStoreOnFootSwitch;
	int					m_iOldMoveStyle;

public:
	CRemotePlayer();
	~CRemotePlayer();	

	bool         Spawn(int iModelId, CVector3 vecSpawnPos, float fSpawnHeading, int ucDimension, bool bDontRecreate = false);
	void         Destroy();
	void         Kill();
	void         Init();

	void         StoreOnFootSync(OnFootSyncData * syncPacket, bool bHasAimSyncData);
	void         StoreInVehicleSync(EntityId vehicleId, InVehicleSyncData * syncPacket);
	void         StorePassengerSync(EntityId vehicleId, PassengerSyncData * syncPacket);
	void         StoreSmallSync(SmallSyncData * syncPacket);

	void         SetColor(unsigned int color);
	void         SetStateType(eStateType stateType) { m_stateType = stateType; };
	void		 SetHelmetState(bool on) { m_helmet = on; };
	eStateType   GetStateType() { return m_stateType; };
};
