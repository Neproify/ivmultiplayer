//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPad.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../IVCommon.h"
#include "CIVPadData.h"
#include <Game/CControlState.h>

// Input values
#define MIN_INPUT_VALUE 0
#define DEFAULT_BINARY_INPUT_VALUE 0
#define DEFAULT_ANALOG_INPUT_VALUE 128
#define MAX_INPUT_VALUE 255

// Enum from 0xF132F8 (char szControlNames[187][44])
enum eInput
{
	INPUT_NEXT_CAMERA,                      // 
	INPUT_SPRINT,                           // On Foot - Sprint
	INPUT_JUMP,                             // On Foot - Jump
	INPUT_ENTER,                            // 
	INPUT_ATTACK,                           // On Foot - Attack
	INPUT_ATTACK2,                          // On Foot - Lock On 1
	INPUT_AIM,                              // On Foot - Lock On 2
	INPUT_LOOK_BEHIND,                      // General - Look Behind
	INPUT_NEXT_WEAPON,                      // On Foot - Next Weapon
	INPUT_PREV_WEAPON,                      // On Foot - Previous Weapon
	INPUT_NEXT_TARGET_LEFT,                 // 
	INPUT_NEXT_TARGET_RIGHT,                // 
	INPUT_MOVE_LEFT,                        // On Foot - Move Left
	INPUT_MOVE_RIGHT,                       // On Foot - Move Right
	INPUT_MOVE_UP,                          // On Foot - Move Forward
	INPUT_MOVE_DOWN,                        // On Foot - Move Backwards
	INPUT_LOOK_LEFT,                        // 
	INPUT_LOOK_RIGHT,                       // 
	INPUT_LOOK_UP,                          // 
	INPUT_LOOK_DOWN,                        // 
	INPUT_DUCK,                             // On Foot - Crouch
	INPUT_PHONE_TAKE_OUT,                   // On Foot - Take Phone Out
	INPUT_PHONE_PUT_AWAY,                   // 
	INPUT_PICKUP,                           // On Foot - Action
	INPUT_SNIPER_ZOOM_IN,                   // On Foot - Sniper Zoom In
	INPUT_SNIPER_ZOOM_OUT,                  // On Foot - Sniper Zoom Out
	INPUT_SNIPER_ZOOM_IN_ALTERNATE,         // 
	INPUT_SNIPER_ZOOM_OUT_ALTERNATE,        // 
	INPUT_COVER,                            // 
	INPUT_RELOAD,                           // On Foot - Reload
	INPUT_VEH_MOVE_LEFT,                    // In Vehicle - Steer Left
	INPUT_VEH_MOVE_RIGHT,                   // In Vehicle - Steer Right
	INPUT_VEH_MOVE_UP,                      // In Vehicle - Lean Forward
	INPUT_VEH_MOVE_DOWN,                    // In Vehicle - Lean Back
	INPUT_VEH_GUN_LEFT,                     // 
	INPUT_VEH_GUN_RIGHT,                    // 
	INPUT_VEH_GUN_UP,                       // 
	INPUT_VEH_GUN_DOWN,                     // 
	INPUT_VEH_ATTACK,                       // In Vehicle - Drive By
	INPUT_VEH_ATTACK2,                      // In Vehicle - Helicoptor Primary Fire
	INPUT_VEH_ACCELERATE,                   // In Vehicle - Accelerate
	INPUT_VEH_BRAKE,                        // In Vehicle - Reverse
	INPUT_VEH_HEADLIGHT,                    // In Vehicle - Headlight
	INPUT_VEH_EXIT,                         // 
	INPUT_VEH_HANDBRAKE,                    // In Vehicle - Handbrake 1
	INPUT_VEH_HANDBRAKE_ALT,                // In Vehicle - Handbrake 2
	INPUT_VEH_HOTWIRE_LEFT,                 // In Vehicle - Hotwire 1
	INPUT_VEH_HOTWIRE_RIGHT,                // In Vehicle - Hotwire 2
	INPUT_VEH_LOOK_LEFT,                    // 
	INPUT_VEH_LOOK_RIGHT,                   // 
	INPUT_VEH_LOOK_BEHIND,                  // In Vehicle - Look Behind
	INPUT_VEH_CIN_CAM,                      // In Vehicle - Cinematic Camera
	INPUT_VEH_NEXT_RADIO,                   // 
	INPUT_VEH_PREV_RADIO,                   // 
	INPUT_VEH_HORN,                         // In Vehicle - Horn
	INPUT_VEH_FLY_THROTTLE_UP,              // Helicoptor - Throttle Up
	INPUT_VEH_FLY_THROTTLE_DOWN,            // Helicoptor - Throttle Down
	INPUT_VEH_FLY_YAW_LEFT,                 // Helicoptor - Rotate Left
	INPUT_VEH_FLY_YAW_RIGHT,                // Helicoptor - Rotate Right
	INPUT_MELEE_ATTACK1,                    // On Foot - Combat Punch 1
	INPUT_MELEE_ATTACK2,                    // On Foot - Combat Punch 2
	INPUT_MELEE_ATTACK3,                    // 
	INPUT_MELEE_KICK,                       // On Foot - Combat Kick
	INPUT_MELEE_BLOCK,                      // On Foot - Combat Block
	INPUT_FRONTEND_DOWN,                    // 
	INPUT_FRONTEND_UP,                      // 
	INPUT_FRONTEND_LEFT,                    // 
	INPUT_FRONTEND_RIGHT,                   // 
	INPUT_FRONTEND_RDOWN,                   // 
	INPUT_FRONTEND_RUP,                     // 
	INPUT_FRONTEND_RLEFT,                   // 
	INPUT_FRONTEND_RRIGHT,                  // 
	INPUT_FRONTEND_AXIS_X,                  // 
	INPUT_FRONTEND_AXIS_Y,                  // 
	INPUT_FRONTEND_RIGHT_AXIS_X,            // 
	INPUT_FRONTEND_RIGHT_AXIS_Y,            // 
	INPUT_FRONTEND_PAUSE,                   // 
	INPUT_FRONTEND_ACCEPT,                  // 
	INPUT_FRONTEND_CANCEL,                  // 
	INPUT_FRONTEND_X,                       // 
	INPUT_FRONTEND_Y,                       // 
	INPUT_FRONTEND_LB,                      // 
	INPUT_FRONTEND_RB,                      // 
	INPUT_FRONTEND_LT,                      // Page Down
	INPUT_FRONTEND_RT,                      // Page Up
	INPUT_MELEE_ATTACK4,                    // 
	INPUT_ZOOM_RADAR,                       // General - Radar Zoom
	INPUT_FREE_AIM,                         // On Foot - Lock On 3
	INPUT_MOUSE_UD,                         // 
	INPUT_MOUSE_LR,                         // 
	INPUT_MOVE_KEY_FB,                      // 
	INPUT_MOVE_KEY_LR,                      // 
	INPUT_MOVE_KEY_STUNTJUMP,               // 
	INPUT_MOUSE_WHEEL_UP,                   // 
	INPUT_MOUSE_WHEEL_DOWN,                 // 
	INPUT_FRONTEND_AXIS_UD,                 // 
	INPUT_FRONTEND_AXIS_LR,                 // 
	INPUT_FRONTEND_LEGEND,                  // 
	INPUT_FRONTEND_APPLY,                   // 
	INPUT_FRONTEND_SHOWSCORE,               // 
	INPUT_WEAPON_UNARMED,                   // Weapons - Unarmed
	INPUT_WEAPON_MELEE,                     // Weapons - Melee
	INPUT_WEAPON_HANDGUN,                   // Weapons - Handgun
	INPUT_WEAPON_SHOTGUN,                   // Weapon - Shotgun
	INPUT_WEAPON_SMG,                       // Weapons - Sub Machine Gun
	INPUT_WEAPON_RIFLE,                     // Weapons - Automatic Rifle
	INPUT_WEAPON_SNIPER,                    // Weapons - Sniper Rifle
	INPUT_WEAPON_HEAVY,                     // Weapons - Heavy Weapon
	INPUT_WEAPON_THROWN,                    // Weapons - Grenade/Molotov
	INPUT_WEAPON_SPECIAL,                   // Weapons - Special
	INPUT_VEH_KEY_UD,                       // 
	INPUT_VEH_KEY_LR,                       // 
	INPUT_VEH_NEXT_WEAPON,                  // In Vehicle - Next Weapon
	INPUT_VEH_PREV_WEAPON,                  // In Vehicle - Previous Weapon
	INPUT_TURN_OFF_RADIO,                   // In Vehicle - Turn Off Radio
	INPUT_FRONTEND_MINIGAME_1,              // 
	INPUT_FRONTEND_MINIGAME_2,              // 
	INPUT_FRONTEND_MINIGAME_3,              // 
	INPUT_FRONTEND_MINIGAME_4,              // 
	INPUT_FRONTEND_MODEL_SCREEN,            // 
	INPUT_FRONTEND_VEHICLE_TOGGLE,          // 
	INPUT_FRONTEND_DELETE_FILTER,           // 
	INPUT_FRONTEND_REFRESH,                 // 
	INPUT_FRONTEND_PLAYER_LIST,             // 
	INPUT_TEXT_CHAT_TOGGLE,                 // 
	INPUT_TEXT_CHAT_TEAM,                   // General - Team Chat Team
	INPUT_TEXT_CHAT_UNIVERSAL,              // General - Team Chat All
	INPUT_TEXT_CHAT_RESULTS_TOGGLE,         // 
	INPUT_TEXT_CHAT_Y,                      // 
	INPUT_FRONTEND_LOCK_LOBBY,              // 
	INPUT_KB_UP,                            // General - Phone Up
	INPUT_KB_DOWN,                          // General - Phone Down
	INPUT_KB_LEFT,                          // General - Phone Left
	INPUT_KB_RIGHT,                         // General - Phone Right
	INPUT_NEXT_TRACK,                       // In Vehicle - Next Song
	INPUT_PREV_TRACK,                       // In Vehicle - Previous Song
	INPUT_DROP_WEAPON,                      // In Vehicle - Drop Weapon
	INPUT_MOUSE_LMB,                        // 
	INPUT_MOUSE_RMB,                        // 
	INPUT_FE_MOUSE_UD,                      // 
	INPUT_FE_MOUSE_LR,                      // 
	INPUT_VEH_FLY_PITCH_UP,                 // Helicopter - Pitch Forward
	INPUT_VEH_FLY_PITCH_DOWN,               // Helicopter - Pitch Back
	INPUT_VEH_FLY_ROLL_LEFT,                // Helicopter - Bank Left
	INPUT_VEH_FLY_ROLL_RIGHT,               // Helicopter - Bank Right
	INPUT_REPLAY_PLAY,                      // F1
	INPUT_REPLAY_SAVE_TO_HDD,               // F2
	INPUT_REPLAY_LOAD_FROM_HDD,             // F3
	INPUT_REPLAY_EXIT,                      // 
	INPUT_FRONTEND_MARKER_DELETE,           // 
	INPUT_FRONTEND_REPLAY_PAUSE,            // 
	INPUT_FRONTEND_REPLAY_REWIND,           // 
	INPUT_FRONTEND_REPLAY_FFWD,             // 
	INPUT_FRONTEND_REPLAY_NEWMARKER,        // 
	INPUT_FRONTEND_REPLAY_RECORD,           // 
	INPUT_FRONTEND_REPLAY_SCREENSHOT,       // 
	INPUT_FRONTEND_REPLAY_HIDEHUD,          // H
	INPUT_FRONTEND_REPLAY_STARTPOINT,       // [
	INPUT_FRONTEND_REPLAY_ENDPOINT,         // ]
	INPUT_FRONTEND_REPLAY_ADVANCE,          // 
	INPUT_FRONTEND_REPLAY_BACK,             // 
	INPUT_FRONTEND_REPLAY_SHOWHOTKEY,       // 
	INPUT_FRONTEND_REPLAY_TOOLS,            // 
	INPUT_FRONTEND_REPLAY_RESTART,          // K
	INPUT_FRONTEND_REPLAY_CYCLEMARKERLEFT,  // ,
	INPUT_FRONTEND_REPLAY_CYCLEMARKERRIGHT, // .
	INPUT_FRONTEND_AXIS_FWD,                // 
	INPUT_FRONTEND_AXIS_BACK,               // 
	INPUT_FRONTEND_AXIS_LEFT,               // 
	INPUT_FRONTEND_AXIS_RIGHT,              // 
	INPUT_FRONTEND_DEFINITION,              // General - Toggle Definition
	INPUT_FRONTEND_F6,                      // 
	INPUT_FE_REPLAY_TOGGLETIME,             // 
	INPUT_FE_REPLAY_TOGGLETIPS,             // 
	INPUT_FE_REPLAY_PREVIEW,                // 
	INPUT_FRONTEND_PASTE,                   // 
	INPUT_FRONTEND_CUT,                     // 
	INPUT_FRONTEND_COPY,                    // 
	INPUT_VOICE_CHAT,                       // 
	INPUT_SNIPER_ZOOM_IN_KB,                // On Foot - Sniper Zoom In Alt
	INPUT_SNIPER_ZOOM_OUT_KB,               // On Foot - Sniper Zoom Out Alt
	INPUT_KB_PHONE_ACCEPT,                  // General - Phone Accept
	INPUT_KB_PHONE_CANCEL,                  // General - Phone Cancel
	INPUT_VEH_ATTACK3_KB,                   // Helicopter - Shoot Secondary
	INPUT_DETONATE,                         // On Foot - Detonate
	INPUT_VEH_MOVE_LEFT_2,                  // 
	INPUT_VEH_MOVE_RIGHT_2,                 // 
	INPUT_COUNT
};

