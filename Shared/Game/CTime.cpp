//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CTime.cpp
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CTime.h"
#include "SharedUtility.h"

// FIXUPDATE
// jenksta: HACKY!
#ifdef _SERVER
#include "../Server/Core/CNetworkManager.h"
extern CNetworkManager * g_pNetworkManager;
#endif

CTime::CTime()
{
	m_ulTimeSet = SharedUtility::GetTime();
	m_ucHour = 12;
	m_ucMinute = 0;
	m_ucDayOfWeek = 0;
	m_uiMinuteDuration = DEFAULT_MINUTE_DURATION;
}

CTime::~CTime()
{
}

void CTime::SetTime(const unsigned char ucHour, const unsigned char ucMinute)
{
	if(ucHour >= 24)
		SetDayOfWeek(((ucHour) / 24) - 1);
	m_ulTimeSet = SharedUtility::GetTime();
	m_ucHour = ucHour;
	m_ucMinute = ucMinute;

#ifdef _SERVER
	SyncTime();
#endif
}

void CTime::GetTime(unsigned char *ucHour, unsigned char *ucMinute)
{
	if(m_uiMinuteDuration == 0)
	{
		*ucHour = m_ucHour;
		*ucMinute = m_ucMinute;
	}
	else
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();
		unsigned int uiMinutesPassed = ( ( ulCurrentTime - m_ulTimeSet ) / m_uiMinuteDuration );
		unsigned int uiHoursPassed = ( m_ucMinute + uiMinutesPassed ) / 60;

		*ucHour = ( uiHoursPassed + m_ucHour ) % 24;
		*ucMinute = ( uiMinutesPassed + m_ucMinute ) % 60;
	}
}

void CTime::SetMinuteDuration(const unsigned int uiMinuteDuration)
{
	// Make sure we're calculating future times from ours
	unsigned char ucHour = 0, ucMinute = 0;
	GetTime(&ucHour, &ucMinute);
	if(uiMinuteDuration > 0)
	{
		m_uiMinuteDuration = uiMinuteDuration;
	}
	SetTime(ucHour, ucMinute);
}

unsigned int CTime::GetMinuteDuration()
{
	return m_uiMinuteDuration;
}

void CTime::SetDayOfWeek(const unsigned char ucDayOfWeek)
{
	// Make sure we're calculating future times from our current ones, if we were to only set the day of week here
	// it would be desynced for that it is calculated from a time in the past
	unsigned char ucHour = 0, ucMinute = 0;
	GetTime(&ucHour, &ucMinute);

	m_ucDayOfWeek = ucDayOfWeek;

	SetTime(ucHour, ucMinute);
}

unsigned char CTime::GetDayOfWeek()
{
	if(m_uiMinuteDuration == 0)
	{
		return m_ucDayOfWeek;
	}
	else
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();
		unsigned int uiMinutesPassed = ( ( ulCurrentTime - m_ulTimeSet ) / m_uiMinuteDuration ) + m_ucHour * 60 + m_ucMinute;
		return ( ( ( uiMinutesPassed ) / ( 24 * 60 ) ) + m_ucDayOfWeek ) % 7;
	}
}

#ifdef _SERVER
void CTime::SyncTime()
{
	CBitStream bsSend;
	bsSend.Write((unsigned char)(m_ucHour + (24 * (m_ucDayOfWeek + 1))));
	bsSend.Write(m_ucMinute);
	if(m_uiMinuteDuration != DEFAULT_MINUTE_DURATION)
		bsSend.Write(m_uiMinuteDuration);

	g_pNetworkManager->RPC(RPC_ScriptingSetPlayerTime, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}
#endif
