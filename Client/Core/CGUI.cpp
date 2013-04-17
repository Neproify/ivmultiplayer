//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGUI.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CGUI.h"
#include <ShlObj.h>
#include <CLogFile.h>
#include "CGame.h"
#include "SharedUtility.h"
#include "CGraphics.h"
#include "CSettings.h"

// TODO: Make CGUI message box members
bool m_bMessageBoxHideCursor = false;
bool m_bCursorChangedForMessageBox = false;
bool m_bInputStateChangedForMessageBox = false;

CGUI::CGUI(IDirect3DDevice9 * pD3DDevice)
	: m_pD3DDevice(pD3DDevice),
	m_pRenderer(NULL),
	m_pSystem(NULL),
	m_pCursor(NULL),
	m_pWindowManager(NULL),
	m_pDefaultWindow(NULL),
	m_pInput(NULL),
	m_pInputMouse(NULL),
	m_bInitialized(false),
	m_iCurrentId(0),
	m_dwDoubleClickTime(GetDoubleClickTime()),
	m_uiCurrentKyeFag(0)
{
	memset(&m_messageBox, 0, sizeof(m_messageBox));
	memset(dwLastClickTime, 0, sizeof(DWORD) * 8);
	memset(byteButtonWasClicked, 0, sizeof(BYTE) * 8);
	memset(byteButtonClicked, 0, sizeof(BYTE) * 8);
	memset(&m_rCursorPosition, 0, sizeof(RECT));
	memset(&m_clickPosition, 0, sizeof(POINT));
}

CGUI::~CGUI()
{
	// Destroy the message box
	HideMessageBox();

	// Mark ourselves as uninitialized so functions don't work
	m_bInitialized = false;

	// Shutdown the input device
	if(m_pInputMouse)
	{
		m_pInputMouse->Unacquire();
		m_pInputMouse->Release();
	}

	// Shutdown the input
	if(m_pInput)
		m_pInput->Release();

	// Destroy the text drawing geometry buffer
	m_pRenderer->destroyGeometryBuffer(*m_pTextDrawingGeometryBuffer);

	// Destroy the GUI fonts
	CEGUI::FontManager::getSingleton().destroyAll();

	// Destroy the default GUI window
	m_pWindowManager->destroyWindow(m_pDefaultWindow);

	// Destroy the GUI scheme
	CEGUI::SchemeManager::getSingleton().destroy(*m_pScheme);

	// Destroy the GUI system
	m_pSystem->destroy();

	// Destroy the GUI renderer
	m_pRenderer->destroy(*m_pRenderer);
}

char * CGUI::GetUniqueName()
{
	static char szKyeFag[20];
	sprintf(szKyeFag, "window%d", m_uiCurrentKyeFag);
	m_uiCurrentKyeFag++;
	return szKyeFag;
}

