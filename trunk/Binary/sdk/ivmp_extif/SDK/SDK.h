//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Definitions.h
//===============================================================================

#pragma once

#include <string.h>
#include <Squirrel/squirrel.h>
#include "Interfaces/InterfaceCommon.h"

#ifndef _LINUX
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

#define MAX_VEHICLES 0xFFFE
#define MAX_OBJECTS 0xFFFE
#define MAX_CHECKPOINTS 0xFFFE
#define MAX_BLIPS 1300
#define MAX_ACTORS 100
#define MAX_PICKUPS 0xFFFE
#define MAX_NAME 32

// Define used for invalid entity ids
#define INVALID_ENTITY_ID 0xFFFF

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction);

struct InterfaceContainer_t
{
	CNetworkManagerInterface* g_pNetworkManager;
	CPlayerManagerInterface* g_pPlayerManager;
	CVehicleManagerInterface* g_pVehicleManager;
	CObjectManagerInterface* g_pObjectManager;
	CBlipManagerInterface* g_pBlipManager;
	CActorManagerInterface* g_pActorManager;
	CPickupManagerInterface* g_pPickupManager;
	CCheckpointManagerInterface* g_pCheckpointManager;
	void* g_pModelManager;
	void* g_pScriptingManager;
	CModuleManagerInterface* g_pModuleManager;
	void* g_pRakServer;
	void* g_pConfig;
	CTimeInterface* g_pTime;
	CTrafficLightsInterface* g_pTrafficLights;
	CEventsInterface* g_pEvents;
	SquirrelArgumentManager* pSquirrelArgumentManager;
};

extern InterfaceContainer_t InterfaceContainer;

inline CNetworkManagerInterface * GetNetworkManager()
{
	return InterfaceContainer.g_pNetworkManager;
}

inline CPlayerManagerInterface * GetPlayerManager()
{
	return InterfaceContainer.g_pPlayerManager;
}

inline CVehicleManagerInterface * GetVehicleManager()
{
	return InterfaceContainer.g_pVehicleManager;
}

inline CObjectManagerInterface * GetObjectManager()
{
	return InterfaceContainer.g_pObjectManager;
}

inline CBlipManagerInterface * GetBlipManager()
{
	return InterfaceContainer.g_pBlipManager;
}

inline CActorManagerInterface * GetActorManager()
{
	return InterfaceContainer.g_pActorManager;
}

inline CPickupManagerInterface * GetPickupManager()
{
	return InterfaceContainer.g_pPickupManager;
}

inline CCheckpointManagerInterface * GetCheckpointManager()
{
	return InterfaceContainer.g_pCheckpointManager;
}

inline CModuleManagerInterface * GetModuleManager()
{
	return InterfaceContainer.g_pModuleManager;
}

inline CTimeInterface * GetTimeManager()
{
	return InterfaceContainer.g_pTime;
}

inline CTrafficLightsInterface * GetTrafficLights()
{
	return InterfaceContainer.g_pTrafficLights;
}

inline CEventsInterface * GetEventManager()
{
	return InterfaceContainer.g_pEvents;
}

inline SquirrelArgumentManager * GetSquirrelArgumentManager()
{
	return InterfaceContainer.pSquirrelArgumentManager;
}

