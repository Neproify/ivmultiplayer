//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCrashFixes.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#include <Patcher/CPatcher.h>
#include "CCrashFixes.h"
#include "CGame.h"

void CCrashFixes::Install()
{
	// Fix vehicle crash(Be carefull, we have to look if this function disables important vehicle stuff..) -> 8 xrefs
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0xCBA1F0), (CGame::GetBase() + 0xCBA230));

	// Entity fix, seems to be associated with IVPedPool
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0x6277B0), (CGame::GetBase() + 0x6279FE));

	// Deformation check
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0xB7CC4F), (CGame::GetBase() + 0xB7CC68));
	// Fix some random crashes(Don't disable this function!)
	//CPatcher::InstallJmpPatch((CGame::GetBase() + 0x72B240), (CGame::GetBase() + 0x72B813));

	// Disables Warning Messages(like "Unkown resource found") -> Disables only the window(and exit code part)...
	// Note: Don't enable it, the window only appears when GTA IV fails to initialize sth...
	//CPatcher::InstallJmpPatch((CGame::GetBase() + /*0x5A932D*/0x5A8CB0), (CGame::GetBase() + 0x5A9361));

}

/*
int __thiscall sub_CBA1F0(int this, unsigned int *a2)
{
  int result; // eax@1
  unsigned int v3; // edx@1
  int v4; // esi@1

  v4 = *(_DWORD *)(this + 1536);
  v3 = *a2;
  result = 0;
  if ( v4 > 0 )
  {
    while ( v3 < *(_DWORD *)(this + 4 * result) || v3 >= *(_DWORD *)(this + 4 * result + 512) )
    {
      ++result;
      if ( result >= v4 )
        return result;
    }
    if ( result != -1 )
    {
      result = v3 + *(_DWORD *)(this + 4 * result + 1024);
      *a2 = result;
    }
  }
  return result;
}
*/

