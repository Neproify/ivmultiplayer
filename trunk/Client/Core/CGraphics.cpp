//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGraphics.cpp
// Project: Client.Core
// Author(s): adamix
//            jenskta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CGraphics.h"
#include "CChatWindow.h"
#include <CLogFile.h>
#include "CGame.h"
#include "SharedUtility.h"
#include "CGUI.h"

extern CGUI * g_pGUI;

struct D3DVERTEX
{
	float fX;
	float fY;
	float fZ;
	float fRHW;
	DWORD dwColor;

	D3DVERTEX()
	{
		fX = 0.0f;
		fY = 0.0f;
		fZ = 0.0f;
		fRHW = 1.0f;
		dwColor = 0;
	}

	D3DVERTEX(float _fX, float _fY, float _fZ, float _fRHW, DWORD _dwColor)
	{
		fX = _fX;
		fY = _fY;
		fZ = _fZ;
		fRHW = _fRHW;
		dwColor = _dwColor;
	}
};

CGraphics::CGraphics(IDirect3DDevice9 * pDevice)
{
	m_pDevice = pDevice;
	m_pStateBlock = NULL;
}

CGraphics::~CGraphics()
{

}

void CGraphics::OnLostDevice()
{
	// If we have a state block release it
	if(m_pStateBlock)
	{
		m_pStateBlock->Release();
		m_pStateBlock = NULL;
	}
}

void CGraphics::OnResetDevice()
{
	// If we don't have a state block create one
	if(!m_pStateBlock)
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock);
}

void CGraphics::Begin()
{
	// If we have a state block capture the current device state
	if(m_pStateBlock)
		m_pStateBlock->Capture();
}

void CGraphics::End()
{
	// If we have a state block apply the captured device state
	if(m_pStateBlock)
		m_pStateBlock->Apply();
}

void CGraphics::DrawPixel(float fX, float fY, unsigned long ulColor)
{
	D3DVERTEX vertex(fX, fY, 0.0f, 1.0f, ulColor);

	Begin();
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &vertex, sizeof(D3DVERTEX));
	End();
}

void CGraphics::DrawLine(float fStartX, float fStartY, float fEndX, float fEndY, unsigned long ulColor)
{
	D3DVERTEX vertex[2];
	vertex[0] = D3DVERTEX(fStartX, fStartY, 0.0f, 1.0f, ulColor);
	vertex[1] = D3DVERTEX(fEndX, fEndY, 0.0f, 1.0f, ulColor);

	Begin();
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vertex[0], sizeof(D3DVERTEX));
	End();
}

void CGraphics::DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor)
{
	// From Irrlicht
	D3DVERTEX vertex[4];
	vertex[0] = D3DVERTEX(fX, fY, 0.0f, 1.0f, ulColor);
	vertex[1] = D3DVERTEX((fX + fWidth), fY, 0.0f, 1.0f, ulColor);
	vertex[2] = D3DVERTEX((fX + fWidth), (fY + fHeight), 0.0f, 1.0f, ulColor);
	vertex[3] = D3DVERTEX(fX, (fY + fHeight), 0.0f, 1.0f, ulColor);
	short indices[6] = {0,1,2,0,2,3};

	Begin();
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0], D3DFMT_INDEX16, &vertex[0], sizeof(D3DVERTEX));
	End();
}
