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
		DWORD dwFunc = (CGame::GetBase() + FUNC_CPhysical__SetMoveSpeed_7);
		_asm
		{
			push pVecMoveSpeed
			mov ecx, pPhysical
			call dwFunc
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
		DWORD dwFunc = (CGame::GetBase() + FUNC_CPhysical__GetMoveSpeed_7);
		_asm
		{
			push pVecMoveSpeed
			mov ecx, pPhysical
			call dwFunc
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
		DWORD dwFunc = (CGame::GetBase() + FUNC_CPhysical__SetTurnSpeed_7);
		_asm
		{
			push pVecTurnSpeed
			mov ecx, pPhysical
			call dwFunc
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
		DWORD dwFunc = (CGame::GetBase() + FUNC_CPhysical__GetTurnSpeed_7);
		_asm
		{
			push pVecTurnSpeed
			mov ecx, pPhysical
			call dwFunc
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
		pPhysical->m_pLastDamageEntity = pLastDamageEntity;
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
