//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPad.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPad.h"
#include "COffsets.h"

// Helper macros for the ToPadState function
#define SET_ANALOG_KEY(key, analog) \
	analog = (bCurrent ? m_pPad->m_padData[key].m_byteCurrentValue : m_pPad->m_padData[key].m_byteLastValue)

#define SET_BINARY_KEY(key, binary) \
	binary = (bCurrent ? ((m_pPad->m_padData[key].m_byteCurrentValue == MAX_INPUT_VALUE) ? true : false) : ((m_pPad->m_padData[key].m_byteLastValue == MAX_INPUT_VALUE) ? true : false))

// Helper macros for the FromPadState function
#define GET_ANALOG_KEY(key, analog) \
	if(bCurrent) { m_pPad->m_padData[key].m_byteCurrentValue = analog; } else { m_pPad->m_padData[key].m_byteLastValue = analog; }

#define GET_BINARY_KEY(key, binary) \
	if(bCurrent) { m_pPad->m_padData[key].m_byteCurrentValue = (binary ? MAX_INPUT_VALUE : DEFAULT_BINARY_INPUT_VALUE); } else { m_pPad->m_padData[key].m_byteLastValue = (binary ? MAX_INPUT_VALUE : DEFAULT_BINARY_INPUT_VALUE); }

CIVPad::CIVPad()
{
	// Flag ourself as created by ourself
	m_bCreatedByUs = true;

	// Allocate the new pad
	m_pPad = new IVPad;

	// Call the CPad constructor
	IVPad * pPad = m_pPad;
	_asm
	{
		mov ecx, pPad
		call COffsets::FUNC_CPad__Constructor
	}

	// Call CPad::Initialize
	_asm
	{
		push 0
		mov ecx, pPad
		call COffsets::FUNC_CPad__Initialize
	}

	// HACK: To fix analog values not being initialized properly
	for(int i = 0; i < INPUT_COUNT; i++)
	{
		if(IsAnalogInput((eInput)i))
		{
			m_pPad->m_padData[i].m_byteCurrentValue = DEFAULT_ANALOG_INPUT_VALUE;
			m_pPad->m_padData[i].m_byteLastValue = DEFAULT_ANALOG_INPUT_VALUE;
			//CLogFile::Printf("Analog input %d values are %d and %d", i, m_pPad->m_padData[i].m_byteCurrentValue, m_pPad->m_padData[i].m_byteLastValue);
		}
	}
}

CIVPad::CIVPad(IVPad * pPad)
{
	m_bCreatedByUs = false;
	m_pPad = pPad;
}

CIVPad::~CIVPad()
{
	// Has the pad been created by us?
	if(m_bCreatedByUs)
	{
		// Call the CPad destructor
		IVPad * pPad = m_pPad;
		_asm
		{
			mov ecx, pPad
			call COffsets::FUNC_CPad__Destructor
		}

		// Delete the pad
		SAFE_DELETE(m_pPad);
	}
}

void CIVPad::SetPad(IVPad * pPad)
{
	m_pPad = pPad;
}

IVPad * CIVPad::GetPad()
{
	return m_pPad;
}

void CIVPad::ToPadState(CPadState& padState, bool bCurrent)
{
	// Do we not have a valid pad?
	if(!m_pPad)
		return;

	// Analog keys
	SET_ANALOG_KEY(INPUT_MOVE_LEFT,         padState.ucOnFootMove[0]);
	SET_ANALOG_KEY(INPUT_MOVE_RIGHT,        padState.ucOnFootMove[1]);
	SET_ANALOG_KEY(INPUT_MOVE_UP,           padState.ucOnFootMove[2]);
	SET_ANALOG_KEY(INPUT_MOVE_DOWN,         padState.ucOnFootMove[3]);
	SET_ANALOG_KEY(INPUT_VEH_MOVE_LEFT,     padState.ucInVehicleMove[0]);
	SET_ANALOG_KEY(INPUT_VEH_MOVE_RIGHT,    padState.ucInVehicleMove[1]);
	SET_ANALOG_KEY(INPUT_VEH_MOVE_UP,       padState.ucInVehicleMove[2]);
	SET_ANALOG_KEY(INPUT_VEH_MOVE_DOWN,     padState.ucInVehicleMove[3]);
	SET_ANALOG_KEY(INPUT_VEH_BRAKE,         padState.ucInVehicleTriggers[0]);
	SET_ANALOG_KEY(INPUT_VEH_ACCELERATE,    padState.ucInVehicleTriggers[1]);

	// Binary keys
	SET_BINARY_KEY(INPUT_ENTER,             padState.keys.bEnterExitVehicle);
	SET_BINARY_KEY(INPUT_SPRINT,            padState.keys.bSprint);
	SET_BINARY_KEY(INPUT_JUMP,              padState.keys.bJump);
	SET_BINARY_KEY(INPUT_ATTACK,            padState.keys.bAttack);
	SET_BINARY_KEY(INPUT_ATTACK2,           padState.keys.bAttack2);
	SET_BINARY_KEY(INPUT_AIM,               padState.keys.bAim);
	SET_BINARY_KEY(INPUT_FREE_AIM,          padState.keys.bFreeAim);
	SET_BINARY_KEY(INPUT_MELEE_ATTACK1,     padState.keys.bMeleeAttack1);
	SET_BINARY_KEY(INPUT_MELEE_ATTACK2,     padState.keys.bMeleeAttack2);
	SET_BINARY_KEY(INPUT_MELEE_KICK,        padState.keys.bMeleeKick);
	SET_BINARY_KEY(INPUT_MELEE_BLOCK,       padState.keys.bMeleeBlock);
	SET_BINARY_KEY(INPUT_VEH_HANDBRAKE,     padState.keys.bHandbrake);
	SET_BINARY_KEY(INPUT_VEH_HANDBRAKE_ALT, padState.keys.bHandbrake2);
	SET_BINARY_KEY(INPUT_VEH_HORN,          padState.keys.bHorn);
	SET_BINARY_KEY(INPUT_VEH_ATTACK,        padState.keys.bDriveBy);
	SET_BINARY_KEY(INPUT_VEH_ATTACK2,       padState.keys.bHeliPrimaryFire);
}

