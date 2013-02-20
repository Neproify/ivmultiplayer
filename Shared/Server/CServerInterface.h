//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

//#include <StdInc.h>
#include <CString.h>

class CServerInterface
{
public:
	virtual bool OnLoad(int argc, char ** argv) = 0;
	virtual void Process() = 0;
	virtual void OnUnload() = 0;
	virtual void SetActive(bool bActive) = 0;
	virtual bool IsActive() = 0;
	virtual void SendInput(String strInput) = 0; // Not threadsafe
	virtual void AddToInputQueue(String strString) = 0; // Threadsafe
};
