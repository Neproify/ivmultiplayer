//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerPacketHandler.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CServerPacketHandler.h"
#include "CPlayerManager.h"
#include <Network/CBitStream.h>
#include <Network/CPlayerSocket.h>
#include <Network/PacketIdentifiers.h>
#include <CLogFile.h>

extern CPlayerManager * g_pPlayerManager;

void CServerPacketHandler::NewConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CLogFile::Printf("[Connect] Authorizing connection from IP %s (ID %d, Serial %s)...", pSenderSocket->GetAddress(false).Get(), pSenderSocket->playerId, pSenderSocket->strSerial.Get());
}

void CServerPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(g_pPlayerManager->DoesExist(pSenderSocket->playerId))
		g_pPlayerManager->Remove(pSenderSocket->playerId, 0);
}

void CServerPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	if(g_pPlayerManager->DoesExist(pSenderSocket->playerId))
		g_pPlayerManager->Remove(pSenderSocket->playerId, 1);
	else // User disconnect between playerConnect and playerJoin
		CLogFile::Printf("[Connect] Authorization for %s failed (connection timed out).", pSenderSocket->GetAddress(false).Get());
}

void CServerPacketHandler::Register()
{
	AddFunction(PACKET_NEW_CONNECTION, NewConnection);
	AddFunction(PACKET_DISCONNECTED, Disconnected);
	AddFunction(PACKET_LOST_CONNECTION, LostConnection);
}

void CServerPacketHandler::Unregister()
{
	RemoveFunction(PACKET_NEW_CONNECTION);
	RemoveFunction(PACKET_DISCONNECTED);
	RemoveFunction(PACKET_LOST_CONNECTION);
}
