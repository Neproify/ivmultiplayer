//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerPacketHandler.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Network/CPacketHandler.h>

class CServerPacketHandler : public CPacketHandler
{
private:
	static void NewConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

public:
	void        Register();
	void        Unregister();
};
