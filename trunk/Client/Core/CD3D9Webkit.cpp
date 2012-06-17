
#include "CD3D9Webkit.hpp"
#include <list>
#include <queue>
#include <CEvents.h>

extern CEvents * g_pEvents;

DWORD DIKToScanCode(WORD DIK)
{
	switch(DIK)
	{
	case DIK_A: return 0x41;               // Control + A (Select All)
	case DIK_C: return 0x43;               // Control + C (Copy)
	case DIK_X: return 0x58;               // Control + X (Cut)
	case DIK_V: return 0x56;               // Control + V (Paste)
	case DIK_TAB: return VK_TAB;           // Tab
	case DIK_HOME: return VK_HOME;         // Home
	case DIK_END: return VK_END;           // End
	case DIK_INSERT: return VK_INSERT;     // Insert
	case DIK_DELETE: return VK_DELETE;     // Delete
	case DIK_UP: return VK_UP;             // Arrow Up
	case DIK_DOWN: return VK_DOWN;         // Arrow Down
	case DIK_LEFT: return VK_LEFT;         // Arrow Left
	case DIK_RIGHT: return VK_RIGHT;       // Arrow Right
	case DIK_LSHIFT: return VK_LSHIFT;     // Left Shift
	case DIK_RSHIFT: return VK_RSHIFT;     // Right Shift
	case DIK_BACK: return VK_BACK;         // Backspace
	case DIK_RETURN: return VK_RETURN;     // Enter
	case DIK_LCONTROL: return VK_LCONTROL; // Left Control
	case DIK_RCONTROL: return VK_RCONTROL; // Right Control
	}

	return 0;
}

const char * ConvertUnicodeToANSI(const char16_t * uniString)
{
	int len = wcslen(uniString);
	char * out = new char[len+1];
	SharedUtility::UnicodeToAnsi(uniString, len, out, len);
	out[len] = 0;
	return out;
}

extern CGUI                 * g_pGUI;
extern CMainMenu            * g_pMainMenu;

struct keyEvent
{
	EA::WebKit::KeyboardEvent e;
	EA::WebKit::View * view;
};

std::queue<keyEvent> keyEventsQueue;

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
	wheelEvent.mLineDelta = 3;
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
	e.mId = DIKToScanCode(keyEventArgs.scancode);
	e.mbChar = false;
	e.mbDepressed = false;
	keyEvent queueEvent;
	queueEvent.e = e;
	queueEvent.view = view->GetView();
	keyEventsQueue.push(queueEvent);
	//view->GetView()->OnKeyboardEvent(e);
	return true;
}
bool OnKeyDown(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs keyEventArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	if(!keyEventArgs.window->isActive())
		return false;
	CD3D9WebView * view = g_pWebkit->GetView(keyEventArgs.window);
	EA::WebKit::KeyboardEvent e = {0};
	e.mId = DIKToScanCode(keyEventArgs.scancode);
	e.mbChar = false;
	e.mbDepressed = true;
	keyEvent queueEvent;
	queueEvent.e = e;
	queueEvent.view = view->GetView();
	keyEventsQueue.push(queueEvent);
	//view->GetView()->OnKeyboardEvent(e);
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
	keyEvent queueEvent;
	queueEvent.e = e;
	queueEvent.view = view->GetView();
	keyEventsQueue.push(queueEvent);
	//view->GetView()->OnKeyboardEvent(e);
	return true;
}

bool CD3D9WebkitNotification::ViewUpdate(EA::WebKit::ViewUpdateInfo& info)
{
	return true;
}
bool CD3D9WebkitNotification::LoadUpdate(EA::WebKit::LoadInfo& info)
{
	const char * szURI = ConvertUnicodeToANSI(g_pWebkit->GetWebKit()->GetCharacters(info.mURI));
	const char * szTitle = ConvertUnicodeToANSI(g_pWebkit->GetWebKit()->GetCharacters(info.mPageTitle));
	CSquirrelArguments table;
	CD3D9WebView * pView = (CD3D9WebView*)info.mpView->GetUserData();
	//CLogFile::Printf("View: 0x%x", pView);
	table.push(pView->GetName().Get());
	table.push(info.mbCompleted);
	table.push((int)info.mContentLength);
	table.push((int)info.mLastChangedTime);
	table.push(szTitle);
	table.push(info.mStatusCode);
	table.push(szURI);
	g_pEvents->Call("webkitLoadUpdate", &table);
	delete szURI;
	delete szTitle;
	return true;
}

