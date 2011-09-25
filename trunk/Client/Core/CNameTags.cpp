//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNameTags.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNameTags.h"
#include "CGUI.h"
#include "CLocalPlayer.h"
#include "CPlayerManager.h"

extern CGUI           * g_pGUI;
extern CLocalPlayer   * g_pLocalPlayer;
extern CPlayerManager * g_pPlayerManager;

CNameTags::CNameTags()
{
	// Flag ourselves as enabled
	m_bEnabled = true;

	// Get the name tag font
	m_pFont = g_pGUI->GetFont("tahoma-bold", 10);
}

CNameTags::~CNameTags()
{

}

void CNameTags::Draw()
{
	// Are we not enabled?
	if(!m_bEnabled)
		return;

	if(g_pPlayerManager)
	{
		// Get the local player position
		// TODO: Use camera position instead of local player position
		CVector3 vecLocalPlayerPosition;
		g_pLocalPlayer->GetPosition(vecLocalPlayerPosition);

		CVector3 vecWorldPosition;
		Vector2 vecScreenPosition;

		// Loop through all players
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			// Is the current player active?
			if(g_pPlayerManager->DoesExist(i))
			{
				// Get the player pointer
				CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(i);

				// Is this the local player?
				if(pPlayer->IsLocalPlayer())
					continue;

				// Is this player not spawned?
				if(!pPlayer->IsSpawned())
					continue;

				// Is this player on screen?
				if(!pPlayer->IsOnScreen())
					continue;

				// Get the player position
				pPlayer->GetPosition(vecWorldPosition);

				// Is this player not within our view range?
				// TODO: Find proper values and base on current weapon e.t.c.
				if((vecLocalPlayerPosition - vecWorldPosition).Length() > 50.0f)
					continue;

				// Convert the position to a screen position
				CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition);

				// Create the name tag string
				String strNameTag("(%d) %s", i, pPlayer->GetName().Get());

				// Get the name tag color
				DWORD dwColor = ((pPlayer->GetColor() >> 8) | 0xFF000000);

				// Draw the name tag
				g_pGUI->DrawText(strNameTag, CEGUI::Vector2(vecScreenPosition.X, vecScreenPosition.Y), CEGUI::colour(dwColor), m_pFont, false);
			}
		}
	}
}
