//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGUI.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// FIXUPDATE
// move gui stuff to its own module and have wrapper classes for each widget
// e.t.c.

#pragma once

#include <list>
#include "DXSDK/Include/d3d9.h"
#include "DXSDK/Include/d3dx9.h"
#include <CEGUI.h>
#include <RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h>
#include "CDirectInput8Proxy.h"
#include <CString.h>
#include <Math\CMath.h>

#define STYLE_SCHEME "WindowsLook.scheme"
#define STYLE_PREFIX "WindowsLook"
#define STYLE_IMAGES "WindowsLook"

// TODO: Rename these to DIB (DIMB?) (DIM?) (Same as DIK_)
#define DIMOUSE_LEFTBUTTON   0
#define DIMOUSE_RIGHTBUTTON  1
#define DIMOUSE_MIDDLEBUTTON 2
#define DIMOUSE_4BUTTON      3
#define DIMOUSE_5BUTTON      4
#define DIMOUSE_6BUTTON      5
#define DIMOUSE_7BUTTON      6
#define DIMOUSE_8BUTTON      7

#define InheritedStruct(inheritFrom, structName) struct structName : inheritFrom { }

// TODO: These should all be the cegui types
struct CGUIWindow : CEGUI::Window
{
	std::string getText();
};

InheritedStruct(CGUIWindow, CGUITitleBar);
InheritedStruct(CGUIWindow, CGUIButton);
InheritedStruct(CGUIWindow, CGUIFrameWindow);
InheritedStruct(CGUIWindow, CGUIEditBox);
InheritedStruct(CGUIWindow, CGUIMultiLineEditBox);
InheritedStruct(CGUIWindow, CGUIStaticImage);
InheritedStruct(CGUIWindow, CGUIStaticText);
InheritedStruct(CGUIWindow, CGUICheckBox);
InheritedStruct(CGUIWindow, CGUIMultiColumnList);
InheritedStruct(CGUIWindow, CGUITabControl);
InheritedStruct(CGUIWindow, CGUITabContentPane);
InheritedStruct(CGUIWindow, CGUIProgressBar);

enum eGUIMessageBoxType
{
	GUI_MESSAGEBOXTYPE_OK,
	GUI_MESSAGEBOXTYPE_YESNO,
	GUI_MESSAGEBOXTYPE_YESNOCANCEL
};

enum eGUIMessageBoxResponse
{
	GUI_MESSAGEBOX_YES,
	GUI_MESSAGEBOX_NO,
	GUI_MESSAGEBOX_CANCEL
};

typedef void (* GUIMessageBoxHandler_t)(eGUIMessageBoxResponse type);

class CGUI
{
private:
	IDirect3DDevice9         * m_pD3DDevice;
	CEGUI::Direct3D9Renderer * m_pRenderer;
	CEGUI::System            * m_pSystem;
	CEGUI::Scheme            * m_pScheme;
	CEGUI::MouseCursor       * m_pCursor;
	CEGUI::WindowManager     * m_pWindowManager;
	CEGUI::DefaultWindow     * m_pDefaultWindow;
	CEGUI::FontManager       * m_pFontManager;
	CEGUI::GeometryBuffer    * m_pTextDrawingGeometryBuffer;

	struct
	{
		CGUIFrameWindow      * pWindow;
		CGUIStaticText       * pText;
		CGUIButton           * pButtons[3];
		GUIMessageBoxHandler_t pfnHandler;
	} m_messageBox;

	IDirectInput8            * m_pInput;
	IDirectInputDevice8      * m_pInputMouse;
	bool                       m_bInitialized;
	bool					   m_bScriptedCursorVisible;
	DWORD                      m_dwInputAxes;
	DWORD                      m_dwInputButtons;
	DIMOUSESTATE2              m_MouseState;
	DWORD                      dwLastClickTime[8];
	BYTE                       byteButtonWasClicked[8];
	BYTE                       byteButtonClicked[8];
	RECT                       m_rCursorPosition;
	int                        m_iCurrentId;
	DWORD                      m_dwDoubleClickTime;
	POINT                      m_clickPosition;
	unsigned int               m_uiCurrentKyeFag;

public:
	CGUI(IDirect3DDevice9 * pD3DDevice);
	~CGUI();

	IDirect3DDevice9         * GetDirect3DDevice() { return m_pD3DDevice; }
	char                     * GetUniqueName();
	bool                       Initialize();
	unsigned int               GetDisplayWidth();
	unsigned int               GetDisplayHeight();
	void                       Render();
	void                       DrawText(String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect = CEGUI::colour(0xFFFFFFFF), CEGUI::Font * pFont = NULL, bool bProcessFormatting = true, bool bAllowColorFormatting = true, CEGUI::Rect * rClipRect = NULL, float fSpaceExtra = 0.0f, float fXScale = 1.0f, float fYScale = 1.0f);
	void                       DrawText(String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, String sFontName, bool bProcessFormatting = true, bool bAllowColorFormatting = true, CEGUI::Rect * rClipRect = NULL, float fSpaceExtra = 0.0f, float fXScale = 1.0f, float fYScale = 1.0f);

