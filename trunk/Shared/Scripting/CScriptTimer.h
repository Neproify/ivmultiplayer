//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptTimer.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CSquirrel.h"
#include "CSquirrelArguments.h"

class CScriptTimer
{
private:
	CSquirrel* m_pSquirrel;
	SQObjectPtr m_pFunction;
	int m_uiInterval;
	int m_iRepeations;
	CSquirrelArguments* m_pArguments;
	unsigned int m_uiLastTick;
	bool m_bIsDead;
	bool m_bTraditional;

public:
	CScriptTimer(CSquirrel* pSquirrel, SQObjectPtr pFunction, int uiInterval, int iRepeations, CSquirrelArguments* pArguments);
	~CScriptTimer();

	bool Pulse();
	CSquirrel* GetScript();
	void Kill();
	bool IsDead();
	void SetTraditionalBehavior(bool bTraditional);
};
