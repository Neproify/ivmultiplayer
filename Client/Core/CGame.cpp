//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGame.cpp
// Project: Client.Core
// Author(s): jenksta
//            aru
//            listener
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==============================================================================

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "CGame.h"
#include <CLogFile.h>
#include "Patcher/CPatcher.h"
#include "Scripting.h"
#include "CGUI.h"
#include "CChatWindow.h"
#include "CInputWindow.h"
#include "KeySync.h"
#include "AimSync.h"
#include "Indicators.h"
#include "TrafficLights.h"
#include "ScriptHook.h"
#include "COffsets.h"
#include "CPools.h"
#include "CClientTaskManager.h"
#include "CCredits.h"
#include "CContextDataManager.h"
#include "CAudio.h"
#include "AimSync.h"
#include "CMainMenu.h"

extern CChatWindow        * g_pChatWindow;
extern CInputWindow       * g_pInputWindow;
extern CClientTaskManager * g_pClientTaskManager;
extern CCredits           * g_pCredits;
extern CMainMenu		  * g_pMainMenu;

unsigned int   CGame::m_uiBaseAddress = 0;
bool           CGame::m_bInputState = false;
eState         CGame::m_State = GAME_STATE_NONE;
bool           CGame::m_bFocused = false;
CPools       * CGame::m_pPools = NULL;
CIVPad       * CGame::m_pPad = NULL;
CIVModelInfo   CGame::m_modelInfos[NUM_ModelInfos];
CIVWeaponInfo  CGame::m_weaponInfos[NUM_WeaponInfos];
CIVStreaming * CGame::m_pStreaming = NULL;
CIVWeather   * CGame::m_pWeather = NULL;
CIVWorld     * CGame::m_pWorld = NULL;
bool		   CGame::m_bNameTags = false;
bool		   CGame::m_bGameLoaded = false;
bool		   CGame::m_bHeadMovement = true;

void CGame::SetFocused(bool bFocused)
{
	// Set the focused flag
	m_bFocused = bFocused;

	if(bFocused) // We have focus
	{
		// Enable input
		SetInputState(true);
		CAudioManager::RestoreAllVolume();

		if(g_pMainMenu)
			g_pMainMenu->HideLoadingScreen();
	}
	else // We do not have focus
	{
		// Disable input
		SetInputState(false);
		CAudioManager::SetAllVolume(0.0f);
	}
}

CIVModelInfo * CGame::GetModelInfo(int iModelIndex)
{
	if(iModelIndex < NUM_ModelInfos && iModelIndex >= 0 && m_modelInfos[iModelIndex].IsValid())
		return &m_modelInfos[iModelIndex];

	return NULL;
}

unsigned int CGame::GetLoadedModelCount()
{
	unsigned int uiCount = 0;

	for(int i = 0; i < NUM_ModelInfos; i++)
	{
		if(m_modelInfos[i].IsValid() && m_modelInfos[i].IsLoaded())
			uiCount++;
	}

	return uiCount;
}

unsigned int CGame::GetUnloadedModelCount()
{
	unsigned int uiCount = 0;

	for(int i = 0; i < NUM_ModelInfos; i++)
	{
		if(m_modelInfos[i].IsValid() && !m_modelInfos[i].IsLoaded())
			uiCount++;
	}

	return uiCount;
}

CIVWeaponInfo * CGame::GetWeaponInfo(eWeaponType weaponType)
{
	if(weaponType < NUM_WeaponInfos && weaponType >= 0)
		return &m_weaponInfos[weaponType];

	return NULL;
}

HWND CGame::GetHWnd()
{
	return *(HWND *)COffsets::VAR_HWnd;
}

DWORD CGame::GetTime()
{
	return *(DWORD *)COffsets::VAR_Time;
}

void CGame::Initialize()
{
	// Ensure all game structures are correctly aligned
	assert(sizeof(IVCamData) == 0x74);
	assert(sizeof(IVCam) == 0x140);
	assert(sizeof(IVCheckpoint) == 0x30);
	assert(sizeof(IVEntity) == 0x74);
	assert(sizeof(IVDynamicEntity) == 0x10C);
	//assert(sizeof(IVBaseModelInfo) == 0x0); // TODO
	assert(sizeof(IVPadConfig) == 0x7B8);
	assert(sizeof(IVPadDataHistoryItem) == 0x8);
	assert(sizeof(IVPadDataHistory) == 0x200);
	assert(sizeof(IVPadData) == 0x10);
	assert(sizeof(IVPad) == 0x3A84);
	//assert(sizeof(IVPedTaskManager) == 0x0); // TODO
	assert(sizeof(IVPedWeaponSlot) == 0xC);
	assert(sizeof(IVPedWeapons) == 0x11A);
	assert(sizeof(IVPhysical) == 0x210);
	assert(sizeof(IVPed) == 0xF00);
	assert(sizeof(IVPlayerInfo) == 0x5C0);
	assert(sizeof(IVPlayerPed) == 0xF00);
	assert(sizeof(IVPool) == 0x1C);
	//assert(sizeof(IVTask) == 0x0); // TODO
	assert(sizeof(IVVehicle) == 0x20D0);
	assert(sizeof(IVWeaponInfo) == 0x110);
	assert(sizeof(CSimpleCollection<void>) == 0x8);

	// Get the process base address
	m_uiBaseAddress = (unsigned int)GetModuleHandle(NULL);

	if(m_uiBaseAddress == 0)
	{
		CLogFile::Printf("Invalid Game Base Detected. Exiting...");
		ExitProcess(0);
	}

	CLogFile::Printf("Game Base: 0x%p (0x%p)", m_uiBaseAddress, (m_uiBaseAddress - 0x400000));
	m_uiBaseAddress -= 0x400000;
	COffsets::Init(m_uiBaseAddress);

	if(COffsets::GetVersion() == GAME_VERSION_UNKNOWN)
	{
		MessageBox(NULL, "Unknown game version detected. IV:MP currently only supports version 1.0.7.0", "Unknown game version", NULL);
		ExitProcess(0);
	}

	if(Patch())
	{
		CLogFile::Printf("Applied patches");
		InstallKeySyncHooks();
		InstallAimSyncHooks();
		CLogFile::Printf("Applied key sync/aim sync hooks");

		InstallIndicatorHooks();
		InstallTrafficLightHooks();
		CLogFile::Printf("Applied misc hooks");

		InstallScriptHooks();
		CLogFile::Printf("Applied script hooks");
	}
	else
	{
		MessageBox(NULL, "Failed to apply patches. IV:MP needs to apply certain patches to run", "Failed to apply patches", NULL);
		ExitProcess(0);
	}

	// Replace "Loading .." with "IV:MP is loading...", some user say that the label sometimes is shown
	Scripting::SetMsgForLoadingScreen("IV:MP is loading....");

	SetInputState(true);
	SetState(GAME_STATE_NONE);

	// Create our pools class
	m_pPools = new CPools();

	// Create the pad instance
	m_pPad = new CIVPad((IVPad *)COffsets::VAR_Pads);

	// Initialize our game classes
	m_pStreaming = new CIVStreaming();
	m_pWeather = new CIVWeather();
	m_pWorld = new CIVWorld();

	// Initialize the model infos
	for(int i = 0; i < NUM_ModelInfos; i++)
		m_modelInfos[i].SetIndex(i);

	// Disable invalid models
	m_modelInfos[125].SetIndex(-1); // Ingot (FIX)
	m_modelInfos[180].SetIndex(-1); // Uranus (FIX)
	m_modelInfos[191].SetIndex(-1); // Hellfury (FIX)
	m_modelInfos[195].SetIndex(-1); // Zombieb (FIX)

	// Initialize the weapon infos
	for(int i = 0; i < NUM_WeaponInfos; i++)
	{
		// TODO: Make SetType update the weapon info pointer?
		m_weaponInfos[i].SetType((eWeaponType)i);
		m_weaponInfos[i].SetWeaponInfo((IVWeaponInfo *)((GetBase() + ARRAY_WeaponInfos) + (i * sizeof(IVWeaponInfo))));
	}
}

void CGame::Shutdown()
{
	// Shutdown our game class instances
	SAFE_DELETE(m_pWorld);
	SAFE_DELETE(m_pWeather);
	SAFE_DELETE(m_pStreaming);
	SAFE_DELETE(m_pPad);

	// Delete our pools instance
	SAFE_DELETE(m_pPools);

	// Uninstall the script hooks
	UninstallScriptHooks();
}

// thx listener
void UnprotectMemory()
{
	BYTE * pImageBase = (BYTE *)(CGame::GetBase() + 0x400000);
	PIMAGE_DOS_HEADER   pDosHeader = (PIMAGE_DOS_HEADER)pImageBase;
	PIMAGE_NT_HEADERS   pNtHeader  = (PIMAGE_NT_HEADERS)(pImageBase + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHeader);

	for(int iSection = 0; iSection < pNtHeader->FileHeader.NumberOfSections; iSection++, pSection++)
	{
		char * pszSectionName = (char *)pSection->Name;

		if(!strcmp(pszSectionName, ".text") || !strcmp(pszSectionName, ".rdata"))
			CPatcher::Unprotect((DWORD)(pImageBase + pSection->VirtualAddress), ((pSection->Misc.VirtualSize + 4095) & ~4095));
	}
}

