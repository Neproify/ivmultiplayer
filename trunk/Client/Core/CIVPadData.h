//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPadData.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IVCommon.h"

#define MAX_HISTORY_ITEMS 64

class IVPadDataHistoryItem
{
public:
	BYTE m_byteValue;                   // 0-1
	PAD(IVPadDataHistoryItem, pad0, 3); // 1-4
	DWORD m_dwLastUpdateTime;           // 4-8
};

class IVPadDataHistory
{
public:
	IVPadDataHistoryItem m_historyItems[MAX_HISTORY_ITEMS]; // 000-200
};

// this is actually rage::ioValue
class IVPadData
{
public:
	// Get current key (m_byteUnknown4 ^ m_byteCurrentValue)
	// Get previous key (m_byteUnknown4 ^ m_byteUnknown7)
	DWORD m_dwVFTable;             // 00-04
	BYTE m_byteUnknown4;           // 04-05 (Control state value? (xor with byte 6/7))
	BYTE m_byteContext;            // 05-06 (Used to avoid pad data being swapped more than once per pad process)
	BYTE m_byteCurrentValue;       // 06-07
	BYTE m_byteLastValue;          // 07-08
	BYTE m_byteHistoryIndex;       // 08-09
	PAD(IVPadData, pad0, 3);       // 09-0C
	IVPadDataHistory * m_pHistory; // 0C-10
};
