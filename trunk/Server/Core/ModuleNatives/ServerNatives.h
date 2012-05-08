//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ServerNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CServerModuleNatives : public ICServerModuleNatives
	{
	public:
		void Log(const char * text);
		void LogF(const char * text);
		void AddRule(const char * rule, const char * value);
		void RemoveRule(const char * rule);
		void SetRule(const char * rule, const char * value);
		const char * GetRule(const char * rule);
		void Shutdown();
		void SetPassword(const char * pass);
		const char * GetPassword();
		int GetPlayerCount();
		int GetPlayerSlots();
		unsigned int GetTickCount();
		void SetHostName(const char * szHostname);
		const char * GetHostName();
		void TogglePayAndSpray(bool toggle);
		void ToggleAutoAim(bool toggle);
		void SendInput(const char * input);
		const char * GetWeaponName(int iWeaponId);
		const char * GetVehicleName(int iModelId);
		void ToggleFrequentEvents(bool bToggle);
		bool AreFrequentEventsEnabled();
		bool SetWeather(int iWeather);
		int GetWeather();
	};
}