IVTask * ___pTask = NULL;

void _declspec(naked) CTask__Destructor_Hook()
{
	_asm
	{
		mov ___pTask, ecx
		pushad
	}


	// Do we have a client task manager?
	if(g_pClientTaskManager)
	{
		// Let the client task manager handle this task deletion
		g_pClientTaskManager->HandleTaskDelete(___pTask);
	}

	_asm
	{
		popad
		push esi
		mov esi, ecx
		push esi
		mov dword ptr [esi], offset COffsets::VAR_CTask__VFTable
		jmp COffsets::RETURN_CTask__Destructor
	}
}

void _declspec(naked) CEpisodes__IsEpisodeAvaliable_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		test eax, eax
		jnz it_not
		mov al, 1
		retn 4
it_not:
		xor al, al
		retn 4
	}
}

unsigned int          uiPlayerInfoIndex = 0;
IVPlayerInfo        * pReturnedPlayerInfo = NULL;
unsigned int          uiReturnedIndex = 0;
IVPlayerPed         * _pPlayerPed = NULL;

IVPlayerInfo * GetPlayerInfoFromIndex(unsigned int uiIndex)
{
	// Default to the local player info just incase the index is invalid
	pReturnedPlayerInfo = CGame::GetPools()->GetPlayerInfoFromIndex(0);

	// Is this not the local player info?
	if(uiIndex != 0)
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(uiIndex);

		if(pContextInfo)
			pReturnedPlayerInfo = pContextInfo->GetPlayerInfo()->GetPlayerInfo();
	}
	
	return pReturnedPlayerInfo;
}

void _declspec(naked) GetPlayerInfoFromIndex_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov uiPlayerInfoIndex, eax
		pushad
	}

	GetPlayerInfoFromIndex(uiPlayerInfoIndex);

	_asm
	{
		popad
		mov eax, pReturnedPlayerInfo
		retn
	}
}

unsigned int GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Default to the local player info just incase the player info is invalid
	uiReturnedIndex = 0;

	// Is this not the local player info?
	if(pPlayerInfo != CGame::GetPools()->GetPlayerInfoFromIndex(0))
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(pPlayerInfo);

		if(pContextInfo)
			uiReturnedIndex = pContextInfo->GetPlayerInfo()->GetPlayerNumber();
	}

	return uiReturnedIndex;
}

void _declspec(naked) GetIndexFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetIndexFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, uiReturnedIndex
		retn
	}
}

bool bInvalidIndex = false;

IVPlayerPed * GetLocalPlayerPed()
{
	// Default to the local player ped (If available)
	IVPlayerInfo * pPlayerInfo = CGame::GetPools()->GetPlayerInfoFromIndex(0);

	if(pPlayerInfo)
		_pPlayerPed = pPlayerInfo->m_pPlayerPed;
	else
		_pPlayerPed = NULL;

	// Is the local player id valid?
	if(CGame::GetPools()->GetLocalPlayerIndex() != -1)
	{
		// Is the player index not the local player?
		if(CGame::GetPools()->GetLocalPlayerIndex() != 0)
		{
			// Get the context info for the player index
			CContextData * pContextInfo = CContextDataManager::GetContextData((BYTE)CGame::GetPools()->GetLocalPlayerIndex());

			// Is the context info valid?
			if(pContextInfo)
			{
				// Set the player ped to the remote player
				_pPlayerPed = pContextInfo->GetPlayerPed()->GetPlayerPed();
			}
		}
	}

	// Some code to test a theory
	if(_pPlayerPed == NULL)
	{
		if(!bInvalidIndex)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Invalid (Index is %d)", CGame::GetPools()->GetLocalPlayerIndex());
			bInvalidIndex = true;
		}
	}
	else
	{
		if(bInvalidIndex)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Now Valid");
			bInvalidIndex = false;
		}
	}

	return _pPlayerPed;
}

void _declspec(naked) GetLocalPlayerPed_Hook()
{
	_asm
	{
		pushad
	}

	GetLocalPlayerPed();

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

IVPlayerPed * GetPlayerPedFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Is the player info pointer valid?
	if(pPlayerInfo)
		_pPlayerPed = pPlayerInfo->m_pPlayerPed;
	else
	{
		// Player info pointer is invalid, use the local player ped
		_pPlayerPed = GetLocalPlayerPed();
	}

	return _pPlayerPed;
}

void _declspec(naked) GetPlayerPedFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetPlayerPedFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

DWORD dwAddJump;
void _declspec(naked) CameraPointAtCoord(CVector3 * vecLookAt)
{
	_asm 
	{
		push ebp
		mov ebp, esp
		mov eax, [ebp+0Ch]
		mov vecLookAt, eax
		pushad
	}

	dwAddJump = ((CGame::GetBase() +  0xAFE840) + 0x6 );

	_asm
	{
		popad
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		jmp dwAddJump
	}
}

bool CGame::Patch()
{
	// Unprotect .text and .rdata memory and leave it unprotected
	UnprotectMemory();

	if(COffsets::GetVersion() == GAME_VERSION_7)
	{
		// Return at start of CTaskSimplePlayRandomAmbients::ProcessPed (Disable random amient animations)
		// NOTE: This also disables ambient head movements and maybe some other stuff we actually want
		*(DWORD *)(GetBase() + 0x9849F0) = 0x900004C2;

		// Hook GetPlayerInfoFromIndex to use our own function
		CPatcher::InstallJmpPatch((GetBase() + 0x817F20), (DWORD)GetPlayerInfoFromIndex_Hook);

		// Hook GetIndexFromPlayerInfo to use our own function
		CPatcher::InstallJmpPatch((GetBase() + 0x817DC0), (DWORD)GetIndexFromPlayerInfo_Hook);

		// Hook GetLocalPlayerPed to use our own function
		CPatcher::InstallJmpPatch((GetBase() + 0x817F40), (DWORD)GetLocalPlayerPed_Hook);

		// Skip main menu
		//CPatcher::InstallJmpPatch( (GetBase() + 0x402B3C), (GetBase() + 0x402C07));

		// Hook GetPlayerPedFromPlayerInfo to use our own function
		//CPatcher::InstallJmpPatch((GetBase() + 0x8788D0), (DWORD)GetPlayerPedFromPlayerInfo_Hook);

		// Hook CTask::~CTask to use our own function
		CPatcher::InstallJmpPatch((GetBase() + 0xA288D0), (DWORD)CTask__Destructor_Hook);

		// Hook CEpisodes::IsEpisodeAvaliable to use our own function
		CPatcher::InstallJmpPatch((GetBase() + 0x814810), (DWORD)CEpisodes__IsEpisodeAvaliable_Hook);

		// Force IsDLCReady to always return 0
		// jenksta: This caused startup issues
		//*(DWORD *)(GetBase() + 0x407320) = 90C3C032; // xor al, al; retn; nop

		// Disable call to CreateWebBrowser
		//CPatcher::InstallNopPatch((GetBase() + 0x42E91A), 5);

		// Disable call to CreateSocialWebBrowser
		//CPatcher::InstallNopPatch((GetBase() + 0x41F68E), 5);

		// Disable vehicle entries
		*(DWORD *)(GetBase() + 0x9B1ED0) = 0x0CC2C033;
		*(BYTE *)(GetBase() + 0x9B1ED4) = 0x00;

		// NOTE: When vehicle exits are disabled, pressing the exit vehicle key
		// still applies the vehicle handbrake, fix this asap
		// Disable vehicle exits
		*(BYTE *)(GetBase() + 0x9BBBFC) = 0xEB;

		// Make the game think we are not connected to the internet
		*(BYTE *)(GetBase() + 0x10F1390) = 0; // byteInternetConnectionState
		*(DWORD *)(GetBase() + 0x7AF1A0) = 0x90C3C032; // xor al, al; retn; nop

		// Disable the 'Loading...' text when the screen is faded
		// jenksta: Disabled this to see if it fixes some issues
		//CPatcher::InstallNopPatch((CGame::GetBase() + 0x7BC874), 5);
		//CPatcher::InstallJmpPatch(0x422CA7, (GetBase() + 0x422CAE));

		// Disable loading tune
		//CPatcher::InstallNopPatch(0x422A92, 10);

		// Don't let the game pause (Possibly use the gfwl multiplayer menu)
		//Scripting::DisablePauseMenu(true);

		// Don't load startup.sco
		// N:IV
		//*(BYTE *)(GetBase() + 0x809A81) = 0xEB;
		//*(BYTE *)(GetBase() + 0x809A82) = 0x61;
		// IV:MP
		*(BYTE *)(GetBase() + 0x809A8A) = 0x75;

		// Always start a new game
		// TODO: Change, this is messy
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x5B0311) = 0xEB; // jmp
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x5B0312) = 0xAE; // +AE
		// 0x5B0311 = 0xEB // jmp
		// 0x5B0312 = 0xAE // +AE
		CPatcher::InstallJmpPatch((GetBase() + 0x5B0311), (GetBase() + 0x5B03BF));

		// Disable parked cars
		CPatcher::InstallRetnPatch(GetBase() + 0xB3EDF0);

		// Disable emergency services and garbage trucks
		CPatcher::InstallNopPatch((GetBase() + 0x4215CF), 5);

		// Disable random peds and vehicles
		CPatcher::InstallNopPatch((GetBase() + 0x8ACD64), 5);

		// Disable scenario peds
		*(BYTE *)(GetBase() + 0x9F72C0) = 0xB8; // mov eax,
		*(DWORD *)(GetBase() + 0x9F72C1) = 0x0; // 0
		*(BYTE *)(GetBase() + 0x9F72C5) = 0xC3; // retn

		// Disable fake cars
		CPatcher::InstallRetnPatch(GetBase() + 0x9055D0);

		// Overwrite the ESP Check in the func responsible for loading load screens
		CPatcher::InstallCallPatch((GetBase() + 0x424B26), (DWORD)RemoveInitialLoadingScreens);

		// Disable sleep
		*(DWORD *)(GetBase() + 0x401835) = 1;

		// Hook for pointcamatcoord
		//CPatcher::InstallJmpPatch((GetBase() + 0xAFE840), (DWORD)CameraPointAtCoord);

		// Don't initialize error reporting
		CPatcher::InstallRetnPatch(GetBase() + 0xD356D0);

		// Certificates check (RETN 8)
		*(DWORD *)(GetBase() + 0x403F10) = 0x900008C2;

		// xor eax, eax - address of the RGSC object
		*(DWORD *)(GetBase() + 0x40262D) = 0x4AE9C033;

		// Skip RGSC connect and EFC checks (jmp 40289E)
		*(DWORD *)(GetBase() + 0x402631) = 0x90000002;

		// NOP; MOV [g_rgsc], eax
		*(WORD *)(GetBase() + 0x402883) = 0xA390;

		// Disable VDS100 error
		// TODO

		// Disable VDS101 error
		// TODO

		// Disable VDS102 error
		CPatcher::InstallNopPatch((GetBase() + 0x4028ED), 42);

		// Last RGSC init check (NOP*6)
		CPatcher::InstallNopPatch((GetBase() + 0x40291D), 6);

		// Skip missing tests
		CPatcher::InstallNopPatch((GetBase() + 0x402B12), 14);
		CPatcher::InstallNopPatch((GetBase() + 0x402D17), 14);
		*(DWORD *)(GetBase() + 0x403870) = 0x90CC033; // xor eax, eax; retn
		*(DWORD *)(GetBase() + 0x404250) = 0x90CC033; // xor eax, eax; retn

		// Disable securom spot checks (mov al, 1; retn)
		*(DWORD *)(GetBase() + 0xBAC160) = 0x90C301B0;
		*(DWORD *)(GetBase() + 0xBAC180) = 0x90C301B0;
		*(DWORD *)(GetBase() + 0xBAC190) = 0x90C301B0;
		*(DWORD *)(GetBase() + 0xBAC1C0) = 0x90C301B0;
		return true;
	}

	return false;
}