bool CGUI::Initialize()
{
	// Make sure we are not initialized
	if(!m_bInitialized)
	{
		// Create the GUI renderer
		m_pRenderer = &CEGUI::Direct3D9Renderer::create(m_pD3DDevice);

		// Create the GUI system
		m_pSystem = &CEGUI::System::create(*m_pRenderer);

		// Get a pointer to the GUI logger
		CEGUI::Logger * pLogger = CEGUI::Logger::getSingletonPtr();

		// Set the GUI log file name
		pLogger->setLogFilename(SharedUtility::GetAbsolutePath("CEGUI.log").Get());

		// Set the GUI log file level
#ifdef IVMP_DEBUG
		pLogger->setLoggingLevel(CEGUI::Informative);
#else
		pLogger->setLoggingLevel(CEGUI::Informative);
#endif

		// Get a pointer to the GUI window manager
		m_pWindowManager = CEGUI::WindowManager::getSingletonPtr();

		// Get a pointer to the GUI font manager
		m_pFontManager = CEGUI::FontManager::getSingletonPtr();

		// Initialize the required directories for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider * rp = (CEGUI::DefaultResourceProvider *)m_pSystem->getResourceProvider();

		// For each resource type, set a resource group directory
		rp->setResourceGroupDirectory("schemes", SharedUtility::GetAbsolutePath("CEGUI/schemes/").Get());
		rp->setResourceGroupDirectory("imagesets", SharedUtility::GetAbsolutePath("CEGUI/imagesets/").Get());
		rp->setResourceGroupDirectory("fonts", SharedUtility::GetAbsolutePath("CEGUI/fonts/").Get());
		rp->setResourceGroupDirectory("layouts", SharedUtility::GetAbsolutePath("CEGUI/layouts/").Get());
		rp->setResourceGroupDirectory("looknfeels", SharedUtility::GetAbsolutePath("CEGUI/looknfeel/").Get());
		rp->setResourceGroupDirectory("schemas", SharedUtility::GetAbsolutePath("CEGUI/xml_schemas/").Get());
		rp->setResourceGroupDirectory("resources", SharedUtility::GetAbsolutePath("clientfiles/resources/").Get());

		// Register the windows fonts directory as a resource group directory
		char strFontPath[MAX_PATH];

		if(SHGetSpecialFolderPath(NULL, strFontPath, CSIDL_FONTS, FALSE))
			rp->setResourceGroupDirectory("winfonts", strFontPath);

		// Set the default resource groups to be used
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		m_pWindowManager->setDefaultResourceGroup("layouts");

		// Setup default group for validation schemas
		CEGUI::XMLParser * parser = m_pSystem->getXMLParser();

		if(parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");

		// Clearing this queue actually makes sure it's created
		//m_pRenderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

		// Load the gui scheme
		m_pScheme = &CEGUI::SchemeManager::getSingleton().create(STYLE_SCHEME);

		// Set the mouse cursor
		m_pSystem->setDefaultMouseCursor(STYLE_IMAGES, "MouseArrow");

		// Get a pointer to the gui cursor
		m_pCursor = CEGUI::MouseCursor::getSingletonPtr();

		// Hide the mouse cursor
		m_pCursor->setVisible(false);

		// Create the default gui window
		m_pDefaultWindow = (CEGUI::DefaultWindow *)m_pWindowManager->createWindow("DefaultWindow", GetUniqueName());

		// Set the default gui window size
		m_pDefaultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

		// Set the default gui window alpha
		m_pDefaultWindow->setAlpha(1.0f);

		// Subscribe to our global key down event
		CEGUI::GlobalEventSet::getSingletonPtr()->subscribeEvent(CEGUI::Window::EventNamespace + "/" + CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&CGUI::OnGUIKeyDown, this));

		// Set the default gui window as the gui sheet
		m_pSystem->setGUISheet(m_pDefaultWindow);

		// Setup text drawing geometry buffer
		m_pTextDrawingGeometryBuffer = &m_pRenderer->createGeometryBuffer();
		m_pTextDrawingGeometryBuffer->setClippingRegion(CEGUI::Rect(CEGUI::Vector2(0, 0), m_pRenderer->getDisplaySize()));

		// Set the default GUI font
		m_pSystem->setDefaultFont(GetFont(CVAR_GET_STRING("chatfont").Get()));

		// Create the input
		if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID *)&m_pInput, NULL)))
			return false;

		// Create the input device
		if(FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pInputMouse, NULL)))
			return false;

		// Set the input device as a mouse
		if(FAILED(m_pInputMouse->SetDataFormat(&c_dfDIMouse2)))
			return false;

		// Set the input device to non exclusive
		if(FAILED(m_pInputMouse->SetCooperativeLevel(CGame::GetHWnd(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			return false;

		// Acquire the input device
		if(FAILED(m_pInputMouse->Acquire()))
			return false;

		// Get the input device info
		DIDEVCAPS MouseCapabilities;
		MouseCapabilities.dwSize = sizeof(MouseCapabilities);

		if(FAILED(m_pInputMouse->GetCapabilities(&MouseCapabilities)))
			return false;

		m_dwInputAxes = MouseCapabilities.dwAxes;
		m_dwInputButtons = MouseCapabilities.dwButtons;

		// Initialize the mouse button info
		for(BYTE i = 0; i < 8; i++)
		{
			dwLastClickTime[i] = 0;
			byteButtonWasClicked[i] = 0;
			byteButtonClicked[i] = 0;
		}

		m_bInitialized = true;
		return true;
	}
	return false;
}

unsigned int CGUI::GetDisplayWidth()
{
	// If we have a renderer get the display width
	if(m_pRenderer)
		return (unsigned int)m_pRenderer->getDisplaySize().d_width;

	return 0;
}

unsigned int CGUI::GetDisplayHeight()
{
	// If we have a renderer get the display height
	if(m_pRenderer)
		return (unsigned int)m_pRenderer->getDisplaySize().d_height;

	return 0;
}

// TODO: Use the game's mouse dinput device not our own?
void CGUI::Render()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Render our GUI
		if(m_pSystem)
			m_pSystem->renderGUI();

		// Process our mouse events
		if(CGame::IsFocused())
		{
			if(!m_pInput || !m_pInputMouse)
				return;

			if(m_pCursor->isVisible())
			{
				// If we have lost the input device re-acquire it
				if(m_pInputMouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState) == DIERR_INPUTLOST)
				{
					if(FAILED(m_pInputMouse->Acquire()))
						return;
				}

				// Handle mouse position changes
				m_rCursorPosition.left += m_MouseState.lX;

				if(m_rCursorPosition.left > (long)GetDisplayWidth())
					m_rCursorPosition.left = (long)GetDisplayWidth();
				else if(m_rCursorPosition.left < 0)
					m_rCursorPosition.left = 0;

				m_rCursorPosition.top += m_MouseState.lY;

				if(m_rCursorPosition.top > (long)GetDisplayHeight())
					m_rCursorPosition.top = (long)GetDisplayHeight();
				else if(m_rCursorPosition.top < 0)
					m_rCursorPosition.top = 0;

				m_pSystem->injectMousePosition((float)m_rCursorPosition.left, (float)m_rCursorPosition.top);

				// If the window has focus sync the windows cursor position with 
				// the direct input cursor position
				if(CGame::IsFocused())
					SetCursorPos(m_rCursorPosition.left, m_rCursorPosition.top);

				// Check the mouse wheel for changes
				if(m_MouseState.lZ != 0)
					m_pSystem->injectMouseWheelChange((float)m_MouseState.lZ);

				// Check the mouse buttons for changes
				// TODO: Fix double click detection
				// TODO: First click after a double click should be a single click
				for(BYTE i = 0; i < 8; i++)
				{
					if(m_MouseState.rgbButtons[i] & 0x80)
					{
						if(!byteButtonClicked[i])
						{
							bool bDoubleClick = false;

							if(dwLastClickTime[i])
							{
								if((SharedUtility::GetTime() - dwLastClickTime[i]) < m_dwDoubleClickTime)
								{
									if(m_clickPosition.x == m_rCursorPosition.left && m_clickPosition.y == m_rCursorPosition.top)
									{
										if(i < 3)
										{
											m_pSystem->injectMouseButtonDoubleClick((CEGUI::MouseButton)i);
											bDoubleClick = true;
										}
									}
								}
							}

							if(!bDoubleClick)
							{
								if(i < 3)
									m_pSystem->injectMouseButtonDown((CEGUI::MouseButton)i);
							}

							dwLastClickTime[i] = SharedUtility::GetTime();
							byteButtonClicked[i] = 1;
						}
					}
					else
					{
						if(byteButtonClicked[i])
						{
							if(i < 3)
								m_pSystem->injectMouseButtonUp((CEGUI::MouseButton)i);
						}

						byteButtonClicked[i] = 0;
					}
				}
			}
		}
	}
}