void CIVPad::FromPadState(CPadState padState, bool bCurrent)
{
	// Do we not have a valid pad?
	if(!m_pPad)
		return;

	// Analog keys
	GET_ANALOG_KEY(INPUT_MOVE_LEFT,         padState.ucOnFootMove[0]);
	GET_ANALOG_KEY(INPUT_MOVE_RIGHT,        padState.ucOnFootMove[1]);
	GET_ANALOG_KEY(INPUT_MOVE_UP,           padState.ucOnFootMove[2]);
	GET_ANALOG_KEY(INPUT_MOVE_DOWN,         padState.ucOnFootMove[3]);
	GET_ANALOG_KEY(INPUT_VEH_MOVE_LEFT,     padState.ucInVehicleMove[0]);
	GET_ANALOG_KEY(INPUT_VEH_MOVE_RIGHT,    padState.ucInVehicleMove[1]);
	GET_ANALOG_KEY(INPUT_VEH_MOVE_UP,       padState.ucInVehicleMove[2]);
	GET_ANALOG_KEY(INPUT_VEH_MOVE_DOWN,     padState.ucInVehicleMove[3]);
	GET_ANALOG_KEY(INPUT_VEH_BRAKE,         padState.ucInVehicleTriggers[0]);
	GET_ANALOG_KEY(INPUT_VEH_ACCELERATE,    padState.ucInVehicleTriggers[1]);

	// Binary keys
	GET_BINARY_KEY(INPUT_ENTER,             padState.keys.bEnterExitVehicle);
	GET_BINARY_KEY(INPUT_SPRINT,            padState.keys.bSprint);
	GET_BINARY_KEY(INPUT_JUMP,              padState.keys.bJump);
	GET_BINARY_KEY(INPUT_ATTACK,            padState.keys.bAttack);
	GET_BINARY_KEY(INPUT_ATTACK2,           padState.keys.bAttack2);
	GET_BINARY_KEY(INPUT_AIM,               padState.keys.bAim);
	GET_BINARY_KEY(INPUT_FREE_AIM,          padState.keys.bFreeAim);
	GET_BINARY_KEY(INPUT_MELEE_ATTACK1,     padState.keys.bMeleeAttack1);
	GET_BINARY_KEY(INPUT_MELEE_ATTACK2,     padState.keys.bMeleeAttack2);
	GET_BINARY_KEY(INPUT_MELEE_KICK,        padState.keys.bMeleeKick);
	GET_BINARY_KEY(INPUT_MELEE_BLOCK,       padState.keys.bMeleeBlock);
	GET_BINARY_KEY(INPUT_VEH_HANDBRAKE,     padState.keys.bHandbrake);
	GET_BINARY_KEY(INPUT_VEH_HANDBRAKE_ALT, padState.keys.bHandbrake2);
	GET_BINARY_KEY(INPUT_VEH_HORN,          padState.keys.bHorn);
	GET_BINARY_KEY(INPUT_VEH_ATTACK,        padState.keys.bDriveBy);
	GET_BINARY_KEY(INPUT_VEH_ATTACK2,       padState.keys.bHeliPrimaryFire);
}

void CIVPad::SetCurrentClientPadState(CPadState padState)
{
	if(m_pPad)
		FromPadState(padState, true);
}

void CIVPad::GetCurrentClientPadState(CPadState& padState)
{
	if(m_pPad)
		ToPadState(padState, true);
}

void CIVPad::SetLastClientPadState(CPadState padState)
{
	if(m_pPad)
		FromPadState(padState, false);
}

void CIVPad::GetLastClientPadState(CPadState& padState)
{
	if(m_pPad)
		ToPadState(padState, false);
}

void CIVPad::SetIsUsingController(bool bIsUsingController)
{
	if(m_pPad)
		m_pPad->m_bIsUsingController = bIsUsingController;
}

bool CIVPad::GetIsUsingController()
{
	if(m_pPad)
		return m_pPad->m_bIsUsingController;

	return false;
}

void CIVPad::SetLastUpdateTime(DWORD dwLastUpdateTime)
{
	if(m_pPad)
		m_pPad->m_dwLastUpdateTime = dwLastUpdateTime;
}

DWORD CIVPad::GetLastUpdateTime()
{
	if(m_pPad)
		return m_pPad->m_dwLastUpdateTime;

	return 0;
}
