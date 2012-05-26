//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CD3D9Webkit.hpp
// Project: Client.Core
// Author(s): adamix
// License: See LICENSE in root directory
//
//==============================================================================

#define _CHAR16T
#include <d3d9.h>
#include <d3dx9.h>
#include <string.h>
#include <stdio.h>

#include <EAWebkit/EAWebkit.h>
#include <EAWebkit/EAWebkitAllocator.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebkit/EAWebKitViewNotification.h>

#include <list>

#include <CLibrary.h>
#include <SharedUtility.h>


class CD3D9WebKit
{
public:
	CD3D9WebKit()
	{
		typedef EA::WebKit::IEAWebkit* (* PF_CreateEAWebkitInstance)(void);
		PF_CreateEAWebkitInstance create_webkit_instance = NULL;
		String strLibraryPath(SharedUtility::GetAbsolutePath("EAWebkit.dll"));
		m_pLibrary = new CLibrary();
		if(m_pLibrary->Load(strLibraryPath.Get()))
		{
			create_webkit_instance = reinterpret_cast<PF_CreateEAWebkitInstance>(m_pLibrary->GetProcedureAddress("CreateEAWebkitInstance"));
		}
		if(!create_webkit_instance)
			return;

		webkit = create_webkit_instance();
		webkit->Init(NULL);
		glyph_cache = webkit->CreateGlyphCacheWrapperInterface(NULL);
		font_server = webkit->CreateFontServerWrapperInterface(NULL);
		
		uint32_t nCount = font_server->AddDirectory(NULL, L"cour.ttf");

		font_style = font_server->CreateTextStyle();
		font_style->SetSize(12.0f);
		font_style->SetSmooth(EA::WebKit::Smooth::kSmoothEnabled);

		EA::WebKit::Parameters& param = webkit->GetParameters();
		param.mpLocale = "ru_RU";
		param.mEnableSmoothText = false;

		sprintf_s(param.mSystemFontDescription.mFamilies, sizeof(param.mSystemFontDescription.mFamilies) / sizeof(param.mSystemFontDescription.mFamilies[0]),\
				"Arial,Microsoft Yahei,Courier,sans-serif");
		sprintf_s(param.mFontFamilyStandard, sizeof(param.mFontFamilyStandard) / sizeof(param.mFontFamilyStandard[0]), "Courier");
		sprintf_s(param.mFontFamilySerif, sizeof(param.mFontFamilySerif) / sizeof(param.mFontFamilySerif[0]), "Courier");
		sprintf_s(param.mFontFamilySansSerif, sizeof(param.mFontFamilySansSerif) / sizeof(param.mFontFamilySansSerif[0]), "Courier");
		sprintf_s(param.mFontFamilyMonospace, sizeof(param.mFontFamilyMonospace) / sizeof(param.mFontFamilyMonospace[0]), "Courier");
		sprintf_s(param.mFontFamilyCursive, sizeof(param.mFontFamilyCursive) / sizeof(param.mFontFamilyCursive[0]), "Courier");
		sprintf_s(param.mFontFamilyFantasy, sizeof(param.mFontFamilyFantasy) / sizeof(param.mFontFamilyFantasy[0]), "Courier");

		webkit->SetParameters(param);

		raster = webkit->GetEARasterInstance();

	}
	~CD3D9WebKit()
	{
		for(std::list<EA::WebKit::View*>::iterator it = views.begin(); it != views.end(); it++)
		{
			DestroyView((*it));
		}
		webkit->DestroyFontServerWrapperInterface(font_server);
		webkit->DestroyGlyphCacheWrapperInterface(glyph_cache);
		webkit->Shutdown();
		webkit->Destroy();

		if(font_style)
			font_style->Destroy();
	}
	EA::WebKit::IEAWebkit * GetWebKit()
	{
		return webkit;
	}
	EA::WebKit::View * CreateView(int width, int height, const char * url)
	{
		EA::WebKit::View * view = webkit->CreateView();
		EA::WebKit::ViewParameters params = view->GetParameters();
		params.mWidth = width;
		params.mHeight = height;
		view->InitView(params);
		view->SetSize(width, height);

		view->SetURI(url);

		views.push_back(view);

		return view;
	}
	void DestroyView(EA::WebKit::View * view)
	{
		view->CancelLoad();
		view->ShutdownView();
		webkit->DestroyView(view);
		views.remove(view);
	}
private:
	EA::WebKit::IEAWebkit * webkit;
	EA::WebKit::IGlyphCache * glyph_cache;
	EA::WebKit::IFontServer * font_server;
	EA::WebKit::IFontStyle * font_style;
	EA::Raster::IEARaster * raster;

	CLibrary * m_pLibrary;
	
	std::list<EA::WebKit::View*> views;
};

class CD3D9WebView
{
public:
	CD3D9WebView(LPDIRECT3DDEVICE9 device, int width, int height, EA::WebKit::View * view)
	{
		this->device = device;
		this->view = view;
		this->width = width;
		this->height = height;
		posX = 0;
		posY = 0;
		D3DXCreateSprite(device, &sprite);
		D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
	}
	void SetSize(int width, int height)
	{
		this->width = width;
		this->height = height;
		texture->Release();
		D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
		this->view->SetSize(width, height);
	}
	void SetPosition(int x, int y)
	{
		posX = x;
		posY = y;
	}
	void SetData(void * buffer)
	{
		RECT rect = {0, 0, width, height};

		D3DLOCKED_RECT lockedRect;
		texture->LockRect(0, &lockedRect, NULL, 0);
		void* destBuffer = lockedRect.pBits;
		memcpy(destBuffer, this->view->GetSurface()->GetData(), width*height*4);
		texture->UnlockRect(0);
	}
	void Render()
	{
		RECT rect = {0, 0, width, height};

		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(texture, &rect, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(posX, posY, 0.0f), 0xFFFFFFFF);
		sprite->End();
	}
	void Release()
	{
		texture->Release();
		sprite->Release();
	}
	EA::WebKit::View * GetView()
	{
		return view;
	}
private:
	LPDIRECT3DDEVICE9 device;
	ID3DXSprite * sprite;
	IDirect3DTexture9 * texture;

	EA::WebKit::View * view;

	int width;
	int height;
	int posX;
	int posY;
};