// TODO: Move these functions to somewhere more appropriate
static bool IsAnalogInput(eInput input)
{
	switch(input)
	{
	case INPUT_MOVE_LEFT:
	case INPUT_MOVE_RIGHT:
	case INPUT_MOVE_UP:
	case INPUT_MOVE_DOWN:
	case INPUT_LOOK_LEFT:
	case INPUT_LOOK_RIGHT:
	case INPUT_LOOK_UP:
	case INPUT_LOOK_DOWN:
	case INPUT_SNIPER_ZOOM_IN:
	case INPUT_SNIPER_ZOOM_OUT:
	case INPUT_SNIPER_ZOOM_IN_ALTERNATE:
	case INPUT_SNIPER_ZOOM_OUT_ALTERNATE:
	case INPUT_VEH_MOVE_LEFT:
	case INPUT_VEH_MOVE_RIGHT:
	case INPUT_VEH_MOVE_UP:
	case INPUT_VEH_MOVE_DOWN:
	case INPUT_MOUSE_UD:
	case INPUT_MOUSE_LR:
	case INPUT_MOVE_KEY_STUNTJUMP:
	case INPUT_FRONTEND_AXIS_UD:
	case INPUT_FRONTEND_AXIS_LR:
	case INPUT_FE_MOUSE_UD:
	case INPUT_FE_MOUSE_LR:
	case INPUT_VEH_MOVE_LEFT_2:
	case INPUT_VEH_MOVE_RIGHT_2:
		return true;
	}

	return false;
}

