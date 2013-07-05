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
#include "../ScriptingEnums.h"

// CVehicle + 0x194 = CVehicle::Fix

enum eVehicleSeat
{
	SEAT_FRONT_LEFT,
	SEAT_BACK_LEFT,
	SEAT_FRONT_RIGHT,
	SEAT_BACK_RIGHT
};

class IVDamageManager
{
public:
	PAD(IVDamageManager, pad0, 0x4); //04
	CVector3 vecDamage; 			 
};

class IVVehicleInfo
{
public:
	char sIdentifier[16];			//0x0000  
	float fMass;					//0x0010  
	float centreOfMassX;			//0x0020  
	float centreOfMassY;			//0x0024  
	float centreOfMassZ;			//0x0028  
	__int32 nPercentSubmerged;		//0x0030  
	float m_fDriveInertia;			//0x003C  
	__int16 m_nDriveGears;			//0x0040  
	float m_fDriveForce;			//0x0044   
	float m_fBrakeForce;			//0x0074   
	float m_fTractionCurveMax;		//0x008C   
	float m_fTractionCurveMin;		//0x0094   
	float m_fSuspensionForce;		//0x00B4  
	float m_fSuspensionCompDamp;	//0x00B8  
	float m_fSuspensionReboundDamp; //0x00BC  
	float m_fSuspensionUpperLimit;	//0x00C0  
	float m_fSuspensionLowerLimit;	//0x00C4  
	float m_fSuspensionRaise;		//0x00C8  
	float m_fCollisionDamageMult;	//0x00D4  
	float m_fWeaponDamageMult;		//0x00D8  
	float m_fDeformationDamageMult; //0x00DC  
	float m_fEngineDamageMult;		//0x00E0  
	float m_fSeatOffsetDist;		//0x00E4  
	__int32 m_nMonetaryValue;		//0x00E8  
	DWORD mFlags;					//0x00EC  

	/*
    float fMass;
    float fTurnMass;
    float fTractionMultiplier;
    float _fC;
    DWORD dwId;
    float _f14;
    float fDragMult;
    float _f1C;
    float CentreOfMassX;
    float CentreOfMassY;
    float CentreOfMassZ;
    float _f2C;
    int nPercentSubmerged
    float _f34;
    float _f38;
    float _f3C;
    float _f40;
    float _f44;
    float _f48;
    float _f4C;
    float _f50;
    float _f54;
    float _f58;
    float _f5C;
    float _f60;
    float _f64;
    float _f68;
    float _f6C;
    float _f70;
    float _f74;
    float _f78;
    float _f7C;
    char cDriveType_TransmissionData;
    char cEngineType_TransmissionData;
    byte nNumberOfGears_TransmissionData;
    byte _f83; / / bABS?
    float _f84;
    float fEngineInertia;
    float _f8C;
    float _f90;
    float _f94;
    float _f98;
    float _f9C;
    float fBrakeBias;
    float _fA4;
    float fSteeringLock;
    float fTractionLoss;
    float fTractionBias;
    float _fB4; / / fSuspensionHighSpdComDamp?  (Always zero)
    float fSuspensionForceLevel;
    float fSuspensionDampingLevel;
    float fSuspensionUpperLimit;
    float fSuspensionLowerLimit;
    float fSuspensionBias;
    float fSuspensionAntiDive;
    DWORD dwflags <format = hex>;
    float fSeatOffsetDistance;
    DWORD dwMonetaryValue;
    byte bFrontLights; / / (0 = long, 1 = small, 2 = big, 3 = tall)
    byte bRearLights; / / (0 = long, 1 = small, 2 = big, 3 = tall)
    byte padding [18];
   */
};

