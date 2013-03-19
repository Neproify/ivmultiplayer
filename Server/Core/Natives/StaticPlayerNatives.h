//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: StaticPlayerNatives.h
// Project: Server.Core
// Author(s):	CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include "../CPlayerManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CNetworkManager.h"
#include <Game/CTime.h>
#include "CEvents.h"
#include "../../Shared/CLogFile.h"

extern CPlayerManager * g_pPlayerManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;

// CrackHD: Moving code to "StaticNatives" namespace opens to us a way for clean and free code in future,
// Do not use here anything connected with Squirrel, because in future we will be able to implement other scripting languages (if not, just makes code cleaner)
// Do not use any RPC send code in this namespace
// And we will not need anything except this:

namespace StaticNatives
{
	// Position, heading, model etc...
	bool SetPlayerCoordinates(EntityId playerId, CVector3 vecPos);
	bool GetPlayerCoordinates(EntityId playerId, CVector3& vecPos);
	
	// Name stuff
	bool SetPlayerName(EntityId playerId, String strPlayerName);
	bool GetPlayerName(EntityId playerId, String& strPlayerName);

	// Wanted level stuff
	bool SetPlayerWantedLevel(EntityId playerId, unsigned int iWantedLevel);
	bool GetPlayerWantedLevel(EntityId playerId, unsigned int& iWantedLevel);
	
	// Weapons stuff
	bool GivePlayerWeapon(EntityId playerId, int iWeaponModelId, int iAmmo);
	bool RemovePlayerWeapons(EntityId playerId);
	bool GetPlayerWeapon(EntityId playerId, unsigned int& iWeaponModelId);
	bool GetPlayerAmmo(EntityId playerId, unsigned int& iWeaponAmmo);
	
	// Other natives
	bool SetPlayerTime(EntityId playerId, BYTE ucHour, BYTE ucMinute);
	bool SetPlayerWeather(EntityId playerId, BYTE ucWeatherId);
	bool SetPlayerGravity(EntityId playerId, float fGravity);
	bool SendPlayerMessage(EntityId playerId, String strMessage, DWORD dwColor, bool bAllowFormatting);
	bool IsPlayerConnected(EntityId playerId);
	bool GetPlayerSerial(EntityId playerId, String& strSerial);
}