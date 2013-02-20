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

#include "../Natives.h"
#include "../CServer.h"
#include "Scripting/CScriptTimerManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/ticpp.h"
#include <CSettings.h>
#include "../CQuery.h"
#include <SharedUtility.h>

extern CServer           * g_pServer;
extern CPlayerManager    * g_pPlayerManager;
extern CNetworkManager   * g_pNetworkManager;
extern CQuery            * g_pQuery;
extern CScriptingManager * g_pScriptingManager;

// Server functions

void CServerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("log", Log, 1, "s");
	pScriptingManager->RegisterFunction("logf", LogF, 1, "s");
	pScriptingManager->RegisterFunction("addRule", AddRule, 2, "ss");
	pScriptingManager->RegisterFunction("removeRule", RemoveRule, 1, "s");
	pScriptingManager->RegisterFunction("setRule", SetRule, 2, "ss");
	pScriptingManager->RegisterFunction("getRule", GetRule, 1, "s");
	pScriptingManager->RegisterFunction("shutdown", Shutdown, 0, NULL);
	pScriptingManager->RegisterFunction("setServerPassword", SetPassword, 1, "s");
	pScriptingManager->RegisterFunction("getServerPassword", GetPassword, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerCount", GetPlayerCount, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayers", GetPlayers, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerSlots", GetPlayerSlots, 0, NULL);
	pScriptingManager->RegisterFunction("getTickCount", GetTickCount, 0, NULL);
	pScriptingManager->RegisterFunction("setHostname", SetHostName, 1, "s");
	pScriptingManager->RegisterFunction("getHostname", GetHostName, 0, NULL);
	pScriptingManager->RegisterFunction("togglePayAndSpray", TogglePayAndSpray, 1, "b");
	pScriptingManager->RegisterFunction("toggleAutoAim", ToggleAutoAim, 1, "b");
	pScriptingManager->RegisterFunction("sendConsoleInput", SendInput, 1, "s");
	pScriptingManager->RegisterFunction("getConfig", GetConfig, 0, NULL);
	pScriptingManager->RegisterFunction("getWeaponName", GetWeaponName, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleName", GetVehicleName, 1, "i");
	pScriptingManager->RegisterFunction("toggleFrequentEvents", ToggleFrequentEvents, 1, "b");
	pScriptingManager->RegisterFunction("areFrequentEventsEnabled", AreFrequentEventsEnabled, 0, NULL);
	pScriptingManager->RegisterFunction("forceWind",ForceWind,1,"f");
	pScriptingManager->RegisterFunction("setWeather", SetWeather, 1, "i");
	pScriptingManager->RegisterFunction("getWeather", GetWeather, 0, NULL);
}

// log(string)
SQInteger CServerNatives::Log(SQVM * pVM)
{
	const char* text;
	sq_getstring(pVM, -1, &text);
	CLogFile::Print(text);
	sq_pushbool(pVM, true);
	return 1;
}

// logf(string)
SQInteger CServerNatives::LogF(SQVM * pVM)
{
	const char * text;
	sq_getstring(pVM, -1, &text);
	CLogFile::PrintfToFile(text);
	sq_pushbool(pVM, true);
	return 1;
}

// addRule(name, value)
SQInteger CServerNatives::AddRule(SQVM * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		sq_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	const char* value;
	sq_getstring(pVM, -2, &rule);
	sq_getstring(pVM, -1, &value);
	sq_pushbool(pVM, g_pQuery->AddRule(rule, value));
	return 1;
}

// removeRule(name)
SQInteger CServerNatives::RemoveRule(SQVM * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		sq_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	sq_getstring(pVM, -1, &rule);
	sq_pushbool(pVM, g_pQuery->RemoveRule(rule));
	return 1;
}

// setRule(name, value)
SQInteger CServerNatives::SetRule(SQVM * pVM)
{	
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		sq_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	const char* value;
	sq_getstring(pVM, -2, &rule);
	sq_getstring(pVM, -1, &value);
	sq_pushbool(pVM, g_pQuery->SetRule(rule, value));
	return 1;
}

// getRule(name)
SQInteger CServerNatives::GetRule(SQVM * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		sq_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	sq_getstring(pVM, -1, &rule);
	QueryRule * pRule = g_pQuery->GetRule(rule);

	if(pRule)
		sq_pushstring(pVM, pRule->strValue.Get(), -1);
	else
		sq_pushbool(pVM, false);

	return 1;
}

// shutdown()
SQInteger CServerNatives::Shutdown(SQVM * pVM)
{
	g_pServer->SetActive(false);
	sq_pushbool(pVM, true);
	return 1;
}

// setServerPassword(password)
SQInteger CServerNatives::SetPassword(SQVM * pVM)
{
	const char* pass;
	sq_getstring(pVM, -1, &pass);
	g_pNetworkManager->GetNetServer()->SetPassword(pass);
	CVAR_SET_STRING("password",String(pass));
	sq_pushbool(pVM, true);
	return 1;
}

// getServerPassword()
SQInteger CServerNatives::GetPassword(SQVM * pVM)
{
	String sPass = CVAR_GET_STRING("password");
	sq_pushstring(pVM, sPass.C_String(), sPass.GetLength());
	return 1;
}

// getPlayerCount()
SQInteger CServerNatives::GetPlayerCount(SQVM * pVM)
{
	sq_pushinteger(pVM, g_pPlayerManager->GetPlayerCount());
	return 1;
}

// getPlayers()
SQInteger CServerNatives::GetPlayers(SQVM * pVM)
{
	SQInteger iCount = 0;

	sq_newtable(pVM);

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
		{
			CPlayer * pPlayer = g_pPlayerManager->GetAt(i);

			if(pPlayer)
			{
				sq_pushinteger(pVM, i);
				sq_pushstring(pVM, pPlayer->GetName(), -1);
				sq_createslot(pVM, -3);
				++iCount;
			}
		}
	}

	return 1;
}

