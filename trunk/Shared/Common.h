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

// TODO: Move this and sync structures to seperate file
// or do it like n:iv with Serialize and Deserialize functions
#include <Math/CMath.h>

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
#if COMPILE_MODE == COMPILE_MODE_CUSTOM
#define NETWORK_MODULE_VERSION 0x00
#else if COMPILE_MODE == COMPILE_MODE_RELEASE
#define NETWORK_MODULE_VERSION 0x03
#endif

// Network version
#if COMPILE_MODE == COMPILE_MODE_CUSTOM
#define NETWORK_VERSION 0x00
#else if COMPILE_MODE == COMPILE_MODE_RELEASE
#define NETWORK_VERSION 0x68
#endif

// Tick Rate
#define TICK_RATE 100

// Define used to limit player slots for a scripting release
#define SCRIPTING_RELEASE

// Defines used for the max amount of entities we (IV:MP, not GTA) can handle
// jenksta: although they may be streamed, shouldn't they at least have some sensible limit?
// NOTE: (if client-side entitys are introduced, those should not use ids from the same range as server ids)
#ifdef SCRIPTING_RELEASE
#define MAX_PLAYERS 6 // Player Pool Size: 32 // Ped Pool Size: 140
#else
#define MAX_PLAYERS 32 // Player Pool Size: 32 // Ped Pool Size: 140
#endif
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

enum eNetPadKeys
{
	// On Foot/In Vehicle
	NET_PAD_KEY_ENTEREXIT_VEHICLE = 1, // Enter/Exit Vehicle (Key 3)

	// On Foot
	NET_PAD_KEY_SPRINT = 2, // Sprint (Key 1)
	NET_PAD_KEY_JUMP = 4, // Jump (Key 2)
	NET_PAD_KEY_ATTACK = 8, // Attack (Key 4)
	NET_PAD_KEY_FREE_AIM_1 = 16, // Free Aim/Melee Lock On (Key 5)
	NET_PAD_KEY_FREE_AIM_2 = 32, // Free Aim/Melee Lock On 2 (Key 6)
	NET_PAD_KEY_MOUSE_AIM = 64, // Mouse Aim (Key 87)
	NET_PAD_KEY_COMBAT_PUNCH_1 = 128, // Combat Punch 1 (Key 59)
	NET_PAD_KEY_COMBAT_PUNCH_2 = 256, // Combat Punch 2 (Key 60)
	NET_PAD_KEY_COMBAT_KICK = 512, // Combat Kick (Key 62)
	NET_PAD_KEY_COMBAT_BLOCK = 1024, // Combat Block (Key 63)

	// In Vehicle
	NET_PAD_KEY_ACCELERATE = 2048, // Accelerate (Key 40)
	NET_PAD_KEY_REVERSE = 4096, // Reverse (Key 41)
	NET_PAD_KEY_HANDBRAKE_1 = 8192, // Handbrake (Key 44)
	NET_PAD_KEY_HANDBRAKE_2 = 16384, // Handbrake 2 (Key 45)
	NET_PAD_KEY_HORN = 32768, // Horn (Key 54)
	NET_PAD_KEY_DRIVE_BY = 65536, // Drive By (Key 38)
	NET_PAD_KEY_HELI_PRIMARY_FIRE = 131072, // Heli Primary Fire (Key 39)
};

struct NetPadState
{
	unsigned char byteLeftAnalogLR[2]; // Left Analog Left/Right (OnFoot: Key 12/13 InCar: Key 30/31)
	unsigned char byteLeftAnalogUD[2]; // Left Analog Up/Down (OnFoot: Key 14/15 InVehicle: Key 32/33)
	unsigned char byteTriggers[2];
	DWORD dwKeys; // All other keys

	NetPadState()
	{
		byteLeftAnalogLR[0] = 0;
		byteLeftAnalogLR[1] = 0;
		byteLeftAnalogUD[0] = 0;
		byteLeftAnalogUD[1] = 0;
		byteTriggers[0] = 0;
		byteTriggers[1] = 0;
		dwKeys = 0;
	}

