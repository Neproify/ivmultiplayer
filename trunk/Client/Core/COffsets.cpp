//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: COffsets.cpp
// Project: Client.Core
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "COffsets.h"

eGameVersion COffsets::m_GameVersion = GAME_VERSION_UNKNOWN;

// Players
unsigned int COffsets::FUNC_CreatePlayerPed = 0;
unsigned int COffsets::FUNC_SetupPedIntelligence = 0;
unsigned int COffsets::FUNC_CPlayerPed__SetModelIndex = 0;

// Game
unsigned int COffsets::VAR_MenuActive_1 = 0;
unsigned int COffsets::VAR_MenuActive_2 = 0;

// Script
unsigned int COffsets::FUNC_GetRunningScriptThread = 0;
unsigned int COffsets::CALL_CScriptVM__Process = 0;
unsigned int COffsets::VAR_ScrVM__ThreadPool = 0;
unsigned int COffsets::VAR_ScrVM__ActiveThread = 0;
unsigned int COffsets::FUNC_ScrVM__FindNativeAddress = 0;
unsigned int COffsets::VAR_ScrVM__GlobalVariablesPool = 0;

// Weather
unsigned int COffsets::FUNC_SetWeather = 0;
unsigned int COffsets::VAR_CurrentWeather = 0;

// Clock
unsigned int COffsets::FUNC_SetTimeOfDay = 0;
unsigned int COffsets::VAR_TimeOfDay = 0;
unsigned int COffsets::VAR_CClock__Hour = 0;
unsigned int COffsets::VAR_CClock__Minute = 0;
unsigned int COffsets::VAR_CClock__LockedHour = 0;
unsigned int COffsets::VAR_CClock__LockedMinute = 0;
unsigned int COffsets::VAR_CClock__DayOfWeek = 0;

// HUD
unsigned int COffsets::VAR_HudEnabled = 0;
unsigned int COffsets::VAR_RadarVisible = 0;
unsigned int COffsets::VAR_PlayerNamesVisible = 0;
unsigned int COffsets::VAR_AreaNamesDisabled = 0;

// World
unsigned int COffsets::FUNC_CWorld__AddEntity = 0;
unsigned int COffsets::FUNC_CWorld__RemoveEntity = 0;
unsigned int COffsets::FUNC_CEntity__IsTouchingEntity = 0;
unsigned int COffsets::FUNC_CMatrix__ConvertFromEulerAngles = 0;
unsigned int COffsets::FUNC_CMatrix__ConvertFromQuaternion = 0;
unsigned int COffsets::FUNC_CQuaternion__ConvertFromMatrix = 0;

// Vehicle
unsigned int COffsets::FUNC_CVehicle__TurnEngineOn = 0;
unsigned int COffsets::FUNC_CVehicle__TurnEngineOff = 0;
unsigned int COffsets::FUNC_CVehicle__RefreshColours = 0;

// Misc
unsigned int COffsets::VAR_CurrentWantedLevel = 0;
unsigned int COffsets::VAR_ScrollBarText = 0;
unsigned int COffsets::VAR_ScrollBarColor = 0;
unsigned int COffsets::VAR_CameraManager = 0;
unsigned int COffsets::VAR_LocalPlayerState = 0;
unsigned int COffsets::FUNC_GetLocalPlayerSpawnPosition = 0;
unsigned int COffsets::CALL_SpawnLocalPlayer = 0;
unsigned int COffsets::FUNC_SpawnPlayer = 0;
unsigned int COffsets::FUNC_GetTrafficLightState1 = 0;
unsigned int COffsets::FUNC_GetTrafficLightState2 = 0;
unsigned int COffsets::VAR_CurrentAreaName = 0;
unsigned int COffsets::VAR_CurrentStreetName = 0;
unsigned int COffsets::VAR_NumLoadingScreens = 0;
unsigned int COffsets::VAR_FirstLoadingScreenDuration = 0;
unsigned int COffsets::VAR_FirstLoadingScreenType = 0;
unsigned int COffsets::VAR_HWnd = 0;
unsigned int COffsets::VAR_Time = 0;

// Input
unsigned int COffsets::VAR_Pads = 0;
unsigned int COffsets::FUNC_CPad__Constructor = 0;
unsigned int COffsets::FUNC_CPad__Initialize = 0;
unsigned int COffsets::FUNC_CPad__Destructor = 0;

