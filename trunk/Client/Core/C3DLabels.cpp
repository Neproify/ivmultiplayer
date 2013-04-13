//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: C3DLabels.cpp
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "C3DLabels.h"
#include "CClient.h"

extern CClient * g_pClient;

C3DLabel::C3DLabel()
	: CStreamableEntity(STREAM_ENTITY_LABEL, 300.0f),
	m_labelId(MAX_3D_LABELS /* INVALID ID */)
{
	SetDimension(0);
	SetCanBeStreamedIn(true);
}

C3DLabel::C3DLabel(LabelId id, String text, CVector3 vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance)
	: CStreamableEntity(STREAM_ENTITY_LABEL, fStreamingDistance),
	m_strText(text),
	m_vecPosition(vecPosition),
	m_dwColor(dwColor),
	m_bVisible(bVisible),
	m_labelId(id)
{
	
	SetDimension(0);
	SetCanBeStreamedIn(true);
}

C3DLabel::~C3DLabel()
{
	OnDelete();
}

void C3DLabel::Render()
{
	Vector2 vecScreenPosition;

	if(!CGame::GetScreenPositionFromWorldPosition(m_vecPosition, vecScreenPosition))
		return;

	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	if(pGUI)
	{
		CEGUI::Font * pFont = g_pClient->GetGUI()->GetFont("tahoma-bold");

		if(pFont) 
		{
			pGUI->DrawText(m_strText.Get(), CEGUI::Vector2(vecScreenPosition.fX - (pFont->getTextExtent(m_strText.Get()) / 2), vecScreenPosition.fY), CEGUI::colour(m_dwColor), pFont, false);
		}
	}
}

//=================================================================
//======================== C3DLabelManager ========================
//=================================================================

C3DLabelManager::C3DLabelManager()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++) 
		m_Labels[i] = NULL;
}

C3DLabelManager::~C3DLabelManager()
{
	Reset();
}

LabelId C3DLabelManager::Add(String text, CVector3 vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance)
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++) 
	{
		if(!DoesExist(i))
		{
			m_Labels[i] = new C3DLabel(i, text, vecPosition, dwColor, bVisible, fStreamingDistance);
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


void C3DLabelManager::Reset()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++)
		SAFE_DELETE(m_Labels[i]);
}

void C3DLabelManager::Remove(LabelId id)
{
	if(DoesExist(id))
		SAFE_DELETE(m_Labels[id]);
}

void C3DLabelManager::Render()
{
	for(LabelId i = 0; i < MAX_3D_LABELS; i++)
	{
		if(DoesExist(i))
		{
			if(m_Labels[i]->IsVisible() && m_Labels[i]->IsStreamedIn())
				m_Labels[i]->Render();
		}
	}
}

C3DLabel * C3DLabelManager::GetAt(LabelId id)
{
	if(id < MAX_3D_LABELS)
	{
		if(DoesExist(id))
			return m_Labels[id];
		else
			return NULL;
	}
	return NULL;
}