bool CD3D9WebkitNotification::TextInputState(EA::WebKit::TextInputStateInfo& info)
{
	return true;
}

bool CD3D9WebkitNotification::JavascriptMethodInvoked(EA::WebKit::JavascriptMethodInvokedInfo& info)
{
	CSquirrelArguments table;
	CD3D9WebView * pView = (CD3D9WebView*)info.mpView->GetUserData();
	table.push(pView->GetName().Get());
	table.push(info.mMethodName);
	CSquirrelArguments array;
	const char * strings[10U] = {0};
	int stringsCount = 0;
	for(int i = 0; i < info.mArgumentCount; i++)
	{
		switch(info.mArguments[i].GetType())
		{
		case EA::WebKit::JavascriptValueType_Boolean:
			array.push(info.mArguments[i].GetBooleanValue());
			break;
		case EA::WebKit::JavascriptValueType_Number:
			array.push((float)info.mArguments[i].GetNumberValue());
			break;
		case EA::WebKit::JavascriptValueType_String:
			strings[stringsCount] = ConvertUnicodeToANSI(info.mArguments[i].GetStringCharacters());
			array.push(strings[stringsCount]);
			stringsCount++;
			break;
		default:
			array.push("undefined");
			break;
		}
	}
	table.push(array, true);
	g_pEvents->Call("webkitJSMethodInvoked", &table);
	for(int i = 0; i < stringsCount; i++)
	{
		if(strings[i] != 0)
		{
			delete strings[i];
		}
	}
	return true;
}

CD3D9WebView::CD3D9WebView(int width, int height, EA::WebKit::View * view)
{
	this->device = device;
	this->view = view;
	this->width = width;
	this->height = height;
	device = g_pGUI->GetDirect3DDevice();
	D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
	D3DXCreateSprite(device, &sprite);

	view->SetUserData(this);

	texturenum = 0;

	name = g_pGUI->GetUniqueName();

	view->GetSurface()->SetPixelFormat(EA::Raster::kPixelFormatTypeARGB);

	CEGUI::Texture & ceguiTexture = g_pGUI->GetRenderer()->createTexture(texture);
	CEGUI::ImagesetManager::getSingleton().create(String("%s_%d", name.Get(), texturenum).Get(), ceguiTexture);
	CEGUI::ImagesetManager::getSingleton().get(String("%s_%d", name.Get(), texturenum).Get()).defineImage("full_image", CEGUI::Rect(0, 0, width, height), CEGUI::Point(0, 0));

	image = g_pGUI->CreateGUIStaticImage(CEGUI::String(name.Get()));
	image->setProperty("FrameEnabled", "false");
	image->setProperty("BackgroundEnabled", "false");
	image->setProperty("Image", String("set:%s image:full_image", String("%s_%d", name.Get(), texturenum).Get()).Get());
	image->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));
	image->setVisible(true);

	image->subscribeEvent(CEGUI::PushButton::EventMouseMove, CEGUI::Event::Subscriber(&OnMouseMove));
	image->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::Event::Subscriber(&OnMouseDown));
	image->subscribeEvent(CEGUI::PushButton::EventMouseWheel, CEGUI::Event::Subscriber(&OnMouseWheel));
	image->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&OnMouseUp));
	image->subscribeEvent(CEGUI::PushButton::EventCharacterKey, CEGUI::Event::Subscriber(&OnCharKey));
	image->subscribeEvent(CEGUI::PushButton::EventKeyUp, CEGUI::Event::Subscriber(&OnKeyUp));
	image->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&OnKeyDown));

	//image->setAlwaysOnTop(true);
}
void CD3D9WebView::RecreateTexture()
{
	texturenum++;
	texture->Release();
	D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
	CEGUI::Texture & ceguiTexture = g_pGUI->GetRenderer()->createTexture(texture);
	CEGUI::ImagesetManager::getSingleton().create(String("%s_%d", name.Get(), texturenum).Get(), ceguiTexture);
	CEGUI::ImagesetManager::getSingleton().get(String("%s_%d", name.Get(), texturenum).Get()).defineImage("full_image", CEGUI::Rect(0, 0, width, height), CEGUI::Point(0, 0));
	image->setProperty("Image", String("set:%s image:full_image", String("%s_%d", name.Get(), texturenum).Get()).Get());
	CEGUI::ImagesetManager::getSingleton().destroy(String("%s_%d", name.Get(), texturenum - 1).Get());
}
void CD3D9WebView::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	RecreateTexture();
	this->view->SetSize(width, height);
	image->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));
}
void CD3D9WebView::SetPosition(CEGUI::UVector2 & vec)
{
	image->setPosition(vec);
}
void CD3D9WebView::SetData(void * buffer)
{
	D3DLOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, NULL, 0);
	unsigned int * buf = (unsigned int*)buffer;
	unsigned int * destBuffer = (unsigned int*)lockedRect.pBits;
	memcpy(lockedRect.pBits, buffer, width*height*4);
	texture->UnlockRect(0);
	
	image->invalidate();
}
void CD3D9WebView::Draw(int x, int y, int w, int h)
{
	RECT rect = {0, 0, w, h};

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &rect, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(x, y, 0.0f), 0xFFFFFFFF);
	sprite->End();
}
EA::WebKit::View * CD3D9WebView::GetView()
{
	return view;
}
IDirect3DTexture9 * CD3D9WebView::GetTexture()
{
	return texture;
}
CEGUI::Window * CD3D9WebView::GetWindow()
{
	return image;
}
String CD3D9WebView::GetName()
{
	return name;
}