// Key Sync
unsigned int COffsets::VAR_CPlayerPed__VFTable = 0;
unsigned int COffsets::VAR_CAutomobile__VFTable = 0;
unsigned int COffsets::VAR_CBike__VFTable = 0;
unsigned int COffsets::VAR_CBoat__VFTable = 0;
unsigned int COffsets::VAR_CTrain__VFTable = 0;
unsigned int COffsets::VAR_CHeli__VFTable = 0;
unsigned int COffsets::VAR_CPlane__VFTable = 0;
unsigned int COffsets::FUNC_CPlayerPed__ProcessInput = 0;
unsigned int COffsets::FUNC_CAutomobile__ProcessInput = 0;
unsigned int COffsets::FUNC_CBike__ProcessInput = 0;
unsigned int COffsets::FUNC_CBoat__ProcessInput = 0;
unsigned int COffsets::FUNC_CTrain__ProcessInput = 0;
unsigned int COffsets::FUNC_CHeli__ProcessInput = 0;
unsigned int COffsets::FUNC_CPlane__ProcessInput = 0;

// Checkpoints
unsigned int COffsets::VAR_RenderCheckpoints_FirstCP = 0;
unsigned int COffsets::VAR_RenderCheckpoints_LastCP = 0;

// Pools
unsigned int COffsets::VAR_PedPool = 0;
unsigned int COffsets::VAR_VehiclePool = 0;
unsigned int COffsets::VAR_PtrNodeSinglePool = 0;
unsigned int COffsets::VAR_PtrNodeDoublePool = 0;
unsigned int COffsets::VAR_EntryInfoNodesPool = 0;
unsigned int COffsets::VAR_AnimatedBuildingPool = 0;
unsigned int COffsets::VAR_BuildingPool = 0;
unsigned int COffsets::VAR_ObjectPool = 0;
unsigned int COffsets::VAR_DummyObjectPool = 0;
unsigned int COffsets::VAR_InteriorInstPool = 0;
unsigned int COffsets::VAR_PortalInstPool = 0;
unsigned int COffsets::VAR_AnimBlenderPool = 0;
unsigned int COffsets::VAR_AtdNodeAnimChangePooledObjectPool = 0;
unsigned int COffsets::VAR_AtdNodeAnimPlayerPool = 0;
unsigned int COffsets::VAR_crFrameFilterBoneMaskPool = 0;
unsigned int COffsets::VAR_crFrameFilterBoneAnaloguePool = 0;
unsigned int COffsets::VAR_crExpressionProcessorPool = 0;
unsigned int COffsets::VAR_crmtObserverPool = 0;
unsigned int COffsets::VAR_TaskPool = 0;
unsigned int COffsets::VAR_DummyTaskPool = 0;
unsigned int COffsets::VAR_EventPool = 0;
unsigned int COffsets::VAR_PointRoutePool = 0;
unsigned int COffsets::VAR_PatrolRoutePool = 0;
unsigned int COffsets::VAR_NavMeshRoutePool = 0;
unsigned int COffsets::VAR_CamPool = 0;
unsigned int COffsets::VAR_ExplosionTypePool = 0;
unsigned int COffsets::VAR_PedMoveBlendPool = 0;
unsigned int COffsets::VAR_ParticleSystemPool = 0;
unsigned int COffsets::VAR_VehicleStructPool = 0;
unsigned int COffsets::VAR_PedBasePool = 0;
unsigned int COffsets::VAR_TaskInfoPool = 0;
unsigned int COffsets::VAR_PedAttractorPool = 0;
unsigned int COffsets::VAR_TargettingPool = 0;
unsigned int COffsets::FUNC_CPool__Allocate = 0;
unsigned int COffsets::FUNC_CPool__Release = 0;
unsigned int COffsets::FUNC_CPool__AtHandle = 0;
unsigned int COffsets::FUNC_CPool__HandleOf = 0;
unsigned int COffsets::FUNC_CPlayer__GetPlayerByNumber = 0;
unsigned int COffsets::VAR_PlayerInfoArray = 0;
unsigned int COffsets::VAR_LocalPlayerId = 0;

// Physical
unsigned int COffsets::FUNC_CPhysical__SetMoveSpeed = 0;
unsigned int COffsets::FUNC_CPhysical__GetMoveSpeed = 0;
unsigned int COffsets::FUNC_CPhysical__SetTurnSpeed = 0;
unsigned int COffsets::FUNC_CPhysical__GetTurnSpeed = 0;

