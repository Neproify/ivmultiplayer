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

#include "CGUI.h"
#include "CMainMenu.h"
#include <CLogFile.h>

extern CGUI                 * g_pGUI;
extern CMainMenu            * g_pMainMenu;

class CD3D9WebView
{
public:
	CD3D9WebView(int width, int height, EA::WebKit::View * view)
	{
		this->device = device;
		this->view = view;
		this->width = width;
		this->height = height;
		posX = 0;
		posY = 0;
		device = g_pGUI->GetDirect3DDevice();
		D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
		m_bBeforeGUI = false;

		name = g_pGUI->GetUniqueName();

		view->GetSurface()->SetPixelFormat(EA::Raster::kPixelFormatTypeARGB);

		CEGUI::Texture & ceguiTexture = g_pGUI->GetRenderer()->createTexture(texture);
		CEGUI::ImagesetManager::getSingleton().create(name.Get(), ceguiTexture);
		CEGUI::ImagesetManager::getSingleton().get(name.Get()).defineImage("full_image", CEGUI::Rect(0, 0, width, height), CEGUI::Point(0, 0));

		image = g_pGUI->CreateGUIStaticImage(CEGUI::String(name.Get()));

		image->setProperty("FrameEnabled", "false");
		image->setProperty("BackgroundEnabled", "false");
		image->setProperty("Image", String("set:%s image:full_image", name.Get()).Get());
		image->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));
		image->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 100), CEGUI::UDim(0, 100)));
		image->setVisible(true);
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

		image->setPosition(CEGUI::UVector2(CEGUI::UDim(0, y), CEGUI::UDim(0, y)));
	}
	void SetData(void * buffer)
	{
		RECT rect = {0, 0, width, height};

		D3DLOCKED_RECT lockedRect;
		texture->LockRect(0, &lockedRect, NULL, 0);
		void* destBuffer = lockedRect.pBits;
		memcpy(destBuffer, this->view->GetSurface()->GetData(), width*height*4);
		texture->UnlockRect(0);
		CEGUI::ImagesetManager::getSingleton().get(name.Get()).getTexture()->loadFromMemory(buffer, CEGUI::Size(width, height), CEGUI::Texture::PF_RGBA);
		//imageset.setTexture(&ceguiTexture);
	}
	void Render()
	{
		
	}
	void Release()
	{
		texture->Release();
	}
	EA::WebKit::View * GetView()
	{
		return view;
	}
	IDirect3DTexture9 * GetTexture()
	{
		return texture;
	}
private:
	LPDIRECT3DDEVICE9 device;
	IDirect3DTexture9 * texture;
	CGUIStaticImage * image;
	String name;

	EA::WebKit::View * view;

	int width;
	int height;
	int posX;
	int posY;

	bool m_bBeforeGUI;
};


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
		for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); it++)
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
	void RenderAll(bool bSetData = true, bool bTick = true, bool bRender = true)
	{
		for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); it++)
		{
			if(bTick)
			{
				(*it)->GetView()->Tick();
			}
			if(bSetData)
			{
				(*it)->SetData((*it)->GetView()->GetSurface()->GetData());
			}
			if(bRender)
			{
				(*it)->Render();
			}
		}
	}
	EA::WebKit::IEAWebkit * GetWebKit()
	{
		return webkit;
	}
	CD3D9WebView * CreateView(int width, int height, const char * url, LPDIRECT3DDEVICE9 device)
	{
		EA::WebKit::View * view = webkit->CreateView();
		EA::WebKit::ViewParameters params = view->GetParameters();
		params.mWidth = width;
		params.mHeight = height;
		view->InitView(params);
		view->SetSize(width, height);

		view->SetURI(url);

		CD3D9WebView * webView = new CD3D9WebView(width, height, view);

		views.push_back(webView);

		return webView;
	}
	void DestroyView(CD3D9WebView * webView)
	{
		webView->GetView()->CancelLoad();
		webView->GetView()->ShutdownView();
		webkit->DestroyView(webView->GetView());
		views.remove(webView);
	}
private:
	EA::WebKit::IEAWebkit * webkit;
	EA::WebKit::IGlyphCache * glyph_cache;
	EA::WebKit::IFontServer * font_server;
	EA::WebKit::IFontStyle * font_style;
	EA::Raster::IEARaster * raster;

	CLibrary * m_pLibrary;
	
	std::list<CD3D9WebView*> views;
};
