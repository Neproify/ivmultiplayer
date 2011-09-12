//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGraphics.h
// Project: Client.Core
// Author(s): adamix
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class CGraphics 
{
private:
	IDirect3DDevice9  * m_pDevice;
	ID3DXSprite       * m_pDXSprite;
	IDirect3DTexture9 * m_pDXPixelTexture;

	static unsigned int m_uiWidth;
	static unsigned int m_uiHeight;

public:
	CGraphics(IDirect3DDevice9 * pDevice);
	~CGraphics();

	void                OnLostDevice();
	void                OnResetDevice();
	void                DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor);
};