// Indicators
unsigned int COffsets::PATCH_CVehicle__HazzardLightsOn = 0;
unsigned int COffsets::FUNC_CVehicle__DrawIndicator = 0;
unsigned int COffsets::RETURN_CVehicle__DrawIndicator = 0;

// Streaming
unsigned int COffsets::VAR_ResourceTypeWdrIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWadIndex = 0;
unsigned int COffsets::VAR_ResourceTypeScoIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWtdIndex = 0;
unsigned int COffsets::VAR_ResourceTypeCutIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWddIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWnvIndex = 0;
unsigned int COffsets::VAR_ResourceTypeRrrIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWbdIndex = 0;
unsigned int COffsets::VAR_ResourceTypeIplIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWbnIndex = 0;
unsigned int COffsets::VAR_ResourceTypeWbsIndex = 0;
unsigned int COffsets::FUNC_RequestResource = 0;
unsigned int COffsets::FUNC_LoadAllResources = 0;
unsigned int COffsets::FUNC_HasResourceLoaded = 0;
unsigned int COffsets::FUNC_ReleaseResource = 0;
unsigned int COffsets::FUNC_GetModelIndexFromHash = 0;
unsigned int COffsets::FUNC_CAnimStore__GetIndexFromName = 0;
unsigned int COffsets::FUNC_LoadWorldAtPosition = 0;

// Ped
unsigned int COffsets::FUNC_CPed__SetDucking = 0;
unsigned int COffsets::FUNC_CPed__IsDucking = 0;
unsigned int COffsets::FUNC_CPed__DisableRagdoll = 0;

// Tasks
unsigned int COffsets::FUNC_CPedTaskManager__SetTaskPriority = 0;
unsigned int COffsets::FUNC_CPedTaskManager__SetTaskSecondary = 0;
unsigned int COffsets::FUNC_CPedTaskManager__SetTaskMovement = 0;
unsigned int COffsets::VAR_CTask__VFTable = 0;
unsigned int COffsets::RETURN_CTask__Destructor = 0;
unsigned int COffsets::FUNC_CTaskComplexNewGetInVehicle__Constructor = 0;
unsigned int COffsets::FUNC_CTaskComplexNewExitVehicle__Constructor = 0;
unsigned int COffsets::FUNC_CTaskComplexDie__Constructor = 0;
unsigned int COffsets::FUNC_CTaskSimpleDead__Constructor = 0;
unsigned int COffsets::FUNC_CTaskSimpleCarSetPedInVehicle__Constructor = 0;
unsigned int COffsets::FUNC_CTaskSimpleCarSetPedOut__Constructor = 0;
unsigned int COffsets::FUNC_CTaskSimpleStartWalking__Constructor = 0;
unsigned int COffsets::FUNC_CTaskSimpleStopWalking__Constructor = 0;

// Weapons
unsigned int COffsets::FUNC_CPedWeapons__RemoveWeapon = 0;
unsigned int COffsets::FUNC_CPedWeapons__RemoveAllWeapons = 0;

