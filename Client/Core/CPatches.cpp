//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPatches.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPatches.h"
#include <Patcher/CPatcher.h>
#include "CClient.h"
#include "CPools.h"

extern CClient * g_pClient;

IVTask       * ___pTask = NULL;
unsigned int   uiPlayerInfoIndex = 0;
IVPlayerInfo * pReturnedPlayerInfo = NULL;
unsigned int   uiReturnedIndex = 0;
IVPlayerPed  * _pPlayerPed = NULL;
bool           bInvalidIndex = false;
IVPool       * g_pIVPool;
BYTE         * g_pPoolAllocatedMemory;

void RemoveInitialLoadingScreens()
{
	// Legal, Legal 2, R*, R*N, GTA:IV, ...
	for(int i = 0; i < *(int *)(COffsets::VAR_NumLoadingScreens); ++i)
	{
		*(DWORD *)(COffsets::VAR_FirstLoadingScreenType + i * 400) = ((i <= 4) ? 4 : i);

		if(i <= 4)
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 0;
	}
}

void _declspec(naked) CTask__Destructor_Hook()
{
	_asm
	{
		mov ___pTask, ecx
		pushad
	}


	// Do we have a client task manager?
	if(g_pClient->GetClientTaskManager())
	{
		// Let the client task manager handle this task deletion
		g_pClient->GetClientTaskManager()->HandleTaskDelete(___pTask);
	}

	_asm
	{
		popad
		push esi
		mov esi, ecx
		push esi
		mov dword ptr [esi], offset COffsets::VAR_CTask__VFTable
		jmp COffsets::RETURN_CTask__Destructor
	}
}

void _declspec(naked) CEpisodes__IsEpisodeAvaliable_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		test eax, eax
		jnz it_not
		mov al, 1
		retn 4
it_not:
		xor al, al
		retn 4
	}
}

IVPlayerInfo * GetPlayerInfoFromIndex(unsigned int uiIndex)
{
	// Default to the local player info just incase the index is invalid
	pReturnedPlayerInfo = CGame::GetPools()->GetPlayerInfoFromIndex(0);

	// Is this not the local player info?
	if(uiIndex != 0)
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(uiIndex);

		if(pContextInfo)
			pReturnedPlayerInfo = pContextInfo->GetPlayerInfo()->GetPlayerInfo();
	}

	return pReturnedPlayerInfo;
}

void _declspec(naked) GetPlayerInfoFromIndex_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov uiPlayerInfoIndex, eax
		pushad
	}

	GetPlayerInfoFromIndex(uiPlayerInfoIndex);

	_asm
	{
		popad
		mov eax, pReturnedPlayerInfo
		retn
	}
}

unsigned int GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Default to the local player info just incase the player info is invalid
	uiReturnedIndex = 0;

	// Is this not the local player info?
	if(pPlayerInfo != CGame::GetPools()->GetPlayerInfoFromIndex(0))
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(pPlayerInfo);

		if(pContextInfo)
			uiReturnedIndex = pContextInfo->GetPlayerInfo()->GetPlayerNumber();
	}

	return uiReturnedIndex;
}

void _declspec(naked) GetIndexFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetIndexFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, uiReturnedIndex
		retn
	}
}

IVPlayerPed * GetLocalPlayerPed()
{
	// Default to the local player ped (If available)
	IVPlayerInfo * pPlayerInfo = CGame::GetPools()->GetPlayerInfoFromIndex(0);

	if(pPlayerInfo)
		_pPlayerPed = pPlayerInfo->m_pPlayerPed;
	else
		_pPlayerPed = NULL;

	// Is the local player id valid?
	if(CGame::GetPools()->GetLocalPlayerIndex() != -1)
	{
		// Is the player index not the local player?
		if(CGame::GetPools()->GetLocalPlayerIndex() != 0)
		{
			// Get the context info for the player index
			CContextData * pContextInfo = CContextDataManager::GetContextData((BYTE)CGame::GetPools()->GetLocalPlayerIndex());

			// Is the context info valid?
			if(pContextInfo)
			{
				// Set the player ped to the remote player
				_pPlayerPed = pContextInfo->GetPlayerPed()->GetPlayerPed();
			}
		}
	}

	// Some code to test a theory
	if(_pPlayerPed == NULL)
	{
		if(!bInvalidIndex)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Invalid (Index is %d)", CGame::GetPools()->GetLocalPlayerIndex());
			bInvalidIndex = true;
		}
	}
	else
	{
		if(bInvalidIndex)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Now Valid");
			bInvalidIndex = false;
		}
	}

	return _pPlayerPed;
}

