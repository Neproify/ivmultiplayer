//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVCheckpoint.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVCheckpoint.h"
#include <Common.h>
#include <CLogFile.h>

CIVCheckpoint::CIVCheckpoint()
	:m_bCreatedByUs(true),
	m_pCheckpoint(new IVCheckpoint)
{
	memset(m_pCheckpoint, 0, sizeof(IVCheckpoint));
}

CIVCheckpoint::CIVCheckpoint(IVCheckpoint * pCheckpoint)
	: m_bCreatedByUs(false)
{
	SetCheckpoint(pCheckpoint);
}

CIVCheckpoint::~CIVCheckpoint()
{
	if(m_bCreatedByUs)
		SAFE_DELETE(m_pCheckpoint);
}

void CIVCheckpoint::SetCheckpoint(IVCheckpoint * pCheckpoint)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	m_pCheckpoint = pCheckpoint;
}

IVCheckpoint * CIVCheckpoint::GetCheckpoint()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	return m_pCheckpoint;
}

void CIVCheckpoint::SetActiveState(BYTE byteActive)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		m_pCheckpoint->m_byteActive = byteActive;
}

BYTE CIVCheckpoint::GetActiveState()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		return m_pCheckpoint->m_byteActive;

	return 0;
}

void CIVCheckpoint::SetType(WORD wType)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		m_pCheckpoint->m_wType = wType;
}

WORD CIVCheckpoint::GetType()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		return m_pCheckpoint->m_wType;

	return 0;
}

void CIVCheckpoint::SetRadius(float fRadius)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		m_pCheckpoint->m_fRadius = fRadius;
}

float CIVCheckpoint::GetRadius()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		return m_pCheckpoint->m_fRadius;

	return 0.0f;
}

void CIVCheckpoint::SetPosition(const CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		memcpy(&m_pCheckpoint->m_vecPosition, &vecPosition, sizeof(CVector3));
}

void CIVCheckpoint::GetPosition(CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		memcpy(&vecPosition, &m_pCheckpoint->m_vecPosition, sizeof(CVector3));
}

void CIVCheckpoint::SetTargetPosition(const CVector3& vecTargetPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		memcpy(&m_pCheckpoint->m_vecTargetPosition, &vecTargetPosition, sizeof(CVector3));
}

void CIVCheckpoint::GetTargetPosition(CVector3& vecTargetPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCheckpoint)
		memcpy(&vecTargetPosition, &m_pCheckpoint->m_vecTargetPosition, sizeof(CVector3));
}