void COffsets::Init(unsigned int uiBase)
{
	m_GameVersion = GAME_VERSION_UNKNOWN;
	unsigned int * uiTestAddr = (unsigned int *)(uiBase + 0x608C34);

	switch(*uiTestAddr)
	{
		// 1.0.7.0
		case 0x1006E857:
			m_GameVersion = GAME_VERSION_7;

			// Player
			FUNC_CreatePlayerPed                            = (uiBase + 0x81CB90);
			FUNC_SetupPedIntelligence                       = (uiBase + 0x89EC20);
			FUNC_CPlayerPed__SetModelIndex                  = (uiBase + 0x9C0AA0);

			// Game
			VAR_MenuActive_1                                = (uiBase + 0x10C7F80);
			VAR_MenuActive_2                                = (uiBase + 0x10C7F6F);

			// Script
			FUNC_GetRunningScriptThread                     = (uiBase +  0x5A4CA0);
			CALL_CScriptVM__Process                         = (uiBase +  0x80A092);
			VAR_ScrVM__ThreadPool                           = (uiBase + 0x1983310);
			VAR_ScrVM__ActiveThread                         = (uiBase + 0x1849AE0);
			FUNC_ScrVM__FindNativeAddress                   = (uiBase +  0x5A76D0);
			VAR_ScrVM__GlobalVariablesPool                  = (uiBase + 0x1849AEC);

			// Weather
			FUNC_SetWeather                                 = (uiBase +  0x9E41D0);
			VAR_CurrentWeather                              = (uiBase + 0x1639524);

			// Time
			FUNC_SetTimeOfDay                               = (uiBase +  0x821200);
			VAR_TimeOfDay                                   = (uiBase + 0x11E8F0C); // Poor Naming (Could be some member of CClock)
			VAR_CClock__Hour                                = (uiBase + 0x11D5300);
			VAR_CClock__Minute                              = (uiBase + 0x11D52FC);
			VAR_CClock__LockedHour                          = (uiBase + 0x11D5308);
			VAR_CClock__LockedMinute                        = (uiBase + 0x11D52F0);
			VAR_CClock__DayOfWeek                           = (uiBase + 0x11D5310);

			// HUD
			VAR_HudEnabled                                  = (uiBase + 0x118034C);
			VAR_RadarVisible                                = (uiBase + 0x118034F);
			VAR_PlayerNamesVisible                          = (uiBase + 0x1967DD8);
			VAR_AreaNamesDisabled                           = (uiBase + 0x118F0D2);

			// World
			FUNC_CWorld__AddEntity                          = (uiBase + 0x817350);
			FUNC_CWorld__RemoveEntity                       = (uiBase + 0x8173C0);
			FUNC_CEntity__IsTouchingEntity                  = (uiBase + 0xA47B30);
			FUNC_CMatrix__ConvertFromEulerAngles            = (uiBase + 0x42F8A0);
			FUNC_CMatrix__ConvertFromQuaternion             = (uiBase + 0x5BE050);
			FUNC_CQuaternion__ConvertFromMatrix             = (uiBase + 0x506DB0);

			// Vehicle
			FUNC_CVehicle__TurnEngineOn						= (uiBase + 0x9D3600);
			FUNC_CVehicle__TurnEngineOff                    = (uiBase + 0x9C6710);
			FUNC_CVehicle__RefreshColours                   = (uiBase + 0x9C52E0);

			// Indicators
			PATCH_CVehicle__HazzardLightsOn                 = (uiBase +  0x9D1D53);
			FUNC_CVehicle__DrawIndicator                    = (uiBase +  0x9CD0F0);
			RETURN_CVehicle__DrawIndicator                  = (uiBase +  0x9CD0F6);

			// Misc
			VAR_CurrentWantedLevel                          = (uiBase + 0x118F0DC); // Poor Naming (Could be some member of CWanted)
			VAR_ScrollBarText                               = (uiBase + 0x15F2228);
			VAR_ScrollBarColor                              = (uiBase +  0x972FFD);
			VAR_CameraManager                               = (uiBase +  0xF21A6C);
			VAR_LocalPlayerState                            = (uiBase + 0x10F8068); // 0 = alive, 1 = dead(?), 2 = being arrested(?), 5 = respawning
			FUNC_GetLocalPlayerSpawnPosition                = (uiBase +  0x7B84E2);
			CALL_SpawnLocalPlayer                           = (uiBase +  0x7B8592);
			FUNC_SpawnPlayer                                = (uiBase +  0x7B7B40);
			FUNC_GetTrafficLightState1                      = (uiBase +  0x88C6F0);
			FUNC_GetTrafficLightState2                      = (uiBase +  0x88C750);
			VAR_CurrentAreaName                             = (uiBase + 0x11DE0DE);
			VAR_CurrentStreetName                           = (uiBase + 0x11DE260);
			VAR_NumLoadingScreens                           = (uiBase + 0x18A8258);
			VAR_FirstLoadingScreenDuration                  = (uiBase + 0x18A8F40);
			VAR_FirstLoadingScreenType                      = (uiBase + 0x18A8F48);
			VAR_HWnd                                        = (uiBase + 0x1849DDC);
			VAR_Time                                        = (uiBase + 0x11DDE74);

			// Input
			VAR_Pads                                        = (uiBase + 0x10FB818);
			FUNC_CPad__Constructor                          = (uiBase +  0x834180);
			FUNC_CPad__Initialize                           = (uiBase +  0x8395B0);
			FUNC_CPad__Destructor                           = (uiBase +  0x7C3680);

			// Key Sync
			VAR_CPlayerPed__VFTable                         = (uiBase +  0xD819C4);
			VAR_CAutomobile__VFTable                        = (uiBase +  0xDB3C3C);
			VAR_CBike__VFTable                              = (uiBase +  0xDB4064);
			VAR_CBoat__VFTable                              = (uiBase +  0xDCA0E4);
			VAR_CTrain__VFTable                             = (uiBase +  0xD78D4C);
			VAR_CHeli__VFTable                              = (uiBase +  0xD6B1CC);
			VAR_CPlane__VFTable                             = (uiBase +  0xD86104);
			FUNC_CPlayerPed__ProcessInput                   = (uiBase +  0x9C0B00);
			FUNC_CAutomobile__ProcessInput                  = (uiBase +  0xBFE870);
			FUNC_CBike__ProcessInput                        = (uiBase +  0xC05C30);
			FUNC_CBoat__ProcessInput                        = (uiBase +  0xCA8C60);
			FUNC_CTrain__ProcessInput                       = (uiBase +  0x94CE70);
			FUNC_CHeli__ProcessInput                        = (uiBase +  0x8B9290);
			FUNC_CPlane__ProcessInput                       = (uiBase +  0xA1B080);

			// Checkpoints
			VAR_RenderCheckpoints_FirstCP                   = (uiBase +  0x855966);
			VAR_RenderCheckpoints_LastCP                    = (uiBase +  0x856705);

			// Pools
			VAR_PedPool                                     = (uiBase + 0x18A82AC);
			VAR_VehiclePool                                 = (uiBase + 0x1619240);
			VAR_PtrNodeSinglePool                           = (uiBase + 0x16B7758);
			VAR_PtrNodeDoublePool                           = (uiBase + 0x16B775C);
			VAR_EntryInfoNodesPool                          = (uiBase + 0x1706E98);
			VAR_AnimatedBuildingPool                        = (uiBase + 0x1706E94);
			VAR_BuildingPool                                = (uiBase + 0x168FED0);
			VAR_ObjectPool                                  = (uiBase + 0x1350CE0);
			VAR_DummyObjectPool                             = (uiBase + 0x168FF90);
			VAR_InteriorInstPool                            = (uiBase + 0x165D2E4);
			VAR_PortalInstPool                              = (uiBase + 0x16C3364);
			VAR_AnimBlenderPool                             = (uiBase + 0x16397B0);
			VAR_AtdNodeAnimChangePooledObjectPool           = (uiBase + 0x16397B8);
			VAR_AtdNodeAnimPlayerPool                       = (uiBase + 0x16397B4);
			VAR_crFrameFilterBoneMaskPool                   = (uiBase + 0x16397BC);
			VAR_crFrameFilterBoneAnaloguePool               = (uiBase + 0x16397C0);
			VAR_crExpressionProcessorPool                   = (uiBase + 0x16397C4);
			VAR_crmtObserverPool                            = (uiBase + 0x16397C8);
			VAR_TaskPool                                    = (uiBase + 0x164B01C);
			VAR_DummyTaskPool                               = (uiBase + 0x1706E8C);
			VAR_EventPool                                   = (uiBase + 0x152F4B4);
			VAR_PointRoutePool                              = (uiBase + 0x1706E80);
			VAR_PatrolRoutePool                             = (uiBase + 0x1706E88);
			VAR_NavMeshRoutePool                            = (uiBase + 0x1706E84);
			VAR_CamPool                                     = (uiBase + 0x1618020);
			VAR_ExplosionTypePool                           = (uiBase + 0x1706E78);
			VAR_PedMoveBlendPool                            = (uiBase + 0x18A82B4);
			VAR_ParticleSystemPool                          = (uiBase + 0x169F4C4);
			VAR_VehicleStructPool                           = (uiBase + 0x16D6594);
			VAR_PedBasePool                                 = (uiBase + 0x18A82B8);
			VAR_TaskInfoPool                                = (uiBase + 0x1618040);
			VAR_PedAttractorPool                            = (uiBase + 0x17168BC);
			VAR_TargettingPool                              = (uiBase + 0x1711134);
			FUNC_CPool__Allocate                            = (uiBase +  0x439CB0);
			FUNC_CPool__Release                             = (uiBase +  0x448300);
			FUNC_CPool__AtHandle                            = (uiBase +  0x426700);
			FUNC_CPool__HandleOf                            = (uiBase +  0x447230);
			FUNC_CPlayer__GetPlayerByNumber                 = (uiBase +  0x817F20);
			VAR_PlayerInfoArray                             = (uiBase + 0x11A7008);
			VAR_LocalPlayerId                               = (uiBase +  0xF1CC68);

			// Physical
			FUNC_CPhysical__SetMoveSpeed                    = (uiBase +  0xA47750);
			FUNC_CPhysical__GetMoveSpeed                    = (uiBase +  0xA477F0);
			FUNC_CPhysical__SetTurnSpeed                    = (uiBase +  0xA47840);
			FUNC_CPhysical__GetTurnSpeed                    = (uiBase +  0xA478E0);

			// Streaming
			VAR_ResourceTypeWdrIndex                        = (uiBase + 0x15F73A0);
			VAR_ResourceTypeWadIndex                        = (uiBase +  0xF27324);
			VAR_ResourceTypeScoIndex                        = (uiBase +  0xF3E28C);
			VAR_ResourceTypeWtdIndex                        = (uiBase +  0xF1CD84);
			VAR_ResourceTypeCutIndex                        = (uiBase +  0xF1EFC0);
			VAR_ResourceTypeWddIndex                        = (uiBase +  0xF272E4);
			VAR_ResourceTypeWnvIndex                        = (uiBase +  0xF2A0C4);
			VAR_ResourceTypeRrrIndex                        = (uiBase + 0x15DAEB0);
			VAR_ResourceTypeWbdIndex                        = (uiBase +  0xF2AAA0);
			VAR_ResourceTypeIplIndex                        = (uiBase +  0xF3BE40);
			VAR_ResourceTypeWbnIndex                        = (uiBase +  0xF3F224);
			VAR_ResourceTypeWbsIndex                        = (uiBase +  0xF412E0);
			FUNC_RequestResource                            = (uiBase +  0x832C40);
			FUNC_LoadAllResources                           = (uiBase +  0x832C20);
			FUNC_HasResourceLoaded                          = (uiBase +  0x832DD0);
			FUNC_ReleaseResource                            = (uiBase +  0x832B60);
			FUNC_GetModelIndexFromHash                      = (uiBase +  0x98AAE0);
			FUNC_CAnimStore__GetIndexFromName               = (uiBase +  0x8631E0);
			FUNC_LoadWorldAtPosition                        = (uiBase +  0xB2BBA0);

			// Ped
			FUNC_CPed__SetDucking                           = (uiBase +  0x8A70C0);
			FUNC_CPed__IsDucking                            = (uiBase +  0x89C780);
			FUNC_CPed__DisableRagdoll                       = (uiBase +  0x8A6910);

			// Tasks
			FUNC_CPedTaskManager__SetTaskPriority           = (uiBase +  0x9E58B0);
			FUNC_CPedTaskManager__SetTaskSecondary          = (uiBase +  0x9E5AC0);
			FUNC_CPedTaskManager__SetTaskMovement           = (uiBase +  0x9E5E20);
			VAR_CTask__VFTable                              = (uiBase +  0xD87224);
			RETURN_CTask__Destructor                        = (uiBase +  0xA288DA);
			FUNC_CTaskComplexNewGetInVehicle__Constructor   = (uiBase +  0xA2BCF0);
			FUNC_CTaskComplexNewExitVehicle__Constructor    = (uiBase +  0xA2C920);
			FUNC_CTaskComplexDie__Constructor               = (uiBase +  0xABC6C0);
			FUNC_CTaskSimpleDead__Constructor               = (uiBase +  0xABCF10);
			FUNC_CTaskSimpleCarSetPedInVehicle__Constructor = (uiBase +  0xA9CB60);
			FUNC_CTaskSimpleCarSetPedOut__Constructor       = (uiBase +  0xA9CFB0);
			FUNC_CTaskSimpleStartWalking__Constructor		= (uiBase +  0xA0E310); // 0xA1D830 // 0xA0DFF0
			FUNC_CTaskSimpleStopWalking__Constructor		= (uiBase +  0xA1D730);

			// aim task simple constructor = 0xA5FD80|native address = 0xB89090(param size 5)|static task select id = 35
			// aim and walk simple constructor = 0xA0E630|native address = 0xB89F50(param size 12)|static task select id = 47
			// shot task simple constructor = 0xA5FD80|native address = 0xB89140(param size 6)|static task select id = 36
			// shot and walk simple constructor = 0xA0E710|native address = 0xB88F30(paran size 9)|static task select id = 33

			// Weapons
			FUNC_CPedWeapons__RemoveWeapon                  = (uiBase +  0x9ABFD0);
			FUNC_CPedWeapons__RemoveAllWeapons              = (uiBase +  0x9AC040);
			break;
	}
}
