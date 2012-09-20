//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPool.h
// Project: Client.Core
// Author(s): jenksta
//            listener
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CGame.h"
#include "COffsets.h"

// From OpenLC by listener
class IVPool
{
public:
	BYTE *  m_pObjects;    // 00-04
	BYTE *  m_pFlags;      // 04-08
	DWORD   m_dwCount;     // 08-0C
	DWORD   m_dwEntrySize; // 0C-10
	int     m_nTop;        // 10-14
	DWORD   m_dwUsed;      // 14-18
	BYTE    m_bAllocated;  // 18-19
	BYTE    _f19;          // 19-1A
	BYTE    _f1A;          // 1A-1B
	BYTE    _f1B;          // 1B-1C
};

template <class T>
class CIVPool
{
private:
	IVPool * m_pPool;

public:
	CIVPool()
	{
		m_pPool = NULL;
	}

	CIVPool(IVPool * pPool)
	{
		m_pPool = pPool;

		// TRy this?
		//m_pPool->m_dwEntrySize = (DWORD)48;
	}

	~CIVPool()
	{

	}

	void     SetPool(IVPool * pPool) { m_pPool = pPool; }
	IVPool * GetPool() { return m_pPool; }

	T      * Allocate()
	{
		T * pObject = NULL;

		if(m_pPool)
		{
			IVPool * pPool = m_pPool;
			_asm
			{
				mov ecx, pPool
				call COffsets::FUNC_CPool__Allocate
				mov pObject, eax
			}
		}

		return pObject;
	}

	DWORD GetCount()
	{
		if(m_pPool)
			return m_pPool->m_dwCount;

		return 0;
	}

	DWORD GetEntrySize()
	{
		if(m_pPool)
			return m_pPool->m_dwEntrySize;

		return 0;
	}

	DWORD GetUsed()
	{
		if(m_pPool)
			return m_pPool->m_dwUsed;

		return 0;
	}

	unsigned int HandleOf(T * pObject)
	{
		unsigned int uiHandle = 0;

		if(m_pPool)
		{
			T * _pObject = pObject;
			IVPool * pPool = m_pPool;
			_asm
			{
				push _pObject
				mov ecx, pPool
				call COffsets::FUNC_CPool__HandleOf
				mov uiHandle, eax
			}
		}

		return uiHandle;
	}

	T * AtHandle(unsigned int uiHandle)
	{
		T * pObject = NULL;

		if(m_pPool)
		{
			IVPool * pPool = m_pPool;
			_asm
			{
				mov ecx, pPool
				push uiHandle
				call COffsets::FUNC_CPool__AtHandle
				mov pObject, eax

			}
		}

		return pObject;
	}

	void Release(T * pObject)
	{
		if(m_pPool)
		{
			IVPool * pPool = m_pPool;
			_asm
			{
				mov ecx, pPool
				push pObject
				call COffsets::FUNC_CPool__Release
			}
		}
	}
};
