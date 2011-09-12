//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: KeySync.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include "IVCommon.h"

enum eGTAKey
{
	GTA_KEY_CAMERA = 0,
	GTA_KEY_ON_FOOT_SPRINT = 1,
	GTA_KEY_ON_FOOT_JUMP = 2,
	GTA_KEY_ENTEREXIT_VEHICLE = 3,
	GTA_KEY_ON_FOOT_ATTACK = 4,
	GTA_KEY_ON_FOOT_LOCK_ON_1 = 5,
	GTA_KEY_ON_FOOT_LOCK_ON_2 = 6,
	GTA_KEY_GENERAL_LOOK_BEHIND = 7,
	GTA_KEY_ON_FOOT_NEXT_WEAPON = 8,
	GTA_KEY_ON_FOOT_PREVIOUS_WEAPON = 9,
	GTA_KEY_ON_FOOT_MOVE_LEFT = 12,
	GTA_KEY_ON_FOOT_MOVE_RIGHT = 13,
	GTA_KEY_ON_FOOT_MOVE_FORWARD = 14,
	GTA_KEY_ON_FOOT_MOVE_BACKWARDS = 15,
	GTA_KEY_ON_FOOT_RIGHT_ANALOG_LEFT = 16,
	GTA_KEY_ON_FOOT_RIGHT_ANALOG_RIGHT = 17,
	GTA_KEY_ON_FOOT_RIGHT_ANALOG_UP = 18,
	GTA_KEY_ON_FOOT_RIGHT_ANALOG_DOWN = 19,
	GTA_KEY_ON_FOOT_CROUCH = 20,
	GTA_KEY_ON_FOOT_TAKE_PHONE_OUT = 21,
	GTA_KEY_ON_FOOT_ACTION = 23,
	GTA_KEY_ON_FOOT_SNIPER_ZOOM_IN = 24,
	GTA_KEY_ON_FOOT_SNIPER_ZOOM_OUT = 25,
	GTA_KEY_ON_FOOT_RELOAD = 29,
	GTA_KEY_IN_VEHICLE_STEER_LEFT = 30,
	GTA_KEY_IN_VEHICLE_STEER_RIGHT = 31,
	GTA_KEY_IN_VEHICLE_LEAN_FORWARD = 32,
	GTA_KEY_IN_VEHICLE_LEAN_BACK = 33,
	GTA_KEY_IN_VEHICLE_SHOOT = 38,
	GTA_KEY_IN_HELI_PRIMARY_FIRE = 39,
	GTA_KEY_IN_VEHICLE_ACCELERATE = 40,
	GTA_KEY_IN_VEHICLE_REVERSE = 41,
	GTA_KEY_IN_VEHICLE_HEADLIGHT = 42,
	GTA_KEY_ON_FOOT_ENTEREXIT_VEHICLE_ALT = 43,
	GTA_KEY_IN_VEHICLE_HANDBRAKE_1 = 44,
	GTA_KEY_IN_VEHICLE_HANDBRAKE_2 = 45,
	GTA_KEY_IN_VEHICLE_HOTWIRE_1 = 46,
	GTA_KEY_IN_VEHICLE_HOTWIRE_2 = 47,
	GTA_KEY_IN_VEHICLE_LOOK_BEHIND = 50,
	GTA_KEY_IN_VEHICLE_CINEMATIC_CAMERA = 51,
	GTA_KEY_IN_VEHICLE_HORN = 54,
	GTA_KEY_HELICOPTOR_THROTTLE_UP = 55,
	GTA_KEY_HELICOPTOR_THROTTLE_DOWN = 56,
	GTA_KEY_HELICOPTOR_ROTATE_LEFT = 57,
	GTA_KEY_HELICOPTOR_ROTATE_RIGHT = 58,
	GTA_KEY_ON_FOOT_COMBAT_PUNCH_1 = 59,
	GTA_KEY_ON_FOOT_COMBAT_PUNCH_2 = 60,
	GTA_KEY_ON_FOOT_COMBAT_KICK = 62,
	GTA_KEY_ON_FOOT_COMBAT_BLOCK = 63,
	GTA_KEYBOARD_PAGE_DOWN = 83,
	GTA_KEYBOARD_PAGE_UP = 84,
	GTA_KEY_GENERAL_RADAR_ZOOM = 86,
	GTA_KEY_ON_FOOT_LOCK_ON_3 = 87,
	// 93 + 94 = Mouse Wheel (Up/Down?)
	GTA_KEY_WEAPONS_UNARMED = 100,
	GTA_KEY_WEAPONS_MELEE = 101,
	GTA_KEY_WEAPONS_HANDGUN = 102,
	GTA_KEY_WEAPONS_SHOTGUN = 103,
	GTA_KEY_WEAPONS_SUBMACHINEGUN = 104,
	GTA_KEY_WEAPONS_AUTOMATICRIFLE = 105,
	GTA_KEY_WEAPONS_SNIPERRIFLE = 106,
	GTA_KEY_WEAPONS_HEAVYWEAPON = 107,
	GTA_KEY_WEAPONS_GRENADEMOLOTOV = 108,
	GTA_KEY_WEAPONS_SPECIAL = 109,
	GTA_KEY_IN_VEHICLE_NEXT_WEAPON = 112,
	GTA_KEY_IN_VEHICLE_PREVIOUS_WEAPON = 113,
	GTA_KEY_IN_VEHICLE_TURN_OFF_RADIO = 114,
	GTA_KEY_GENERAL_TEAM_CHAT_TEAM = 125,
	GTA_KEY_GENERAL_TEAM_CHAT_ALL = 126,
	GTA_KEY_GENERAL_PHONE_UP = 130,
	GTA_KEY_GENERAL_PHONE_DOWN = 131,
	GTA_KEY_GENERAL_PHONE_LEFT = 132,
	GTA_KEY_GENERAL_PHONE_RIGHT = 133,
	GTA_KEY_IN_VEHICLE_NEXT_SONG = 134,
	GTA_KEY_IN_VEHICLE_PREVIOUS_SONG = 135,
	GTA_KEY_IN_VEHICLE_DROP_WEAPON = 136,
	GTA_MOUSE_LMB = 137,
	GTA_MOUSE_RMB = 138,
	GTA_KEY_HELICOPTOR_PITCH_FORWARD = 141,
	GTA_KEY_HELICOPTOR_PITCH_BACK = 142,
	GTA_KEY_HELICOPTOR_BANK_LEFT = 143,
	GTA_KEY_HELICOPTOR_BANK_RIGHT = 144,
	GTA_KEYBOARD_F1 = 145,
	GTA_KEYBOARD_F2 = 146,
	GTA_KEYBOARD_F3 = 147,
	GTA_KEYBOARD_H = 156,
	GTA_KEYBOARD_OPEN_SQUARE_BRACE = 157,
	GTA_KEYBOARD_CLOSE_SQUARE_BRACE = 158,
	GTA_KEYBOARD_K = 161,
	GTA_KEYBOARD_COMMA = 164,
	GTA_KEYBOARD_PERIOD = 165,
	GTA_KEY_GENERAL_TOGGLE_DEFINITION = 170,
	GTA_KEY_ON_FOOT_SNIPER_ZOOM_IN_ALT = 179,
	GTA_KEY_ON_FOOT_SNIPER_ZOOM_OUT_ALT = 180,
	GTA_KEY_GENERAL_PHONE_ACCEPT = 181,
	GTA_KEY_GENERAL_PHONE_CANCEL = 182,
	GTA_KEY_HELICOPTOR_SHOOT_SECONDARY = 183,
	GTA_KEY_ON_FOOT_DETONATE = 184,
	GTA_KEY_MAX = 187
};

