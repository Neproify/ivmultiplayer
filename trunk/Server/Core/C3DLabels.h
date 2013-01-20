//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: C3DLabels.h
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "./Main.h"
#include "CNetworkManager.h"
#include <CLogFile.h>
#include "CEvents.h"

extern CNetworkManager * g_pNetworkManager;
#include <Common.h>

typedef unsigned long LabelId;
#define MAX_3D_LABELS 512 //0xFFFFFF

// TODO: create interface
// TODO: move C3DLabel class to another file
class C3DLabel
{
private:
	String		m_strText;
	CVector3	m_vecPosition;
	DWORD		m_dwColor;
	LabelId		m_labelId;
	bool		m_bVisible;
	float		m_fStreamingDistance;
	DimensionId m_dimensionId;

public:
	C3DLabel();
	C3DLabel(LabelId id, const String& text, const CVector3& vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance);
	~C3DLabel();

	/*void		Render();*/

	void		SetPosition(const CVector3& vecPosition);
	void		GetPosition(CVector3& vecPosition) { vecPosition = m_vecPosition; }
	CVector3	GetPosition() { return m_vecPosition; }

	void		SetText(const String& strText);
	void		GetText(String& strText) { strText = m_strText; }
	String		GetText() { return m_strText; }

	void		SetColor(DWORD dwColor);
	void		GetColor(DWORD& dwColor) { dwColor = m_dwColor; }
	DWORD		GetColor() { return m_dwColor; }

	void		SetVisible(bool bVisible);
	bool		IsVisible() { return m_bVisible; }

	void		SetStreamingDistance(float fDistance);
	float		GetStreamingDistance() { return m_fStreamingDistance; }

	void		SetDimension(DimensionId dimensionId);
	DimensionId GetDimension() { return m_dimensionId; }

	LabelId		GetId() { return m_labelId; }
};


class C3DLabelManager
{
private:
	bool	  m_bActive[MAX_3D_LABELS];
	C3DLabel* m_Labels[MAX_3D_LABELS];

public:
	C3DLabelManager();
	~C3DLabelManager();


	LabelId		Add(const String& text, const CVector3& vecPosition, DWORD dwColor, bool bVisible, float fStreamingDistance);
	bool		DoesExist(LabelId id);

	void		Remove(LabelId id);
	void		Reset();
	/*void		Render();*/

	void		HandleClientJoin(EntityId playerId);

	C3DLabel*	GetAt(LabelId id);
};
