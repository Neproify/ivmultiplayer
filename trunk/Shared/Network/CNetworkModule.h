//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkModule.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetServerInterface.h"
#include "CNetClientInterface.h"
#include <CLibrary.h>

typedef bool                     (* VerifyVersion_t)(unsigned char ucVersion);
typedef CNetServerInterface *    (* GetNetServerInterface_t)();
typedef void                     (* DestroyNetServerInterface_t)(CNetServerInterface * pNetServer);
typedef CNetClientInterface *    (* GetNetClientInterface_t)();
typedef void                     (* DestroyNetClientInterface_t)(CNetClientInterface * pNetClient);

class CNetworkModule
{
private:
	static CLibrary *                  m_pLibrary;
	static GetNetServerInterface_t     m_pfnGetNetServerInterface;
	static DestroyNetServerInterface_t m_pfnDestroyNetServerInterface;
	static GetNetClientInterface_t     m_pfnGetNetClientInterface;
	static DestroyNetClientInterface_t m_pfnDestroyNetClientInterface;

public:
	CNetworkModule();
	~CNetworkModule();

	static bool                     Init();
	static void                     Shutdown();
	static bool                     VerifyVersion(unsigned char ucVersion);
	static CNetServerInterface *    GetNetServerInterface();
	static void                     DestroyNetServerInterface(CNetServerInterface * pNetServer);
	static CNetClientInterface *    GetNetClientInterface();
	static void                     DestroyNetClientInterface(CNetClientInterface * pNetClient);
};
