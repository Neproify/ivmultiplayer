//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
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

_BEGIN_CLASS(world)
_MEMBER_FUNCTION(world, setTime, 2, "ii")
_MEMBER_FUNCTION(world, getTime, 0, NULL)
_MEMBER_FUNCTION(world, setMinuteDuration, 1, "i")
_MEMBER_FUNCTION(world, getMinuteDuration, 0, NULL)
_MEMBER_FUNCTION(world, setDayOfWeek, 1, "i")
_MEMBER_FUNCTION(world, getDayOfWeek, 0, NULL)
_END_CLASS(world)

void RegisterWorldNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(world));
}

// setTime(hour, minute)
_MEMBER_FUNCTION_IMPL(world, setTime)
{
	SQInteger iHour;
	SQInteger iMinute;
	sq_getinteger(pVM, 2, &iHour);
	sq_getinteger(pVM, 3, &iMinute);

	if (iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59)
	{
		CTime::GetInstance()->SetTime(iHour, iMinute);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getTime()
_MEMBER_FUNCTION_IMPL(world, getTime)
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
_MEMBER_FUNCTION_IMPL(world, setMinuteDuration)
{
	SQInteger iMinuteDuration;
	sq_getinteger(pVM, 2, &iMinuteDuration);

	if (iMinuteDuration > 0)
	{
		CTime::GetInstance()->SetMinuteDuration(iMinuteDuration);
		sq_pushbool(pVM, true);
		return 1;
	}
	else if (iMinuteDuration < 1)
	{
		CLogFile::Printf("Failed to set MinuteDuration for %d ms(Minimum 1ms)", iMinuteDuration);
		sq_pushbool(pVM, false);
		return 1;
	}

	return 1;
}

// getMinuteDuration()
_MEMBER_FUNCTION_IMPL(world, getMinuteDuration)
{
	sq_pushinteger(pVM, CTime::GetInstance()->GetMinuteDuration());
	return 1;
}

// setDayOfWeek(day)
_MEMBER_FUNCTION_IMPL(world, setDayOfWeek)
{
	SQInteger iDay;
	sq_getinteger(pVM, 2, &iDay);

	if (iDay >= 0 && iDay <= 6)
	{
		CTime::GetInstance()->SetDayOfWeek(iDay);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getDayOfWeek()
_MEMBER_FUNCTION_IMPL(world, getDayOfWeek)
{
	sq_pushinteger(pVM, CTime::GetInstance()->GetDayOfWeek());
	return 1;
}

// move it to own class
/*void CWorldNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("setTrafficLightsState", SetTrafficLightsState, 1, "i");
	pScriptingManager->RegisterFunction("getTrafficLightsState", GetTrafficLightsState, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsLocked", SetTrafficLightsLocked, 1, "b");
	pScriptingManager->RegisterFunction("areTrafficLightsLocked", AreTrafficLightsLocked, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsPhaseDuration", SetTrafficLightsPhaseDuration, 2, "ii");
	pScriptingManager->RegisterFunction("getTrafficLightsPhaseDuration", GetTrafficLightsPhaseDuration, 0, NULL);
	pScriptingManager->RegisterFunction("resetTrafficLightsPhaseDuration",ResetTrafficLightsPhaseDuration,0,NULL);
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
}*/
