//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVEvent.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../IVCommon.h"

// IV's CEvent
class IVEvent
{
public:
	PAD(IVEvent, pad0, 0x60);
};

class CIVEvent
{
private:
	IVEvent * m_pEvent;

public:
	CIVEvent();
	CIVEvent(IVEvent * pEvent);
	~CIVEvent();

	void      SetEvent(IVEvent * pEvent);
	IVEvent * GetEvent();
};