//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Scripting.h
// Project: Client.Core
// Author(s): aru
//            jenksta
//            Einstein
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==============================================================================

#pragma once

#include "NativeInvoke.h"
#include "ScriptingEnums.h"
#include "NativesList.h"

namespace Scripting
{
	// Game stuff
	static void SetRenderTrainAsDerailed(unsigned int p0, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_RENDER_TRAIN_AS_DERAILED, p0, p1); }
	static void SetRocketLauncherFreebieInHeli(unsigned int p0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ROCKET_LAUNCHER_FREEBIE_IN_HELI, p0); }
	static void SetZonePopulationType(const char *szZone, int iType) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ZONE_POPULATION_TYPE, szZone, iType); }
	static void SetZoneScumminess(const char *szZone, int iType) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ZONE_SCUMMINESS, szZone, iType); }

	// Controls
	static bool IsControlJustPressed(unsigned int p0, unsigned int p1) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CONTROL_JUST_PRESSED, p0, p1); }
	static bool IsControlPressed(unsigned int p0, unsigned int p1) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CONTROL_PRESSED, p0, p1); }
	static int GetControlValue(unsigned int p0, unsigned int p1) { return NativeInvoke::Invoke<int>(NATIVE_GET_CONTROL_VALUE, p0, p1); }
	static void GetPadState(unsigned int p0, unsigned int p1, int * p2) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PAD_STATE, p0, p1, p2); }

	// Handy
	static void SetMobileRadioEnabledDuringGameplay(bool bToggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLA, bToggle); }
	static void SetMobilePhoneRadioState(unsigned int p0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MOBILE_PHONE_RADIO_STATE, p0); }

	// Models
	static void RequestModel(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_MODEL, model); }

	// In game text
	static void SetTextColour(unsigned int* red, unsigned int* green, unsigned int* blue, unsigned int* a) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_COLOUR, red, green, blue, a); }
	static void SetTextFont(const char *font) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_FONT, font); }

	// Script
	static void SetMultiplayerHUDCash(int cash) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MULTIPLAYER_HUD_CASH, cash); }
	static void SetMsgForLoadingScreen(const char *msg) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MSG_FOR_LOADING_SCREEN, msg); }
	static void FlushAllOutOfDateRadarBlipsFromMissionCleanupList() { NativeInvoke::Invoke<unsigned int>(NATIVE_FLUSH_ALL_OUT_OF_DATE_RADAR_BLIPS_FROM_MISSION_CLEANUP_LIST); }
	static void TerminateAllScriptsWithThisName(const char* name) { NativeInvoke::Invoke<unsigned int>(NATIVE_TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME, name); } // ???
	static void DisableCarGenerators(bool toggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_CAR_GENERATORS, toggle); }
	static void RequestScript(const char *scriptName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_SCRIPT, scriptName); }
	static void StartNewScript(const char *scriptName, int size) { NativeInvoke::Invoke<unsigned int>(NATIVE_START_NEW_SCRIPT, scriptName, size); }
	static void SetThisScriptCanRemoveBlipsCreatedByAnyScript(bool toggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_THIS_SCRIPT_CAN_REMOVE_BLIPS_CREATED_BY_ANY_SCRIPT, toggle); }
	static void SetNoResprays(bool toggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_NO_RESPRAYS, toggle); }
	static void SetMaxWantedLevel(int max) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MAX_WANTED_LEVEL, max); }
	static void AllowThisScriptToBePaused(bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_THIS_SCRIPT_TO_BE_PAUSED, allow); }
	static void SetCreateRandomCops(bool set) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CREATE_RANDOM_COPS, set); }
	static void EnableSaveHouse(int house, bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_SAVE_HOUSE, house, enable); }
	static void DontSuppressAnyPedModels() { NativeInvoke::Invoke<unsigned int>(NATIVE_DONT_SUPPRESS_ANY_PED_MODELS); }
	static void DeleteAllTrains() { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_ALL_TRAINS); }
	static void SetCarDensityMultiplier(unsigned int* density) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_DENSITY_MULTIPLIER, density); }
	static void SetPedDensityMultiplier(unsigned int* density) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_DENSITY_MULTIPLIER, density); }
	static void SetRandomCarDensityMultiplier(unsigned int* density) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_RANDOM_CAR_DENSITY_MULTIPLIER, density); }
	static void SetScenarioPedDensityMultiplier(unsigned int* scenario, unsigned int*density) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_SCENARIO_PED_DENSITY_MULTIPLIER, scenario, density); }
	static void SetParkedCarDensityMultiplier(unsigned int* density) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PARKED_CAR_DENSITY_MULTIPLIER, density); }
	static void AddHospitalRestart(float x, float y, float z, float a, int id) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_HOSPITAL_RESTART, x, y, z, a, id); }
	static void NetworkExpandTo32Players() { NativeInvoke::Invoke<unsigned int>(NATIVE_NETWORK_EXPAND_TO_32_PLAYERS); }
	static void ActivateNetworkMenu() { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_NETWORK_SETTINGS_MENU); }
	static bool LocalPlayerIsReadyToStartPlaying() { return NativeInvoke::Invoke<bool>(NATIVE_LOCAL_PLAYER_IS_READY_TO_START_PLAYING); }
	static void LaunchLocalPlayerInNetworkGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_LAUNCH_LOCAL_PLAYER_IN_NETWORK_GAME); }
	static void ShutdownAndLaunchSinglePlayerGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME); }
	static void SuppressFadeInAfterDeathArrest(bool suppress) { NativeInvoke::Invoke<unsigned int>(NATIVE_SUPPRESS_FADE_IN_AFTER_DEATH_ARREST, suppress); }
	static void DontSuppressCarModel(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_DONT_SUPPRESS_CAR_MODEL, model); }
	static void DontSuppressPedModel(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_DONT_SUPPRESS_PED_MODEL, model); }
	static void SuppressCarModel(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_SUPPRESS_CAR_MODEL, model); }
	static void SuppressPedModel(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_SUPPRESS_PED_MODEL, model); }
	static void ThisScriptShouldBeSaved() { NativeInvoke::Invoke<unsigned int>(NATIVE_THIS_SCRIPT_SHOULD_BE_SAVED); }
	static void ThisScriptIsSafeForNetworkGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME); }
	static void DestroyAllCams() { NativeInvoke::Invoke<unsigned int>(NATIVE_DESTROY_ALL_CAMS); }
	static void ClearHelp() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_HELP); }
	static void ClearPrints() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_PRINTS); }
	static void NetworkStoreSinglePlayerGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_NETWORK_STORE_SINGLE_PLAYER_GAME); }
	static void SetPlayerControlForNetwork(unsigned int playerIndex, bool control, bool unk_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CONTROL_FOR_NETWORK, playerIndex, control, unk_0); }
	static void SetCharCollision(unsigned int ped, bool collision) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_COLLISION, ped, collision); }
	static void SetCharNeverTargetted(unsigned int ped, bool nevertargetted) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_NEVER_TARGETTED, ped, nevertargetted); }
	static void SetFreeResprays(bool free) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_FREE_RESPRAYS, free); }
	static void SetInMPTutorial(bool intut) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_IN_MP_TUTORIAL, intut); } // If current fails i could try this to see if it disables loading screen
	static void ResurrectNetworkPlayer(unsigned int playerIndex, float x, float y, float z, float angle) { NativeInvoke::Invoke<unsigned int>(NATIVE_RESURRECT_NETWORK_PLAYER, playerIndex, x, y, z, angle); }
	static const char* GetEpisodeName(unsigned int episodeId) { return NativeInvoke::Invoke<const char*>(NATIVE_GET_EPISODE_NAME, episodeId); }
	static bool IsNetworkGameRunning() { return NativeInvoke::Invoke<bool>(NATIVE_IS_NETWORK_GAME_RUNNING); }
	static void SetPedNonCreationArea(float x1, float y1, float z1, float x2, float y2, float z2) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_NON_CREATION_AREA, x1, y1, z1, x2, y2, z2); }
	static void SetSirenWithNoDriver(unsigned int p0, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_SIREN_WITH_NO_DRIVER, p0, p1); }
	static void SetCharAnimCurrentTime(unsigned int ped, const char *animGroup, const char *animName, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_ANIM_CURRENT_TIME, ped, animGroup, animName, value); }
	static void SetPedDontDoEvasiveDives(unsigned int p0, bool p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_DONT_DO_EVASIVE_DIVES, p0, p1); }
	static void SetPedDontUseVehicleSpecificAnims(unsigned int p0, bool p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_DONT_USE_VEHICLE_SPECIFIC_ANIMS, p0, p1); }
	static void SetPlayerKeepsWeaponsWhenRespawned(unsigned int p0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_KEEPS_WEAPONS_WHEN_RESPAWNED, p0); }
	static void DisplayHudNotification(int icon, const char * name, int number) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PHONE_HUD_ITEM, icon, name, number); }
	
	// Vehicles
	static void SmashCarWindow(unsigned int vehicle, eVehicleWindow window) { NativeInvoke::Invoke<unsigned int>(NATIVE_SMASH_CAR_WINDOW, vehicle, window); }
	static void SwitchCarSiren(unsigned int vehicle, bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_CAR_SIREN, vehicle, on); }
	static void SetCarCoordinatesNoOffset(unsigned int vehicle, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_COORDINATES_NO_OFFSET, vehicle, x, y, z); }
	static void SetVehicleSteerBias(unsigned int vehicle, float bias) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_VEHICLE_STEER_BIAS, vehicle, bias); }
	static void LockCarDoor(unsigned int vehicle, int flag) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOCK_CAR_DOORS, vehicle, flag); }
	static void SetCarLightMultiplier(unsigned int vehicle, float multiplier) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_LIGHT_MULTIPLIER, vehicle, multiplier); }
	
	// Peds
	static void SetNextDesiredMoveState(ePedMoveState state) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_NEXT_DESIRED_MOVE_STATE, state); }
	static void SetDontActivateRagdollFromPlayerImpact(unsigned int ped, bool Trigger) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DONT_ACTIVATE_RAGDOLL_FROM_PLAYER_IMPACT, ped, Trigger); }
	static void RemoveFakeNetworkNameFromPed(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_FAKE_NETWORK_NAME_FROM_PED, ped); }
	static void GivePedFakeNetworkName(unsigned int ped, const char* name, int r, int g, int b, int a) { NativeInvoke::Invoke<unsigned int>(NATIVE_GIVE_PED_FAKE_NETWORK_NAME, ped, name, r, g, b, a); }
	static void DisplayPlayerNames(bool toggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_PLAYER_NAMES, toggle); }
	static void DisableCarGeneratorsWithHeli(bool toggle) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_CAR_GENERATORS_WITH_HELI, toggle); }
	static void SetCharMeleeActionFlag0(unsigned int ped, bool flag) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_MELEE_ACTION_FLAG0, ped, flag); }
	static void SetCharMeleeActionFlag1(unsigned int ped, bool flag) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_MELEE_ACTION_FLAG1, ped, flag); }
	static void AllowReactionAnims(unsigned int ped, bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_REACTION_ANIMS, ped, allow); }
	static void TaskPlayAnim(unsigned int ped, const char *animSequence, const char *animName, float speed, int loop, float x, float y, float z, int ms) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_PLAY_ANIM, ped, animSequence, animName, speed, loop, x, y, z, ms); }
	static void OverrideNextRestart(float x, float y, float z, float angle) { NativeInvoke::Invoke<unsigned int>(NATIVE_OVERRIDE_NEXT_RESTART, x, y, z, angle); }
	static void SetCharCoordinatesNoOffset(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_COORDINATES_NO_OFFSET, ped, x, y, z); }
	static void TaskJump(unsigned int ped, int type) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_JUMP, ped, type); }
	static void SetAnimGroupForChar(unsigned int ped, char * group) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ANIM_GROUP_FOR_CHAR, ped, group); }
	static void SetCharGestureGroup(unsigned int ped, char * group) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_GESTURE_GROUP, ped, group); }
	
	// Could be handy
	static void SetCharGetOutUpsideDownCar(unsigned int ped, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_GET_OUT_UPSIDE_DOWN_CAR, ped, p1); }
	static void BlockCharAmbientAnims(unsigned int ped, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_CHAR_AMBIENT_ANIMS, ped, p1); }
	static void BlockCharGestureAnims(unsigned int ped, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_CHAR_GESTURE_ANIMS, ped, p1); }
	static void BlockCharHeadIk(unsigned int ped, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_CHAR_HEAD_IK, ped, p1); }
	static void BlockCharVisemeAnims(unsigned int ped, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_CHAR_VISEME_ANIMS, ped, p1); }
	static void BlockCoweringInCover(unsigned int p0, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_COWERING_IN_COVER, p0, p1); }
	static void BlockPeekingInCover(unsigned int p0, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_PEEKING_IN_COVER, p0, p1); }
	static void TaskShootAtCoord(unsigned int ped, float x, float y, float z, unsigned int p4, unsigned int p5) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SHOOT_AT_COORD, ped, x, y, z, p4, p5); }

	// Players
	static unsigned int GetLeftPlayerCashToReachLevel(unsigned int player) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_LEFT_PLAYER_CASH_TO_REACH_LEVEL, player); }
	static void DeletePlayer(unsigned int player) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_PLAYER, player); }
	static void SayAmbientSpeechWithVoice(unsigned int ped, const char *szText, const char *szVoice, unsigned int p3, unsigned int p4, unsigned int p5) { NativeInvoke::Invoke<unsigned int>(NATIVE_SAY_AMBIENT_SPEECH_WITH_VOICE, ped, szText, szVoice, p3, p4, p5); }
	static void TaskHoldObject(unsigned int ped, unsigned int object) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_HOLD_OBJECT, ped, object); }

	// Camera
	static void ActivateScriptedCams(int unknown1_1, int unknown2_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_SCRIPTED_CAMS, unknown1_1, unknown2_1); }
	static void PointCamAtCoord(unsigned int cam, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_POINT_CAM_AT_COORD, cam, pX, pY, pZ); }
	static void PointCamAtVehicle(unsigned int cam, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_POINT_CAM_AT_VEHICLE, cam, vehicle); }
	static void PointCamAtObject(unsigned int cam, unsigned int obj) { NativeInvoke::Invoke<unsigned int>(NATIVE_POINT_CAM_AT_OBJECT, cam, obj); }
	static void PointCamAtPed(unsigned int cam, unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_POINT_CAM_AT_PED, cam, ped); }
	static void PointCamFixedAtVehicle(unsigned int cam, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_POINT_FIXED_CAM_AT_VEHICLE, cam, vehicle); }
	static void SetFollowVehicleCamSubmode(unsigned int mode) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_FOLLOW_VEHICLE_CAM_SUBMODE, mode); }
	static void SetFollowVehicleCamOffset(unsigned int vehicle, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_FOLLOW_VEHICLE_CAM_OFFSET, vehicle, pX, pY, pZ); }
	static void SetDrunkCam(unsigned int cam, float drunk, int ms) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DRUNK_CAM, cam, drunk, ms); }

	// Objects
	static void SetObjectRotation(unsigned int objectIndex, float fX, float fY, float fZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_OBJECT_ROTATION, objectIndex, fX, fY, fZ); }
	static void RequestCollisionForModel(int model) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_COLLISION_FOR_MODEL, model); }
	static bool HasCollisionForModelLoaded(int model) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_COLLISION_FOR_MODEL_LOADED, model); }

	// These are properly documented functions that were moved from ScriptingDirty.h and cleaned up
	// Beware... not all of them are tested.

	// If you need to add your own functions, add it to a separate file in your own project, not here!
	// This file may be updated very often as new functions are documented.

	// unsigned int
	static void AddScore(unsigned int playerIndex, int score) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_SCORE, playerIndex, score); }
	static void AllowPlayerToCarryNonMissionObjects(unsigned int playerIndex, bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_PLAYER_TO_CARRY_NON_MISSION_OBJECTS, playerIndex, allow); }
	static void AlterWantedLevel(unsigned int playerIndex,  unsigned int level) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALTER_WANTED_LEVEL, playerIndex, level); }
	static void AlterWantedLevelNoDrop(unsigned int playerIndex, unsigned int level) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALTER_WANTED_LEVEL_NO_DROP, playerIndex, level); }
	static void ApplyWantedLevelChangeNow(unsigned int playerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_APPLY_WANTED_LEVEL_CHANGE_NOW, playerIndex); }
	static void ChangePlayerModel(unsigned int playerIndex, eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_PLAYER_MODEL, playerIndex, model); }
	static void ClearPlayerHasDamagedAtLeastOnePed(unsigned int playerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED, playerIndex); }
	static unsigned int ConvertIntToPlayerIndex(unsigned int playerId) { return NativeInvoke::Invoke<unsigned int>(NATIVE_CONVERT_INT_TO_PLAYERINDEX, playerId); }
	static void ClearWantedLevel(unsigned int playerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_WANTED_LEVEL, playerIndex); }
	static void CreatePlayer(unsigned int playerId, float x, float y, float z, unsigned int *pPlayerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_PLAYER, playerId, x, y, z, pPlayerIndex); }
	static void DisablePlayerLockon(unsigned int playerIndex, bool disabled) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_PLAYER_LOCKON, playerIndex, disabled); }
	static void DisablePlayerSprint(unsigned int playerIndex, bool disabled) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_PLAYER_SPRINT, playerIndex, disabled); }
	static void GetPlayerChar(unsigned int playerIndex, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYER_CHAR, playerIndex, pPed); }
	static void GetPlayerGroup(unsigned int playerIndex, unsigned int *pGroup) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYER_GROUP, playerIndex, pGroup); }
	static unsigned int GetPlayerId() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYER_ID); }
	static void GetPlayersLastCarNoSave(unsigned int* pVehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYERS_LAST_CAR_NO_SAVE, pVehicle); }
	static void GetPlayerMaxArmour(unsigned int playerIndex, unsigned int *pMaxArmour) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYER_MAX_ARMOUR, playerIndex, pMaxArmour); }
	static const char *GetPlayerName(unsigned int playerIndex) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_PLAYER_NAME, playerIndex); }
	static eModel GetPlayerSettingsModelChoice() { return NativeInvoke::Invoke<eModel>(NATIVE_GET_PLAYERSETTINGS_MODEL_CHOICE); }
	static unsigned int GetPlayerToPlaceBombInCar(unsigned int vehicle) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PLAYER_TO_PLACE_BOMB_IN_CAR, vehicle); }
	static unsigned int GetTimeSincePlayerDroveAgainstTraffic(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_DROVE_AGAINST_TRAFFIC, playerIndex); }
	static unsigned int GetTimeSincePlayerDroveOnPavement(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_DROVE_ON_PAVEMENT, playerIndex); }
	static unsigned int GetTimeSincePlayerHitBuilding(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_HIT_BUILDING, playerIndex); }
	static unsigned int GetTimeSincePlayerHitCar(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_HIT_CAR, playerIndex); }
	static unsigned int GetTimeSincePlayerHitObject(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_HIT_OBJECT, playerIndex); }
	static unsigned int GetTimeSincePlayerHitPed(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_HIT_PED, playerIndex); }
	static unsigned int GetTimeSincePlayerRanLight(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_PLAYER_RAN_LIGHT, playerIndex); }
	static bool HasPlayerCollectedPickup(unsigned int playerIndex, unsigned int pickup) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_PLAYER_COLLECTED_PICKUP, playerIndex, pickup); }
	static bool HasPlayerDamagedAtLeastOnePed(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED, playerIndex); }
	static bool IsPlayerClimbing(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_CLIMBING, playerIndex); }
	static bool IsPlayerControlOn(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_CONTROL_ON, playerIndex); }
	static bool IsPlayerDead(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_DEAD, playerIndex); }
	static bool IsPlayerFreeAimingAtChar(unsigned int playerIndex, unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_FREE_AIMING_AT_CHAR, playerIndex, ped); }
	static bool IsPlayerFreeForAmbientTask(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_FREE_FOR_AMBIENT_TASK, playerIndex); }
	static bool IsPlayerPlaying(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_PLAYING, playerIndex); }
	static bool IsPlayerPressingHorn(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_PRESSING_HORN, playerIndex); }
	static bool IsPlayerTargettingAnything(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_TARGETTING_ANYTHING, playerIndex); }
	static bool IsPlayerTargettingChar(unsigned int playerIndex, unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_TARGETTING_CHAR, playerIndex, ped); }
	static bool IsPlayerTargettingObject(unsigned int playerIndex, unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_TARGETTING_OBJECT, playerIndex, obj); }
	static bool IsScoreGreater(unsigned int playerIndex, unsigned int score) { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCORE_GREATER, playerIndex, score); }
	static bool IsWantedLevelGreater(unsigned int playerIndex, unsigned int level) { return NativeInvoke::Invoke<bool>(NATIVE_IS_WANTED_LEVEL_GREATER, playerIndex, level); }
	static bool PlayerHasChar(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_PLAYER_HAS_CHAR, playerIndex); }
	static bool PlayerHasFlashingStarsAboutToDrop(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_PLAYER_HAS_FLASHING_STARS_ABOUT_TO_DROP, playerIndex); }
	static bool PlayerHasGreyedOutStars(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>(NATIVE_PLAYER_HAS_GREYED_OUT_STARS, playerIndex); }
	static void RegisterPlayerRespawnCoords(unsigned int playerIndex, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_REGISTER_PLAYER_RESPAWN_COORDS, playerIndex, x, y, z); }
	static void SetEveryoneIgnorePlayer(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_EVERYONE_IGNORE_PLAYER, playerIndex, value); }
	static void SetPlayerCanBeHassledByGangs(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CAN_BE_HASSLED_BY_GANGS, playerIndex, value); }
	static void SetPlayerCanDoDriveBy(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CAN_DO_DRIVE_BY, playerIndex, value); }
	static void SetPlayerCanUseCover(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CAN_USE_COVER, playerIndex, value); }
	static void SetPlayerControl(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CONTROL, playerIndex, value); }
	static void SetPlayerControlAdvanced(unsigned int playerIndex, bool unknown1, bool unknown2, bool unknown3) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_CONTROL_ADVANCED, playerIndex, unknown1, unknown2, unknown3); }
	static void SetPlayerFastReload(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_FAST_RELOAD, playerIndex, value); }
	static void SetPlayerGroupToFollowAlways(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_GROUP_TO_FOLLOW_ALWAYS, playerIndex, value); }
	static void SetPlayerInvincible(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_INVINCIBLE, playerIndex, value); }
	static void SetPlayerMoodNormal(unsigned int playerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_MOOD_NORMAL, playerIndex); }
	static void SetPlayerMoodPissedOff(unsigned int playerIndex, unsigned int unknown150) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_MOOD_PISSED_OFF, playerIndex, unknown150); }
	static void SetPlayerNeverGetsTired(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYER_NEVER_GETS_TIRED, playerIndex, value); }
	static void SetPlayerSettingsModelVariationsChoice(unsigned int playerIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE, playerIndex); }
	static void SetPoliceIgnorePlayer(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_POLICE_IGNORE_PLAYER, playerIndex, value); }
	static void StoreScore(unsigned int playerIndex, unsigned int* value) { NativeInvoke::Invoke<unsigned int>(NATIVE_STORE_SCORE, playerIndex, value); }
	static void StoreWantedLevel(unsigned int playerIndex, unsigned int* value) { NativeInvoke::Invoke<unsigned int>(NATIVE_STORE_WANTED_LEVEL, playerIndex, value); }
	static void RemovePlayerHelmet(unsigned int playerIndex, bool remove) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_PLAYER_HELMET, playerIndex, remove); }
	static unsigned int SlideObject(unsigned int objectIndex, float pX, float pY, float pZ, float rX, float rY, float rZ, float speed) { return NativeInvoke::Invoke<unsigned int>(NATIVE_SLIDE_OBJECT, objectIndex, pX, pY, pZ, rX, rY, rZ, speed); }

	// unsigned int
	static void SetCharDucking(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DUCKING, ped, value); }
	static void AddAmmoToChar(unsigned int ped, eWeapon weapon, unsigned int amount) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_AMMO_TO_CHAR, ped, weapon, amount); }
	static void AddArmourToChar(unsigned int ped, unsigned int amount) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_ARMOUR_TO_CHAR, ped, amount); }
	static void ApplyForceToPed(unsigned int ped, unsigned int unknown0_3, float x, float y, float z, float spinX, float spinY, float spinZ, unsigned int unknown4_0, unsigned int unknown5_1, unsigned int unknown6_1, unsigned int unknown7_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_APPLY_FORCE_TO_PED, ped, unknown0_3, x, y, z, spinX, spinY, spinZ, unknown4_0, unknown5_1, unknown6_1, unknown7_1); }
	static void AttachPedToCar(unsigned int ped, unsigned int vehicle, unsigned int unknown0_0, float offsetX, float offsetY, float offsetZ, float unknown1_276, float unknown2_0, unsigned int unknown3_0, unsigned int unknown4_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_ATTACH_PED_TO_CAR, ped, vehicle, unknown0_0, offsetX, offsetY, offsetZ, unknown1_276, unknown2_0, unknown3_0, unknown4_0); }
	static void BlockCharGestureAnims(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_CHAR_GESTURE_ANIMS, ped, value); }
	static void BlockPedWeaponSwitching(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_BLOCK_PED_WEAPON_SWITCHING, ped, value); }
	static void CancelCurrentlyPlayingAmbientSpeech(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CANCEL_CURRENTLY_PLAYING_AMBIENT_SPEECH, ped); }
	static void ClearAllCharProps(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_ALL_CHAR_PROPS, ped); }
	static void ClearCharLastDamageBone(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_LAST_DAMAGE_BONE, ped); }
	static void ClearCharLastDamageEntity(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_LAST_DAMAGE_ENTITY, ped); }
	static void ClearCharLastWeaponDamage(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_LAST_WEAPON_DAMAGE, ped); }	
	static void ClearCharProp(unsigned int ped, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_PROP, ped, unknown); }
	static void ClearCharSecondaryTask(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_SECONDARY_TASK, ped); }
	static void ClearCharTasks(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_TASKS, ped); }
	static void ClearCharTasksImmediately(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_TASKS_IMMEDIATELY, ped); }
	static void ClearRoomForChar(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_ROOM_FOR_CHAR, ped); }
	static void CreateChar(unsigned int type, eModel model, float x, float y, float z, unsigned int *pPed, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CHAR, type, model, x, y, z, pPed, unknownTrue); }
	static void CreateRandomChar(float x, float y, float z, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_RANDOM_CHAR, x, y, z, pPed); }
	static void CreateRandomCharAsDriver(unsigned int vehicle, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_RANDOM_CHAR_AS_DRIVER, vehicle, pPed); }
	static void CreateRandomFemaleChar(float x, float y, float z, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_RANDOM_FEMALE_CHAR, x, y, z, pPed); }
	static void CreateRandomMaleChar(float x, float y, float z, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_RANDOM_MALE_CHAR, x, y, z, pPed); }
	static void DamageChar(unsigned int ped, unsigned int hitPoints, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_DAMAGE_CHAR, ped, hitPoints, unknown); }
	static void DamagePedBodyPart(unsigned int ped, ePedBodyPart part, unsigned int hitPoints) { NativeInvoke::Invoke<unsigned int>(NATIVE_DAMAGE_PED_BODY_PART, ped, part, hitPoints); }
	static void DeleteChar(unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_CHAR, pPed); }
	static void DetachPed(unsigned int ped, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_DETACH_PED, ped, unknown); }
	static void DetachPedFromWithinCar(unsigned int ped, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_DETACH_PED_FROM_WITHIN_CAR, ped, unknown); }
	static bool DoesCharExist(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_CHAR_EXIST, ped); }
	static void DropObject(unsigned int ped, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_DROP_OBJECT, ped, unknownTrue); }
	static void ExplodeCharHead(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXPLODE_CHAR_HEAD, ped); }
	static void ExtinguishCharFire(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXTINGUISH_CHAR_FIRE, ped); }
	static void FirePedWeapon(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_FIRE_PED_WEAPON, ped, x, y, z); }
	static void ForceCharToDropWeapon(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_CHAR_TO_DROP_WEAPON, ped); }
	static unsigned int ForcePedPinnedDown(unsigned int ped, bool force, unsigned int timerMaybe) { return NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_PED_PINNED_DOWN, ped, force, timerMaybe); }
	static void ForcePedToFleeWhilstDrivingVehicle(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_PED_TO_FLEE_WHILST_DRIVING_VEHICLE, ped, vehicle); }
	static void FreezeCharPosition(unsigned int ped, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_CHAR_POSITION, ped, frozen); }
	static void FreezeCharPositionAndDontLoadCollision(unsigned int ped, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION, ped, frozen); }
	static void GetAmmoInCharWeapon(unsigned int ped, eWeapon weapon, unsigned int *pAmmo) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_AMMO_IN_CHAR_WEAPON, ped, weapon, pAmmo); }
	static bool GetAmmoInClip(unsigned int ped, eWeapon weapon, unsigned int *pAmmo) { return NativeInvoke::Invoke<bool>(NATIVE_GET_AMMO_IN_CLIP, ped, weapon, pAmmo); }
	static const char *GetAnimGroupFromChar(unsigned int ped) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_ANIM_GROUP_FROM_CHAR, ped); }
	static void GetCharCoordinates(unsigned int ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_COORDINATES, ped, pX, pY, pZ); }
	static void GetCharHealth(unsigned int ped, unsigned int *pHealth) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_HEALTH, ped, pHealth); }
	static void GetCharAnimCurrentTime(unsigned int ped, const char *animGroup, const char *animName, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_ANIM_CURRENT_TIME, ped, animGroup, animName, pValue); }
	static void GetCharAnimTotalTime(unsigned int ped, const char *animGroup, const char *animName, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_ANIM_TOTAL_TIME, ped, animGroup, animName, pValue); }
	static void GetCharArmour(unsigned int ped, unsigned int *pArmour) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_ARMOUR, ped, pArmour); }
	static unsigned int GetCharDrawableVariation(unsigned int ped, ePedComponent component) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_DRAWABLE_VARIATION, ped, component); }
	static void GetCharExtractedDisplacement(unsigned int ped, bool unknown, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_EXTRACTED_DISPLACEMENT, ped, unknown, pX, pY, pZ); }
	static void GetCharHeading(unsigned int ped, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_HEADING, ped, pValue); }
	static void GetCharHeightAboveGround(unsigned int ped, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_HEIGHT_ABOVE_GROUND, ped, pValue); }
	static unsigned int GetCharLastDamageBone(unsigned int ped, ePedBone *pBone) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_LAST_DAMAGE_BONE, ped, pBone); }
	static bool GetCharMeleeActionFlag0(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CHAR_MELEE_ACTION_FLAG0, ped); }
	static bool GetCharMeleeActionFlag1(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CHAR_MELEE_ACTION_FLAG1, ped); }
	static void GetCharModel(unsigned int ped, eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_MODEL, ped, pModel); }
	static unsigned int GetCharMoney(unsigned int ped) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_MONEY, ped); }
	static void GetCharPropIndex(unsigned int ped, bool unknown, unsigned int *pIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_PROP_INDEX, ped, unknown, pIndex); }
	static bool GetCharReadyToBeExecuted(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CHAR_READY_TO_BE_EXECUTED, ped); }
	static bool GetCharReadyToBeStunned(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CHAR_READY_TO_BE_STUNNED, ped); }
	static void GetCharSpeed(unsigned int ped, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_SPEED, ped, pValue); }
	static unsigned int GetCharTextureVariation(unsigned int ped, ePedComponent component) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_TEXTURE_VARIATION, ped, component); }
	static void GetCharVelocity(unsigned int ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_VELOCITY, ped, pX, pY, pZ); }
	static void GetCharWeaponInSlot(unsigned int ped, eWeaponSlot slot, eWeapon *pWeapon, unsigned int *pAmmo, unsigned int *pUnknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_WEAPON_IN_SLOT, ped, slot, pWeapon, pAmmo, pUnknown); }
	static bool GetCharWillCowerInsteadOfFleeing(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING, ped); }
	static unsigned int GetCurrentCharWeapon(unsigned int ped, eWeapon *pWeapon) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_CHAR_WEAPON, ped, pWeapon); }
	static unsigned int GetDamageToPedBodyPart(unsigned int ped, ePedBodyPart part) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DAMAGE_TO_PED_BODY_PART, ped, part); }
	static void GetDeadCharCoordinates(unsigned int ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DEAD_CHAR_COORDINATES, ped, pX, pY, pZ); }
	static void GetDeadCharPickupCoords(unsigned int ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DEAD_CHAR_PICKUP_COORDS, ped, pX, pY, pZ); }
	static void GetKeyForCharInRoom(unsigned int ped, eInteriorRoomKey *pKey) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_KEY_FOR_CHAR_IN_ROOM, ped, pKey); }
	static unsigned int GetMaxAmmo(unsigned int ped, eWeapon weapon, unsigned int *pMaxAmmo) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MAX_AMMO, ped, weapon, pMaxAmmo); }
	static void GetMaxAmmoInClip(unsigned int ped, eWeapon weapon, unsigned int *pMaxAmmo) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MAX_AMMO_IN_CLIP, ped, weapon, pMaxAmmo); }
	static unsigned int GetNumberOfCharDrawableVariations(unsigned int ped, ePedComponent component) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS, ped, component); }
	static unsigned int GetNumberOfCharTextureVariations(unsigned int ped, ePedComponent component, unsigned int unknown1) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS, ped, component, unknown1); }
	static unsigned int GetObjectPedIsHolding(unsigned int ped) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_PED_IS_HOLDING, ped); }
	static void GetOffsetFromCharInWorldCoords(unsigned int ped, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, ped, x, y, z, pOffX, pOffY, pOffZ); }
	static ePedClimbState GetPedClimbState(unsigned int ped) { return NativeInvoke::Invoke<ePedClimbState>(NATIVE_GET_PED_CLIMB_STATE, ped); }
	static void GetPedBonePosition(unsigned int ped, ePedBone bone, float x, float y, float z, CVector3 *pPosition) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PED_BONE_POSITION, ped, bone, x, y, z, pPosition); }
	static void GetPedGroupIndex(unsigned int ped, unsigned int* pIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PED_GROUP_INDEX, ped, pIndex); }
	static void GetPedType(unsigned int ped, ePedType *pType) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PED_TYPE, ped, pType); }
	static void GivePedHelmet(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_GIVE_PED_HELMET, ped); }
	static void GiveWeaponToChar(unsigned int ped, eWeapon weapon, unsigned int ammo, bool unknown0) { NativeInvoke::Invoke<unsigned int>(NATIVE_GIVE_WEAPON_TO_CHAR, ped, weapon, ammo, unknown0); }
	static bool HasCharBeenDamagedByCar(unsigned int ped, unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_BEEN_DAMAGED_BY_CAR, ped, vehicle); }
	static bool HasCharBeenDamagedByChar(unsigned int ped, unsigned int otherChar, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_BEEN_DAMAGED_BY_CHAR, ped, otherChar, unknownFalse); }
	static bool HasCharBeenDamagedByWeapon(unsigned int ped, eWeapon weapon) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, ped, weapon); }
	static bool HasCharGotWeapon(unsigned int ped, eWeapon weapon) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_GOT_WEAPON, ped, weapon); }
	static bool HasCharSpottedChar(unsigned int ped, unsigned int otherChar) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_SPOTTED_CHAR, ped, otherChar); }
	static bool HasCharSpottedCharInFront(unsigned int ped, unsigned int otherChar) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_SPOTTED_CHAR_IN_FRONT, ped, otherChar); }
	static bool IsAmbientSpeechPlaying(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_AMBIENT_SPEECH_PLAYING, ped); }
	static bool IsCharArmed(unsigned int ped, eWeaponSlot slot) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_ARMED, ped, slot); }
	static bool IsCharDead(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_DEAD, ped); }
	static bool IsCharDucking(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_DUCKING, ped); }
	static bool IsCharFacingChar(unsigned int ped, unsigned int otherChar, float angle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_FACING_CHAR, ped, otherChar, angle); }
	static bool IsCharFatallyInjured(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_FATALLY_INJURED, ped); }
	static bool IsCharGesturing(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_GESTURING, ped); }
	static bool IsCharGettingInToACar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_GETTING_IN_TO_A_CAR, ped); }
	static bool IsCharGettingUp(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_GETTING_UP, ped); }
	static bool IsCharHealthGreater(unsigned int ped, unsigned int health) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_HEALTH_GREATER, ped, health); }
	static bool IsCharInAir(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_AIR, ped); }	
	static bool IsCharInAngledArea2D(unsigned int ped, float x1, float y1, float x2, float y2, float unknown, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANGLED_AREA_2D, ped, x1, y1, x2, y2, unknown, unknownFalse); }
	static bool IsCharInAngledArea3D(unsigned int ped, float x1, float y1, float z1, float x2, float y2, float z2, float unknown, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANGLED_AREA_3D, ped, x1, y1, z1, x2, y2, z2, unknown, unknownFalse); }
	static bool IsCharInAnyBoat(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_BOAT, ped); }
	static bool IsCharInAnyCar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_CAR, ped); }
	static bool IsCharInAnyHeli(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_HELI, ped); }
	static bool IsCharInAnyPlane(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_PLANE, ped); }
	static bool IsCharInAnyPoliceVehicle(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_POLICE_VEHICLE, ped); }
	static bool IsCharInAnyTrain(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_ANY_TRAIN, ped); }
	static bool IsCharInArea2D(unsigned int ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_AREA_2D, ped, x1, y1, x2, y2, unknownFalse); }
	static bool IsCharInArea3D(unsigned int ped, float x1, float y1, float z1, float x2, float y2, float z2, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_AREA_3D, ped, x1, y1, z1, x2, y2, z2, unknownFalse); }
	static bool IsCharInAreaOnFoot2D(unsigned int ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_AREA_ON_FOOT_2D, ped, x1, y1, x2, y2, unknownFalse); }
	static bool IsCharInCar(unsigned int ped, unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_CAR, ped, vehicle); }
	static bool IsCharInFlyingVehicle(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_FLYING_VEHICLE, ped); }
	static bool IsCharInMeleeCombat(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_MELEE_COMBAT, ped); }
	static bool IsCharInModel(unsigned int ped, eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_MODEL, ped, model); }
	static bool IsCharInTaxi(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_TAXI, ped); }
	static bool IsCharInWater(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_IN_WATER, ped); }
	static bool IsCharInjured(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_INJURED, ped); }
	static bool IsCharMale(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_MALE, ped); }
	static bool IsCharModel(unsigned int ped, eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_MODEL, ped, model); }
	static bool IsCharOnAnyBike(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_ON_ANY_BIKE, ped); }
	static bool IsCharOnFire(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_ON_FIRE, ped); }
	static bool IsCharOnFoot(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_ON_FOOT, ped); }
	static bool IsCharOnScreen(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_ON_SCREEN, ped); }
	static bool IsCharPlayingAnim(unsigned int ped, char *animSet, char *animName) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_PLAYING_ANIM, ped, animSet, animName); }
	static bool IsCharShooting(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SHOOTING, ped); }
	static bool IsCharShootingInArea(unsigned int ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SHOOTING_IN_AREA, ped, x1, y1, x2, y2, unknownFalse); }
	static bool IsCharSittingIdle(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SITTING_IDLE, ped); }
	static bool IsCharSittingInAnyCar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SITTING_IN_ANY_CAR, ped); }
	static bool IsCharSittingInCar(unsigned int ped, unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SITTING_IN_CAR, ped, vehicle); }
	static bool IsCharStopped(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_STOPPED, ped); }
	static bool IsCharStuckUnderCar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_STUCK_UNDER_CAR, ped); }
	static bool IsCharSwimming(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_SWIMMING, ped); }
	static bool IsCharTouchingChar(unsigned int ped, unsigned int otherChar) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_TOUCHING_CHAR, ped, otherChar); }
	static bool IsCharTouchingObject(unsigned int ped, unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_TOUCHING_OBJECT, ped, obj); }
	static bool IsCharTouchingObjectOnFoot(unsigned int ped, unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_TOUCHING_OBJECT_ON_FOOT, ped, obj); }
	static bool IsCharTouchingVehicle(unsigned int p0, unsigned int p1) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_TOUCHING_VEHICLE, p0, p1); }
	static bool IsCharTryingToEnterALockedCar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_TRYING_TO_ENTER_A_LOCKED_CAR, ped); }
	static bool IsCharUsingAnyScenario(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_USING_ANY_SCENARIO, ped); }
	static bool IsCharUsingScenario(unsigned int ped, const char *scenarioName) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_USING_SCENARIO, ped, scenarioName); }
	static bool IsCharVisible(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_VISIBLE, ped); }
	static bool IsCharWaitingForWorldCollision(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CHAR_WAITING_FOR_WORLD_COLLISION, ped); }
	static bool IsPedAMissionPed(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_A_MISSION_PED, ped); }
	static bool IsPedAttachedToAnyCar(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_ATTACHED_TO_ANY_CAR, ped); }
	static bool IsPedAttachedToObject(unsigned int ped, unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_ATTACHED_TO_OBJECT, ped, obj); }
	static bool IsPedBeingJacked(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_BEING_JACKED, ped); }
	static bool IsPedDoingDriveby(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_DOING_DRIVEBY, ped); }
	static bool IsPedFleeing(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_FLEEING, ped); }
	static bool IsPedHoldingAnObject(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_HOLDING_AN_OBJECT, ped); }
	static bool IsPedInCombat(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_IN_COMBAT, ped); }
	static bool IsPedInCover(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_IN_COVER, ped); }
	static bool IsPedInGroup(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_IN_GROUP, ped); }
	static bool IsPedJacking(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_JACKING, ped); }
	static bool IsPedLookingAtPed(unsigned int ped, unsigned int otherChar) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_LOOKING_AT_PED, ped, otherChar); }
	static bool IsPedRagdoll(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_RAGDOLL, ped); }
	static bool IsPedRetreating(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_PED_RETREATING, ped); }
	static bool IsScriptedSpeechPlaying(unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCRIPTED_SPEECH_PLAYING, ped); }
	static void MarkCharAsNoLongerNeeded(unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_CHAR_AS_NO_LONGER_NEEDED, pPed); }
	static void ModifyCharMoveState(unsigned int ped, ePedMoveState state) { NativeInvoke::Invoke<unsigned int>(NATIVE_MODIFY_CHAR_MOVE_STATE, ped, state); }
	static void RemoveAllCharWeapons(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_ALL_CHAR_WEAPONS, ped); }
	static void RemoveCharDefensiveArea(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_CHAR_DEFENSIVE_AREA, ped); }
	static void RemoveCharElegantly(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_CHAR_ELEGANTLY, ped); }
	static void RemoveCharFromGroup(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_CHAR_FROM_GROUP, ped); }
	static void ReviveInjuredPed(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_REVIVE_INJURED_PED, ped); }
	static void SayAmbientSpeech(unsigned int ped, char *phraseName, unsigned int unknown0_1, unsigned int unknown1_1, unsigned int unknown2_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SAY_AMBIENT_SPEECH, ped, phraseName, unknown0_1, unknown1_1, unknown2_0); }
	static unsigned int SetAmmoInClip(unsigned int ped, eWeapon weapon, unsigned int ammo) { return NativeInvoke::Invoke<unsigned int>(NATIVE_SET_AMMO_IN_CLIP, ped, weapon, ammo); }
	static void SetBlockingOfNonTemporaryEvents(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS, ped, value); }
	static void SetCharAccuracy(unsigned int ped, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_ACCURACY, ped, value); }
	static void SetCharAmmo(unsigned int ped, eWeapon weapon, unsigned int ammo) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_AMMO, ped, weapon, ammo); }
	static void SetCharAsEnemy(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_AS_ENEMY, ped, value); }
	static void SetCharAsMissionChar(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_AS_MISSION_CHAR, ped); }
	static void SetCharCanBeKnockedOffBike(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE, ped, value); }
	static void SetCharCanBeShotInVehicle(unsigned int ped, bool enabled) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_CAN_BE_SHOT_IN_VEHICLE, ped, enabled); }
	static void SetCharCantBeDraggedOut(unsigned int ped, bool enabled) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_CANT_BE_DRAGGED_OUT, ped, enabled); }
	static void SetCharComponentVariation(unsigned int ped, ePedComponent component, unsigned int modelVariation, unsigned int textureVariation) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_COMPONENT_VARIATION, ped, component, modelVariation, textureVariation); }
	static void SetCharCoordinates(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_COORDINATES, ped, x, y, z); }
	static void SetCharDefaultComponentVariation(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DEFAULT_COMPONENT_VARIATION, ped); }
	static void SetCharDesiredHeading(unsigned int ped, float heading) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DESIRED_HEADING, ped, heading); }
	static void SetCharDropsWeaponsWhenDead(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DROPS_WEAPONS_WHEN_DEAD, ped, value); }
	static void SetCharDruggedUp(unsigned int ped, bool drugged) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DRUGGED_UP, ped, drugged); }
	static void SetCharFireDamageMultiplier(unsigned int ped, float multiplier) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_FIRE_DAMAGE_MULTIPLIER, ped, multiplier); }
	static void SetCharGravity(unsigned int ped, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_GRAVITY, ped, value); }
	static void SetCharHeading(unsigned int ped, float heading) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_HEADING, ped, heading); }
	static void SetCharHealth(unsigned int ped, unsigned int health) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_HEALTH, ped, health); }
	static void SetCharInvincible(unsigned int ped, bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_INVINCIBLE, ped, enable); }
	static void SetCharIsTargetPriority(unsigned int ped, bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_IS_TARGET_PRIORITY, ped, enable); }
	static void SetCharMaxHealth(unsigned int ped, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_MAX_HEALTH, ped, value); }
	static void SetCharMoney(unsigned int ped, unsigned int amount) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_MONEY, ped, amount); }
	static void SetCharNameDebug(unsigned int ped, const char *value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_NAME_DEBUG, ped, value); }
	static void SetCharNeverLeavesGroup(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_NEVER_LEAVES_GROUP, ped, value); }
	static void SetCharProofs(unsigned int ped, bool unknown0, bool fallingDamage, bool unknown1, bool unknown2, bool unknown3) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_PROOFS, ped, unknown0, fallingDamage, unknown1, unknown2, unknown3); }
	static void SetCharPropIndex(unsigned int ped, ePedPropType propType, unsigned int index) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_PROP_INDEX, ped, propType, index); }
	static void SetCharRandomComponentVariation(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_RANDOM_COMPONENT_VARIATION, ped); }
	static void SetCharSphereDefensiveArea(unsigned int ped, float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_SPHERE_DEFENSIVE_AREA, ped, x, y, z, radius); }
	static void SetCharSuffersCriticalHits(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_SUFFERS_CRITICAL_HITS, ped, value); }
	static void SetCharVelocity(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_VELOCITY, ped, x, y, z); }
	static void SetCharVisible(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_VISIBLE, ped, value); }
	static void SetCharWantedByPolice(unsigned int ped, bool wanted) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WANTED_BY_POLICE, ped, wanted); }
	static void SetCharWillDoDrivebys(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WILL_DO_DRIVEBYS, ped, value); }
	static void SetCharWillFlyThroughWindscreen(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WILL_FLY_THROUGH_WINDSCREEN, ped, value); }
	static void SetCharWillMoveWhenInjured(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WILL_MOVE_WHEN_INJURED, ped, value); }
	static void SetCharWillUseCarsInCombat(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WILL_USE_CARS_IN_COMBAT, ped, value); }
	static void SetCharWillUseCover(unsigned int ped, unsigned int coverPoint) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_WILL_USE_COVER, ped, coverPoint); }
	static void SetCurrentCharWeapon(unsigned int ped, eWeapon w, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CURRENT_CHAR_WEAPON, ped, w, unknownTrue); }
	static void SetGroupCharDucksWhenAimedAt(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_CHAR_DUCKS_WHEN_AIMED_AT, ped, value); }
	static void SetIgnoreLowPriorityShockingEvents(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS, ped, value); }
	static void SetPedDiesWhenInjured(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_DIES_WHEN_INJURED, ped, value); }
	static void SetPedIsBlindRaging(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_IS_BLIND_RAGING, ped, value); }
	static void SetPedIsDrunk(unsigned int ped, bool value ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_IS_DRUNK, ped, value); }
	static void SetPedPathMayDropFromHeight(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_PATH_MAY_DROP_FROM_HEIGHT, ped, value); }
	static void SetPedPathMayUseClimbovers(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_PATH_MAY_USE_CLIMBOVERS, ped, value); }
	static void SetPedPathMayUseLadders(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PED_PATH_MAY_USE_LADDERS, ped, value); }
	static void SetRoomForCharByKey(unsigned int ped, eInteriorRoomKey key) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ROOM_FOR_CHAR_BY_KEY, ped, key); }
	static void SetSenseRange(unsigned int ped, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_SENSE_RANGE, ped, value); }
	static void SwitchPedToAnimated(unsigned int ped, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_PED_TO_ANIMATED, ped, unknownTrue); }
	static unsigned int SwitchPedToRagdoll(unsigned int ped, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5, unsigned int p6) { return NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_PED_TO_RAGDOLL, ped, p1, p2, p3, p4, p5, p6); }    
	static void RemovePedHelmet(unsigned int ped, bool removed) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_PED_HELMET, ped, removed); }
	static void RemoveWeaponFromChar(unsigned int ped, eWeapon weapon) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_WEAPON_FROM_CHAR, ped, weapon); }
	static void UnlockRagdoll(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_UNLOCK_RAGDOLL, ped, value); }
	static void WarpCharFromCarToCoord(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_WARP_CHAR_FROM_CAR_TO_COORD, ped, x, y, z); }
	static void WarpCharIntoCar(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_WARP_CHAR_INTO_CAR, ped, vehicle); }
	static void WarpCharIntoCarAsPassenger(unsigned int ped, unsigned int vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_WARP_CHAR_INTO_CAR_AS_PASSENGER, ped, vehicle, seatIndex); }
	static void WarpCharFromCarToCar(unsigned int ped, unsigned int vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_WARP_CHAR_FROM_CAR_TO_CAR, ped, vehicle, seatIndex); }
	static void	ScreenFadeIn(unsigned int time) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_IN, time); }
	static void	ScreenFadeInUnhacked(unsigned int time) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_IN_UNHACKED, time); }
	static void	ScreenFadeOut(unsigned int time) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_OUT, time); }
	static void	ScreenFadeOutUnhacked(unsigned int time) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_OUT_UNHACKED, time); }
	static void TriggerPoliceReport(const char * szReport) { NativeInvoke::Invoke<unsigned int>(NATIVE_TRIGGER_POLICE_REPORT, szReport); }
	static void TriggerMissionComplete(int iMission) { NativeInvoke::Invoke<unsigned int>(NATIVE_TRIGGER_MISSION_COMPLETE_AUDIO, iMission); }
	static void TriggerGameSound(const char *szMusic) { NativeInvoke::Invoke<unsigned int>(NATIVE_PLAY_SOUND, szMusic/*, NULL, SND_FILENAME | SND_ASYNC*/); }

	// unsigned int Tasks
	static void SetCharKeepTask(unsigned int ped, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_KEEP_TASK, ped, value); }
	static void SetDriveTaskCruiseSpeed(unsigned int ped, float speed) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DRIVE_TASK_CRUISE_SPEED, ped, speed); }
	static void TaskAchieveHeading(unsigned int ped, float heading) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_ACHIEVE_HEADING, ped, heading); }
	static void TaskAimGunAtChar(unsigned int ped, unsigned int targetPed, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_AIM_GUN_AT_CHAR, ped, targetPed, duration); } 
	static void TaskAimGunAtCoord(unsigned int ped, float tX, float tY, float tZ, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_AIM_GUN_AT_COORD, ped, tX, tY, tZ, duration); }
	static void TaskCarDriveWander(unsigned int ped, unsigned int vehicle, float speed, eVehicleDrivingStyle drivingStyle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_DRIVE_WANDER, ped, vehicle, speed, drivingStyle); }
	static void TaskCarDriveToCoord(unsigned int unknown_0, unsigned int vehicle, float fX, float fY, float fZ, float speed, unsigned int unknown_1, unsigned int unknown_2, unsigned int unknown_3, float funknown, unsigned int unknown_4) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRA, unknown_0, vehicle, fX, fY, fZ, speed, unknown_1, unknown_2, unknown_3, funknown, unknown_4); }
	static void TaskCarMission(unsigned int ped, unsigned int vehicle, unsigned int targetEntity, unsigned int missionType, float speed, eVehicleDrivingStyle drivingStyle, unsigned int unknown6_10, unsigned int unknown7_5) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_MISSION, ped, vehicle, targetEntity, missionType, speed, drivingStyle, unknown6_10, unknown7_5); } // target is whatever missiontype requires (ie. vehicle or just 0). missiontypes: 5=wait(, 21=drivetoplayer()
	static void TaskCarMissionNotAgainstTraffic(unsigned int ped, unsigned int vehicle, unsigned int targetEntity, unsigned int missionType, float speed, eVehicleDrivingStyle  drivingStyle, unsigned int unknown6_10, unsigned int unknown7_5) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_MISSION_NOT_AGAINST_TRAFFIC, ped, vehicle, targetEntity, missionType, speed, drivingStyle, unknown6_10, unknown7_5); }
	static void TaskCarMissionCoorsTarget(unsigned int ped, unsigned int vehicle, float x, float y, float z, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_MISSION_COORS_TARGET, ped, vehicle, x, y, z, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
	static void TaskCarMissionCoorsTargetNotAgainstTraffic(unsigned int ped, unsigned int vehicle, float x, float y, float z, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_MISSION_COORS_TARGET_NOT_AGAINST_TRAFFIC, ped, vehicle, x, y, z, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
	static void TaskCarMissionPedTarget(unsigned int ped, unsigned int vehicle, unsigned int target, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_MISSION_PED_TARGET, ped, vehicle, target, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
	static void TaskCarTempAction(unsigned int ped, unsigned int vehicle, unsigned int action, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_CAR_TEMP_ACTION, ped, vehicle, action, duration); } // action 6 = wait?
	static void TaskCombat(unsigned int ped, unsigned int target) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_COMBAT, ped, target); }
	static void TaskCombatHatedTargetsAroundChar(unsigned int ped, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_COMBAT_HATED_TARGETS_AROUND_CHAR, ped, radius); }
	static void TaskCombatHatedTargetsAroundCharTimed(unsigned int ped, float radius, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_COMBAT_HATED_TARGETS_AROUND_CHAR_TIMED, ped, radius, duration); }
	static void TaskCombatTimed(unsigned int ped, unsigned int target, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_COMBAT_TIMED, ped, target, duration); }
	static void TaskDie(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_DIE, ped); }
	static void TaskEnterCarAsDriver(unsigned int ped, unsigned int vehicle, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_ENTER_CAR_AS_DRIVER, ped, vehicle, duration); }
	static void TaskEnterCarAsPassenger(unsigned int ped, unsigned int vehicle, unsigned int duration, unsigned int seatIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_ENTER_CAR_AS_PASSENGER, ped, vehicle, duration, seatIndex); }
	static void TaskEveryoneLeaveCar(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_EVERYONE_LEAVE_CAR, vehicle); }
	static void TaskFollowNavMeshToCoord(unsigned int ped, float x, float y, float z, unsigned int unknown0_2, unsigned int unknown1_minus1, float unknown2_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_FOLLOW_NAV_MESH_TO_COORD, ped, x, y, z, unknown0_2, unknown1_minus1, unknown2_1); }
	static void TaskFollowNavMeshToCoordNoStop(unsigned int ped, float x, float y, float z,unsigned int unknown0_2, unsigned int unknown1_minus1, float unknown2_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_FOLLOW_NAV_MESH_TO_COORD_NO_STOP, ped, x, y, z, unknown0_2, unknown1_minus1, unknown2_1); }
	static void TaskGoStraightToCoord(unsigned int ped, float x, float y, float z, unsigned int unknown2, unsigned int unknown45000) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_GO_STRAIGHT_TO_COORD, ped, x, y, z, unknown2, unknown45000); }
	static void TaskGotoCharOffset(unsigned int ped, unsigned int target, unsigned int duration, float offsetRight, float offsetFront) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_GOTO_CHAR_OFFSET, ped, target, duration, offsetRight, offsetFront); }
	static void TaskGuardCurrentPosition(unsigned int ped, float unknown0_15, float unknown1_10, unsigned int unknown2_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_GUARD_CURRENT_POSITION, ped, unknown0_15, unknown1_10, unknown2_1); }
	static void TaskHandsUp(unsigned int ped, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_HANDS_UP, ped, duration); }
	static void TaskHeliMission(unsigned int ped, unsigned int heli, unsigned int uk0_0, unsigned int uk1_0, float pX, float pY, float pZ, unsigned int uk2_4, float speed, unsigned int uk3_5, float uk4_minus1, unsigned int uk5_round_z_plus_1, unsigned int uk6_40) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_HELI_MISSION, ped, heli, uk0_0, uk1_0, pX, pY, pZ, uk2_4, speed, uk3_5, uk4_minus1, uk5_round_z_plus_1, uk6_40); }
	static void TaskLeaveAnyCar(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LEAVE_ANY_CAR, ped); }
	static void TaskLeaveCar(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LEAVE_CAR, ped, vehicle); }
	static void TaskLeaveCarDontCloseDoor(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LEAVE_CAR_DONT_CLOSE_DOOR, ped, vehicle); }
	static void TaskLeaveCarImmediately(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LEAVE_CAR_IMMEDIATELY, ped, vehicle); }
	static void TaskLookAtChar(unsigned int ped, unsigned int targetPed, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LOOK_AT_CHAR, ped, targetPed, duration, unknown_0); }
	static void TaskLookAtCoord(unsigned int ped, float x, float y, float z, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LOOK_AT_COORD, ped, x, y, z, duration, unknown_0); }
	static void TaskLookAtObject(unsigned int ped, unsigned int targetObject, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LOOK_AT_OBJECT, ped, targetObject, duration, unknown_0); }
	static void TaskLookAtVehicle(unsigned int ped, unsigned int targetVehicle, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_LOOK_AT_VEHICLE, ped, targetVehicle, duration, unknown_0); }
	static void TaskOpenDriverDoor(unsigned int ped, unsigned int vehicle, unsigned int unknown0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_OPEN_DRIVER_DOOR, ped, vehicle, unknown0); }
	static void TaskOpenPassengerDoor(unsigned int ped, unsigned int vehicle, unsigned int seatIndex, unsigned int unknown0) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_OPEN_PASSENGER_DOOR, ped, vehicle, seatIndex, unknown0); }
	static void TaskPause(unsigned int ped, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_PAUSE, ped, duration); }
	static void TaskPlayAnimWithFlags(unsigned int ped, const char *animName, const char *animSet, float unknown0_8, unsigned int unknown1_0, unsigned int flags) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_PLAY_ANIM_WITH_FLAGS, ped, animName, animSet, unknown0_8, unknown1_0, flags); }
	static void TaskSetCharDecisionMaker(unsigned int ped, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SET_CHAR_DECISION_MAKER, ped, dm); }
	static void TaskSetCombatDecisionMaker(unsigned int ped, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SET_COMBAT_DECISION_MAKER, ped, dm); }
	static void TaskSmartFleeChar(unsigned int ped, unsigned int fleeFromPed, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SMART_FLEE_CHAR, ped, fleeFromPed, unknown0_100, duration); }
	static void TaskSmartFleeCharPreferringPavements(unsigned int ped, unsigned int fleeFromPed, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SMART_FLEE_CHAR_PREFERRING_PAVEMENTS, ped, fleeFromPed, unknown0_100, duration); }
	static void TaskSmartFleePoint(unsigned int ped, float x, float y, float z, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SMART_FLEE_POINT, ped, x, y, x, unknown0_100, duration); }
	static void TaskStandStill(unsigned int ped, int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_STAND_STILL, ped, duration); }
	static void TaskSwapWeapon(unsigned int ped, eWeapon weapon) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_SWAP_WEAPON, ped, weapon); }
	static void TaskTurnCharToFaceChar(unsigned int ped, unsigned int targetPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_TURN_CHAR_TO_FACE_CHAR, ped, targetPed); }
	static void TaskTurnCharToFaceCoord(unsigned int ped, float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_TURN_CHAR_TO_FACE_COORD, ped, x, y, z); }
	static void TaskUseMobilePhone(unsigned int ped, bool use) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_USE_MOBILE_PHONE, ped, use); }
	static void TaskUseMobilePhoneTimed(unsigned int ped, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_USE_MOBILE_PHONE_TIMED, ped, duration); }
	static void TaskWanderStandard(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_WANDER_STANDARD, ped); }
	static void TaskWarpCharIntoCarAsDriver(unsigned int ped, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_WARP_CHAR_INTO_CAR_AS_DRIVER, ped, vehicle); }
	static void TaskWarpCharIntoCarAsPassenger(unsigned int ped, unsigned int vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER, ped, vehicle, seatIndex); }
	static void DebugOn() { NativeInvoke::Invoke<unsigned int>(NATIVE_DEBUG_ON); }

	// Task Sequence
	static void OpenSequenceTask(unsigned int *pTaskSequence) { NativeInvoke::Invoke<unsigned int>(NATIVE_OPEN_SEQUENCE_TASK, pTaskSequence); }
	static void CloseSequenceTask(unsigned int taskSequence) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLOSE_SEQUENCE_TASK, taskSequence); }
	static void TaskPerformSequence(unsigned int ped, unsigned int taskSequence) { NativeInvoke::Invoke<unsigned int>(NATIVE_TASK_PERFORM_SEQUENCE, ped, taskSequence); }
	static void ClearSequenceTask(unsigned int taskSequence) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_SEQUENCE_TASK, taskSequence); }

	// unsigned int Search
	static void AllowScenarioPedsToBeReturnedByNextCommand(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND, value); }
	static void BeginCharSearchCriteria() { NativeInvoke::Invoke<unsigned int>(NATIVE_BEGIN_CHAR_SEARCH_CRITERIA); }
	static void EndCharSearchCriteria() { NativeInvoke::Invoke<unsigned int>(NATIVE_END_CHAR_SEARCH_CRITERIA); }
	static bool GetClosestChar(float x, float y, float z, float radius, unsigned int unknown1, unsigned int unknown2, unsigned int *pPed) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CLOSEST_CHAR, x, y, z, radius, unknown1, unknown2, pPed); }
	static void GetRandomCharInAreaOffsetNoSave(float x, float y, float z, float sx, float sy, float sz, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE, x, y, z, sx, sy, sz, pPed); }
	static void SearchCriteriaConsiderPedsWithFlagTrue(unsigned int flagId) { NativeInvoke::Invoke<unsigned int>(NATIVE_SEARCH_CRITERIA_CONSIDER_PEDS_WITH_FLAG_TRUE, flagId); }
	static void SearchCriteriaRejectPedsWithFlagTrue(unsigned int flagId) { NativeInvoke::Invoke<unsigned int>(NATIVE_SEARCH_CRITERIA_REJECT_PEDS_WITH_FLAG_TRUE, flagId); }

	// unsigned int Groups
	static void CreateGroup(bool unknownFalse, unsigned int *pGroup, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_GROUP, unknownFalse, pGroup, unknownTrue); }
	static bool DoesGroupExist(unsigned int group) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_GROUP_EXIST, group); }
	static void GetGroupLeader(unsigned int group, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GROUP_LEADER, group, pPed); }
	static void GetGroupMember(unsigned int group, unsigned int index, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GROUP_MEMBER, group, index, pPed); }
	static void GetGroupSize(unsigned int group, unsigned int *pStartIndex, unsigned int *pCount) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GROUP_SIZE, group, pStartIndex, pCount); }
	static bool IsGroupLeader(unsigned int ped, unsigned int group) { return NativeInvoke::Invoke<bool>(NATIVE_IS_GROUP_LEADER, ped, group); }
	static bool IsGroupMember(unsigned int ped, unsigned int g) { return NativeInvoke::Invoke<bool>(NATIVE_IS_GROUP_MEMBER, ped, g); }
	static void RemoveGroup(unsigned int group) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_GROUP, group); }
	static void SetGroupFollowStatus(unsigned int group, unsigned int status) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_FOLLOW_STATUS, group, status); }
	static void SetGroupFormation(unsigned int group, unsigned int formation) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_FORMATION, group, formation); }
	static void SetGroupFormationSpacing(unsigned int group, float space) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_FORMATION_SPACING, group, space); }
	static void SetGroupLeader(unsigned int group, unsigned int leader) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_LEADER, group, leader); }
	static void SetGroupMember(unsigned int group, unsigned int member) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_MEMBER, group, member); }
	static void SetGroupSeparationRange(unsigned int group, float seperation) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_SEPARATION_RANGE, group, seperation); }

	// unsigned int Relationships
	static void AllowGangRelationshipsToBeChangedByNextCommand(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_GANG_RELATIONSHIPS_TO_BE_CHANGED_BY_NEXT_COMMAND, value); }
	static void SetCharNotDamagedByRelationshipGroup(unsigned int ped, unsigned int relationshipGroup, bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_NOT_DAMAGED_BY_RELATIONSHIP_GROUP, ped, relationshipGroup, enable); }
	static void SetCharRelationship(unsigned int ped, unsigned int relationshipLevel, unsigned int relationshipGroup) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_RELATIONSHIP, ped, relationshipLevel, relationshipGroup); }
	static void SetCharRelationshipGroup(unsigned int ped, unsigned int relationshipGroup) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_RELATIONSHIP_GROUP, ped, relationshipGroup); }
	static void SetRelationship(unsigned int relationshipLevel, unsigned int relationshipGroup1, unsigned int relationshipGroup2) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_RELATIONSHIP, relationshipLevel, relationshipGroup1, relationshipGroup2); }

	// unsigned int
	static void AddUpsideDownCarCheck(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_UPSIDEDOWN_CAR_CHECK, vehicle); }
	static void AnchorBoat(unsigned int boat, bool anchor) { NativeInvoke::Invoke<unsigned int>(NATIVE_ANCHOR_BOAT, boat, anchor); }
	static void ApplyForceToCar(unsigned int vehicle, unsigned int unknown0_3, float x, float y, float z, float spinX, float spinY, float spinZ, unsigned int unknown4_0, unsigned int unknown5_1, unsigned int unknown6_1, unsigned int unknown7_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_APPLY_FORCE_TO_CAR, vehicle, unknown0_3, x, y, z, spinX, spinY, spinZ, unknown4_0, unknown5_1, unknown6_1, unknown7_1); }
	static bool AreTaxiLightsOn(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_ARE_TAXI_LIGHTS_ON, vehicle); }
	static void BreakCarDoor(unsigned int vehicle, eVehicleDoor door, bool unknownFalse) { NativeInvoke::Invoke<unsigned int>(NATIVE_BREAK_CAR_DOOR, vehicle, door, unknownFalse); }
	static void BurstCarTyre(unsigned int vehicle, eVehicleTyre tyre) { NativeInvoke::Invoke<unsigned int>(NATIVE_BURST_CAR_TYRE, vehicle, tyre); }
	static void CreateCar(unsigned int nameHash, float x, float y, float z, unsigned int *pVehicle, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CAR, nameHash, x, y, z, pVehicle, unknownTrue); }
	static void CreateCharAsPassenger(unsigned int vehicle, ePedType charType, eModel model, unsigned int passengerIndex, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CHAR_AS_PASSENGER, vehicle, charType, model, passengerIndex, pPed); }
	static void CreateCharInsideCar(unsigned int vehicle, ePedType charType, eModel model, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CHAR_INSIDE_CAR, vehicle, charType, model, pPed); }
	static void ChangeCarColour(unsigned int vehicle, unsigned int colour1, unsigned int colour2) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_CAR_COLOUR, vehicle, colour1, colour2); }
	static void ClearCarLastDamageEntity(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CAR_LAST_DAMAGE_ENTITY, vehicle); }
	static void ClearCarLastWeaponDamage(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CAR_LAST_WEAPON_DAMAGE, vehicle); }
	static void ClearRoomForCar(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_ROOM_FOR_CAR, vehicle); }
	static void CloseAllCarDoors(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLOSE_ALL_CAR_DOORS, vehicle); }
	static void ControlCarDoor(unsigned int vehicle, eVehicleDoor door, unsigned int unknown_maybe_open, float angle) { NativeInvoke::Invoke<unsigned int>(NATIVE_CONTROL_CAR_DOOR, vehicle, door, unknown_maybe_open, angle); }
	static bool CreateEmergencyServicesCar(eModel model, float x, float y, float z) { return NativeInvoke::Invoke<bool>(NATIVE_CREATE_EMERGENCY_SERVICES_CAR, model, x, y, z); }
	static void CreateRandomCharAsPassenger(unsigned int vehicle, unsigned int seat, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_RANDOM_CHAR_AS_PASSENGER, vehicle, seat, pPed); }
	static void DeleteCar(unsigned int *pVehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_CAR, pVehicle); }
	static void DetachCar(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_DETACH_CAR, vehicle); }
	static bool DoesCarHaveRoof(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_CAR_HAVE_ROOF, vehicle); }
	static bool DoesCarHaveStuckCarCheck(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_CAR_HAVE_STUCK_CAR_CHECK, vehicle); }
	static bool DoesVehicleExist(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_VEHICLE_EXIST, vehicle); }
	static void EnablePedHelmet(unsigned int ped, bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_PED_HELMET, ped, enable); }
	static void ExplodeCar(unsigned int vehicle, bool unknownTrue, bool unknownFalse) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXPLODE_CAR, vehicle, unknownTrue, unknownFalse); }
	static void ExtinguishCarFire(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXTINGUISH_CAR_FIRE, vehicle); }
	static void FixCar(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_FIX_CAR, vehicle); }
	static void FixCarTyre(unsigned int vehicle, eVehicleTyre tyre) { NativeInvoke::Invoke<unsigned int>(NATIVE_FIX_CAR_TYRE, vehicle, tyre); }
	static void ForceVehicleLights(unsigned int vehicle, eVehicleLights lights) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_CAR_LIGHTS, vehicle, lights); }
	static void FreezeCarPosition(unsigned int vehicle, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_CAR_POSITION, vehicle, frozen); }
	static void FreezeCarPositionAndDontLoadCollision(unsigned int vehicle, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION, vehicle, frozen); }
	static void GetCarCharIsUsing(unsigned int ped, unsigned int *pVehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_CHAR_IS_USING, ped, pVehicle); }
	static void GetCarColours(unsigned int vehicle, unsigned int *pColour1, unsigned int *pColour2) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_COLOURS, vehicle, pColour1, pColour2); }
	static void GetCarCoordinates(unsigned int vehicle, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_COORDINATES, vehicle, pX, pY, pZ); }
	static void GetCarDeformationAtPos(unsigned int vehicle, float x, float y, float z, CVector3 *pDeformation) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_DEFORMATION_AT_POS, vehicle, x, y, z, pDeformation); }
	static void GetCarDoorLockStatus(unsigned int vehicle, eVehicleDoorLock *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_DOOR_LOCK_STATUS, vehicle, pValue); }
	static void GetCarForwardX(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_FORWARD_X, vehicle, pValue); }
	static void GetCarForwardY(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_FORWARD_Y, vehicle, pValue); }
	static void GetCarHeading(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_HEADING, vehicle, pValue); }
	static void GetCarHealth(unsigned int vehicle, unsigned int *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_HEALTH, vehicle, pValue); }
	static void GetCarModel(unsigned int vehicle, eModel *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_MODEL, vehicle, pValue); }
	static void GetCarPitch(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_PITCH, vehicle, pValue); }
	static void GetCarRoll(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_ROLL, vehicle, pValue); }
	static void GetCarSpeed(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_SPEED, vehicle, pValue); }
	static void GetCarSpeedVector(unsigned int vehicle, CVector3 *pVector, bool unknownFalse) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_SPEED_VECTOR, vehicle, pVector, unknownFalse); }
	static void GetCarUprightValue(unsigned int vehicle, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_UPRIGHT_VALUE, vehicle, pValue); }
	static void GetCharInCarPassengerSeat(unsigned int vehicle, unsigned int seatIndex, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CHAR_IN_CAR_PASSENGER_SEAT, vehicle, seatIndex, pPed); }
	static unsigned int GetClosestCar(float x, float y, float z, float radius, unsigned int unknown0_0, unsigned int unknown1_70) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CLOSEST_CAR, x, y, z, radius, unknown0_0, unknown1_70); }
	static bool GetClosestCarNode(float x, float y, float z, float *pResX, float *pResY, float *pResZ) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CLOSEST_CAR_NODE, x, y, z, pResX, pResY, pResZ); }
	static bool GetClosestCarNodeWithHeading(float x, float y, float z, float *pResX, float *pResY, float *pResZ, float *pHeading) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CLOSEST_CAR_NODE_WITH_HEADING, x, y, z, pResX, pResY, pResZ, pHeading); }
	static bool GetClosestMajorCarNode(float x, float y, float z, float *pResX, float *pResY, float *pResZ) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CLOSEST_MAJOR_CAR_NODE, x, y, z, pResX, pResY, pResZ); }
	static bool GetNthClosestCarNodeWithHeading(float x, float y, float z, unsigned int nodeNum,  float *pResX, float *pResY, float *pResZ, float *pHeading) { return NativeInvoke::Invoke<bool>(NATIVE_GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING, x, y, z, nodeNum, pResX, pResY, pResZ, pHeading); }
	static bool GetNthClosestCarNodeWithHeadingOnIsland(float x, float y, float z, unsigned int nodeNum, unsigned int areaId, float *pResX, float *pResY, float *pResZ, float *pHeading, unsigned int *pUnknownMaybeAreaId) { return NativeInvoke::Invoke<bool>(NATIVE_GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING_ON_ISLAND, x, y, z, nodeNum, areaId, pResX, pResY, pResZ, pHeading, pUnknownMaybeAreaId); }
	static void GetDeadCarCoordinates(unsigned int vehicle, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DEAD_CAR_COORDINATES, vehicle, pX, pY, pZ); }
	static void GetDoorAngleRatio(unsigned int vehicle, eVehicleDoor door, float *pAngleRatio) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DOOR_ANGLE_RATIO, vehicle, door, pAngleRatio); }
	static void GetDriverOfCar(unsigned int vehicle, unsigned int *pPed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DRIVER_OF_CAR, vehicle, pPed); }
	static float GetEngineHealth(unsigned int vehicle) { return NativeInvoke::Invoke<float>(NATIVE_GET_ENGINE_HEALTH, vehicle); }
	static void GetExtraCarColours(unsigned int vehicle, unsigned int *pColour1, unsigned int *pColour2) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_EXTRA_CAR_COLOURS, vehicle, pColour1, pColour2); }
	static float GetHeightOfVehicle(unsigned int vehicle, float x, float y, float z, bool unknownTrue1, bool unknownTrue2) { return NativeInvoke::Invoke<float>(NATIVE_GET_HEIGHT_OF_VEHICLE, vehicle, x, y, z, unknownTrue1, unknownTrue2); }
	static void GetKeyForCarInRoom(unsigned int vehicle, eInteriorRoomKey *pKey) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_KEY_FOR_CAR_IN_ROOM, vehicle, pKey); }
	static void GetMaximumNumberOfPassengers(unsigned int vehicle, unsigned int *pMax) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MAXIMUM_NUMBER_OF_PASSENGERS, vehicle, pMax); }
	static void GetNearestCableCar(float x, float y, float z, float radius, unsigned int *pVehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NEAREST_CABLE_CAR, x, y, z, radius, pVehicle); }
	static void GetNumCarColours(unsigned int vehicle, unsigned int *pNumColours) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUM_CAR_COLOURS, vehicle, pNumColours); }
	static void GetNumberOfPassengers(unsigned int vehicle, unsigned int *pNumPassengers) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_PASSENGERS, vehicle, pNumPassengers); }
	static void GetOffsetFromCarGivenWorldCoords(unsigned int vehicle, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OFFSET_FROM_CAR_GIVEN_WORLD_COORDS, vehicle, x, y, z, pOffX, pOffY, pOffZ); }
	static void GetOffsetFromCarInWorldCoords(unsigned int vehicle, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OFFSET_FROM_CAR_IN_WORLD_COORDS, vehicle, x, y, z, pOffX, pOffY, pOffZ); }
	static float GetPetrolTankHealth(unsigned int vehicle) { return NativeInvoke::Invoke<float>(NATIVE_GET_PETROL_TANK_HEALTH, vehicle); }
	static void GetRandomCarModelInMemory(bool unknownTrue, unsigned int *pHash, unsigned int *pUnknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_RANDOM_CAR_MODEL_IN_MEMORY, unknownTrue, pHash, pUnknown); }
	static void GetVehicleDirtLevel(unsigned int vehicle, float* pIntensity) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_VEHICLE_DIRT_LEVEL, vehicle, pIntensity); }
	static bool HasCarBeenDamagedByCar(unsigned int vehicle, unsigned int otherCar) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CAR_BEEN_DAMAGED_BY_CAR, vehicle, otherCar); }
	static bool HasCarBeenDamagedByChar(unsigned int vehicle, unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CAR_BEEN_DAMAGED_BY_CHAR, vehicle, ped); }
	static bool HasCarBeenDamagedByWeapon(unsigned int vehicle, eWeapon weapon) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, vehicle, weapon); }
	static bool HasCarBeenResprayed(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CAR_BEEN_RESPRAYED, vehicle); }
	static void GetVehicleQuaternion(unsigned int vehicle, float* one, float* two, float* three, float* four) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_VEHICLE_QUATERNION, vehicle, one, two, three, four); }
	static bool IsBigVehicle(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BIG_VEHICLE, vehicle); }
	static bool IsCarAMissionCar(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_A_MISSION_CAR, vehicle); }
	static bool IsCarAttached(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_ATTACHED, vehicle); }
	static bool IsCarDead(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_DEAD, vehicle); }
	static bool IsCarDoorDamaged(unsigned int vehicle, eVehicleDoor door) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_DOOR_DAMAGED, vehicle, door); }
	static bool IsCarDoorFullyOpen(unsigned int vehicle, eVehicleDoor door) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_DOOR_FULLY_OPEN, vehicle, door); }
	static bool IsCarInAirProper(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_IN_AIR_PROPER, vehicle); }
	static bool IsCarInArea2D(unsigned int vehicle, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_IN_AREA_2D, vehicle, x1, y1, x2, y2, unknownFalse); }
	static bool IsCarInArea3D(unsigned int vehicle, float x1, float y1, float z1, float x2, float y2, float z2, unsigned char unknownFalse) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_IN_AREA_3D, vehicle, x1, y1, z1, x2, y2, z2, unknownFalse); }
	static bool IsCarInWater(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_IN_WATER, vehicle); }
	static bool IsCarModel(unsigned int vehicle, eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_MODEL, vehicle, model); }
	static bool IsCarOnFire(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_ON_FIRE, vehicle); }
	static bool IsCarOnScreen(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_ON_SCREEN, vehicle); }
	static bool IsCarPassengerSeatFree(unsigned int vehicle, unsigned int seatIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_PASSENGER_SEAT_FREE, vehicle, seatIndex); }
	static bool IsCarSirenOn(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_SIREN_ON, vehicle); }
	static bool IsCarStopped(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_STOPPED, vehicle); }
	static bool IsCarStoppedAtTrafficLights(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_STOPPED_AT_TRAFFIC_LIGHTS, vehicle); }
	static bool IsCarStuckOnRoof(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_STUCK_ON_ROOF, vehicle); }
	static bool IsCarTouchingCar(unsigned int vehicle, unsigned int otherCar) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_TOUCHING_CAR, vehicle, otherCar); }
	static bool IsCarTyreBurst(unsigned int vehicle, eVehicleTyre tyre) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_TYRE_BURST, vehicle, tyre); }
	static bool IsCarUpright(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_UPRIGHT, vehicle); }
	static bool IsCarUpsidedown(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_UPSIDEDOWN, vehicle); }
	static bool IsCarWaitingForWorldCollision(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_WAITING_FOR_WORLD_COLLISION, vehicle); }
	static bool IsVehDriveable(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_VEH_DRIVEABLE, vehicle); }
	static bool IsVehWindowIntact(unsigned int vehicle, eVehicleWindow window) { return NativeInvoke::Invoke<bool>(NATIVE_IS_VEH_WINDOW_INTACT, vehicle, window); }
	static bool IsVehicleExtraTurnedOn(unsigned int vehicle, eVehicleExtra extra) { return NativeInvoke::Invoke<bool>(NATIVE_IS_VEHICLE_EXTRA_TURNED_ON, vehicle, extra); }
	static bool IsVehicleOnAllWheels(unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_VEHICLE_ON_ALL_WHEELS, vehicle); }
	static void KnockPedOffBike(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_KNOCK_PED_OFF_BIKE, vehicle); }
	static void LockCarDoors(unsigned int vehicle, eVehicleDoorLock value) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOCK_CAR_DOORS, vehicle, value); }
	static void MarkCarAsConvoyCar(unsigned int vehicle, bool convoyCar) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_CAR_AS_CONVOY_CAR, vehicle, convoyCar); }
	static void MarkCarAsNoLongerNeeded(unsigned int *pVehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_CAR_AS_NO_LONGER_NEEDED, pVehicle); }
	static void OpenCarDoor(unsigned int vehicle, eVehicleDoor door) { NativeInvoke::Invoke<unsigned int>(NATIVE_OPEN_CAR_DOOR, vehicle, door); }
	static void PopCarBoot(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_POP_CAR_BOOT, vehicle); }
	static void RemoveVehicleWindow(unsigned int vehicle, eVehicleWindow window) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_CAR_WINDOW, vehicle, window); }
	static void RemoveStuckCarCheck(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_STUCK_CAR_CHECK, vehicle); }
	static void RemoveUpsidedownCarCheck(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_UPSIDEDOWN_CAR_CHECK, vehicle); }
	static void SetCarCoordinates(unsigned int vehicle, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_COORDINATES, vehicle, pX, pY, pZ); }
	static void SetCarHealth(unsigned int vehicle, unsigned int pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_HEALTH, vehicle, pValue); }
	static void SetPetrolTankHealth(unsigned int vehicle, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_PETROL_TANK_HEALTH, vehicle, value); }
	static void SetCarCanBeDamaged(unsigned int vehicle, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_CAN_BE_DAMAGED, vehicle, value); }
	static void SetCarCanBeVisiblyDamaged(unsigned int vehicle, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_CAN_BE_VISIBLY_DAMAGED, vehicle, value); }
	static void SetCarForwardSpeed(unsigned int vehicle, float speed) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_FORWARD_SPEED, vehicle, speed); }
	static void SetCarHeading(unsigned int vehicle, float dir) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_HEADING, vehicle, dir); }
	static unsigned int SetCarOnGroundProperly(unsigned int vehicle) { return NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_ON_GROUND_PROPERLY, vehicle); }
	static void SetCarProofs(unsigned int vehicle, bool bulletProof, bool fireProof, bool explosionProof, bool collisionProof, bool meleeProof) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_PROOFS, vehicle, bulletProof, fireProof, explosionProof, collisionProof, meleeProof); }
	static void SetCarStrong(unsigned int vehicle, bool strong) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_STRONG, vehicle, strong); }
	static void SetCarVisible(unsigned int vehicle, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_VISIBLE, vehicle, value); }
	static void SetExtraCarColours(unsigned int vehicle, unsigned int colour1, unsigned int colour2) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_EXTRA_CAR_COLOURS, vehicle, colour1, colour2); }
	static void SetEngineHealth(unsigned int vehicle, float health) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ENGINE_HEALTH, vehicle, health); }
	static void SetVehHazardLights(unsigned int vehicle, bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_VEH_HAZARDLIGHTS, vehicle, on); }
	static void SetVehicleDirtLevel(unsigned int vehicle, float intensity) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_VEHICLE_DIRT_LEVEL, vehicle, intensity); }
	static void SetVehicleQuaternion(unsigned int vehicle, float one, float two, float three, float four) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_VEHICLE_QUATERNION, vehicle, one, two, three, four); }
	static void ShutCarDoor(unsigned int vehicle, eVehicleDoor door) { NativeInvoke::Invoke<unsigned int>(NATIVE_SHUT_CAR_DOOR, vehicle, door); }
	static void SoundCarHorn(unsigned int vehicle, unsigned int duration) { NativeInvoke::Invoke<unsigned int>(NATIVE_SOUND_CAR_HORN, vehicle, duration); }
	static void WashVehicleTextures(unsigned int vehicle, unsigned int intensity) { NativeInvoke::Invoke<unsigned int>(NATIVE_WASH_VEHICLE_TEXTURES, vehicle, intensity); }
	static void SetRoomForCarByKey(unsigned int vehicle, eInteriorRoomKey key) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ROOM_FOR_CAR_BY_KEY, vehicle, key); }
	static void SetCarEngineOn( unsigned int vehicle, bool toggle, bool toggle2 ) { NativeInvoke::Invoke< unsigned int >( NATIVE_SET_CAR_ENGINE_ON, vehicle, toggle, toggle2 ); }
	static void SetTaxiLights(unsigned int vehicle, bool toggle) { NativeInvoke::Invoke<unsigned int>( NATIVE_SET_TAXI_LIGHTS, vehicle, toggle); }
	static void ForceCarLights(unsigned int vehicle, int flag) { NativeInvoke::Invoke<unsigned int>( NATIVE_FORCE_CAR_LIGHTS, vehicle, flag); }
	static void ForceCarLightsOff(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>( NATIVE_FORCE_ALL_VEHICLE_LIGHTS_OFF, vehicle); }
	static void SetCarCanBurstTyres(unsigned int vehicle, bool toggle) { NativeInvoke::Invoke<unsigned int>( NATIVE_SET_CAN_BURST_CAR_TYRES, vehicle, toggle); }
	static void	EnableGPSInVehicle(unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>( NATIVE_ENABLE_GPS_IN_VEHICLE, vehicle ); }

	// unsigned int
	static void CreateMissionTrain(unsigned int unknown1, float x, float y, float z, bool unknown2, unsigned int *pTrain) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_MISSION_TRAIN, unknown1, x, y, z, unknown2, pTrain); }
	static void DeleteMissionTrain(unsigned int *pTrain) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_MISSION_TRAIN, pTrain); }
	static eTrainStation GetCurrentStationForTrain(unsigned int train) { return NativeInvoke::Invoke<eTrainStation>(NATIVE_GET_CURRENT_STATION_FOR_TRAIN, train); }
	static eTrainStation GetNextStationForTrain(unsigned int train) { return NativeInvoke::Invoke<eTrainStation>(NATIVE_GET_NEXT_STATION_FOR_TRAIN, train); }
	static const char *GetStationName(unsigned int train, eTrainStation station) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_STATION_NAME, train, station); }
	static void MarkMissionTrainAsNoLongerNeeded(unsigned int train) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_MISSION_TRAIN_AS_NO_LONGER_NEEDED, train); }
	static void MarkMissionTrainsAsNoLongerNeeded() { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED); }
	static void SetTrainCuriseSpeed(unsigned int train, float fSpeed) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TRAIN_CRUISE_SPEED, train, fSpeed); }

	// unsigned int
	static void AddObjectToInteriorRoomByKey(unsigned int obj, eInteriorRoomKey roomKey) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY, obj, roomKey); }
	static void ApplyForceToObject(unsigned int obj, unsigned int uk0_3, float pX, float pY, float pZ, float spinX, float spinY, float spinZ, unsigned int uk4_0, unsigned int uk5_1, unsigned int uk6_1, unsigned int uk7_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_APPLY_FORCE_TO_OBJECT, obj, uk0_3, pX, pY, pZ, spinX, spinY, spinZ, uk4_0, uk5_1, uk6_1, uk7_1); }
	static void AttachObjectToCar(unsigned int obj, unsigned int v, unsigned int unknown0_0, float pX, float pY, float pZ, float rX, float rY, float rZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_ATTACH_OBJECT_TO_CAR, obj, v, unknown0_0, pX, pY, pZ, rX, rY, rZ); }
	static void AttachObjectToPed(unsigned int obj, unsigned int c, ePedBone bone, float pX, float pY, float pZ, float rX, float rY, float rZ, unsigned int unknown1_0) { NativeInvoke::Invoke<unsigned int>(NATIVE_ATTACH_OBJECT_TO_PED, obj, c, bone, pX, pY, pZ, rX, rY, rZ, unknown1_0); }
	static void ClearObjectLastDamageEntity(unsigned int obj) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_OBJECT_LAST_DAMAGE_ENTITY, obj); }
	static void ClearRoomForObject(unsigned int obj) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_ROOM_FOR_OBJECT, obj); }
	static void CreateObject(eModel model, float x, float y, float z, unsigned int *pObj, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_OBJECT, model, x, y, z, pObj, unknownTrue); }
	static void CreateObjectNoOffset(eModel model, float x, float y, float z, unsigned int *pObj, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_OBJECT_NO_OFFSET, model, x, y, z, pObj, unknownTrue); }
	static void DeleteObject(unsigned int *pObj) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_OBJECT, pObj); }
	static void DetachObject(unsigned int obj, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_DETACH_OBJECT, obj, unknown); }
	static bool DoesObjectExist(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_OBJECT_EXIST, obj); }
	static bool DoesObjectHavePhysics(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_OBJECT_HAVE_PHYSICS, obj); }
	static bool DoesObjectOfTypeExistAtCoords(float x, float y, float z, float radius, eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS, x, y, z, radius, model); }
	static void ExtinguishObjectFire(unsigned int obj) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXTINGUISH_OBJECT_FIRE, obj); }
	static void FreezeObjectPosition(unsigned int obj, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_OBJECT_POSITION, obj, frozen); }
	static unsigned int GetCarObjectIsAttachedTo(unsigned int obj) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_OBJECT_IS_ATTACHED_TO, obj); }
	static void GetObjectCoordinates(unsigned int obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_COORDINATES, obj, pX, pY, pZ); }
	static float GetObjectFragmentDamageHealth(unsigned int obj, bool unknown) { return NativeInvoke::Invoke<float>(NATIVE_GET_OBJECT_FRAGMENT_DAMAGE_HEALTH, obj, unknown); }
	static void GetObjectHeading(unsigned int obj, float *pHeading) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_HEADING, obj, pHeading); }
	static void GetObjectHealth(unsigned int obj, float *pHealth) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_HEALTH, obj, pHealth); }
	static void GetObjectModel(unsigned int obj, eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_MODEL, obj, pModel); }
	static void GetObjectRotationVelocity(unsigned int obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_ROTATION_VELOCITY, obj, pX, pY, pZ); }
	static void GetObjectSpeed(unsigned int obj, float *pSpeed) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_SPEED, obj, pSpeed); }
	static void GetObjectVelocity(unsigned int obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OBJECT_VELOCITY, obj, pX, pY, pZ); }
	static void GetOffsetFromObjectInWorldCoords(unsigned int obj, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, obj, x, y, z, pOffX, pOffY, pOffZ); }
	static unsigned int GetPedObjectIsAttachedTo(unsigned int obj) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PED_OBJECT_IS_ATTACHED_TO, obj); }
	static bool HasObjectBeenDamaged(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_OBJECT_BEEN_DAMAGED, obj); }
	static bool HasObjectBeenDamagedByCar(unsigned int obj, unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_OBJECT_BEEN_DAMAGED_BY_CAR, obj, vehicle); }
	static bool HasObjectBeenDamagedByChar(unsigned int obj, unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_OBJECT_BEEN_DAMAGED_BY_CHAR, obj, ped); }
	static bool HasObjectBeenUprooted(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_OBJECT_BEEN_UPROOTED, obj); }
	static bool HasObjectCollidedWithAnything(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_OBJECT_COLLIDED_WITH_ANYTHING, obj); }
	static bool HasPoolObjectCollidedWithCushion(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_POOL_OBJECT_COLLIDED_WITH_CUSHION, obj); }
	static bool HasPoolObjectCollidedWithObject(unsigned int obj, unsigned int otherObj) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_POOL_OBJECT_COLLIDED_WITH_OBJECT, obj, otherObj); }
	static bool IsObjectAttached(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_ATTACHED, obj); }
	static bool IsObjectInWater(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_IN_WATER, obj); }
	static bool IsObjectOnFire(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_ON_FIRE, obj); }
	static bool IsObjectOnScreen(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_ON_SCREEN, obj); }
	static bool IsObjectStatic(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_STATIC, obj); }
	static bool IsObjectUpright(unsigned int obj, float angle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_UPRIGHT, obj, angle); }
	static bool IsObjectWithinBrainActivationRange(unsigned int obj) { return NativeInvoke::Invoke<bool>(NATIVE_IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE, obj); }
	static void LoadAllObjectsNow() { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_ALL_OBJECTS_NOW); }
	static void MakeObjectTargettable(unsigned int obj, bool targettable) { NativeInvoke::Invoke<unsigned int>(NATIVE_MAKE_OBJECT_TARGETTABLE, obj, targettable); }
	static void MarkObjectAsNoLongerNeeded(unsigned int *pObj) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_OBJECT_AS_NO_LONGER_NEEDED, pObj); }
	static void SetObjectCollision(unsigned int obj, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_OBJECT_COLLISION, obj, value); }
	static void SetObjectCoordinates(unsigned int obj, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_OBJECT_COORDINATES, obj, pX, pY, pZ); }
	static void SetObjectHeading(unsigned int obj, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_OBJECT_HEADING, obj, value); }
	static void SetObjectVisible(unsigned int obj, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_OBJECT_VISIBLE, obj, value); }
	static unsigned int StartObjectFire(unsigned int obj) { return NativeInvoke::Invoke<unsigned int>(NATIVE_START_OBJECT_FIRE, obj); }

	// Models
	static bool HasModelLoaded(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_MODEL_LOADED, model); }
	static void GetCurrentBasicCopModel(eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_BASIC_COP_MODEL, pModel); }
	static void GetCurrentBasicPoliceCarModel(eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_BASIC_POLICE_CAR_MODEL, pModel); }
	static void GetCurrentCopModel(eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_COP_MODEL, pModel); }
	static void GetCurrentPoliceCarModel(eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_POLICE_CAR_MODEL, pModel); }
	static void GetCurrentTaxiCarModel(eModel *pModel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_TAXI_CAR_MODEL, pModel); }
	static const char *GetDisplayNameFromVehicleModel(eModel model) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_DISPLAY_NAME_FROM_VEHICLE_MODEL, model); }
	static void GetModelDimensions(eModel model, CVector3 *pMinVector, CVector3 *pMaxVector) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MODEL_DIMENSIONS, model, pMinVector, pMaxVector); }
	static const char *GetModelNameForDebug(eModel model) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_MODEL_NAME_FOR_DEBUG, model); }
	static bool IsThisModelABike(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_BIKE, model); }
	static bool IsThisModelABoat(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_BOAT, model); }
	static bool IsThisModelACar(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_CAR, model); }
	static bool IsThisModelAHeli(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_HELI, model); }
	static bool IsThisModelAPed(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_PED, model); }
	static bool IsThisModelAPlane(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_PLANE, model); }
	static bool IsThisModelATrain(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_TRAIN, model); }
	static bool IsThisModelAVehicle(eModel model) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_MODEL_A_VEHICLE, model); }
	static void MarkModelAsNoLongerNeeded(eModel model) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_MODEL_AS_NO_LONGER_NEEDED, model); }

	// Interiors
	static void ActivateInterior(unsigned int interior, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_INTERIOR, interior, unknownTrue); }
	static void GetInteriorAtCoords(float x, float y, float z, unsigned int *pInterior) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_INTERIOR_AT_COORDS, x, y, z, pInterior); }
	static void GetInteriorFromCar(unsigned int vehicle, unsigned int *pInterior) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_INTERIOR_FROM_CAR, vehicle, pInterior); }
	static void GetInteriorFromChar(unsigned int ped, unsigned int *pInterior) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_INTERIOR_FROM_CHAR, ped, pInterior); }
	static void GetInteriorHeading(unsigned int interior, float *pHeading) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_INTERIOR_HEADING, interior, pHeading); }
	static void GetOffsetFromInteriorInWorldCoords(unsigned int interior, float x, float y, float z, float *pOffset) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_OFFSET_FROM_INTERIOR_IN_WORLD_COORDS, interior, x, y, z, pOffset); }
	static void GetRoomKeyFromObject(unsigned int obj, eInteriorRoomKey *pRoomKey) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_ROOM_KEY_FROM_OBJECT, obj, pRoomKey); }
	static void RequestInteriorModels(eModel model, const char *interiorName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_INTERIOR_MODELS, model, interiorName); }

	// Decision Maker
	static void AddCharDecisionMakerEventResponse(unsigned int dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE, dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
	//static void AddCombatDecisionMakerEventResponse(unsigned int dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_COMBAT_DECISION_MAKER_EVENT_RESPONSE, dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
	static void AddGroupDecisionMakerEventResponse(unsigned int dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_GROUP_DECISION_MAKER_EVENT_RESPONSE, dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
	static void ClearCharDecisionMakerEventResponse(unsigned int dm, unsigned int eventid) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_CHAR_DECISION_MAKER_EVENT_RESPONSE, dm, eventid); }
	//static void ClearCombatDecisionMakerEventResponse(unsigned int dm, unsigned int eventid) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_COMBAT_DECISION_MAKER_EVENT_RESPONSE, dm, eventid); }
	//static void ClearGroupDecisionMakerEventResponse(unsigned int dm, unsigned int eventid) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_GROUP_DECISION_MAKER_EVENT_RESPONSE, dm, eventid); }
	static void CopyCharDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_CHAR_DECISION_MAKER, type, pDM); }
	static void CopyCombatDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_COMBAT_DECISION_MAKER, type, pDM); }
	static void CopyGroupCharDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_GROUP_CHAR_DECISION_MAKER, type, pDM); }
	static void CopyGroupCombatDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_GROUP_COMBAT_DECISION_MAKER, type, pDM); }
	static void CopySharedCharDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_SHARED_CHAR_DECISION_MAKER, type, pDM); }
	static void CopySharedCombatDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_COPY_SHARED_COMBAT_DECISION_MAKER, type, pDM); }
	static bool DoesDecisionMakerExist(unsigned int dm) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_DECISION_MAKER_EXIST, dm); }
	static void LoadCharDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_CHAR_DECISION_MAKER, type, pDM); }
	//static void LoadCombatDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_COMBAT_DECISION_MAKER, type, pDM); }
	//static void LoadGroupDecisionMaker(unsigned int type, unsigned int *pDM) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_GROUP_DECISION_MAKER, type, pDM); }
	static void RemoveDecisionMaker(unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_DECISION_MAKER, dm); }
	static void SetCharDecisionMaker(unsigned int ped, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DECISION_MAKER, ped, dm); }
	static void SetCharDecisionMakerToDefault(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CHAR_DECISION_MAKER_TO_DEFAULT, ped); }
	static void SetCombatDecisionMaker(unsigned int ped, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_COMBAT_DECISION_MAKER, ped, dm); }
	//static void SetDecisionMakerAttributeCanChangeTarget(unsigned int dm, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_CAN_CHANGE_TARGET, dm, value); }
	static void SetDecisionMakerAttributeCaution(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_CAUTION, dm, value); }
	static void SetDecisionMakerAttributeFireRate(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_FIRE_RATE, dm, value); }
	static void SetDecisionMakerAttributeLowHealth(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_LOW_HEALTH, dm, value); }
	//static void SetDecisionMakerAttributeMovementStyle(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_MOVEMENT_STYLE, dm, value); }
	//static void SetDecisionMakerAttributeNavigationStyle(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_NAVIGATION_STYLE, dm, value); }
	//static void SetDecisionMakerAttributeRetreatingBehaviour(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_RETREATING_BEHAVIOUR, dm, value); }
	//static void SetDecisionMakerAttributeSightRange(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_SIGHT_RANGE, dm, value); }
	//static void SetDecisionMakerAttributeStandingStyle(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_STANDING_STYLE, dm, value); }
	//static void SetDecisionMakerAttributeTargetInjuredReaction(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_TARGET_INJURED_REACTION, dm, value); }
	//static void SetDecisionMakerAttributeTargetLossResponse(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_TARGET_LOSS_RESPONSE, dm, value); }
	static void SetDecisionMakerAttributeTeamwork(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_TEAMWORK, dm, value); }
	//static void SetDecisionMakerAttributeWeaponAccuracy(unsigned int dm, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_DECISION_MAKER_ATTRIBUTE_WEAPON_ACCURACY, dm, value); }
	static void SetGroupCharDecisionMaker(unsigned int group, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_CHAR_DECISION_MAKER, group, dm); }
	static void SetGroupCombatDecisionMaker(unsigned int group, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_COMBAT_DECISION_MAKER, group, dm); }
	//static void SetGroupDecisionMaker(unsigned int group, unsigned int dm) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_GROUP_DECISION_MAKER, group, dm); }

	// Blips
	static void AddBlipForCar(unsigned int vehicle, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_CAR, vehicle, pBlip); }
	static void AddBlipForChar(unsigned int ped, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_CHAR, ped, pBlip); }
	static void AddBlipForContact(float x, float y, float z, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_CONTACT, x, y, z, pBlip); }
	static void AddBlipForCoord(float x, float y, float z, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_COORD, x, y, z, pBlip); }
	static void AddBlipForObject(unsigned int obj, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_OBJECT, obj, pBlip); }
	static void AddBlipForPickup(unsigned int pickup, unsigned int *pBlip) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_BLIP_FOR_PICKUP, pickup, pBlip); }
	static void AddSimpleBlipForPickup(unsigned int pickup) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_SIMPLE_BLIP_FOR_PICKUP, pickup); }
	static void ChangeBlipAlpha(unsigned int blip, unsigned int alpha) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_ALPHA, blip, alpha); }
	static void ChangeBlipColour(unsigned int blip, unsigned int colour) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_COLOUR, blip, colour); }
	static void ChangeBlipDisplay(unsigned int blip, eBlipDisplay display) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_DISPLAY, blip, display); }
	static void ChangeBlipNameFromAscii(unsigned int blip, const char *blipName) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_NAME_FROM_ASCII, blip, blipName); }
	static void ChangeBlipNameFromTextFile(unsigned int blip, const char *gxtName) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_NAME_FROM_TEXT_FILE, blip, gxtName); }
	static void ChangeBlipPriority(unsigned int blip, eBlipPriority priority) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_PRIORITY, blip, priority); }
	static void ChangeBlipScale(unsigned int blip, float scale) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_SCALE, blip, scale); }
	static void ChangeBlipSprite(unsigned int blip, eBlipSprite sprite) { NativeInvoke::Invoke<unsigned int>(NATIVE_CHANGE_BLIP_SPRITE, blip, sprite); }
	static void DimBlip(unsigned int blip, bool unknownTrue) { NativeInvoke::Invoke<unsigned int>(NATIVE_DIM_BLIP, blip, unknownTrue); }
	static bool DoesBlipExist(unsigned int blip) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_BLIP_EXIST, blip); }
	static void FlashBlip(unsigned int blip, bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_FLASH_BLIP, blip, on); }
	static void FlashBlipAlt(unsigned int blip, bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_FLASH_BLIP_ALT, blip, on); }
	static void GetBlipColour(unsigned int blip, unsigned int *pColour) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_COLOUR, blip, pColour); }
	static void GetBlipCoords(unsigned int blip, CVector3 *pVector) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_COORDS, blip, pVector); }
	static eBlipType GetBlipInfoIdType(unsigned int blip) { return NativeInvoke::Invoke<eBlipType>(NATIVE_GET_BLIP_INFO_ID_TYPE, blip); }
	static unsigned int GetBlipInfoIdCarIndex(unsigned int blip) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_INFO_ID_CAR_INDEX, blip); }
	static unsigned int GetBlipInfoIdDisplay(unsigned int blip) { return NativeInvoke::Invoke<bool>(NATIVE_GET_BLIP_INFO_ID_DISPLAY, blip); }
	static unsigned int GetBlipInfoIdObjectIndex(unsigned int blip) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_INFO_ID_OBJECT_INDEX, blip); }
	static unsigned int GetBlipInfoIdPedIndex(unsigned int blip) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_INFO_ID_PED_INDEX, blip); }
	static unsigned int GetBlipInfoIdPickupIndex(unsigned int blip) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_BLIP_INFO_ID_PICKUP_INDEX, blip); }
	//static const char *GetBlipName(unsigned int blip) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_BLIP_NAME, blip); }
	static eBlipSprite GetBlipSprite(unsigned int blip) { return NativeInvoke::Invoke<eBlipSprite>(NATIVE_GET_BLIP_SPRITE, blip); }
	static unsigned int GetFirstBlipInfoId(eBlipSprite type) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_FIRST_BLIP_INFO_ID, type); }
	static unsigned int GetNextBlipInfoId(eBlipSprite type) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NEXT_BLIP_INFO_ID, type); }
	static bool IsBlipShortRange(unsigned int blip) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BLIP_SHORT_RANGE, blip); }
	static void RemoveBlip(unsigned int blip) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_BLIP, blip); }
	static void SetBlipAsFriendly(unsigned int blip, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_BLIP_AS_FRIENDLY, blip, value); }
	static void SetBlipAsShortRange(unsigned int blip, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_BLIP_AS_SHORT_RANGE, blip, value); }
	static void SetRoute(unsigned int blip, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_ROUTE, blip, value); }

	// Pickups
	static void AddPickupToInteriorRoomByName(unsigned int pickup, const char *roomName) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_PICKUP_TO_INTERIOR_ROOM_BY_NAME, pickup, roomName); }
	static void AddPickupToInteriorRoomByKey(unsigned int pickup, eInteriorRoomKey roomKey) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_PICKUP_TO_INTERIOR_ROOM_BY_KEY, pickup, roomKey); }
	static void CreateMoneyPickup(float x, float y, float z, unsigned int amount, bool unknownTrue, unsigned int *pPickup) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_MONEY_PICKUP, x, y, z, amount, unknownTrue, pPickup); }
	static void CreatePickup(eModel model, ePickupType pickupType, float x, float y, float z, unsigned int *pPickup, bool unknownFalse) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_PICKUP, model, pickupType, x, y, z, pPickup, unknownFalse); }
	static void CreatePickupRotate(eModel model, ePickupType pickupType, unsigned int unknown, float x, float y, float z, float rX, float rY, float rZ, unsigned int *pPickup) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_PICKUP_ROTATE, model, pickupType, unknown, x, y, z, rX, rY, rZ, pPickup); }
	static void CreatePickupWithAmmo(eModel model, ePickupType pickupType, unsigned int unknown, float x, float y, float z, unsigned int *pPickup) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_PICKUP_WITH_AMMO, model, pickupType, unknown, x, y, z, pPickup); }
	static bool DoesPickupExist(unsigned int pickup) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_PICKUP_EXIST, pickup); }
	static void GetPickupCoordinates(unsigned int pickup, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_PICKUP_COORDINATES, pickup, pX, pY, pZ); }
	static bool HasPickupBeenCollected(unsigned int pickup) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_PICKUP_BEEN_COLLECTED, pickup); }
	static void PickupsPassTime(unsigned int time) { NativeInvoke::Invoke<unsigned int>(NATIVE_PICKUPS_PASS_TIME, time); }
	static void RemovePickup(unsigned int pickup) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_PICKUP, pickup); }
	static void RemoveTemporaryRadarBlipsForPickups() { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_TEMPORARY_RADAR_BLIPS_FOR_PICKUPS); }

	// unsigned int
	static bool CamIsSphereVisible(unsigned int camera, float pX, float pY, float pZ, float radius) { return NativeInvoke::Invoke<bool>(NATIVE_CAM_IS_SPHERE_VISIBLE, camera, pX, pY, pZ, radius); }
	static void CreateCam(unsigned int camtype_usually14, unsigned int *camera) { NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CAM, camtype_usually14, camera); }
	static void DestroyCam(unsigned int camera) { NativeInvoke::Invoke<unsigned int>(NATIVE_DESTROY_CAM, camera); }
	static bool DoesCamExist(unsigned int camera) { return NativeInvoke::Invoke<bool>(NATIVE_DOES_CAM_EXIST, camera); }
	static void SetCamActive(unsigned int camera, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_ACTIVE, camera, value); }
	static void GetCamFov(unsigned int camera, float *fov) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAM_FOV, camera, fov); }
	static void GetCamPos(unsigned int camera,  float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAM_POS, camera, pX, pY, pZ); }
	static void GetCamRot(unsigned int camera, float *angleX, float *angleY, float *angleZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAM_ROT, camera, angleX, angleY, angleZ); }
	static void GetGameCam(unsigned int *camera) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GAME_CAM, camera); }
	static void GetGameCamChild(unsigned int *camera) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GAME_CAM_CHILD, camera); }
	static bool IsCamActive(unsigned int camera) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAM_ACTIVE, camera); }
	static bool IsCamInterpolating() { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAM_INTERPOLATING); }
	static bool IsCamPropagating(unsigned int camera) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAM_PROPAGATING, camera); }
	static void SetCamBehindPed(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_BEHIND_PED, ped); }
	static void SetCamFov(unsigned int camera, float fov) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_FOV, camera, fov); }
	static void SetCamInFrontOfPed(unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_IN_FRONT_OF_PED, ped); }
	static void SetCamPos(unsigned int camera, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_POS, camera, pX, pY, pZ); }
	static void SetCamPropagate(unsigned int camera, bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_PROPAGATE, camera, value); }
	static void SetCamRot(unsigned int camera, float angleX, float angleY, float angleZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_ROT, camera, angleX, angleY, angleZ); }
	static void SetCamTargetPed(unsigned int camera, unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_TARGET_PED, camera, ped); }
	static void AttachCamToPed(unsigned int camera, unsigned int ped) { NativeInvoke::Invoke<unsigned int>(NATIVE_ATTACH_CAM_TO_PED, camera, ped); }
	static void AttachCamToVehicle(unsigned int camera, unsigned int vehicle) { NativeInvoke::Invoke<unsigned int>(NATIVE_ATTACH_CAM_TO_VEHICLE, camera, vehicle); }
	static void SetCamAttachOffset(unsigned int camera, float pX, float pY, float pZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_ATTACH_OFFSET, camera, pX, pY, pZ); }
	static void SetCamAttachOffsetIsRelative(unsigned int p0, unsigned int p1) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAM_ATTACH_OFFSET_IS_RELATIVE, p0, p1); }

	// Network
	static bool NetworkIsGameRanked() { return NativeInvoke::Invoke<bool>(NATIVE_NETWORK_IS_GAME_RANKED); }
	static bool NetworkIsSessionStarted() { return NativeInvoke::Invoke<bool>(NATIVE_NETWORK_IS_SESSION_STARTED); }

	// World
	static bool AreAllNavmeshRegionsLoaded() { return NativeInvoke::Invoke<bool>(NATIVE_ARE_ALL_NAVMESH_REGIONS_LOADED); }
	static void ClearArea(float x, float y, float z, float radius, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_AREA, x, y, z, radius, unknown); }
	static void ClearAreaOfCars(float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_AREA_OF_CARS, x, y, z, radius); }
	static void ClearAreaOfChars(float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_AREA_OF_CHARS, x, y, z, radius); }
	static void ClearAreaOfCops(float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_AREA_OF_COPS, x, y, z, radius); }
	static void ClearAreaOfObjects(float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_AREA_OF_OBJECTS, x, y, z, radius); }
	static void ClearPedNonCreationArea() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_PED_NON_CREATION_AREA); }
	static void ClearPedNonRemovalArea() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_PED_NON_REMOVAL_AREA); }
	static void ExtinguishFireAtPoint(float x, float y, float z, float radius) { NativeInvoke::Invoke<unsigned int>(NATIVE_EXTINGUISH_FIRE_AT_POINT, x, y, z, radius); }
	static void ForceWeather(eWeather weather) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_WEATHER, weather); }
	static void ForceWeatherNow(eWeather weather) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_WEATHER_NOW, weather); }
	static void ForceWind(float wind) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_WIND, wind); }
	static void FreezePositionOfClosestObjectOfType(float x, float y, float z, float radius, eModel model, bool frozen) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_POSITION_OF_CLOSEST_OBJECT_OF_TYPE, x, y, z, radius, model, frozen); }
	static unsigned int GetClosestCar(float x, float y, float z, float radius, eModel model, unsigned int unknown70) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CLOSEST_CAR, x, y, z, radius, model, unknown70); }
	static bool GetClosestChar(float x, float y, float z, float radius, bool unknown1, bool unknown2, unsigned int *pPed) { return NativeInvoke::Invoke<bool>(NATIVE_GET_CLOSEST_CHAR, x, y, z, radius, unknown1, unknown2, pPed); }
	static void GetCurrentWeather(eWeather *pWeather) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_WEATHER, pWeather); }
	static unsigned int GetGroundZFor3DCoord(float x, float y, float z, float *pGroundZ) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GROUND_Z_FOR_3D_COORD, x, y, z, pGroundZ); }
	static unsigned int GetNumberOfFiresInRange(float x, float y, float z, float radius) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_FIRES_IN_RANGE, x, y, z, radius); }
	static void GetSafePickupCoords(float x, float y, float z, float *pSafeX, float *pSafeY, float *pSafeZ) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_SAFE_PICKUP_COORDS, x, y, z, pSafeX, pSafeY, pSafeZ); }
	static bool GetSafePositionForChar(float x, float y, float z, bool unknownTrue, float *pSafeX, float *pSafeY, float *pSafeZ) { return NativeInvoke::Invoke<bool>(NATIVE_GET_SAFE_POSITION_FOR_CHAR, x, y, z, unknownTrue, pSafeX, pSafeY, pSafeZ); }
	//static bool HasClosestObjectOfTypeBeenDamagedByChar(float x, float y, float z, float radius, eModel objectModel, unsigned int ped) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED_BY_CHAR, x, y, z, radius, objectModel, ped); }
	static bool IsAreaOccupied(float x1, float y1, float z1, float x2, float y2, float z2, bool unknownFalse1, bool unknownTrue, bool unknownFalse2, bool unknownFalse3, bool unknownFalse4) { return NativeInvoke::Invoke<bool>(NATIVE_IS_AREA_OCCUPIED, x1, y1, z1, x2, y2, z2, unknownFalse1, unknownTrue, unknownFalse2, unknownFalse3, unknownFalse4); }
	static bool IsBulletInArea(float x, float y, float z, float radius, bool unknownTrue) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BULLET_IN_AREA, x, y, z, radius, unknownTrue); }
	static bool IsBulletInBox(float x1, float y1, float z1, float x2, float y2, float z2, bool unknown) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BULLET_IN_BOX, x1, y1, z1, x2, y2, z2, unknown); }
	static bool IsPointObscuredByAMissionEntity(float pX, float pY, float pZ, float sizeX, float sizeY, float sizeZ) { return NativeInvoke::Invoke<bool>(NATIVE_IS_POINT_OBSCURED_BY_A_MISSION_ENTITY, pX, pY, pZ, sizeX, sizeY, sizeZ); }
	static void LoadScene(float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_SCENE, x, y, z); }
	static void RemoveAllPickupsOfType(ePickupType type) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_ALL_PICKUPS_OF_TYPE, type); }
	static void RequestCollisionAtPosn(float x, float y, float z) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_COLLISION_AT_POSN, x, y, z); }
	static void SwitchAmbientPlanes(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_AMBIENT_PLANES, on); }	
	static void SwitchArrowAboveBlippedPickups(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_ARROW_ABOVE_BLIPPED_PICKUPS, on); }
	static void SwitchRandomBoats(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_RANDOM_BOATS, on); }
	static void SwitchRandomTrains(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_RANDOM_TRAINS, on); }
	static void SwitchGarbageTrucks(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_GARBAGE_TRUCKS, on); }
	static void SwitchMadDrivers(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_SWITCH_MAD_DRIVERS, on); }
	static void SetStateOfClosestDoorOfType(unsigned int dwHash, float fX, float fY, float fZ, bool bLock, float fSwing) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_STATE_OF_CLOSEST_DOOR_OF_TYPE, dwHash, fX, fY, fZ, bLock, fSwing); }

	// Garages
	static void CloseGarage(const char *garageName) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLOSE_GARAGE, garageName); }
	static bool IsCarInGarageArea(const char *garageName, unsigned int vehicle) { return NativeInvoke::Invoke<bool>(NATIVE_IS_CAR_IN_GARAGE_AREA, garageName, vehicle); }
	static unsigned int IsGarageClosed(const char *garageName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_IS_GARAGE_CLOSED, garageName); }
	static unsigned int IsGarageOpen(const char *garageName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_IS_GARAGE_OPEN, garageName); }
	static void OpenGarage(const char *name) { NativeInvoke::Invoke<unsigned int>(NATIVE_OPEN_GARAGE, name); }

	// Text (GXT stuff)
	static void ScriptingDisplayText(float x, float y, const char *gxtName) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT, x, y, gxtName); }
	static void DisplayTextWith2Numbers(float x, float y, const char *gxtName, int number1, int number2) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_2_NUMBERS, x, y, gxtName, number1, number2); }
	static void DisplayTextWithFloat(float x, float y, const char *gxtName, float value, unsigned int unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_FLOAT, x, y, gxtName, value, unknown); }
	static void DisplayTextWithLiteralString(float x, float y, const char *gxtName, const char *literalStr) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_LITERAL_STRING, x, y, gxtName, literalStr); }
	static void DisplayTextWithNumber(float x, float y, const char *gxtName, int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_NUMBER, x, y, gxtName, value); }
	static void DisplayTextWithString(float x, float y, const char *gxtName, const char *gxtStringName) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_STRING, x, y, gxtName, gxtStringName); }
	static void DisplayTextWithTwoLiteralStrings(float x, float y, const char *gxtName, const char *literalStr1, const char *literalStr2) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_TWO_LITERAL_STRINGS, x, y, gxtName, literalStr1, literalStr2); }
	static void DisplayTextWithTwoStrings(float x, float y, const char *gxtName, const char *gxtStringName1, const char *gxtStringName2) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_TEXT_WITH_TWO_STRINGS, x, y, gxtName, gxtStringName1, gxtStringName2); }
	static unsigned int GetLengthOfStringWithThisTextLabel(const char *gxtName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL, gxtName); }
	static const char *GetFirstNCharactersOfString(const char *gxtName, unsigned int chars) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_FIRST_N_CHARACTERS_OF_STRING, gxtName, chars); }
	static void GetMobilePhoneRenderId(unsigned int *pRenderId) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MOBILE_PHONE_RENDER_ID, pRenderId); }
	static const char *GetNthIntegerInString(const char *gxtName, unsigned int index) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_NTH_INTEGER_IN_STRING, gxtName, index); }
	static void GetScriptRenderTargetRenderId(unsigned int *pRenderId) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_SCRIPT_RENDERTARGET_RENDER_ID, pRenderId); }
	static const char *GetStringFromHashKey(unsigned int hashKey) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_STRING_FROM_HASH_KEY, hashKey); }
	static unsigned int GetStringWidth(const char *gxtName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_STRING_WIDTH, gxtName); }
	static unsigned int GetStringWidthWithNumber(const char *gxtName, int number) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_STRING_WIDTH_WITH_NUMBER, gxtName, number); }
	static unsigned int GetStringWidthWithString(const char *gxtName, const char *literalString) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_STRING_WIDTH_WITH_STRING, gxtName, literalString); }
	static bool HasAdditionalTextLoaded(unsigned int textIndex) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_ADDITIONAL_TEXT_LOADED, textIndex); }
	static bool HasThisAdditionalTextLoaded(const char *textName, unsigned int textIndex) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_THIS_ADDITIONAL_TEXT_LOADED, textName, textIndex); }
	static bool IsFontLoaded(eTextFont font) { return NativeInvoke::Invoke<bool>(NATIVE_IS_FONT_LOADED, font); }
	static bool IsStreamingAdditionalText(unsigned int textIndex) { return NativeInvoke::Invoke<bool>(NATIVE_IS_STREAMING_ADDITIONAL_TEXT, textIndex); }
	static void LoadAdditionalText(const char *textName, unsigned int textIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_ADDITIONAL_TEXT, textName, textIndex); }
	static void LoadTextFont(eTextFont font) { NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_TEXT_FONT, font); }
	static void PrintStringWithLiteralStringNow(const char* gxt, const char* text, unsigned int ms, unsigned int unknown1) { NativeInvoke::Invoke<unsigned int>(NATIVE_PRINT_STRING_WITH_LITERAL_STRING_NOW, gxt, text, ms, unknown1); }
	static void RequestAdditionalText(const char *textName, unsigned int textIndex) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_ADDITIONAL_TEXT, textName, textIndex); }
	static void SetTextScale(float w, float h) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_SCALE, w, h); }
	static void SetTextBackground(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_BACKGROUND, value); }
	static void SetTextCentre(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_CENTRE, value); }
	static void SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_COLOUR, r, g, b, a); }
	static void SetTextDrawBeforeFade(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_DRAW_BEFORE_FADE, value); }
	static void SetTextDropshadow(bool displayShadow, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_DROPSHADOW, displayShadow, r, g, b, a); }
	static void SetTextEdge(bool displayEdge, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_EDGE, displayEdge, r, g, b, a); }
	static void SetTextFont(eTextFont font) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_FONT, font); }
	static void SetTextJustify(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_JUSTIFY, value); }
	static void SetTextLineDisplay(unsigned int unk1, unsigned int unk2) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT__LINE__DISPLAY, unk1, unk2); }
	static void SetTextLineHeightMult(float lineHeight) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT__LINE__HEIGHT_MULT, lineHeight); }
	static void SetTextProportional(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_PROPORTIONAL, value); }
	static void SetTextRenderId(unsigned int renderId) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_RENDER_ID, renderId); }
	static void SetTextRightJustify(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_RIGHT_JUSTIFY, value); }
	static void SetTextToUseTextFileColours(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_TO_USE_TEXT__FILE__COLOURS, value); }
	static void SetTextUseUnderscore(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_USE_UNDERSCORE, value); }
	static void SetTextWrap(float unk1, float unk2) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TEXT_WRAP, unk1, unk2); }

	// Textures
	static unsigned int GetTexture(unsigned int dictionary, const char *textureName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TEXTURE, dictionary, textureName); }
	static unsigned int GetTextureFromStreamedTxd(const char *txdName, const char *textureName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TEXTURE_FROM_STREAMED_TXD, txdName, textureName); }
	static unsigned int GetTxd(const char *txdName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TXD, txdName); }
	static bool HasStreamedTxdLoaded(const char *txdName) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_STREAMED_TXD_LOADED, txdName); }
	static unsigned int LoadTxd(const char *txdName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_TXD, txdName); }
	static void MarkStreamedTxdAsNoLongerNeeded(const char *txdName) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_STREAMED_TXD_AS_NO_LONGER_NEEDED, txdName); }
	static void ReleaseTexture(unsigned int texture) { NativeInvoke::Invoke<unsigned int>(NATIVE_RELEASE_TEXTURE, texture); }
	static void RequestStreamedTxd(const char *txdName, bool unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_STREAMED_TXD, txdName, unknown); }
	static void RemoveTxd(unsigned int txd) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_TXD, txd); }

	// Stats
	static void DecrementIntStat(eIntStatistic stat, unsigned int amount) { NativeInvoke::Invoke<unsigned int>(NATIVE_DECREMENT_INT_STAT, stat, amount); }
	static unsigned int GetIntStat(eIntStatistic stat) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_INT_STAT, stat); }
	static void IncrementFloatStatNoMessage(eFloatStatistic stat, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_INCREMENT_FLOAT_STAT_NO_MESSAGE, stat, value); }
	static void IncrementIntStat(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_INCREMENT_INT_STAT, stat, value); }
	static void IncrementIntStatNoMessage(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_INCREMENT_INT_STAT_NO_MESSAGE, stat, value); }
	static void SetIntStat(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_INT_STAT, stat, value); }
	static void SetPlayStatsInt(const char * szname, int iname) {NativeInvoke::Invoke<unsigned int>(NATIVE_PLAYSTATS_INT, szname, iname); }

	// Pad (controls)
	static bool GetPadPitchRoll(unsigned int padIndex, float *pPitch, float *pRoll) { return NativeInvoke::Invoke<bool>(NATIVE_GET_PAD_PITCH_ROLL, padIndex, pPitch, pRoll); }
	static void GetPositionOfAnalogueSticks(unsigned int padIndex, unsigned int *pLeftX, unsigned int *pLeftY, unsigned int *pRightX, unsigned int *pRightY) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_POSITION_OF_ANALOGUE_STICKS, padIndex, pLeftX, pLeftY, pRightX, pRightY); }
	static bool IsButtonJustPressed(unsigned int padIndex, ePadButton button) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BUTTON_JUST_PRESSED, padIndex, button); }
	static bool IsButtonPressed(unsigned int padIndex, ePadButton button) { return NativeInvoke::Invoke<bool>(NATIVE_IS_BUTTON_PRESSED, padIndex, button); }

	// Sound
	static eSound GetSoundId() { return NativeInvoke::Invoke<eSound>(NATIVE_GET_SOUND_ID); }
	static bool HasSoundFinished(eSound sound) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_SOUND_FINISHED, sound); }
	static void PlayAudioEvent(const char *name) { NativeInvoke::Invoke<unsigned int>(NATIVE_PLAY_AUDIO_EVENT, name); }
	static void PlaySoundFrontend(eSound sound, char *soundName) { NativeInvoke::Invoke<unsigned int>(NATIVE_PLAY_SOUND_FRONTEND, sound, soundName); }
	static void SetVariableOnSound(eSound sound, char *varname, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_VARIABLE_ON_SOUND, sound, varname, value); }
	static void StopSound(eSound sound) { NativeInvoke::Invoke<unsigned int>(NATIVE_STOP_SOUND, sound); }
	static void ReleaseSoundId(eSound sound) { NativeInvoke::Invoke<unsigned int>(NATIVE_RELEASE_SOUND_ID, sound); }

	// Time
	static void ForceTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_TIME_OF_DAY, hour, minute); }
	static void ForwardToTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORWARD_TO_TIME_OF_DAY, hour, minute); }
	static void GetCurrentDate(unsigned int *day, unsigned int *month) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_DATE, day, month); }
	static unsigned int GetCurrentDayOfWeek() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_DAY_OF_WEEK); }
	static unsigned int GetHoursOfDay() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_HOURS_OF_DAY); }
	static unsigned int GetMinutesOfDay() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MINUTES_OF_DAY); }
	static unsigned int GetMinutesToTimeOfDay(unsigned int hour, unsigned int minute) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MINUTES_TO_TIME_OF_DAY, hour, minute); }
	static void GetTimeOfDay(unsigned int *hour, unsigned int *minute) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_OF_DAY, hour, minute); }
	static void ReleaseTimeOfDay() { NativeInvoke::Invoke<unsigned int>(NATIVE_RELEASE_TIME_OF_DAY); }
	static void SetTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TIME_OF_DAY, hour, minute); }
	static void SetTimeOneDayBack() { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TIME_ONE_DAY_BACK); }
	static void SetTimeOneDayForward() { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TIME_ONE_DAY_FORWARD); }
	static void SetTimeScale(float scale) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_TIME_SCALE, scale); }

	// Fires
	static bool IsScriptFireExtinguished(unsigned int fire) { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCRIPT_FIRE_EXTINGUISHED, fire); }
	static void RemoveScriptFire(unsigned int fire) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_SCRIPT_FIRE, fire); }
	//static unsigned int StartCarFire(unsigned int vehicle) { return NativeInvoke::Invoke<unsigned int>(NATIVE_START_CAR_FIRE, vehicle); }
	static unsigned int StartCharFire(unsigned int ped) { return NativeInvoke::Invoke<unsigned int>(NATIVE_START_CHAR_FIRE, ped); }
	static unsigned int StartScriptFire(float x, float y, float z, unsigned char numGenerationsAllowed = 0, unsigned int strength = 1) { return NativeInvoke::Invoke<unsigned int>(NATIVE_START_SCRIPT_FIRE, x, y, z, numGenerationsAllowed, strength); }

	// Radio
	static void DisableFrontEndRadio() { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_FRONTEND_RADIO); }
	static void EnableFrontEndRadio() { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_FRONTEND_RADIO); }
	static void FreezeRadioStation(const char *stationName) { NativeInvoke::Invoke<unsigned int>(NATIVE_FREEZE_RADIO_STATION, stationName); }
	static eRadioStation GetPlayerRadioStationIndex() { return NativeInvoke::Invoke<eRadioStation>(NATIVE_GET_PLAYER_RADIO_STATION_INDEX); }
	static const char *GetPlayerRadioStationName() { return NativeInvoke::Invoke<const char *>(NATIVE_GET_PLAYER_RADIO_STATION_NAME); }
	static bool IsRadioRetuning() { return NativeInvoke::Invoke<bool>(NATIVE_IS_RADIO_RETUNING); }
	static void RetuneRadioToStationIndex(eRadioStation radioStation) { NativeInvoke::Invoke<unsigned int>(NATIVE_RETUNE_RADIO_TO_STATION_INDEX, radioStation); }

	// Game/Misc
	static void SetFakeWantedLevel(unsigned int p0) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_FAKE_WANTED_LEVEL, p0); }
	static void AbortAllGarageActivity() { NativeInvoke::Invoke<unsigned int>(NATIVE_ABORT_ALL_GARAGE_ACTIVITY); }
	static void ActivateCheat(eCheat cheat) { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_CHEAT, cheat); }
	static void ActivateFrontEnd() { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_FRONTEND); }
	static void ActivateSaveMenu() { NativeInvoke::Invoke<unsigned int>(NATIVE_ACTIVATE_SAVE_MENU); }
	static void AddExplosion(float x, float y, float z, unsigned int p3, float p4, unsigned int p5, unsigned int p6, float p7) { NativeInvoke::Invoke<unsigned int>(NATIVE_ADD_EXPLOSION, x, y, z, p3, p4, p5, p6, p7); }
	static void AllowEmergencyServices(bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_EMERGENCY_SERVICES, allow); }
	static void AllowGameToPauseForStreaming(bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_GAME_TO_PAUSE_FOR_STREAMING, allow); }
	static void AllowStuntJumpsToTrigger(bool allow) { NativeInvoke::Invoke<unsigned int>(NATIVE_ALLOW_STUNT_JUMPS_TO_TRIGGER, allow); }
	static bool AreWidescreenBordersActive() { return NativeInvoke::Invoke<bool>(NATIVE_ARE_WIDESCREEN_BORDERS_ACTIVE); }
	static unsigned int AwardAchievement(eAchievement achievement) { return NativeInvoke::Invoke<unsigned int>(NATIVE_AWARD_ACHIEVEMENT, achievement); }
	static bool CanPhoneBeSeenOnScreen() { return NativeInvoke::Invoke<bool>(NATIVE_CAN_PHONE_BE_SEEN_ON_SCREEN); }
	static void CancelOverrideRestart() { NativeInvoke::Invoke<unsigned int>(NATIVE_CANCEL_OVERRIDE_RESTART); }
	static void ClearShakePlayerPadWhenControllerDisabled() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_SHAKE_PLAYERPAD_WHEN_CONTROLLER_DISABLED); }
	static void ClearTimeCycleModifier() { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_TIMECYCLE_MODIFIER); }
	static void DeactivateFrontEnd() { NativeInvoke::Invoke<unsigned int>(NATIVE_DEACTIVATE_FRONTEND); }
	static bool DidSaveCompleteSuccessfully() { return NativeInvoke::Invoke<bool>(NATIVE_DID_SAVE_COMPLETE_SUCCESSFULLY); }
	static void DisablePauseMenu(bool disabled) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_PAUSE_MENU, disabled); }
	static void DisablePoliceScanner() { NativeInvoke::Invoke<unsigned int>(NATIVE_DISABLE_POLICE_SCANNER); }
	static void DisplayAmmo(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_AMMO, display); }
	static void DisplayAreaName(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_AREA_NAME, display); }
	static void DisplayCash(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_CASH, display); }
	static void DisplayFrontEndMapBlips(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_FRONTEND_MAP_BLIPS, display); }
	static void DisplayHUD(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_HUD, display); }
	static void DisplayRadar(bool display) { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_RADAR, display); }
	static void DisplaySniperScopeThisFrame() { NativeInvoke::Invoke<unsigned int>(NATIVE_DISPLAY_SNIPER_SCOPE_THIS_FRAME); }
	static void DoAutoSave() { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_AUTO_SAVE); }
	static void DoScreenFadeIn(unsigned int timeMS) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_IN, timeMS); }
	static void DoScreenFadeInUnhacked(unsigned int timeMS) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_IN_UNHACKED, timeMS); }
	static void DoScreenFadeOut(unsigned int timeMS) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_OUT, timeMS); }
	static void DoScreenFadeOutUnhacked(unsigned int timeMS) { NativeInvoke::Invoke<unsigned int>(NATIVE_DO_SCREEN_FADE_OUT_UNHACKED, timeMS); }
	static void DrawRect(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<unsigned int>(NATIVE_DRAW_RECT, x1, y1, x2, y2, r, g, b, a); }
	static void DrawLightWithRange(float x, float y, float z, int red, int green, int blue, float intensity, float range) { NativeInvoke::Invoke<unsigned int>(NATIVE_DRAW_LIGHT_WITH_RANGE,x, y, z, red, green, blue, intensity, range); }
	static void EnableMaxAmmoCap(bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_MAX_AMMO_CAP, enable); }
	static void EnablePoliceScanner() { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_POLICE_SCANNER); }
	static void EnableSceneStreaming(bool enable) { NativeInvoke::Invoke<unsigned int>(NATIVE_ENABLE_SCENE_STREAMING, enable); }
	static void FakeDeathArrest() { NativeInvoke::Invoke<unsigned int>(NATIVE_FAKE_DEATHARREST); }
	static void FindStreetNameAtPosition(float pX, float pY, float pZ, unsigned int *strHash0, unsigned int *strHash1) { NativeInvoke::Invoke<unsigned int>(NATIVE_FIND_STREET_NAME_AT_POSITION, pX, pY, pZ, strHash0, strHash1); }
	static void FireSingleBullet(float x, float y, float z, float targetX, float targetY, float targetZ, unsigned int unknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_FIRE_SINGLE_BULLET, x, y, z, targetX, targetY, targetZ, unknown); }
	static void FlashWeaponIcon(bool on) { NativeInvoke::Invoke<unsigned int>(NATIVE_FLASH_WEAPON_ICON, on); }
	static void ForceInitialPlayerStation(const char *stationName) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_INITIAL_PLAYER_STATION, stationName); }
	static void ForceLoadingScreen(bool force) { NativeInvoke::Invoke<unsigned int>(NATIVE_FORCE_LOADING_SCREEN, force); }
	static void GetCorrectedColour(unsigned int r, unsigned int g, unsigned int b, unsigned int *pR, unsigned int *pG, unsigned int *pB) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CORRECTED_COLOUR, r, g, b, pR, pG, pB); }
	static bool GetCreateRandomCops() { return NativeInvoke::Invoke<bool>(NATIVE_GET_CREATE_RANDOM_COPS); }
	static unsigned int GetCurrentEpisode() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CURRENT_EPISODE); }
	static eLanguage GetCurrentLanguage() { return NativeInvoke::Invoke<eLanguage>(NATIVE_GET_CURRENT_LANGUAGE); }
	static unsigned int GetEpisodeIndexFromSummons() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_EPISODE_INDEX_FROM_SUMMONS); }
	static float GetFloatStat(eFloatStatistic stat) { return NativeInvoke::Invoke<float>(NATIVE_GET_FLOAT_STAT, stat); }
	static void GetFrameTime(float *time) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_FRAME_TIME, time); }
	static void GetGameTimer(unsigned int *pTimer) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_GAME_TIMER, pTimer); }
	static unsigned int GetHashKey(const char *value) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_HASH_KEY, value); }
	static void GetHUDColour(eHUDType type, unsigned int *pR, unsigned int *pG, unsigned int *pB, unsigned int *pUnknown) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_HUD_COLOUR, type, pR, pG, pB, pUnknown); }
	static unsigned int GetIdOfThisThread() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_ID_OF_THIS_THREAD); }
	static bool GetIsDepositAnimRunning() { return NativeInvoke::Invoke<bool>(NATIVE_GET_IS_DEPOSIT_ANIM_RUNNING); }
	static bool GetIsHiDef() { return NativeInvoke::Invoke<bool>(NATIVE_GET_IS_HIDEF); }
	static bool GetIsWidescreen() { return NativeInvoke::Invoke<bool>(NATIVE_GET_IS_WIDESCREEN); }
	static eMapArea GetMapAreaFromCoords(float x, float y, float z) { return NativeInvoke::Invoke<eMapArea>(NATIVE_GET_MAP_AREA_FROM_COORDS, x, y, z); }
	static bool GetMissionFlag() { return NativeInvoke::Invoke<bool>(NATIVE_GET_MISSION_FLAG); }
	static void GetMaxWantedLevel(unsigned int *pMaxWantedLevel) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_MAX_WANTED_LEVEL, pMaxWantedLevel); }
	static const char *GetNameOfInfoZone(float x, float y, float z) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_NAME_OF_INFO_ZONE, x, y, z); }
	static const char *GetNameOfZone(float x, float y, float z) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_NAME_OF_ZONE, x, y, z); }
	static unsigned int GetNumStreamingRequests() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUM_STREAMING_REQUESTS); }
	static unsigned int GetNumberOfInstancesOfStreamedScript(const char *scriptName) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT, scriptName); }
	static unsigned int GetNumberOfPlayers() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUMBER_OF_PLAYERS); }
	static unsigned int GetTimeSinceLastArrest() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_LAST_ARREST); }
	static unsigned int GetTimeSinceLastDeath() { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_TIME_SINCE_LAST_DEATH); }
	static bool HasAchievementBeenPassed(eAchievement achievement) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_ACHIEVEMENT_BEEN_PASSED, achievement); }
	static bool HasDeathArrestExecuted() { return NativeInvoke::Invoke<bool>(NATIVE_HAS_DEATHARREST_EXECUTED); }
	static bool HasResprayHappened() { return NativeInvoke::Invoke<bool>(NATIVE_HAS_RESPRAY_HAPPENED); }
	static bool HasScriptLoaded(const char *scriptName) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_SCRIPT_LOADED, scriptName); }
	static bool HaveAnimsLoaded(const char *animName) { return NativeInvoke::Invoke<bool>(NATIVE_HAVE_ANIMS_LOADED, animName); }
	static bool HaveRequestedPathNodesBeenLoaded(unsigned int requestId) { return NativeInvoke::Invoke<bool>(NATIVE_HAVE_REQUESTED_PATH_NODES_BEEN_LOADED, requestId); }
	static void HideHelpTextThisFrame() { NativeInvoke::Invoke<unsigned int>(NATIVE_HIDE_HELP_TEXT_THIS_FRAME); }
	static void HideHUDAndRadarThisFrame() { NativeInvoke::Invoke<unsigned int>(NATIVE_HIDE_HUD_AND_RADAR_THIS_FRAME); }
	static bool IsAussieVersion() { return NativeInvoke::Invoke<bool>(NATIVE_IS_AUSSIE_VERSION); }
	static bool IsAutoAimingOn() { return NativeInvoke::Invoke<bool>(NATIVE_IS_AUTO_AIMING_ON); }
	static bool IsAutoSaveInProgress() { return NativeInvoke::Invoke<bool>(NATIVE_IS_AUTO_SAVE_IN_PROGRESS); }
	static bool IsInteriorScene() { return NativeInvoke::Invoke<bool>(NATIVE_IS_INTERIOR_SCENE); }
	static bool IsFrontEndFading() { return NativeInvoke::Invoke<bool>(NATIVE_IS_FRONTEND_FADING); }
	static bool IsGameInControlOfMusic() { return NativeInvoke::Invoke<bool>(NATIVE_IS_GAME_IN_CONTROL_OF_MUSIC); }
	static bool IsGermanVersion() { return NativeInvoke::Invoke<bool>(NATIVE_IS_GERMAN_VERSION); }
	static bool IsHelpMessageBeingDisplayed() { return NativeInvoke::Invoke<bool>(NATIVE_IS_HELP_MESSAGE_BEING_DISPLAYED); }
	static bool IsHintRunning() { return NativeInvoke::Invoke<bool>(NATIVE_IS_HINT_RUNNING); }
	static bool IsHUDPreferenceSwitchedOn() { return NativeInvoke::Invoke<bool>(NATIVE_IS_HUD_PREFERENCE_SWITCHED_ON); }
	static bool IsHUDReticuleComplex() { return NativeInvoke::Invoke<bool>(NATIVE_IS_HUD_RETICULE_COMPLEX); }
	static bool IsInSpectatorMode() { return NativeInvoke::Invoke<bool>(NATIVE_IS_IN_SPECTATOR_MODE); }
	static bool IsLookInverted() { return NativeInvoke::Invoke<bool>(NATIVE_IS_LOOK_INVERTED); }
	static bool IsMemoryCardInUse() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MEMORY_CARD_IN_USE); }
	static bool IsMessageBeingDisplayed() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MESSAGE_BEING_DISPLAYED); }
	static bool IsMinigameInProgress() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MINIGAME_IN_PROGRESS); }
	static bool IsMissionCompletePlaying() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MISSION_COMPLETE_PLAYING); }
	static bool IsMobilePhoneCallOngoing() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MOBILE_PHONE_CALL_ONGOING); }
	static bool IsMobilePhoneRadioActive() { return NativeInvoke::Invoke<bool>(NATIVE_IS_MOBILE_PHONE_RADIO_ACTIVE); }
	static bool IsPauseMenuActive() { return NativeInvoke::Invoke<bool>(NATIVE_IS_PAUSE_MENU_ACTIVE); }
	static bool IsPayNSprayActive() { return NativeInvoke::Invoke<bool>(NATIVE_IS_PAY_N_SPRAY_ACTIVE); }
	static bool IsPlayerBeingArrested() { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_BEING_ARRESTED); }
	static bool IsPlayerOnline() { return NativeInvoke::Invoke<bool>(NATIVE_IS_PLAYER_ONLINE); }
	static bool IsScreenFadedIn() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCREEN_FADED_IN); }
	static bool IsScreenFadedOut() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCREEN_FADED_OUT); }
	static bool IsScreenFading() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCREEN_FADING); }
	static bool IsScreenFadingIn() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCREEN_FADING_IN); }
	static bool IsScreenFadingOut() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SCREEN_FADING_OUT); }
	static bool IsSniperInverted() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SNIPER_INVERTED); }
	static bool IsSystemUIShowing() { return NativeInvoke::Invoke<bool>(NATIVE_IS_SYSTEM_UI_SHOWING); }
	static bool IsThisAMinigameScript() { return NativeInvoke::Invoke<bool>(NATIVE_IS_THIS_A_MINIGAME_SCRIPT); }
	static bool IsThreadActive(unsigned int threadId) { return NativeInvoke::Invoke<bool>(NATIVE_IS_THREAD_ACTIVE, threadId); }
	static unsigned int LoadAllPathNodes(bool value) { return NativeInvoke::Invoke<unsigned int>(NATIVE_LOAD_ALL_PATH_NODES, value); }
	static void MarkScriptAsNoLongerNeeded(const char *scriptName) { NativeInvoke::Invoke<unsigned int>(NATIVE_MARK_SCRIPT_AS_NO_LONGER_NEEDED, scriptName); }
	static void PauseGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_PAUSE_GAME); }
	static void PlayMovie() { NativeInvoke::Invoke<unsigned int>(NATIVE_PLAY_MOVIE); }
	static void PopulateNow() { NativeInvoke::Invoke<unsigned int>(NATIVE_POPULATE_NOW); }
	static void RequestAnims(const char *animName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_ANIMS, animName); }
	static void ReleaseMovie() { NativeInvoke::Invoke<unsigned int>(NATIVE_RELEASE_MOVIE); }
	static void ReleaseWeather() { NativeInvoke::Invoke<unsigned int>(NATIVE_RELEASE_WEATHER); }
	static void RemoveAnims(const char *animName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_ANIMS, animName); }
	static void RemoveCoverPoint(unsigned int coverPoint) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_COVER_POINT, coverPoint); }
	static void RemoveIpl(const char *iplName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REMOVE_IPL, iplName); }
	static void RequestIpl(const char *iplName) { NativeInvoke::Invoke<unsigned int>(NATIVE_REQUEST_IPL, iplName); }
	static void SetCameraControlsDisabledWithPlayerControls(bool value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAMERA_CONTROLS_DISABLED_WITH_PLAYER_CONTROLS, value); }
	static void SetMissionFlag(bool isMission) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_MISSION_FLAG, isMission); }
	static void SetFloatStat(eFloatStatistic stat, float value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_FLOAT_STAT, stat, value); }
	static void SetRadarZoom(float zoom) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_RADAR_ZOOM, zoom); }
	static void SetTimerA(unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SETTIMERA, value); }
	static void SetTimerB(unsigned int value) { NativeInvoke::Invoke<unsigned int>(NATIVE_SETTIMERB, value); }
	static void SetWantedMultiplier(float multiplier) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_WANTED_MULTIPLIER, multiplier); }
	static unsigned int TimerA() { return NativeInvoke::Invoke<unsigned int>(NATIVE_TIMERA); }
	static unsigned int TimerB() { return NativeInvoke::Invoke<unsigned int>(NATIVE_TIMERB); }
	static unsigned int TimeStep() { return NativeInvoke::Invoke<unsigned int>(NATIVE_TIMESTEP); }
	static void UnlockLazlowStation() { NativeInvoke::Invoke<unsigned int>(NATIVE_UNLOCK_LAZLOW_STATION); }
	static void UnregisterScriptWithAudio() { NativeInvoke::Invoke<unsigned int>(NATIVE_UNREGISTER_SCRIPT_WITH_AUDIO); }
	static void UnPauseGame() { NativeInvoke::Invoke<unsigned int>(NATIVE_UNPAUSE_GAME); }
	static bool HasCharAnimFinished(unsigned int ped, const char *anim) { return NativeInvoke::Invoke<bool>(NATIVE_HAS_CHAR_ANIM_FINISHED, ped, anim); }

	// General
	static void GenerateRandomFloatInRange(float min, float max, float *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GENERATE_RANDOM_FLOAT_IN_RANGE, min, max, pValue); }
	static void GenerateRandomIntInRange(unsigned int min, unsigned int max, unsigned int *pValue) { NativeInvoke::Invoke<unsigned int>(NATIVE_GENERATE_RANDOM_INT_IN_RANGE, min, max, pValue); }
	static void GetAngleBetween2DVectors(float x1, float y1, float x2, float y2, float *pResult) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_ANGLE_BETWEEN_2D_VECTORS, x1, y1, x2, y2, pResult); }
	static void GetDistanceBetweenCoords2D(float x1, float y1, float x2, float y2, float *pDist) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DISTANCE_BETWEEN_COORDS_2D, x1, y1, x2, y2, pDist); }
	static void GetDistanceBetweenCoords3D(float x1, float y1, float z1, float x2, float y2, float z2, float *pDist) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_DISTANCE_BETWEEN_COORDS_3D, x1, y1, z1, x2, y2, z2, pDist); }
	static const char *GetFirstNCharactersOfLiteralString(const char *literalString, unsigned int chars) { return NativeInvoke::Invoke<const char *>(NATIVE_GET_FIRST_N_CHARACTERS_OF_LITERAL_STRING, literalString, chars); }
	static void GetHeadingFromVector2D(float x, float y, float *pHeading) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_HEADING_FROM_VECTOR_2D, x, y, pHeading); }
	static unsigned int GetLengthOfLiteralString(const char *literalString) { return NativeInvoke::Invoke<unsigned int>(NATIVE_GET_LENGTH_OF_LITERAL_STRING, literalString); }

	static unsigned int CreateCheckpoint(unsigned int type, float x, float y, float z, float nextx, float nexty, float nextz, float radius) { return NativeInvoke::Invoke<unsigned int>(NATIVE_CREATE_CHECKPOINT, type, x, y, z, nextx, nexty, nextz, radius); }
	static void DeleteCheckpoint(unsigned int id) { NativeInvoke::Invoke<unsigned int>(NATIVE_DELETE_CHECKPOINT, id); }

	static void TurnOffVehicleExtra(unsigned int vehicle, eVehicleExtra extra, int p2) { NativeInvoke::Invoke<unsigned int>(NATIVE_TURN_OFF_VEHICLE_EXTRA, vehicle, extra, p2); }
	static void InitCutscene(const char* name) { NativeInvoke::Invoke<unsigned int>(NATIVE_INIT_CUTSCENE, name); }
	static void ClearNamedCutscene(const char* name) { NativeInvoke::Invoke<unsigned int>(NATIVE_CLEAR_NAMED_CUTSCENE, name); }
	static void SetCarLivery(unsigned int vehicle, int livery) { NativeInvoke::Invoke<unsigned int>(NATIVE_SET_CAR_LIVERY, vehicle, livery); }
	static void GetCarLivery(unsigned int vehicle, int* livery) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_CAR_LIVERY, vehicle, livery); }
	static void GetNumCarLiveries(unsigned int vehicle, int* livery) { NativeInvoke::Invoke<unsigned int>(NATIVE_GET_NUM_CAR_LIVERIES, vehicle, livery); }
}
