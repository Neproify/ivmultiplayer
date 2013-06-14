//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWeather.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVWeather.h"
#include "../COffsets.h"
#include <CLogFile.h>

void CIVWeather::SetWeather(eWeather weather)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(weather >= WEATHER_EXTRA_SUNNY && weather < WEATHER_COUNT)
	{
		DWORD dwFunc = COffsets::FUNC_SetWeather;
		_asm
		{
			push weather
			call dwFunc
			add esp, 4
		}
	}
}

eWeather CIVWeather::GetWeather()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	return *(eWeather *)(COffsets::VAR_CurrentWeather);
}