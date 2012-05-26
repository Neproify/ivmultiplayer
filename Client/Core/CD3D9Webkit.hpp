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

bool OnMouseMove(const CEGUI::EventArgs &eventArgs);
bool OnMouseWheel(const CEGUI::EventArgs &eventArgs);
bool OnMouseUp(const CEGUI::EventArgs &eventArgs);
bool OnMouseDown(const CEGUI::EventArgs &eventArgs);
bool OnKeyUp(const CEGUI::EventArgs &eventArgs);
bool OnKeyDown(const CEGUI::EventArgs &eventArgs);
bool OnCharKey(const CEGUI::EventArgs &eventArgs);

class CD3D9WebkitNotifications : public EA::WebKit::ViewNotification
{
public:

};

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

		//view->GetSurface()->SetPixelFormat(EA::Raster::kPixelFormatTypeRGBA);

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

		g_pGUI->SetCursorVisible(true);

		image->subscribeEvent(CEGUI::PushButton::EventMouseMove, CEGUI::Event::Subscriber(&OnMouseMove));
		image->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::Event::Subscriber(&OnMouseDown));
		image->subscribeEvent(CEGUI::PushButton::EventMouseWheel, CEGUI::Event::Subscriber(&OnMouseWheel));
		image->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&OnMouseUp));
		image->subscribeEvent(CEGUI::PushButton::EventCharacterKey, CEGUI::Event::Subscriber(&OnCharKey));
		image->subscribeEvent(CEGUI::PushButton::EventKeyUp, CEGUI::Event::Subscriber(&OnKeyUp));
		image->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&OnKeyDown));


		image->setAlwaysOnTop(true);
		
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
		//view->GetSurface()->SetPixelFormat(EA::Raster::kPixelFormatTypeRGBA);
		memcpy(destBuffer, this->view->GetSurface()->GetData(), width*height*4);
		
		unsigned int* data = (unsigned int*)destBuffer;
		for ( int idx = 0; idx < width*height; ++idx ) { 
			data[idx] = (data[idx] << 8) | (data[idx] >> 24);
		}
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
	CEGUI::Window * GetWindow()
	{
		return image;
	}
	String GetName()
	{
		return name;
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
		glyph_cache = webkit->GetGlyphCache();
		font_server = webkit->GetFontServer();
		
		wchar_t szWindowsDir[MAX_PATH] = {0};
		GetWindowsDirectoryW(szWindowsDir, MAX_PATH);
		wchar_t szFontDir[MAX_PATH] = {0};
		swprintf_s(szFontDir, MAX_PATH - 1, L"%s\\Fonts", szWindowsDir);
		font_server->AddDirectory(szFontDir);


		font_style = font_server->CreateTextStyle();
		font_style->SetSize(12.0f);
		font_style->SetSmooth(EA::WebKit::Smooth::kSmoothEnabled);

		EA::WebKit::Parameters& param = webkit->GetParameters();
		param.mpLocale = "zh-cn";
		param.mEnableSmoothText = false;
		//param.mPluginsEnabled = true;
		//param.mJavaScriptEnabled = true;

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

		tickCount = GetTickCount();
	}
	~CD3D9WebKit()
	{
		for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); it++)
		{
			DestroyView((*it));
		}