bool CGame::IsMenuActive()
{
	DWORD dwMenuActive1 = (COffsets::VAR_MenuActive_1);

	if(dwMenuActive1 == 2 || dwMenuActive1 == 6)
		return 0;

	return *(bool *)(COffsets::VAR_MenuActive_2);
}

void CGame::SetTime(int uHour, int uMinute)
{
	DWORD dwFunc = COffsets::FUNC_SetTimeOfDay;
	_asm
	{
		push -1
		push 0
		push uMinute
		push uHour
		call dwFunc
		add esp, 10h
	}
	*(DWORD *)(*(DWORD *)(COffsets::VAR_TimeOfDay) + 0x260) = 2;
}

void CGame::SetDayOfWeek(int uDay)
{
	*(DWORD *)(COffsets::VAR_CClock__DayOfWeek) = uDay;
}

void CGame::LockTime(int uHour, int uMinute)
{
	*(DWORD *)(COffsets::VAR_CClock__LockedHour) = uHour;
	*(DWORD *)(COffsets::VAR_CClock__LockedMinute) = uMinute;
}

void CGame::GetTime(int * uHour, int * uMinute)
{
	*uHour = *(DWORD *)(COffsets::VAR_CClock__LockedHour);

	if(*uHour == -1)
		*uHour = *(DWORD *)(COffsets::VAR_CClock__Hour);

	*uMinute = *(DWORD *)(COffsets::VAR_CClock__LockedMinute);

	if(*uMinute == -1)
		*uMinute = *(DWORD *)(COffsets::VAR_CClock__Minute);
}

void CGame::SetWantedLevel(DWORD dwLevel)
{
	*(DWORD *)(COffsets::VAR_CurrentWantedLevel) = dwLevel;
}

DWORD CGame::GetWantedLevel()
{
	return *(DWORD *)(COffsets::VAR_CurrentWantedLevel);
}

void CGame::SetHudVisible(bool bVisible)
{
	*(bool *)(COffsets::VAR_HudEnabled) = bVisible;
}

bool CGame::IsHudVisible()
{
	return *(bool *)COffsets::VAR_HudEnabled;
}

void CGame::SetRadarVisible(bool bVisible)
{
	*(bool *)(COffsets::VAR_RadarVisible) = bVisible;
}

bool CGame::IsRadarVisible()
{
	return *(bool *)COffsets::VAR_RadarVisible;
}

void CGame::SetPlayerNamesVisible(BYTE byteVisible)
{
	if(byteVisible < 2)
		*(BYTE *)(COffsets::VAR_PlayerNamesVisible) = byteVisible;
	else
		*(BYTE *)(COffsets::VAR_PlayerNamesVisible) = 0;
}

BYTE CGame::ArePlayerNamesVisible()
{
	return *(BYTE *)(COffsets::VAR_PlayerNamesVisible);
}

void CGame::SetAreaNamesEnabled(bool bEnabled)
{
	*(bool *)(COffsets::VAR_AreaNamesDisabled) = !bEnabled;
}

bool CGame::AreAreaNamesEnabled()
{
	return !(*(bool *)COffsets::VAR_AreaNamesDisabled);
}

/**@brief Get the matrix represented as euler angles around ZYX
   * @param yaw Yaw around X axis
   * @param pitch Pitch around Y axis
   * @param roll around X axis 
   * @param solution_number Which solution of two possible solutions ( 1 or 2) are possible values*/	
  /*void getEulerZYX(btScalar& yaw, btScalar& pitch, btScalar& roll, unsigned int solution_number = 1) const
  {
    struct Euler{btScalar yaw, pitch, roll;};
    Euler euler_out;
    Euler euler_out2; //second solution
    //get the pointer to the raw data
    
    // Check that pitch is not at a singularity
    if (btFabs(m_el[2].x()) >= 1)
    {
      euler_out.yaw = 0;
      euler_out2.yaw = 0;
	
      // From difference of angles formula
      btScalar delta = btAtan2(m_el[0].x(),m_el[0].z());
      if (m_el[2].x() > 0)  //gimbal locked up
      {
        euler_out.pitch = SIMD_PI / btScalar(2.0);
        euler_out2.pitch = SIMD_PI / btScalar(2.0);
        euler_out.roll = euler_out.pitch + delta;
        euler_out2.roll = euler_out.pitch + delta;
      }
      else // gimbal locked down
      {
        euler_out.pitch = -SIMD_PI / btScalar(2.0);
        euler_out2.pitch = -SIMD_PI / btScalar(2.0);
        euler_out.roll = -euler_out.pitch + delta;
        euler_out2.roll = -euler_out.pitch + delta;
      }
    }
    else
    {
      euler_out.pitch = - btAsin(m_el[2].x());
      euler_out2.pitch = SIMD_PI - euler_out.pitch;
	
      euler_out.roll = btAtan2(m_el[2].y()/btCos(euler_out.pitch), 
			       m_el[2].z()/btCos(euler_out.pitch));
      euler_out2.roll = btAtan2(m_el[2].y()/btCos(euler_out2.pitch), 
				m_el[2].z()/btCos(euler_out2.pitch));
	
      euler_out.yaw = btAtan2(m_el[1].x()/btCos(euler_out.pitch), 
			      m_el[0].x()/btCos(euler_out.pitch));
      euler_out2.yaw = btAtan2(m_el[1].x()/btCos(euler_out2.pitch), 
                               m_el[0].x()/btCos(euler_out2.pitch));
    }
    
    if (solution_number == 1)
    { 
		yaw = euler_out.yaw; 
		pitch = euler_out.pitch;
		roll = euler_out.roll;
    }
    else
    { 
		yaw = euler_out2.yaw; 
		pitch = euler_out2.pitch;
		roll = euler_out2.roll;
    }
  }*/

