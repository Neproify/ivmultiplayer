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

#include "IModuleNatives.h"

namespace Modules
{
	class ICPlayerModuleNatives
	{
	public:
		virtual bool IsConnected(EntityId playerId) = 0;
		virtual const char * GetName(EntityId playerId) = 0;
		virtual bool SetName(EntityId playerId, const char * szName) = 0;
		virtual bool GiveWeapon(EntityId playerId, int iWeaponId, int iAmmo) = 0;
		virtual bool RemoveWeapons(EntityId playerId) = 0;
		virtual bool SetWantedLevel(EntityId playerId, int iWantedLevel) = 0;
		virtual bool SetHealth(EntityId playerId, int iHealth) = 0;
		virtual int GetHealth(EntityId playerId) = 0;
		virtual bool SetArmour(EntityId playerId, int iArmour) = 0;
		virtual int GetArmour(EntityId playerId) = 0;
		virtual bool SetCoordinates(EntityId playerId, CVector3 vecPos) = 0;
		virtual CVector3 GetCoordinates(EntityId playerId) = 0;
		virtual bool SetTime(EntityId playerId, unsigned char iHour, unsigned char iMinute) = 0;
		virtual bool SetWeather(EntityId playerId, unsigned char iWeather) = 0;
		virtual bool SetGravity(EntityId playerId, float fGravity) = 0;
		virtual bool SendMessage(EntityId playerId, const char * szMessage, int iColor, bool sqbAllowFormatting) = 0;
		virtual bool SendMessageToAll(const char * szMessage, int iColor, bool sqbAllowFormatting) = 0;
		virtual bool IsInAnyVehicle(EntityId playerId) = 0;
		virtual bool IsInVehicle(EntityId playerId, EntityId vehicleId) = 0;
		virtual EntityId GetVehicleId(EntityId playerId) = 0;
		virtual int GetSeatId(EntityId playerId) = 0;
		virtual bool IsOnFoot(EntityId playerId) = 0;
		virtual bool TogglePayAndSpray(EntityId playerId, bool sqbToggle) = 0;
		virtual bool ToggleAutoAim(EntityId playerId, bool sqbToggle) = 0;
		virtual bool SetSpawnLocation(EntityId playerId, CVector3 vecPos, float fRotation) = 0;
		virtual bool SetModel(EntityId playerId, int iModelId) = 0;
		virtual int GetModel(EntityId playerId) = 0;
		virtual bool ToggleControls(EntityId playerId, bool sqbControls) = 0;
		virtual bool IsSpawned(EntityId playerId) = 0;
		virtual bool SetHeading(EntityId playerId, float fHeading) = 0;
		virtual float GetHeading(EntityId playerId) = 0;
		virtual bool TogglePhysics(EntityId playerId, bool sqbToggle) = 0;
		virtual bool Kick(EntityId playerId, bool sqbKickMessage) = 0;
		virtual bool Ban(EntityId playerId, unsigned int uiSeconds) = 0;
		virtual const char * GetIp(EntityId playerId) = 0;
		virtual bool GiveMoney(EntityId playerId, int iMoney) = 0;
		virtual bool SetMoney(EntityId playerId, int iMoney) = 0;
		virtual bool ResetMoney(EntityId playerId) = 0;
		virtual int GetMoney(EntityId playerId) = 0;
		virtual bool ToggleFrozen(EntityId playerId, bool sqbPlayerFrozen, bool sqbCameraFrozen) = 0;
		virtual int GetState(EntityId playerId) = 0;
		virtual bool DisplayText(EntityId playerId, float fPosX, float fPosY, const char * szText, int iTime) = 0;
		virtual bool DisplayTextForAll(float fPosX, float fPosY, const char * szText, int iTime) = 0;
		virtual bool DisplayInfoText(EntityId playerId, const char * szText, int iTime) = 0;
		virtual bool DisplayInfoTextForAll(const char * szText, int iTime) = 0;
		virtual bool SetVelocity(EntityId playerId, CVector3 vecMoveSpeed) = 0;
		virtual CVector3 GetVelocity(EntityId playerId) = 0;
		virtual bool WarpIntoVehicle(EntityId playerId, EntityId vehicleId, int iSeatId) = 0;
		virtual bool RemoveFromVehicle(EntityId playerId, bool bGraceful) = 0;
		virtual int GetWeapon(EntityId playerId) = 0;
		virtual int GetAmmo(EntityId playerId) = 0;
		virtual const char * GetSerial(EntityId playerId) = 0;
		virtual bool SetCameraBehind(EntityId playerId) = 0;
		virtual bool SetDucking(EntityId playerId, bool sqbDucking) = 0;
		virtual bool IsDucking(EntityId playerId) = 0;
		virtual bool SetInvincible(EntityId playerId, bool sqbInvincible) = 0;
		virtual bool ToggleHUD(EntityId playerId, bool sqbToggle) = 0;
		virtual bool ToggleRadar(EntityId playerId, bool sqbToggle) = 0;
		virtual bool ToggleNames(EntityId playerId, bool sqbToggle) = 0;
		virtual bool ToggleAreaNames(EntityId playerId, bool sqbToggle) = 0;
		virtual bool SetColor(EntityId playerId, int color) = 0;
		virtual int GetColor(EntityId playerId) = 0;
		virtual int GetPing(EntityId) = 0;
		virtual bool SetClothes(EntityId iPlayerId, int iBodyPart, int iClothes) = 0;
		virtual int * GetClothes(EntityId playerId) = 0;
		virtual bool ResetClothes(EntityId playerId) = 0;
		virtual bool Respawn(EntityId playerId) = 0;
		virtual bool GiveHelmet(EntityId playerId) = 0;
		virtual bool RemoveHelmet(EntityId playerId) = 0;
		virtual bool ToggleHelmet(EntityId playerId, bool sqbToggle) = 0;
		virtual bool SetCameraPos(EntityId playerId, CVector3 vecPos) = 0;
		virtual bool SetCameraLookAt(EntityId playerId, CVector3 vecPos) = 0;
		virtual bool ResetCamera(EntityId playerId) = 0;
		virtual CControlState GetEmptyControlState() = 0;
		virtual CControlState GetPreviousControlState(EntityId playerId) = 0;
		virtual CControlState GetControlState(EntityId playerId) = 0;
		virtual bool TriggerEvent(EntityId playerid, const char * szEventName, const char * szFormat, ...) = 0;
		virtual bool forceAnim(EntityId playerId, const char * szGroup, const char * szAnim) = 0;
		virtual bool requestAnim(EntityId playerId) = 0;
		virtual bool triggerAudioEvent(EntityId playerId, const char * szAudio) = 0;
		virtual bool triggerMissionCompleteAudio(EntityId playerId, int iMission) = 0;
		virtual bool triggerPoliceReport(EntityId playerId, const char * szAudio) = 0;
		virtual bool fadeScreenIn(EntityId playerId, int iDuration) = 0;
		virtual bool fadeScreenOut(EntityId playerId, int iDuration) = 0;
		virtual bool blockWeaponChange(EntityId playerId, bool btoggle) = 0;
		virtual bool blockWeaponDrop(EntityId playerId, bool btoggle) = 0;
	};
}