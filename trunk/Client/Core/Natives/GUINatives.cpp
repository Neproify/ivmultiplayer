//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: GUINatives.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqstring.h>
#include "../CClient.h"
#include <SharedUtility.h>
//#include "../CD3D9Webkit.hpp"

extern CClient * g_pClient;
//extern CD3D9WebKit * g_pWebkit;

String GetKeyNameByScan(DWORD dwCode)
{
	switch(dwCode)
	{
	case 0x1:
		return "escape";
	case 0x2:
		return "one";
	case 0x3:
		return "two";
	case 0x4:
		return "three";
	case 0x5:
		return "four";
	case 0x6:
		return "five";
	case 0x7:
		return "six";
	case 0x8:
		return "seven";
	case 0x9:
		return "eight";
	case 0x0a:
		return "nine";
	case 0x0b:
		return "zero";
	case 0x0c:
		return "minus";
	case 0x0d:
		return "equals";
	case 0x0e:
		return "backspace";
	case 0x0f:
		return "tab";
	case 0x10:
		return "q";
	case 0x11:
		return "w";
	case 0x12:
		return "e";
	case 0x13:
		return "r";
	case 0x14:
		return "t";
	case 0x15:
		return "y";
	case 0x16:
		return "u";
	case 0x17:
		return "i";
	case 0x18:
		return "o";
	case 0x19:
		return "p";
	case 0x1a:
		return "leftbracket";
	case 0x1b:
		return "rightbracket";
	case 0x1c:
		return "return";
	case 0x1d:
		return "leftcontrol";
	case 0x1e:
		return "a";
	case 0x1f:
		return "s";
	case 0x20:
		return "d";
	case 0x21:
		return "f";
	case 0x22:
		return "g";
	case 0x23:
		return "h";
	case 0x24:
		return "j";
	case 0x25:
		return "k";
	case 0x26:
		return "l";
	case 0x27:
		return "semicolon";
	case 0x28:
		return "apostrophe";
	case 0x29:
		return "grave";
	case 0x2a:
		return "leftshift";
	case 0x2b:
		return "backslash";
	case 0x2c:
		return "z";
	case 0x2d:
		return "x";
	case 0x2e:
		return "c";
	case 0x2f:
		return "v";
	case 0x30:
		return "b";
	case 0x31:
		return "n";
	case 0x32:
		return "m";
	case 0x33:
		return "comma";
	case 0x34:
		return "period";
	case 0x35:
		return "slash";
	case 0x36:
		return "rightshift";
	case 0x37:
		return "multiply";
	case 0x38:
		return "leftalt";
	case 0x39:
		return "space";
	case 0x3a:
		return "capital";
	case 0x3b:
		return "f1";
	case 0x3c:
		return "f2";
	case 0x3d:
		return "f3";
	case 0x3e:
		return "f4";
	case 0x3f:
		return "f5";
	case 0x40:
		return "f6";
	case 0x41:
		return "f7";
	case 0x42:
		return "f8";
	case 0x43:
		return "f9";
	case 0x44:
		return "f10";
	case 0x45:
		return "numlock";
	case 0x46:
		return "scrolllock";
	case 0x47:
		return "numpad7";
	case 0x48:
		return "numpad8";
	case 0x49:
		return "numpad9";
	case 0x4a:
		return "subtract";
	case 0x4b:
		return "numpad4";
	case 0x4c:
		return "numpad5";
	case 0x4d:
		return "numpad6";
	case 0x4e:
		return "add";
	case 0x4f:
		return "numpad1";
	case 0x50:
		return "numpad2";
	case 0x51:
		return "numpad3";
	case 0x52:
		return "numpad0";
	case 0x53:
		return "decimal";
	case 0x56:
		return "oem_102";
	case 0x57:
		return "f11";
	case 0x58:
		return "f12";
	case 0x64:
		return "f13";
	case 0x65:
		return "f14";
	case 0x66:
		return "f15";
	case 0x70:
		return "kana";
	case 0x73:
		return "abnt_c1";
	case 0x79:
		return "convert";
	case 0x7b:
		return "noconvert";
	case 0x7d:
		return "yen";
	case 0x7e:
		return "abnt_c2";
	case 0x8d:
		return "numpadequals";
	case 0x90:
		return "prevtrack";
	case 0x91:
		return "at";
	case 0x92:
		return "colon";
	case 0x93:
		return "underline";
	case 0x94:
		return "kanji";
	case 0x95:
		return "stop";
	case 0x96:
		return "ax";
	case 0x97:
		return "unlabeled";
	case 0x99:
		return "nexttrack";
	case 0x9c:
		return "numpadenter";
	case 0x9d:
		return "rightcontrol";
	case 0xa0:
		return "mute";
	case 0xa1:
		return "calculator";
	case 0xa2:
		return "playpause";
	case 0xa4:
		return "mediastop";
	case 0xae:
		return "volumedown";
	case 0xb0:
		return "volumeup";
	case 0xb2:
		return "webhome";
	case 0xb3:
		return "numpadcomma";
	case 0xb5:
		return "divide";
	case 0xb7:
		return "sysrq";
	case 0xb8:
		return "rightalt";
	case 0xc5:
		return "pause";
	case 0xc7:
		return "home";
	case 0xc8:
		return "arrowup";
	case 0xc9:
		return "pageup";
	case 0xcb:
		return "arrowleft";
	case 0xcd:
		return "arrowright";
	case 0xcf:
		return "end";
	case 0xd0:
		return "arrowdown";
	case 0xd1:
		return "pagedown";
	case 0xd2:
		return "insert";
	case 0xd3:
		return "delete";
	case 0xdb:
		return "leftwindows";
	case 0xdc:
		return "rightwindows";
	case 0xdd:
		return "appmenu";
	case 0xde:
		return "power";
	case 0xdf:
		return "sleep";
	case 0xe3:
		return "wake";
	case 0xe5:
		return "websearch";
	case 0xe6:
		return "webfavorites";
	case 0xe7:
		return "webrefresh";
	case 0xe8:
		return "webstop";
	case 0xe9:
		return "webforward";
	case 0xea:
		return "webback";
	case 0xeb:
		return "mycomputer";
	case 0xec:
		return "mail";
	case 0xed:
		return "mediaselect";
	default:
		return "unknown";
	}
}