void CGame::ConvertRotationMatrixToEulerAngles(Matrix& matRotation, CVector3& vecRotation)
{
	// This function uses RADIANS not DEGREES
	// Matrix->XYZEuler
	// From http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl.html
	if(matRotation.vecRight.fZ < 1.0f)
	{
		if(matRotation.vecRight.fZ > -1.0f)
		{
			vecRotation = CVector3(atan2(-matRotation.vecForward.fZ, matRotation.vecUp.fZ), asin(matRotation.vecRight.fZ), 
				atan2(-matRotation.vecRight.fY, matRotation.vecRight.fX));
		}
		else
		{
			// WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
			vecRotation = CVector3(-atan2(matRotation.vecForward.fX, matRotation.vecForward.fY), -HALF_PI, 0.0f);
		}
	}
	else
	{
		// WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
		vecRotation = CVector3(atan2(matRotation.vecForward.fX, matRotation.vecForward.fY), HALF_PI, 0.0f);
	}
}

void CGame::ConvertEulerAnglesToRotationMatrix(CVector3 &vecRotation, Matrix &matRotation)
{
#if 0
	// This function uses RADIANS not DEGREES
	// XYZEuler(Pitch, Roll, Yaw)->Matrix // WAT?
	// From Ogre
	float fYAngle = 
	//
	float fCos, fSin;

	fCos = (float)cos(fYAngle);
	fSin = (float)sin(fYAngle);
	Matrix kXMat(CVector3(1.0,0.0,0.0),CVector3(0.0,fCos,-fSin),CVector3(0.0,fSin,fCos));

	fCos = (float)cos(fPAngle);
	fSin = (float)sin(fPAngle);
	Matrix kYMat(CVector3(fCos,0.0,fSin),CVector3(0.0,1.0,0.0),CVector3(-fSin,0.0,fCos));

	fCos = (float)cos(fRAngle);
	fSin = (float)sin(fRAngle);
	Matrix kZMat(CVector3(fCos,-fSin,0.0),CVector3(fSin,fCos,0.0),CVector3(0.0,0.0,1.0));

	/**this*/matRotation = kXMat*(kYMat*kZMat);
#endif
	// This function uses RADIANS not DEGREES
	// XYZEuler(Pitch, Roll, Yaw)->Matrix
	// Taken from the Quake3 source code
	// test, swap around if needed
	/*float yaw = vecRotation.fZ;
	float pitch = vecRotation.fX;
	float roll = vecRotation.fY;

	float sy = sin(yaw);
	float cy = cos(yaw);
	float sp = sin(pitch);
	float cp = cos(pitch);
	float sr = sin(roll);
	float cr = cos(roll);

	// for d3d
	matRotation.vecRight = CVector3(cp * cy, sr * sp * cy + cr * -sy, cr * sp * cy + -sr * -sy);
	// for gta swap the forward and up
	matRotation.vecForward = CVector3(cp * sy, sr * sp * sy + cr * cy, cr * sp * sy + -sr * cy);
	matRotation.vecUp = CVector3(-sp, sr * cp, cr * cp);*/
	Matrix34 matGameRotation;
	matGameRotation.FromMatrix(&matRotation);
	Matrix34 * pMatGameRotation = &matGameRotation;
	CVector3 * pVecGameRotation = &vecRotation;
	DWORD dwFunc = COffsets::FUNC_CMatrix__ConvertFromEulerAngles;
	_asm
	{
		push pVecGameRotation
		mov ecx, pMatGameRotation
		call dwFunc
	}
	matGameRotation.ToMatrix(&matRotation);
}

/*void CGame::ConvertQuaternionToRotationMatrix(Quaternion * quatRotation, GameMatrix34 * matRotation)
{
	DWORD dwFunc = COffsets::FUNC_CMatrix__ConvertFromQuaternion;
	_asm
	{
		push quatRotation
		mov ecx, matRotation
		call dwFunc
	}
}

void CGame::ConvertRotationMatrixToQuaternion(GameMatrix34 * matRotation, Quaternion * quatRotation)
{
	DWORD dwFunc = COffsets::FUNC_CQuaternion__ConvertFromMatrix;
	_asm
	{
		push matRotation
		mov ecx, quatRotation
		call dwFunc
	}
}*/

bool CGame::SetScrollBarText(unsigned char ucScrollbar, const char* szText)
{
	if(ucScrollbar > 7)
		return false;

	if(strlen(szText) >= 1299)
		return false;

	if(strlen(szText) == 0)
	{
		// Will throw a division by zero error if its length is zero, so use a pretty much empty string instead
		return SetScrollBarText(ucScrollbar, " ");
	}

	strncpy((char *)(COffsets::VAR_ScrollBarText + ucScrollbar * 1300), szText, strlen(szText) + 1);
	return true;
}

void CGame::ResetScrollBars(unsigned char ucScrollbar)
{
	// *financial
	if(ucScrollbar == 0 || ucScrollbar == 0xFF)
		SetScrollBarText(0, "BSHT+3%  CUBEL+2%  SPUK+1%  CABOX-2%  OOT+3%  ECLA-7%  BEAN-5%  LOGR+3%  RWDC-2%  CHKO+6%  BIGL-2%       Sprunk overcomes aggressive Logger.       TIKL+59%  FRUC-12%  WHIZ+69%  BADR+22%  GNIC-32%       Whiz has liquid assets worth over 1 Billion.       NXJN+36%  XGAS+26%  SYNC+8%  RONO+16%  GLBOIL+26%       Economic growth eases slightly.       LSNW+3%  METV-2%  WZNT+2%  CANE-3%  LIBT+10%  CNTU-2%  EYEF+3%       Corporate America braces for impact.       BOLS-10%  JLZA+1%  LOMB+17%  FLEE-18%  SHRK+20%  BAWS-21%       Only one in five SA:MP players have left their house this year.       BNCO+3%  BAWSAQ-2%  PLAP+2%  ERIS-3%       Shares unruffled by surprise activist assault.       BETA+14%  UGEN-1%  MOLIS+8%  MAXR-4%  MTA+30%       Energy bills cut for millions of people.       ~~~~~       ");

	// *theatre
	if(ucScrollbar == 1 || ucScrollbar == 0xFF)
		SetScrollBarText(1, "A new musical 'RAINCOAT EXPOSE' A real eye-opener at The Seagull Theater.       Tickets available for all shows at the eyefind.info box office.       Do not miss 'Banging trash can lids for an hour' at the Liberteen Theater  What a racket!       Magic Curtain Theater presents 'Swinging Puppets' and a fantastic new play 'Shakespeare on Drugs' The bard trip.       'The Flying Whore' at the Amdram Theatre  Enormous talents!  Needs to be seen to be believed!       'WTF' A tale of online love showing at The Maginot Theater.       You'll be LOL       ROFL       Returns after a sell out UK tour 'ABANDON SHIT' back at the Seagull Theater.       Performance Nonsense 'Fairy Body Paint' at the Liberteen Theater  What a big bunch of fairies.       ~~~~~       ");

	// *advertising
	if(ucScrollbar == 2 || ucScrollbar == 0xFF)
	{
		// Get the credits string
		String strCredits = g_pCredits->GetCreditsString();

		// Append the clock to the credits string
		strCredits.Append("  ~~~~       ");

		// Set the scroll bar text
		SetScrollBarText(2, strCredits.Get());
	}
	
	// *clock (doesn't scroll)
	if(ucScrollbar == 3 || ucScrollbar == 0xFF)
		SetScrollBarText(3, "~~~~~       ");

	// *urls
	if(ucScrollbar == 4 || ucScrollbar == 0xFF)
		SetScrollBarText(4, "autoeroticar.com       chiropracticovernight.com       ilovetoblow.org       a-thousand-words.net       libertytreeonline.com       babiesovernight.com       love-meet.net       loveyourmeat.com       myonlineme.com       outsourceforamerica.com       pointclickshipabitch.com       weazelnews.com       piswasser.com       beanmachinecoffee.com       burgershot.net       electrictit.com       krapea.com       lipurgex.com       littlelacysurprisepageant.com       publiclibertyonline.com       vipluxuryringtones.com       americantravelguide.net       fruitcomputers.com       designerslave.com       erisfootware.com       eugenicsincorporated.com       flyhighpizzapie.com       gloryholethemepark.com       craplist.net       blogsnobs.org       libertycitypolice.com       sprunksoda.com       rustybrownsringdonuts.com       peepthatshit.com       lootandwank.com       myroomonline.net       dragonbrainthemovie.com       eatbiglogs.com       friendswithoutfaces.net       hornyhighschoolreunions.com       money2makemoney.com       poker-in-the-rear.com       redwoodcigarettes.com       roidsforlittleboys.com       ");
	
	// *comedyclub
	if(ucScrollbar == 5 || ucScrollbar == 0xFF)
		SetScrollBarText(5, "split sides proudly presents special guest -  ricky gervais - with resident comedian - katt williams -       ");

	// *traffic
	if(ucScrollbar == 6 || ucScrollbar == 0xFF)
		SetScrollBarText(6, "stay in sane       min speed 20 miles       max speed 30 miles       check your fuel       tiredness killed her       car thefts on the up. lock your car       be a courteous driver       watch your speed       ");

	// *news
	if(ucScrollbar == 7 || ucScrollbar == 0xFF)
		SetScrollBarText(7, "crime on an all time low.       ~~~~~       sudden influx illegal immigrants from balkan says mayor.       liberty city attracts more tourist than any other city in the world       ");
}