	NetPadState(unsigned char _byteLeftAnalogLR[2], unsigned char _byteLeftAnalogUD[2], DWORD _dwKeys)
	{
		byteLeftAnalogLR[0] = _byteLeftAnalogLR[0];
		byteLeftAnalogLR[1] = _byteLeftAnalogLR[1];
		byteLeftAnalogUD[0] = _byteLeftAnalogUD[0];
		byteLeftAnalogUD[1] = _byteLeftAnalogUD[1];
		dwKeys = _dwKeys;
	}

	bool operator== (const NetPadState& o) const
	{
		return (byteLeftAnalogLR[0] == o.byteLeftAnalogLR[0] && 
				byteLeftAnalogLR[1] == o.byteLeftAnalogLR[1] && 
				byteLeftAnalogUD[0] == o.byteLeftAnalogUD[0] && 
				byteLeftAnalogUD[1] == o.byteLeftAnalogUD[1] && 
				dwKeys == o.dwKeys);
	}

	bool operator!= (const NetPadState& o) const
	{
		return (byteLeftAnalogLR[0] != o.byteLeftAnalogLR[0] || 
			byteLeftAnalogLR[1] != o.byteLeftAnalogLR[1] || 
			byteLeftAnalogUD[0] != o.byteLeftAnalogUD[0] || 
			byteLeftAnalogUD[1] != o.byteLeftAnalogUD[1] || 
			dwKeys != o.dwKeys);
	}

	bool IsDoingCombat()
	{
		return (dwKeys & NET_PAD_KEY_COMBAT_PUNCH_1 || dwKeys & NET_PAD_KEY_COMBAT_PUNCH_2 || 
			    dwKeys & NET_PAD_KEY_COMBAT_KICK);
	}

	bool IsFiring()
	{
		return ((dwKeys & NET_PAD_KEY_ATTACK) != 0);
	}

	bool IsAiming()
	{
		return (dwKeys & NET_PAD_KEY_FREE_AIM_1 || dwKeys & NET_PAD_KEY_FREE_AIM_2 || 
				dwKeys & NET_PAD_KEY_MOUSE_AIM);
	}

	bool IsDoingDriveBy()
	{
		return ((dwKeys & NET_PAD_KEY_DRIVE_BY) != 0);
	}

	bool IsFiringHelicoptor()
	{
		return ((dwKeys & NET_PAD_KEY_HELI_PRIMARY_FIRE) != 0);
	}

	bool IsSprinting()
	{
		return ((dwKeys & NET_PAD_KEY_SPRINT) != 0);
	}

	bool IsUsingHorn()
	{
		return ((dwKeys & NET_PAD_KEY_HORN) != 0);
	}

	bool IsJumping()
	{
		return ((dwKeys & NET_PAD_KEY_JUMP) != 0);
	}

	bool IsUsingHandbrake()
	{
		return (dwKeys & NET_PAD_KEY_HANDBRAKE_1 || dwKeys & NET_PAD_KEY_HANDBRAKE_2);
	}
};

struct OnFootSyncData
{
	NetPadState padState;            // pad state
	CVector3 vecPos;                 // player position
	float fHeading;                  // player heading
	CVector3 vecMoveSpeed;           // player move speed
	bool bDuckState : 1;             // ducking
	unsigned int uHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uWeaponInfo;        // player weapon and ammo
};

struct InVehicleSyncData
{
	NetPadState padState;                  // pad state
	CVector3 vecPos;                       // vehicle position
	CVector3 vecRotation;                  // vehicle rotation
	unsigned int uiHealth : 16;            // vehicle health
	unsigned char byteColors[4];           // vehicle colors
	CVector3 vecTurnSpeed;                 // vehicle turn speed
	CVector3 vecMoveSpeed;                 // vehicle move speed
	float fDirtLevel;                      // vehicle dirt level
	bool bSirenState : 1;                  // siren state
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo;        // player weapon and ammo
};

struct PassengerSyncData
{
	NetPadState padState; // pad state
	unsigned char byteSeatId; // vehicle seat id
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo; // player weapon and ammo
};

struct SmallSyncData
{
	NetPadState padState; // pad state
	bool bDuckState : 1; // ducking
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
	CVector3 vecFront;
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
	REFUSE_REASON_INVALIDNAME
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