struct CPadData_m1_m1
{
	BYTE m_byteUnknown0;          // 0-1
	PAD(CPadData_m1_m1, pad0, 3); // 1-3 (Unknown)
	DWORD m_dwUnknown4;           // 4-8
};

struct CPadData_m1 // CPadData + 0xC = KeyBlock_m1 * pUnknown
{
	CPadData_m1_m1 m_unknown[64]; // 000-200
};

struct CPadData
{
	// Control State Value:
	// Min = 0
	// Default = 0/128
	// Max = 255
	// Get current key (m_byteUnknown4 ^ m_byteUnknown6)
	// Get previous key (m_byteUnknown4 ^ m_byteUnknown7)
	DWORD m_dwUnknown0;  // 00-04 (Function pointer (0xD55F2C))
	BYTE m_byteUnknown4; // 04-05 (Control state value? (xor with byte 6/7))
	BYTE m_byteUnknown5; // 05-06
	BYTE m_byteUnknown6; // 06-07 (Control state value (Current?))
	BYTE m_byteUnknown7; // 07-08 (Control state value (Previous?))
	BYTE m_byteUnknown8; // 08-09
	PAD(CPadData, pad0, 3);        // 09-0C
	CPadData_m1 * m_pUnknown; // 0C-10 (Pointer to a 512 size dynamically allocated block of memory (KeyBlock_m1) (See 0x8340F0))
};

struct CPad
{
	// 0000 (DWORD) = Some Counter
	PAD(CPad, pad0, 0x2698);               // 0000-2698
	CPadData m_padData[GTA_KEY_MAX]; // 2698-3238
	PAD(CPad, pad1, 0x54);                 // 3238-328C
	bool m_bIsUsingController;       // 328C-328D
	PAD(CPad, pad2, 0x7F7);                // 328D-3A84
};

