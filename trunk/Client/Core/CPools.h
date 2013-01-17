//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPools.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVPool.h"
#include "CIVPlayerPed.h"
#include "CIVVehicle.h"
#include "CIVTask.h"
#include "CIVPlayerInfo.h"
#include "CIVCam.h"
#include "CIVCheckpoint.h"
#include "CIVTrain.h"

// Player info array size
#define PLAYER_INFO_ARRAY_SIZE 32 // 32

// Custom (increased) checkpoint array size
#define CHECKPOINT_ARRAY_SIZE 128

// Invalid checkpoint array index
#define INVALID_CHECKPOINT 255

template <class T>
class CIVPool;

class CPools
{
private:
	// Game pools
	CIVPool<IVPed>     * m_pPedPool;
	CIVPool<IVVehicle> * m_pVehiclePool; // Size: 140
	//#define VAR_BuildingPool_7 0x168FED0
	//#define VAR_ObjectPool_7 0x1350CE0
	CIVPool<IVTask>    * m_pTaskPool; // Size: 1200
	//#define VAR_EventPool_7 0x152F4B4 // Size: 300
	CIVPool<IVCam>     * m_pCamPool;
	//#define VAR_TaskInfoPool_7 0x1618040
	CIVPool<IVTrain>	* m_pTrainPool;

	// Custom checkpoint array
	IVCheckpoint         m_checkpoints[CHECKPOINT_ARRAY_SIZE];

public:
	CPools();
	~CPools();

	void                 Initialize();

	// Pools
	CIVPool<IVPed>     * GetPedPool() { return m_pPedPool; }
	CIVPool<IVVehicle> * GetVehiclePool() { return m_pVehiclePool; }
	CIVPool<IVTask>    * GetTaskPool() { return m_pTaskPool; }
	CIVPool<IVCam>     * GetCamPool() { return m_pCamPool; }

	// Player Infos (An array not a pool)
	IVPlayerInfo       * GetPlayerInfoFromIndex(unsigned int uiIndex);
	IVPlayerInfo       * GetPlayerInfoFromPlayerPed(IVPlayerPed * pPlayerPed);
	unsigned int         GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo);
	void                 SetPlayerInfoAtIndex(unsigned int uiIndex, IVPlayerInfo * pPlayerInfo);
	unsigned int         FindFreePlayerInfoIndex();

	// Current Player Info Index (Doesn't really belong here, but it was the only place to put it)
	unsigned int         GetLocalPlayerIndex();
	void                 SetLocalPlayerIndex(unsigned int uiIndex);

	// Checkpoints (An array not a pool)
	IVCheckpoint       * GetCheckpointFromIndex(unsigned int uiIndex);
	unsigned int         FindFreeCheckpointIndex();
};
