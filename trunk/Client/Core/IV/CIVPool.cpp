//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPool.cpp
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPool.h"

/*
CPool::CPool(IVPool* pPool)
{
	// TODO
}

CPool::CPool(int maxObjects, const char* Name, int entrySize)
{
	this->m_dwEntrySize = entrySize;
	this->m_pObjects = (BYTE*)malloc(entrySize * maxObjects);
	this->m_pFlags = (BYTE*)malloc(maxObjects);
	this->m_bAllocated = 1;
	this->m_dwCount = maxObjects;
	this->m_nTop = -1;
	int n = 0;
	char flag;

	for(this->m_dwUsed = 0; n < maxObjects; this->m_pFlags[n] = flag & 0x81 | 1)
	{
		this->m_pFlags[n] |= 0x80u;
		flag = this->m_pFlags[n];
		++n;
	}
};

CPool::~CPool()
{
	free(this->m_pObjects);
	free(this->m_pFlags);
}

BYTE* CPool::Allocate()
{
	DWORD dwCount = this->m_dwCount;
	int iTop = this->m_nTop;
	bool checkedAll = false;

	while(true)
	{
		++iTop;
		if(iTop == dwCount)
		{
			if(checkedAll)
				return 0;

			checkedAll = true;
			iTop = 0;
		}
		if((this->m_pFlags[iTop]) & 0x80)
		{
			this->m_pFlags[iTop] &= 0x7Fu;
			this->SetFlags(iTop, (this->m_pFlags[iTop] & 0x7F) + 1);
			++this->m_dwUsed;
			this->m_nTop = iTop;
			return &this->m_pObjects[iTop * this->m_dwEntrySize];
		}
	}
}

int CPool::SetFlags(int iTop, char flag)
{
	if((flag & 0x7Fu) <= 1)
	{
		this->m_pFlags[iTop] = this->m_pFlags[iTop] & 0x80 | 1;
		return 1;
	} else {
		this->m_pFlags[iTop] = (flag & 0x7F) | this->m_pFlags[iTop] & 0x80;
		return (flag & 0x7F);
	}
}

BYTE* CPool::AtHandle(signed int siHandle)
{
	if(this->m_pFlags[(siHandle >> 8)] == siHandle)
		return &this->m_pObjects[(siHandle >> 8) * this->m_dwEntrySize];

	return NULL;
}

int CPool::HandleOf(BYTE* pObject)
{
	int index  = (pObject - this->m_pObjects) / this->m_dwEntrySize;
	return ((index << 8) + this->m_pFlags[index]);
}


int CPool::Release(BYTE* pObject)
{
	int index; 
	index = (pObject - this->m_pObjects) / this->m_dwEntrySize;
	this->m_pFlags[index] |= 0x80;
	--this->m_dwUsed;
	if(index < this->m_nTop)
		this->m_nTop = index;
	return index;
};
*/
