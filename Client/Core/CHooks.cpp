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
	CLogFile::Printf("Start patching crash fixes...");

	// jenksta: disabling this until you properly find out what it does
	// Fix vehicle crash(Be carefull, we have to look if this function disables important vehicle stuff..) -> 8 xrefs
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// jenksta: whats the point in disabling this? if this is called something went wrong
	// in the game and it wants to close, cancelling this will just force it to stay open
	// possibly causing even more issues?
	// Disables Warning Messages(like "Unkown resource found") -> Disables only the window(and exit code part)...
	CPatcher::InstallJmpPatch((CGame::GetBase() + /*0x5A932D*/0x5A8CB0), (CGame::GetBase() + 0x5A9361));

	// jenksta: disabling this until you properly find out what it does
	//*(BYTE*)(CGame::GetBase() + 0x119DB14) = 1;

	// jenksta: do you even know what any of this does?
	CPatcher::InstallJmpPatch((GetBase() + 0x9E2E30), (GetBase() + 0x9E2FFB));
	CPatcher::InstallJmpPatch((GetBase() + 0xCA76E0), (GetBase() + 0xCA79C9));
	CPatcher::InstallJmpPatch((GetBase() + 0x446970), (GetBase() + 0x446AFF));

	// Disable automatic radar turn-on(in vehicle)
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x42D3D1), 7); // initialize or render(seems to be a render func)
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x811B6E), 5); // from init blip gtaiv func(startup)
	
	// jenksta: do you even know what any of this does?
	// Prevent crashes on player connect(associated with ped intelligence)
	CPatcher::InstallJmpPatch((GetBase() + 0x815380), (GetBase() + 0x8153D4));
	CPatcher::InstallJmpPatch((GetBase() + 0x625F15), (GetBase() + 0x625F1D));
	CPatcher::InstallJmpPatch((GetBase() + 0xB2B24D), (GetBase() + 0xB2B259));

	// Disable weapon when entering vehicle
	CPatcher::InstallNopPatch((CGame::GetBase() + 0x9C5994), 0x30);

	// jenksta: disabling this until you properly find out what it does
	//CPatcher::InstallJmpPatch((GetBase() + 0x639710), (DWORD)TextureSelect_Hook);

	CLogFile::Printf("Finished patching crash fixes.");
}
