//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVVehicle.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVPhysical.h"
#include "CIVPed.h"

// CVehicle + 0x194 = CVehicle::Fix

enum eVehicleSeat
{
	SEAT_FRONT_LEFT,
	SEAT_BACK_LEFT,
	SEAT_FRONT_RIGHT,
	SEAT_BACK_RIGHT
};

class IVVehicle : public IVPhysical
{
public:
	// 0000-0210
	// 0x118 - DWORD m_dwUnknownFlags; // 1024 - Invincible (must be some CPhysical member)
	// 0x34C - WORD m_wModelIndex2
	PAD(IVVehicle, pad0, 0xB50);  // 0210-0D60
	// 0x260 - Audio? (+0xB39 - BYTE m_byteLoudRadio)
	BYTE m_byteRadioStation;      // 0D60-0D61
	PAD(IVVehicle, pad1, 0x37);   // 0D61-0D98
	BYTE m_byteSirenWithNoDriver; // 0D98-0D99
	PAD(IVVehicle, pad2, 0x1CB);  // 0D99-0F64
	BYTE m_byteFlags1;            // 0F64-0F65 // 128 - Handbrake On
	BYTE m_byteFlags2;            // 0F65-0F66 // 1 - Lights On, 2 - Freebies
	BYTE m_byteFlags3;            // 0F66-0F67 // 1 - Takes Less Damage
	BYTE m_byteFlags4;            // 0F67-0F68 // 1 - Part Of Convoy, 8 - Is Drowning
	BYTE m_byteFlags5;            // 0F68-0F69 // 64 - Can Be Targeted
	PAD(IVVehicle, pad3, 0x3);    // 0F69-0F6C
	BYTE m_byteFlags6;            // 0F6C-0F6D // 2 - Hazard Lights Flashing, 4 - Hazard Lights Constant, 16 - Interior Lights
	BYTE m_byteFlags7;            // 0F6D-0F6E // 64 - Pretend Occupants
	PAD(IVVehicle, pad4, 0x1);    // 0F6E-0F6F
	BYTE m_byteFlags8;            // 0F6F-0F70 // 32 - Is Police Vehicle
	BYTE m_byteFlags9;            // 0F70-0F71 // 4 - Can Be Visibly Damaged
	PAD(IVVehicle, pad5, 0x3);    // 0F71-0F74
	DWORD m_dwTimeOfCreation2;    // 0F74-0F78
	PAD(IVVehicle, pad6, 0x20);   // 0F78-0F98
	WORD m_wAlarm;                // 0F98-0F9A
	PAD(IVVehicle, pad7, 0x6);    // 0F9A-0FA0
	IVPed * m_pDriver;            // 0FA0-0FA4
	IVPed * m_pPassengers[8];     // 0FA4-0FC4
	// 0xFDC - DWORD m_byteBoneCount;
	PAD(IVVehicle, pad8, 0x20);   // 0FC4-0FE4
	BYTE m_byteColors[4];         // 0FE4-0FE8
	PAD(IVVehicle, pad9, 0xD8);   // 0FE8-10C0
	BYTE m_byteMaxPassengers;     // 10C0-10C1
	PAD(IVVehicle, pad10, 0x7);   // 10C1-10C8
	float m_fGasPedal;            // 10C8-10CC
	float m_fBrakePedal;          // 10CC-10D0
	PAD(IVVehicle, pad11, 0x4);   // 10D0-10D4
	float m_fSteerBias;           // 10D4-10D8
	float m_fSteering;            // 10D8-10DC
	PAD(IVVehicle, pad12, 0x20);  // 10DC-10FC
	float m_fEngineHealth;        // 10FC-1100
	PAD(IVVehicle, pad13, 0x8);   // 1100-1108
	BYTE m_byteCreatedBy;         // 1108-1109
	PAD(IVVehicle, pad14, 0xF);   // 1109-1118
	float m_fDirtLevel;           // 1118-111C
	PAD(IVVehicle, pad15, 0x4);   // 111C-1120
	BYTE m_byteSirenHealth[8];    // 1120-1128
	float m_fPetrolTankHealth;    // 1128-112C
	PAD(IVVehicle, pad16, 0xC4);  // 112C-11F0
	BYTE m_byteSirenOn[8];        // 11F0-11F8
	// 0x1130 - Damage Manager?
	PAD(IVVehicle, pad17, 0xEC);  // 11F8-12E4
	DWORD m_dwTimeOfCreation;     // 12E4-12E8
	PAD(IVVehicle, pad18, 0x38);  // 12E8-1320
	DWORD m_dwDoorLockState;      // 1320-1324 // 0 - Unlocked, 1+ - Locked
	PAD(IVVehicle, pad19, 0x18);  // 1324-133C
	// 0x1330 - float m_fLightMultiplier;
	DWORD m_dwHorn;               // 133C-1340
	PAD(IVVehicle, pad20, 0xD90); // 1340-20D0
	// 0x14C4 - BYTE m_byteUnknownFlags; // 8 - Not Damaged Upside Down, 64 - Firing Water Cannon
	// 0x14E8 - float m_fWaterCannonOrientation;
	// 0x14EC - float m_fWaterCannonElevation;
	// 0x1510 - Vector3 m_vecWaterCannonDirection;
	// 0x1354 - eVehicleType m_type;
	// 1120-12DC Damage
	// 1112 Lights On (word i *think*)
};