// Custom replacement for CEGUI::Font::drawText
void CGUI::DrawText(String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, CEGUI::Font * pFont, bool bProcessFormatting, bool bAllowColorFormatting, CEGUI::Rect * rClipRect, float fSpaceExtra, float fXScale, float fYScale)
{
	if(m_bInitialized)
	{
		// Get the font pointer
		CEGUI::Font * pTextFont = pFont;

		// If we have no valid font use the default one
		if(!pTextFont)
			pTextFont = GetFont(CVAR_GET_STRING("chatfont").Get());

		// Do we have a valid font?
		if(pTextFont)
		{
			// Reset the text geometry buffer
			m_pTextDrawingGeometryBuffer->reset();

			const float fBaseY = (vecPosition.d_y + pTextFont->getBaseline(fYScale));
			CEGUI::Vector2 vecGlyphPos(vecPosition);
			float fOtherY = 0;
			CEGUI::ColourRect rColourRect = rColorRect;

			// Temporary strings for unicode conversion
			unsigned char ucAnsi = 0;
			WCHAR wcUnicode = 0;

			// Loop through all characters
			unsigned int uiTextLength = sText.GetLength();

			for(unsigned int c = 0; c < uiTextLength; c++)
			{
				// Set the current character in our ANSI string
				ucAnsi = sText[c];

				// Convert the current character to unicode
				SharedUtility::AnsiToUnicode((const char *)&ucAnsi, 1, &wcUnicode, 1);

				// Check for font formatting
				if(bProcessFormatting)
				{
					// Check for newline constant
					if(sText[c] == '\n')
					{
						vecGlyphPos.d_x = vecPosition.d_x;
						fOtherY += pTextFont->getFontHeight();
						continue;
					}

					// Check for color formatting
					else if(uiTextLength >= 10 && (sText[c] == '[' && sText[c + 9] == ']'))
					{
						bool bValid = true;
						CEGUI::String sColour;

						// Loop through all color chars
						for(size_t i = 0; i < 8; i++)
						{
							unsigned char cChar = sText[(c + i) + 1];

							// Make sure its 0-99, A-F or a-f
							if((cChar < '0' || cChar > '9') && (cChar < 'A' || cChar > 'F') && 
								(cChar < 'a' || cChar > 'f'))
							{
								// char is invalid
								bValid = false;
								break;
							}

							// Add the char to the color string
							sColour += cChar;
						}

						// Set the color if its valid
						if(bValid)
						{
							// Is color formatting allowed?
							if(bAllowColorFormatting)
							{
								// Set the color
								rColourRect.setColours((CEGUI::colour(strtoul(sColour.c_str(), NULL, 16)) >> 8) | 0xFF000000);
							}

							// Increment the current char by 9
							c += 9;
							continue;
						}
					}
				}

				// Attempt to get the glyph data
				const CEGUI::FontGlyph * glyph = pTextFont->getGlyphData((unsigned long)wcUnicode);

				// Do we have valid glyph data?
				if(glyph)
				{
					// Get the glyph image
					const CEGUI::Image * const img = glyph->getImage();

					// Calculate the glyph y position
					vecGlyphPos.d_y = (fBaseY - (img->getOffsetY() - img->getOffsetY() * fYScale) + fOtherY);

					// Draw the glyph
					img->draw(*m_pTextDrawingGeometryBuffer, vecGlyphPos, glyph->getSize(fXScale, fYScale), rClipRect, rColourRect);

					// Increment the x position
					vecGlyphPos.d_x += glyph->getAdvance(fXScale);

					// Apply extra spacing to space chars
					if(sText[c] == ' ')
						vecGlyphPos.d_x += fSpaceExtra;
				}
			}

			// Draw the text geometry buffer
			m_pTextDrawingGeometryBuffer->draw();
		}
	}
}

void CGUI::DrawText(String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, String strFontName, bool bProcessFormatting, bool bAllowColorFormatting, CEGUI::Rect * rClipRect, float fSpaceExtra, float fXScale, float fYScale)
{
	DrawText(sText, vecPosition, rColorRect, GetFont(strFontName), bProcessFormatting, bAllowColorFormatting, rClipRect, fSpaceExtra, fXScale, fYScale);
}

bool CGUI::OnMessageBoxClick(const CEGUI::EventArgs &eventArgs)
{
	// Get the window
	CEGUI::Window * pWindow = ((const CEGUI::WindowEventArgs&)eventArgs).window;

	// Get the response
	eGUIMessageBoxResponse response = GUI_MESSAGEBOX_YES;

	if(pWindow == m_messageBox.pButtons[0])
		response = GUI_MESSAGEBOX_YES;
	else if(pWindow == m_messageBox.pButtons[1])
		response = GUI_MESSAGEBOX_NO;
	else if(pWindow == m_messageBox.pButtons[2])
		response = GUI_MESSAGEBOX_CANCEL;

	// Do we have a handler?
	if(m_messageBox.pfnHandler)
	{
		// Call the handler
		m_messageBox.pfnHandler(response);
	}

	// Hide the message box
	HideMessageBox();
	return true;
}

