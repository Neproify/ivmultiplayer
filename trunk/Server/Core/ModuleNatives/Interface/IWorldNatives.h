//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICWorldModuleNatives
	{
	public:
		virtual bool SetTime(int iHour, int iMinute) = 0;
		virtual bool SetMinuteDuration(int iMinuteDuration) = 0;
		virtual int GetMinuteDuration() = 0;
		virtual bool SetDayOfWeek(int iDay) = 0;
		virtual int GetDayOfWeek() = 0;
		virtual bool SetTrafficLightsState(int iState) = 0;
		virtual int GetTrafficLightsState() = 0;
		virtual bool SetTrafficLightsLocked(bool b) = 0;
		virtual bool AreTrafficLightsLocked() = 0;
		virtual bool SetTrafficLightsPhaseDuration(int iPhase, int iDuration) = 0;
		virtual bool ResetTrafficLightsPhaseDuration() = 0;
	};
}