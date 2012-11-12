//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVVehicle.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVVehicle.h"
#include "COffsets.h"
#include "CPools.h"
#include "Scripting.h"
#include <CLogFile.h>

CIVVehicle::CIVVehicle() : CIVPhysical()
{
}

CIVVehicle::CIVVehicle(IVVehicle * pVehicle) : CIVPhysical(pVehicle)
{
}

CIVVehicle::~CIVVehicle()
{
}

void CIVVehicle::SetVehicle(IVVehicle * pVehicle)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	SetPhysical(pVehicle);
}

IVVehicle * CIVVehicle::GetVehicle()
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	return (IVVehicle *)GetPhysical();
}

void CIVVehicle::SetAlarmDuration(WORD wDuration)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_wAlarm = wDuration;
}

WORD CIVVehicle::GetAlarmDuration()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_wAlarm;

	return 0;
}

void CIVVehicle::SetDriver(IVPed * pPed)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_pDriver = pPed;
}

IVPed * CIVVehicle::GetDriver()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_pDriver;

	return NULL;
}

void CIVVehicle::SetPassenger(BYTE bytePassengerSeatId, IVPed * pPassenger)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			pVehicle->m_pPassengers[bytePassengerSeatId] = pPassenger;
	}
}

IVPed * CIVVehicle::GetPassenger(BYTE bytePassengerSeatId)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			return pVehicle->m_pPassengers[bytePassengerSeatId];
	}

	return NULL;
}

void CIVVehicle::SetColors(unsigned char ucColor1, unsigned char ucColor2, unsigned char ucColor3, unsigned char ucColor4)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		pVehicle->m_byteColors[0] = ucColor1;
		pVehicle->m_byteColors[1] = ucColor2;
		pVehicle->m_byteColors[2] = ucColor3;
		pVehicle->m_byteColors[3] = ucColor4;

		_asm
		{
			mov ecx, pVehicle
			call COffsets::FUNC_CVehicle__RefreshColours
		}
	}
}
void CIVVehicle::GetColors(unsigned char * ucColor1, unsigned char * ucColor2, unsigned char * ucColor3, unsigned char * ucColor4)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		*ucColor1 = pVehicle->m_byteColors[0];
		*ucColor2 = pVehicle->m_byteColors[1];
		*ucColor3 = pVehicle->m_byteColors[2];
		*ucColor4 = pVehicle->m_byteColors[3];
	}
}

void CIVVehicle::SetMaxPassengers(BYTE byteMaxPassengers)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_byteMaxPassengers = byteMaxPassengers;
}

BYTE CIVVehicle::GetMaxPasssengers()
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_byteMaxPassengers;

	return 0;
}

bool CIVVehicle::HasSiren()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		// Check if the siren flag is set
		return IS_BIT_SET(*(BYTE *)(pVehicle + 0xF6E), 0x20);
	}

	return false;
}

void CIVVehicle::SetSirenState(bool bStatus)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle && HasSiren())
	{
		if(bStatus)
			*(BYTE *)(pVehicle + 0xF69) = *(BYTE *)(pVehicle + 0xF69) & 0x9F | 0x10;
		else
			*(BYTE *)(pVehicle + 0xF69) &= 0x8F;
	}
}

bool CIVVehicle::GetSirenState()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle && HasSiren())
	{
		if(!IS_BIT_SET(*(BYTE *)(pVehicle + 0xF69), 0x10))
			return false;

		if(!IS_BIT_SET(*(BYTE *)(pVehicle + 0xF69), 0x20))
			return false;

		return true;
	}

	return false;
}

void CIVVehicle::SetGasPedal(float fGasPedal)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fGasPedal = fGasPedal;
}

float CIVVehicle::GetGasPedal()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fGasPedal;

	return 0.0f;
}

void CIVVehicle::SetBrakePedal(float fBrakePedal)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fBrakePedal = fBrakePedal;
}

float CIVVehicle::GetBrakePedal()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fBrakePedal;

	return 0.0f;
}

