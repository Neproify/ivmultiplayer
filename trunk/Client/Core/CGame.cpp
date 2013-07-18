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
#include "CClient.h"
#include "Scripting.h"
#include "KeySync.h"
#include "AimSync.h"
#include "TrafficLights.h"
#include "ScriptHook.h"
#include "COffsets.h"
#include "CPools.h"
#include "CContextDataManager.h"
#include "AimSync.h"
#include "AnimGroups.h"
#include <SharedUtility.h>
#include "IV/CIVObject.h"
#include "CPatches.h"
#include "CCrashFixes.h"

extern CClient * g_pClient;
extern bool	g_bControlsDisabled;

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
bool           CGame::m_bNameTags = false;
bool           CGame::m_bGameLoaded = false;
bool           CGame::m_bHeadMovement = true;
bool           CGame::m_bSpecialData[2] = {false, true};
bool           CGame::m_bKickedFromServer = false;

void CGame::SetFocused(bool bFocused)
{
	// Set the focused flag
	m_bFocused = bFocused;

	if(bFocused) // We have focus
	{
		// Enable input
		SetInputState(!g_bControlsDisabled);

		// Unmute our audio
		g_pClient->GetAudioManager()->UnmuteAll();

	}
	else // We do not have focus
	{
		// Disable input
		SetInputState(false);

		// Mute our audio
		g_pClient->GetAudioManager()->MuteAll();
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

// thx listener
void CGame::UnprotectMemory()
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
	assert(sizeof(IVaudWeaponAudioEntity) == 0x18);
	assert(sizeof(IVWeapon) == 0x70);
	assert(sizeof(IVObject) == 0x320);

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

	// Make sure we have a valid game version
	if(COffsets::GetVersion() != GAME_VERSION_7)
	{
		MessageBox(NULL, "Invalid game version detected. IV:MP currently only supports version 1.0.7.0", "Invalid game version", NULL);
		ExitProcess(0);
	}

	// Unprotect all game memory
	UnprotectMemory();
	CLogFile::Printf("Unprotected game memory");

	// Apply our patches and hooks
	CPatches::Apply();
	CLogFile::Printf("Applied patches");

	// Apply our crash fixes
	CCrashFixes::Apply();
	CLogFile::Printf("Applied crash fixes");

	// Install our hooks
	// TODO: All hooks from CPatches.cpp and here to be put into CHooks/CKeySyncHooks/e.t.c.
	InstallKeySyncHooks();
	InstallAimSyncHooks();
	InstallTrafficLightHooks();
	InstallScriptHooks();
	CLogFile::Printf("Installed hooks");

	// Replace "Loading .." with "IV:MP is loading...", some user say that the label sometimes is shown
	// Scripting::SetMsgForLoadingScreen("IV:MP is loading....");

	SetInputState(true);
	SetState(GAME_STATE_NONE);

	// Create our pools class
	m_pPools = new CPools();

	// Increase our pool limits
	m_pPools->SetVehiclePoolLimit(MAX_STREAMED_VEHICLES);
	m_pPools->SetVehicleStructPoolLimit(124); // 123 vehicle in gta +1 for good luck
	CLogFile::Printf("Increased pool limits");

	// Create the pad instance
	m_pPad = new CIVPad((IVPad *)COffsets::VAR_Pads);

	// Initialize our game classes
	m_pStreaming = new CIVStreaming();
	m_pWeather = new CIVWeather();
	m_pWorld = new CIVWorld();

	// Initialize the model infos
	for(int i = 0; i < NUM_ModelInfos; i++)
		m_modelInfos[i].SetIndex(i);

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

void CGame::LoadHUD(String strPath)
{
	// Set our HUD.dat path
	const char * szPath = strPath.C_String();
	*(DWORD *)(CGame::GetBase() + 0x84841A) = (DWORD)szPath;

	// Load our HUD
	DWORD FUNC_LoadHUD (CGame::GetBase() + 0x848390);
	_asm call FUNC_LoadHUD;

	// Restore our HUD.dat path
	*(DWORD *)(CGame::GetBase() + 0x84841A) = (DWORD)(CGame::GetBase() + 0xD5DCF4);
}

void CGame::LoadRadioLogo(String strPath)
{
	// Set our RadioLogo.dat path
	const char * szPath = strPath.C_String();
	*(DWORD *)(CGame::GetBase() + 0x822E76) = (DWORD)szPath;

	// Load our radio logo
	DWORD FUNC_LoadRadioLogo = (CGame::GetBase() + 0x822E30);
	_asm call FUNC_LoadRadioLogo

	// Restore our HUD.dat path
	*(DWORD *)(CGame::GetBase() + 0x822E76) = (DWORD)(CGame::GetBase() + 0xD5B7B8);
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
		String strCredits = g_pClient->GetCredits()->GetCreditsString();

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
		SetScrollBarText(4, "autoeroticar.com       chiropracticovernight.com       ilovetoblow.org       a-thousand-words.net       libertytreeonline.com       babiesovernight.com       love-meet.net       loveyourmeat.com       nyan.cat       outsourceforamerica.com       pointclickshipabitch.com       weazelnews.com       piswasser.com       beanmachinecoffee.com       burgershot.net       electrictit.com       krapea.com       lipurgex.com       littlelacysurprisepageant.com       publiclibertyonline.com       vipluxuryringtones.com       americantravelguide.net       fruitcomputers.com       designerslave.com       erisfootware.com       eugenicsincorporated.com       flyhighpizzapie.com       gloryholethemepark.com       craplist.net       blogsnobs.org       libertycitypolice.com       sprunksoda.com       rustybrownsringdonuts.com       peepthatshit.com       lootandwank.com       myroomonline.net       dragonbrainthemovie.com       eatbiglogs.com       friendswithoutfaces.net       hornyhighschoolreunions.com       money2makemoney.com       poker-in-the-rear.com       redwoodcigarettes.com       roidsforlittleboys.com       ");
	
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

bool CGame::GetScreenPositionFromWorldPosition(CVector3 &vecWorldPosition, Vector2 &vecScreenPosition)
{
	// AFEA30 // AFF290 // B002E0
	DWORD dwAddress = (CGame::GetBase() + 0xAFF3A0);
    CVector3 * pWorldPosition = &vecWorldPosition;
    Vector2 * pScreenPosition = &vecScreenPosition;

	_asm
    {
            push pScreenPosition
            push 2					;game viewport id ; 1= mapicon
            push pWorldPosition
            call dwAddress
	}


	return g_pClient->GetCamera()->IsOnScreen(vecWorldPosition);
}

void CGame::CreateExplosion(CVector3& vecPosition, unsigned int uiExplosionType, float fRadius, bool bSound, bool bInvisible, float fCameraShake)
{
	return Scripting::AddExplosion(vecPosition.fX, vecPosition.fY, vecPosition.fZ, uiExplosionType, fRadius, bSound, bInvisible, fCameraShake);
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