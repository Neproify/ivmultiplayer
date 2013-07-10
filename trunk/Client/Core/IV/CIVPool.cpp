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


/*static int mulPoolSize = 1;
int __stdcall  CPool_hook_chunk(void* this_, int maxObjects, const char* Name, int entrySize);
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

int __stdcall  CPool_hook_chunk(void* this_, int maxObjects, const char* Name, int entrySize)
{
	IVPool *pPool = (IVPool*)this_;

	// Here we simply increase the size but we could also replace the entire pool with our own
	// Or we can filter the pools by their name for example Task and only modify this pool
	// to increase only the task pool replace maxObjects *= mulPoolSize with if(!strcmp("Task", Name)) maxObjects *= mulPoolSize;
	if(pPool)
	{

		if(!strcmp("PtrNode Double", (const char*)Name)
			|| !strcmp("EntryInfoNodes", Name)
 			|| !strcmp("PtrNode Single", Name)
 			|| !strcmp("Vehicles", (const char*)Name)
-			|| !strcmp("VehicleStruct", Name))
-			//|| !strcmp("fragInstGta", Name)
-			//|| !strcmp("phInstGta", Name)
-			//|| !strcmp("fragInstNMGta", Name))
		{
			CLogFile::Printf("Increaing %sPool from %i Objects to %i Objects", Name, maxObjects, maxObjects*mulPoolSize);
			maxObjects *= mulPoolSize;

			pPool->m_dwEntrySize = entrySize;
			pPool->m_pObjects = (BYTE*)CGame::Alloc(entrySize * maxObjects);
			pPool->m_pFlags = (BYTE*)CGame::Alloc(maxObjects);

			pPool->m_bAllocated = 1;
			pPool->m_dwCount = maxObjects;
			pPool->m_nTop = -1;

			int n = 0;
			char flag;
			int v5 = 0;
			BYTE* v8;
			BYTE v7;
			BYTE* v6;
			for(pPool->m_dwUsed = 0; v5 < maxObjects; *v8 = v7 & 0x81 | 1)
			{
				*(pPool->m_pFlags + v5) |= 0x80;
				v6 = pPool->m_pFlags;
				v7 = *(v6 + v5);
				v8 = v5++ +v6;
			}
			
			CLogFile::Printf("Increased %sPool to %i Objects", Name, maxObjects);
		}
		else
		{
			pPool->m_dwEntrySize = entrySize;
			pPool->m_pObjects = (BYTE*)CGame::Alloc(entrySize * maxObjects);
			pPool->m_pFlags = (BYTE*)CGame::Alloc(maxObjects);

			pPool->m_bAllocated = 1;
			pPool->m_dwCount = maxObjects;
			pPool->m_nTop = -1;

			int n = 0;
			char flag;
			int v5 = 0;
			BYTE* v8;
			BYTE v7;
			BYTE* v6;
			for(pPool->m_dwUsed = 0; v5 < maxObjects; *v8 = v7 & 0x81 | 1)
			{
				*(pPool->m_pFlags + v5) |= 0x80;
				v6 = pPool->m_pFlags;
				v7 = *(v6 + v5);
				v8 = v5++ +v6;
			}
		}
		return (int)pPool;
	}

	CLogFile::Printf("Something wrong (%i %s)", pPool, Name);
	
	return 0;
};*/

/*
This will multiply the size of the given pools by the value in multi [default: 4]
*/
void IncreasePoolSizes(int multi)
{
	//mulPoolSize = multi;
	DWORD * dwPtrNodeDoubleMaxObjects	= (DWORD*)(CGame::GetBase() + 0xB534F6);
	DWORD * dwEntryInfoNodesMaxObjects	= (DWORD*)(CGame::GetBase() + 0xC796D6);
	DWORD * dwPtrNodeSingleMaxObjects	= (DWORD*)(CGame::GetBase() + 0xB534B6);
	DWORD * dwVehiclesMaxObjects		= (DWORD*)(CGame::GetBase() + 0x9D43B9);
	BYTE * byteVehicleStructMaxObjects	= (BYTE*)(CGame::GetBase() + 0xBEA871);

	*dwPtrNodeDoubleMaxObjects		*= multi;
	*dwEntryInfoNodesMaxObjects		*= multi;
	*dwPtrNodeSingleMaxObjects		*= multi;
	*dwVehiclesMaxObjects			*= multi;
	*byteVehicleStructMaxObjects	= (BYTE)124; // 123 vehicles are in gta + 1 spare just in case
	
	//CPatcher::InstallJmpPatch(CGame::GetBase() + 0xC72F10, (DWORD)CPool_hook);
	/*CPatcher::InstallJmpPatch(CGame::GetBase() + 0x43AA61, (DWORD)SetupPool_Hook);*/ //PedPool
	/*CPatcher::InstallJmpPatch(CGame::GetBase() + 0x43A8C1, (DWORD)SetupPool_Hook);*/
};


/*_declspec(naked) void __stdcall SetupPool_Hook()
{
	IVPool *pPool;
	_asm
	{
		mov pPool, esi
		pushad
	}

	CLogFile::Printf("Increaing Pool");

	int entrySize;
	int maxObjects;
	maxObjects = pPool->m_dwCount;
	entrySize = pPool->m_dwEntrySize;
	maxObjects *= mulPoolSize;

	pPool->m_dwEntrySize = entrySize;
	
	CGame::Free(pPool->m_pObjects);
	CGame::Free(pPool->m_pFlags);

	pPool->m_pObjects = (BYTE*)CGame::Alloc(entrySize * maxObjects);
	pPool->m_pFlags = (BYTE*)CGame::Alloc(maxObjects);

	pPool->m_bAllocated = 1;
	pPool->m_dwCount = maxObjects;
	pPool->m_nTop = -1;

	int n;
	n = 0;
	char flag;
	int v5;
		v5 = 0;
	BYTE* v8;
	BYTE v7;
	BYTE* v6;
	for(pPool->m_dwUsed = 0; v5 < maxObjects; *v8 = v7 & 0x81 | 1)
	{
		*(pPool->m_pFlags + v5) |= 0x80;
		v6 = pPool->m_pFlags;
		v7 = *(v6 + v5);
		v8 = v5++ +v6;
	}
	CLogFile::Printf("Increased %sPool to %i Objects", "Hallo", maxObjects);

	_asm 
	{
		popad
		mov eax, pPool
		retn
	}
}*/

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