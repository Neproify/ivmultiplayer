//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPlayerPed.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPlayerPed.h"
#include <CLogFile.h>

CIVPlayerPed::CIVPlayerPed() : CIVPed()
{
}

CIVPlayerPed::CIVPlayerPed(IVPlayerPed * pPlayerPed) : CIVPed(pPlayerPed)
{
}

CIVPlayerPed::~CIVPlayerPed()
{
	CIVPed::~CIVPed();
}

void CIVPlayerPed::SetPlayerPed(IVPlayerPed * pPlayerPed)
{
	SetPed(pPlayerPed);
}

IVPlayerPed * CIVPlayerPed::GetPlayerPed()
{
	return (IVPlayerPed *)GetPed();
}