void CGUI::ShowMessageBox(const CEGUI::String &sText, const CEGUI::String &sTitle, eGUIMessageBoxType style, GUIMessageBoxHandler_t pfnHandler)
{
	// Are we initialized?
	if(m_bInitialized)
	{
		// Hide the current message box (if any)
		HideMessageBox();

		// Create the message box window

		// IF any messagebox already exist, destroy it
		if(m_messageBox.pWindow)
			m_messageBox.pWindow->destroy();

		m_messageBox.pWindow = CreateGUIFrameWindow();
		((CEGUI::FrameWindow *)m_messageBox.pWindow)->setCloseButtonEnabled(false);

		m_messageBox.pWindow->setText(sTitle);
		m_messageBox.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.2f, 0)));
		m_messageBox.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.4f, 0)));
		m_messageBox.pWindow->setAlwaysOnTop(true);
		m_messageBox.pWindow->setVisible(true);
		//m_messageBox.pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxOkClick, this));

		// Create the message box window text
		m_messageBox.pText = CreateGUIStaticText(m_messageBox.pWindow);
		m_messageBox.pText->setText(sText);
		m_messageBox.pText->setSize(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.6f, 0)));
		m_messageBox.pText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.01f, 0)));
		m_messageBox.pText->setProperty("FrameEnabled", "false");
		m_messageBox.pText->setProperty("BackgroundEnabled", "false");
		m_messageBox.pText->setVisible(true);

		// Create the message box window button(s)
		if(style == GUI_MESSAGEBOXTYPE_OK)
		{
			// Create the 'Ok' button
			m_messageBox.pButtons[0] = CreateGUIButton(m_messageBox.pWindow);
			m_messageBox.pButtons[0]->setText("Ok");
			m_messageBox.pButtons[0]->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.15f, 0)));
			m_messageBox.pButtons[0]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.75f, 0)));
			m_messageBox.pButtons[0]->setVisible(true);
			m_messageBox.pButtons[0]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxClick, this));
		}
		else if(style == GUI_MESSAGEBOXTYPE_YESNO || style == GUI_MESSAGEBOXTYPE_YESNOCANCEL)
		{
			// Create the 'Yes' button
			m_messageBox.pButtons[0] = CreateGUIButton(m_messageBox.pWindow);
			m_messageBox.pButtons[0]->setText("Yes");

			if(style == GUI_MESSAGEBOXTYPE_YESNO)
			{
				m_messageBox.pButtons[0]->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.15f, 0)));
				m_messageBox.pButtons[0]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.75f, 0)));
			}
			else if(style == GUI_MESSAGEBOXTYPE_YESNOCANCEL)
			{
				m_messageBox.pButtons[0]->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.15f, 0)));
				m_messageBox.pButtons[0]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.75f, 0)));
			}

			m_messageBox.pButtons[0]->setVisible(true);
			m_messageBox.pButtons[0]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxClick, this));

			// Create the 'No' button
			m_messageBox.pButtons[1] = CreateGUIButton(m_messageBox.pWindow);
			m_messageBox.pButtons[1]->setText("No");

			if(style == GUI_MESSAGEBOXTYPE_YESNO)
			{
				m_messageBox.pButtons[1]->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.15f, 0)));
				m_messageBox.pButtons[1]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.75f, 0)));
			}
			else if(style == GUI_MESSAGEBOXTYPE_YESNOCANCEL)
			{
				m_messageBox.pButtons[1]->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.15f, 0)));
				m_messageBox.pButtons[1]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0), CEGUI::UDim(0.75f, 0)));
			}

			m_messageBox.pButtons[1]->setVisible(true);
			m_messageBox.pButtons[1]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxClick, this));

			if(style == GUI_MESSAGEBOXTYPE_YESNOCANCEL)
			{
				// Create the 'Cancel' button
				m_messageBox.pButtons[2] = CreateGUIButton(m_messageBox.pWindow);
				m_messageBox.pButtons[2]->setText("Cancel");
				m_messageBox.pButtons[2]->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.15f, 0)));
				m_messageBox.pButtons[2]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.75f, 0)));
				m_messageBox.pButtons[2]->setVisible(true);
				m_messageBox.pButtons[2]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxClick, this));
			}
		}

		// Set the handler
		m_messageBox.pfnHandler = pfnHandler;

		// Show the cursor if needed
		if(!m_pCursor->isVisible())
		{
			m_bMessageBoxHideCursor = true;
			m_bCursorChangedForMessageBox = false;
			m_pCursor->setVisible(true);
		}

		// Set the input state if needed
		if(CGame::GetInputState())
		{
			m_bInputStateChangedForMessageBox = true;
			CGame::SetInputState(false);
		}
	}
}

void CGUI::HideMessageBox()
{
	// Are we initialized?
	if(m_bInitialized)
	{
		// Loop through all message box buttons
		for(int i = 0; i < ARRAY_LENGTH(m_messageBox.pButtons); i++)
		{
			// Does this button exist?
			if(m_messageBox.pButtons[i])
			{
				// Remove this button
				RemoveGUIWindow(m_messageBox.pButtons[i]);
				m_messageBox.pButtons[i] = NULL;
			}
		}

		// Does the message box text exist?
		if(m_messageBox.pText)
		{
			// Remove the message box text
			RemoveGUIWindow(m_messageBox.pText);
			m_messageBox.pText = NULL;
		}

		// Does the message box window exist?
		if(m_messageBox.pWindow)
		{
			// Remove the message box window
			RemoveGUIWindow(m_messageBox.pWindow);
			m_messageBox.pWindow = NULL;
		}

		// Reset the handler
		m_messageBox.pfnHandler = NULL;

		// Restore the cursor state if needed
		if(!m_bCursorChangedForMessageBox)
		{
			if(m_bMessageBoxHideCursor)
			{
				m_bMessageBoxHideCursor = false;
				m_pCursor->setVisible(false);
			}
		}

		// Restore the input state if needed
		if(m_bInputStateChangedForMessageBox)
		{
			m_bInputStateChangedForMessageBox = false;
			CGame::SetInputState(true);
		}
	}
}