void CIVVehicle::SetSteerBias(float fSteerBias)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteerBias = fSteerBias;
}

float CIVVehicle::GetSteerBias()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteerBias;

	return 0.0f;
}

void CIVVehicle::SetSteeringAngle(float fSteeringAngle)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteering = fSteeringAngle;
}

float CIVVehicle::GetSteeringAngle()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteering;

	return 0.0f;
}

void CIVVehicle::SetEngineHealth(float fEngineHealth)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fEngineHealth = fEngineHealth;
}

float CIVVehicle::GetEngineHealth()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fEngineHealth;

	return 0.0f;
}

void CIVVehicle::SetDirtLevel(float fDirtLevel)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fDirtLevel = fDirtLevel;
}

float CIVVehicle::GetDirtLevel()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fDirtLevel;

	return 0.0f;
}

void CIVVehicle::SetPetrolTankHealth(float fPetrolTankHealth)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fPetrolTankHealth = fPetrolTankHealth;
}

float CIVVehicle::GetPetrolTankHealth()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fPetrolTankHealth;

	return 0.0f;
}

void CIVVehicle::SetDoorLockState(DWORD dwDoorLockState)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwDoorLockState = dwDoorLockState;
}

DWORD CIVVehicle::GetDoorLockState()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_dwDoorLockState;

	return 0;
}

void CIVVehicle::SetEngineStatus(bool bStatus, bool bUnknown)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bStatus)
		{
			bool bUnknownTrue = true;
			DWORD CVehicle__TurnOnEngine = (CGame::GetBase() + 0x9D3600);
			_asm
			{
				push bUnknownTrue
				mov ecx, pVehicle
				call CVehicle__TurnOnEngine
			}
		}
		else
		{
			DWORD CVehicle__TurnOffEngine = (CGame::GetBase() + 0x9C6710);
			_asm
			{
				mov ecx, pVehicle
				call CVehicle__TurnOffEngine
			}
		}
	}
}

bool CIVVehicle::GetEngineStatus()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return *(bool *)( pVehicle + 0x1344 );

	return false;
}

void CIVVehicle::SoundHorn(int iDuration)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwHorn = 30 * iDuration / 1000;
}

void CIVVehicle::SetComponentState(int iComponent, bool bState)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if((iComponent - 1) <= 8)
		{
			DWORD dwFunc = (CGame::GetBase() + 0x9C65B0);
			bool _bState = !bState;
			int _iComponent = (iComponent + 0x58);
			_asm
			{
				push _bState
				push _iComponent
				mov ecx, pVehicle
				call dwFunc
			}
		}
	}
}

BYTE CIVVehicle::GetTextureVariationCount()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return CGame::GetModelInfo(pVehicle->m_wModelIndex)->GetModelInfo()->byteTextureVariationCount;

	return 255;
}

void CIVVehicle::SetTextureVariation(int iVariation)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		// Get the number of texture variations
		BYTE byteTextureVariationCount = GetTextureVariationCount();

		// Do we have no texture variations?
		if(byteTextureVariationCount == 255)
			return;

		// Is the texture variation valid?
		if(iVariation < byteTextureVariationCount)
		{
			DWORD dwTemp = (*(DWORD *)(pVehicle + 0x34) + 4);
			*(DWORD *)(dwTemp + 0xD8) = iVariation;
			DWORD dwFunc = (CGame::GetBase() + 0xCBA620);
			_asm
			{
				push -1
				push pVehicle
				mov ecx, dwTemp
				call dwFunc
			}
		}
	}
}

int CIVVehicle::GetTextureVariation()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		// Get the number of texture variations
		BYTE byteTextureVariationCount = GetTextureVariationCount();

		// Do we have any texture variations?
		if(byteTextureVariationCount)
		{
			DWORD dwTemp = *(DWORD *)(*(DWORD *)(pVehicle + 0x34) + 4);
			return *(DWORD *)(dwTemp + 0xD8);
		}
	}

	return 0;
}

