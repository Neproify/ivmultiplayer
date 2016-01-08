//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: WorldNatives.cpp
// Project: Shared
// Author(s): mabako, Neproify
// License: See LICENSE in root directory
//
//==============================================================================

#include "WorldNatives.h"
#include "../../Game/CTime.h"
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