struct PadState
{
	BYTE byteCurrentKeys[GTA_KEY_MAX];
	BYTE bytePreviousKeys[GTA_KEY_MAX];

	PadState()
	{
		Reset();
	}

	void Reset()
	{
		memset(this, 0, sizeof(PadState));

		byteCurrentKeys[GTA_KEY_ON_FOOT_MOVE_LEFT] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_MOVE_LEFT] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_MOVE_RIGHT] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_MOVE_RIGHT] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_MOVE_FORWARD] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_MOVE_FORWARD] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_MOVE_BACKWARDS] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_MOVE_BACKWARDS] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_LEFT] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_LEFT] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_RIGHT] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_RIGHT] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_UP] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_UP] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_DOWN] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_RIGHT_ANALOG_DOWN] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_SNIPER_ZOOM_IN] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_SNIPER_ZOOM_IN] = 128;
		byteCurrentKeys[GTA_KEY_ON_FOOT_SNIPER_ZOOM_OUT] = 128;
		bytePreviousKeys[GTA_KEY_ON_FOOT_SNIPER_ZOOM_OUT] = 128;
		byteCurrentKeys[26] = 128;
		bytePreviousKeys[26] = 128;
		byteCurrentKeys[27] = 128;
		bytePreviousKeys[27] = 128;
		byteCurrentKeys[GTA_KEY_IN_VEHICLE_STEER_LEFT] = 128;
		bytePreviousKeys[GTA_KEY_IN_VEHICLE_STEER_LEFT] = 128;
		byteCurrentKeys[GTA_KEY_IN_VEHICLE_STEER_RIGHT] = 128;
		bytePreviousKeys[GTA_KEY_IN_VEHICLE_STEER_RIGHT] = 128;
		byteCurrentKeys[GTA_KEY_IN_VEHICLE_LEAN_FORWARD] = 128;
		bytePreviousKeys[GTA_KEY_IN_VEHICLE_LEAN_FORWARD] = 128;
		byteCurrentKeys[GTA_KEY_IN_VEHICLE_LEAN_BACK] = 128;
		bytePreviousKeys[GTA_KEY_IN_VEHICLE_LEAN_BACK] = 128;
		byteCurrentKeys[88] = 128;
		bytePreviousKeys[88] = 128;
		byteCurrentKeys[89] = 128;
		bytePreviousKeys[89] = 128;
		byteCurrentKeys[92] = 128;
		bytePreviousKeys[92] = 128;
		byteCurrentKeys[95] = 128;
		bytePreviousKeys[95] = 128;
		byteCurrentKeys[96] = 128;
		bytePreviousKeys[96] = 128;
		byteCurrentKeys[139] = 128;
		bytePreviousKeys[139] = 128;
		byteCurrentKeys[140] = 128;
		bytePreviousKeys[140] = 128;
		byteCurrentKeys[185] = 128;
		bytePreviousKeys[185] = 128;
		byteCurrentKeys[186] = 128;
		bytePreviousKeys[186] = 128;
	}

	void Invalidate()
	{
		for(int i = 0; i < GTA_KEY_MAX; i++)
			bytePreviousKeys[i] = byteCurrentKeys[i];
	}
};

struct CCam
{
	PAD(CCam, pad0, 0x10);    // 000-010
	Matrix34 m_matMatrix; // 010-050
	PAD(CCam, pad1, 0xF0);    // 050-140
};

void         SetLocalPlayerId(unsigned int uiPlayerId);
unsigned int GetLocalPlayerId();
void         ResetPlayerPadState(unsigned int uiPlayerId);
void         ResetPlayerPadStates();
void         SetPlayerPadState(unsigned int uiPlayerId, PadState * padState);
void         GetPlayerPadState(unsigned int uiPlayerId, PadState * padState);
CPad *       GetGamePad();
void         ResetGamePadState();
void         SetGamePadState(PadState * padState);
void         GetGamePadState(PadState * padState);
void         ResetPlayerCameraMatrix(unsigned int uPlayerId);
void         ResetPlayerCameraMatrixs();
void         SetPlayerCameraMatrix(unsigned int uiPlayerId, Matrix34 * matMatrix);
void         GetPlayerCameraMatrix(unsigned int uiPlayerId, Matrix34 * matMatrix);
CCam *       GetGameCam();
void         SetGameCameraMatrix(Matrix34 * matMatrix);
void         GetGameCameraMatrix(Matrix34 * matMatrix);
void         InstallKeySyncHooks();
