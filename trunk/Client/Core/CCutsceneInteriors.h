//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCutsceneInteriors.h
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Scripting.h"
#include "CLocalPlayer.h"

extern CLocalPlayer * g_pLocalPlayer;

class CCutsceneInteriors
{
public:
	static void Update(unsigned int uiLocalPlayerInterior)
	{
		switch(uiLocalPlayerInterior)
		{
			case 0x25bb0628: // Bernie's apartment
			case 0x3fc9984c: // Bernie's apartment (kitchen)
			case 0x91d06b05: // Bernie's apartment (hall)
			case 0x8d02b26d: // Brucie's apartment
			case 0x426596cb: // Church & Italian Restaurant & United Liberty Paper
			case 0x7d943bab: // Italian Restaurant (behind the counter)
			case 0x8f3c0a12: // Italian Restaurant (kitchen)
			case 0x34527aa1: // Italian Restaurant (front area), Little Jacob's apartment (hall)
			case 0xb919b6f7: // Elizabeta's apartment
			case 0x927153fd: // Francis/police station (downstairs/reception)
			case 0x104bcbab: // Francis/police station (upstairs/office)
			case 0xacad6b3d: // Little Jacob's apartment
			case 0x1d391b6c: // Little Jacob's apartment (stairs)
			case 0x2d09a627: // Manny's Community Center
			case 0x96971bbe: // Manny's Community Center (office)
			case 0xd89c7306: // McReary (Living Room)
			case 0x6553024e: // McReary (Hall)
			case 0x4fc237a5: // McReary (Kitchen)
			case 0xdee4e578: // Pegorino's house
			case 0x7e8801d3: // Pegorino's house (office)
			case 0x11fd2c94: // Roman's cab office
			case 0xd565a9e0: // Michelle? (mich_a.cut 934.73 -189.6 34.16)
			case 0xfa8eb76:  // Michelle? (mich_a.cut 934.73 -189.6 34.16)
				{
					CVector3 vecPosition;
					g_pLocalPlayer->GetPosition(vecPosition);

					if(vecPosition.fZ < 10.0f)
						break;
				}
			case 0x810bf758: // Faustin's basement
			case 0x7ab52f5b: // Faustin's house (1)
			case 0x5ffc63b3: // Faustin's house (kitchen)
			case 0xf6b31e9a: // Faustin's house (3)
			case 0xde8c4cb4: // Faustin's house (4)
			case 0xa1270ec4: // Phil Bell's office
			case 0x9199dedc: // Prison
				Scripting::InitCutscene("kyefag");
				break;
			default:
				Scripting::ClearNamedCutscene("kyefag");
		}
	}
};
