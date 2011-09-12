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

#define VAR_PedPool_7 0x18A82AC
#define VAR_VehiclePool_7 0x1619240
#define VAR_PtrNodeSinglePool_7 0x16B7758
#define VAR_PtrNodeDoublePool_7 0x16B775C
#define VAR_EntryInfoNodesPool_7 0x1706E98
#define VAR_AnimatedBuildingPool_7 0x1706E94
#define VAR_BuildingPool_7 0x168FED0
#define VAR_ObjectPool_7 0x1350CE0
#define VAR_DummyObjectPool_7 0x168FF90
#define VAR_InteriorInstPool_7 0x165D2E4
#define VAR_PortalInstPool_7 0x16C3364
#define VAR_AnimBlenderPool_7 0x16397B0
#define VAR_AtdNodeAnimChangePooledObjectPool_7 0x16397B8
#define VAR_AtdNodeAnimPlayerPool_7 0x16397B4
#define VAR_crFrameFilterBoneMaskPool_7 0x16397BC
#define VAR_crFrameFilterBoneAnaloguePool_7 0x16397C0
#define VAR_crExpressionProcessorPool_7 0x16397C4
#define VAR_crmtObserverPool_7 0x16397C8
#define VAR_TaskPool_7 0x164B01C
#define VAR_DummyTaskPool_7 0x1706E8C
#define VAR_EventPool_7 0x152F4B4
#define VAR_PointRoutePool_7 0x1706E80
#define VAR_PatrolRoutePool_7 0x1706E88
#define VAR_NavMeshRoutePool_7 0x1706E84
#define VAR_CamPool_7 0x1618020
#define VAR_ExplosionTypePool_7 0x1706E78
#define VAR_PedMoveBlendPool_7 0x18A82B4
#define VAR_ParticleSystemPool_7 0x169F4C4
#define VAR_VehicleStructPool_7 0x16D6594
#define VAR_PedBasePool_7 0x18A82B8
#define VAR_TaskInfoPool_7 0x1618040
#define VAR_PedAttractorPool_7 0x17168BC
#define VAR_TargettingPool_7 0x1711134 // look into this...
#define VAR_PlayerInfoArray 0x11A7008
#define VAR_LocalPlayerId_7 0xF1CC68
#define PLAYER_INFO_ARRAY_SIZE 32

template <class T>
class CIVPool;

class CPools
{
private:
	CIVPool<IVPed>     * m_pPedPool;
	CIVPool<IVVehicle> * m_pVehiclePool;
	//#define VAR_BuildingPool_7 0x168FED0
	//#define VAR_ObjectPool_7 0x1350CE0
	CIVPool<IVTask>    * m_pTaskPool;
	//#define VAR_EventPool_7 0x152F4B4
	//#define VAR_CamPool_7 0x1618020
	//#define VAR_TaskInfoPool_7 0x1618040

public:
	void                 Initialize();
	void                 Shutdown();

	// Pools
	CIVPool<IVPed>     * GetPedPool() { return m_pPedPool; }
	CIVPool<IVVehicle> * GetVehiclePool() { return m_pVehiclePool; }
	CIVPool<IVTask>    * GetTaskPool() { return m_pTaskPool; }

	// Player Infos (An array not a pool)
	IVPlayerInfo       * GetPlayerInfoFromIndex(unsigned int uiIndex);
	IVPlayerInfo       * GetPlayerInfoFromPlayerPed(IVPlayerPed * pPlayerPed);
	unsigned int         GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo);
	void                 SetPlayerInfoAtIndex(unsigned int uiIndex, IVPlayerInfo * pPlayerInfo);
	unsigned int         FindFreePlayerInfoIndex();

	// Current Player Info Index (Doesn't really belong here, but it was the only place to put it)
	unsigned int         GetLocalPlayerIndex();
	void                 SetLocalPlayerIndex(unsigned int uiIndex);
};
