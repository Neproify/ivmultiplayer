//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: KeySync.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include "IVCommon.h"

struct CCam
{
	PAD(CCam, pad0, 0x10); // 000-010
	Matrix34 m_matMatrix;  // 010-050
	PAD(CCam, pad1, 0xF0); // 050-140
};

CCam *       GetGameCam();
void         SetGameCameraMatrix(Matrix * matMatrix);
void         GetGameCameraMatrix(Matrix * matMatrix);
void         InstallKeySyncHooks();
