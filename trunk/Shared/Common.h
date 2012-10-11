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
#include "Game/CControlState.h"
#include "CString.h"

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
#define NETWORK_MODULE_VERSION 0x06

// Network version - increment this when packet layouts change!
#define NETWORK_VERSION 0x7A

// Tick Rate
#define TICK_RATE 50

// Defines used for the max amount of entities we (IV:MP, not GTA) can handle
// jenksta: although they may be streamed, shouldn't they at least have some sensible limit?
// NOTE: (if client-side entitys are introduced, those should not use ids from the same range as server ids)
#define MAX_PLAYERS 64/*128*/ // Player Info Array Size: 64 // Ped Pool Size: 64
#define MAX_VEHICLES 0xFFFE // Streamed. See note on Pickups. Vehicle Pool Size: 140
#define MAX_OBJECTS 0xFFFE // Streamed. See note on Pickups. Object Pool Size: 1300
#define MAX_CHECKPOINTS 0xFFFE // Streamed. Checkpoint Pool Size: See CStreamer.h
#define MAX_BLIPS 1300 // Blip Pool Size: 1500
#define MAX_PICKUPS 0xFFFE // Streamed. Pickup Pool Size: TODO: 1500?
#define MAX_FIRE 32
#define MAX_ACTORS 72/*252*/ 

// TODO: RC2: Players: 128, Actors, 252

// Max amount of characters a player can use in their name
#define MAX_NAME_LENGTH 18

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
#define HAPPINESS_CAMERA_POS 488.702759f, 540.719421f, 161.584091f
#define HAPPINESS_CAMERA_LOOK_AT -16.441833f, 69.520508f, 140.250290f

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
#define MOD_VERSION_STRING "0.1 RC2 DEV"

// Query port offset
#define QUERY_PORT_OFFSET 137

// Masterlist information
#if COMPILE_MODE == COMPILE_MODE_CUSTOM
#define MASTERLIST_ADDRESS ""
#define MASTERLIST_VERSION ""
#define MASTERLIST_TIMEOUT 0
#else if COMPILE_MODE == COMPILE_MODE_RELEASE
#define MASTERLIST_ADDRESS "www.master.iv-multiplayer.com"
#define MASTERLIST_VERSION "01RC2DEV"
#define MASTERLIST_TIMEOUT 50000
#endif

// Version identifiers
#define VERSION_IDENTIFIER MOD_NAME " " MOD_VERSION_STRING /*" (" __DATE__ ", " __TIME__ ")"*/ DEBUG_IDENTIFIER
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
#define SAFE_DELETE_ARRAY(memory) if(memory) { delete [] memory; memory = NULL; }
#define SAFE_FREE(memory) if(memory) { free(memory); memory = NULL; }

// Bit manipulation macros
#define SET_BIT(a, b) a |= b
#define IS_BIT_SET(a, b) ((a & b) != 0)
#define UNSET_BIT(a, b) a &= ~(b)

// Library debug suffix
//#ifdef IVMP_DEBUG
#ifdef _DEBUG
#define DEBUG_SUFFIX ".Debug"
#else
#define DEBUG_SUFFIX
#endif

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

// Nametags definitions etc..
#define b_w					80
#define	b_h					11
#define b_i_p				2
#define h_b_w				(b_w - b_i_p)
#define	nt_a				18
#define nt_a_a				30

// Debug Commands Enabled
//#define DEBUG_COMMANDS_ENABLED

struct OnFootSyncData
{
	CControlState controlState;      // control state
	CVector3 vecPos;                 // player position
	float fHeading;                  // player heading
	CVector3 vecMoveSpeed;           // player move speed
	CVector3 vecTurnSpeed;			 // player turn speed
	bool bDuckState : 1;             // ducking
	unsigned int uHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uWeaponInfo;        // player weapon and ammo
	bool bAnim;						 // player anim
	char szAnimGroup[256];			 // anim group
	char szAnimSpecific[256];	     // anim category from group
	float fAnimTime;				 // set anim time
};

struct InVehicleSyncData
{
	CControlState controlState;            // control state
	CVector3 vecPos;                       // vehicle position
	CVector3 vecRotation;                  // vehicle rotation
	unsigned int uiHealth	  ;            // vehicle health
	unsigned char byteColors[4];           // vehicle colors
	CVector3 vecTurnSpeed;                 // vehicle turn speed
	CVector3 vecMoveSpeed;                 // vehicle move speed
	bool bEngineStatus : 1;				   // vehicle engine status
	bool hHazardLights : 1;				   // hazardlights status
	float fPetrolHealth;				   // vehicle petrol tank health
	float fDirtLevel;					   // vehicle dirt
	float fDoor[6];						   // vehicle doors(open angle)
	bool bLights;						   // vehicle lights
	bool bTaxiLights : 1;				   // vehicle taxilight
	bool bSirenState : 1;				   // vehicle siren state
	bool bWindow[4];					   // vehicle window
	bool bTyre[6];						   // vehicle tyres
	bool bGpsState : 1;				       // gps state
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo;        // player weapon and ammo
};

struct PassengerSyncData
{
	CControlState controlState;            // control state
	unsigned char byteSeatId;              // vehicle seat id
	unsigned int uPlayerHealthArmour : 32; // player health and armour (first 16bit Health last 16bit Armour)
	unsigned int uPlayerWeaponInfo;        // player weapon and ammo
};

struct SmallSyncData
{
	CControlState controlState; // control state
	bool bDuckState : 1;        // ducking
	unsigned int uWeaponInfo;   // weapon and ammo
};

struct ActorSyncData
{
	EntityId	actorId;	// actorid
	EntityId	vehicleId;	// vehicleid
	CVector3	vecPos;		// vehicle position
	CVector3	vecRot;		// vehicle rotation
	bool		bDriving;	// driving(yes/no)
};

struct EMPTYVEHICLESYNCPACKET
{
	EntityId vehicleId;			// vehicleId
	CVector3 vecPosition;		// vehicle position
	CVector3 vecRotation;		// vehicle rotation
	CVector3 vecTurnSpeed;      // vehicle turn speed
	CVector3 vecMoveSpeed;      // vehicle move speed
	unsigned int uiHealth;		// vehicle health
	float fPetrolHealth;		// vehicle petrol health
	float fDirtLevel;			// vehicle dirt
	float fDoor[6];				// vehicle door(open angle)
	bool bLights;				// vehicle lights
	bool bTaxiLights : 1;		// vehicle taxilight
	bool bSirenState : 1;		// vehicle siren state
	bool bWindow[4];			// vehicle window
	bool bEngineStatus : 1;		// vehicle engine
	bool bTyre[6];				// vehicle tyre
};

struct AimSyncData
{
	CVector3 vecAimTarget;		// aim target
	CVector3 vecShotSource;		// shot source
	CVector3 vecShotTarget;		// shot target
	CVector3 vecLookAt;			// look at position
};

// Refuse Reasons
enum eRefuseReason
{
	REFUSE_REASON_INVALID_VERSION,
	REFUSE_REASON_NAME_IN_USE,
	REFUSE_REASON_NAME_INVALID,
	REFUSE_REASON_ABORTED_BY_SCRIPT,
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
