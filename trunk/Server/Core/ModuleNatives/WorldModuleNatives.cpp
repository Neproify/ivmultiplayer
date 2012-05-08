//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.cpp
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "../../../Shared/Game/CTime.h"
#include "../../../Shared/Game/CTrafficLights.h"
#include "../../../Shared/CLogFile.h"

extern CTime * g_pTime;
extern CTrafficLights * g_pTrafficLights;

namespace Modules
{
	// setTime(hour, minute)
	bool CWorldModuleNatives::SetTime(int iHour, int iMinute)
	{
		if(iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59)
		{
			g_pTime->SetTime(iHour, iMinute);
			return true;
		}

		return false;
	}
	/*
	// getTime()
	int CWorldModuleNatives::GetTime(SQVM * pVM)
	{
		unsigned char ucHour = 0, ucMinute = 0;
		g_pTime->GetTime(&ucHour, &ucMinute);

		sq_newarray(pVM, 0);

		sq_pushinteger(pVM, ucHour);
		sq_arrayappend(pVM, -2);

		sq_pushinteger(pVM, ucMinute);
		sq_arrayappend(pVM, -2);

		return 1;
	}
	*/ // ADAMIX: todo

	// setMinuteDuration(milliseconds)
	bool CWorldModuleNatives::SetMinuteDuration(int iMinuteDuration)
	{
		if(iMinuteDuration > 0)
		{
			g_pTime->SetMinuteDuration(iMinuteDuration);
			return true;
		}
		else if(iMinuteDuration < 1)
		{
			CLogFile::Printf("Failed to set MinuteDuration for %d ms(Minimum 1ms)",iMinuteDuration);
			return false;
		}
		return false;
	}

	// getMinuteDuration()
	int CWorldModuleNatives::GetMinuteDuration()
	{
		return g_pTime->GetMinuteDuration();
	}

	// setDayOfWeek(day)
	bool CWorldModuleNatives::SetDayOfWeek(int iDay)
	{
		if(iDay >= 0 && iDay <= 6)
		{
			g_pTime->SetDayOfWeek(iDay);
			return true;
		}

		return false;
	}

	// getDayOfWeek()
	int CWorldModuleNatives::GetDayOfWeek()
	{
		return g_pTime->GetDayOfWeek();
	}

	// setTrafficLightsState(state)
	bool CWorldModuleNatives::SetTrafficLightsState(int iState)
	{
		if(iState > 0 && iState <= 3)
		{
			return g_pTrafficLights->SetState((CTrafficLights::eTrafficLightState)iState);
		}
		else
		{
			CLogFile::Printf("Failed to set TrafficLightsState to %d(Only available from 0 to 3)",iState);
			return false;
		}
	}

	// getTrafficLightsState()
	int CWorldModuleNatives::GetTrafficLightsState()
	{
		return (int)g_pTrafficLights->GetState();
	}

	// setTrafficLightsLocked(bool locked)
	bool CWorldModuleNatives::SetTrafficLightsLocked(bool b)
	{
		g_pTrafficLights->SetLocked(b != 0);
		return true;
	}

	// areTrafficLightsLocked()
	bool CWorldModuleNatives::AreTrafficLightsLocked()
	{
		return g_pTrafficLights->IsLocked();
	}

	// setTrafficLightsPhaseDuration(phase, duration)
	bool CWorldModuleNatives::SetTrafficLightsPhaseDuration(int iPhase, int iDuration)
	{
		if(iDuration > 0)
		{
			if(iPhase > 0 && iPhase <= 3)
			{
				switch(iPhase)
				{
					case 1:
						g_pTrafficLights->SetGreenDuration(iDuration);
						break;
					case 2:
						g_pTrafficLights->SetYellowDuration(iDuration);
						break;
					case 3:
						g_pTrafficLights->SetRedDuration(iDuration);
						break;
				}
				return true;
			}
			else
			{
				CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Trafficlights are only supported from 1 to 3)",iPhase,iDuration);
				return false;
			}
		}
		else
		{
			CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Minimum duration: 1ms)",iPhase,iDuration);
			return false;
		}
		return false;
	}

	/*
	// getTrafficLightsPhaseDuration()
	bool CWorldModuleNatives::GetTrafficLightsPhaseDuration(SQVM * pVM)
	{
		sq_newarray(pVM, 0);

		sq_pushbool(pVM, g_pTrafficLights->IsUsingDefaultDurations());
		sq_arrayappend(pVM, -2);

		sq_pushinteger(pVM, g_pTrafficLights->GetGreenDuration());
		sq_arrayappend(pVM, -2);

		sq_pushinteger(pVM, g_pTrafficLights->GetYellowDuration());
		sq_arrayappend(pVM, -2);

		sq_pushinteger(pVM, g_pTrafficLights->GetRedDuration());
		sq_arrayappend(pVM, -2);

		return 1;
	}
	*/ // ADAMIX: todo

	// resetTrafficLightsPhaseDuration()
	bool CWorldModuleNatives::ResetTrafficLightsPhaseDuration()
	{
		g_pTrafficLights->ResetDefaultDurations();
		return true;
	}
}