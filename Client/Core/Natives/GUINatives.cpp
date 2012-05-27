//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: GUINatives.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "../CD3D9Webkit.hpp"
#include "../Natives.h"
#include <Scripting/CScriptingManager.h>
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqstring.h>
#include "../CNetworkManager.h"
#include "../CVehicleManager.h"
#include "../CPlayerManager.h"
#include "../CNetworkManager.h"
#include "../CChatWindow.h"
#include "../CGUI.h"
#include "../CGraphics.h"
#include <SharedUtility.h>
#include "../CClientScriptManager.h"
#include <CEvents.h>

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CChatWindow * g_pChatWindow;
extern CGUI * g_pGUI;
extern CGraphics * g_pGraphics;
extern CScriptingManager * g_pScriptingManager;
extern CClientScriptManager * g_pClientScriptManager;
extern CEvents * g_pEvents;
extern CD3D9WebKit * g_pWebkit;

bool OnButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	const CEGUI::MouseEventArgs mouseEventArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CSquirrel * pScript = g_pClientScriptManager->GetGUIManager()->GetScript(pWindow);
	CEGUI::String buttonName = pWindow->getName();
	CSquirrelArguments pArguments;
	pArguments.push(buttonName.c_str());
	pArguments.push(0);
	g_pEvents->Call("buttonClick", &pArguments, pScript);
	return true;
}

bool OnWindowClose(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CSquirrel * pScript = g_pClientScriptManager->GetGUIManager()->GetScript(pWindow);
	CEGUI::String windowName = pWindow->getName();
	CSquirrelArguments pArguments;
	pArguments.push(windowName.c_str());
	pArguments.push(0);
	g_pEvents->Call("windowClose", &pArguments, pScript);
	return true;
}
// GUI

