//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGraphics.cpp
// Project: Client.Core
// Author(s): jenksta
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

	OnResetDevice();
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

void CGraphics::DrawBox( float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColour )
{
	GenerateTextures( &m_pTexture, dwColour );

	CVertexList qV[4];

	qV[0].dwColor = qV[1].dwColor = qV[2].dwColor = qV[3].dwColor = dwColour;
	qV[0].z = qV[1].z = qV[2].z = qV[3].z = 0.0f;
	qV[0].rhw = qV[1].rhw = qV[2].rhw = qV[3].rhw = 0.0f;

	qV[0].x = fLeft;
	qV[0].y = (fTop + fHeight);
	qV[1].x = fLeft;
	qV[1].y = fTop;
	qV[2].x = (fLeft + fWidth);
	qV[2].y = (fTop + fHeight);
	qV[3].x = (fLeft + fWidth);
	qV[3].y = fTop;

	const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_pDevice->SetFVF( D3DFVF_TL );
	m_pDevice->SetTexture( 0, m_pTexture );
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, qV, sizeof( CVertexList ) );
}

float CGraphics::GetFontHeight( float fScale )
{
	// Get the font
	CEGUI::Font* pFont = g_pGUI->GetFont( "tahoma" );

	// Is the font valid?
	if( pFont )
	{
		float desc = 0.0f;
		pFont->getFontHeight( desc );
		return ( (float) desc * fScale );
	}

	return 0.0f;
}

void CGraphics::GenerateTextures( IDirect3DTexture9 ** pTexture, DWORD dwColour )
{
	// Create the texture
	m_pDevice->CreateTexture( 8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, pTexture, NULL );

	// Generate the colour
	WORD colour16 = ( (WORD)((dwColour >> 28) & 0xF) << 12) |
		(WORD)(((dwColour >> 20) & 0xF) << 8) |
		(WORD)(((dwColour >> 12) & 0xF) << 4) |
        (WORD)(((dwColour >> 4) & 0xF) << 0);

	D3DLOCKED_RECT lockedRect;
	(*pTexture)->LockRect( 0, &lockedRect, 0, 0 );

	WORD *pDst16 = (WORD*)lockedRect.pBits;
	for(int xy=0; xy < 8*8; xy++)
	*pDst16++ = colour16;
	(*pTexture)->UnlockRect(0);
}
