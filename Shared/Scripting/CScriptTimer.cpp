//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptTimer.cpp
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CScriptTimer.h"
#include "../SharedUtility.h"

CScriptTimer::CScriptTimer(CSquirrel* pSquirrel, SQObjectPtr pFunction, int uiInterval, int iRepeations, CSquirrelArguments* pArguments)
{
	m_bIsDead = false;
	m_pSquirrel = pSquirrel;
	m_pFunction = pFunction;
	m_uiInterval = uiInterval;
	m_iRepeations = iRepeations;
	m_pArguments = pArguments;
	m_uiLastTick = SharedUtility::GetTime();
	m_bTraditional = false;
}

CScriptTimer::~CScriptTimer()
{
	if(m_pArguments)
		delete m_pArguments;
}

bool CScriptTimer::Pulse()
{
	if(m_bIsDead)
		return false;

	// max number of iterations to run in one pulse (since timers are delayed somewhat after a few executions, it might be worth doing more)
	// 'traditional behavior' means only one iteration at most
	unsigned int iCount = m_bTraditional ? 1 : 10;

	// save the current time
	unsigned int uiNow = SharedUtility::GetTime();

	// call the timer function as long as we should by time & iterations
	while(uiNow >= m_uiLastTick + m_uiInterval && iCount -- > 0)
	{
		// call the function
		m_pSquirrel->Call(m_pFunction, m_pArguments);

		// update the last tick count. 'traditional behaviour' implies the timer to go off after some time, yet forces at least m_uiInterval to
		// elapse inbetween two calls. By default, it is assumed that the timer should be happening in regular intervals, so it might be a few
		// milliseconds below or above the interval - yet in the long run/on average is on that interval.
		m_uiLastTick = m_bTraditional ? uiNow : ( m_uiLastTick + m_uiInterval );

		if(m_iRepeations > 0)
		{
			if(-- m_iRepeations == 0)
				return false;
		}
	}
	return true;
}

CSquirrel* CScriptTimer::GetScript()
{
	return m_pSquirrel;
}

void CScriptTimer::Kill()
{
	m_bIsDead = true;
}

bool CScriptTimer::IsDead()
{
	return m_bIsDead;
}

void CScriptTimer::SetTraditionalBehavior(bool bTraditional)
{
	m_bTraditional = bTraditional;
}
