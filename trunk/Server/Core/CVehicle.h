//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicle.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

class CPlayer;

class CVehicle : public CVehicleInterface
{
private:
	EntityId      m_vehicleId;
	CPlayer     * m_pDriver;
	CPlayer     * m_pPassengers[MAX_VEHICLE_PASSENGERS];
	int           m_iModelId;
	unsigned int  m_uiHealth;
	float		  m_fPetrolTankHealth;
	CVector3      m_vecSpawnPosition;
	CVector3      m_vecPosition;
	CVector3      m_vecSpawnRotation;
	CVector3      m_vecRotation;
	CVector3      m_vecTurnSpeed;
	CVector3      m_vecMoveSpeed;
	BYTE          m_byteSpawnColors[4];
	BYTE          m_byteColors[4];
	float         m_fDirtLevel;
	int           m_iHornDuration;
	bool          m_bSirenState;
	unsigned int  m_iLocked;
	bool          m_bIndicatorState[4];
	bool          m_bComponents[9];
	unsigned char m_ucVariation;
	bool		  m_bEngineStatus;
	bool		  m_bLights;
	float		  m_fDoor[6];
	bool		  m_bWindow[4];
	bool		  m_bTaxiLight;
	bool		  m_bTyre[6];

public:
	CVehicle(EntityId vehicleId, int iModelId, CVector3 vecSpawnPosition, CVector3 vecSpawnRotation, BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4);
	~CVehicle();

	EntityId      GetVehicleId() { return m_vehicleId; }
	void          Reset();
	void          SpawnForPlayer(EntityId playerId);
	void          DestroyForPlayer(EntityId playerId);
	void          SpawnForWorld();
	void          DestroyForWorld();
	bool          IsOccupied();
	void          SetDriver(CPlayer * pDriver) { m_pDriver = pDriver; }
	CPlayer     * GetDriver() { return m_pDriver; }
	void          SetPassenger(BYTE byteSeatId, CPlayer * pPassenger);
	CPlayer     * GetPassenger(BYTE byteSeatId);
	void          SetOccupant(BYTE byteSeatId, CPlayer * pOccupant);
	CPlayer     * GetOccupant(BYTE byteSeatId);
	void          Respawn();
	void          StoreInVehicleSync(InVehicleSyncData * syncPacket);
	void          StorePassengerSync(PassengerSyncData * syncPacket);
	void		  StoreEmptyVehicle(EMPTYVEHICLESYNCPACKET * syncPacket);
	void          SetPassengerId(BYTE byteSeatId, EntityId passengerId);
	void          SetModel(int iModelId);
	int           GetModel();
	void          SetHealth(unsigned int uHealth);
	unsigned int  GetHealth();
	void		  SetPetrolTankHealth(float fHealth);
	float		  GetPetrolTankHealth();
	void          SetPosition(const CVector3& vecPosition);
	void          GetPosition(CVector3& vecPosition);
	void          SetRotation(const CVector3& vecRotation);
	void          GetRotation(CVector3& vecRotation);
	void          SetDirtLevel(float fDirtLevel);
	float         GetDirtLevel();
	void          SetTurnSpeed(const CVector3& vecTurnSpeed);
	void          GetTurnSpeed(CVector3& vecTurnSpeed);
	void          SetMoveSpeed(const CVector3& vecMoveSpeed);
	void          GetMoveSpeed(CVector3& vecMoveSpeed);
	void          SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4);
	void          GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4);
	void          SoundHorn(unsigned int iDuration);
	void          SetSirenState(bool bSirenState);
	bool          GetSirenState();
	bool          SetLocked(unsigned int uiLocked);
	unsigned int  GetLocked();
	void          SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight);
	bool          GetIndicatorState(unsigned char ucSlot);
	void          SetComponentState(unsigned char ucSlot, bool bOn);
	bool          GetComponentState(unsigned char ucSlot);
	void          ResetComponents(bool bNotify = true);
	void          SetVariation(unsigned char ucVariation);
	unsigned char GetVariation();
	void          SetEngineStatus(bool bEngineStatus);
	bool          GetEngineStatus();
	void		  TurnTaxiLights(bool bTaxilights);
	bool		  GetTaxiLights();
	void		  RepairWindows();
	void		  RepairWheels();
	void		  SetCarDoorAngle(unsigned int uiDoor, bool bClosed, float fAngle);
	void		  SetLights(bool blights);
	bool		  GetLights();
	bool		  GetWindowState(unsigned int uiWindow);
	void		  SetWindowState(unsigned int uiWindow, bool bState);
	bool		  GetTyreState(unsigned int uiTyre);
	void		  SetTyreState(unsigned int uiTyre, bool bState);
};
