//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ServerNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptTimerManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/ticpp.h"
#include <CSettings.h>
#include "../CQuery.h"
#include <SharedUtility.h>

extern CPlayerManager    * g_pPlayerManager;
extern CNetworkManager   * g_pNetworkManager;
extern CQuery            * g_pQuery;
extern CScriptingManager * g_pScriptingManager;

void SendConsoleInput(String strInput);

// Server functions
namespace Modules
{

	// log(string)
	void CServerModuleNatives::Log(const char * text)
	{
		CLogFile::Print(text);
	}

	// logf(string)
	void CServerModuleNatives::LogF(const char * text)
	{
		CLogFile::PrintfToFile(text);
	}

	// addRule(name, value)
	void CServerModuleNatives::AddRule(const char * rule, const char * value)
	{
		if(g_pQuery)
			g_pQuery->AddRule(rule, value);
	}

	// removeRule(name)
	void CServerModuleNatives::RemoveRule(const char * rule)
	{
		if(g_pQuery)
			g_pQuery->RemoveRule(rule);
	}

	// setRule(name, value)
	void CServerModuleNatives::SetRule(const char * rule, const char * value)
	{
		if(g_pQuery)
			g_pQuery->SetRule(rule, value);
	}

	// getRule(name)
	const char * CServerModuleNatives::GetRule(const char * rule)
	{
		QueryRule * pRule = g_pQuery->GetRule(rule);

		if(pRule)
			return pRule->strValue.CopyData();
		else
			return NULL;
	}

	// shutdown()
	void CServerModuleNatives::Shutdown()
	{
		g_pNetworkManager->bRunning = false;
	}

	// setServerPassword(password)
	void CServerModuleNatives::SetPassword(const char * pass)
	{
		g_pNetworkManager->GetNetServer()->SetPassword(pass);
		CVAR_SET_STRING("password",String(pass));
	}

	// getServerPassword()
	const char * CServerModuleNatives::GetPassword()
	{
		String sPass = CVAR_GET_STRING("password");
		return sPass.C_String();
	}

	// getPlayerCount()
	int CServerModuleNatives::GetPlayerCount()
	{
		return g_pPlayerManager->GetPlayerCount();
	}

	// getPlayerSlots()
	int CServerModuleNatives::GetPlayerSlots()
	{
		return CVAR_GET_INTEGER("maxplayers");
	}

	// getTickCount()
	unsigned int CServerModuleNatives::GetTickCount()
	{
		return SharedUtility::GetTime();
	}

	// setHostname(hostname)
	void CServerModuleNatives::SetHostName(const char * szHostname)
	{
		CVAR_SET_STRING("hostname", String(szHostname));
	}

	// getHostname()
	const char * CServerModuleNatives::GetHostName()
	{
		return CVAR_GET_STRING("hostname");
	}

	// togglePayAndSpray(toggle)
	void CServerModuleNatives::TogglePayAndSpray(bool toggle)
	{
		CVAR_SET_BOOL("paynspray", toggle);
		CBitStream bsSend;
		bsSend.Write(toggle);
		g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	}