//		webkit->DestroyFontServerWrapperInterface(font_server);
//		webkit->DestroyGlyphCacheWrapperInterface(glyph_cache);
		webkit->Shutdown();
		webkit->Destroy();

		if(font_style)
			font_style->Destroy();
	}
	void RenderAll(bool bSetData = true, bool bTick = true, bool bRender = true)
	{
		for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); it++)
		{
			bool b = false;
			if(bTick && ((GetTickCount() - tickCount) > 75))
			{
				tickCount = GetTickCount();
				b = (*it)->GetView()->Tick();
			}
			if(bSetData && b)
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
	CD3D9WebView * GetView(CEGUI::Window * window)
	{
		std::list<CD3D9WebView*>::const_iterator it;
		for(it = views.begin(); it != views.end(); it++)
		{
			if((*it)->GetWindow() == window)
				return (*it);
		}
	}
	CD3D9WebView * GetView(EA::WebKit::View * view)
	{
		std::list<CD3D9WebView*>::const_iterator it;
		for(it = views.begin(); it != views.end(); it++)
		{
			if((*it)->GetView() == view)
				return (*it);
		}
	}
	CD3D9WebView * GetView(String name)
	{
		std::list<CD3D9WebView*>::const_iterator it;
		for(it = views.begin(); it != views.end(); it++)
		{
			if((*it)->GetName() == name)
				return (*it);
		}
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

	unsigned long tickCount;
	
	std::list<CD3D9WebView*> views;
};

extern CD3D9WebKit * g_pWebkit;

bool OnMouseMove(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::MouseEventArgs mouseEventArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	if(!mouseEventArgs.window->isActive())
		return false;
	CEGUI::Vector2 localMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CEGUI::Vector2 deltaMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CD3D9WebView * view = g_pWebkit->GetView(mouseEventArgs.window);
	EA::WebKit::MouseMoveEvent moveEvent = {0};
	moveEvent.mX = localMousePos.d_x;
	moveEvent.mY = localMousePos.d_y;
	moveEvent.mDX = deltaMousePos.d_x;
	moveEvent.mDY = deltaMousePos.d_y;
	view->GetView()->OnMouseMoveEvent(moveEvent);
	return true;
}

bool OnMouseUp(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::MouseEventArgs mouseEventArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	if(!mouseEventArgs.window->isActive())
		return false;
	CEGUI::Vector2 localMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CLogFile::Printf("Mouse click: %f %f", localMousePos.d_x, localMousePos.d_y);
	CD3D9WebView * view = g_pWebkit->GetView(mouseEventArgs.window);
	EA::WebKit::MouseButtonEvent buttonEvent = {0};
	buttonEvent.mX = localMousePos.d_x;
	buttonEvent.mY = localMousePos.d_y;
	buttonEvent.mbDepressed = false;
	buttonEvent.mId = EA::WebKit::kMouseLeft;
	view->GetView()->OnMouseButtonEvent(buttonEvent);
	return true;
}

bool OnMouseDown(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::MouseEventArgs mouseEventArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	if(!mouseEventArgs.window->isActive())
		return false;
	CEGUI::Vector2 localMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CLogFile::Printf("Mouse click: %f %f", localMousePos.d_x, localMousePos.d_y);
	CD3D9WebView * view = g_pWebkit->GetView(mouseEventArgs.window);
	EA::WebKit::MouseButtonEvent buttonEvent = {0};
	buttonEvent.mX = localMousePos.d_x;
	buttonEvent.mY = localMousePos.d_y;
	buttonEvent.mbDepressed = true;
	buttonEvent.mId = EA::WebKit::kMouseLeft;
	view->GetView()->OnMouseButtonEvent(buttonEvent);
	return true;
}

bool OnMouseWheel(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::MouseEventArgs mouseEventArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	if(!mouseEventArgs.window->isActive())
		return false;
	CEGUI::Vector2 localMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CEGUI::Vector2 deltaMousePos = CEGUI::CoordConverter::screenToWindow(*mouseEventArgs.window, CEGUI::MouseCursor::getSingleton().getPosition());
	CD3D9WebView * view = g_pWebkit->GetView(mouseEventArgs.window);
	EA::WebKit::MouseWheelEvent wheelEvent = {0};
	wheelEvent.mX = localMousePos.d_x;
	wheelEvent.mY = localMousePos.d_y;
	wheelEvent.mZDelta = mouseEventArgs.wheelChange;
	view->GetView()->OnMouseWheelEvent(wheelEvent);
	return true;
}

bool OnKeyUp(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs keyEventArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	if(!keyEventArgs.window->isActive())
		return false;
	CD3D9WebView * view = g_pWebkit->GetView(keyEventArgs.window);
	EA::WebKit::KeyboardEvent e = {0};
	e.mId = keyEventArgs.scancode;
	e.mbChar = false;
	e.mbDepressed = false;
	view->GetView()->OnKeyboardEvent(e);
	return true;
}
bool OnKeyDown(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs keyEventArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	if(!keyEventArgs.window->isActive())
		return false;
	CD3D9WebView * view = g_pWebkit->GetView(keyEventArgs.window);
	EA::WebKit::KeyboardEvent e = {0};
	e.mId = keyEventArgs.scancode;
	e.mbChar = false;
	e.mbDepressed = true;
	view->GetView()->OnKeyboardEvent(e);
	return true;
}
bool OnCharKey(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs keyEventArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	if(!keyEventArgs.window->isActive())
		return false;
	CD3D9WebView * view = g_pWebkit->GetView(keyEventArgs.window);
	EA::WebKit::KeyboardEvent e = {0};
	e.mId = keyEventArgs.codepoint;
	e.mbChar = true;
	e.mbDepressed = false;
	view->GetView()->OnKeyboardEvent(e);
	return true;
}