// event guiVisibleChanged(string guiName)
bool OnVisibleChanged(const CEGUI::EventArgs &eventArgs)
{	
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiVisibleChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::WindowEventArgs eArgs = static_cast<const CEGUI::WindowEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());

	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}
bool OnHidden(const CEGUI::EventArgs &eventArgs)
{
	return OnVisibleChanged(eventArgs);
}
bool OnShown(const CEGUI::EventArgs &eventArgs)
{
	return OnVisibleChanged(eventArgs);
}

// event guiKeyPress(string guiName, string key, string keyState)
bool OnKeyPress(const CEGUI::KeyEventArgs &eArgs, String keyState)
{	
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiKeyPress");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	CEGUI::Key::Scan keyCode = eArgs.scancode;

	CSquirrelArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pArguments.push(GetKeyNameByScan(eArgs.scancode));
	pArguments.push(keyState);

	CSquirrelArgument pReturn = pEvents->Call(eventName, &pArguments, pScript);
	return pReturn.GetBool();
}
bool OnKeyDown(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs eArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	return OnKeyPress(eArgs, "down");
}
bool OnKeyUp(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs eArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	return OnKeyPress(eArgs, "up");
}

// event editBoxTextChanged(string guiName)
bool OnEditboxTextChanged(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName ("editBoxTextChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::WindowEventArgs eArgs = static_cast<const CEGUI::WindowEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event rowClick(string guiName)
bool OnRowClick(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName ("rowClick");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::WindowEventArgs eArgs = static_cast<const CEGUI::WindowEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CEGUI::MultiColumnList * pMultiColumnList = static_cast<CEGUI::MultiColumnList*>(eArgs.window);
	int iItemID = pMultiColumnList->getItemRowIndex(pMultiColumnList->getFirstSelectedItem());

	CSquirrelArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str()); // Window Name
	pArguments.push(iItemID); // Row

	CLogFile::Printf("Value: %i",iItemID);

	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiClick(guiName, mouseButton)
bool OnClick(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiClick");

	if(!pEvents->IsEventRegistered(eventName))
		return false;
	
	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pArguments.push(eArgs.button);
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseEnter(guiName)
bool OnMouseEnters(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseEnter");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseMove(guiName)
bool OnMouseMove(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseMove");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseLeave(guiName)
bool OnMouseLeaves(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseLeave");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event buttonClick(buttonName, bState)
// TODO: remove it. We have better alternative guiClick (OnClick) for any gui element
bool OnButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("buttonClick");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pArguments.push(0);	// FIXME or what?
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event checkBoxCheckedChanged(checkBoxName)
bool OnCheckboxCheckStateChanged(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("checkBoxCheckedChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event windowClose(windowName)
bool OnWindowClose(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("windowClose");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CSquirrel * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CSquirrelArguments pArguments;
	pArguments.push(pWindow->getName().c_str());

	// Event handler must return 1 to close window, otherwise, 0.
	CSquirrelArgument pReturn = pEvents->Call(eventName, &pArguments, pScript);

	if(pReturn.GetInteger())
		pWindow->hide();

	return true;
}

extern void SubscribeGuiEvents(CEGUI::Window * pWindow)
{		
	pWindow->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&OnClick));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&OnMouseEnters));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&OnMouseMove));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&OnMouseLeaves));
	pWindow->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&OnShown));
	pWindow->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&OnHidden));
	pWindow->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&OnKeyDown));
	pWindow->subscribeEvent(CEGUI::Window::EventKeyUp, CEGUI::Event::Subscriber(&OnKeyUp));
}

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


	CEGUI::Font * pFont = g_pClient->GetGUI()->GetFont(szFont, iSize, bScaled);

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

	sq_pushfloat(pVM, pFont->getTextExtent(g_pClient->GetGUI()->AnsiToCeguiFriendlyString(text)));
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

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();
	
	if(sbRelative != 0) 
	{
		x *= pGUI->GetDisplayWidth();
		y *= pGUI->GetDisplayHeight();
	}

	pGUI->DrawText(text, CEGUI::Vector2( x, y ), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), pFont, true, true);
	sq_pushbool(pVM, true);
	return 1;
}