// Font
_MEMBER_FUNCTION_IMPL(GUIFont, constructor)
{
	if(sq_gettop(pVM) < 2)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * szFont;
	int iSize = 8;
	bool bScaled = false;

	sq_getstring(pVM, 2, &szFont);
	if(sq_gettop(pVM) >= 3)
	{
		sq_getinteger(pVM, 3, &iSize);
		if(iSize < 3 || iSize > 100)
		{
			sq_pushbool(pVM, false);
			return 1;
		}

		if(sq_gettop(pVM) >= 4)
		{
			SQBool sqb;
			sq_getbool(pVM, 4, &sqb);
			bScaled = sqb != 0;
		}
	}


	CEGUI::Font * pFont = g_pGUI->GetFont(szFont, iSize, bScaled);

	if(!pFont || SQ_FAILED(sq_setinstance(pVM, pFont)))
	{
		CLogFile::Printf("Can't create GUIFont.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, getTextExtent)
{
	const char * text;
	sq_getstring(pVM, -1, (const char **)&text);
	CEGUI::Font * pFont = sq_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushfloat(pVM, pFont->getTextExtent(text));
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, getFontHeight)
{
	CEGUI::Font * pFont = sq_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushfloat(pVM, pFont->getFontHeight());
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, drawText)
{
	CEGUI::Font * pFont = sq_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	const char* text;
	SQBool sbRelative;
	float x, y;
	sq_getfloat(pVM, -4, &x);
	sq_getfloat(pVM, -3, &y);
	sq_getstring(pVM, -2, &text);
	sq_getbool(pVM, -1, &sbRelative);
	
	if(sbRelative != 0) 
	{
		x *= g_pGUI->GetDisplayWidth();
		y *= g_pGUI->GetDisplayHeight();
	}

	g_pGUI->DrawText(text, CEGUI::Vector2( x, y ), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), pFont, true, true);
	sq_pushbool(pVM, true);
	return 1;
}

// Main GUIElement

// GUIWindow
_MEMBER_FUNCTION_RELEASE_HOOK(GUIElement)
{
	g_pClientScriptManager->GetGUIManager()->Delete((CEGUI::Window *)pInst);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, constructor)
{
	const char * name;
	sq_getstring(pVM, -1, &name);
	String szName = name;

	if(szName.IsEmpty())
		_SET_RELEASE_HOOK(GUIElement);

	try
	{
		g_pGUI->GetWindowManager()->getWindow(szName.C_String());
	}
	catch(CEGUI::Exception &e)
	{
		e;
		_SET_RELEASE_HOOK(GUIElement);
	}

	CEGUI::Window * pWindow = g_pGUI->GetWindowManager()->getWindow(szName.C_String());
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIElement.");
		sq_pushbool(pVM, false);
		return 1;
	}
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setText)
{
	const char * text;
	sq_getstring(pVM, -1, (const char **)&text);
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setText(text);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setSize)
{
	float x, y;
	SQBool relative;
	sq_getbool(pVM, -1, &relative);
	sq_getfloat(pVM, -2, &y);
	sq_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	if(relative != 0)
	{
		pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
	}
	else
	{
		pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
	}
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setPosition)
{
	float x, y;
	SQBool relative;
	sq_getbool(pVM, -1, &relative);
	sq_getfloat(pVM, -2, &y);
	sq_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	if(relative != 0)
	{
		pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
	}
	else
	{
		pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
	}
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setRotation)
{
	float x, y, z;
	sq_getfloat(pVM, -1, &z);
	sq_getfloat(pVM, -2, &y);
	sq_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setRotation(CEGUI::Vector3(x, y, z));
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setAlpha)
{
	float alpha;
	sq_getfloat(pVM, -1, &alpha);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setAlpha(alpha);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setVisible)
{
	SQBool visible;
	sq_getbool(pVM, -1, &visible);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setVisible(visible != 0);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setParent)
{
	const char * parent;
	sq_getstring(pVM, -1, &parent);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	
	CEGUI::Window * pParentWindow = NULL;

	try
	{
		pParentWindow = g_pGUI->GetWindowManager()->getWindow(parent);
	}
	catch(CEGUI::UnknownObjectException &e)
	{
		(void)e;
		sq_pushbool(pVM, false);
		return 1;
	}

	pParentWindow->addChildWindow(pWindow);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getName)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushstring(pVM, pWindow->getName().c_str(), -1);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getText)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushstring(pVM, pWindow->getText().c_str(), -1);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setFont)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);
	CEGUI::Font * pFont = sq_getinstance<CEGUI::Font *>(pVM, 2);

	if(!pWindow || !pFont)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setFont(pFont);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setProperty)
{
	const char * prop;
	const char * value;
	sq_getstring(pVM, -1, &value);
	sq_getstring(pVM, -2, &prop);

	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	try
	{
		pWindow->setProperty(prop, value);
	}
	catch(CEGUI::UnknownObjectException &e)
	{
		(void)e;
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, activate)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->activate();
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, deactivate)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->deactivate();
	sq_pushbool(pVM, true);
	return 1;
}

// GUIWindow
_MEMBER_FUNCTION_IMPL(GUIWindow, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIFrameWindow();

	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIWindow.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&OnWindowClose));
	sq_pushbool(pVM, true);
	return 1;
}

// GUIText
_MEMBER_FUNCTION_IMPL(GUIText, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIStaticText();

	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIText.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	pWindow->setProperty("FrameEnabled", "false");
	pWindow->setProperty("BackgroundEnabled", "false");
	pWindow->setFont(g_pGUI->GetFont("tahoma-bold"));
	pWindow->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIText, setText)
{
	const char * text;
	sq_getstring(pVM, -1, (const char **)&text);
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	CEGUI::Font * pFont = g_pGUI->GetFont("tahoma-bold");
	float fTextWidth = pFont->getTextExtent(text);
	float fTextHeight = pFont->getFontHeight();
	pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
	pWindow->setText(text);
	sq_pushbool(pVM, true);
	return 1;
}

// GUIButton
_MEMBER_FUNCTION_IMPL(GUIButton, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIButton();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIButton.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	pWindow->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OnButtonClick));
	sq_pushbool(pVM, true);
	return 1;
}

