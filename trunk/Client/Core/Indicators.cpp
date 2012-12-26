//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Indicators.cpp
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVVehicle.h"
#include "COffsets.h"
#include "Patcher/CPatcher.h"
#include "CNetworkVehicle.h"
#include "CStreamer.h"

extern CStreamer * g_pStreamer;

IVVehicle * pIndicatorIVVehicle;
int         iIndicatorType;

bool GetIndicatorState(IVVehicle * pGameVehicle, int iIndicatorNumber)
{
	if(!pGameVehicle)
		return false;

	if(iIndicatorNumber < 0 || iIndicatorNumber > 3)
		return false;

	CNetworkVehicle * pVehicle = g_pStreamer->GetVehicleFromGameVehicle(pGameVehicle);

	if(!pVehicle)
		return false;

	return pVehicle->GetIndicatorState(iIndicatorNumber);
}

// TODO: turn indicator audio(sound) off when turning indicator off...
void _declspec(naked) CVehicle__DrawIndicator_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
		mov eax, [ebp+8]
		mov iIndicatorType, eax
		pop ebp
		mov pIndicatorIVVehicle, ecx
		pushad
	}

	// 54 = Front Left
	// 55 = Front Right
	// 56 = Back Left
	// 57 = Back Right
	iIndicatorType -= 54; // Subtract 54 to make it a valid indicator number (0-3)

	if(GetIndicatorState(pIndicatorIVVehicle, iIndicatorType))
	{
		_asm
		{
			popad
			push ebp
			mov ebp, esp
			and esp, 0FFFFFFF0h
			jmp COffsets::RETURN_CVehicle__DrawIndicator
		}
	}
	else
	{
		_asm
		{
			popad
			retn 10h
		}
	}
}

void InstallIndicatorHooks()
{
	// TODO: When you're away from a vehicle and look at the front indicators, there's two possibilities:
	//       Front Left Indicator is on -> from a distance, both appear to be on
	//       Front Right Indicator is on -> from a distance, both appear to be off
	//       Back indicators seem to work normally, when you're closer to the cars front indicators work fine

	// Always draw vehicle hazzard lights
	CPatcher::InstallNopPatch(COffsets::PATCH_CVehicle__HazzardLightsOn, 2);

	// Hook CVehicle::DrawIndicator to use our hook function
	CPatcher::InstallJmpPatch(COffsets::FUNC_CVehicle__DrawIndicator, (DWORD)CVehicle__DrawIndicator_Hook, 5);
}