/*
int __cdecl sub_6277B0(int a1, int a2, int a3, int a4)
{
  int v4; // edx@1
  int v5; // edi@1
  int v6; // ebp@2
  int v7; // esi@3
  int v8; // eax@7
  int i; // ecx@9
  int v10; // ebx@17
  int v11; // ecx@19
  char v12; // cl@22
  int v13; // eax@23
  int result; // eax@35
  int v15; // eax@39
  int v16; // ecx@42
  int v17; // ecx@44
  int v18; // ecx@46
  int v19; // ecx@48
  int v20; // eax@51
  int v21; // ecx@56
  int v22; // ecx@58
  char v23; // zf@60
  int v24; // ecx@62
  int v25; // ecx@64
  int v26; // eax@67

  v5 = a1;
  v4 = *(_DWORD *)(a1 + 8);
  if ( !v4 )
  {
    v6 = *(_DWORD *)(a1 + 12);
LABEL_3:
    v7 = *(_DWORD *)(a1 + 4);
    if ( v6 )
      *(_DWORD *)(v6 + 4) = v7;
    if ( *(_DWORD *)a2 == v5 )
    {
      *(_DWORD *)a2 = v6;
    }
    else
    {
      v13 = *(_DWORD *)(v5 + 4);
      if ( *(_DWORD *)(v13 + 8) == v5 )
        *(_DWORD *)(v13 + 8) = v6;
      else
        *(_DWORD *)(v13 + 12) = v6;
    }
    if ( *(_DWORD *)a3 == v5 )
    {
      if ( *(_DWORD *)(v5 + 12) )
        *(_DWORD *)a3 = sub_623D20(v6);
      else
        *(_DWORD *)a3 = *(_DWORD *)(v5 + 4);
    }
    if ( *(_DWORD *)a4 == v5 )
    {
      if ( *(_DWORD *)(v5 + 8) )
        *(_DWORD *)a4 = sub_623D40(v6);
      else
        *(_DWORD *)a4 = *(_DWORD *)(v5 + 4);
    }
    v10 = a2;
    goto LABEL_35;
  }
  v8 = *(_DWORD *)(a1 + 12);
  if ( !v8 )
  {
    v6 = *(_DWORD *)(a1 + 8);
    goto LABEL_3;
  }
  for ( i = *(_DWORD *)(v8 + 8); i; i = *(_DWORD *)(i + 8) )
    v8 = i;
  v6 = *(_DWORD *)(v8 + 12);
  a1 = v8;
  if ( v8 == v5 )
    goto LABEL_3;
  *(_DWORD *)(v4 + 4) = v8;
  *(_DWORD *)(v8 + 8) = *(_DWORD *)(v5 + 8);
  if ( v8 == *(_DWORD *)(v5 + 12) )
  {
    v7 = v8;
  }
  else
  {
    v7 = *(_DWORD *)(v8 + 4);
    if ( v6 )
      *(_DWORD *)(v6 + 4) = v7;
    *(_DWORD *)(*(_DWORD *)(v8 + 4) + 8) = v6;
    *(_DWORD *)(v8 + 12) = *(_DWORD *)(v5 + 12);
    *(_DWORD *)(*(_DWORD *)(v5 + 12) + 4) = v8;
  }
  v10 = a2;
  if ( *(_DWORD *)a2 == v5 )
  {
    *(_DWORD *)a2 = v8;
  }
  else
  {
    v11 = *(_DWORD *)(v5 + 4);
    if ( *(_DWORD *)(v11 + 8) == v5 )
      *(_DWORD *)(v11 + 8) = v8;
    else
      *(_DWORD *)(v11 + 12) = v8;
  }
  *(_DWORD *)(v8 + 4) = *(_DWORD *)(v5 + 4);
  v12 = *(_BYTE *)v8;
  *(_BYTE *)v8 = *(_BYTE *)v5;
  *(_BYTE *)v5 = v12;
  a1 = v5;
LABEL_35:
  result = a1;
  if ( *(_BYTE *)a1 )
  {
    if ( v6 != *(_DWORD *)v10 )
    {
      while ( !v6 || *(_BYTE *)v6 == 1 )
      {
        v15 = *(_DWORD *)(v7 + 8);
        if ( v6 == v15 )
        {
          v15 = *(_DWORD *)(v7 + 12);
          if ( !*(_BYTE *)v15 )
          {
            *(_BYTE *)v15 = 1;
            *(_BYTE *)v7 = 0;
            sub_627BE0(v7, v10);
            v15 = *(_DWORD *)(v7 + 12);
          }
          v16 = *(_DWORD *)(v15 + 8);
          if ( v16 && *(_BYTE *)v16 != 1 || (v17 = *(_DWORD *)(v15 + 12)) != 0 && *(_BYTE *)v17 != 1 )
          {
            v18 = *(_DWORD *)(v15 + 12);
            if ( !v18 || *(_BYTE *)v18 == 1 )
            {
              v19 = *(_DWORD *)(v15 + 8);
              if ( v19 )
                *(_BYTE *)v19 = 1;
              *(_BYTE *)v15 = 0;
              sub_627C30(v15, v10);
              v15 = *(_DWORD *)(v7 + 12);
            }
            *(_BYTE *)v15 = *(_BYTE *)v7;
            *(_BYTE *)v7 = 1;
            v20 = *(_DWORD *)(v15 + 12);
            if ( v20 )
              *(_BYTE *)v20 = 1;
            sub_627BE0(v7, v10);
            break;
          }
        }
        else
        {
          if ( !*(_BYTE *)v15 )
          {
            *(_BYTE *)v15 = 1;
            *(_BYTE *)v7 = 0;
            sub_627C30(v7, v10);
            v15 = *(_DWORD *)(v7 + 8);
          }
          v21 = *(_DWORD *)(v15 + 12);
          if ( v21 && *(_BYTE *)v21 != 1 || (v22 = *(_DWORD *)(v15 + 8)) != 0 && *(_BYTE *)v22 != 1 )
          {
            v24 = *(_DWORD *)(v15 + 8);
            if ( !v24 || *(_BYTE *)v24 == 1 )
            {
              v25 = *(_DWORD *)(v15 + 12);
              if ( v25 )
                *(_BYTE *)v25 = 1;
              *(_BYTE *)v15 = 0;
              sub_627BE0(v15, v10);
              v15 = *(_DWORD *)(v7 + 8);
            }
            *(_BYTE *)v15 = *(_BYTE *)v7;
            *(_BYTE *)v7 = 1;
            v26 = *(_DWORD *)(v15 + 8);
            if ( v26 )
              *(_BYTE *)v26 = 1;
            sub_627C30(v7, v10);
            break;
          }
        }
        v6 = v7;
        *(_BYTE *)v15 = 0;
        v23 = v7 == *(_DWORD *)v10;
        v7 = *(_DWORD *)(v7 + 4);
        if ( v23 )
          break;
      }
    }
    result = a1;
    if ( v6 )
      *(_BYTE *)v6 = 1;
  }
  return result;
}
*/