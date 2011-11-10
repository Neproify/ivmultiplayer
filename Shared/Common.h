//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Common.h
// Project: Shared
// Author(s): jenksta
//            TrojaA
//            Einstein
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Common namespace?
// TODO: Move most (If not all) of these structures to their own class

#pragma once

// TODO: Move this and sync structures to separate file
// or do it like n:iv with Serialize and Deserialize functions
#include "Math/CMath.h"
#include "Game/CPadState.h"

// Linux stuff
#ifdef _LINUX
#include <Linux.h>
#endif

// Compile modes
#define COMPILE_MODE_CUSTOM 0
#define COMPILE_MODE_RELEASE 1

// Compile mode
#define COMPILE_MODE COMPILE_MODE_RELEASE

// Debug mode
//#define IVMP_DEBUG 1

// If compiling in debug mode force mod debug mode
#ifdef _DEBUG
#undef IVMP_DEBUG
#define IVMP_DEBUG 1
#endif

// Network module version
#define NETWORK_MODULE_VERSION 0x04

// Network version
#define NETWORK_VERSION 0x6A

// Tick Rate
#define TICK_RATE 100

// Defines used for the max amount of entities we (IV:MP, not GTA) can handle
// jenksta: although they may be streamed, shouldn't they at least have some sensible limit?
// NOTE: (if client-side entitys are introduced, those should not use ids from the same range as server ids)
#define MAX_PLAYERS 32 // Player Info Array Size: 32 // Ped Pool Size: 140
#define MAX_VEHICLES 0xFFFE // Streamed. See note on Pickups. Vehicle Pool Size: 140
#define MAX_OBJECTS 0xFFFE // Streamed. See note on Pickups. Object Pool Size: 1300
#define MAX_CHECKPOINTS 0xFFFE // Streamed. Checkpoint Pool Size: See CStreamer.h
#define MAX_BLIPS 1300 // Blip Pool Size: 1500
#define MAX_ACTORS 100 // Ped Pool Size: 140
#define MAX_PICKUPS 0xFFFE // Streamed. Pickup Pool Size: TODO: 1500?

// Max amount of characters a player can use in their name
#define MAX_NAME 32

// Max vehicle passengers (NOTE: This is also a GTA limit)
#define MAX_VEHICLE_PASSENGERS 8

// Type used for entity ids
// NOTE: MUST be unsigned!
typedef unsigned short EntityId;

// Define used for invalid entity ids
#define INVALID_ENTITY_ID 0xFFFF

// The Triangle Camera Coordinates
#define TRIANGLE_CAMERA_POS_OLD -230.82f, 287.53f, 69.43f
#define TRIANGLE_CAMERA_POS -203.221664f, 265.353424f, 86.0f
#define TRIANGLE_CAMERA_LOOK_AT_OLD -199.46f, 382.55f, 87.24f
#define TRIANGLE_CAMERA_LOOK_AT -228.926285f, 458.884644f, 58.0f

// Happiness Island Camera Coordinates
#define HAPPINESS_CAMERA_POS -593.514526f, -590.455505f, 122.936066f
#define HAPPINESS_CAMERA_LOOK_AT -608.250122f, -744.111877f, 20.703838f

// Operating system string
#ifdef WIN32
#define OS_STRING "Windows"
#else
#define OS_STRING "Linux"
#endif

// Mod name
#define MOD_NAME "IV:MP"

// Version defines
#ifdef IVMP_DEBUG
#define DEBUG_IDENTIFIER " - Debug"
#else
#define DEBUG_IDENTIFIER
#endif

// Mod version string
#define MOD_VERSION_STRING "0.1 T4"

// Query port offset
#define QUERY_PORT_OFFSET 137

// Masterlist information
#if COMPILE_MODE == COMPILE_MODE_CUSTOM
#define MASTERLIST_ADDRESS ""
#define MASTERLIST_VERSION ""
#define MASTERLIST_TIMEOUT 0
#else if COMPILE_MODE == COMPILE_MODE_RELEASE
#define MASTERLIST_ADDRESS "www.master.iv-multiplayer.com"
#define MASTERLIST_VERSION "01T4"
#define MASTERLIST_TIMEOUT 50000
#endif

