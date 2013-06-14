//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVEntity.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVEntity.h"
#include "../COffsets.h"
#include "../CGame.h"
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
	m_pEntity = pEntity;
}

IVEntity * CIVEntity::GetEntity()
{
	return m_pEntity;
}

void CIVEntity::SetMatrix(const Matrix& matMatrix)
{
	if(m_pEntity && m_pEntity->m_pMatrix) 
	{
		Matrix34* mat = new Matrix34();
		mat->FromMatrix((Matrix*)&matMatrix);
		m_pEntity->SetMatrix(mat, 0, 0);
		delete mat;
	}
}

void CIVEntity::GetMatrix(Matrix& matMatrix)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
	{
		memcpy(&matMatrix.vecRight, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
		memcpy(&matMatrix.vecForward, &m_pEntity->m_pMatrix->vecForward, sizeof(CVector3));
		memcpy(&matMatrix.vecUp, &m_pEntity->m_pMatrix->vecUp, sizeof(CVector3));
		memcpy(&matMatrix.vecPosition, &m_pEntity->m_pMatrix->vecPosition, sizeof(CVector3));
	}
}

#include "../CClient.h"
extern CClient* g_pClient;
void CIVEntity::SetPosition(const CVector3& vecPosition)
{
	if(m_pEntity) 
	{
		Vector4 vecPos;
		vecPos.fX = vecPosition.fX;
		vecPos.fY = vecPosition.fY;
		vecPos.fZ = vecPosition.fZ;

		m_pEntity->SetCoordinates(&vecPos, 0, 0);

		m_pEntity->UpdatePhysicsMatrix(true);
	}
}

unsigned long lastLoaded = 0;

void CIVEntity::GetPosition(CVector3& vecPosition)
{
	if(m_pEntity)
	{
		Vector4 vecPos;
		
		m_pEntity->GetCoordinates(&vecPos);

		vecPosition.fX = vecPos.fX;
		vecPosition.fY = vecPos.fY;
		vecPosition.fZ = vecPos.fZ;

		//if(lastLoaded + 5000 <= SharedUtility::GetTime())
		//{
		//	Scripting::AllowGameToPauseForStreaming(false);
		//	DWORD dwFunc = (CGame::GetBase() + 0x7B7600);
		//	Vector4* pPos = &vecPos;
		//	__asm
		//	{
		//		push pPos
		//		call dwFunc
		//	}
		//	lastLoaded = SharedUtility::GetTime();
		//}
	}
}

void CIVEntity::SetHeading(float fHeading)
{
	if(m_pEntity)	
		m_pEntity->SetHeading(fHeading);
}	

void CIVEntity::SetRoll(const CVector3& vecRoll)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&m_pEntity->m_pMatrix->vecRight, &vecRoll, sizeof(CVector3));
}

void CIVEntity::GetRoll(CVector3& vecRoll)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&vecRoll, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
}

void CIVEntity::SetDirection(const CVector3& vecDirection)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&m_pEntity->m_pMatrix->vecForward, &vecDirection, sizeof(CVector3));
}

void CIVEntity::GetDirection(CVector3& vecDirection)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&vecDirection, &m_pEntity->m_pMatrix->vecForward, sizeof(CVector3));
}

void CIVEntity::SetModelIndex(WORD wModelIndex)
{
	if(m_pEntity)
		m_pEntity->SetModelIndex(wModelIndex);
}

WORD CIVEntity::GetModelIndex()
{
	if(m_pEntity)
		return m_pEntity->m_wModelIndex;

	return 0;
}

void CIVEntity::SetAlpha(BYTE byteAlpha)
{
	if(m_pEntity)
		m_pEntity->m_byteAlpha = byteAlpha;
}

BYTE CIVEntity::GetAlpha()
{
	if(m_pEntity)
		return m_pEntity->m_byteAlpha;

	return 0;
}

bool CIVEntity::IsTouchingEntity(CIVEntity * pTouchingEntity)
{
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
	if(m_pEntity)
		CGame::GetWorld()->AddEntity(this);
}

void CIVEntity::RemoveFromWorld()
{
	if(m_pEntity)
		CGame::GetWorld()->RemoveEntity(this);
}