void CGame::GetScrollBarText(unsigned char ucScrollbar, char* szReturn)
{
	if(ucScrollbar > 7)
	{
		szReturn[0] = '\0';
		return;
	}

	strcpy(szReturn, (char *)(COffsets::VAR_ScrollBarText + ucScrollbar * 1300));
}

void CGame::SetScrollBarColor(unsigned int uiColor)
{
	*(DWORD *)(COffsets::VAR_ScrollBarColor) = ((uiColor >> 8) & 0xFFFFFF) + ((uiColor << 24) & 0xFF000000);
}

unsigned int CGame::GetScrollBarColor()
{
	unsigned int uiColor = *(DWORD *)(COffsets::VAR_ScrollBarColor);
	return ((uiColor << 8) & 0xFFFFFF00) + ((uiColor >> 24) & 0xFF);
}

String CGame::GetCurrentAreaName()
{
	String strAreaName;
	char * szAreaName = (char *)(COffsets::VAR_CurrentAreaName);

	for(int i = 0; i < 128; i += 2)
	{
		char c = szAreaName[i];

		if(c == 0)
			break;

		strAreaName += c;
	}

	return strAreaName;
}

String CGame::GetCurrentStreetName()
{
	String strStreetName;
	char * szStreetName = (char *)(COffsets::VAR_CurrentStreetName);

	for(int i = 0; i < 128; i += 2)
	{
		char c = szStreetName[i];

		if(c == 0)
			break;

		strStreetName += c;
	}

	return strStreetName;
}
void CGame::RemoveInitialLoadingScreens()
{
	// Legal, Legal 2, R*, R*N, GTA:IV, ...
	/*for( int i = 0; i < *(int *)(COffsets::VAR_NumLoadingScreens); ++i ) // For loading screen the make the black background
	{
		if( i < 2 )
		{
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenType + i * 400) = ((i < 2) ? 0 : 0);
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 3600000;
		}

		if( i >= 2 && i <= 4 )
		{
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 0;
		}
	}*/
	// Legal, Legal 2, R*, R*N, GTA:IV, ...
	for(int i = 0; i < *(int *)(COffsets::VAR_NumLoadingScreens); ++i)
	{
		*(DWORD *)(COffsets::VAR_FirstLoadingScreenType + i * 400) = ((i <= 4) ? 4 : i);

		if(i <= 4)
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 0;
	}
}

void CGame::GetScreenPositionFromWorldPosition(CVector3 vecWorldPosition, Vector2 &vecScreenPosition)
{
	DWORD dwAddress = (CGame::GetBase() + 0xAFF3A0);
    CVector3 * pWorldPosition = &vecWorldPosition;
    Vector2 * pScreenPosition = &vecScreenPosition;
    _asm
    {
            push pScreenPosition
            push 2 ; game viewport id
            push pWorldPosition
            call dwAddress
    }

}

void CGame::CreateExplosion(CVector3& vecPosition, unsigned int uiExplosionType, float fRadius, bool bSound, bool bInvisible, float fCameraShake)
{
	DWORD dwFunc = (CGame::GetBase() + 0x9609F0);
	CVector3 * pPosition = &vecPosition;
	_asm
	{
		push -1
		push 0
		push 0
		push 0
		push 0
		push 0
		push 0
		push 0
		push bInvisible
		push 0
		push fCameraShake
		push bSound
		push 0
		push 0
		push pPosition
		push fRadius
		push uiExplosionType
		push 0
		push 0
		call dwFunc
		add esp, 4Ch
	}
}

unsigned int CGame::CreateFire(CVector3 vecPosition, unsigned int uiStrength, unsigned char ucAllowedGenerations)
{
	return Scripting::StartScriptFire(vecPosition.fX, vecPosition.fY, vecPosition.fZ, ucAllowedGenerations, uiStrength);
}

bool CGame::DeleteFire(unsigned int uiFire)
{
	Scripting::RemoveScriptFire(uiFire);
	return true;
}

void * CGame::Alloc(DWORD dwSize)
{
	DWORD dwFunc = (CGame::GetBase() + 0x4011D0); // alloc
	void * pMemory = NULL;
	_asm
	{
		push dwSize
		call dwFunc
		mov pMemory, eax
		add esp, 4
	}
	return pMemory;
}

void CGame::Free(void * pMemory)
{
	DWORD dwFunc = (CGame::GetBase() + 0x5B1C10); // gta_free
	_asm
	{
		push pMemory
		call dwFunc
		add esp, 4
	}
}

void CGame::ToggleLazlowStation(bool bToggle)
{
	*(BYTE *)(GetBase() + 0xF38728) = (bToggle == false);
	*(BYTE *)(GetBase() + 0x166805C) = (bToggle == true);
}

void CGame::InitCutscene(char * szCutsceneName, bool bUnknown1, bool bUnknown2)
{
	DWORD dwFunc = (CGame::GetBase() + 0x866980);
	_asm
	{
		push bUnknown2
		push bUnknown1
		push szCutsceneName
		call dwFunc
		add esp, 0Ch
	}
}

void CGame::StopCutscene(char * szCutsceneName)
{
	DWORD dwFunc = (CGame::GetBase() + 0x865E10);
	_asm
	{
		push szCutsceneName
		call dwFunc
		add esp, 4
	}
}

