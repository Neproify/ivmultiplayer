//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWeather.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../IVCommon.h"

// From aru's GTAIV C++ ScriptHook
enum eWeather
{
	WEATHER_EXTRA_SUNNY,
	WEATHER_SUNNY,
	WEATHER_SUNNY_WINDY,
	WEATHER_CLOUDY,
	WEATHER_RAINING,
	WEATHER_DRIZZLE,
	WEATHER_FOGGY,
	WEATHER_LIGHTNING,
	WEATHER_EXTRA_SUNNY_2,	// Not sure what these are, but sounds for these
	WEATHER_SUNNY_WINDY_2,	// two use the same as EXTRA_SUNNY and SUNNY_WINDY
	WEATHER_COUNT
};

class CIVWeather
{
public:
	void     SetWeather(eWeather weather);
	eWeather GetWeather();
};