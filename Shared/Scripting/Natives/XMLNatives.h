//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: XMLNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

void RegisterXMLNatives(CScriptingManager * pScriptingManager);

_MEMBER_FUNCTION_IMPL(xml, constructor);
_MEMBER_FUNCTION_IMPL(xml, save);
_MEMBER_FUNCTION_IMPL(xml, setTabSize);
_MEMBER_FUNCTION_IMPL(xml, nodeNext);
_MEMBER_FUNCTION_IMPL(xml, nodePrevious);
_MEMBER_FUNCTION_IMPL(xml, nodeAttribute);
_MEMBER_FUNCTION_IMPL(xml, nodeRemoveAttribute);
_MEMBER_FUNCTION_IMPL(xml, nodeSetAttribute);
_MEMBER_FUNCTION_IMPL(xml, nodeRoot);
_MEMBER_FUNCTION_IMPL(xml, nodeFind);
_MEMBER_FUNCTION_IMPL(xml, nodeFirstChild);
_MEMBER_FUNCTION_IMPL(xml, nodeParent);
_MEMBER_FUNCTION_IMPL(xml, nodeName);
_MEMBER_FUNCTION_IMPL(xml, nodeContent);
_MEMBER_FUNCTION_IMPL(xml, nodeSetContent);
_MEMBER_FUNCTION_IMPL(xml, nodeClear);
_MEMBER_FUNCTION_IMPL(xml, nodeNew);
_MEMBER_FUNCTION_IMPL(xml, lastError);
_MEMBER_FUNCTION_IMPL(xml, commentNew);
_MEMBER_FUNCTION_IMPL(xml, isComment);
