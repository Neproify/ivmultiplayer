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

DWORD dwOriginalIndicatorFunc = 0;
IVVehicle * pIndicatorIVVehicle;
CNetworkVehicle * pIndicatorVehicle;
int iIndicatorType;

void _declspec(naked) CVehicle__DrawIndicator()
{
	_asm
	{
		mov eax, [ebp]
		mov iIndicatorType, eax
		mov pIndicatorIVVehicle, ecx
		pushad
	}

	// 54 = Front Left
	// 55 = Front Right
	// 56 = Back Left
	// 57 = Back Right
	iIndicatorType -= 54; // Subtract 54 to make it a valid value for CNetworkVehicle::GetIndicatorState
	pIndicatorVehicle = g_pStreamer->GetVehicleFromGameVehicle(pIndicatorIVVehicle);

	if(pIndicatorVehicle != NULL && pIndicatorVehicle->GetIndicatorState(iIndicatorType))
	{
		_asm
		{
			popad
			jmp dwOriginalIndicatorFunc
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
	// Save the original CNetworkVehicle::DrawIndicator function address
	dwOriginalIndicatorFunc = COffsets::FUNC_CVehicle__DrawIndicator;

	// TODO: When you're away from a vehicle and look at the front indicators, there's two possibilities:
	//       Front Left Indicator is on -> from a distance, both appear to be on
	//       Front Right Indicator is on -> from a distance, both appear to be off
	//       Back indicators seem to work normally, when you're closer to the cars front indicators work fine

	// Check if hazard lights should be drawn -> always
	CPatcher::InstallNopPatch(COffsets::PATCH_CVehicle__HazzardLightsOn, 2);

	// TODO: Hook CNetworkVehicle::DrawIndicator function instead of all individual calls to it
	// Draws individual indicators
	CPatcher::InstallCallPatch(COffsets::PATCH_CVehicle__DrawIndicator1, (DWORD)CVehicle__DrawIndicator, 5);
	CPatcher::InstallCallPatch(COffsets::PATCH_CVehicle__DrawIndicator2, (DWORD)CVehicle__DrawIndicator, 5);
	CPatcher::InstallCallPatch(COffsets::PATCH_CVehicle__DrawIndicator3, (DWORD)CVehicle__DrawIndicator, 5);
	CPatcher::InstallCallPatch(COffsets::PATCH_CVehicle__DrawIndicator4, (DWORD)CVehicle__DrawIndicator, 5);
}
