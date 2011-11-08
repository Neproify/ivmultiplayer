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
#include "CIVPlayerPed.h"
#include "CIVPlayerInfo.h"
#include "CContextDataManager.h"

class CNetworkVehicle;

class CNetworkPlayer
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
	CPadState         m_previousPadState;
	CPadState         m_currentPadState;
	unsigned short    m_usPing;
	CNetworkVehicle * m_pVehicle;
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
	int				  m_iWorldDimension;
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
	bool                     GetKillInfo(EntityId * playerId, EntityId * vehicleId);

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

	void                     Teleport(const CVector3& vecCoordinates, bool bResetInterpolation = true);
	void                     SetPosition(const CVector3& vecCoordinates, bool bResetInterpolation);
	void                     GetPosition(CVector3& vecCoordinates);
	void                     SetCurrentHeading(float fHeading);
	float                    GetCurrentHeading();
	void                     SetDesiredHeading(float fHeading);
	float                    GetDesiredHeading();
	void                     SetMoveSpeed(const CVector3& vecMoveSpeed);
	void                     GetMoveSpeed(CVector3& vecMoveSpeed);
	void                     SetTurnSpeed(const CVector3& vecTurnSpeed);
	void                     GetTurnSpeed(CVector3& vecTurnSpeed);
	void					 GiveHelmet();
	void					 RemoveHelmet();
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
	void                     GiveMoney(int iAmount);
	void                     SetMoney(int iAmount);
	void                     ResetMoney();
	int                      GetMoney();
	void                     SetPadState(CPadState * padState);
	void                     GetPreviousPadState(CPadState * padState);
	void                     GetPadState(CPadState * padState);
	void                     SetAimSyncData(AimSyncData * aimSyncData);
	void                     GetAimSyncData(AimSyncData * aimSyncData);
	void                     AddToWorld();
	void                     RemoveFromWorld(bool bStopMoving = true);
	void                     SetInterior(unsigned int uiInterior);
	unsigned int             GetInterior();
	void                     UpdateTargetPosition();
	void                     Interpolate();
	void                     SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay);
	void                     RemoveTargetPosition();
	void                     ResetInterpolation();
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

	//
	bool                     GetClosestVehicle(bool bPassenger, CNetworkVehicle ** pVehicle, BYTE &byteSeatId);
	void                     EnterVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId);
	void                     ExitVehicle();
	void                     PutInVehicle(CNetworkVehicle * pVehicle, BYTE byteSeatId);
	void                     RemoveFromVehicle();
	void                     CheckVehicleEntryExitKey();
	void                     ProcessVehicleEntryExit();
	void                     ResetVehicleEnterExit();
	bool                     HasVehicleEnterExit() { return (m_vehicleEnterExit.bEntering || m_vehicleEnterExit.bExiting); }
	void                     SetRequestingVehicleEnterExit(bool bRequesting) { m_vehicleEnterExit.bRequesting = bRequesting; }
	bool                     GetRequestingVehicleEnterExit() { return m_vehicleEnterExit.bRequesting; }

	void                     ToggleRagdoll(bool bToggle);
	bool                     IsOnScreen();

	void					 SetWorldDimensions ( int iWorldDimension );
};
