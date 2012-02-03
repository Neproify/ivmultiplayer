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
	SetPhysical(pVehicle);
}

IVVehicle * CIVVehicle::GetVehicle()
{
	return (IVVehicle *)GetPhysical();
}

void CIVVehicle::SetAlarmDuration(WORD wDuration)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_wAlarm = wDuration;
}

WORD CIVVehicle::GetAlarmDuration()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_wAlarm;

	return 0;
}

void CIVVehicle::SetDriver(IVPed * pPed)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_pDriver = pPed;
}

IVPed * CIVVehicle::GetDriver()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_pDriver;

	return NULL;
}

void CIVVehicle::SetPassenger(BYTE bytePassengerSeatId, IVPed * pPassenger)
{
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
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_byteMaxPassengers = byteMaxPassengers;
}

BYTE CIVVehicle::GetMaxPasssengers()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_byteMaxPassengers;

	return 0;
}

bool CIVVehicle::HasSiren()
{
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
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fGasPedal = fGasPedal;
}

float CIVVehicle::GetGasPedal()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fGasPedal;

	return 0.0f;
}

void CIVVehicle::SetBrakePedal(float fBrakePedal)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fBrakePedal = fBrakePedal;
}

float CIVVehicle::GetBrakePedal()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fBrakePedal;

	return 0.0f;
}

void CIVVehicle::SetSteerBias(float fSteerBias)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteerBias = fSteerBias;
}

float CIVVehicle::GetSteerBias()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteerBias;

	return 0.0f;
}

void CIVVehicle::SetSteeringAngle(float fSteeringAngle)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteering = fSteeringAngle;
}

float CIVVehicle::GetSteeringAngle()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteering;

	return 0.0f;
}

void CIVVehicle::SetEngineHealth(float fEngineHealth)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fEngineHealth = fEngineHealth;
}

float CIVVehicle::GetEngineHealth()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fEngineHealth;

	return 0.0f;
}

void CIVVehicle::SetDirtLevel(float fDirtLevel)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fDirtLevel = fDirtLevel;
}

float CIVVehicle::GetDirtLevel()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fDirtLevel;

	return 0.0f;
}

void CIVVehicle::SetPetrolTankHealth(float fPetrolTankHealth)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fPetrolTankHealth = fPetrolTankHealth;
}

float CIVVehicle::GetPetrolTankHealth()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fPetrolTankHealth;

	return 0.0f;
}

void CIVVehicle::SetDoorLockState(DWORD dwDoorLockState)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwDoorLockState = dwDoorLockState;
}

DWORD CIVVehicle::GetDoorLockState()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_dwDoorLockState;

	return 0;
}

void CIVVehicle::SetEngineStatus(bool bStatus, bool bUnknown)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bStatus)
		{
			DWORD dwFunc = COffsets::FUNC_CVehicle__SetEngineOn;
            _asm
            {
				push bUnknown
                mov ecx, pVehicle
                call dwFunc
            }
		}
		else
		{
			pVehicle->m_byteFlags1 &= 0xE7;
			*(BYTE *)(pVehicle + 0x1344) = 0;
			// jenksta: this is incorrect, the native takes 3 parameters, not 2
			//Scripting::SetCarEngineOn(CGame::GetPools()->GetVehiclePool()->HandleOf(pVehicle), false);
		}
	}
}

bool CIVVehicle::GetEngineStatus()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		// not working, need way to detect engine
		return ( *(BYTE *)( pVehicle + 0x1344 ) == 1 );

	return false;
}

void CIVVehicle::SoundHorn(int iDuration)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwHorn = 30 * iDuration / 1000;
}

void CIVVehicle::SetComponentState(int iComponent, bool bState)
{
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
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return CGame::GetModelInfo(pVehicle->m_wModelIndex)->GetModelInfo()->byteTextureVariationCount;

	return 255;
}

void CIVVehicle::SetTextureVariation(int iVariation)
{
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
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return IS_BIT_SET(pVehicle->m_byteFlags9, 4);

	return false;
}