void CIVVehicle::SetCanBeVisiblyDamaged(bool bState)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bState)
			SET_BIT(pVehicle->m_byteFlags9, 4);
		else
			UNSET_BIT(pVehicle->m_byteFlags9, 4);
	}
}

bool CIVVehicle::CanBeVisiblyDamaged()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return IS_BIT_SET(pVehicle->m_byteFlags9, 4);

	return false;
}

void CIVVehicle::SetGPSState(bool bState)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		*(bool*)(pVehicle + 0xB02) = bState;
	}
}

bool CIVVehicle::GetGPSState()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		return *(bool*)(pVehicle + 0xB02);
	}
	return false;
}

//TODO: Add Get? *(DWORD *)(pVehicle + 0xFD0); // 4048
void CIVVehicle::BurstCarTyre(int iTyre)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		DWORD dwFunc = (CGame::GetBase() + 0x9C5510);
		_asm
		{
			push iTyre
			mov ecx, pVehicle
			call dwFunc
		}
	}
}

void CIVVehicle::GetDeformation(CVector3& vecPos)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		//vecPos = pVehicle->m_pDamageManager->vecDamage;
	}
}

void CIVVehicle::SetCarCanBeDamaged(bool bDamage)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle * pVehicle = GetVehicle();
    if(pVehicle)
    {
        DWORD byteFlagDamage = *(DWORD *)(pVehicle + 0x118); // 280 (flag?)
        if(bDamage)
            *(DWORD *)byteFlagDamage &= 0x400u;
        else
            *(DWORD *)byteFlagDamage |= 0xFFFFFBFFu;
    }
}

void CIVVehicle::RemoveCarWindow(int iWindow)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle * pVehicle = GetVehicle();
    if(pVehicle)
    {
        DWORD dwFunc = (CGame::GetBase() + 0x9C6500);
        _asm
        {
            push iWindow
            mov ecx, pVehicle
            call dwFunc
        }
    }
}

void CIVVehicle::SetLightsState(int iState) // 0(on),1(aways off),2(always on) // doesn't work - i know why wait
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle * pVehicle = GetVehicle();
    if(pVehicle)
        *(WORD *)(pVehicle + 0x1112) ^= ((char)iState ^ *(WORD *)(pVehicle + 0x1112)) & 3; // 4370
}

int CIVVehicle::GetLightsState()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle * pVehicle = GetVehicle();
    if(pVehicle)
    {
        return (*(WORD  *)(pVehicle + 0x1112)); // 4370(flag?)
    }
    return 0;
}

void CIVVehicle::RemoveVehicleWindow(Scripting::eVehicleWindow window)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
        IVVehicle *pVehicle = GetVehicle();
        if(pVehicle)
        {
                DWORD CVehicle__RemoveVehicleWindow = (CGame::GetBase() + 0x9C6500);
                _asm
                {
                        push window
                        mov ecx, pVehicle
                        call CVehicle__RemoveVehicleWindow
                }
        }
}

bool CIVVehicle::IsCarInWater()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle *pVehicle = GetVehicle();
    
    bool bInWater = false;
    if(pVehicle)
    {
        if( *(int *)(pVehicle + 0x1354) == 2 )
        {
            bInWater = *(BYTE *)(pVehicle + 0x1360) & 1;   
        } else {
                        if((*(BYTE *)(pVehicle + 0x118) & 1) || ((*(DWORD *)(pVehicle + 0x6C) != NULL) && *(BYTE *)(pVehicle + 0x141)))
                                bInWater = true;
                }
    }
    return  bInWater;
}

void CIVVehicle::SetCarCanBurstTyres(bool bState)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle *pVehicle = GetVehicle();
    if(pVehicle)
    {
            if(bState)
                    *(BYTE *)(pVehicle + 0xF67) &= 0xEFu;
            else
                    *(BYTE *)(pVehicle + 0xF67) |= 0x10u;
    }
}

bool CIVVehicle::GetCarCanBurstTyres()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
    IVVehicle *pVehicle = GetVehicle();
    if(pVehicle)
		return ((*(BYTE *)(pVehicle + 0xF67) & 0xEFu) != NULL);

    return false;
}