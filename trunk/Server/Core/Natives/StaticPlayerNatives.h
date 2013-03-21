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
// Do not use any RPC send code in this namespace (use only managers -> clean code -> safe to change code)
// CrackHD: planning to implement StaticNatives namespace to Module SDK, if it will be possible
// And we will not need anything except this:

namespace StaticNatives
{
	// Set/Get
	bool SetPlayerCoordinates(EntityId playerId, CVector3 vecPos);
	bool GetPlayerCoordinates(EntityId playerId, CVector3& vecPos);
	bool SetPlayerName(EntityId playerId, String strPlayerName);
	bool GetPlayerName(EntityId playerId, String& strPlayerName);
	bool SetPlayerWantedLevel(EntityId playerId, unsigned int iWantedLevel);
	bool GetPlayerWantedLevel(EntityId playerId, unsigned int& iWantedLevel);
	bool SetPlayerHealth(EntityId playerId, int iHealth);
	bool GetPlayerHealth(EntityId playerId, int& iHealth);
	bool SetPlayerArmour(EntityId playerId, unsigned int uiArmour);
	bool GetPlayerArmour(EntityId playerId, unsigned int& uiArmour);
	bool SetPlayerGravity(EntityId playerId, float fGravity);
	bool SetPlayerSpawnLocation(EntityId playerId, CVector3 vecCoords, float fHeading);
	bool GetPlayerSpawnLocation(EntityId playerId, CVector3& vecCoords, float& fHeading);
	bool SetPlayerModel(EntityId playerId, unsigned short usModelId);
	bool GetPlayerModel(EntityId playerId, unsigned short& usModelId);
	bool SetPlayerHeading(EntityId playerId, float fHeading);
	bool GetPlayerHeading(EntityId playerId, float& fHeading);
	bool SetPlayerVelocity(EntityId playerId, CVector3 vecSpeed);
	bool GetPlayerVelocity(EntityId playerId, CVector3& vecSpeed);
	bool SetPlayerInvincible(EntityId playerId, bool bInvincible);
	bool SetPlayerDucking(EntityId playerId, bool bDucking);
	bool IsPlayerDucking(EntityId playerId);
	bool SetPlayerColor(EntityId playerId, unsigned int uiColor);
	bool GetPlayerColor(EntityId playerId, unsigned int& uiColor);
	// TODO: clothes set/get
	bool SetPlayerDimension(EntityId playerId, DimensionId dimensionId);
	bool GetPlayerDimension(EntityId playerId, DimensionId& dimensionId);
	
	// Weapons stuff
	bool GivePlayerWeapon(EntityId playerId, int iWeaponModelId, int iAmmo);
	bool RemovePlayerWeapons(EntityId playerId);
	bool GetPlayerWeapon(EntityId playerId, unsigned int& iWeaponModelId);
	bool GetPlayerAmmo(EntityId playerId, unsigned int& iWeaponAmmo);

	// Money stuff
	bool GivePlayerMoney(EntityId playerId, int iMoneyAdd);
	bool SetPlayerMoney(EntityId playerId, int iMoney);
	bool GetPlayerMoney(EntityId playerId, int& iMoney);
	bool ResetPlayerMoney(EntityId playerId);
	
	// Toggles
	bool TogglePlayerPayAndSpray(EntityId playerId, bool bToggle);
	bool TogglePlayerAutoAim(EntityId playerId, bool bToggle);
	bool TogglePlayerDrunk(EntityId playerId, bool bToggle);
	bool TogglePlayerControls(EntityId playerId, bool bControlsEnabled);
	bool TogglePlayerFrozen(EntityId playerId, bool bFrozen, bool bCameraFrozen);
	bool TogglePlayerPhysics(EntityId playerId, bool bEnabled);
	bool TogglePlayerHelmet(EntityId playerId, bool bHelmet);
	bool TogglePlayerHUD(EntityId playerId, bool bVisible);
	bool TogglePlayerRadar(EntityId playerId, bool bVisible);
	bool TogglePlayerNameTags(EntityId playerId, bool bDraw);
	bool TogglePlayerAreaNames(EntityId playerId, bool bShowAreaNames);

	// Camera
	bool SetPlayerCameraBehind(EntityId playerId);
	bool SetPlayerCameraPos(EntityId playerId, CVector3 vecPos);
	bool SetPlayerCameraLookAt(EntityId playerId, CVector3 vecLookAt);
	bool ResetPlayerCamera(EntityId playerId);

	// Screen text
	bool DisplayPlayerText(EntityId playerId, float fX, float fY, String strText, unsigned int uiTime);
	bool DisplayTextForAll(float fX, float fY, String strText, unsigned int uiTime);
	bool DisplayPlayerInfoText(EntityId playerId, String strText, unsigned int uiTime);
	bool DisplayInfoTextForAll(String strText, unsigned int uiTime);

	// Other natives
	bool SetPlayerTime(EntityId playerId, BYTE byteHour, BYTE byteMinute);
	bool SetPlayerWeather(EntityId playerId, BYTE ucWeatherId);
	bool SendPlayerMessage(EntityId playerId, String strMessage, DWORD dwColor, bool bAllowFormatting);
	bool SendMessageToAll(String strMessage, DWORD dwColor, bool bAllowFormatting);
	bool IsPlayerConnected(EntityId playerId);
	bool GetPlayerSerial(EntityId playerId, String& strSerial);
	bool IsPlayerInAnyVehicle(EntityId playerId);
	bool IsPlayerInVehicle(EntityId playerId, EntityId vehicleId);
	bool GetPlayerVehicleId(EntityId playerId, EntityId& vehicleId);
	bool GetPlayerSeatId(EntityId playerId, BYTE& byteSeat);
	bool IsPlayerOnFoot(EntityId playerId);
	bool IsPlayerSpawned(EntityId playerId);
	bool KickPlayer(EntityId playerId, bool bSendKickMessage);
	bool BanPlayer(EntityId playerId, unsigned int uiSeconds);
	bool GetPlayerIp(EntityId playerId, String& strIp);
	bool GetPlayerState(EntityId playerId, eStateType& state);
	bool WarpPlayerIntoVehicle(EntityId playerId, EntityId vehicleId, BYTE seatId = 0);
	bool RemovePlayerFromVehicle(EntityId playerId, bool bAnimated);
	bool TriggerPlayerClientEvent(EntityId playerId, CSquirrelArguments sqArgs);
	bool GetPlayerPing(EntityId playerId, unsigned short& usPing);
}
