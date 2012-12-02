//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVEntity.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVEntity.h"
#include "COffsets.h"
#include "CGame.h"
#include <CLogFile.h>

CIVEntity::CIVEntity()
	: m_pEntity(NULL)
{
}

CIVEntity::CIVEntity(IVEntity * pEntity)
	: m_pEntity(pEntity)
{
}

CIVEntity::~CIVEntity()
{
}

void CIVEntity::SetEntity(IVEntity * pEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	m_pEntity = pEntity;
}

IVEntity * CIVEntity::GetEntity()
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	return m_pEntity;
}

void CIVEntity::SetMatrix(const Matrix& matMatrix)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
	{
		memcpy(&m_pEntity->m_pMatrix->vecRight, &matMatrix.vecRight, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecForward, &matMatrix.vecForward, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecUp, &matMatrix.vecUp, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecPosition, &matMatrix.vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::GetMatrix(Matrix& matMatrix)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
	{
		memcpy(&matMatrix.vecRight, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
		memcpy(&matMatrix.vecForward, &m_pEntity->m_pMatrix->vecForward, sizeof(CVector3));
		memcpy(&matMatrix.vecUp, &m_pEntity->m_pMatrix->vecUp, sizeof(CVector3));
		memcpy(&matMatrix.vecPosition, &m_pEntity->m_pMatrix->vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::SetPosition(const CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
	{
		if(m_pEntity->m_pMatrix)
			memcpy(&m_pEntity->m_pMatrix->vecPosition, &vecPosition, sizeof(CVector3));
		else
			memcpy(&m_pEntity->m_vecPosition, &vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::GetPosition(CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
	{
		if(m_pEntity->m_pMatrix)
			memcpy(&vecPosition, &m_pEntity->m_pMatrix->vecPosition, sizeof(CVector3));
		else
			memcpy(&vecPosition, &m_pEntity->m_vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::SetRoll(const CVector3& vecRoll)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&m_pEntity->m_pMatrix->vecRight, &vecRoll, sizeof(CVector3));
}

void CIVEntity::GetRoll(CVector3& vecRoll)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&vecRoll, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
}

void CIVEntity::SetDirection(const CVector3& vecDirection)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&m_pEntity->m_pMatrix->vecForward, &vecDirection, sizeof(CVector3));
}

void CIVEntity::GetDirection(CVector3& vecDirection)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&vecDirection, &m_pEntity->m_pMatrix->vecForward, sizeof(CVector3));
}

void CIVEntity::SetModelIndex(WORD wModelIndex)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
	{
		DWORD dwFunc = m_pEntity->m_VFTable->SetModelIndex;
		int iModelIndex = wModelIndex;
		_asm
		{
			push /*w*/iModelIndex
			call dwFunc
			add esp, 4
		}
	}
}

WORD CIVEntity::GetModelIndex()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
		return m_pEntity->m_wModelIndex;

	return 0;
}

void CIVEntity::SetAlpha(BYTE byteAlpha)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
		m_pEntity->m_byteAlpha = byteAlpha;
}

BYTE CIVEntity::GetAlpha()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
		return m_pEntity->m_byteAlpha;

	return 0;
}

bool CIVEntity::IsTouchingEntity(CIVEntity * pTouchingEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
	{
		IVEntity * pGameEntity = m_pEntity;
		IVEntity * pTouchingGameEntity = pTouchingEntity->GetEntity();
		bool bResult = false;
		_asm
		{
			push pTouchingGameEntity
			push pGameEntity
			call COffsets::FUNC_CEntity__IsTouchingEntity
			mov bResult, al
		}
		return bResult;
	}

	return false;
}

void  CIVEntity::AddToWorld()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
		CGame::GetWorld()->AddEntity(this);
}

void CIVEntity::RemoveFromWorld()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pEntity)
		CGame::GetWorld()->RemoveEntity(this);
}
