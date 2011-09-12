//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: TrafficLights.cpp
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "patcher/CPatcher.h"
#include "COffsets.h"
#include <Game/CTrafficLights.h>

extern CTrafficLights * g_pTrafficLights;
CTrafficLights::eGTATrafficLightState dwTrafficLightState;

/*
Proof that you can sync every crap

colors loaded in sub_88C880
.data:01209120 fTrafficLightGreen__ColorRed dd ?
.data:01209124 fTrafficLightGreen__ColorGreen dd ?
.data:01209128 fTrafficLightGreen__ColorBlue dd ?
.data:0120912C fTrafficLightGreen__ColorAlpha? dd ?

.data:01209130 fTrafficLightAmber__ColorRed dd ?
.data:01209134 fTrafficLightAmber__ColorGreen dd ?
.data:01209138 fTrafficLightAmber__ColorBlue dd ?
.data:0120913C fTrafficLightAmber__ColorAlpha? dd ?

.data:01209150 fTrafficLightRed__ColorRed dd ?
.data:01209154 fTrafficLightRed__ColorGreen dd ?
.data:01209158 fTrafficLightRed__ColorBlue dd ?
.data:0120915C fTrafficLightRed__ColorAlpha? dd ?

*/

void __declspec(naked) GetTrafficLightState1()
{
	_asm
	{
		pushad;
	}

	dwTrafficLightState = g_pTrafficLights->GetTrafficLightState();

	_asm
	{
		popad;
		mov eax, dwTrafficLightState;
		retn;
	}
}

void __declspec(naked) GetTrafficLightState2()
{
	_asm
	{
		pushad;
	}

	dwTrafficLightState = g_pTrafficLights->GetTrafficLightAltState();

	_asm
	{
		popad;
		mov eax, dwTrafficLightState;
		retn;
	}
}

void InstallTrafficLightHooks()
{
	CPatcher::InstallJmpPatch(COffsets::FUNC_GetTrafficLightState1, (DWORD)GetTrafficLightState1, 5);
	CPatcher::InstallJmpPatch(COffsets::FUNC_GetTrafficLightState2, (DWORD)GetTrafficLightState2, 5);
}