// Version identifiers
#define VERSION_IDENTIFIER MOD_NAME " " MOD_VERSION_STRING " (" __DATE__ ", " __TIME__ ")" DEBUG_IDENTIFIER
#define VERSION_IDENTIFIER_2 MOD_NAME " " MOD_VERSION_STRING DEBUG_IDENTIFIER

// Vehicle entry/exit rpc types
enum eVehicleEntryExit
{
	VEHICLE_ENTRY_REQUEST,
	VEHICLE_ENTRY_RETURN,
	VEHICLE_ENTRY_CANCELLED,
	VEHICLE_ENTRY_COMPLETE,
	VEHICLE_EXIT_REQUEST,
	VEHICLE_EXIT_RETURN,
	VEHICLE_EXIT_COMPLETE,
	VEHICLE_EXIT_FORCEFUL,
	VEHICLE_ENTRY_EXIT_COUNT
};

// Array length macro
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

// Safe memory macros
#define SAFE_DELETE(memory) if(memory) { delete memory; memory = NULL; }
#define SAFE_FREE(memory) if(memory) { free(memory); memory = NULL; }

// Bit manipulation macros
#define SET_BIT(a, b) a |= b
#define IS_BIT_SET(a, b) ((a & b) != 0)
#define UNSET_BIT(a, b) a &= ~(b)

// Library extension
#ifdef WIN32
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_EXTENSION ".so"
#endif

// Library Names
#define CLIENT_CORE_NAME "Client.Core"
#define CLIENT_LAUNCH_HELPER_NAME "Client.LaunchHelper"
#define SERVER_CORE_NAME "Server.Core"
#define NETWORK_MODULE_NAME "Network.Core"
#define VFS_MODULE_NAME "Client.VFS"

// Library Exports
#ifdef EXPORT
#undef EXPORT
#endif
#ifdef WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

// Debug Commands Enabled
//#define DEBUG_COMMANDS_ENABLED

struct OnFootSyncData
{
	CPadState padState;              // pad state
	CVector3 vecPos;                 // player position
	float fHeading;                  // player heading
	CVector3 vecMoveSpeed;           // player move speed
	bool bDuckState : 1;             // ducking
	unsigned int uHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uWeaponInfo;        // player weapon and ammo
};

struct InVehicleSyncData
{
	CPadState padState;                    // pad state
	CVector3 vecPos;                       // vehicle position
	CVector3 vecRotation;                  // vehicle rotation
	unsigned int uiHealth : 16;            // vehicle health
	unsigned char byteColors[4];           // vehicle colors
	CVector3 vecTurnSpeed;                 // vehicle turn speed
	CVector3 vecMoveSpeed;                 // vehicle move speed
	float fDirtLevel;                      // vehicle dirt level
	bool bSirenState : 1;                  // siren state
	bool bEngineStatus : 1;
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo;        // player weapon and ammo
};

struct PassengerSyncData
{
	CPadState padState;                    // pad state
	unsigned char byteSeatId;              // vehicle seat id
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo;        // player weapon and ammo
};

struct SmallSyncData
{
	CPadState padState;       // pad state
	bool bDuckState : 1;      // ducking
	unsigned int uWeaponInfo; // weapon and ammo
};

struct EMPTYVEHICLESYNCPACKET
{
	EntityId playerId;
	EntityId vehicleId;
	CVector3 vecPos;
	float fHeading;
	//Quaternion quatQuaternion;
	CVector3 vecTurnSpeed;
	CVector3 vecMoveSpeed;
	float fHealth;
};

struct AimSyncData
{
	CVector3 vecRight;
	CVector3 vecForward;
	CVector3 vecUp;
	CVector3 vecPosition;
};

// Refuse Reasons
enum eRefuseReason
{
	REFUSE_REASON_INVALIDVERSION,
	REFUSE_REASON_TOOSHORT,
	REFUSE_REASON_TOOLONG,
	REFUSE_REASON_INUSE,
	REFUSE_REASON_INVALIDNAME,
	REFUSE_REASON_FILES_MODIFIED
};

// State types
enum eStateType
{
	STATE_TYPE_DISCONNECT,
	STATE_TYPE_CONNECT,
	STATE_TYPE_SPAWN,
	STATE_TYPE_DEATH,
	STATE_TYPE_ONFOOT,
	STATE_TYPE_ENTERVEHICLE,
	STATE_TYPE_INVEHICLE,
	STATE_TYPE_PASSENGER,
	STATE_TYPE_EXITVEHICLE
};
