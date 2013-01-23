//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPhysical.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPhysical.h"
#include "CGame.h"
#include <CLogFile.h>
#include "COffsets.h"

CIVPhysical::CIVPhysical() : CIVDynamicEntity()
{

}

CIVPhysical::CIVPhysical(IVPhysical * pPhysical) : CIVDynamicEntity(pPhysical)
{

}

CIVPhysical::~CIVPhysical()
{

}

void CIVPhysical::SetPhysical(IVPhysical * pPhysical)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	SetDynamicEntity(pPhysical);
}

IVPhysical * CIVPhysical::GetPhysical()
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	return (IVPhysical *)GetDynamicEntity();
}

void CIVPhysical::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		const CVector3 * pVecMoveSpeed = &vecMoveSpeed;
		_asm
		{
			push pVecMoveSpeed
			mov ecx, pPhysical
			call COffsets::FUNC_CPhysical__SetMoveSpeed
		}
	}
}

void CIVPhysical::GetMoveSpeed(CVector3& vecMoveSpeed)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		CVector3 * pVecMoveSpeed = &vecMoveSpeed;
		_asm
		{
			push pVecMoveSpeed
			mov ecx, pPhysical
			call COffsets::FUNC_CPhysical__GetMoveSpeed
		}
	}
}

void CIVPhysical::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		const CVector3 * pVecTurnSpeed = &vecTurnSpeed;
		_asm
		{
			push pVecTurnSpeed
			mov ecx, pPhysical
			call COffsets::FUNC_CPhysical__SetTurnSpeed
		}
	}
}

void CIVPhysical::GetTurnSpeed(CVector3& vecTurnSpeed)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		CVector3 * pVecTurnSpeed = &vecTurnSpeed;
		_asm
		{
			push pVecTurnSpeed
			mov ecx, pPhysical
			call COffsets::FUNC_CPhysical__GetTurnSpeed
		}
	}
}

void CIVPhysical::SetLastDamageEntity(IVEntity * pLastDamageEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		IVEntity * _pLastDamageEntity = pLastDamageEntity;
		_asm
		{
			push _pLastDamageEntity
			mov ecx, pPhysical
			call COffsets::FUNC_CPhysical__SetLastDamageEntity
		}
	}
}

IVEntity * CIVPhysical::GetLastDamageEntity()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
		return pPhysical->m_pLastDamageEntity;

	return NULL;
}

void CIVPhysical::SetHealth(float fHealth)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		DWORD dwFunc = ((IVPhysicalVFTable *)pPhysical->m_VFTable)->SetHealth;
		_asm
		{
			push 1
			push fHealth
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

float CIVPhysical::GetHealth()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();
	float fHealth = 0.0f;

	if(pPhysical)
	{
		DWORD dwFunc = ((IVPhysicalVFTable *)pPhysical->m_VFTable)->GetHealth;
		_asm
		{
			mov ecx, pPhysical
			call dwFunc
			mov fHealth, eax
		}
	}

	return fHealth;
}
