//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#include <Patcher/CPatcher.h>
#include "CHooks.h"
#include "Scripting.h"

int thisobject;
void _declspec(naked) PoolCalculationHook()
{
	_asm
	{
		mov eax, [ecx + 8]
		mov thisobject, eax
		sub eax, [ecx + 14h]
		pushad
	}
	// CLogFile::Printf("[PoolCalculation]Hook: %d",thisobject);
	_asm
	{
		popad
		retn
	}
}

bool bCracked = false;
void _declspec(naked) CrackedStartup()
{
	_asm 
	{
		xor	eax, eax
		cmp	bCracked, 1
		setz al
		pushad
	}
	//CLogFile::Printf("Cracked connect: %d,", bCracked);
	_asm
	{
		popad
		retn
	}
}

DWORD dwAddress;
void _declspec(naked) C_0xBFE330_Hook()
{
	_asm pushad;
	dwAddress = (CGame::GetBase() + 0xBFE476);
	_asm
	{
		popad
		jmp dwAddress
		retn
	}
}

void _declspec(naked) C_0xC03830_Hook()
{
	_asm pushad;
	dwAddress = (CGame::GetBase() + 0xBFE476);
	_asm
	{
		popad
		jmp dwAddress
		retn
	}
}

void _declspec(naked) C_0xCA8260_Hook()
{
	_asm pushad;
	dwAddress = (CGame::GetBase() + 0xCA834F);
	_asm
	{
		popad
		jmp dwAddress
		retn
	}
}

void _declspec(naked) C_0x8B8460_Hook()
{
	_asm pushad;
	dwAddress = (CGame::GetBase() + 0x8B86BD);
	_asm
	{
		popad
		jmp dwAddress
		retn
	}
}

void _declspec(naked) C_0xA1AEF0_Hook()
{
	_asm pushad;
	dwAddress = (CGame::GetBase() + 0xA1B018);
	_asm
	{
		popad
		jmp dwAddress
		retn
	}
}

void _declspec(naked) C_0xA1E5F0_Hook()
{
	_asm pushad;
	CLogFile::Printf("Call unkown function 0xA1E5F0");
	_asm popad;
}

int hFile2;
HANDLE iTexture;
int v2;// edi@1
signed int v3;// esi@1
int v4;// ecx@2
int v5;// eax@3
int result;// eax@5
DWORD dwJump;
void _declspec(naked) TextureSelect_Hook()
{   
	_asm
	{
		mov eax, [esp+4]
        mov iTexture, eax
        mov eax, [esp+8]
        mov hFile2, eax  
        pushad
	}

	if(hFile2 < 10000000) 
	{
		CLogFile::Printf("WARNING! TextureSelect has an invalid pointer((Pointer)%p/(File/Type)%s!", hFile2, iTexture);
		/*_asm
		{
			popad
			retn 4
		}*/
	}
	else	
		;//CLogFile::Printf("TextureSelect_Hook: (Pointer)%p/(File/Type)%s", hFile2, iTexture);

	dwJump = (CGame::GetBase() + 0x639715);
	_asm
	{
		popad
		push ebx
		push esi
		push edi
		mov edi, ecx
		jmp dwJump
	}
}

void CHooks::Install()
{
	CLogFile::Printf("Start patching crashfixes...");

#ifdef IVMP_TRAINS
	// Disable "normal" vehicle generation
	/*CPatcher::InstallJmpPatch((GetBase() + 0x973B06), (GetBase() + 0x973BC2));
	CPatcher::InstallJmpPatch((GetBase() + 0x973B28), (GetBase() + 0x973BC2));
	CPatcher::InstallJmpPatch((GetBase() + 0x973B4A), (GetBase() + 0x973BC2));
	CPatcher::InstallJmpPatch((GetBase() + 0x973B86), (GetBase() + 0x973BC2));
	CPatcher::InstallJmpPatch((GetBase() + 0x973BA4), (GetBase() + 0x973BC2));*/

	CPatcher::InstallJmpPatch((GetBase() + 0x973B06), (DWORD)C_0xBFE330_Hook);
	CPatcher::InstallJmpPatch((GetBase() + 0x973B28), (DWORD)C_0xC03830_Hook);
	CPatcher::InstallJmpPatch((GetBase() + 0x973B4A), (DWORD)C_0xCA8260_Hook);
	CPatcher::InstallJmpPatch((GetBase() + 0x973B86), (DWORD)C_0x8B8460_Hook);
	CPatcher::InstallJmpPatch((GetBase() + 0x973BA4), (DWORD)C_0xA1AEF0_Hook);

#endif
	// Fix vehicle crash(Be carefull, we have to look if this function disables important vehicle stuff..) -> 8 xrefs
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// Disables Warning Messages(like "Unkown resource found") -> Disables only the window(and exit code part)...
	CPatcher::InstallJmpPatch((CGame::GetBase() + /*0x5A932D*/0x5A8CB0), (CGame::GetBase() + 0x5A9361));

	CPatcher::Unprotect((CGame::GetBase() + 0x119DB14), 1);
	*(BYTE*)(CGame::GetBase() + 0x119DB14) = 1;

	//*(BYTE *)(CGame::GetBase() + 0xF21D36) = 0; // control issue
	
	CPatcher::InstallJmpPatch((GetBase() + 0x9E2E30), (GetBase() + 0x9E2FFB));
	CPatcher::InstallJmpPatch((GetBase() + 0xCA76E0), (GetBase() + 0xCA79C9));
	CPatcher::InstallJmpPatch((GetBase() + 0x446970), (GetBase() + 0x446AFF));
	CPatcher::InstallJmpPatch((GetBase() + 0x447270), (DWORD)PoolCalculationHook);
	CPatcher::InstallJmpPatch((GetBase() + 0x7B79E0), (DWORD)CrackedStartup);

	// Disable automatic radar turn-on(in vehicle)
	CPatcher::InstallJmpPatch((GetBase() + 0x42D3D1), (GetBase() + 0x42D3D8)); // initialize or render(seems to be a render func)
	CPatcher::InstallJmpPatch((GetBase() + 0x811B6E), (GetBase() + 0x811B73)); // from init blip gtaiv func(startup)
	
	// Prevent crashes on player connect(associated with ped intelligence)
	CPatcher::InstallJmpPatch((GetBase() + 0x815380), (GetBase() + 0x8153D4));
	CPatcher::InstallJmpPatch((GetBase() + 0x625F15), (GetBase() + 0x625F1D));
	CPatcher::InstallJmpPatch((GetBase() + 0xB2B24D), (GetBase() + 0xB2B259));
	CPatcher::InstallJmpPatch((GetBase() + 0x9C5994), (GetBase() + 0x9C59C4)); // Disables shotgun given when enter vehicle

	CPatcher::InstallJmpPatch((GetBase() + 0x639710), (DWORD)TextureSelect_Hook);
	CLogFile::Printf("Finished patching crashfixes..");
}