//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: C3DLabels.h
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Common.h>
#include "CCamera.h"
#include "CGUI.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CStreamer.h"

class C3DLabel : public CStreamableEntity
{
private:
	String		m_strText;
	CVector3	m_vecPosition;
	DWORD		m_dwColor;
	LabelId		m_labelId;
	bool		m_bVisible;
	bool		streamed;

public:
	C3DLabel();
	C3DLabel(LabelId id, String text, CVector3 vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance);
	~C3DLabel();

	void		Render();

	void		SetPosition(const CVector3& vecPosition) { m_vecPosition = vecPosition; }
	void		GetPosition(CVector3& vecPosition) { vecPosition = m_vecPosition; }
	CVector3	GetPosition() { return m_vecPosition; }

	void		SetText(const String& strText) { m_strText = strText; }
	void		GetText(String& strText) { strText = m_strText; }
	String		GetText() { return m_strText; }

	void		SetColor(const DWORD& dwColor) { m_dwColor = dwColor; }
	void		GetColor(DWORD& dwColor) { dwColor = m_dwColor; }
	DWORD		GetColor() { return m_dwColor; }

	void		SetVisible(bool bVisible) { m_bVisible = bVisible; }
	bool		IsVisible() { return m_bVisible; }

	void StreamIn() { streamed = true; }
	void StreamOut() { streamed = false; }

	/*void SetStreamingDistance(float fDistance) { m_fStreamingDistance = fDistance; }*/
	void GetStreamPosition(CVector3& vecPosition) { vecPosition = m_vecPosition; }

	LabelId GetId() { return m_labelId; }
};

class C3DLabelManager
{
private:
	C3DLabel* m_Labels[MAX_3D_LABELS];

public:
	C3DLabelManager();
	~C3DLabelManager();

	LabelId		Add(String text, CVector3 vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance);
	bool		DoesExist(LabelId id);

	void		Remove(LabelId id);
	void		Reset();
	void		Render();

	/*void		HandleClientJoin(EntityId playerId);*/

	C3DLabel*	GetAt(LabelId id);
};
