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
	IDirect3DDevice9     * m_pDevice;
	IDirect3DStateBlock9 * m_pStateBlock;

public:
	CGraphics(IDirect3DDevice9 * pDevice);
	~CGraphics();

	void OnLostDevice();
	void OnResetDevice();
	void Begin();
	void End();
	void DrawPixel(float fX, float fY, unsigned long ulColor);
	void DrawLine(float fStartX, float fStartY, float fEndX, float fEndY, unsigned long ulColor);
	void DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor);
};