// Main GUIElement

// GUIWindow
_MEMBER_FUNCTION_RELEASE_HOOK(GUIElement)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete((CEGUI::Window *)pInst);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, constructor)
{
	const char * name;
	sq_getstring(pVM, -1, &name);
	String szName = name;

	if(szName.IsEmpty())
		_SET_RELEASE_HOOK(GUIElement);

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	try
	{
		pGUI->GetWindowManager()->getWindow(szName.C_String());
	}
	catch(CEGUI::Exception &e)
	{
		e;
		_SET_RELEASE_HOOK(GUIElement);
	}

	CEGUI::Window * pWindow = pGUI->GetWindowManager()->getWindow(szName.C_String());
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIElement.");
		sq_pushbool(pVM, false);
		return 1;
	}

	SubscribeGuiEvents(pWindow);

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

	pWindow->setText(CGUI::AnsiToCeguiFriendlyString(text, strlen(text)));
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getSize)
{
	CGUIFrameWindow * pWindow = sq_getinstance<CGUIFrameWindow *>(pVM);

	if(pWindow)
	{
		CEGUI::UVector2 sz = pWindow->getSize();
		CSquirrelArguments args;
		args.push(sz.d_x.d_offset);
		args.push(sz.d_y.d_offset);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}

	sq_pushbool(pVM, false);
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
		pParentWindow = g_pClient->GetGUI()->GetWindowManager()->getWindow(parent);
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
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIFrameWindow();

	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIWindow.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&OnWindowClose));

	sq_pushbool(pVM, true);
	return 1;
}

// GUIText
_MEMBER_FUNCTION_IMPL(GUIText, constructor)
{
	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	CEGUI::Window * pWindow = pGUI->CreateGUIStaticText();

	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIText.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	pWindow->setProperty("FrameEnabled", "false");
	pWindow->setProperty("BackgroundEnabled", "false");
	pWindow->setFont(pGUI->GetFont("tahoma-bold"));
	pWindow->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	
	SubscribeGuiEvents(pWindow);

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

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	CEGUI::Font * pFont = g_pClient->GetGUI()->GetFont("tahoma-bold");
	float fTextWidth = pFont->getTextExtent(text);
	float fTextHeight = pFont->getFontHeight();
	pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
	pWindow->setText(CGUI::AnsiToCeguiFriendlyString(text, strlen(text)));
	sq_pushbool(pVM, true);
	return 1;
}

// GUIButton
_MEMBER_FUNCTION_IMPL(GUIButton, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIButton();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIButton.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OnButtonClick));

	sq_pushbool(pVM, true);
	return 1;
}