// GUIEditBox
_MEMBER_FUNCTION_IMPL(GUIEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIEditBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIEditBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	sq_pushbool(pVM, true);
	return 1;
}
// GUIMultiLineEditBox
_MEMBER_FUNCTION_IMPL(GUIMultiLineEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIMultiLineEditBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIMultiLineEditBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	sq_pushbool(pVM, true);
	return 1;
}

// GUICheckBox
_MEMBER_FUNCTION_IMPL(GUICheckBox, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUICheckBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUICheckBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUICheckBox, setChecked)
{
	SQBool sqbChecked;
	sq_getbool(pVM, -1, &sqbChecked);

	CEGUI::Checkbox * pWindow = sq_getinstance<CEGUI::Checkbox *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setSelected(sqbChecked != 0);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUICheckBox, isChecked)
{
	CEGUI::Checkbox * pWindow = sq_getinstance<CEGUI::Checkbox *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pWindow->isSelected());
	return 1;
}

// GUIImage
_MEMBER_FUNCTION_IMPL(GUIImage, constructor)
{
	const char * filename;
	sq_getstring(pVM, -1, &filename);

	String szName = g_pGUI->GetUniqueName();

	// Try to load the image
	try
	{
		CEGUI::ImagesetManager::getSingleton().createFromImageFile(szName.C_String(), filename, "resources");
		CGUIStaticImage * pImage = g_pGUI->CreateGUIStaticImage(CEGUI::String(szName.C_String()));
		
		pImage->setProperty("FrameEnabled", "false");
		pImage->setProperty("BackgroundEnabled", "false");
		pImage->setProperty("Image", String("set:%s image:full_image", szName.C_String()).C_String());

		if(!pImage || SQ_FAILED(sq_setinstance(pVM, pImage)))
		{
			CLogFile::Printf("Can't create GUIImage.");
			sq_pushbool(pVM, false);
			return 1;
		}

		//_SET_RELEASE_HOOK(GUIElement);
		g_pClientScriptManager->GetGUIManager()->Add(pImage, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
		pImage->setVisible(true);
		sq_pushbool(pVM, true);
		return 1;
	}
	catch(CEGUI::Exception e)
	{
		// Failed to load, might not exist
		CLogFile::Printf("Can't create GUIImage (%s does not exist)", filename);
		sq_pushbool(pVM, false);
		return 1;
	}
}
// GUIProgressBar
_MEMBER_FUNCTION_IMPL(GUIProgressBar, constructor)
{
	CEGUI::Window * pWindow = g_pGUI->CreateGUIProgressBar();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIProgressBar.");
		sq_pushbool(pVM, false);
		return 1;
	}
	g_pClientScriptManager->GetGUIManager()->Add(pWindow, g_pClientScriptManager->GetScriptingManager()->Get(pVM));
 	pWindow->setVisible(true);
 	pWindow->setProperty("CurrentProgress", "0.0");
 	sq_pushbool(pVM, true);
	return 1;
}
_MEMBER_FUNCTION_IMPL(GUIProgressBar, setValue)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	float fCurrentValue = 0.0f;
	sq_getfloat(pVM, 2, &fCurrentValue);

 	if ( fCurrentValue > 100.0f || fCurrentValue < 0.0f )
	{
 		CLogFile::Printf("Invalid first argument value. (Argument accept values from 0.0 to 100.0 [float], your value %f)", fCurrentValue);
 		sq_pushbool(pVM, false);
 	}

 	char szNewProgress[4];
	sprintf(szNewProgress, "%f", fCurrentValue*0.01);
	pWindow->setProperty("CurrentProgress", szNewProgress);

	sq_pushbool(pVM, true);
	return 1;
}

