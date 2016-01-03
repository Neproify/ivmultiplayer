//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: WorldNatives.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CWorldModuleNatives : public ICWorldModuleNatives
	{
	public:
		bool SetTime(int iHour, int iMinute);
		bool SetMinuteDuration(int iMinuteDuration);
		int GetMinuteDuration();
		bool SetDayOfWeek(int iDay);
		int GetDayOfWeek();
		bool SetTrafficLightsState(int iState);
		int GetTrafficLightsState();
		bool SetTrafficLightsLocked(bool b);
		bool AreTrafficLightsLocked();
		bool SetTrafficLightsPhaseDuration(int iPhase, int iDuration);
		bool ResetTrafficLightsPhaseDuration();
	};
}