CEGUI::Window * CGUI::CreateGUIWindow(const CEGUI::String &sType, const CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	if(m_bInitialized)
	{
		CEGUI::Window * pWindow = m_pWindowManager->createWindow(sType, sName);
		if(pWindow)
		{
			CEGUI::Window * pParent = (pParentWindow == NULL) ? m_pDefaultWindow : pParentWindow;
			pParent->addChildWindow(pWindow);
		}
		return pWindow;
	}
	return NULL;
}

void CGUI::RemoveGUIWindow(CEGUI::String &sName)
{
	if(m_bInitialized)
	{
		m_pDefaultWindow->removeChildWindow(sName);
		m_pWindowManager->destroyWindow(sName);
	}
}

void CGUI::RemoveGUIWindow(CEGUI::Window * pWindow)
{
	if(m_bInitialized)
	{
		// Make sure the window exists
		if(m_pWindowManager->isWindowPresent(pWindow->getName()))
		{
			// Remove the window from the parent window
			pWindow->getParent()->removeChildWindow(pWindow);

			// Destroy the window
			m_pWindowManager->destroyWindow(pWindow);

			// Reset the window pointer
			pWindow = NULL;
		}
	}
}

std::string CGUIWindow::getText()
{
	CEGUI::String str = CEGUI::Window::getText();
	int len = str.length();

	if(len == 0)
		return std::string();

	char *Ansi = new char[str.length() + 1];
	wchar_t *Unicode = new wchar_t[str.length() + 1];

	for(size_t i = 0; i < str.size(); i++)
		Unicode[i] = str[i];

	WideCharToMultiByte(CP_ACP, NULL, Unicode, -1, Ansi, len, NULL, NULL);

	Ansi[str.size()] = 0;
	std::string out = Ansi;
	delete[] Ansi;
	delete[] Unicode;

	return out;
}

void CGUI::RemoveGUIWindow(CGUITitleBar * pTitleBar)
{
	RemoveGUIWindow((CEGUI::Window *)pTitleBar);
}

void CGUI::RemoveGUIWindow(CGUIButton * pButton)
{
	RemoveGUIWindow((CEGUI::Window *)pButton);
}

void CGUI::RemoveGUIWindow(CGUIFrameWindow * pFrameWindow)
{
	RemoveGUIWindow((CEGUI::Window *)pFrameWindow);
}

void CGUI::RemoveGUIWindow(CGUIEditBox * pEditBox)
{
	RemoveGUIWindow((CEGUI::Window *)pEditBox);
}

void CGUI::RemoveGUIWindow(CGUIMultiLineEditBox * pMultiLineEditBox)
{
	RemoveGUIWindow((CEGUI::Window *)pMultiLineEditBox);
}

void CGUI::RemoveGUIWindow(CGUIStaticImage * pStaticText)
{
	RemoveGUIWindow((CEGUI::Window *)pStaticText);
}

void CGUI::RemoveGUIWindow(CGUIStaticText * pStaticText)
{
	RemoveGUIWindow((CEGUI::Window *)pStaticText);
}

void CGUI::RemoveGUIWindow(CGUICheckBox * pCheckBox)
{
	RemoveGUIWindow((CEGUI::Window *)pCheckBox);
}

void CGUI::RemoveGUIWindow(CGUIProgressBar * pProgressBar)
{
	RemoveGUIWindow((CEGUI::Window *)pProgressBar);
}

CGUITitleBar * CGUI::CreateGUITitleBar(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUITitleBar *)CreateGUIWindow(STYLE_PREFIX "/Titlebar", sName, pParentWindow);
}

CGUITitleBar * CGUI::CreateGUITitleBar(CEGUI::Window * pParentWindow)
{
	return (CGUITitleBar *)CreateGUIWindow(STYLE_PREFIX "/Titlebar", GetUniqueName(), pParentWindow);
}

CGUIButton * CGUI::CreateGUIButton(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIButton *)CreateGUIWindow(STYLE_PREFIX "/Button", sName, pParentWindow);
}

CGUIButton * CGUI::CreateGUIButton(CEGUI::Window * pParentWindow)
{
	return (CGUIButton *)CreateGUIWindow(STYLE_PREFIX "/Button", GetUniqueName(), pParentWindow);
}

CGUIFrameWindow * CGUI::CreateGUIFrameWindow(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIFrameWindow *)CreateGUIWindow(STYLE_PREFIX "/FrameWindow", sName, pParentWindow);
}

CGUIFrameWindow * CGUI::CreateGUIFrameWindow(CEGUI::Window * pParentWindow)
{
	return (CGUIFrameWindow *)CreateGUIWindow(STYLE_PREFIX "/FrameWindow", GetUniqueName(), pParentWindow);
}

CGUIEditBox * CGUI::CreateGUIEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIEditBox *)CreateGUIWindow(STYLE_PREFIX "/Editbox", sName, pParentWindow);
}

CGUIMultiLineEditBox * CGUI::CreateGUIMultiLineEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIMultiLineEditBox *)CreateGUIWindow(STYLE_PREFIX "/MultiLineEditbox", sName, pParentWindow);
}

CGUIEditBox * CGUI::CreateGUIEditBox(CEGUI::Window * pParentWindow)
{
	return (CGUIEditBox *)CreateGUIWindow(STYLE_PREFIX "/Editbox", GetUniqueName(), pParentWindow);
}

CGUIMultiLineEditBox * CGUI::CreateGUIMultiLineEditBox(CEGUI::Window * pParentWindow)
{
	return (CGUIMultiLineEditBox *)CreateGUIWindow(STYLE_PREFIX "/MultiLineEditbox", GetUniqueName(), pParentWindow);
}

