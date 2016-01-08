//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: TrafficLightsNatives.cpp
// Project: Shared
// Author(s): mabako, Neproify
// License: See LICENSE in root directory
//
//==============================================================================

#include "TrafficLightsNatives.h"
#include "../../Game/CTrafficLights.h"
#include "../../CLogFile.h"

_BEGIN_CLASS(trafficLights)
_MEMBER_FUNCTION(trafficLights, setState, 1, "i")
_MEMBER_FUNCTION(trafficLights, getState, 0, NULL)
_MEMBER_FUNCTION(trafficLights, setLocked, 1, "b")
_MEMBER_FUNCTION(trafficLights, areLocked, 0, NULL)
_MEMBER_FUNCTION(trafficLights, setPhaseDuration, 2, "ii")
_MEMBER_FUNCTION(trafficLights, getPhaseDuration, 0, NULL)
_MEMBER_FUNCTION(trafficLights, resetPhaseDuration, 0, NULL)
_END_CLASS(trafficLights)

void RegisterTrafficLightsNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(trafficLights));
}

// setState(state)
_MEMBER_FUNCTION_IMPL(trafficLights, setState)
{
	SQInteger iState;
	sq_getinteger(pVM, 2, &iState);
	if (iState > 0 && iState <= 3)
	{
		sq_pushbool(pVM, CTrafficLights::GetInstance()->SetState((CTrafficLights::eTrafficLightState)iState));
		return 1;
	}
	else
	{
		CLogFile::Printf("Failed to set TrafficLightsState to %d(Only available from 0 to 3)", iState);
		sq_pushbool(pVM, false);
		return 1;
	}
}

// getState()
_MEMBER_FUNCTION_IMPL(trafficLights, getState)
{
	sq_pushinteger(pVM, (int)CTrafficLights::GetInstance()->GetState());
	return 1;
}

// setLocked(bool locked)
_MEMBER_FUNCTION_IMPL(trafficLights, setLocked)
{
	SQBool b;
	sq_getbool(pVM, 2, &b);
	
	CTrafficLights::GetInstance()->SetLocked(b != 0);
	sq_pushbool(pVM, true);
	return 1;
}

// areLocked()
_MEMBER_FUNCTION_IMPL(trafficLights, areLocked)
{
	sq_pushbool(pVM, CTrafficLights::GetInstance()->IsLocked());
	return 1;
}

// setPhaseDuration(phase, duration)
_MEMBER_FUNCTION_IMPL(trafficLights, setPhaseDuration)
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

// getPhaseDuration()
_MEMBER_FUNCTION_IMPL(trafficLights, getPhaseDuration)
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

// resetPhaseDuration()
_MEMBER_FUNCTION_IMPL(trafficLights, resetPhaseDuration)
{
	CTrafficLights::GetInstance()->ResetDefaultDurations();
	sq_pushbool(pVM, true);
	return 1;
}