// getPlayerSlots()
SQInteger CServerNatives::GetPlayerSlots(SQVM * pVM)
{
	sq_pushinteger(pVM, CVAR_GET_INTEGER("maxplayers"));
	return 1;
}

// getTickCount()
SQInteger CServerNatives::GetTickCount(SQVM * pVM)
{
	sq_pushinteger(pVM, SharedUtility::GetTime());
	return 1;
}

// setHostname(hostname)
SQInteger CServerNatives::SetHostName(SQVM * pVM)
{
	const char * szHostname;
	sq_getstring(pVM, -1, &szHostname);
	CVAR_SET_STRING("hostname", String(szHostname));
	sq_pushbool(pVM, true);
	return 1;
}

// getHostname()
SQInteger CServerNatives::GetHostName(SQVM * pVM)
{
	sq_pushstring(pVM, CVAR_GET_STRING("hostname"), -1);
	return 1;
}

// togglePayAndSpray(toggle)
SQInteger CServerNatives::TogglePayAndSpray(SQVM * pVM)
{
	SQBool bSwitch;
	sq_getbool(pVM, -1, &bSwitch);

	bool bToggle = (bSwitch != 0);
	CVAR_SET_BOOL("paynspray", bToggle);

	CBitStream bsSend;
	bsSend.Write(bToggle);
	g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	return 1;
}

// toggleAutoAim(toggle)
SQInteger CServerNatives::ToggleAutoAim(SQVM * pVM)
{
	SQBool bSwitch;
	sq_getbool(pVM, -1, &bSwitch);

	bool bToggle = (bSwitch != 0);
	CVAR_SET_BOOL("autoaim", bToggle);

	CBitStream bsSend;
	bsSend.Write(bToggle);
	g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	return 1;
}

// sendConsoleInput(input)
SQInteger CServerNatives::SendInput(SQVM * pVM)
{
	const char * input;
	sq_getstring(pVM, -1, &input);

	if(g_pServer)
		g_pServer->SendInput(input);

	return 1;
}

// getConfig()
SQInteger CServerNatives::GetConfig(SQVM * pVM)
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

