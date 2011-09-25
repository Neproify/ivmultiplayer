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

//CCam *       GetGameCam();
void SetGameCameraMatrix(Matrix * matMatrix);
void GetGameCameraMatrix(Matrix * matMatrix);
void InstallKeySyncHooks();
