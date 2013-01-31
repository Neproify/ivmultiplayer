//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.cpp
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "WorldNatives.h"
#include "../../Game/CTime.h"
#include "../../Game/CTrafficLights.h"
#include "../../CLogFile.h"

void CWorldNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("setTime", SetTime, 2, "ii");
	pScriptingManager->RegisterFunction("getTime", GetTime, 0, NULL);
	pScriptingManager->RegisterFunction("setMinuteDuration", SetMinuteDuration, 1, "i");
	pScriptingManager->RegisterFunction("getMinuteDuration", GetMinuteDuration, 0, NULL);
	pScriptingManager->RegisterFunction("setDayOfWeek", SetDayOfWeek, 1, "i");
	pScriptingManager->RegisterFunction("getDayOfWeek", GetDayOfWeek, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsState", SetTrafficLightsState, 1, "i");
	pScriptingManager->RegisterFunction("getTrafficLightsState", GetTrafficLightsState, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsLocked", SetTrafficLightsLocked, 1, "b");
	pScriptingManager->RegisterFunction("areTrafficLightsLocked", AreTrafficLightsLocked, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsPhaseDuration", SetTrafficLightsPhaseDuration, 2, "ii");
	pScriptingManager->RegisterFunction("getTrafficLightsPhaseDuration", GetTrafficLightsPhaseDuration, 0, NULL);
	pScriptingManager->RegisterFunction("resetTrafficLightsPhaseDuration",ResetTrafficLightsPhaseDuration,0,NULL);
}

// setTime(hour, minute)
SQInteger CWorldNatives::SetTime(SQVM * pVM)
{
	SQInteger iHour;
	SQInteger iMinute;
	sq_getinteger(pVM, 2, &iHour);
	sq_getinteger(pVM, 3, &iMinute);

	if(iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59)
	{
		CTime::GetInstance()->SetTime(iHour, iMinute);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getTime()
SQInteger CWorldNatives::GetTime(SQVM * pVM)
{
	unsigned char ucHour = 0, ucMinute = 0;
	CTime::GetInstance()->GetTime(&ucHour, &ucMinute);

	sq_newarray(pVM, 0);

	sq_pushinteger(pVM, ucHour);
	sq_arrayappend(pVM, -2);

	sq_pushinteger(pVM, ucMinute);
	sq_arrayappend(pVM, -2);

	return 1;
}

// setMinuteDuration(milliseconds)
SQInteger CWorldNatives::SetMinuteDuration(SQVM * pVM)
{
	SQInteger iMinuteDuration;
	sq_getinteger(pVM, 2, &iMinuteDuration);

	if(iMinuteDuration > 0)
	{
		CTime::GetInstance()->SetMinuteDuration(iMinuteDuration);
		sq_pushbool(pVM, true);
		return 1;
	}
	else if(iMinuteDuration < 1)
	{
		CLogFile::Printf("Failed to set MinuteDuration for %d ms(Minimum 1ms)", iMinuteDuration);
		sq_pushbool(pVM, false);
		return 1;
	}

	return 1;
}

// getMinuteDuration()
SQInteger CWorldNatives::GetMinuteDuration(SQVM * pVM)
{
	sq_pushinteger(pVM, CTime::GetInstance()->GetMinuteDuration());
	return 1;
}

// setDayOfWeek(day)
SQInteger CWorldNatives::SetDayOfWeek(SQVM * pVM)
{
	SQInteger iDay;
	sq_getinteger(pVM, 2, &iDay);

	if(iDay >= 0 && iDay <= 6)
	{
		CTime::GetInstance()->SetDayOfWeek(iDay);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getDayOfWeek()
SQInteger CWorldNatives::GetDayOfWeek(SQVM * pVM)
{
	sq_pushinteger(pVM, CTime::GetInstance()->GetDayOfWeek());
	return 1;
}

// setTrafficLightsState(state)
SQInteger CWorldNatives::SetTrafficLightsState(SQVM * pVM)
{
	SQInteger iState;
	sq_getinteger(pVM, 2, &iState);
	if(iState > 0 && iState <= 3)
	{
		sq_pushbool(pVM, CTrafficLights::GetInstance()->SetState((CTrafficLights::eTrafficLightState)iState));
		return 1;
	}
	else
	{
		CLogFile::Printf("Failed to set TrafficLightsState to %d(Only available from 0 to 3)",iState);
		sq_pushbool(pVM,false);
		return 1;
	}
}

// getTrafficLightsState()
SQInteger CWorldNatives::GetTrafficLightsState(SQVM * pVM)
{
	sq_pushinteger(pVM, (int)CTrafficLights::GetInstance()->GetState());
	return 1;
}

// setTrafficLightsLocked(bool locked)
SQInteger CWorldNatives::SetTrafficLightsLocked(SQVM * pVM)
{
	SQBool b;
	sq_getbool(pVM, 2, &b);
	
	CTrafficLights::GetInstance()->SetLocked(b != 0);
	sq_pushbool(pVM, true);
	return 1;
}

// areTrafficLightsLocked()
SQInteger CWorldNatives::AreTrafficLightsLocked(SQVM * pVM)
{
	sq_pushbool(pVM, CTrafficLights::GetInstance()->IsLocked());
	return 1;
}

// setTrafficLightsPhaseDuration(phase, duration)
SQInteger CWorldNatives::SetTrafficLightsPhaseDuration(SQVM * pVM)
{
	SQInteger iPhase, iDuration;
	sq_getinteger(pVM, 2, &iPhase);
	sq_getinteger(pVM, 3, &iDuration);

	if(iDuration > 0)
	{
		if(iPhase > 0 && iPhase <= 3)
		{
			switch(iPhase)
			{
				case 1:
					CTrafficLights::GetInstance()->SetGreenDuration(iDuration);
					break;
				case 2:
					CTrafficLights::GetInstance()->SetYellowDuration(iDuration);
					break;
				case 3:
					CTrafficLights::GetInstance()->SetRedDuration(iDuration);
					break;
			}

			sq_pushbool(pVM, true);
			return 1;
		}
		else
		{
			CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Trafficlights are only supported from 1 to 3)",iPhase,iDuration);
			sq_pushbool(pVM, false);
			return 1;
		}
	}

	CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Minimum duration: 1ms)",iPhase,iDuration);
	sq_pushbool(pVM, false);
	return 1;
}

// getTrafficLightsPhaseDuration()
SQInteger CWorldNatives::GetTrafficLightsPhaseDuration(SQVM * pVM)
{
	sq_newarray(pVM, 0);

	sq_pushbool(pVM, CTrafficLights::GetInstance()->IsUsingDefaultDurations());
	sq_arrayappend(pVM, -2);

	sq_pushinteger(pVM, CTrafficLights::GetInstance()->GetGreenDuration());
	sq_arrayappend(pVM, -2);

	sq_pushinteger(pVM, CTrafficLights::GetInstance()->GetYellowDuration());
	sq_arrayappend(pVM, -2);

	sq_pushinteger(pVM, CTrafficLights::GetInstance()->GetRedDuration());
	sq_arrayappend(pVM, -2);

	return 1;
}

// resetTrafficLightsPhaseDuration()
SQInteger CWorldNatives::ResetTrafficLightsPhaseDuration(SQVM * pVM)
{
	CTrafficLights::GetInstance()->ResetDefaultDurations();
	sq_pushbool(pVM, true);
	return 1;
}
