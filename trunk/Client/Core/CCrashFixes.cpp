//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.cpp
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================
#include <Patcher/CPatcher.h>
#include "CCrashFixes.h"
#include "Scripting.h"
#include "CClient.h"

extern CClient *g_pClient;

void _declspec(naked) CRASH_625F15_HOOK()
{
	_asm
	{
		test	eax, eax
		jz		keks
		cmp     eax, 100000h
		jl		keks
		mov     edx, [eax]
		push    1
		mov     ecx, edi
		call    edx

keks_patch:
		mov     al, 1
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		add     esp, 0Ch
		retn    4
keks:
		pushad
	}

	g_pClient->GetChatWindow()->AddErrorMessage("Prevent crash at 0x625F15");

	_asm
	{
		popad
		jmp keks_patch
	}
}


void CCrashFixes::Install()
{
	// This disables some calculate for modelinfo but it seems this is not necessary
	// Maybe we can disable this patch
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// this disables a call to a destructor of a member in rageResourceCache [field_244] 
	CPatcher::InstallJmpPatch((GetBase() + 0x625F15), (DWORD)CRASH_625F15_HOOK /*(GetBase() + 0x625F1D)*/);

	// This needs to be disabled due to some crashes and to enable the blocked vehicles such as uranus, hellfury, etc.
	/* INFO: crash occure exactly when accessing dword_13BEEE0 this is related to ZonesNames, but disabling this function dont destroy anything
	TODO: find what this function does
	this function checks some flags in modelInfos and loading some models they seems to be not needed
	This seems to be associated to loading models but they are not used!?
	*/
	CPatcher::InstallRetnPatch(GetBase() + 0x8F2F40);
}
