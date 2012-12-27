//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkPlayer.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"
#include "CStreamer.h"
#include "CIVPlayerPed.h"
#include "CIVPlayerInfo.h"
#include "CContextDataManager.h"
#include "CIVTask.h"
#include "IVTasks.h"

class CNetworkVehicle;

enum eExitVehicleMode
{
	EXIT_VEHICLE_NORMAL,
	EXIT_VEHICLE_JACKED
};

class CNetworkPlayer : public CStreamableEntity
{
private:
	EntityId          m_playerId;
	bool              m_bIsLocalPlayer;
	unsigned int      m_byteGamePlayerNumber;
	CIVPlayerPed    * m_pPlayerPed;
	CIVPlayerInfo   * m_pPlayerInfo;
	CContextData    * m_pContextData;
	String            m_strName;
	CIVModelInfo    * m_pModelInfo;
	bool              m_bSpawned;
	bool              m_bHelmet;
	bool			  m_bUseMobilePhone;
	unsigned int      m_uiColor;
	struct
	{
		struct
		{
			CVector3      vecStart;
			CVector3      vecTarget;
			CVector3      vecError;
			float         fLastAlpha;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} pos;
	}                 m_interp;
	unsigned char     m_ucClothes[11];
	bool              m_bUseCustomClothesOnSpawn;
	CControlState     m_previousControlState;
	CControlState     m_currentControlState;
	CVector3          m_vecAimTarget;
	CVector3          m_vecShotSource;
	CVector3          m_vecShotTarget;
	unsigned short    m_usPing;
	CNetworkVehicle * m_pVehicle;
	bool			  m_bVehicleDeathCheck;
	BYTE              m_byteVehicleSeatId;
	struct
	{
		bool              bEntering;
		CNetworkVehicle * pVehicle;
		BYTE              byteSeatId;
		bool              bExiting;
		bool              bRequesting;
	}                 m_vehicleEnterExit;
	bool              m_bHealthLocked;
	unsigned int      m_uiLockedHealth;
	bool              m_bArmourLocked;
	unsigned int      m_uiLockedArmour;
	bool			  m_bPlayerBlipCreated;
	unsigned int	  m_uiPlayerBlipHandle;
	unsigned int	  m_iPlayerBlipSprite;

	// Streaming info
	unsigned int	  m_uiHealth;
	CVector3		  m_vecPos;

	// Task info
	CIVTaskSimpleStartWalking * m_pOldTask;

public:
	CNetworkPlayer(bool bIsLocalPlayer = false);
	~CNetworkPlayer();

	bool                     IsNetworkPlayer() { return (m_playerId != INVALID_ENTITY_ID); }
	EntityId                 GetPlayerId() { return m_playerId; }
	void                     SetPlayerId(EntityId playerId) { m_playerId = playerId; }
	bool                     IsLocalPlayer() { return m_bIsLocalPlayer; }
	BYTE                     GetGamePlayerNumber() { return m_byteGamePlayerNumber; }
	CIVPlayerPed           * GetGamePlayerPed() { return m_pPlayerPed; }
	CIVPlayerInfo          * GetGamePlayerInfo() { return m_pPlayerInfo; }
	CIVModelInfo           * GetModelInfo() { return m_pModelInfo; }
	bool                     IsSpawned() { return m_bSpawned; }
	void                     SetSpawned(bool bSpawned) { m_bSpawned = bSpawned; } 
	void                     SetName(String strName);
	String                   GetName() { return m_strName; }
	void                     SetPing(unsigned short usPing) { m_usPing = usPing; }
	unsigned short           GetPing() { return m_usPing; }
	virtual bool             Create();
	virtual void             Init();
	virtual void             Destroy();

	//
	virtual void             Kill(bool bInstantly = false);
	bool                     IsDying();
	bool                     IsDead();
	IVEntity               * GetLastDamageEntity();
	bool                     GetKillInfo(EntityId * playerId, EntityId * vehicleId, EntityId * weaponId);

	//
	bool                     IsMoving();
	void                     StopMoving();

