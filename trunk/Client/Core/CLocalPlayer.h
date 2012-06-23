//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CLocalPlayer.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include "CNetworkPlayer.h"

class CLocalPlayer : public CNetworkPlayer
{
private:
	bool				m_bIsDead;
	CVector3			m_vecSpawnPosition;
	float				m_fSpawnAngle;
	bool				m_bToggleControl;
	unsigned long		m_ulLastPureSyncTime;
	unsigned int		m_uiLastInterior;
	bool				m_bDisableVehicleInfo;
	CControlState		m_lastControlStateSent;
	/*bool			    m_bAnimating;
	char*				m_strAnimGroup;
	char*				m_strAnimSpec;*/

public:
	CLocalPlayer();
	~CLocalPlayer();

	void           Respawn();
	void           HandleSpawn();
	void           DoDeathCheck();
	void           Pulse();
	void           SetSpawnLocation(CVector3 vecPosition, float fHeading);
	void           SetPlayerControlAdvanced(bool bControl, bool bCamera);
	void           SendOnFootSync();
	void           SendInVehicleSync();
	void           SendPassengerSync();
	void           SendSmallSync();
	bool           IsPureSyncNeeded();
	bool           IsSmallSyncNeeded();
	unsigned short GetPing();
	void           GetSpawnPosition(CVector3 * vecPosition) { memcpy(vecPosition, &m_vecSpawnPosition, sizeof(CVector3)); }
	float          GetSpawnRotation() { return m_fSpawnAngle; }
	void		   SetControl(bool control);
	bool		   GetControl();
	bool		   GetVehicleInfos() { return m_bDisableVehicleInfo; }
	void		   SetVehicleInfos(bool bInfo) { m_bDisableVehicleInfo = bInfo; }
	/*void		   SetAnimation(const char * strGroup, const char * strAnim);*/
};
