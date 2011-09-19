//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPlayerInfo.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPlayerInfo.h"
#include "CGame.h"
#include <CLogFile.h>

CIVPlayerInfo::CIVPlayerInfo()
{
	m_bCreatedByUs = false;
	m_pPlayerInfo = NULL;
}

CIVPlayerInfo::CIVPlayerInfo(BYTE bytePlayerNumber)
{
	IVPlayerInfo * pPlayerInfo = (IVPlayerInfo *)CGame::Alloc(sizeof(IVPlayerInfo));

	if(!pPlayerInfo)
		CLogFile::Printf("ERROR: CIVPlayerInfo::Constructor: Alloc failed.");

#define FUNC_CPlayerInfo__Constructor 0x87AB70
	DWORD dwFunc = (CGame::GetBase() + FUNC_CPlayerInfo__Constructor);
	_asm
	{
		mov ecx, pPlayerInfo
		call dwFunc
	}
	/*DWORD dwCount = *(DWORD *)(g_pClient->GetBaseAddress() + 0x11A7108);
	dwCount++;
	DWORD * dwArray = *(DWORD **)(g_pClient->GetBaseAddress() + 0x11A7088);
	dwArray[bytePlayerNumber] = dwCount;
	*(DWORD *)(g_pClient->GetBaseAddress() + 0x11A7108) = dwCount;*/
	pPlayerInfo->m_bytePlayerNumber = bytePlayerNumber;
	//memcpy(result, a2, 0x60u);
	*(DWORD *)(pPlayerInfo + 0x15C) = 0;
	*(DWORD *)(pPlayerInfo + 0x15D) = 0;
	*(DWORD *)(pPlayerInfo + 0x15B) = 0;
	*(DWORD *)(pPlayerInfo + 0x137) = 2;
	m_bCreatedByUs = true;
	m_pPlayerInfo = pPlayerInfo;
}

CIVPlayerInfo::CIVPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	m_bCreatedByUs = false;
	SetPlayerInfo(pPlayerInfo);
}

CIVPlayerInfo::~CIVPlayerInfo()
{
	SetPlayerInfo(NULL);
}

void CIVPlayerInfo::SetPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	if(m_bCreatedByUs)
	{
		XLivePBufferFree(m_pPlayerInfo->m_pDisplayScore);
		XLivePBufferFree(m_pPlayerInfo->m_pScore);
		IVPlayerInfo * pPlayerInfo = m_pPlayerInfo;
		DWORD dwFunc = (CGame::GetBase() + 0x878AF0); // CPlayerInfo::Destructor(or Reset)
		_asm
		{
			mov ecx, pPlayerInfo
			call dwFunc
		}
		/*dwFunc = (CGame::GetBase() + 0x878AE0);
		DWORD dwUnknown = (DWORD)(pPlayerInfo + 0x60);
		_asm
		{
			push dwUnknown
			call dwFunc
		}*/
		CGame::Free(pPlayerInfo);
	}

	m_pPlayerInfo = pPlayerInfo;
}

IVPlayerInfo * CIVPlayerInfo::GetPlayerInfo()
{
	return m_pPlayerInfo;
}

void CIVPlayerInfo::SetName(char * szName)
{
	if(m_pPlayerInfo)
		strncpy(m_pPlayerInfo->m_szName, szName, 20);
}

char * CIVPlayerInfo::GetName()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_szName;

	return 0;
}

void CIVPlayerInfo::SetPlayerNumber(BYTE bytePlayerNumber)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_bytePlayerNumber = bytePlayerNumber;
}

BYTE CIVPlayerInfo::GetPlayerNumber()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_bytePlayerNumber;

	return INVALID_PLAYER_PED;
}

void CIVPlayerInfo::SetColour(DWORD dwColour)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_dwColour = dwColour;
}

DWORD CIVPlayerInfo::GetColour()
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_dwColour;

	return 0;
}

void CIVPlayerInfo::SetPlayerPed(IVPlayerPed * pPlayerPed)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_pPlayerPed = pPlayerPed;
}

IVPlayerPed * CIVPlayerInfo::GetPlayerPed()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_pPlayerPed;

	return NULL;
}

int CIVPlayerInfo::GetScore()
{
	if(m_pPlayerInfo)
	{
		int iScore;
		XLivePBufferGetDWORD(m_pPlayerInfo->m_pScore, 0, (DWORD *)&iScore);
		return iScore;
	}

	return 0;
}

void CIVPlayerInfo::SetScore(int iScore)
{
	if(m_pPlayerInfo)
		XLivePBufferSetDWORD(m_pPlayerInfo->m_pScore, 0, (DWORD)iScore);
}

int CIVPlayerInfo::GetDisplayScore()
{
	if(m_pPlayerInfo)
	{
		int iScore;
		XLivePBufferGetDWORD(m_pPlayerInfo->m_pDisplayScore, 0, (DWORD *)&iScore);
		return iScore;
	}

	return 0;
}

void CIVPlayerInfo::SetDisplayScore(int iScore)
{
	if(m_pPlayerInfo)
		XLivePBufferSetDWORD(m_pPlayerInfo->m_pDisplayScore, 0, (DWORD)iScore);
}