static bool IsBinaryInput(eInput input)
{
	return (!IsAnalogInput(input));
}

static bool InputHasHistoryData(eInput input)
{
	switch(input)
	{
	case INPUT_AIM:
	case INPUT_MOVE_LEFT:
	case INPUT_MOVE_RIGHT:
	case INPUT_MOVE_UP:
	case INPUT_MOVE_DOWN:
	case INPUT_LOOK_LEFT:
	case INPUT_LOOK_RIGHT:
	case INPUT_PHONE_PUT_AWAY:
	case INPUT_VEH_HEADLIGHT:
	case INPUT_VEH_EXIT:
	case INPUT_VEH_NEXT_RADIO:
	case INPUT_VEH_PREV_RADIO:
	case INPUT_VEH_HORN:
	case INPUT_MELEE_ATTACK1:
	case INPUT_MELEE_ATTACK2:
	case INPUT_MELEE_ATTACK3:
	case INPUT_MELEE_KICK:
	case INPUT_MELEE_BLOCK:
	case INPUT_MELEE_ATTACK4:
	case INPUT_FREE_AIM:
	case INPUT_MOUSE_LR:
	case INPUT_TURN_OFF_RADIO:
	case INPUT_NEXT_TRACK:
	case INPUT_PREV_TRACK:
	case INPUT_REPLAY_SAVE_TO_HDD:
	case INPUT_FRONTEND_REPLAY_ADVANCE:
	case INPUT_FRONTEND_REPLAY_BACK:
		return true;
	}

	return false;
}