CGUIStaticImage * CGUI::CreateGUIStaticImage(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIStaticImage *)CreateGUIWindow(STYLE_PREFIX "/StaticImage", sName, pParentWindow);
}

CGUIStaticImage * CGUI::CreateGUIStaticImage(CEGUI::Window * pParentWindow)
{
	return (CGUIStaticImage *)CreateGUIWindow(STYLE_PREFIX "/StaticImage", GetUniqueName(), pParentWindow);
}

CGUIStaticText * CGUI::CreateGUIStaticText(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIStaticText *)CreateGUIWindow(STYLE_PREFIX "/StaticText", sName, pParentWindow);
}

CGUIStaticText * CGUI::CreateGUIStaticText(CEGUI::Window * pParentWindow)
{
	return (CGUIStaticText *)CreateGUIWindow(STYLE_PREFIX "/StaticText", GetUniqueName(), pParentWindow);
}

CGUICheckBox * CGUI::CreateGUICheckBox(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUICheckBox *)CreateGUIWindow(STYLE_PREFIX "/Checkbox", sName, pParentWindow);
}

CGUICheckBox * CGUI::CreateGUICheckBox(CEGUI::Window * pParentWindow)
{
	return (CGUICheckBox *)CreateGUIWindow(STYLE_PREFIX "/Checkbox", GetUniqueName(), pParentWindow);
}

CGUIMultiColumnList * CGUI::CreateGUIMultiColumnList(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIMultiColumnList *)CreateGUIWindow(STYLE_PREFIX "/MultiColumnList", sName, pParentWindow);
}

CGUIMultiColumnList * CGUI::CreateGUIMultiColumnList(CEGUI::Window * pParentWindow)
{
	return (CGUIMultiColumnList *)CreateGUIWindow(STYLE_PREFIX "/MultiColumnList", GetUniqueName(), pParentWindow);
}

CGUITabControl * CGUI::CreateGUITabControl(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUITabControl *)CreateGUIWindow(STYLE_PREFIX "/TabControl", sName, pParentWindow);
}

CGUITabControl * CGUI::CreateGUITabControl(CEGUI::Window * pParentWindow)
{
	return (CGUITabControl *)CreateGUIWindow(STYLE_PREFIX "/TabControl", GetUniqueName(), pParentWindow);
}

CGUITabContentPane * CGUI::CreateGUITabContentPane(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUITabContentPane *)CreateGUIWindow(STYLE_PREFIX "/TabContentPane", sName, pParentWindow);
}

CGUITabContentPane * CGUI::CreateGUITabContentPane(CEGUI::Window * pParentWindow)
{
	return (CGUITabContentPane *)CreateGUIWindow(STYLE_PREFIX "/TabContentPane", GetUniqueName(), pParentWindow);
}

CGUIProgressBar * CGUI::CreateGUIProgressBar(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIProgressBar *)CreateGUIWindow(STYLE_PREFIX "/ProgressBar", sName, pParentWindow);
}

CGUIProgressBar * CGUI::CreateGUIProgressBar(CEGUI::Window * pParentWindow)
{
	return (CGUIProgressBar *)CreateGUIWindow(STYLE_PREFIX "/ProgressBar", GetUniqueName(), pParentWindow);
}

void CGUI::SetCursorVisible(bool bVisible)
{
	if(m_bInitialized)
	{
		if(!bVisible && m_bScriptedCursorVisible)
		{
			// We do not hide cursor if some script shown it.
			return;
		}

		m_pCursor->setVisible(bVisible);
		m_bCursorChangedForMessageBox = true;
	}
}

void CGUI::SetScriptedCursorVisible(bool bVisible)
{
	// Set scripted cursor visible state
	m_bScriptedCursorVisible = bVisible;
}

void CGUI::SetCursorPosition(float posX, float posY)
{
	if(m_bInitialized) {
		CEGUI::Point cursorPoint;
		cursorPoint.d_x = posX; cursorPoint.d_y = posY;
		m_pCursor->setPosition(cursorPoint);
	}
}

RECT CGUI::GetCursorPosition()
{
	if(m_bInitialized)
		return this->m_rCursorPosition;

	return RECT();
}

bool CGUI::IsCursorVisible(bool bVisible)
{
	if(m_bInitialized)
		return m_pCursor->isVisible();

	return false;
}

void CGUI::OnLostDevice()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Notify our GUI renderer that the device has been lost
		if(m_pRenderer)
			m_pRenderer->preD3DReset();
	}
}

void CGUI::OnResetDevice()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Notify our GUI renderer that the device has been reset
		if(m_pRenderer)
			m_pRenderer->postD3DReset();
	}
}

DWORD ScanCodeToDIK(WORD wScanCode)
{
	switch(wScanCode)
	{
	case 0x41: return DIK_A;               // Control + A (Select All)
	case 0x43: return DIK_C;               // Control + C (Copy)
	case 0x58: return DIK_X;               // Control + X (Cut)
	case 0x56: return DIK_V;               // Control + V (Paste)
	case VK_TAB: return DIK_TAB;           // Tab
	case VK_HOME: return DIK_HOME;         // Home
	case VK_END: return DIK_END;           // End
	case VK_INSERT: return DIK_INSERT;     // Insert
	case VK_DELETE: return DIK_DELETE;     // Delete
	case VK_UP: return DIK_UP;             // Arrow Up
	case VK_DOWN: return DIK_DOWN;         // Arrow Down
	case VK_LEFT: return DIK_LEFT;         // Arrow Left
	case VK_RIGHT: return DIK_RIGHT;       // Arrow Right
	case VK_SHIFT: return DIK_LSHIFT;      // Shift
	case VK_LSHIFT: return DIK_LSHIFT;     // Left Shift
	case VK_RSHIFT: return DIK_RSHIFT;     // Right Shift
	case VK_BACK: return DIK_BACK;         // Backspace
	case VK_RETURN: return DIK_RETURN;     // Enter
	case VK_CONTROL: return DIK_LCONTROL;  // Control
	case VK_LCONTROL: return DIK_LCONTROL; // Left Control
	case VK_RCONTROL: return DIK_RCONTROL; // Right Control
	}

	return 0;
}

