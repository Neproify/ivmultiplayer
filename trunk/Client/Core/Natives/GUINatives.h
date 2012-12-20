//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: GUINatives.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

// GUIFont
_MEMBER_FUNCTION_IMPL(GUIFont, constructor);
_MEMBER_FUNCTION_IMPL(GUIFont, getTextExtent);
_MEMBER_FUNCTION_IMPL(GUIFont, getFontHeight);
_MEMBER_FUNCTION_IMPL(GUIFont, drawText);

// Main GUIElement
_MEMBER_FUNCTION_IMPL(GUIElement, constructor);
_MEMBER_FUNCTION_IMPL(GUIElement, setText);
_MEMBER_FUNCTION_IMPL(GUIElement, setSize);
_MEMBER_FUNCTION_IMPL(GUIElement, getSize);
_MEMBER_FUNCTION_IMPL(GUIElement, setPosition);
_MEMBER_FUNCTION_IMPL(GUIElement, setRotation);
_MEMBER_FUNCTION_IMPL(GUIElement, setAlpha);
_MEMBER_FUNCTION_IMPL(GUIElement, setVisible);
_MEMBER_FUNCTION_IMPL(GUIElement, setParent);
_MEMBER_FUNCTION_IMPL(GUIElement, getName);
_MEMBER_FUNCTION_IMPL(GUIElement, getText);
_MEMBER_FUNCTION_IMPL(GUIElement, setFont);
_MEMBER_FUNCTION_IMPL(GUIElement, setProperty);
_MEMBER_FUNCTION_IMPL(GUIElement, activate);
_MEMBER_FUNCTION_IMPL(GUIElement, deactivate);

// GUIWindow
_MEMBER_FUNCTION_IMPL(GUIWindow, constructor);

// GUIText
_MEMBER_FUNCTION_IMPL(GUIText, constructor);
_MEMBER_FUNCTION_IMPL(GUIText, setText);

// GUIButton
_MEMBER_FUNCTION_IMPL(GUIButton, constructor);

// GUIEditBox
_MEMBER_FUNCTION_IMPL(GUIEditBox, constructor);

// GUIMultiLineEditBox
_MEMBER_FUNCTION_IMPL(GUIMultiLineEditBox, constructor);

// GUICheckBox
_MEMBER_FUNCTION_IMPL(GUICheckBox, constructor);
_MEMBER_FUNCTION_IMPL(GUICheckBox, setChecked);
_MEMBER_FUNCTION_IMPL(GUICheckBox, isChecked);

// GUIImage
_MEMBER_FUNCTION_IMPL(GUIImage, constructor);

// GUIProgressBar
_MEMBER_FUNCTION_IMPL(GUIProgressBar, constructor);
_MEMBER_FUNCTION_IMPL(GUIProgressBar, setValue);

// GUIWebView
_MEMBER_FUNCTION_IMPL(GUIWebView, constructor);
_MEMBER_FUNCTION_IMPL(GUIWebView, setURI);
_MEMBER_FUNCTION_IMPL(GUIWebView, evaluateJavaScript);
_MEMBER_FUNCTION_IMPL(GUIWebView, sendSignal);
_MEMBER_FUNCTION_IMPL(GUIWebView, setHTML);
_MEMBER_FUNCTION_IMPL(GUIWebView, setElementText);
_MEMBER_FUNCTION_IMPL(GUIWebView, getLoadInfo);
_MEMBER_FUNCTION_IMPL(GUIWebView, clickElement);
_MEMBER_FUNCTION_IMPL(GUIWebView, setSize);
_MEMBER_FUNCTION_IMPL(GUIWebView, registerJavaScriptMethod);
_MEMBER_FUNCTION_IMPL(GUIWebView, draw);