class IVPadConfig
{
public:
	DWORD dwCount;                // 000-004
	//DWORD dwUnknown;              // 004-008
	//CPadMap * m_pPadMaps[492]; // ??
	PAD(IVPadConfig, pad0, 0x7B4); // ?
	// +0x4 - (DWORD) Unknown (Initializes at 0)
	// +0x8 - (DWORD) First Control Type (9 - analog)
	// +0x4 - (DWORD) Next Control Type (Continue for dwCount)
	// +0x528 - (CPadData) Pad Data
	// +0x4 - (CPadData) Next Pad Data (Continue for dwCount)
};

class IVPad
{
public:
	IVPadConfig m_padConfig[5];       // 0000-2698
	IVPadData m_padData[INPUT_COUNT]; // 2698-3248
	IVPadData m_otherPadData0;        // 3248-3258
	IVPadData m_otherPadData1;        // 3258-3268
	IVPadData m_otherPadData2;        // 3268-3278
	IVPadData m_otherPadData3;        // 3278-3288
	PAD(IVPad, pad0, 0x4);            // 3288-328C
	bool m_bIsUsingController;        // 328C-328D
	PAD(IVPad, pad1, 0x3);            // 328D-3290
	IVPadData m_otherPadData4;        // 3290-32A0
	PAD(IVPad, pad2, 0xC);            // 32A0-32AC
	IVPadConfig m_textPadConfig;      // 32AC-3A64
	PAD(IVPad, pad3, 0x8);            // 3A64-3A6C
	DWORD m_dwLastUpdateTime;         // 3A6C-3A70
	PAD(IVPad, pad4, 0x14);           // 3A70-3A84
};

class CIVPad
{
private:
	bool    m_bCreatedByUs;
	IVPad * m_pPad;

public:
	CIVPad();
	CIVPad(IVPad * pPad);
	~CIVPad();

	void    SetPad(IVPad * pPad);
	IVPad * GetPad();

	void    ToControlState(CControlState& controlState, bool bCurrent);
	void    FromControlState(CControlState controlState, bool bCurrent);
	void    SetCurrentClientControlState(CControlState controlState);
	void    GetCurrentClientControlState(CControlState& controlState);
	void    SetLastClientControlState(CControlState controlState);
	void    GetLastClientControlState(CControlState& controlState);
	void    SetIsUsingController(bool bIsUsingController);
	bool    GetIsUsingController();
	void    SetLastUpdateTime(DWORD dwLastUpdateTime);
	DWORD   GetLastUpdateTime();
};