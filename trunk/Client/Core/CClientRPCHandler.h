//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientRPCHandler.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Network/CRPCHandler.h>

class CClientRPCHandler : public CRPCHandler
{
private:
	// Network
	// FIXUPDATE
	// TODO: NewEntity and DeleteEntity RPC's with CEntity client and server
	// class that has abstract SpawnForWorld e.t.c. to call New/DeleteEntity
	// TODO: SetEntityPosition/Rotation e.t.c. as part of CEntity with abstracted
	// funcs and stuff?
	static void JoinedGame(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeletePlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PlayerSpawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PlayerDeath(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Chat(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void InVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PassengerSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void SmallSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Message(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ConnectionRefused(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NameChange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewPickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeletePickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

	// Scripting
	static void ScriptingSetPlayerHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerTime(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerWeather(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingTogglePayAndSpray(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleAutoAim(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	//static void SetPlayerDrunk(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGivePlayerWeapon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetSpawnLocation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleIndicators(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSoundVehicleHorn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGiveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemoveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetModel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleControls(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleDirtLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleSirenState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleEngineState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleRagdoll(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGivePlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDisplayText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDisplayInfoText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleFrozen(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleEngineHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerArmour(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleTurnSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemoveWeapons(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetWantedLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingWarpPlayerIntoVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemovePlayerFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetCameraBehindPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerDucking(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerInvincible(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingActorWalkToCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipSize(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleBlipShortRange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleBlipRoute(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipName(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingShowCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingHideCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleHUD(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleRadar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleAreaNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingEventCall(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleLocked(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingResetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetTrafficLightState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleComponents(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleVariation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetObjectPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetObjectRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupValue(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCameraPos(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCameraLookAt(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingResetPlayerCamera(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

public:
	void        Register();
	void        Unregister();
};