void CGame::InstallAnimGroups()
{
	Scripting::RequestAnims("amb@arcade");
	Scripting::RequestAnims("amb@argue");
	Scripting::RequestAnims("amb@atm");
	Scripting::RequestAnims("amb@bag_a_create");
	Scripting::RequestAnims("amb@bag_a_hold");
	Scripting::RequestAnims("amb@bag_a_idles");
	Scripting::RequestAnims("amb@bag_b_create");
	Scripting::RequestAnims("amb@bag_b_hold");
	Scripting::RequestAnims("amb@bag_b_idles");
	Scripting::RequestAnims("amb@bar");
	Scripting::RequestAnims("amb@baseball");
	Scripting::RequestAnims("amb@beg_sitting");
	Scripting::RequestAnims("amb@beg_standing");
	Scripting::RequestAnims("amb@bnch_bum_idl");
	Scripting::RequestAnims("amb@bnch_dnk_idl");
	Scripting::RequestAnims("amb@bnch_dnk_idl_f");
	Scripting::RequestAnims("amb@bnch_eat_idl");
	Scripting::RequestAnims("amb@bnch_eat_idl_f");
	Scripting::RequestAnims("amb@bnch_read_idl");
	Scripting::RequestAnims("amb@bnch_read_idl_f");
	Scripting::RequestAnims("amb@bnch_smk_idl");
	Scripting::RequestAnims("amb@bnch_smk_idl_f");
	Scripting::RequestAnims("amb@bnch_std_idl");
	Scripting::RequestAnims("amb@bnch_std_idl_f");
	Scripting::RequestAnims("amb@book");
	Scripting::RequestAnims("amb@boot_idles");
	Scripting::RequestAnims("amb@bottle_create");
	Scripting::RequestAnims("amb@bottle_destroy");
	Scripting::RequestAnims("amb@bottle_hold");
	Scripting::RequestAnims("amb@bottle_idle");
	Scripting::RequestAnims("amb@bouncer_idles_a");
	Scripting::RequestAnims("amb@bouncer_idles_b");
	Scripting::RequestAnims("amb@brazier");
	Scripting::RequestAnims("amb@bridgecops");
	Scripting::RequestAnims("amb@broken_d_idles_a");
	Scripting::RequestAnims("amb@broken_d_idles_b");
	Scripting::RequestAnims("amb@bum_a");
	Scripting::RequestAnims("amb@bum_b");
	Scripting::RequestAnims("amb@bum_c");
	Scripting::RequestAnims("amb@burger_create");
	Scripting::RequestAnims("amb@burger_destroy");
	Scripting::RequestAnims("amb@burger_hold");
	Scripting::RequestAnims("amb@burger_idle");
	Scripting::RequestAnims("amb@burgercart");
	Scripting::RequestAnims("amb@busker");
	Scripting::RequestAnims("amb@cafe_eat_idles");
	Scripting::RequestAnims("amb@cafe_empty_idl");
	Scripting::RequestAnims("amb@cafe_empty_idl_f");
	Scripting::RequestAnims("amb@cafe_idles");
	Scripting::RequestAnims("amb@cafe_idles_f");
	Scripting::RequestAnims("amb@cafe_read_idl");
	Scripting::RequestAnims("amb@cafe_read_idl_f");
	Scripting::RequestAnims("amb@cafe_smk_create");
	Scripting::RequestAnims("amb@cafe_smk_idl_f");
	Scripting::RequestAnims("amb@cafe_smoke_idl_a");
	Scripting::RequestAnims("amb@cafe_smoke_idl_b");
	Scripting::RequestAnims("amb@car_cell_crte_ds");
	Scripting::RequestAnims("amb@car_cell_crte_ps");
	Scripting::RequestAnims("amb@car_cell_dsty_ds");
	Scripting::RequestAnims("amb@car_cell_dsty_ps");
	Scripting::RequestAnims("amb@car_cell_idle_ds");
	Scripting::RequestAnims("amb@car_cell_idle_ps");
	Scripting::RequestAnims("amb@car_low_ps_loops");
	Scripting::RequestAnims("amb@car_stash");
	Scripting::RequestAnims("amb@car_std_bk_seat");
	Scripting::RequestAnims("amb@car_std_ds_a");
	Scripting::RequestAnims("amb@car_std_ds_b");
	Scripting::RequestAnims("amb@car_std_ds_c");
	Scripting::RequestAnims("amb@car_std_ds_d");
	Scripting::RequestAnims("amb@car_std_ds_trash");
	Scripting::RequestAnims("amb@car_std_f_id_ds");
	Scripting::RequestAnims("amb@car_std_f_id_ps");
	Scripting::RequestAnims("amb@car_std_ps_b");
	Scripting::RequestAnims("amb@car_std_ps_c");
	Scripting::RequestAnims("amb@car_std_ps_d");
	Scripting::RequestAnims("amb@car_std_ps_e");
	Scripting::RequestAnims("amb@car_std_ps_loops");
	Scripting::RequestAnims("amb@car_std_ps_trash");
	Scripting::RequestAnims("amb@carry_create");
	Scripting::RequestAnims("amb@carry_hold");
	Scripting::RequestAnims("amb@carry_idles");
	Scripting::RequestAnims("amb@cartcommon");
	Scripting::RequestAnims("amb@carwash");
	Scripting::RequestAnims("amb@club_");
	Scripting::RequestAnims("amb@coffee_hold");
	Scripting::RequestAnims("amb@coffee_idle_f");
	Scripting::RequestAnims("amb@coffee_idle_m");
	Scripting::RequestAnims("amb@cold");
	Scripting::RequestAnims("amb@comedy");
	Scripting::RequestAnims("amb@dance_femidl_a");
	Scripting::RequestAnims("amb@dance_femidl_b");
	Scripting::RequestAnims("amb@dance_femidl_c");
	Scripting::RequestAnims("amb@dance_maleidl_a");
	Scripting::RequestAnims("amb@dance_maleidl_b");
	Scripting::RequestAnims("amb@dance_maleidl_c");
	Scripting::RequestAnims("amb@dance_maleidl_d");
	Scripting::RequestAnims("amb@dating");
	Scripting::RequestAnims("amb@default");
	Scripting::RequestAnims("amb@drill_create");
	Scripting::RequestAnims("amb@drill_idles");
	Scripting::RequestAnims("amb@drink_bottle");
	Scripting::RequestAnims("amb@drink_can");
	Scripting::RequestAnims("amb@drink_f_fat");
	Scripting::RequestAnims("amb@drink_fbottle");
	Scripting::RequestAnims("amb@drink_fcan");
	Scripting::RequestAnims("amb@drugd_idl_a");
	Scripting::RequestAnims("amb@drugd_idl_b");
	Scripting::RequestAnims("amb@drugd_sell");
	Scripting::RequestAnims("amb@drunk");
	Scripting::RequestAnims("amb@eat_chocolate");
	Scripting::RequestAnims("amb@eat_fruit");
	Scripting::RequestAnims("amb@ffood_server");
	Scripting::RequestAnims("amb@flee");
	Scripting::RequestAnims("amb@flee_a");
	Scripting::RequestAnims("amb@garbage");
	Scripting::RequestAnims("amb@hang_fat_cl_a");
	Scripting::RequestAnims("amb@hang_fat_cl_b");
	Scripting::RequestAnims("amb@hang_fat_idls");
	Scripting::RequestAnims("amb@hang_st_i_f_cl_a");
	Scripting::RequestAnims("amb@hang_st_i_f_cl_c");
	Scripting::RequestAnims("amb@hang_str_f_idls");
	Scripting::RequestAnims("amb@hang_str_i_cl_a");
	Scripting::RequestAnims("amb@hang_str_i_cl_c");
	Scripting::RequestAnims("amb@hang_str_idls");
	Scripting::RequestAnims("amb@hang_thin_cl");
	Scripting::RequestAnims("amb@hang_thin_idls");
	Scripting::RequestAnims("amb@homeless_a");
	Scripting::RequestAnims("amb@homeless_b");
	Scripting::RequestAnims("amb@hooker");
	Scripting::RequestAnims("amb@hooker_fat");
	Scripting::RequestAnims("amb@hospital_idles_a");
	Scripting::RequestAnims("amb@hospital_idles_b");
	Scripting::RequestAnims("amb@hot");
	Scripting::RequestAnims("amb@hotdog_destroy");
	Scripting::RequestAnims("amb@hotdog_destroy_f");
	Scripting::RequestAnims("amb@hotdog_hold");
	Scripting::RequestAnims("amb@hotdog_idle");
	Scripting::RequestAnims("amb@hotdog_idle_f");
	Scripting::RequestAnims("amb@hotdogcart");
	Scripting::RequestAnims("amb@hurry_head");
	Scripting::RequestAnims("amb@hurry_walk");
	Scripting::RequestAnims("amb@ice_vendor");
	Scripting::RequestAnims("amb@icecream_default");
	Scripting::RequestAnims("amb@icecream_destroy");
	Scripting::RequestAnims("amb@icecream_idles");
	Scripting::RequestAnims("amb@injured_front");
	Scripting::RequestAnims("amb@injured_side");
	Scripting::RequestAnims("amb@inquisitive");
	Scripting::RequestAnims("amb@int_cafe_idles");
	Scripting::RequestAnims("amb@kiosk");
	Scripting::RequestAnims("amb@lean_balc_cre");
	Scripting::RequestAnims("amb@lean_balc_i_a");
	Scripting::RequestAnims("amb@lean_balc_i_b");
	Scripting::RequestAnims("amb@lean_balc_i_b_f");
	Scripting::RequestAnims("amb@lean_balc_idl_b");
	Scripting::RequestAnims("amb@lean_idl_b");
	Scripting::RequestAnims("amb@lean_idles");
	Scripting::RequestAnims("amb@lean_phone_idles");
	Scripting::RequestAnims("amb@lean_smoke_idles");
	Scripting::RequestAnims("amb@look_dead_ped");
	Scripting::RequestAnims("amb@mag_vendor");
	Scripting::RequestAnims("amb@misc");
	Scripting::RequestAnims("amb@mission_end");
	Scripting::RequestAnims("amb@music_create");
	Scripting::RequestAnims("amb@music_destroy");
	Scripting::RequestAnims("amb@music_hold");
	Scripting::RequestAnims("amb@music_idles");
	Scripting::RequestAnims("amb@newspaper_create");
	Scripting::RequestAnims("amb@newspaper_destry");
	Scripting::RequestAnims("amb@newspaper_hold");
	Scripting::RequestAnims("amb@newspaper_idles");
	Scripting::RequestAnims("amb@newspaper_vend");
	Scripting::RequestAnims("amb@nightclub_ext");
	Scripting::RequestAnims("amb@nutcart");
	Scripting::RequestAnims("amb@nuts_create");
	Scripting::RequestAnims("amb@nuts_destroy");
	Scripting::RequestAnims("amb@nuts_hold");
	Scripting::RequestAnims("amb@nuts_idle");
	Scripting::RequestAnims("amb@park_taichi_a");
	Scripting::RequestAnims("amb@park_taichi_b");
	Scripting::RequestAnims("amb@payphone");
	Scripting::RequestAnims("amb@payphone_create");
	Scripting::RequestAnims("amb@payphone_idl_a");
	Scripting::RequestAnims("amb@payphone_idl_b");
	Scripting::RequestAnims("amb@phone_f");
	Scripting::RequestAnims("amb@phone_m_a");
	Scripting::RequestAnims("amb@phone_m_b");
	Scripting::RequestAnims("amb@pimps_pros");
	Scripting::RequestAnims("amb@plank_create");
	Scripting::RequestAnims("amb@plank_hold");
	Scripting::RequestAnims("amb@postman_idles");
	Scripting::RequestAnims("amb@preach_idl_a");
	Scripting::RequestAnims("amb@preach_idl_b");
	Scripting::RequestAnims("amb@preen");
	Scripting::RequestAnims("amb@preen_bsness");
	Scripting::RequestAnims("amb@rain_newspaper");
	Scripting::RequestAnims("amb@rain_newspaper_b");
	Scripting::RequestAnims("amb@rake_create");
	Scripting::RequestAnims("amb@rake_f_create");
	Scripting::RequestAnims("amb@rake_f_idles");
	Scripting::RequestAnims("amb@rake_f_walk");
	Scripting::RequestAnims("amb@rake_hold");
	Scripting::RequestAnims("amb@rake_idles");
	Scripting::RequestAnims("amb@rake_walk");
	Scripting::RequestAnims("amb@roadcross");
	Scripting::RequestAnims("amb@roadcross_head");
	Scripting::RequestAnims("amb@roadcross_rain");
	Scripting::RequestAnims("amb@roadcross_rain_b");
	Scripting::RequestAnims("amb@rubbish");
	Scripting::RequestAnims("amb@savegame");
	Scripting::RequestAnims("amb@security_idles_a");
	Scripting::RequestAnims("amb@security_idles_b");
	Scripting::RequestAnims("amb@security_idles_c");
	Scripting::RequestAnims("amb@security_idles_d");
	Scripting::RequestAnims("amb@service_idles_a");
	Scripting::RequestAnims("amb@service_idles_b");
	Scripting::RequestAnims("amb@shock");
	Scripting::RequestAnims("amb@shock_events");
	Scripting::RequestAnims("amb@shoes_a");
	Scripting::RequestAnims("amb@shoes_b");
	Scripting::RequestAnims("amb@shop_int");
	Scripting::RequestAnims("amb@sledge_create");
	Scripting::RequestAnims("amb@sledge_idles");
	Scripting::RequestAnims("amb@smk_scn_create");
	Scripting::RequestAnims("amb@smk_scn_create_f");
	Scripting::RequestAnims("amb@smk_scn_dest_f");
	Scripting::RequestAnims("amb@smk_scn_destroy");
	Scripting::RequestAnims("amb@smk_scn_idles");
	Scripting::RequestAnims("amb@smk_scn_idles_f");
	Scripting::RequestAnims("amb@smoking");
	Scripting::RequestAnims("amb@smoking_create");
	Scripting::RequestAnims("amb@smoking_create_f");
	Scripting::RequestAnims("amb@smoking_dest_f");
	Scripting::RequestAnims("amb@smoking_destroy");
	Scripting::RequestAnims("amb@smoking_f");
	Scripting::RequestAnims("amb@smoking_idles");
	Scripting::RequestAnims("amb@smoking_idles_f");
	Scripting::RequestAnims("amb@smoking_spliff");
	Scripting::RequestAnims("amb@spade_idles");
	Scripting::RequestAnims("amb@sprunk_ped");
	Scripting::RequestAnims("amb@sprunk_plyr");
	Scripting::RequestAnims("amb@standing_female");
	Scripting::RequestAnims("amb@standing_male");
	Scripting::RequestAnims("amb@standing_vendor");
	Scripting::RequestAnims("amb@step_idl_a");
	Scripting::RequestAnims("amb@step_idl_b");
	Scripting::RequestAnims("amb@step_idles");
	Scripting::RequestAnims("amb@step_idles_b");
	Scripting::RequestAnims("amb@stoop_argue");
	Scripting::RequestAnims("amb@stop_create");
	Scripting::RequestAnims("amb@stop_idles");
	Scripting::RequestAnims("amb@strip_chr_idls_a");
	Scripting::RequestAnims("amb@strip_chr_idls_b");
	Scripting::RequestAnims("amb@strip_rail_idl");
	Scripting::RequestAnims("amb@strip_stool_i_a");
	Scripting::RequestAnims("amb@strip_stool_i_b");
	Scripting::RequestAnims("amb@super_create");
	Scripting::RequestAnims("amb@super_idles_a");
	Scripting::RequestAnims("amb@super_idles_b");
	Scripting::RequestAnims("amb@super_part");
	Scripting::RequestAnims("amb@sweep_create");
	Scripting::RequestAnims("amb@sweep_ffood_i_f");
	Scripting::RequestAnims("amb@sweep_ffood_idl");
	Scripting::RequestAnims("amb@sweep_hold");
	Scripting::RequestAnims("amb@sweep_idles");
	Scripting::RequestAnims("amb@taxi");
	Scripting::RequestAnims("amb@taxi_hail_f");
	Scripting::RequestAnims("amb@taxi_hail_m");
	Scripting::RequestAnims("amb@taxi_wait_i_f_a");
	Scripting::RequestAnims("amb@taxi_wait_i_f_b");
	Scripting::RequestAnims("amb@taxi_wait_i_m_a");
	Scripting::RequestAnims("amb@taxi_wait_i_m_b");
	Scripting::RequestAnims("amb@taxi_wait_i_m_c");
	Scripting::RequestAnims("amb@telegraph_idles");
	Scripting::RequestAnims("amb@telescope_idl_f");
	Scripting::RequestAnims("amb@telescope_idles");
	Scripting::RequestAnims("amb@telescope_player");
	Scripting::RequestAnims("amb@tollbooth");
	Scripting::RequestAnims("amb@tourist_camera");
	Scripting::RequestAnims("amb@tourist_create");
	Scripting::RequestAnims("amb@tourist_f");
	Scripting::RequestAnims("amb@tourist_hold");
	Scripting::RequestAnims("amb@tourist_idles_a");
	Scripting::RequestAnims("amb@tourist_idles_b");
	Scripting::RequestAnims("amb@tripup");
	Scripting::RequestAnims("amb@umbrella_close");
	Scripting::RequestAnims("amb@umbrella_f_fat");
	Scripting::RequestAnims("amb@umbrella_hold");
	Scripting::RequestAnims("amb@umbrella_idl_a");
	Scripting::RequestAnims("amb@umbrella_idl_b");
	Scripting::RequestAnims("amb@umbrella_open_a");
	Scripting::RequestAnims("amb@umbrella_open_b");
	Scripting::RequestAnims("amb@vendor");
	Scripting::RequestAnims("amb@wall_idles");
	Scripting::RequestAnims("amb@wall_idles_f");
	Scripting::RequestAnims("amb@wall_read_idl");
	Scripting::RequestAnims("amb@wall_read_idl_f");
	Scripting::RequestAnims("amb@wallet");
	Scripting::RequestAnims("amb@wasted_a");
	Scripting::RequestAnims("amb@wasted_b");
	Scripting::RequestAnims("amb@watch_melee");
	Scripting::RequestAnims("amb@wcan_create");
	Scripting::RequestAnims("amb@wcan_idles");
	Scripting::RequestAnims("amb@wcan_part");
	Scripting::RequestAnims("amb@winclean_idl");
	Scripting::RequestAnims("amb@winshop_idles");
	Scripting::RequestAnims("amb@yawn");
	Scripting::RequestAnims("defend@gen_1h");
	Scripting::RequestAnims("defend@gen_2h");
	Scripting::RequestAnims("facials@f_hi");
	Scripting::RequestAnims("facials@f_lo");
	Scripting::RequestAnims("facials@m_hi");
	Scripting::RequestAnims("facials@m_lo");
	Scripting::RequestAnims("gestures@car");
	Scripting::RequestAnims("gestures@car_f");
	Scripting::RequestAnims("gestures@female");
	Scripting::RequestAnims("gestures@m_seated");
	Scripting::RequestAnims("gestures@male");
	Scripting::RequestAnims("gestures@mp_female");
	Scripting::RequestAnims("gestures@mp_male");
	Scripting::RequestAnims("gestures@niko");
	Scripting::RequestAnims("gun@aim_idles");
	Scripting::RequestAnims("gun@ak47");
	Scripting::RequestAnims("gun@baretta");
	Scripting::RequestAnims("gun@cops");
	Scripting::RequestAnims("gun@deagle");
	Scripting::RequestAnims("gun@handgun");
	Scripting::RequestAnims("gun@mp5k");
	Scripting::RequestAnims("gun@partials");
	Scripting::RequestAnims("gun@rifle");
	Scripting::RequestAnims("gun@rocket");
	Scripting::RequestAnims("gun@shotgun");
	Scripting::RequestAnims("gun@uzi");
	Scripting::RequestAnims("move_f@armed");
	Scripting::RequestAnims("move_f@bness_a");
	Scripting::RequestAnims("move_f@bness_b");
	Scripting::RequestAnims("move_f@bness_c");
	Scripting::RequestAnims("move_f@bness_d");
	Scripting::RequestAnims("move_f@bness_e");
	Scripting::RequestAnims("move_f@casual");
	Scripting::RequestAnims("move_f@casual_b");
	Scripting::RequestAnims("move_f@casual_c");
	Scripting::RequestAnims("move_f@cower");
	Scripting::RequestAnims("move_f@fat");
	Scripting::RequestAnims("move_f@generic");
	Scripting::RequestAnims("move_f@m_p");
	Scripting::RequestAnims("move_f@michelle");
	Scripting::RequestAnims("move_f@multiplyr");
	Scripting::RequestAnims("move_f@old_a");
	Scripting::RequestAnims("move_f@old_b");
	Scripting::RequestAnims("move_f@old_c");
	Scripting::RequestAnims("move_f@old_d");
	Scripting::RequestAnims("move_f@puffer");
	Scripting::RequestAnims("move_f@sexy");
	Scripting::RequestAnims("move_gng@afro_a");
	Scripting::RequestAnims("move_gng@afro_b");
	Scripting::RequestAnims("move_gng@afro_c");
	Scripting::RequestAnims("move_gng@generic_a");
	Scripting::RequestAnims("move_gng@generic_b");
	Scripting::RequestAnims("move_gng@generic_c");
	Scripting::RequestAnims("move_gng@jam_a");
	Scripting::RequestAnims("move_gng@jam_b");
	Scripting::RequestAnims("move_gng@jam_c");
	Scripting::RequestAnims("move_gng@latino_a");
	Scripting::RequestAnims("move_gng@latino_b");
	Scripting::RequestAnims("move_gng@latino_c");
	Scripting::RequestAnims("move_m@bernie");
	Scripting::RequestAnims("move_m@bness_a");
	Scripting::RequestAnims("move_m@bness_b");
	Scripting::RequestAnims("move_m@bness_c");
	Scripting::RequestAnims("move_m@bum");
	Scripting::RequestAnims("move_m@case");
	Scripting::RequestAnims("move_m@casual");
	Scripting::RequestAnims("move_m@casual_b");
	Scripting::RequestAnims("move_m@casual_c");
	Scripting::RequestAnims("move_m@cower");
	Scripting::RequestAnims("move_m@cs_swat");
	Scripting::RequestAnims("move_m@eddie");
	Scripting::RequestAnims("move_m@fat");
	Scripting::RequestAnims("move_m@generic");
	Scripting::RequestAnims("move_m@h_cuffed");
	Scripting::RequestAnims("move_m@multiplyr");
	Scripting::RequestAnims("move_m@old_a");
	Scripting::RequestAnims("move_m@old_b");
	Scripting::RequestAnims("move_m@old_c");
	Scripting::RequestAnims("move_m@playboy");
	Scripting::RequestAnims("move_m@roman");
	Scripting::RequestAnims("move_m@roman_inj");
	Scripting::RequestAnims("move_m@swat");
	Scripting::RequestAnims("move_m@tourist");
	Scripting::RequestAnims("move_melee");
	Scripting::RequestAnims("reaction@male_flee");
	Scripting::RequestAnims("veh@base");
	Scripting::RequestAnims("veh@bike_chopper");
	Scripting::RequestAnims("veh@bike_dirt");
	Scripting::RequestAnims("veh@bike_freeway");
	Scripting::RequestAnims("veh@bike_scooter");
	Scripting::RequestAnims("veh@bike_spt");
	Scripting::RequestAnims("veh@boat_speed");
	Scripting::RequestAnims("veh@boat_stand_big");
	Scripting::RequestAnims("veh@boat_standing");
	Scripting::RequestAnims("veh@bus");
	Scripting::RequestAnims("veh@busted_low");
	Scripting::RequestAnims("veh@busted_std");
	Scripting::RequestAnims("veh@busted_truck");
	Scripting::RequestAnims("veh@busted_van");
	Scripting::RequestAnims("veh@cablecar");
	Scripting::RequestAnims("veh@drivebyairtug");
	Scripting::RequestAnims("veh@drivebybike_chop");
	Scripting::RequestAnims("veh@drivebybike_dirt");
	Scripting::RequestAnims("veh@drivebybike_free");
	Scripting::RequestAnims("veh@drivebybike_scot");
	Scripting::RequestAnims("veh@drivebybike_spt");
	Scripting::RequestAnims("veh@drivebyboat_big");
	Scripting::RequestAnims("veh@drivebyboat_spee");
	Scripting::RequestAnims("veh@drivebyboat_stnd");
	Scripting::RequestAnims("veh@drivebycop_std");
	Scripting::RequestAnims("veh@drivebyheli");
	Scripting::RequestAnims("veh@drivebylow");
	Scripting::RequestAnims("veh@drivebylow_conv");
	Scripting::RequestAnims("veh@drivebystd");
	Scripting::RequestAnims("veh@drivebytruck");
	Scripting::RequestAnims("veh@drivebyvan");
	Scripting::RequestAnims("veh@heli_annih");
	Scripting::RequestAnims("veh@helicopter");
	Scripting::RequestAnims("veh@helicopter_xx_h");
	Scripting::RequestAnims("veh@low");
	Scripting::RequestAnims("veh@low_conv");
	Scripting::RequestAnims("veh@low_hi2_hi2");
	Scripting::RequestAnims("veh@low_hi3_hi3");
	Scripting::RequestAnims("veh@low_hi4_hi4");
	Scripting::RequestAnims("veh@low_hi_hi");
	Scripting::RequestAnims("veh@low_infernus");
	Scripting::RequestAnims("veh@low_jack_pistol");
	Scripting::RequestAnims("veh@low_jack_rifle");
	Scripting::RequestAnims("veh@low_jack_rpg");
	Scripting::RequestAnims("veh@low_le1_ri1");
	Scripting::RequestAnims("veh@low_le2_ri2");
	Scripting::RequestAnims("veh@low_lu1_ri1");
	Scripting::RequestAnims("veh@low_lu2_ri2");
	Scripting::RequestAnims("veh@low_xx_lo1");
	Scripting::RequestAnims("veh@std");
	Scripting::RequestAnims("veh@std_conv");
	Scripting::RequestAnims("veh@std_df8");
	Scripting::RequestAnims("veh@std_fo1");
	Scripting::RequestAnims("veh@std_jack_pistol");
	Scripting::RequestAnims("veh@std_jack_pistolb");
	Scripting::RequestAnims("veh@std_jack_rifle");
	Scripting::RequestAnims("veh@std_jack_rifle_b");
	Scripting::RequestAnims("veh@std_jack_rifle_c");
	Scripting::RequestAnims("veh@std_jack_rpg");
	Scripting::RequestAnims("veh@std_jack_unarmdb");
	Scripting::RequestAnims("veh@std_jack_unarmdc");
	Scripting::RequestAnims("veh@std_jack_unarmdd");
	Scripting::RequestAnims("veh@std_jack_unarmed");
	Scripting::RequestAnims("veh@std_le1_ri1");
	Scripting::RequestAnims("veh@std_lo1_lo1");
	Scripting::RequestAnims("veh@std_lo2_lo2");
	Scripting::RequestAnims("veh@std_lo3_lo3");
	Scripting::RequestAnims("veh@std_lo4_lo4");
	Scripting::RequestAnims("veh@std_lo5_lo5");
	Scripting::RequestAnims("veh@std_ri1");
	Scripting::RequestAnims("veh@std_ri1_lo1");
	Scripting::RequestAnims("veh@std_ri2");
	Scripting::RequestAnims("veh@std_ri2_lo2");
	Scripting::RequestAnims("veh@std_ri3");
	Scripting::RequestAnims("veh@std_ri3_lo3");
	Scripting::RequestAnims("veh@std_xx_ri1");
	Scripting::RequestAnims("veh@std_xx_ri2");
	Scripting::RequestAnims("veh@train");
	Scripting::RequestAnims("veh@tru_jack_pistol");
	Scripting::RequestAnims("veh@tru_jack_rifle");
	Scripting::RequestAnims("veh@truck");
	Scripting::RequestAnims("veh@truck_ba_xx");
	Scripting::RequestAnims("veh@truck_le1_ri1");
	Scripting::RequestAnims("veh@truck_lo1_lo1");
	Scripting::RequestAnims("veh@truck_phantom");
	Scripting::RequestAnims("veh@van");
	Scripting::RequestAnims("veh@van_ba1");
	Scripting::RequestAnims("veh@van_fo1_fo1");
	Scripting::RequestAnims("veh@van_fo2_fo2");
	Scripting::RequestAnims("veh@van_hl1_hi1");
	Scripting::RequestAnims("veh@van_jack_pistol");
	Scripting::RequestAnims("veh@van_jack_rifle");
	Scripting::RequestAnims("veh@van_jack_unarmed");
	Scripting::RequestAnims("veh@van_ll_lo");
	Scripting::RequestAnims("veh@van_lr1_lf1");
	Scripting::RequestAnims("veh@van_ri1_le1");
	Scripting::RequestAnims("veh@van_ri2_le2");
	Scripting::RequestAnims("veh@van_ri3_le3");
	Scripting::RequestAnims("veh@van_xx_le1");
	Scripting::RequestAnims("visemes@f_hi");
	Scripting::RequestAnims("visemes@f_lo");
	Scripting::RequestAnims("visemes@m_hi");
	Scripting::RequestAnims("visemes@m_lo");
}

DWORD CGame::GetNativeAddress(DWORD dwNative)
{
	DWORD dwFunc = COffsets::FUNC_ScrVM__FindNativeAddress;
	DWORD dwNativeFunc = NULL;
	_asm
	{
		push esi
		mov esi, dwNative
		call dwFunc
		pop esi
		mov dwNativeFunc, eax
	}

	if(dwNativeFunc != NULL)
		return dwNativeFunc;

	return -1;
}