// GUIWebView
_MEMBER_FUNCTION_IMPL(GUIWebView, constructor)
{
	SQInteger width, height;
	const char * szUrl;
	sq_getinteger(pVM, -3, &width);
	sq_getinteger(pVM, -2, &height);
	sq_getstring(pVM, -1, &szUrl);
	CD3D9WebView * pView = g_pWebkit->CreateView(width, height, szUrl, g_pGUI->GetDirect3DDevice());

	if(!pView || SQ_FAILED(sq_setinstance(pVM, pView->GetWindow())))
	{
		CLogFile::Printf("Can't create GUIWebView.");
		sq_pushbool(pVM, false);
		return 1;
	}

	g_pClientScriptManager->GetGUIManager()->Add(pView->GetWindow(), g_pClientScriptManager->GetScriptingManager()->Get(pVM));
	pView->GetWindow()->setVisible(true);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setURI)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * szURL;
	sq_getstring(pVM, -1, &szURL);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->CancelLoad();
		pView->GetView()->SetURI(szURL);
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, evaluateJavaScript)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * script;
	sq_getstring(pVM, -1, &script);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		EA::WebKit::JavascriptValue * pValue = g_pWebkit->GetWebKit()->CreateJavaScriptValue();
		pView->GetView()->EvaluateJavaScript(script, strlen(script), pValue);
		switch(pValue->GetType())
		{
		case EA::WebKit::JavascriptValueType_Boolean:
			sq_pushbool(pVM, pValue->GetBooleanValue());
			break;
		case EA::WebKit::JavascriptValueType_Number:
			sq_pushfloat(pVM, (float)pValue->GetNumberValue());
			break;
		//case EA::WebKit::JavascriptValueType_String:
		//	sq_pushstring(pVM, pValue->GetStringCharacters());
		default:
			sq_pushbool(pVM, false);
		}
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, sendSignal)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * szSignal;
	sq_getstring(pVM, -1, &szSignal);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		if(strcmp(szSignal, "back") == 0)
		{
			pView->GetView()->GoBack();
		}
		else if(strcmp(szSignal, "forward") == 0)
		{
			pView->GetView()->GoForward();
		}
		else if(strcmp(szSignal, "cancel") == 0)
		{
			pView->GetView()->CancelLoad();
		}
		else if(strcmp(szSignal, "refresh") == 0)
		{
			pView->GetView()->Refresh();
		}
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setHTML)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * szHTML;
	sq_getstring(pVM, -1, &szHTML);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->SetHTML(szHTML, strlen(szHTML));
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setElementText)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * text;
	const char * id;
	sq_getstring(pVM, -1, &id);
	sq_getstring(pVM, -2, &text);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->SetElementTextById(id, text);
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, getLoadInfo)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		CSquirrelArguments table;
		table.push("completed");
		table.push(pView->GetView()->GetLoadInfo().mbCompleted);
		table.push("content-length");
		table.push((int)pView->GetView()->GetLoadInfo().mContentLength);
		table.push("last-changed-time");
		table.push((int)pView->GetView()->GetLoadInfo().mLastChangedTime);
		table.push("title");
		table.push(0); // ADAMIX: todo
		table.push("status-code");
		table.push(pView->GetView()->GetLoadInfo().mStatusCode);
		table.push("uri");
		table.push(0); // ADAMIX: todo
		sq_pusharg(pVM, CSquirrelArgument(table, false));
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, clickElement)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * szElementOrId;
	sq_getstring(pVM, -1, &szElementOrId);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->ClickElementsByIdOrClass(szElementOrId);
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setSize)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	int width, height;
	sq_getinteger(pVM, -1, &width);
	sq_getinteger(pVM, -2, &height);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->SetSize(width, height);
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, registerJavaScriptMethod)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	const char * szMethod;
	sq_getstring(pVM, -1, &szMethod);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->RegisterJavascriptMethod(szMethod);
		pView->GetView()->RebindJavascript();
	}

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, draw)
{
 	CEGUI::Window * pWindow = sq_getinstance<CEGUI::Window *>(pVM);
	int x, y;
	sq_getinteger(pVM, -1, &x);
	sq_getinteger(pVM, -2, &y);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->Draw(x, y);
	}

	sq_pushbool(pVM, true);
	return 1;
}