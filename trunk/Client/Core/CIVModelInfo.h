//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVModelInfo.h
// Project: Client.Core
// Author(s): jenksta
//            Multi Theft Auto Team
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IVCommon.h"

#define NUM_ModelInfos 65535
#define ARRAY_ModelInfos 0x15F73B0
// void CModelInfo::SetAnimation(this, char * szAnimationName)
#define FUNC_CModelInfo__SetAnimation 0x98F3F0

enum eModelType
{
	MODEL_TYPE_BASE = 1, // Base and Instance
	MODEL_TYPE_MLO = 2,
	MODEL_TYPE_TIME = 3,
	MODEL_TYPE_WEAPON = 4,
	MODEL_TYPE_VEHICLE = 5,
	MODEL_TYPE_PED = 6
};

enum eVehicleType
{
	VEHICLE_TYPE_AUTOMOBILE,
	VEHICLE_TYPE_BIKE,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_TRAIN,
	VEHICLE_TYPE_HELI,
	VEHICLE_TYPE_PLANE
};

// TODO: eModel enum
#define MODEL_ADMIRAL_INDEX 84
#define MODEL_PLAYER_INDEX 211

class IVBaseModelInfoVFTable
{
public:
	DWORD ScalarDeletingDestructor;
	DWORD m4;
	DWORD m8;
	DWORD GetType;
	DWORD m10;
	DWORD m14;
	DWORD m18;
	DWORD m1C;
	DWORD m20;
	DWORD m24;
	DWORD m28;
	DWORD m2C;
	DWORD m30;
	DWORD m34;
	DWORD m38;
};

class IVInstanceModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVMloModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVTimeModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVWeaponModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVVehicleModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVPedModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

// Sizes are as follows: 
// CBaseModelInfo - 0x60
// CInstanceModelInfo - 0x70
// CMloModelInfo - 0x80
// CTimeModelInfo - 0x70
// CWeaponModelInfo - 0x80
// CVehicleModelInfo - 0x3D0
// CPedModelInfo - 0x160

class IVBaseModelInfo
{
public:
	IVBaseModelInfoVFTable * m_VFTable; // 00-04
	PAD(IVBaseModelInfo, pad0, 0x38);   // 04-3C
	// 0x4 - DWORD
	// 0x8 - DWORD
	// 0xC - DWORD
	// 0x10 - float
	// 0x14 - float
	// 0x18 - float
	// 0x1C - float
	// 0x20 - float
	// 0x24 - float
	// 0x28 - float
	// 0x2C - float
	// 0x30 - float
	// 0x34 - float
	// 0x38 - float
	// 0x3C - float
	DWORD dwHash;                       // 3C-40
	// 0x40 - DWORD
	PAD(IVBaseModelInfo, pad1, 0x4);    // 40-44
	DWORD dwReferenceCount;             // 44-48
	// 0x48 - WORD
	// 0x4A - WORD
	// 0x4C - WORD
	// 0x4E - WORD
	// 0x50 - WORD
	// 0x52 - WORD
	// 0x54 - WORD
	PAD(IVBaseModelInfo, pad2, 0xE);    // 48-56
	WORD  wAnimIndex;                   // 56-58
	// 0x48 - WORD
	// 0x5A - BYTE
	// 0x5B - BYTE
	// 0x60 in IVWeaponModelInfo is IVWeaponInfo * m_pWeaponInfo
	PAD(IVBaseModelInfo, pad3, 0x14);   // 58-6C
	DWORD dwVehicleType;                // 6C-70
	// 0x8C - DWORD dwHandlingIndex;
	// CHandling (size = 0x110)
	PAD(IVBaseModelInfo, pad4, 0x52);   // 70-C2
	BYTE byteTextureVariationCount;     // C2-C3
	// 0xCC - IVVehicleStruct * m_pVehicleStruct (Vehicle Struct Size = 0x20C)
	// TODO: Find size(s)
};

class CIVModelInfo
{
private:
	int   m_iModelIndex;
	DWORD m_dwReferenceCount;

public:
	CIVModelInfo();
	CIVModelInfo(int iModelIndex);
	~CIVModelInfo();

	IVBaseModelInfo * GetModelInfo();
	void              SetIndex(int iModelIndex);
	int               GetIndex() { return m_iModelIndex; }
	bool              IsValid() { return (GetModelInfo() != NULL); }
	BYTE              GetType();
	bool              IsBase() { return (GetType() == MODEL_TYPE_BASE); }
	bool              IsInstance() { return (GetType() == MODEL_TYPE_BASE); }
	bool              IsMlo() { return (GetType() == MODEL_TYPE_MLO); }
	bool              IsTime() { return (GetType() == MODEL_TYPE_TIME); }
	bool              IsWeapon() { return (GetType() == MODEL_TYPE_WEAPON); }
	bool              IsVehicle() { return (GetType() == MODEL_TYPE_VEHICLE); }
	bool              IsPed() { return (GetType() == MODEL_TYPE_PED); }
	bool              IsLoaded();
	void              Load(bool bWaitForLoad);
	void              Unload();
	DWORD             GetHash();
	void              AddReference(bool bWaitForLoad);
	void              RemoveReference();
	DWORD             GetReferenceCount();
	WORD              GetAnimIndex();
	bool              IsAutomobile();
	bool              IsBike();
	bool              IsBoat();
	bool              IsTrain();
	bool              IsHeli();
	bool              IsPlane();
};