void _declspec(naked) GetLocalPlayerPed_Hook()
{
	_asm
	{
		pushad
	}

	GetLocalPlayerPed();

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

IVPlayerPed * GetPlayerPedFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Is the player info pointer valid?
	if(pPlayerInfo)
		_pPlayerPed = pPlayerInfo->m_pPlayerPed;
	else
	{
		// Player info pointer is invalid, use the local player ped
		_pPlayerPed = GetLocalPlayerPed();
	}

	return _pPlayerPed;
}

void _declspec(naked) GetPlayerPedFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetPlayerPedFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

BYTE * CPool__Allocate(IVPool * pPool)
{
	// Find a free pool slot
	// (Check all slots from pool top, if no free slots found check from the start of the pool)
	int iSlot = pPool->m_nTop;
	bool bFromStart = 0;
	do
	{
		iSlot++;

		if(iSlot == pPool->m_dwCount)
		{
			iSlot = 0;

			if(bFromStart)
			{
				CLogFile::Printf("ERROR: Pool overflow (All %d slots used) (Pool 0x%x)", pPool->m_dwCount, (pPool - CGame::GetBase()));
				g_pClient->GetChatWindow()->AddErrorMessage("ERROR: Pool overflow (All %d slots used) (Pool 0x%x)", pPool->m_dwCount, (pPool - CGame::GetBase()));
				//_asm int 3;
				return NULL;
			}

			bFromStart = true;
		}
	}
	while(!(pPool->m_pFlags[iSlot] & 0x80));

	// Unset the slot is free flag
	pPool->m_pFlags[iSlot] &= 0x7F;

	// Set the new flags
	char cFlag = (pPool->m_pFlags[iSlot] & 0x7F) + 1;

	if((cFlag & 0x7Fu) <= 1)
		pPool->m_pFlags[iSlot] = ((pPool->m_pFlags[iSlot] & 0x80) | 1);
	else
		pPool->m_pFlags[iSlot] = ((cFlag & 0x7F) | pPool->m_pFlags[iSlot] & 0x80);

	// Increment the used object count
	pPool->m_dwUsed++;

	// Set the new pool top
	pPool->m_nTop = iSlot;

	// Return our pool allocated memory
	return &pPool->m_pObjects[iSlot * pPool->m_dwEntrySize];
}

void _declspec(naked) CPool__Allocate_Hook()
{
	_asm
	{
		mov g_pIVPool, ecx
		pushad
	}

	// Call our CPool::Allocate
	g_pPoolAllocatedMemory = CPool__Allocate(g_pIVPool);

	_asm
	{
		popad
		mov eax, g_pPoolAllocatedMemory
		retn
	}
}

