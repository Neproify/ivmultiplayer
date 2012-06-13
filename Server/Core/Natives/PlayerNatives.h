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

#include "../Natives.h"

class CPlayerNatives
{
private:
	static SQInteger IsConnected(SQVM * pVM);
	static SQInteger GetName(SQVM * pVM);
	static SQInteger SetName(SQVM * pVM);
	static SQInteger SetHealth(SQVM * pVM);
	static SQInteger GetHealth(SQVM * pVM);
	static SQInteger SetArmour(SQVM * pVM);
	static SQInteger GetArmour(SQVM * pVM);
	static SQInteger SetCoordinates(SQVM * pVM);
	static SQInteger GetCoordinates(SQVM * pVM);
	static SQInteger SetTime(SQVM * pVM);
	static SQInteger SetWeather(SQVM * pVM);
	static SQInteger SetGravity(SQVM * pVM);
	static SQInteger SendMessage(SQVM * pVM);
	static SQInteger SendMessageToAll(SQVM * pVM);
	static SQInteger IsInAnyVehicle(SQVM * pVM);
	static SQInteger IsInVehicle(SQVM * pVM);
	static SQInteger GetVehicleId(SQVM * pVM);
	static SQInteger GetSeatId(SQVM * pVM);
	static SQInteger IsOnFoot(SQVM * pVM);
	static SQInteger TogglePayAndSpray(SQVM * pVM);
	static SQInteger ToggleAutoAim(SQVM * pVM);
	//static SQInteger SetPlayerDrunk(SQVM * pVM);
	static SQInteger GiveWeapon(SQVM * pVM);
	static SQInteger RemoveWeapons(SQVM * pVM);
	static SQInteger SetSpawnLocation(SQVM * pVM);
	static SQInteger SetModel(SQVM * pVM);
	static SQInteger GetModel(SQVM * pVM);
	static SQInteger ToggleControls(SQVM * pVM);
	static SQInteger IsSpawned(SQVM * pVM);
	static SQInteger SetHeading(SQVM * pVM);
	static SQInteger GetHeading(SQVM * pVM);
	static SQInteger TogglePhysics(SQVM * pVM);
	static SQInteger Kick(SQVM * pVM);
	static SQInteger Ban(SQVM * pVM);
	static SQInteger GetIp(SQVM * pVM);
	static SQInteger GiveMoney(SQVM * pVM);
	static SQInteger SetMoney(SQVM * pVM);
	static SQInteger ResetMoney(SQVM * pVM);
	static SQInteger GetMoney(SQVM * pVM);
	static SQInteger DisplayText(SQVM * pVM);
	static SQInteger DisplayTextForAll(SQVM * pVM);
	static SQInteger DisplayInfoText(SQVM * pVM);
	static SQInteger DisplayInfoTextForAll(SQVM * pVM);
	static SQInteger ToggleFrozen(SQVM * pVM);
	static SQInteger GetState(SQVM * pVM);
	static SQInteger SetVelocity(SQVM * pVM);
	static SQInteger GetVelocity(SQVM * pVM);
	static SQInteger SetWantedLevel(SQVM * pVM);
	static SQInteger WarpIntoVehicle(SQVM * pVM);
	static SQInteger RemoveFromVehicle(SQVM * pVM);
	static SQInteger GetWeapon(SQVM * pVM);
	static SQInteger GetAmmo(SQVM * pVM);
	static SQInteger GetSerial(SQVM * pVM);
	static SQInteger SetCameraBehind(SQVM * pVM);
	static SQInteger SetDucking(SQVM * pVM);
	static SQInteger IsDucking(SQVM * pVM);
	static SQInteger SetInvincible(SQVM * pVM);
	static SQInteger ToggleHUD(SQVM * pVM);
	static SQInteger ToggleRadar(SQVM * pVM);
	static SQInteger ToggleNames(SQVM * pVM);
	static SQInteger ToggleAreaNames(SQVM * pVM);
	static SQInteger GetEmptyControlState(SQVM * pVM);
	static SQInteger GetPreviousControlState(SQVM * pVM);
	static SQInteger GetControlState(SQVM * pVM);
	static SQInteger TriggerEvent(SQVM * pVM);
	static SQInteger GetColor(SQVM * pVM);
	static SQInteger SetColor(SQVM * pVM);
	static SQInteger GetPing(SQVM * pVM);
	static SQInteger SetClothes(SQVM * pVM);
	static SQInteger GetClothes(SQVM * pVM);
	static SQInteger ResetClothes(SQVM * pVM);
	static SQInteger Respawn(SQVM * pVM);
	static SQInteger GiveHelmet(SQVM * pVM);
	static SQInteger RemoveHelmet(SQVM * pVM);
	static SQInteger ToggleHelmet(SQVM * pVM);
	static SQInteger SetCameraPos(SQVM * pVM);
	static SQInteger SetCameraLookAt(SQVM * pVM);
	static SQInteger ResetCamera(SQVM * pVM);
	static SQInteger forceAnim(SQVM * pVM);
	static SQInteger requestAnim(SQVM * pVM);
	static SQInteger triggerAudioEvent(SQVM * pVM);
	static SQInteger triggerMissionCompleteAudio(SQVM * pVM);
	static SQInteger triggerPoliceReport(SQVM * pVM);
	static SQInteger fadeScreenIn(SQVM * pVM);
	static SQInteger fadeScreenOut(SQVM * pVM);
	static SQInteger blockWeaponChange(SQVM * pVM);
	static SQInteger blockWeaponDrop(SQVM * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