CD3D9WebKit::CD3D9WebKit()
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

	EA::WebKit::Parameters& param = webkit->GetParameters();
	param.mpLocale = "en-us";
	param.mEnableSmoothText = false;
	param.mPluginsEnabled = false;
	param.mJavaScriptEnabled = true;

	webkit->SetParameters(param);

	raster = webkit->GetEARasterInstance();

	tickCount = GetTickCount();

	m_pNotification = new CD3D9WebkitNotification();
	webkit->SetViewNotification(m_pNotification);
}
CD3D9WebKit::~CD3D9WebKit()
{
	/*for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); ++it)
	{
		DestroyView((*it));
	}*/
	webkit->Shutdown();
	webkit->Destroy();

	if(font_style)
		font_style->Destroy();
}
void CD3D9WebKit::RenderAll(bool bSetData, bool bTick)
{
	for(std::list<CD3D9WebView*>::iterator it = views.begin(); it != views.end(); it++)
	{
		bool b = false;
		if(bTick && ((GetTickCount() - tickCount) > 75))
		{
			tickCount = GetTickCount();
			if(keyEventsQueue.size() != 0)
			{
				keyEventsQueue.front().view->OnKeyboardEvent(keyEventsQueue.front().e);
				keyEventsQueue.pop();
			}
			b = (*it)->GetView()->Tick();
		}
		if(bSetData && b)
		{
			(*it)->SetData((*it)->GetView()->GetSurface()->GetData());
		}
	}
}
EA::WebKit::IEAWebkit * CD3D9WebKit::GetWebKit()
{
	return webkit;
}
CD3D9WebView * CD3D9WebKit::GetView(CEGUI::Window * window)
{
	std::list<CD3D9WebView*>::const_iterator it;
	for(it = views.begin(); it != views.end(); it++)
	{
		if((*it)->GetWindow() == window)
			return (*it);
	}
	return NULL;
}
CD3D9WebView * CD3D9WebKit::GetView(EA::WebKit::View * view)
{
	std::list<CD3D9WebView*>::const_iterator it;
	for(it = views.begin(); it != views.end(); it++)
	{
		if((*it)->GetView() == view)
			return (*it);
	}
	return NULL;
}
CD3D9WebView * CD3D9WebKit::GetView(String name)
{
	std::list<CD3D9WebView*>::const_iterator it;
	for(it = views.begin(); it != views.end(); it++)
	{
		if((*it)->GetName() == name)
			return (*it);
	}
	return NULL;
}
CD3D9WebView * CD3D9WebKit::CreateView(int width, int height, LPDIRECT3DDEVICE9 device)
{
	EA::WebKit::View * view = webkit->CreateView();
	EA::WebKit::ViewParameters params = view->GetParameters();
	params.mWidth = width;
	params.mHeight = height;
	view->InitView(params);
	view->SetSize(width, height);

	view->CreateJavascriptBindings("IVMP");

	CD3D9WebView * webView = new CD3D9WebView(width, height, view);

	views.push_back(webView);
		
	return webView;
}
void CD3D9WebKit::DestroyView(CD3D9WebView * webView)
{
	webView->GetView()->CancelLoad();
	webView->GetView()->ShutdownView();
	webkit->DestroyView(webView->GetView());
}