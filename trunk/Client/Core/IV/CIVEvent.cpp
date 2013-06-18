//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVEvent.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVEvent.h"
#include <CLogFile.h>

CIVEvent::CIVEvent()
	: m_pEvent(NULL)
{
}

CIVEvent::CIVEvent(IVEvent * pEvent)
	: m_pEvent(pEvent)
{
}

CIVEvent::~CIVEvent()
{
}

void CIVEvent::SetEvent(IVEvent * pEvent)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	m_pEvent = pEvent;
}

IVEvent * CIVEvent::GetEvent()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	return m_pEvent;
}