// GUIEditBox
_MEMBER_FUNCTION_IMPL(GUIEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIEditBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIEditBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);	

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&OnEditboxTextChanged));

	sq_pushbool(pVM, true);
	return 1;
}

// GUIMultiLineEditBox
_MEMBER_FUNCTION_IMPL(GUIMultiLineEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIMultiLineEditBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIMultiLineEditBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);	

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&OnEditboxTextChanged));

	sq_pushbool(pVM, true);
	return 1;
}

// GUIMultiColumnList
_MEMBER_FUNCTION_IMPL(GUIMultiColumnList, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIMultiColumnList();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIMultiColumnList.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);	

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber(&OnRowClick));

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIMultiColumnList, addRow)
{
	CEGUI::MultiColumnList * pWindow = sq_getinstance<CEGUI::MultiColumnList *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushinteger(pVM, pWindow->addRow());
	return 1;
}

// Custom ListboxTextItem class to automatically set the selection brush image on creation
class MultiColumnListItem : public CEGUI::ListboxTextItem
{
public:
	MultiColumnListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionColours(CEGUI::colour(0x0073AAFF));
		setSelectionBrushImage("WindowsLook", "Background");
	}
};

_MEMBER_FUNCTION_IMPL(GUIMultiColumnList, setItem)
{
	SQInteger sqiRow;
	sq_getinteger(pVM, -1, &sqiRow);

	SQInteger sqiColumn;
	sq_getinteger(pVM, -2, &sqiColumn);

	const char * value;
	sq_getstring(pVM, -3, &value);


	CEGUI::MultiColumnList * pWindow = sq_getinstance<CEGUI::MultiColumnList *>(pVM);

	if(!pWindow)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pWindow->setItem(new MultiColumnListItem(value), sqiColumn, sqiRow);
	sq_pushinteger(pVM, true);
	return 1;
}

// GUICheckBox
_MEMBER_FUNCTION_IMPL(GUICheckBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUICheckBox();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUICheckBox.");
		sq_pushbool(pVM, false);
		return 1;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OnCheckboxCheckStateChanged));

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

	// check file size
	/*FILE* pFile = fopen(filename, "rb");
	fseek(pFile, 0L, SEEK_END);
	int len = ftell(pFile);
	if(len == 0) {
		sq_pushbool(pVM, false);
		fclose(pFile);
		return 1;
	}
	fclose(pFile);*/

	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	String szName = pGUI->GetUniqueName();

	// Try to load the image
	try
	{

		CEGUI::ImagesetManager::getSingleton().createFromImageFile(szName.C_String(), filename, "resources");
		CGUIStaticImage * pImage = pGUI->CreateGUIStaticImage(CEGUI::String(szName.C_String()));
		
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
		CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
		pClientScriptManager->GetGUIManager()->Add(pImage, pClientScriptManager->GetScriptingManager()->Get(pVM));

		SubscribeGuiEvents(pImage);

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
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIProgressBar();
	if(!pWindow || SQ_FAILED(sq_setinstance(pVM, pWindow)))
	{
		CLogFile::Printf("Can't create GUIProgressBar.");
		sq_pushbool(pVM, false);
		return 1;
	}

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
 	pWindow->setVisible(true);
 	pWindow->setProperty("CurrentProgress", "0.0");

	SubscribeGuiEvents(pWindow);

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

/*

// GUIWebView
_MEMBER_FUNCTION_IMPL(GUIWebView, constructor)
{
	SQInteger width, height;
	const char * szUrl;
	sq_getinteger(pVM, -3, &width);
	sq_getinteger(pVM, -2, &height);
	sq_getstring(pVM, -1, &szUrl);
	CD3D9WebView * pView = g_pWebkit->CreateView(width, height, g_pGUI->GetDirect3DDevice());
	pView->GetView()->SetURI(szUrl);

	if(!pView || SQ_FAILED(sq_setinstance(pVM, pView->GetWindow())))
	{
		CLogFile::Printf("Can't create GUIWebView.");
		sq_pushbool(pVM, false);
		return 1;
	}

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pView->GetWindow(), pClientScriptManager->GetScriptingManager()->Get(pVM));
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
	int x, y, width, height;
	sq_getinteger(pVM, -1, &x);
	sq_getinteger(pVM, -2, &y);
	sq_getinteger(pVM, -3, &width);
	sq_getinteger(pVM, -4, &height);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->Draw(x, y, width, height);
	}

	sq_pushbool(pVM, true);
	return 1;
}

*/