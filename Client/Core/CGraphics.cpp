//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGraphics.cpp
// Project: Client.Core
// Author(s): adamix
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

const unsigned char g_szPixel [] = { 0x42, 0x4D, 0x3A, 0, 0, 0, 0, 0, 0, 0, 0x36, 0, 0, 0, 0x28, 0, 0,
                                    0, 0x1, 0, 0, 0, 0x1, 0, 0, 0, 0x1, 0, 0x18, 0, 0, 0, 0, 0,
                                    0x4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0xFF, 0xFF, 0xFF, 0 };

CGraphics::CGraphics(IDirect3DDevice9 * pDevice)
{
	m_pDevice = pDevice;
	D3DXCreateTextureFromFileInMemory(pDevice, g_szPixel, sizeof (g_szPixel), &m_pDXPixelTexture);
	D3DXCreateSprite(pDevice, &m_pDXSprite);
}

CGraphics::~CGraphics()
{

}

void CGraphics::OnLostDevice()
{
	// If the sprite is valid inform it of the device loss
	if(m_pDXSprite)
		m_pDXSprite->OnLostDevice();
}

void CGraphics::OnResetDevice()
{
	// If the sprite is valid inform it of the device reset
	if(m_pDXSprite)
		m_pDXSprite->OnResetDevice();
}

void CGraphics::DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor)
{
    m_pDXSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    D3DXMATRIX matrix;
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &D3DXVECTOR2(fWidth, fHeight), NULL, 0.0f, &D3DXVECTOR2(fX, fY));
    m_pDXSprite->SetTransform(&matrix);
    m_pDXSprite->Draw(m_pDXPixelTexture, NULL, NULL, NULL, ulColor);
    m_pDXSprite->End();
}