	// Message box
	bool                       OnMessageBoxClick(const CEGUI::EventArgs& eventArgs);
	void                       ShowMessageBox(const CEGUI::String &sText, const CEGUI::String &sTitle = "", eGUIMessageBoxType style = GUI_MESSAGEBOXTYPE_OK, GUIMessageBoxHandler_t pfnHandler = NULL);
	void                       HideMessageBox();

	CEGUI::Window            * CreateGUIWindow(const CEGUI::String &sType, const CEGUI::String &sName = "", CEGUI::Window * pParentWindow = NULL);
	void                       RemoveGUIWindow(CEGUI::String &sName);
	void                       RemoveGUIWindow(CEGUI::Window * pWindow);
	void                       RemoveGUIWindow(CGUITitleBar * pTitleBar);
	void                       RemoveGUIWindow(CGUIButton * pButton);
	void                       RemoveGUIWindow(CGUIFrameWindow * pFrameWindow);
	void                       RemoveGUIWindow(CGUIEditBox * pEditBox);
	void                       RemoveGUIWindow(CGUIMultiLineEditBox * pMultiLineEditBox);
	void                       RemoveGUIWindow(CGUIStaticImage * pStaticText);
	void                       RemoveGUIWindow(CGUIStaticText * pStaticText);
	void                       RemoveGUIWindow(CGUICheckBox * pCheckBox);
	void                       RemoveGUIWindow(CGUIProgressBar * pProgressBar);
	CGUITitleBar             * CreateGUITitleBar(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUITitleBar             * CreateGUITitleBar(CEGUI::Window * pParentWindow = NULL);
	CGUIButton               * CreateGUIButton(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIButton               * CreateGUIButton(CEGUI::Window * pParentWindow = NULL);
	CGUIFrameWindow          * CreateGUIFrameWindow(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIFrameWindow          * CreateGUIFrameWindow(CEGUI::Window * pParentWindow = NULL);
	CGUIEditBox              * CreateGUIEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIEditBox              * CreateGUIEditBox(CEGUI::Window * pParentWindow = NULL);
	CGUIMultiLineEditBox	 * CreateGUIMultiLineEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIMultiLineEditBox     * CreateGUIMultiLineEditBox(CEGUI::Window * pParentWindow = NULL);
	CGUIStaticImage          * CreateGUIStaticImage(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIStaticImage          * CreateGUIStaticImage(CEGUI::Window * pParentWindow = NULL);
	CGUIStaticText           * CreateGUIStaticText(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIStaticText           * CreateGUIStaticText(CEGUI::Window * pParentWindow = NULL);
	CGUICheckBox             * CreateGUICheckBox(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUICheckBox             * CreateGUICheckBox(CEGUI::Window * pParentWindow = NULL);
	CGUIMultiColumnList      * CreateGUIMultiColumnList(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIMultiColumnList      * CreateGUIMultiColumnList(CEGUI::Window * pParentWindow = NULL);
	CGUITabControl           * CreateGUITabControl(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUITabControl           * CreateGUITabControl(CEGUI::Window * pParentWindow = NULL);
	CGUITabContentPane       * CreateGUITabContentPane(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUITabContentPane       * CreateGUITabContentPane(CEGUI::Window * pParentWindow = NULL);
	CGUIProgressBar			 * CreateGUIProgressBar(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIProgressBar		     * CreateGUIProgressBar(CEGUI::Window * pParentWindow = NULL);
	void                       SetCursorVisible(bool bVisible);
	void                       SetScriptedCursorVisible(bool bVisible);
	RECT                       GetCursorPosition();
	void					   SetCursorPosition(float posX, float posY);
	bool                       IsCursorVisible(bool bVisible);
	void                       OnLostDevice();
	void                       OnResetDevice();
	bool                       MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool                       OnGUIKeyDown(const CEGUI::EventArgs &eventArgs);
	bool                       IsInitialized() { return m_bInitialized; }
	CEGUI::Direct3D9Renderer * GetRenderer() { return m_pRenderer; }
	CEGUI::System            * GetSystem() { return m_pSystem; }
	CEGUI::WindowManager     * GetWindowManager() { return m_pWindowManager; }
	CEGUI::DefaultWindow     * GetDefaultWindow() { return m_pDefaultWindow; }
	CEGUI::Font              * GetFont(String strFont, unsigned int uiSize = 8, bool bScaled = false);

	static CEGUI::String       AnsiToCeguiFriendlyString(const char * szAnsiString, unsigned int uiLength);
	static CEGUI::String       AnsiToCeguiFriendlyString(String strAnsiString);
};

class CGUIElement
{
private:
	CGUI * m_pGUI;
	CEGUI::Window * m_pWindow;
	CGUIElement * m_pParent;

public:
	CGUIElement(CGUI * pGUI, std::string strType, CGUIElement * pParent)
	{
		// Set the gui pointer
		m_pGUI = pGUI;

		// Create the window
		m_pWindow = pGUI->GetWindowManager()->createWindow(strType.c_str(), pGUI->GetUniqueName());

		// Did the window create successfully?
		if(m_pWindow)
		{
			// Set the window user data to this class instance
			m_pWindow->setUserData(this);

			// Set the parent to NULL
			m_pParent = NULL;

			// Ensure we have a valid parent window
			if(!pParent)
				//m_pParent = pGUI->GetDefaultWindow(); // FIXME: GUI Default Window needs to have its own CGUIElement

			// Set our parent window
			SetParent(pParent);
		}
	}

	~CGUIElement()
	{
		// Do we have a valid window?
		if(m_pWindow)
		{
			// Destroy the window
			m_pGUI->GetWindowManager()->destroyWindow(m_pWindow);
		}
	}

	// this will be the only non virtual func
	CEGUI::Window * GetWindow()
	{
		return m_pWindow;
	}

	void SetParent(CGUIElement * pParent)
	{
		if(m_pWindow && (m_pParent != pParent))
		{
			pParent->AddChild(this);
			m_pParent = pParent;
		}
	}

	CGUIElement * GetParent()
	{
		return m_pParent;
	}

	void AddChild(CGUIElement * pChild)
	{
		if(m_pWindow)
			m_pWindow->addChildWindow(pChild->GetWindow());
	}

	void RemoveChild(CGUIElement * pChild)
	{
		if(m_pWindow)
			m_pWindow->removeChildWindow(pChild->GetWindow());
	}

	void SetEnabled(bool bEnabled)
	{
		if(m_pWindow)
			m_pWindow->setEnabled(bEnabled);
	}

	bool IsEnabled()
	{
		if(m_pWindow)
			return !m_pWindow->isDisabled();

		return false;
	}

	void SetVisible(bool bVisible)
	{
		if(m_pWindow)
			m_pWindow->setVisible(bVisible);
	}

	bool IsVisible()
	{
		if(m_pWindow)
			return m_pWindow->isVisible();

		return false;
	}

	void SetActive(bool bActive)
	{
		if(m_pWindow)
		{
			if(bActive)
				m_pWindow->activate();
			else
				m_pWindow->deactivate();
		}
	}

	bool IsActive()
	{
		if(m_pWindow)
			return m_pWindow->isActive();

		return false;
	}

	void SetAlpha(float fAlpha)
	{
		if(m_pWindow)
			m_pWindow->setAlpha(fAlpha);
	}

	float GetAlpha()
	{
		if(m_pWindow)
			return m_pWindow->getAlpha();

		return 0.0f;
	}

	void SetFont(std::string strFont = "")
	{
		if(m_pWindow)
			m_pWindow->setFont(strFont.c_str());
	}

	std::string GetFont()
	{
		if(m_pWindow)
			return m_pWindow->getFont()->getName().c_str();

		return "";
	}

	// not sure about this one, should have two Vector2's (one relative, one absolute?)
	void SetPosition(Vector2 vecPosition, bool bRelative = false)
	{
		if(m_pWindow)
		{
			CEGUI::UDim x((bRelative ? vecPosition.fX : 0.0f), (bRelative ? 0.0f : vecPosition.fX));
			CEGUI::UDim y((bRelative ? vecPosition.fY : 0.0f), (bRelative ? 0.0f : vecPosition.fY));
			m_pWindow->setPosition(CEGUI::UVector2(x, y));
		}
	}

	// not sure about this one, return two Vector2's (one relative, one absolute)
	Vector2 GetPosition(bool bRelative = false)
	{
		if(m_pWindow)
		{
			CEGUI::Vector2 vecPosition = (bRelative ? m_pWindow->getPosition().asRelative(m_pWindow->getParentPixelSize()) : m_pWindow->getPosition().asAbsolute(m_pWindow->getParentPixelSize()));
			return Vector2(vecPosition.d_x, vecPosition.d_y);
		}
	}

	void SetProperty(std::string strProperty, std::string strValue)
	{
		if(m_pWindow)
			m_pWindow->setProperty(strProperty.c_str(), strValue.c_str());
	}

	std::string GetProperty(std::string strProperty)
	{
		if(m_pWindow)
			return m_pWindow->getProperty(strProperty.c_str()).c_str();

		return "";
	}
};
