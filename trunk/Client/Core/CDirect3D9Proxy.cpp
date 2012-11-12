//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDirect3D9Proxy.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <Common.h>
#include <windowsx.h>
#include "CDirect3D9Proxy.h"
#include <CLogFile.h>
#include "Input.h"

extern WNDPROC m_wWndProc;
extern bool               g_bWindowedMode;

CDirect3D9Proxy::CDirect3D9Proxy(IDirect3D9 * pD3D)
{
	m_pD3D = pD3D;
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::QueryInterface(REFIID riid, void ** ppvObj)
{
	return m_pD3D->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE CDirect3D9Proxy::AddRef()
{
	return m_pD3D->AddRef();
}

ULONG STDMETHODCALLTYPE CDirect3D9Proxy::Release()
{
	ULONG uRet = m_pD3D->Release();

	if(uRet == 0)
	{
		// If the reference count is 0 delete ourselves
		delete this;
	}

	return uRet;
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::RegisterSoftwareDevice(void * pInitializeFunction)
{
	return m_pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

UINT STDMETHODCALLTYPE CDirect3D9Proxy::GetAdapterCount()
{
	return m_pD3D->GetAdapterCount();
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier)
{
	return m_pD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT STDMETHODCALLTYPE CDirect3D9Proxy::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pD3D->GetAdapterModeCount(Adapter, Format);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
	return m_pD3D->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pD3D->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pD3D->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD * pQualityLevels)
{
	return m_pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pD3D->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
	return m_pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR STDMETHODCALLTYPE CDirect3D9Proxy::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3D->GetAdapterMonitor(Adapter);
}

HRESULT STDMETHODCALLTYPE CDirect3D9Proxy::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DDevice9 ** ppReturnedDeviceInterface)
{
	// Set the window text
	SetWindowText(hFocusWindow, MOD_NAME DEBUG_IDENTIFIER);

	// Disable minimize & maximize box
	SetWindowLong(hFocusWindow, GWL_STYLE, GetWindowLong(hFocusWindow, GWL_STYLE) & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);

	// Set windowed mode
	if(g_bWindowedMode)
	{
		pPresentationParameters->Windowed = 1;
		pPresentationParameters->Flags = 0;
		pPresentationParameters->FullScreen_RefreshRateInHz = 0;
		pPresentationParameters->PresentationInterval = 0;
		LONG_PTR style = GetWindowLongPtr(pPresentationParameters->hDeviceWindow, GWL_STYLE);
		SetWindowLongPtr(pPresentationParameters->hDeviceWindow, GWL_STYLE, style | WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME);
		SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, SWP_SHOWWINDOW);
	}

	// Create the d3d device
	HRESULT hr = m_pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	// Subclass the games window
	// TODO: Move this and the code from Input.cpp to a new
	// CWindowSubclass.cpp/h
	if(!m_wWndProc)
	{
		CLogFile::Printf("SubclassWindow");
		m_wWndProc = SubclassWindow(hFocusWindow, WndProc_Hook);
	}

	// Create the proxy device
	if(SUCCEEDED(hr))
		*ppReturnedDeviceInterface = new CDirect3DDevice9Proxy(this, *ppReturnedDeviceInterface);

	return hr;
}