class CIVVehicle : public CIVPhysical
{
public:
	CIVVehicle();
	CIVVehicle(IVVehicle * pVehicle);
	~CIVVehicle();

	void        SetVehicle(IVVehicle * pVehicle);
	IVVehicle * GetVehicle();

	void        SetAlarmDuration(WORD wDuration);
	WORD        GetAlarmDuration();
	void        SetDriver(IVPed * pPed);
	IVPed *     GetDriver();
	void        SetPassenger(BYTE bytePassengerSeatId, IVPed * pPassenger);
	IVPed *     GetPassenger(BYTE bytePassengerSeatId);
	void        SetColors(unsigned char ucColor1, unsigned char ucColor2, unsigned char ucColor3, unsigned char ucColor4);
	void        GetColors(unsigned char * ucColor1, unsigned char * ucColor2, unsigned char * ucColor3, unsigned char * ucColor4);
	void        SetMaxPassengers(BYTE byteMaxPassengers);
	BYTE        GetMaxPasssengers();
	bool        HasSiren();
	void        SetSirenState(bool bStatus);
	bool        GetSirenState();
	void        SetGasPedal(float fGasPedal);
	float       GetGasPedal();
	void        SetBrakePedal(float fBrakePedal);
	float       GetBrakePedal();
	void        SetSteerBias(float fSteerBias);
	float       GetSteerBias();
	void        SetSteeringAngle(float fSteeringAngle);
	float       GetSteeringAngle();
	void        SetEngineHealth(float fEngineHealth);
	float       GetEngineHealth();
	void        SetDirtLevel(float fDirtLevel);
	float       GetDirtLevel();
	void        SetPetrolTankHealth(float fPetrolTankHealth);
	float       GetPetrolTankHealth();
	void        SetDoorLockState(DWORD dwDoorLockState);
	DWORD       GetDoorLockState();
	void        SetEngineStatus(bool bStatus, bool bUnknown);
	bool        GetEngineStatus();
	void        SoundHorn(int iDuration);
	void        SetComponentState(int iComponent, bool bState);
	BYTE        GetTextureVariationCount();
	void        SetTextureVariation(int iVariation);
	int         GetTextureVariation();
	void        SetCanBeVisiblyDamaged(bool bState);
	bool        CanBeVisiblyDamaged();
	void		SetGPSState(bool bState);
	bool		GetGPSState();
};
