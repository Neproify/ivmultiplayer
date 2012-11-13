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
#include "CActorManager.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CCamera.h"
#include "CChatWindow.h"
#include <Common.h>
#include "CLogFile.h"

extern CGUI           * g_pGUI;
extern CLocalPlayer   * g_pLocalPlayer;
extern CPlayerManager * g_pPlayerManager;
extern CActorManager  * g_pActorManager;
extern CGame		  * g_pGame;
extern CGraphics	  * g_pGraphics;
extern CCamera		  * g_pCamera;
extern CChatWindow	  * g_pChatWindow;

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

	// Have we our graphics and device class?
	if(!g_pGraphics || !g_pGraphics->GetDevice())
		return;

	if(g_pPlayerManager && g_pCamera && g_pLocalPlayer && g_pLocalPlayer->IsSpawned() && g_pGame->GetNameTags())
	{
		// Get the local player position
		CVector3 vecLocalPlayerPosition;
		g_pLocalPlayer->GetPosition(vecLocalPlayerPosition);
			
		// Get the lookat data from camera
		CIVCam * pGameCam = g_pCamera->GetGameCam();
		pGameCam->GetPosition(vecCamPosition);
		vecCamForward = pGameCam->GetCam()->m_data1.m_matMatrix.vecForward;
		vecLookAt.fX = vecCamPosition.fX + vecCamForward.fX;
		vecLookAt.fY = vecCamPosition.fY + vecCamForward.fY;
		vecLookAt.fZ = vecCamPosition.fZ + vecCamForward.fZ;

		CVector3 vecWorldPosition;
		Vector2 vecScreenPosition;
			
		// First render gui stuff(nametags), than boxes
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
				// Is the current player active?
				if(g_pPlayerManager->DoesExist(i) && g_pLocalPlayer->GetPlayerId() != i)
				{
					CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(i);

					// Get the player position + add z coord
					pPlayer->GetPosition(vecWorldPosition);
					vecWorldPosition.fZ += 1.15f;
					
					// Convert the position to a screen position
					CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition);

					// Is this player not within our view range?
					if((vecLocalPlayerPosition - vecWorldPosition).Length() > 60.0f)
						continue;

					//if(g_pGraphics)
					//{
					//	CVector3 vecScreen;
					//	g_pGraphics->GetScreenPositionFromWorldPosition(vecWorldPosition, &vecScreen);
					//	
					//	if( vecScreen.fZ < 0 )
					//		return;
					//}
					//else
					//	return;

					// Check the cam position with the world position(fix desync)
					if(((vecLookAt - vecWorldPosition).Length() * 1.35) < ((vecLocalPlayerPosition - vecWorldPosition).Length()))
						continue;
					
					// set the position to the newest
					vecScreenPosition.Y += ( (vecLocalPlayerPosition - vecWorldPosition).Length() * 0.25f); // must be added, otherwise wrong pos

					// set the name
					String strNameTag("(%d) %s",i, pPlayer->GetName().Get());

					// Get the name tag color
					DWORD dwColor = ((pPlayer->GetColor() >> 8) | 0xFF000000);

					// Draw the name tag
					g_pGUI->DrawText(strNameTag, CEGUI::Vector2((vecScreenPosition.X - (b_w / 2)), vecScreenPosition.Y), CEGUI::colour(dwColor), m_pFont, false);

				}
			}
		
		// Loop through all active actors
		for(EntityId i = 0; i < MAX_ACTORS; i++)
		{
			// Is the current actor active?
			if(g_pActorManager->DoesExist(i)/* && g_pActorManager->IsNameTagEnabled(i)*/)
			{
				// Get the player position + add z coord
				vecWorldPosition = g_pActorManager->GetPosition(i);
				vecWorldPosition.fZ += 1.0f;
					
				// Convert the position to a screen position
				CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition);

				// Is this player not within our view range?
				if((vecLocalPlayerPosition - vecWorldPosition).Length() > 30.0f)
					continue;

				//if(g_pGraphics)
				//{
				//	CVector3 vecScreen;
				//	g_pGraphics->GetScreenPositionFromWorldPosition(vecWorldPosition, &vecScreen);

				//	if( vecScreen.fZ < 0 )
				//		return;
				//}
				//else
				//	return;

				// Check the cam position with the world position(fix desync)
				if(((vecLookAt - vecWorldPosition).Length() * 1.35) < ((vecLocalPlayerPosition - vecWorldPosition).Length()))
					continue;

				// set the position to the newest
				vecScreenPosition.Y += ( (vecLocalPlayerPosition - vecWorldPosition).Length() * 0.15f); // must be added, otherwise wrong pos

				// set the name
				String strNameTag("%s", g_pActorManager->GetName(i).Get());

				// Get the name tag color
				unsigned int dwColor = ((g_pActorManager->GetNametagColor(i) >> 8) | 0xFF000000);

				// Draw the name tag
				g_pGUI->DrawText(strNameTag, CEGUI::Vector2((vecScreenPosition.X - (b_w / 2)), vecScreenPosition.Y), CEGUI::colour(dwColor), m_pFont, false);

			}
		}
		// Now render the boxes

		// Loop through all active actors
		for(EntityId i = 0; i < MAX_PLAYERS; i++)
		{
			// Is the current actor active?
			if(g_pPlayerManager->DoesExist(i) && g_pLocalPlayer->GetPlayerId() != i)
			{
				CNetworkPlayer * pPlayer = g_pPlayerManager->GetAt(i);

				// Get the player position + add z coord
				pPlayer->GetPosition(vecWorldPosition);
				vecWorldPosition.fZ += 1.15f;
					
				// Convert the position to a screen position
				CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition);

				// Is this player not within our view range?
				if((vecLocalPlayerPosition - vecWorldPosition).Length() > 60.0f)
					continue;

			    //if(g_pGraphics)
				//{
				//	CVector3 vecScreen;
				//	g_pGraphics->GetScreenPositionFromWorldPosition(vecWorldPosition, &vecScreen);
				//		
				//	if( vecScreen.fZ < 0 )
				//		return;
				//}
				//else
				//	return;

				// Check the cam position with the world position(fix desync)
				if(((vecLookAt - vecWorldPosition).Length() * 1.35) < ((vecLocalPlayerPosition - vecWorldPosition).Length()))
					continue;

				// Get the ped health (Subtract 100 as IV health is health + 100)
				float m_fHealth = ( h_b_w * ((float)(pPlayer->GetHealth() - 100.0f) / 100) );
				float m_fArmour = ( h_b_w * ((float)(pPlayer->GetArmour()) / 100) );
				float fHealthWidth = Math::Clamp< float >( 0, (b_i_p + m_fHealth), 100 );
				float fArmourWidth = Math::Clamp< float >( 0, (b_i_p + m_fArmour), 100 );

				// Check health and armour(otherwise if < 0 -> crash)
				if(m_fHealth < 0)
					m_fHealth = 0.0f;

				if(m_fArmour < 0)
					m_fArmour = 0.0f;

				// set the position to the newest
				vecScreenPosition.Y += ( (vecLocalPlayerPosition - vecWorldPosition).Length() * 0.15f); // must be added, otherwise wrong pos

				// Draw boxes
				if(pPlayer->GetArmour() > 2.0)
				{
					// Background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

					// Armour background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 180, 180, 180) );

					// Armour
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), fArmourWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(225, 225, 225, 225) );

					// Background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

					// Health background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

					// Health
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );
				}
				else
				{
					// Background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

					// Health background
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

					// Health
					g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );
					
				}
			}
		}
	}
	if(g_pActorManager)
	{
		if(g_pGame->GetNameTags())
		{
			// Get the local player position
			CVector3 vecLocalPlayerPosition;
			g_pLocalPlayer->GetPosition(vecLocalPlayerPosition);
			
			// Get the lookat data from camera
			CIVCam * pGameCam = g_pCamera->GetGameCam();
			pGameCam->GetPosition(vecCamPosition);
			vecCamForward = pGameCam->GetCam()->m_data1.m_matMatrix.vecForward;
			vecLookAt.fX = vecCamPosition.fX + vecCamForward.fX;
			vecLookAt.fY = vecCamPosition.fY + vecCamForward.fY;
			vecLookAt.fZ = vecCamPosition.fZ + vecCamForward.fZ;

			CVector3 vecWorldPosition;
			Vector2 vecScreenPosition;

			// Loop through all active actors
			for(EntityId i = 0; i < MAX_ACTORS; i++)
			{
				// Is the current actor active?
				if(g_pActorManager->DoesExist(i)/* && g_pActorManager->IsNameTagEnabled(i)*/)
				{
					// Get the player position + add z coord
					vecWorldPosition = g_pActorManager->GetPosition(i);
					vecWorldPosition.fZ += 1.0f;
					
					// Convert the position to a screen position
					CGame::GetScreenPositionFromWorldPosition(vecWorldPosition, vecScreenPosition);

					// Is this player not within our view range?
					if((vecLocalPlayerPosition - vecWorldPosition).Length() > 30.0f)
						continue;

					//if(g_pGraphics)
					//{
					//	CVector3 vecScreen;
					//	g_pGraphics->GetScreenPositionFromWorldPosition(vecWorldPosition, &vecScreen);
					//	
					//	if( vecScreen.fZ < 0 )
					//		return;
					//}
					//else
					//	return;

					// Check the cam position with the world position(fix desync)
					if(((vecLookAt - vecWorldPosition).Length() * 1.35) < ((vecLocalPlayerPosition - vecWorldPosition).Length()))
						continue;

					// Get the ped health (Subtract 100 as IV health is health + 100)
					float m_fHealth = ( h_b_w * ((float)(g_pActorManager->GetHealth(i) - 100.0f) / 100) );
					float m_fArmour = ( h_b_w * ((float)g_pActorManager->GetArmour(i) / 100) );
					float fHealthWidth = Math::Clamp< float >( 0, (b_i_p + m_fHealth), 100 );
					float fArmourWidth = Math::Clamp< float >( 0, (b_i_p + m_fArmour), 100 );

					// Check health and armour(otherwise if < 0 -> crash)
					if(m_fHealth < 0)
						m_fHealth = 0.0f;

					if(m_fArmour < 0)
						m_fArmour = 0.0f;

					// set the position to the newest
					vecScreenPosition.Y += ( (vecLocalPlayerPosition - vecWorldPosition).Length() * 0.15f); // must be added, otherwise wrong pos

					if(g_pActorManager->GetArmour(i) > 2.0)
					{
						// Background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

						// Armour background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 180, 180, 180) );

						// Armour
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), fArmourWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(225, 225, 225, 225) );

						// Background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

						// Health background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

						// Health
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );
					}
					else
					{
						// Background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - (b_w / 2)), (vecScreenPosition.Y + nt_a), b_w, b_h, D3DCOLOR_ARGB(120, 0, 0, 0) );

						// Health background
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), (b_w - (b_i_p * 2)), (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 110, 0, 0) );

						// Health
						g_pGraphics->DrawBox_2( (vecScreenPosition.X - ((b_w / 2) - b_i_p)), (vecScreenPosition.Y + (nt_a + b_i_p)), fHealthWidth, (b_h - (b_i_p * 2)), D3DCOLOR_ARGB(180, 255, 0, 0) );
					
					}
				}
			}
		}
	}
}