bool CGUI::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Are we initialized?
	if(m_bInitialized)
	{
		// Is this a mouse event?
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
			{
				// We handle the mouse manually with direct input
				return false;
			}
		}

		// Is it a char?
		if(uMsg == WM_CHAR)
		{
			// Temporary strings for unicode conversion
			unsigned char ucAnsi = wParam;
			WCHAR wcUnicode = 0;
			SharedUtility::AnsiToUnicode((const char *)&ucAnsi, 1, &wcUnicode, 1);

			if(m_pSystem->injectChar((CEGUI::utf32)wcUnicode))
				return true;
		}

		// Is it a key down or key up?
		else if(uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		{
           // For some reason when you press ALT it sends 3 messages: (WM_KEYDOWN VK_CONTROL), (WM_KEYDOWN VK_MENU) and (WM_KEYUP VK_MENU)
           // This caused that after ALT has been pressed CEGUI thought control(VK_CONTROL) is always down
            if(wParam == VK_MENU && uMsg == WM_KEYDOWN)
				return m_pSystem->injectKeyUp(DIK_LCONTROL);

			// Convert it to a CEGUI scan code
			DWORD dwKey = ScanCodeToDIK(wParam);

			// Do we have a valid scan code?
			if(dwKey)
			{
				// Pass it to the GUI
				if(uMsg == WM_KEYDOWN)
				{
					if(m_pSystem->injectKeyDown(dwKey))
						return true;
				}
				else
				{
					if(m_pSystem->injectKeyUp(dwKey))
						return true;
				}
			}
		}
	}

	// Not handled
	return false;
}

bool CGUI::OnGUIKeyDown(const CEGUI::EventArgs &eventArgs)
{
	// Cast the argument to a key event args structure
	const CEGUI::KeyEventArgs &keyEventArgs = (const CEGUI::KeyEventArgs &)eventArgs;

	// Check if the left or right control key is held down
	if(keyEventArgs.sysKeys & CEGUI::Control)
	{
		// Check if its the a key that was pressed
		if(keyEventArgs.scancode == CEGUI::Key::A)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
			{
				// Get the edit box pointer
				CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;

				// Set the selection start to 0 and end to the edit box text length
				pEditBox->setSelection(0, pEditBox->getText().size());
			} 
			// If its an multi line edit box that is selected
			else if(keyEventArgs.window->getType() == STYLE_PREFIX "/MultiLineEditbox")
			{
				// Get the multi line edit box pointer
				CEGUI::MultiLineEditbox * pMultiLineEditBox = (CEGUI::MultiLineEditbox *)keyEventArgs.window;

				// Set the selection start to 0 and end to the multi line edit box text length
				pMultiLineEditBox->setSelection(0, pMultiLineEditBox->getText().size());
			} 
		}
		// Check if its the c or x key that was pressed
		else if(keyEventArgs.scancode == CEGUI::Key::C || keyEventArgs.scancode == CEGUI::Key::X)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox" || keyEventArgs.window->getType() == STYLE_PREFIX "/MultiLineEditbox")
			{
				// Get the edit box pointer
				if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
				{
					CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;
					// Get the edit box selection length
					size_t sSelectionLength = pEditBox->getSelectionLength();
					if(sSelectionLength > 0)
					{
						// Get the edit box selection start index
						size_t sSelectionStartIndex = pEditBox->getSelectionStartIndex();

						// Get the edit box selection end index
						size_t sSelectionEndIndex = (sSelectionStartIndex + sSelectionLength);

						// Get the edit box text
						CEGUI::String sEditBoxText = pEditBox->getText();

						// Get the text we wish to copy from the edit box text
						CEGUI::String sSelectionText = sEditBoxText.substr(sSelectionStartIndex, sSelectionEndIndex);

						// Set the clipboard text
						SharedUtility::SetClipboardText(sSelectionText.c_str(), (sSelectionText.length() + 1));

						// If its the control + x key cut the selection from the edit box text
						if(keyEventArgs.scancode == CEGUI::Key::X)
						{
							// Cut the text from the edit box text
							sEditBoxText.replace(sSelectionStartIndex, sSelectionEndIndex, "");

							// Set the edit box text to the new cut text
							pEditBox->setText(sEditBoxText);
						}
					}
				} else { // If its a multi line edit box
					CEGUI::MultiLineEditbox * pMultiLineEditBox = (CEGUI::MultiLineEditbox *)keyEventArgs.window;
					// Get the edit box selection length
					size_t sSelectionLength = pMultiLineEditBox->getSelectionLength();
					if(sSelectionLength > 0)
					{
						// Get the edit box selection start index
						size_t sSelectionStartIndex = pMultiLineEditBox->getSelectionStartIndex();

						// Get the edit box selection end index
						size_t sSelectionEndIndex = (sSelectionStartIndex + sSelectionLength);

						// Get the edit box text
						CEGUI::String sMultiLineEditBoxText = pMultiLineEditBox->getText();

						// Get the text we wish to copy from the edit box text
						CEGUI::String sSelectionText = sMultiLineEditBoxText.substr(sSelectionStartIndex, sSelectionEndIndex);

						// Set the clipboard text
						SharedUtility::SetClipboardText(sSelectionText.c_str(), (sSelectionText.length() + 1));

						// If its the control + x key cut the selection from the edit box text
						if(keyEventArgs.scancode == CEGUI::Key::X)
						{
							// Cut the text from the edit box text
							sMultiLineEditBoxText.replace(sSelectionStartIndex, sSelectionEndIndex, "");

							// Set the edit box text to the new cut text
							pMultiLineEditBox->setText(sMultiLineEditBoxText);
						}
					}
				}		
			}
		}
		// Check if its the v key that was pressed
		else if(keyEventArgs.scancode == CEGUI::Key::V)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox" || keyEventArgs.window->getType() == STYLE_PREFIX "/MultiLineEditbox")
			{
				// If its an edit box
				if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
				{
					// Get the edit box pointer
					CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;

					// Get the edit box selection length
					size_t sSelectionLength = pEditBox->getSelectionLength();

					// Get the edit box selection start index
					size_t sSelectionStartIndex = pEditBox->getSelectionStartIndex();

					// Get the edit box text
					CEGUI::String sEditBoxText = pEditBox->getText();
					
					// Get the clipboard text
					const char * szClipboardText = SharedUtility::GetClipboardText();
					
					// Do we have any clipboard text?
					if(szClipboardText)
					{
						// Get the caret index
						size_t sCaretIndex = pEditBox->getCaratIndex();
						
						// Add the clipboard text length to the caret index
						sCaretIndex += strlen(szClipboardText);
						
						// If we don't have a selection just insert the text
						if(sSelectionLength == 0)
							sEditBoxText.insert(sSelectionStartIndex, szClipboardText);
						// If we do have a selection overwrite the selected text
						else
							sEditBoxText.replace(sSelectionStartIndex, sSelectionLength, szClipboardText);
						
						// Set the edit box text to the string with the pasted text
						pEditBox->setText(sEditBoxText);
						
						// Set the edit box caret index to the new index after the pasted text
						pEditBox->setCaratIndex(sCaretIndex);
					}
				}
				// If its a multi line edit box
				else
				{
					// Get the edit box pointer
					CEGUI::MultiLineEditbox * pMultiLineEditBox = (CEGUI::MultiLineEditbox *)keyEventArgs.window;

					// Get the edit box selection length
					size_t sSelectionLength = pMultiLineEditBox->getSelectionLength();

					// Get the edit box selection start index
					size_t sSelectionStartIndex = pMultiLineEditBox->getSelectionStartIndex();

					// Get the edit box text
					CEGUI::String sMultiLineEditBoxText = pMultiLineEditBox->getText();

					// Get the clipboard text
					const char * szClipboardText = SharedUtility::GetClipboardText();

					// Do we have any clipboard text?
					if(szClipboardText)
					{
						// Get the caret index
						size_t sCaretIndex = pMultiLineEditBox->getCaratIndex();
						
						// Add the clipboard text length to the caret index
						sCaretIndex += strlen(szClipboardText);
						
						// If we don't have a selection just insert the text
						if(sSelectionLength == 0)
							sMultiLineEditBoxText.insert(sSelectionStartIndex, szClipboardText);
						// If we do have a selection overwrite the selected text
						else
							sMultiLineEditBoxText.replace(sSelectionStartIndex, sSelectionLength, szClipboardText);
						
						// Set the edit box text to the string with the pasted text
						pMultiLineEditBox->setText(sMultiLineEditBoxText);
						
						// Set the edit box caret index to the new index after the pasted text
						pMultiLineEditBox->setCaratIndex(sCaretIndex);
					}
				}
			}
		}
	}

	return false;
}

