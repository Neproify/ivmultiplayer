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

#include "IModuleNatives.h"

namespace Modules
{
	class ICServerModuleNatives
	{
	public:
		virtual void Log(const char * text) = 0;
		virtual void LogF(const char * text) = 0;
		virtual void AddRule(const char * rule, const char * value) = 0;
		virtual void RemoveRule(const char * rule) = 0;
		virtual void SetRule(const char * rule, const char * value) = 0;
		virtual const char * GetRule(const char * rule) = 0;
		virtual void Shutdown() = 0;
		virtual void SetPassword(const char * pass) = 0;
		virtual const char * GetPassword() = 0;
		virtual int GetPlayerCount() = 0;
		virtual int GetPlayerSlots() = 0;
		virtual unsigned int GetTickCount() = 0;
		virtual void SetHostName(const char * szHostname) = 0;
		virtual const char * GetHostName() = 0;
		virtual void TogglePayAndSpray(bool toggle) = 0;
		virtual void ToggleAutoAim(bool toggle) = 0;
		virtual void SendInput(const char * input) = 0;
		virtual const char * GetWeaponName(int iWeaponId) = 0;
		virtual const char * GetVehicleName(int iModelId) = 0;
		virtual void ToggleFrequentEvents(bool bToggle) = 0;
		virtual bool AreFrequentEventsEnabled() = 0;
		virtual bool SetWeather(int iWeather) = 0;
		virtual int GetWeather() = 0;
		virtual bool ForceWind(float fWind) = 0;
	};
}