void CPatches::Apply()
{
	// Return at start of CTaskSimplePlayRandomAmbients::ProcessPed (Disable random ambient animations)
	// NOTE: This also disables ambient head movements and maybe some other stuff we actually want
	*(DWORD *)(CGame::GetBase() + 0x9849F0) = 0x900004C2;

	// Hook all CPool::Allocate functions to use our own function
	/*CPatcher::InstallJmpPatch((GetBase() + 0x96D520), (DWORD)CPool__Allocate_Hook); // CPool::Allocate
	CPatcher::InstallJmpPatch((GetBase() + 0x439CB0), (DWORD)CPool__Allocate_Hook); // CPool::Allocate_0
	CPatcher::InstallJmpPatch((GetBase() + 0x47E4A0), (DWORD)CPool__Allocate_Hook); // CPool::Allocate_1
	CPatcher::InstallJmpPatch((GetBase() + 0x43A966), (DWORD)CPool__Allocate_Hook); // CPool<CPedData>::Allocate*/

	// Hook GetPlayerInfoFromIndex to use our own function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x817F20), (DWORD)GetPlayerInfoFromIndex_Hook);

	// Hook GetIndexFromPlayerInfo to use our own function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x817DC0), (DWORD)GetIndexFromPlayerInfo_Hook);

	// Hook GetLocalPlayerPed to use our own function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x817F40), (DWORD)GetLocalPlayerPed_Hook);

	// Hook CTask::~CTask to use our own function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xA288D0), (DWORD)CTask__Destructor_Hook);

	// Hook CEpisodes::IsEpisodeAvaliable to use our own function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x814810), (DWORD)CEpisodes__IsEpisodeAvaliable_Hook);

	// Disable initial loading screens
	CPatcher::InstallCallPatch((CGame::GetBase() + 0x424B26), (DWORD)RemoveInitialLoadingScreens);

	// Always draw vehicle hazzard lights
	CPatcher::InstallNopPatch(COffsets::PATCH_CVehicle__HazzardLightsOn, 2);

	// jenksta: dont think you realise what your doing here, so disabling it...
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xD549DC), (CGame::GetBase() + 0xD549C0));// Disables loading music, reduces gta loading time & fix crash
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xD549EC), (CGame::GetBase() + 0xD549D0));// Disables loading music, reduces gta loading time & fix crash

	// Make the game think we are not connected to the internet
	*(BYTE *)(CGame::GetBase() + 0x10F1390) = 0; // byteInternetConnectionState
	*(DWORD *)(CGame::GetBase() + 0x7AF1A0) = 0x90C3C032; // xor al, al; retn; nop

	// Don't load startup.sco
	*(BYTE *)(CGame::GetBase() + 0x809A8A) = 0x75;

	// Always start a new game
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x5B0311), (CGame::GetBase() + 0x5B03BF));

	// Disable parked cars
	CPatcher::InstallRetnPatch(CGame::GetBase() + 0xB3EDF0);

	// Disable emergency services and garbage trucks
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x4215CF), 5);

	// Disable vehicle entries
	*(DWORD *)(CGame::GetBase() + 0x9B1ED0) = 0x0CC2C033;
	*(BYTE *)(CGame::GetBase() + 0x9B1ED4) = 0x00;

	// Disable vehicle exits
	*(BYTE *)(CGame::GetBase() + 0x9BBBFC) = 0xEB;

	// Disable random peds and vehicles
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x8ACD64), 5);
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x421610), 5);
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x81B22E), 5);

	// Disable scenario peds
	*(BYTE *)(CGame::GetBase() + 0x9F72C0) = 0xB8; // mov eax,
	*(DWORD *)(CGame::GetBase() + 0x9F72C1) = 0x0; // 0
	*(BYTE *)(CGame::GetBase() + 0x9F72C5) = 0xC3; // retn

	// Disable fake cars
	CPatcher::InstallRetnPatch(CGame::GetBase() + 0x9055D0);

	// Disable long sleep in CGame::Run
	*(DWORD *)(CGame::GetBase() + 0x401835) = 1;

	// Don't initialize error reporting
	CPatcher::InstallRetnPatch(CGame::GetBase() + 0xD356D0);

	// Certificates check (RETN 8)
	*(DWORD *)(CGame::GetBase() + 0x403F10) = 0x900008C2;

	// xor eax, eax - address of the RGSC object
	*(DWORD *)(CGame::GetBase() + 0x40262D) = 0x4AE9C033;

	// Skip RGSC connect and EFC checks (jmp 40289E)
	*(DWORD *)(CGame::GetBase() + 0x402631) = 0x90000002;

	// NOP; MOV [g_rgsc], eax
	*(WORD *)(CGame::GetBase() + 0x402883) = 0xA390;

	// Disable VDS102 error
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x4028ED), 42);

	// Last RGSC init check (NOP*6)
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x40291D), 6);

	// Skip missing tests
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x402B12), 14);
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x402D17), 14);
	*(DWORD *)(CGame::GetBase() + 0x403870) = 0x90CC033; // xor eax, eax; retn
	*(DWORD *)(CGame::GetBase() + 0x404250) = 0x90CC033; // xor eax, eax; retn

	// Disable securom spot checks (mov al, 1; retn)
	*(DWORD *)(CGame::GetBase() + 0xBAC160) = 0x90C301B0;
	*(DWORD *)(CGame::GetBase() + 0xBAC180) = 0x90C301B0;
	*(DWORD *)(CGame::GetBase() + 0xBAC190) = 0x90C301B0;
	*(DWORD *)(CGame::GetBase() + 0xBAC1C0) = 0x90C301B0;
}
