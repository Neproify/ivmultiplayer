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
#include "Indicators.h"
#include "TrafficLights.h"
#include "ScriptHook.h"
#include "COffsets.h"
#include "CPools.h"
#include "CClientTaskManager.h"
#include "CCredits.h"

extern CChatWindow        * g_pChatWindow;
extern CInputWindow       * g_pInputWindow;
extern CClientTaskManager * g_pClientTaskManager;
extern CCredits           * g_pCredits;

unsigned int   CGame::m_uiBaseAddress;
bool           CGame::m_bInputState;
eState         CGame::m_State;
bool           CGame::m_bFocused;
CPools       * CGame::m_pPools;
CIVModelInfo   CGame::m_modelInfos[NUM_ModelInfos];
CIVWeaponInfo  CGame::m_weaponInfos[NUM_WeaponInfos];
CIVStreaming * CGame::m_pStreaming;
CIVWeather   * CGame::m_pWeather;

void CGame::SetFocused(bool bFocused)
{
	// Set the focused flag
	m_bFocused = bFocused;

	if(bFocused) // We have focus
	{
		// Enable input
		SetInputState(true);
	}
	else // We do not have focus
	{
		// Disable input
		SetInputState(false);
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
	return *(HWND *)(GetBase() + VAR_HWnd);
}

DWORD CGame::GetTime()
{
	return *(DWORD *)(GetBase() + VAR_Time);
}

void CGame::Initialize()
{
	m_uiBaseAddress = (unsigned int)GetModuleHandle(NULL);

	if(m_uiBaseAddress)
		CLogFile::Printf("Game Base: 0x%p (0x%p)", m_uiBaseAddress, (m_uiBaseAddress - 0x400000));
	else
	{
		CLogFile::Printf("Invalid Game Base Detected. Exiting...");
		ExitProcess(0);
	}

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
		CLogFile::Printf("Applied key sync hooks");

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

	SetInputState(true);
	SetState(GAME_STATE_NONE);

	// Create our pools class
	m_pPools = new CPools();

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

	// Initialize our game classes
	m_pStreaming = new CIVStreaming();
	m_pWeather = new CIVWeather();
}

void CGame::Shutdown()
{
	// Shutdown our game classes
	SAFE_DELETE(m_pWeather);
	SAFE_DELETE(m_pStreaming);

	// Shutdown our pools class
	m_pPools->Shutdown();

	// Delete our pools class
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
DWORD dwTaskDestructorReturn = NULL;
DWORD dwTaskVFTable = NULL;

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

	dwTaskDestructorReturn = (CGame::GetBase() + 0xA288DA);
	dwTaskVFTable = (CGame::GetBase() + 0xD87224);

	_asm
	{
		popad
		push esi
		mov esi, ecx
		push esi
		mov dword ptr [esi], offset dwTaskVFTable
		jmp dwTaskDestructorReturn
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

bool CGame::Patch()
{
	// Unprotect .text and .rdata memory and leave it unprotected
	UnprotectMemory();

	if(COffsets::GetVersion() == GAME_VERSION_7)
	{
		// Hook CTask::~CTask to use our own function
#define FUNC_CTask__Destructor 0xA288D0
		CPatcher::InstallJmpPatch((GetBase() + FUNC_CTask__Destructor), (DWORD)CTask__Destructor_Hook);

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

		// 
		//CPatcher::InstallJmpPatch(0x422CA7, (GetBase() + 0x422CAE));

		// Disable loading tune
		//CPatcher::InstallNopPatch(0x422A92, 10);

		// Don't let the game pause (Possibly use the gfwl multiplayer menu)
		// TODO

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

		// Disable rendering of distant cars at night
		// TODO: Test this
		//CPatcher::InstallRetnPatch(0x9055D0);

		// Overwrite the ESP Check in the func responsible for loading screens
		CPatcher::InstallCallPatch((GetBase() + 0x424B26), (DWORD)RemoveInitialLoadingScreens);

		// Disable sleep
		*(DWORD *)(GetBase() + 0x401835) = 1;

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

		// UPDATE
		// Bypass main menu
		// TODO: If i'm not gonna use the StartGame function
		// then write its code directly to this address
		//CPatcher::InstallCallPatch(0x40BF8D, (DWORD)StartGame);
		// Works fine (Except black loading screens)
		//CPatcher::InstallCallPatch(0x40BFBC, (DWORD)StartGame);
		return true;
	}

	return false;
}

#if 0
// UPDATE
#define VAR_AudioFrontend_4 0x1004020
#define VAR_AudioFrontend_5 0x11F2468
#define VAR_AudioFrontend_6 0x10F75A0
#define FUNC_StartGame_4 0xCC2437
#define FUNC_StartGame_5 0xCEF690
#define FUNC_StartGame_6 /*0x7B9A10*/0x420AE1

// TODO: Find a way to shut the game down and get it back to this state
// TODO: Find a faster way of doing this than just setting a flag
void CGame::StartGame()
{
	CPatcher::Unprotect((CGame::GetBase() + 0x119DB14), 1);
	*(BYTE *)(CGame::GetBase() + 0x119DB14) = 1;
	ptr g_audioFrontend = ptr_cast(GetBase() + VAR_AudioFrontend_6);
	ptr func = ptr_cast(GetBase() + FUNC_StartGame_6);
	_asm
	{
		mov ecx, g_audioFrontend
		call func
	}
	SetInputState(false);
	SetState(GAME_STATE_LOADING);
}
#endif

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
	{
		*uHour = *(DWORD *)(COffsets::VAR_CClock__Hour);
	}
	*uMinute = *(DWORD *)(COffsets::VAR_CClock__LockedMinute);
	if(*uMinute == -1)
	{
		*uMinute = *(DWORD *)(COffsets::VAR_CClock__Minute);
	}
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

void CGame::AddEntityToWorld(IVEntity * pEntity)
{
	DWORD dwFunc = COffsets::FUNC_CWorld__AddEntity;
	_asm
	{
		push 0
		push pEntity
		call dwFunc
		add esp, 8
	}
}

void CGame::RemoveEntityFromWorld(IVEntity * pEntity)
{
	DWORD dwFunc = COffsets::FUNC_CWorld__RemoveEntity;
	_asm
	{
		push 0
		push pEntity
		call dwFunc
		add esp, 8
	}
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
	if(ucScrollbar < 0 || ucScrollbar > 7)
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
	if(ucScrollbar < 0 || ucScrollbar > 7)
	{
		szReturn[0] = 0;
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
	for(int i = 0; i < *(int *)(COffsets::VAR_NumLoadingScreens); ++i)
	{
		*(DWORD *)(COffsets::VAR_FirstLoadingScreenType + i * 400) = ((i <= 4) ? 4 : i);

		if(i <= 4)
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 0;
	}
}

/*class CViewport
{
public:
};

class CViewportManager
{
public:
	PAD(pad0, 0x10);
	CViewport * pGameViewport;
};*/

void CGame::GetScreenPositionFromWorldPosition(CVector3 vecWorldPosition, Vector2 &vecScreenPosition)
{
	// Get the game viewport
/*#define VAR_ViewportManager 0x10F47E0
	CViewportManager * pViewportManager = *(CViewportManager **)(CGame::GetBase() + VAR_ViewportManager);
	CViewport * pGameViewport = pViewportManager->pGameViewport;

	// Get the screen position for the world position
	DWORD dwAddress = (CGame::GetBase() + 0x62F1C0);
	float * fScreenPositionX = &vecScreenPosition.X;
	float * fScreenPositionY = &vecScreenPosition.Y;
	Vector3 * pWorldPosition = &vecWorldPosition;
	_asm
	{
		push fScreenPositionY
		push fScreenPositionX
		push pWorldPosition
		mov ecx, pGameViewport
		add ecx, 10h
		call dwAddress
	}*/
	// TODO: Fix above code
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

void CGame::CreateExplosion(CVector3 vecPosition, unsigned int uiExplosionType, float fRadius, bool bSound, bool bInvisible, float fCameraShake)
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

unsigned int CGame::CreateFire(CVector3 vecPosition, unsigned char ucAllowedGenerations, unsigned int uiStrength)
{
	return Scripting::StartScriptFire(vecPosition.fX, vecPosition.fY, vecPosition.fZ, ucAllowedGenerations, uiStrength);
}

bool CGame::DeleteFire(unsigned int uiFire)
{
	Scripting::RemoveScriptFire(uiFire);
	return true;
}
