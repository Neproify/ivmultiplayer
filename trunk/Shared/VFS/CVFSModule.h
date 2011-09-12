//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFSModule.h
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//============================================================================== 

#pragma once

#include "CVFSInterface.h"
#include "CLibrary.h"

typedef bool                     (* VerifyVFSVersion_t)(unsigned char ucVersion);
typedef CVFSInterface       *    (* GetVFSInterface_t)();

class CVFSModule
{
private:
	static CVFSInterface * m_pVFSInterface;
	static CLibrary * m_pLibrary;
	static GetVFSInterface_t m_pfnGetVFSInterface;

public:
	CVFSModule();
	static bool Load();
	static CVFSInterface * GetInterface() { return m_pVFSInterface; };
};