/* Maybe:
visible:
  if(true)
    *(_DWORD *)(result + 36) |= 0x20u;
  else
    *(_DWORD *)(result + 36) &= 0xFFFFFFDFu;

*(DWORD *)(pVehicle + 0x1354) --> Vehicle Type(3 = train|5 = plane|)
*(BYTE* )(pVehicle  0xF66) -> (true |= 1u;), false( &= 0FEu);

!0B7F3D0
*/

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
	PAD(IVVehicle, pad16, 0xB8); // 112C-11F0
	BYTE indicators[4];			// 11E4 - 11E8
	PAD(IVVehicle, pad17, 0x8);
	//PAD(IVVehicle, pad16, 0x88);  // 112C-11B4
	//IVDamageManager *m_pDamageManager; // 11B4-11B8
	//PAD(IVVehicle, pad17, 0x38);  // 11B8-11F0
	BYTE m_byteSirenOn[8];        // 11F0-11F8
	// 0x1130 - Unknown
	PAD(IVVehicle, pad18, 0xEC);  // 11F8-12E4
	DWORD m_dwTimeOfCreation;     // 12E4-12E8
	PAD(IVVehicle, pad19, 0x38);  // 12E8-1320
	DWORD m_dwDoorLockState;      // 1320-1324 // 0 - Unlocked, 1+ - Locked
	PAD(IVVehicle, pad20, 0x18);  // 1324-133C
	// 0x1330 - float m_fLightMultiplier;
	// 0x1337 - BYTE m_byteWeaponForVehicle;
	DWORD m_dwHorn;               // 133C-1340
	PAD(IVVehicle, pad21, 0xD90); // 1340-20D0
	// 0x14C4 - BYTE m_byteUnknownFlags; // 8 - Not Damaged Upside Down, 64 - Firing Water Cannon
	// 0x14E8 - float m_fWaterCannonOrientation;
	// 0x14EC - float m_fWaterCannonElevation;
	// 0x1510 - Vector3 m_vecWaterCannonDirection;
	// 0x1354 - eVehicleType m_type;
	// 1120-12DC Damage

	//virtual 			~IVVehicle();
	virtual void		Function72();					// m120
	virtual void		Function73(); // something with release model
	virtual void		IsPretendOccupants();			// m128
	virtual void		Function75();					// m12C
	virtual bool 		Function76(bool);				// m130
	virtual bool		Function77(bool, bool, int);	// m134
	virtual bool		Function78(int);				// m138
	virtual bool		Function79();					// m13C
	virtual bool		Function80();					// m140 returns 1 updates field_210[3340] this +0x0F1Ch
	virtual bool		Function81();					// m144
	virtual bool		Function82();					// m148
	virtual bool		Function83();					// m14C
	virtual void		Function84(int);				// m150
	virtual signed int	Function85(); // nullsub		// m154
	virtual void		Function86(int); // nullsub
	virtual int			Function87(int id, int id2, char *format, ...); // nullsub
	virtual void		Function88(int); // return 0
	virtual void		Function89();
	virtual void		Function90(int id, int id2, char *format, ...);
	virtual void		Function91(); // return 0
	virtual void		Function92(); // return 0
	virtual void		Function93(); // return 0
	virtual void		Function94(); // nullsub
	virtual void		Function95(); // nullsub
	virtual void		Function96(); // nullsub
	virtual void		Function97();
	virtual void		Function98(); // return 0;
	virtual void		Function99(); // nullsub
	virtual void		Function100();
	virtual void		Repair();
	virtual void		Function102(); // nullsub
	virtual void		Function103(); // nullsub
	virtual void		Function104();
	virtual void		Function105(); // nullsub
	virtual void		Function106();
	virtual void		Function107(); 
	virtual void		Function108();
	virtual void		Function109(); 
	virtual void		Function110(); // return -1;
};

class CIVVehicle : public CIVPhysical
{
public:
	CIVVehicle();
	CIVVehicle(IVVehicle * pVehicle);
	~CIVVehicle();

	void        SetVehicle(IVVehicle * pVehicle);
	IVVehicle * GetVehicle();

	void        SetLoadCollision(bool bLoadCollision);
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
	void        SetComponentState(BYTE byteComponent, bool bState);
	BYTE        GetTextureVariationCount();
	void        SetTextureVariation(int iVariation);
	int         GetTextureVariation();
	void        SetCanBeVisiblyDamaged(bool bState);
	bool        CanBeVisiblyDamaged();
	void		SetGPSState(bool bState);
	bool		GetGPSState();
	void		BurstCarTyre(int iTyre);
	void		GetDeformation(CVector3& vecPos);
	void		SetCarCanBeDamaged(bool bDamage);
	void		RemoveCarWindow(int iWindow);
	void		SetLightsState(int iState);
	int			GetLightsState();
	void		RemoveVehicleWindow(Scripting::eVehicleWindow window);
	bool		IsCarInWater();
	void		SetCarCanBurstTyres(bool bState);
	bool		GetCarCanBurstTyres();
};