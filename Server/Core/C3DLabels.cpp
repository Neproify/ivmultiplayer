//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: C3DLabels.cpp
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================


#include "C3DLabels.h"


C3DLabel::C3DLabel()
	: m_fStreamingDistance(300.0f),
	m_dimensionId(0),
	m_labelId(MAX_3D_LABELS /* INVALID ID */)
{
	/* m_pFont = g_pGUI->GetFont("tahoma-bold", 10); */
}

C3DLabel::C3DLabel(LabelId id, const String& text, const CVector3& vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance)
	: 	m_labelId(id),
	m_dimensionId(0),
	m_strText(text),
	m_vecPosition(vecPosition),
	m_dwColor(dwColor),
	m_bVisible(bVisible),
	m_fStreamingDistance(fStreamingDistance)
{
	/* m_pFont = g_pGUI->GetFont("tahoma-bold", 10); */
}


void C3DLabel::SetPosition(const CVector3& vecPosition)
{ 
	m_vecPosition = vecPosition;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(m_vecPosition);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelPosition, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}


void C3DLabel::SetText(const String& strText)
{
	m_strText = strText;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(m_strText);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}


void C3DLabel::SetColor(DWORD dwColor)
{
	m_dwColor = dwColor;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(m_dwColor);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}

void C3DLabel::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(m_bVisible);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelVisible, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}

void C3DLabel::SetStreamingDistance(float fDistance)
{
	m_fStreamingDistance = fDistance;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(m_fStreamingDistance);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelStreamingDistance, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}

void C3DLabel::SetDimension(DimensionId dimensionId)
{
	m_dimensionId = dimensionId;
	CBitStream bsSend;
	bsSend.WriteCompressed(GetId());
	bsSend.Write(dimensionId);
	g_pNetworkManager->RPC(RPC_ScriptingSet3DLabelDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, false);
}


C3DLabel::~C3DLabel()
{
}


/*void C3DLabel::Render()
{
	Vector2 vecScreenPosition;
	if(!CGame::GetScreenPositionFromWorldPosition(m_vecPosition, vecScreenPosition))
		return;

	g_pGUI->DrawText(m_strText, CEGUI::Vector2(vecScreenPosition.X, vecScreenPosition.Y), CEGUI::colour(m_dwColor), m_pFont, false);
}*/


//=================================================================
//======================== C3DLabelManager ========================
//=================================================================

C3DLabelManager::C3DLabelManager()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++) 
	{
		m_Labels[i] = NULL;
	}
}


C3DLabelManager::~C3DLabelManager()
{
	Reset();
}


LabelId C3DLabelManager::Add(const String& text, const CVector3& vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance)
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++) 
	{
		if(!DoesExist(i)) {
			CBitStream bsSend;

			m_Labels[i] = new C3DLabel(i, text, vecPosition, dwColor, bVisible, fStreamingDistance);
			m_Labels[i]->SetText(text);
			m_Labels[i]->SetPosition(vecPosition);
			m_Labels[i]->SetVisible(true);

			bsSend.WriteCompressed(i);
			bsSend.Write(m_Labels[i]->GetText());
			bsSend.Write(m_Labels[i]->GetPosition());
			bsSend.Write(m_Labels[i]->GetColor());
			bsSend.Write(m_Labels[i]->GetStreamingDistance());
			bsSend.Write(m_Labels[i]->GetDimension());
			bsSend.WriteBit(m_Labels[i]->IsVisible());

			g_pNetworkManager->RPC(RPC_New3DLabel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return i;
		}
	}
	return MAX_3D_LABELS; /* INVALID ID */
}


bool C3DLabelManager::DoesExist(LabelId labelId)
{
	if(labelId < 0 || labelId >= MAX_3D_LABELS)
		return false;

	return (m_Labels[labelId] != NULL);
}


void C3DLabelManager::HandleClientJoin(EntityId playerId)
{
	CBitStream bsSend;
	for(LabelId i = 0; i < MAX_3D_LABELS; i++)
	{
		if(DoesExist(i)) {
			
			bsSend.WriteCompressed(i);
			bsSend.Write(m_Labels[i]->GetText());
			bsSend.Write(m_Labels[i]->GetPosition());
			bsSend.Write(m_Labels[i]->GetColor());
			bsSend.Write(m_Labels[i]->GetStreamingDistance());
			bsSend.Write(m_Labels[i]->GetDimension());
			bsSend.WriteBit(m_Labels[i]->IsVisible());

			g_pNetworkManager->RPC(RPC_New3DLabel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

			bsSend.Reset();
		}
	}
}

void C3DLabelManager::Reset()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++)
	{
		if(DoesExist(i)) {
			SAFE_DELETE(m_Labels[i]);
		}
	}
}


void C3DLabelManager::Remove(LabelId id)
{
	if(DoesExist(id)) {
		CBitStream bsSend;
		bsSend.Write(id);
		g_pNetworkManager->RPC(RPC_Delete3DLabel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		SAFE_DELETE(m_Labels[id]);
	}
}


/*void C3DLabelManager::Render()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++)
	{
		if(m_Labels[i]) {
			if(m_Labels[i]->IsVisible() && m_Labels[i]->IsStreamedIn()) {
				m_Labels[i]->Render();
			}
		}
	}
}*/


C3DLabel* C3DLabelManager::GetAt(LabelId id)
{
	if(DoesExist(id))
		return m_Labels[id];
	else
		return NULL;
}