// getWeaponName(weaponid)
SQInteger CServerNatives::GetWeaponName(SQVM * pVM)
{
	SQInteger iWeaponId;
	sq_getinteger(pVM, -1, &iWeaponId);

	switch(iWeaponId)
	{
	case 0:
		sq_pushstring(pVM, "Fists", -1);
		break;
	case 1:
		sq_pushstring(pVM, "Baseball Bat", -1);
		break;
	case 2:
		sq_pushstring(pVM, "Pool Cue", -1);
		break;
	case 3:
		sq_pushstring(pVM, "Knife", -1);
		break;
	case 4:
		sq_pushstring(pVM, "Grenade", -1);
		break;
	case 5:
		sq_pushstring(pVM, "Molotov Cocktail", -1);
		break;
	case 7:
		sq_pushstring(pVM, "Pistol", -1);
		break;
	case 9:
		sq_pushstring(pVM, "Desert Eagle", -1);
		break;
	case 10:
		sq_pushstring(pVM, "Shotgun", -1);
		break;
	case 11:
		sq_pushstring(pVM, "Baretta", -1);
		break;
	case 12:
		sq_pushstring(pVM, "Micro UZI", -1);
		break;
	case 13:
		sq_pushstring(pVM, "MP5", -1);
		break;
	case 14:
		sq_pushstring(pVM, "AK-47", -1);
		break;
	case 15:
		sq_pushstring(pVM, "M4", -1);
		break;
	case 16:
		sq_pushstring(pVM, "Sniper Rifle", -1);
		break;
	case 17:
		sq_pushstring(pVM, "M40-A1", -1);
		break;
	case 18:
		sq_pushstring(pVM, "Rocket Launcher", -1);
		break;
	case 19:
		sq_pushstring(pVM, "Flame Thrower", -1);
		break;
	case 20:
		sq_pushstring(pVM, "Minigun", -1);
		break;
	default:
		sq_pushbool(pVM, false);
		break;
	}
	
	return 1;
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
SQInteger CServerNatives::GetVehicleName(SQVM * pVM)
{
	SQInteger iModelId;
	sq_getinteger(pVM, -1, &iModelId);

	if(iModelId < 0 || iModelId > 123)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushstring(pVM, szVehicleNames[iModelId], -1);
	return 1;
}

// toggleFrequentEvents(toggle)
SQInteger CServerNatives::ToggleFrequentEvents(SQVM * pVM)
{
	bool bToggle;
	sq_getinteger(pVM, -1, (SQInteger*)&bToggle);
	CVAR_SET_BOOL("frequentevents", bToggle);
	sq_pushbool(pVM, true);
	return 1;
}

// areFrequentEventsEnabled()
SQInteger CServerNatives::AreFrequentEventsEnabled(SQVM * pVM)
{
	sq_pushbool(pVM, CVAR_GET_BOOL("frequentevents"));
	return 1;
}

// setWeather(weather)
SQInteger CServerNatives::SetWeather(SQVM * pVM)
{
	SQInteger iWeather;
	sq_getinteger(pVM, 2, &iWeather);
	if(iWeather >= 1 && iWeather <= 10 && iWeather != CVAR_GET_INTEGER("weather"))
	{
		CVAR_SET_INTEGER("weather", (int)iWeather);

		CBitStream bsSend;
		bsSend.Write((unsigned char)iWeather);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getWeather(weather)
SQInteger CServerNatives::GetWeather(SQVM * pVM)
{
	sq_pushinteger(pVM, CVAR_GET_INTEGER("weather"));
	return 1;
}

// setWeather(weather)
SQInteger CServerNatives::ForceWind(SQVM * pVM)
{
	SQFloat fWind;
	sq_getfloat(pVM, 2, &fWind);
	if(fWind != CVAR_GET_INTEGER("wind"))
	{
		CVAR_SET_FLOAT("wind", (float)fWind);

		CBitStream bsSend;
		bsSend.Write((float)fWind);
		g_pNetworkManager->RPC(RPC_ScriptingForceWind, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
