//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNameTags.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNameTags.h"
#include <Common.h>
#include <CLogFile.h>
#include "CClient.h"

// Nametags definitions etc..
#define b_w					80
#define	b_h					11
#define b_i_p				2
#define h_b_w				(b_w - b_i_p)
#define	nt_a				18
#define nt_a_a				30

extern CClient * g_pClient;

// TODO: if the distance between you and actor/other players is growing, the nametag/health/armor-bar should be getting smaller
CNameTags::CNameTags()
{
	// Flag ourselves as enabled
	m_bEnabled = true;

	// Get the name tag font
	m_pFont = g_pClient->GetGUI()->GetFont("tahoma-bold", 10);
}

CNameTags::~CNameTags()
{

}

void CNameTags::DrawTag(String strName, unsigned int uiHealth, unsigned int uiArmour, Vector2 vecScreenPosition, DWORD dwColor)
{
	// Draw the name tag
	g_pClient->GetGUI()->DrawText(strName, CEGUI::Vector2((vecScreenPosition.fX - (b_w / 2)), vecScreenPosition.fY), 
					 CEGUI::colour(dwColor), m_pFont, false);

	// Ensure correct health and armor values
	if(uiHealth < 0)
		uiHealth = 0;

	if(uiHealth > 100)
		uiHealth = 100;

	if(uiArmour < 0)
		uiArmour = 0;

	if(uiArmour > 100)
		uiArmour = 100;

	// Calculate the correct health and armor values
	float m_fHealth = (h_b_w * ((float)uiHealth / 100));
	float m_fArmour = (h_b_w * ((float)uiArmour / 100));
	float fHealthWidth = Math::Clamp< float >( 0, (b_i_p + m_fHealth), 100 );
	float fArmourWidth = Math::Clamp< float >( 0, (b_i_p + m_fArmour), 100 );

	// Check the health and armor values
	if(m_fHealth < 0)
		m_fHealth = 0.0f;

	if(m_fArmour < 0)
		m_fArmour = 0.0f;

	// Get our graphics
	CGraphics * pGraphics = g_pClient->GetGraphics();

	// Draw boxes
	if(uiArmour > 2)
	{
		// Background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - (b_w / 2)), (vecScreenPosition.fY + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

		// Armour background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 180, 180, 180) );

		// Armour
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a + b_i_p)), fArmourWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(225, 225, 225, 225) );

		// Background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - (b_w / 2)), (vecScreenPosition.fY + nt_a_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

		// Health background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

		// Health
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );
	}
	else
	{
		// Background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - (b_w / 2)), (vecScreenPosition.fY + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

		// Health background
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

		// Health
		pGraphics->DrawBox_2( (vecScreenPosition.fX - ((b_w / 2) - b_i_p)), (vecScreenPosition.fY + (nt_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );

	}
}

void CNameTags::Draw()
{
	// Are we not enabled?
	if(!m_bEnabled)
		return;

	// Get our graphics, player manager and local player
	CGraphics * pGraphics = g_pClient->GetGraphics();
	CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	// Have we our graphics and device class?
	if(!pGraphics || !pGraphics->GetDevice())
		return;

	if(pPlayerManager && pLocalPlayer && pLocalPlayer->IsSpawned() && CGame::GetNameTags())
	{
		// Get the local player position
		CVector3 vecLocalPlayerPosition;
		CVector3 vecWorldPosition;
		Vector2 vecScreenPosition;
		pLocalPlayer->GetPosition(vecLocalPlayerPosition);
			
		// Render player tags
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			// Get the player
			CNetworkPlayer * pPlayer = pPlayerManager->GetAt(i);

			// Is the current player spawned and not the local player?
			if(pPlayer->IsSpawned() && !pPlayer->IsLocalPlayer())
			{
				// Get the player position + add z coord
				pPlayer->GetPosition(vecWorldPosition);
				vecWorldPosition.fZ += 1.15f;

				if(!pPlayer->IsOnScreen())
					continue;
					
				// Convert the position to a screen position
				if(!CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition))
					continue;

				// Is this player not within our view range?
				if((vecLocalPlayerPosition - vecWorldPosition).Length() > 60.0f)
					continue;

				// set the name
				String strNameTag("(%d) %s",i, pPlayer->GetName().Get());

				// Get the name tag color
				DWORD dwColor = ((pPlayer->GetColor() >> 8) | 0xFF000000);

				// Get the player health and armour
				unsigned int uiHealth = (pPlayer->GetHealth() - 100);
				unsigned int uiArmour = (pPlayer->GetArmour());

				// Draw the name tag
				DrawTag(strNameTag, uiHealth, uiArmour, vecScreenPosition, dwColor);
			}
		}

		// Get our actor manager
		CActorManager * pActorManager = g_pClient->GetActorManager();
		
		// Render actor tags
		for(EntityId i = 0; i < MAX_ACTORS; i++)
		{
			// Is the current actor active?
			if(pActorManager->DoesExist(i)/* && g_pActorManager->IsNameTagEnabled(i)*/)
			{
				// Get the player position + add z coord
				vecWorldPosition = pActorManager->GetPosition(i);
				vecWorldPosition.fZ += 1.0f;
					
				// Convert the position to a screen position
				if(!CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition))
					continue;

				// Is this player not within our view range?
				if((vecLocalPlayerPosition - vecWorldPosition).Length() > 60.0f)
					continue;

				// set the name
				String strNameTag("%s", pActorManager->GetName(i).Get());

				// Get the name tag color
				unsigned int dwColor = ((pActorManager->GetNametagColor(i) >> 8) | 0xFF000000);

				// Get the actor health and armour
				unsigned int uiHealth = (unsigned int)(pActorManager->GetHealth(i) - 100);
				unsigned int uiArmour = (unsigned int)(pActorManager->GetArmour(i));

				// Draw the name tag
				DrawTag(strNameTag, uiHealth, uiArmour, vecScreenPosition, dwColor);
			}
		}
	}
}
