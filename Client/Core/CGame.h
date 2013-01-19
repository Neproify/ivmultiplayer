//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGame.h
// Project: Client.Core
// Author(s): jenksta
//            aru
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <Common.h>
#include <Math/CMath.h>
#include <CString.h>
#include "CIVPad.h"
#include "CIVModelInfo.h"
#include "CIVWeaponInfo.h"
#include "CIVStreaming.h"
#include "CIVWeather.h"
#include "CIVWorld.h"
#include "Patcher/CPatcher.h"
#include <CLogFile.h>

enum eState
{
	GAME_STATE_NONE,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_LOADING,
	GAME_STATE_INGAME,
	GAME_STATE_PAUSE_MENU,
	GAME_STATE_IVMP_PAUSE_MENU,
};

// Pointer template
typedef void * ptr;

template <typename T>
inline T *ptr_cast(unsigned int value)
{
	return *reinterpret_cast<T **>(&value);
}

inline ptr ptr_cast(unsigned int value)
{
	return *reinterpret_cast<ptr *>(&value);
}

template <class T>
inline T * ptr_cast(ptr value)
{
	return reinterpret_cast<T *>(value);
}

class CPools;
class IVEntity;
struct Vector2;

class CGame
{
private:
	static unsigned int   m_uiBaseAddress;
	static bool           m_bInputState;
	static eState         m_State;
	static bool           m_bFocused;
	static CPools       * m_pPools;
	static CIVPad       * m_pPad;
	static CIVModelInfo   m_modelInfos[NUM_ModelInfos];
	static CIVWeaponInfo  m_weaponInfos[NUM_WeaponInfos];
	static CIVStreaming * m_pStreaming;
	static CIVWeather   * m_pWeather;
	static CIVWorld     * m_pWorld;
	static bool			  m_bNameTags;
	static bool			  m_bGameLoaded;
	static bool			  m_bKickedFromServer;
	static bool			  m_bHeadMovement;
	static bool			  m_bSpecialData[2];

public:
	static unsigned int    GetBase() { return m_uiBaseAddress; }
	static void            SetInputState(bool bState) { m_bInputState = bState; }
	static bool            GetInputState() { return m_bInputState; }
	static void            SetState(eState State) { m_State = State; }
	static eState          GetState() { return m_State; }
	static void            SetFocused(bool bFocused);
	static bool            IsFocused() { return m_bFocused; }
	static CPools        * GetPools() { return m_pPools; }
	static CIVPad        * GetPad() { return m_pPad; }
	static CIVModelInfo  * GetModelInfo(int iModelIndex);
	static unsigned int    GetLoadedModelCount();
	static unsigned int    GetUnloadedModelCount();
	static CIVWeaponInfo * GetWeaponInfo(eWeaponType weaponType);
	static CIVStreaming  * GetStreaming() { return m_pStreaming; }
	static CIVWeather    * GetWeather() { return m_pWeather; }
	static CIVWorld      * GetWorld() { return m_pWorld; }
	static HWND            GetHWnd();
	static DWORD           GetTime();
	static void            Initialize();
	static void            Shutdown();
	static bool            Patch();
	static bool            IsMenuActive();
	static void            SetTime(int uHour, int uMinute);
	static void            SetDayOfWeek(int uDay);
	static void            LockTime(int uHour, int uMinute);
	static void            GetTime(int * uHour, int * uMinute);
	static void            SetWantedLevel(DWORD dwLevel);
	static DWORD           GetWantedLevel();
	static void            SetHudVisible(bool bVisible);
	static bool            IsHudVisible();
	static void            LoadHUD(String strPath = "common:/DATA/HUD.DAT");
	static void            LoadRadioLogo(String strPath = "common:/DATA/RadioLogo.DAT");
	static void            SetRadarVisible(bool bVisible);
	static bool            IsRadarVisible();
	static void            SetPlayerNamesVisible(BYTE byteVisible);
	static BYTE            ArePlayerNamesVisible();
	static void            SetAreaNamesEnabled(bool bEnabled);
	static bool            AreAreaNamesEnabled();
	static void            ConvertRotationMatrixToEulerAngles(Matrix &matRotation, CVector3 &vecRotation);
	static void            ConvertEulerAnglesToRotationMatrix(CVector3 &vecRotation, Matrix &matRotation);
	/*static void            ConvertQuaternionToRotationMatrix(Quaternion * quatRotation, GameMatrix34 * matRotation);
	static void            ConvertRotationMatrixToQuaternion(GameMatrix34 * matRotation, Quaternion * quatRotation);*/
	static bool            SetScrollBarText(unsigned char ucScrollbar, const char* szText);
	static void            ResetScrollBars(unsigned char ucScrollbar = 0xFF);
	static void            GetScrollBarText(unsigned char ucScrollbar, char* szReturn);
	static void            SetScrollBarColor(unsigned int uiColor = 0xFF8000FF);
	static unsigned int    GetScrollBarColor();
	static String          GetCurrentAreaName();
	static String          GetCurrentStreetName();
	static void            RemoveInitialLoadingScreens();
	static bool            GetScreenPositionFromWorldPosition(CVector3 vecWorldPosition, Vector2 &vecScreenPosition);
	static void            CreateExplosion(CVector3& vecPosition, unsigned int uiExplosionType, float fRadius, bool bSound = true, bool bInvisible = false, float fCameraShake = 0.0f);
	static unsigned int    CreateFire(CVector3 vecPosition, unsigned int uiStrength = 1, unsigned char ucAllowedGenerations = 0);
	static bool            DeleteFire(unsigned int uiFire);
	static void          * Alloc(DWORD dwSize);
	static void            Free(void * pMemory);
	static void            ToggleLazlowStation(bool bToggle);
	static void            InitCutscene(char * szCutsceneName, bool bUnknown1 = false, bool bUnknown2 = false);
	static void            StopCutscene(char * szCutsceneName);
	static void			   SetNameTags(bool bNametags) { m_bNameTags = bNametags; CLogFile::Printf("SETENABLED: %d",bNametags); }
	static bool			   GetNameTags() { return m_bNameTags; }
	static void			   SetHeadMovement(bool bMove) { m_bHeadMovement = bMove; }
	static bool			   GetHeadMovement() { return m_bHeadMovement; }
	static void			   SetGameLoaded(bool load) { m_bGameLoaded = load; }
	static bool			   IsGameLoaded() { return m_bGameLoaded; }
	// jenksta: this isn't related to the game, TODO: move it!
	static void			   SetKickedFromServer(bool bKick) { m_bKickedFromServer = bKick; }
	static bool			   IsKickedFromServer() { return m_bKickedFromServer; }
	static DWORD		   GetNativeAddress(DWORD dwNative);
	static void			   PatchWorldAndTrain();
	static void			   InitializeDefaultGameComponents();
	static void			   SetSpecialData(bool bData1, bool bData2) { m_bSpecialData[0] = bData1; m_bSpecialData[1] = bData2; }
	static bool			   GetSpecialData(int iData) { return m_bSpecialData[iData]; }
};
