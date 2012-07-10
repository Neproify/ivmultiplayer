//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkVehicle.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CLocalPlayer.h"
#include "CStreamer.h"

class CNetworkVehicle : public CStreamableEntity
{
private:
	CIVVehicle     * m_pVehicle;
	EntityId         m_vehicleId;
	CNetworkPlayer * m_pDriver;
	CNetworkPlayer * m_pPassengers[MAX_VEHICLE_PASSENGERS];
	CIVModelInfo   * m_pModelInfo;
	CVector3         m_vecPosition;
	CVector3         m_vecRotation;
	CVector3         m_vecMoveSpeed;
	CVector3         m_vecTurnSpeed;
	BYTE             m_byteColors[4];
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
		struct
		{
			CVector3      vecStart;
			CVector3      vecTarget;
			CVector3      vecError;
			float         fLastAlpha;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} rot;
	}                m_interp;
	bool             m_bSirenState;
	bool             m_bIndicatorState[4];
	bool             m_bComponents[9];
	DWORD            m_dwDoorLockState;
	unsigned long    m_ulHornDurationEnd;
	unsigned int     m_uiHealth;
	float            m_fDirtLevel;
	float            m_fPetrolTankHealth;
	unsigned char    m_ucVariation;
	bool			 m_bEngineStatus;
	bool			 m_bTaxiLights;
	float			 m_fDoor[6];
	bool			 m_bLights;
	bool			 m_bWindow[4];

	bool             Create();
	void             Destroy();

public:
	CNetworkVehicle(DWORD dwModelHash);
	~CNetworkVehicle();

	bool             IsNetworkVehicle() { return (m_vehicleId != INVALID_ENTITY_ID); }

	void             SetVehicleId(EntityId vehicleId) { m_vehicleId = vehicleId; }
	EntityId         GetVehicleId() { return m_vehicleId; }

	CIVVehicle *     GetGameVehicle() { return m_pVehicle; }
	CIVModelInfo   * GetModelInfo() { return m_pModelInfo; }

	bool             IsSpawned() { return (m_pVehicle != NULL); }
	bool             IsOccupied();

	void             SetDriver(CNetworkPlayer * pDriver) { m_pDriver = pDriver; }
	CNetworkPlayer * GetDriver() { return m_pDriver; }

	void             SetPassenger(BYTE bytePassengerId, CNetworkPlayer * pPassenger);
	CNetworkPlayer * GetPassenger(BYTE bytePassengerId);
	BYTE             GetMaxPassengers();

	void             SetOccupant(BYTE byteSeatId, CNetworkPlayer * pOccupant);
	CNetworkPlayer * GetOccupant(BYTE byteSeatId);

	void             StreamIn();
	void             StreamOut();
	void             GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }

	void             AddToWorld();
	void             RemoveFromWorld(bool bStopMoving = true);

	bool             IsMoving();
	void             StopMoving();

	unsigned int     GetScriptingHandle();

	void             SetModel(DWORD dwModelHash);

	void             SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4);
	void             GetColors(BYTE &byteColor1, BYTE &byteColor2, BYTE &byteColor3, BYTE &byteColor4);

	void             SetPosition(const CVector3& vecPosition, bool bDontCancelTasks = false, bool bResetInterpolation = true);
	void             GetPosition(CVector3& vecPosition);

	void             SetRotation(const CVector3& vecRotation, bool bResetInterpolation = true);
	void             GetRotation(CVector3& vecRotation);

	void             SetHealth(unsigned int uiHealth);
	unsigned int     GetHealth();

	void			 SetPetrolTankHealth(float fHealth);
	float			 GetPetrolTankHealth();

	void             SetMoveSpeed(const CVector3& vecMoveSpeed);
	void             GetMoveSpeed(CVector3& vecMoveSpeed);

	void             SetTurnSpeed(const CVector3& vecTurnSpeed);
	void             GetTurnSpeed(CVector3& vecTurnSpeed);

	void             SetSirenState(bool bSirenState);
	bool             GetSirenState();

	void             SetDirtLevel(float fDirtLevel);
	float            GetDirtLevel();

	void             StoreEmptySync(EMPTYVEHICLESYNCPACKET * emptyVehicleSync);
	
	void             SetDoorLockState(DWORD dwDoorLockState);
	DWORD            GetDoorLockState();

	void             SoundHorn(int iDuration);

	void             Interpolate();
	void             ResetInterpolation();

	void             Pulse();
	void             UpdateTargetPosition();
	void             UpdateTargetRotation();
	
	void             SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay);
	void             SetTargetRotation(const CVector3& vecRotation, unsigned long ulDelay);
	void             RemoveTargetPosition();
	void             RemoveTargetRotation();
	
	bool             HasTargetPosition() { return (m_interp.pos.ulFinishTime != 0); }
	bool             HasTargetRotation() { return (m_interp.rot.ulFinishTime != 0); }

	void             SetInterior(unsigned int uiInterior);
	unsigned int     GetInterior();

	void             SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight);
	bool             GetIndicatorState(unsigned char ucSlot);

	void             SetComponentState(unsigned char ucSlot, bool bOn);
	bool             GetComponentState(unsigned char ucSlot);

	void             SetVariation(unsigned char ucVariation);
	unsigned char    GetVariation();

	bool             IsOnScreen();

	void             SetSteeringAngle(float fSteeringAngle);
	float            GetSteeringAngle();

	void			 SetEngineState(bool bState);
	bool			 GetEngineState();

	void		     SetTaxiLightsState(bool bState);
	bool			 GetTaxiLightsState();

	void			 SetCarDoorAngle(int iDoor, bool bClose, float fAngle);
	float			 GetCarDoorAngle(int iDoor);

	void			 SetLightsState(bool bLights);
	bool			 GetLightsState();

	bool			 GetWindowState(int iWindow);
	void			 SetWindowState(int iWindow, bool bBroken);

	void			 SetDeformation(CVector3 vecPos, CVector3 vecDeformation);
	CVector3		 GetDeformation(CVector3 vecPos);

	void			 SetDamageable(bool bToggle);
};
