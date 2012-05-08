//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.h
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
	class CPlayerModuleNatives : public ICPlayerModuleNatives
	{
	public:
		bool IsConnected(EntityId playerId);
		const char * GetName(EntityId playerId);
		bool SetName(EntityId playerId, const char * szName);
		bool GiveWeapon(EntityId playerId, int iWeaponId, int iAmmo);
		bool RemoveWeapons(EntityId playerId);
		bool SetWantedLevel(EntityId playerId, int iWantedLevel);
		bool SetHealth(EntityId playerId, int iHealth);
		int GetHealth(EntityId playerId);
		bool SetArmour(EntityId playerId, int iArmour);
		int GetArmour(EntityId playerId);
		bool SetCoordinates(EntityId playerId, CVector3 vecPos);
		CVector3 GetCoordinates(EntityId playerId);
		bool SetTime(EntityId playerId, unsigned char iHour, unsigned char iMinute);
		bool SetWeather(EntityId playerId, unsigned char iWeather);
		bool SetGravity(EntityId playerId, float fGravity);
		bool SendMessage(EntityId playerId, const char * szMessage, int iColor, bool sqbAllowFormatting);
		bool SendMessageToAll(const char * szMessage, int iColor, bool sqbAllowFormatting);
		bool IsInAnyVehicle(EntityId playerId);
		bool IsInVehicle(EntityId playerId, EntityId vehicleId);
		EntityId GetVehicleId(EntityId playerId);
		int GetSeatId(EntityId playerId);
		bool IsOnFoot(EntityId playerId);
		bool TogglePayAndSpray(EntityId playerId, bool sqbToggle);
		bool ToggleAutoAim(EntityId playerId, bool sqbToggle);
		bool SetSpawnLocation(EntityId playerId, CVector3 vecPos, float fRotation);
		bool SetModel(EntityId playerId, int iModelId);
		int GetModel(EntityId playerId);
		bool ToggleControls(EntityId playerId, bool sqbControls);
		bool IsSpawned(EntityId playerId);
		bool SetHeading(EntityId playerId, float fHeading);
		float GetHeading(EntityId playerId);
		bool TogglePhysics(EntityId playerId, bool sqbToggle);
		bool Kick(EntityId playerId, bool sqbKickMessage);
		bool Ban(EntityId playerId, unsigned int uiSeconds);
		const char * GetIp(EntityId playerId);
		bool GiveMoney(EntityId playerId, int iMoney);
		bool SetMoney(EntityId playerId, int iMoney);
		bool ResetMoney(EntityId playerId);
		int GetMoney(EntityId playerId);
		bool ToggleFrozen(EntityId playerId, bool sqbPlayerFrozen, bool sqbCameraFrozen);
		int GetState(EntityId playerId);
		bool DisplayText(EntityId playerId, float fPosX, float fPosY, const char * szText, int iTime);
		bool DisplayTextForAll(float fPosX, float fPosY, const char * szText, int iTime);
		bool DisplayInfoText(EntityId playerId, const char * szText, int iTime);
		bool DisplayInfoTextForAll(const char * szText, int iTime);
		bool SetVelocity(EntityId playerId, CVector3 vecMoveSpeed);
		CVector3 GetVelocity(EntityId playerId);
		bool WarpIntoVehicle(EntityId playerId, EntityId vehicleId, int iSeatId);
		bool RemoveFromVehicle(EntityId playerId, bool bGraceful);
		int GetWeapon(EntityId playerId);
		int GetAmmo(EntityId playerId);
		const char * GetSerial(EntityId playerId);
		bool SetCameraBehind(EntityId playerId);
		bool SetDucking(EntityId playerId, bool sqbDucking);
		bool IsDucking(EntityId playerId);
		bool SetInvincible(EntityId playerId, bool sqbInvincible);
		bool ToggleHUD(EntityId playerId, bool sqbToggle);
		bool ToggleRadar(EntityId playerId, bool sqbToggle);
		bool ToggleNames(EntityId playerId, bool sqbToggle);
		bool ToggleAreaNames(EntityId playerId, bool sqbToggle);
		bool SetColor(EntityId playerId, int color);
		int GetColor(EntityId playerId);
		int GetPing(EntityId);
		bool SetClothes(EntityId iPlayerId, int iBodyPart, int iClothes);
		int * GetClothes(EntityId playerId);
		bool ResetClothes(EntityId playerId);
		bool Respawn(EntityId playerId);
		bool GiveHelmet(EntityId playerId);
		bool RemoveHelmet(EntityId playerId);
		bool ToggleHelmet(EntityId playerId, bool sqbToggle);
		bool SetCameraPos(EntityId playerId, CVector3 vecPos);
		bool SetCameraLookAt(EntityId playerId, CVector3 vecPos);
		bool ResetCamera(EntityId playerId);
	};
}