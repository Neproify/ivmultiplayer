//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientPacketHandler.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Network/CPacketHandler.h>

class CClientPacketHandler : public CPacketHandler
{
private:
	static void ConnectionRejected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ConnectionFailed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void AlreadyConnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ServerFull(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Banned(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PasswordInvalid(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

public:
	void        Register();
	void        Unregister();
};
