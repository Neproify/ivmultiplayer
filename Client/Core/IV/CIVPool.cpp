//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPool.cpp
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPool.h"
#include <CLogFile.h>
#include "../CGame.h"


// This is how to increase the pool size of all pools in IV
static int mulPoolSize = 1;
int __stdcall  CPool_hook_chunk(void* this_, int maxObjects, int Name, int entrySize);
__declspec(naked) void __stdcall CPool_hook()
{
	_asm
	{
		pop eax
		push ecx
		push eax
		jmp CPool_hook_chunk
	}
}

int __stdcall  CPool_hook_chunk(void* this_, int maxObjects, int Name, int entrySize)
{
	IVPool *pPool = (IVPool*)this_;


	// Here we simply increase the size buf we could also replace the entire pool with our own
	// Or we can filter the pools by their name for example Task and only modify this pool
	// to increase only the task pool replace maxObjects *= mulPoolSize with if(!strcmp("Task", Name)) maxObjects *= mulPoolSize;
	if(pPool)
	{
		CLogFile::Printf("Increaing %sPool from %i Objects to %i Objects", Name, maxObjects, maxObjects*mulPoolSize);

		maxObjects *= mulPoolSize;
		pPool->m_dwEntrySize = entrySize;
		pPool->m_pObjects = (BYTE*)malloc(entrySize * maxObjects);
		pPool->m_pFlags = (BYTE*)malloc(maxObjects);
		pPool->m_bAllocated = 1;
		pPool->m_dwCount = maxObjects;
		pPool->m_nTop = -1;
		
		int n = 0;
		char flag;
		for(pPool->m_dwUsed = 0; n < maxObjects; pPool->m_pFlags[n] = flag & 0x81 | 1)
		{
			pPool->m_pFlags[n] |= 0x80u;
			flag = pPool->m_pFlags[n];
			++n;
		}
		
		CLogFile::Printf("Increased %sPool to %i Objects", Name, maxObjects);
		

		return (int)pPool;
	}

	CLogFile::Printf("Something wrong (%i %s)", pPool, Name);
	
	return 0;
};

void QuadPoolSizes()
{
	mulPoolSize = 2;
	CPatcher::InstallJmpPatch(CGame::GetBase() + 0xC72F10, (DWORD)CPool_hook);
};

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