	//
	bool                     InternalIsInVehicle();
	CNetworkVehicle        * InternalGetVehicle();
	void                     InternalPutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId);
	void                     InternalRemoveFromVehicle();
	bool                     IsOnFoot() { return (m_pVehicle == NULL); }
	bool                     IsInVehicle() { return (m_pVehicle != NULL); }
	CNetworkVehicle        * GetVehicle() { return m_pVehicle; }
	BYTE                     GetVehicleSeatId() { return m_byteVehicleSeatId; }
	CNetworkVehicle        * GetEnterExitVehicle() { return m_vehicleEnterExit.pVehicle; }
	bool                     IsAPassenger() { return (m_pVehicle != NULL && m_byteVehicleSeatId != 0); }

	unsigned int             GetScriptingHandle();

	//
	void                     SetModel(DWORD dwModelHash);

	void                     Teleport(const CVector3& vecPosition, bool bResetInterpolation = true);

	void                     SetPosition(const CVector3& vecPosition, bool bResetInterpolation = true);
	void                     GetPosition(CVector3& vecPosition);

	void                     SetCurrentHeading(float fHeading);
	float                    GetCurrentHeading();

	void					 SetCurrentSyncHeading(float fHeading);

	void                     SetDesiredHeading(float fHeading);
	float                    GetDesiredHeading();

	void					 SetBonePosition(CVector3 vecBones);
	CVector3				 GetBonePosition(int iBone);

	void                     SetMoveSpeed(const CVector3& vecMoveSpeed);
	void                     GetMoveSpeed(CVector3& vecMoveSpeed);

	void                     SetTurnSpeed(const CVector3& vecTurnSpeed);
	void                     GetTurnSpeed(CVector3& vecTurnSpeed);

	void					 GiveHelmet();
	void					 RemoveHelmet();

	void					 SetHelmet(bool helmet);
	void                     SetHealth(unsigned int uiHealth);
	void                     LockHealth(unsigned int uiHealth);
	unsigned int             GetHealth();

	void                     SetArmour(unsigned int uiArmour);
	void                     LockArmour(unsigned int uiArmour);
	unsigned int             GetArmour();

	void                     GiveWeapon(unsigned int uiWeaponId, unsigned int uiAmmo);
	void                     RemoveWeapon(unsigned int uiWeaponId);
	void                     RemoveAllWeapons();
	void                     SetCurrentWeapon(unsigned int uiWeaponId);
	unsigned int             GetCurrentWeapon();
	void                     SetAmmo(unsigned int uiWeaponId, unsigned int uiAmmo);
	unsigned int             GetAmmo(unsigned int uiWeaponId);
	void					 GetWeaponInSlot(unsigned int uiWeaponSlot, unsigned int &uiWeaponId, unsigned int &uiAmmo, unsigned int &uiUnknown);
	unsigned int             GetAmmoInClip(unsigned int uiWeaponId);
	void                     SetAmmoInClip(unsigned int uiAmmoInClip);
	unsigned int             GetMaxAmmoInClip(unsigned int uiWeaponId);

	void                     GiveMoney(int iAmount);
	void                     SetMoney(int iAmount);
	void                     ResetMoney();
	int                      GetMoney();

	void                     SetControlState(CControlState * controlState);
	void                     GetPreviousControlState(CControlState * controlState);
	void                     GetControlState(CControlState * controlState);

	void                     SetAimTarget(const CVector3& vecAimTarget);
	void                     GetAimTarget(CVector3& vecAimTarget);
	void                     SetShotSource(const CVector3& vecShotSource);
	void                     GetShotSource(CVector3& vecShotSource);
	void                     SetShotTarget(const CVector3& vecShotTarget);
	void                     GetShotTarget(CVector3& vecShotTarget);
	void                     SetAimSyncData(AimSyncData * aimSyncData);
	void                     GetAimSyncData(AimSyncData * aimSyncData);

	void                     AddToWorld();
	void                     RemoveFromWorld(bool bStopMoving = true);

	void                     SetInterior(unsigned int uiInterior);
	unsigned int             GetInterior();

	void                     UpdateTargetPosition();

	void                     Interpolate();
	void                     ResetInterpolation();

	void                     SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay);
	void					 SetMoveToDirection(CVector3 vecPos, CVector3 vecMove, int iMoveType);

	void                     RemoveTargetPosition();

	bool                     HasTargetPosition() { return (m_interp.pos.ulFinishTime != 0); }

	virtual void             SetColor(unsigned int uiColor);
	unsigned int             GetColor();

	void                     SetClothes(unsigned char ucBodyPart, unsigned char ucClothes);
	unsigned char            GetClothes(unsigned char ucBodyPart);

	void                     SetDucking(bool bDucking);
	bool                     IsDucking();

	void                     SetCameraBehind();
	virtual void             Pulse(); 

	// 
	bool                     IsGettingInToAVehicle();
	bool                     IsGettingOutOfAVehicle();
	bool                     IsJackingAVehicle();
	bool                     IsGettingJackedFromVehicle();
	bool                     ClearVehicleEntryTask();
	bool                     ClearVehicleExitTask();
	bool					 ClearDieTask();

	//
	bool                     GetClosestVehicle(bool bPassenger, CNetworkVehicle ** pVehicle, BYTE &byteSeatId);
	void                     EnterVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId);
	void                     ExitVehicle(eExitVehicleMode exitmode);
	void                     PutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId);
	void                     RemoveFromVehicle();
	void                     CheckVehicleEntryExitKey();
	void                     ProcessVehicleEntryExit();
	void                     ResetVehicleEnterExit();
	bool                     HasVehicleEnterExit() { return (m_vehicleEnterExit.bEntering || m_vehicleEnterExit.bExiting); }
	void                     SetRequestingVehicleEnterExit(bool bRequesting) { m_vehicleEnterExit.bRequesting = bRequesting; }
	bool                     GetRequestingVehicleEnterExit() { return m_vehicleEnterExit.bRequesting; }

	//
	void                     ToggleRagdoll(bool bToggle);
	bool                     IsOnScreen();

	//
	void					 SetBlip(unsigned int uiHandle) { m_uiPlayerBlipHandle = uiHandle; }
	unsigned int			 GetBlip() { return m_uiPlayerBlipHandle; }

	void					 SetBlipSprite(int iSprite) { m_iPlayerBlipSprite = iSprite; }
	int						 GetBlipSprite() { return m_iPlayerBlipSprite; }

	void					 SetBlipActive(bool bActive) { m_bPlayerBlipCreated = bActive; }
	bool					 GetBlipActivity() { return m_bPlayerBlipCreated; }

	//
	void					 TaskLookAtCoord(float fX, float fY, float fZ);

	//
	void					 UseMobilePhone(bool bUse);

	// Streaming
	void					 StreamIn();
	void					 StreamOut();
	void					 GetStreamPosition(CVector3& position) { GetPosition(position); }
};