CEGUI::Font * CGUI::GetFont(String strFont, unsigned int uiSize, bool bScaled)
{
	if(m_bInitialized && strFont.IsNotEmpty())
	{
		// Get the font internal name
		String strInternalFont("%s#%d#%d", strFont.ToLower().Get(), uiSize, bScaled);

		// Is the font already loaded?
		if(m_pFontManager->isDefined(strInternalFont.Get()))
			return &m_pFontManager->get(strInternalFont.Get());

		// Get the font name
		String strName("%s.ttf", strFont.ToLower().Get());

		// Attempt to create the front from the fonts directory
		try {
			return &CEGUI::FontManager::getSingleton().createFreeTypeFont(strInternalFont.Get(), (float)uiSize, true, strName.Get(), "", bScaled);
		} catch(CEGUI::Exception e) {}

		// Attempt to create the font from the client resource directory
		try {
			return &CEGUI::FontManager::getSingleton().createFreeTypeFont(strInternalFont.Get(), (float)uiSize, true, strName.Get(), "resources", bScaled);
		} catch(CEGUI::Exception e) {}

		// Attempt to create the font from the windows fonts directory
		try {
			return &CEGUI::FontManager::getSingleton().createFreeTypeFont(strInternalFont.Get(), (float)uiSize, true, strName.Get(), "winfonts", bScaled);
		} catch(CEGUI::Exception e) {}
	}

	// Font does not exist and font creation failed
	return NULL;
}

CEGUI::String CGUI::AnsiToCeguiFriendlyString(const char * szAnsiString, unsigned int uiLength)
{
	wchar_t * wcUnicode = new wchar_t[uiLength + 1];
	SharedUtility::AnsiToUnicode(szAnsiString, -1, wcUnicode, uiLength);
	CEGUI::String strCegui;
	strCegui.resize(uiLength);

	for(size_t i = 0; i < uiLength; i++)
		strCegui[i] = (CEGUI::utf32)wcUnicode[i];

	delete[] wcUnicode;
	return strCegui;
}

CEGUI::String CGUI::AnsiToCeguiFriendlyString(String strAnsiString)
{
	return AnsiToCeguiFriendlyString(strAnsiString.Get(), strAnsiString.GetLength());
}