	// toggleAutoAim(toggle)
	void CServerModuleNatives::ToggleAutoAim(bool toggle)
	{
		CVAR_SET_BOOL("autoaim", toggle);
		CBitStream bsSend;
		bsSend.Write(toggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	}

	// sendConsoleInput(input)
	void CServerModuleNatives::SendInput(const char * input)
	{
		SendConsoleInput((char*)input);
	}

	/*
	// ADAMIX: todo
	// getConfig()
	SQInteger CServerModuleNatives::GetConfig(SQVM * pVM)
	{
		// Create a new table
		sq_newtable(pVM);

		for(std::map<String, SettingsValue *>::iterator iter = CSettings::GetValues()->begin(); iter != CSettings::GetValues()->end(); iter++)
		{
			// Get the setting
			SettingsValue * setting = iter->second;

			// Push the setting name onto the stack
			sq_pushstring(pVM, iter->first.Get(), iter->first.GetLength());

			// Push the value onto the stack
			if(setting->IsBool())
				sq_pushbool(pVM, setting->bValue);
			else if(setting->IsInteger())
				sq_pushinteger(pVM, setting->iValue);
			else if(setting->IsFloat())
				sq_pushfloat(pVM, setting->fValue);
			else if(setting->IsString())
				sq_pushstring(pVM, setting->strValue, setting->strValue.GetLength());
			else if(setting->IsList())
			{
				// Create a new array
				sq_newarray(pVM, 0);

				for(std::list<String>::iterator iter2 = setting->listValue.begin(); iter2 != setting->listValue.end(); iter2++)
				{
					// Push the list value onto the stack
					sq_pushstring(pVM, (*iter2).Get(), (*iter2).GetLength());

					// Create a new array slot
					sq_arrayappend(pVM, -2);
				}
			}

			// Create a new table slot
			sq_createslot(pVM, -3);
		}

		return 1;
	}
	*/

	// getWeaponName(weaponid)
	const char * CServerModuleNatives::GetWeaponName(int iWeaponId)
	{
		switch(iWeaponId)
		{
		case 0:
			return "Fists";
			break;
		case 1:
			return "Baseball Bat";
			break;
		case 2:
			return "Pool Cue";
			break;
		case 3:
			return "Knife";
			break;
		case 4:
			return "Grenade";
			break;
		case 5:
			return "Molotov Cocktail";
			break;
		case 7:
			return "Pistol";
			break;
		case 9:
			return "Desert Eagle";
			break;
		case 10:
			return "Shotgun";
			break;
		case 11:
			return "Baretta";
			break;
		case 12:
			return "Micro UZI";
			break;
		case 13:
			return "MP5";
			break;
		case 14:
			return "AK-47";
			break;
		case 15:
			return "M4";
			break;
		case 16:
			return "Sniper Rifle";
			break;
		case 17:
			return "M40-A1";
			break;
		case 18:
			return "Rocket Launcher";
			break;
		case 19:
			return "Flame Thrower";
			break;
		case 20:
			return "Minigun";
			break;
		default:
			return NULL;
			break;
		}
	
		return NULL;
	}


	char szVehicleNames[124][32] = 
	{
		"Admiral", "Airtug", "Ambulance", "Banshee", "Benson", "Biff", "Blista Compact", "Bobcat", "Boxville",
		"Buccaneer", "Burrito", "Burrito", "Bus", "Cabby", "Cavalcade", "Chavos", "Cognoscenti", "Comet",
		"Coquette", "DF8", "Dilettante", "Dukes", "E109", "Emperor", "Emperor", "Esperanto", "Faction", "FBI",
		"Feltzer", "Feroci", "Feroci", "Firetruck", "Flatbed", "Fortune", "Forklift", "Futo", "FXT", "Habanero",
		"Hakumai", "Huntley", "Infernus", "Ingot", "Intruder", "Landstalker", "Lokus", "Manana", "Marbella",
		"Merit", "Minivan", "Moonbeam", "Mr Tasty", "Mule", "Noose", "Noose Stockade", "Oracle", "Packer", "Patriot",
		"Perennial", "Perennial", "Peyote", "Phantom", "Pinnacle", "PMP600", "Police", "Police", "Police Patriot",
		"Pony", "Premier", "Pres", "Primo", "Police Stockade", "Rancher", "Rebla", "Ripley", "Romero", "Rom",
		"Ruiner", "Sabre", "Sabre", "Sabre GT", "Schafter", "Sentinel", "Solair", "Speedo", "Stalion", "Steed",
		"Stockade", "Stratum", "Stretch", "Sultan", "Sultan RS", "Super GT", "Taxi", "Taxi", "Trash", "Turismo",
		"Uranus", "Vigero", "Vigero", "Vincent", "Virgo", "Voodoo", "Washington", "Willard", "Yankee", "Freeway",
		"Faggio", "Hellfury", "NRG 900", "PCJ600", "Sanchez", "Zombie", "Annihilator", "Maverick", "Police Maverick",
		"Tour Maverick", "Dinghy", "Jetmax", "Marquis", "Predator", "Reefer", "Squalo", "Tug", "Tropic",
	};

	// getVehicleName(modelid)
	const char * CServerModuleNatives::GetVehicleName(int iModelId)
	{
		if(iModelId < 0 || iModelId > 123)
		{
			return NULL;
		}

		return szVehicleNames[iModelId];
	}

	// toggleFrequentEvents(toggle)
	void CServerModuleNatives::ToggleFrequentEvents(bool bToggle)
	{
		CVAR_SET_BOOL("frequentevents", bToggle);
	}

	// areFrequentEventsEnabled()
	bool CServerModuleNatives::AreFrequentEventsEnabled()
	{
		return CVAR_GET_BOOL("frequentevents");
	}

	// setWeather(weather)
	bool CServerModuleNatives::SetWeather(int iWeather)
	{
		if(iWeather >= 1 && iWeather <= 10 && iWeather != CVAR_GET_INTEGER("weather"))
		{
			CVAR_SET_INTEGER("weather", (int)iWeather);

			CBitStream bsSend;
			bsSend.Write((unsigned char)iWeather);
			g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

			return true;
		}

		return false;
	}

	// getWeather(weather)
	int CServerModuleNatives::GetWeather()
	{
		return CVAR_GET_INTEGER("weather");
	}

	bool CServerModuleNatives::ForceWind(float fWind)
	{
		if(fWind != CVAR_GET_INTEGER("wind"))
		{
			CVAR_SET_FLOAT("wind", (float)fWind);

			CBitStream bsSend;
			bsSend.Write((float)fWind);
			g_pNetworkManager->RPC(RPC_ScriptingForceWind, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return true;
		}
		return false;
	}
}