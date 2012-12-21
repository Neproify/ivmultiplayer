//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "CNetworkManager.h"
#include "CChatWindow.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CCheckpointManager.h"
#include "CLocalPlayer.h"
#include "CStreamer.h"
#include "Scripting/CScriptTimerManager.h"
#include <Network/CNetworkModule.h>
#include "CFileTransfer.h"
#include "CAudio.h"
#include "CActorManager.h"

extern String g_strNick;
extern CLocalPlayer * g_pLocalPlayer;
extern CChatWindow * g_pChatWindow;
extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CObjectManager * g_pObjectManager;
extern CBlipManager * g_pBlipManager;
extern CActorManager * g_pActorManager;
extern CCheckpointManager * g_pCheckpointManager;
extern CStreamer * g_pStreamer;
extern CScriptTimerManager * g_pScriptTimerManager;
extern CNetworkManager * g_pNetworkManager;
extern CFileTransfer * g_pFileTransfer;
extern CActorManager * g_pActorManager;


bool IsPlayerLimitReached()
{
	return false;
}

bool IsVehicleLimitReached()
{
	return false;
}
CNetworkManager::CNetworkManager()
	: m_pNetClient(CNetworkModule::GetNetClientInterface()),
	m_pClientPacketHandler(new CClientPacketHandler()),
	m_pClientRPCHandler(new CClientRPCHandler()),
	m_bJoinedServer(false),
	m_bJoinedGame(false)
{
	// Set the default host name
	m_sHostName.Set("IV:MP");

	// Set the net client packet handler function
	m_pNetClient->SetPacketHandler(PacketHandler);

	g_pChatWindow->AddInfoMessage(VERSION_IDENTIFIER_2 " Initialized");


}

CNetworkManager::~CNetworkManager()
{
	// Unregister the rpcs
	m_pClientRPCHandler->Unregister();

	// Delete the rpc handler instance
	SAFE_DELETE(m_pClientRPCHandler);

	// Unregister the packets
	m_pClientPacketHandler->Unregister();

	// Delete the packet handler instance
	SAFE_DELETE(m_pClientPacketHandler);

	// Shutdown the net client instance
	m_pNetClient->Shutdown(500);

	// Destroy the net client instance
	CNetworkModule::DestroyNetClientInterface(m_pNetClient);
}

void CNetworkManager::Startup(String strHost, unsigned short usPort, String strPassword)
{
	// Start up the net client
	m_pNetClient->Startup();

	// Set the net client host
	m_pNetClient->SetHost(strHost);

	// Set the net client port
	m_pNetClient->SetPort(usPort);

	// Set the net client password
	m_pNetClient->SetPassword(strPassword);

	// Register the packets
	m_pClientPacketHandler->Register();

	// Register the rpcs
	m_pClientRPCHandler->Register();
}

void CNetworkManager::PacketHandler(CPacket * pPacket)
{
	// Get the network manager pointer
	CNetworkManager * pNetworkManager = g_pNetworkManager;
	if(!g_pNetworkManager)
		return;
	// Pass it to the packet handler, if that doesn't handle it, pass it to the rpc handler
	if(!pNetworkManager->m_pClientPacketHandler->HandlePacket(pPacket) && !pNetworkManager->m_pClientRPCHandler->HandlePacket(pPacket))
		CLogFile::PrintDebugf("Warning: Unhandled packet (Id: %d)\n", pPacket->packetId);
}

void CNetworkManager::Process()
{
	// If our file transfer class exists process it>	Client.Core.dll!CNetworkManager::PacketHandler(CPacket * pPacket)  Zeile 106 + 0x1b Bytes	C++

	if(g_pFileTransfer)
		g_pFileTransfer->Process();

	// Have we joined a server and not joined a game yet?
	if(m_bJoinedServer && !m_bJoinedGame)
	{
		// Is the file transfer list empty?
		if(g_pFileTransfer->GetTransferListSize() == 0)
		{
			// Flag ourselves as joined a game
			m_bJoinedGame = true;

			// Respawn the local player
			g_pLocalPlayer->Respawn();
		}
	}

	if(!m_pNetClient)
		return;

	// Process the net client
	m_pNetClient->Process();

	// Are we connected to a server?
	if(m_pNetClient->IsConnected())
	{
		// If our streamer exists, process it
		if(g_pStreamer)
			g_pStreamer->Pulse();

		// Is our script timer manager exists, process it
		if(g_pScriptTimerManager)
			g_pScriptTimerManager->Pulse();

		// If our player manager exists process it
		if(g_pPlayerManager)
			g_pPlayerManager->Pulse();

		// If our vehicle manager exists process it
		if(g_pVehicleManager)
			g_pVehicleManager->Pulse();

		// If our checkpoint manager exists process it
		if(g_pCheckpointManager)
			g_pCheckpointManager->Pulse();

		// Process the audio manager
		CAudioManager::Process();

		// Process the actor manager
		g_pActorManager->Process();
	}
}

void CNetworkManager::Connect()
{
	// Are we not already connected?
	if(!IsConnected())
	{
		// Start the net client connection process
		eConnectionAttemptResult connectionAttemptResult = m_pNetClient->Connect();

		switch(connectionAttemptResult)
		{
		case CONNECTION_ATTEMPT_STARTED:
			g_pChatWindow->AddInfoMessage("Establishing connection to %s:%d...", m_pNetClient->GetHost(), m_pNetClient->GetPort());
			break;
		case INVALID_PARAMETER:
			g_pChatWindow->AddInfoMessage("Connection failed! (Invalid Parameter(Different Client-Server version))");
			break;
		case CANNOT_RESOLVE_DOMAIN_NAME:
			g_pChatWindow->AddInfoMessage("Connection failed! (Cannot Resolve Domain Name)");
			break;
		case ALREADY_CONNECTED_TO_ENDPOINT:
			g_pChatWindow->AddInfoMessage("Connection failed! (Already Connected To Endpoint)");
			break;
		case CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
			g_pChatWindow->AddInfoMessage("Connection failed! (Connection Attempt Already In Progress)");
			break;
		case SECURITY_INITIALIZATION_FAILED:
			g_pChatWindow->AddInfoMessage("Connection failed! (Security Initialization Failed)");
			break;
		case NO_HOST_SET:
			g_pChatWindow->AddInfoMessage("Connection failed! (No Host Set)");
			break;
		}
	}
}

bool CNetworkManager::IsConnected()
{
	return m_pNetClient->IsConnected();
}

void CNetworkManager::Disconnect()
{
	// Are we connected?
	if(IsConnected())
	{
		// Disconnect from the server
		m_pNetClient->Disconnect();

		// Delete and stop all Audio Client elements.
		CAudioManager::SetAllVolume(0.0f);
		CAudioManager::RemoveAll();

		// Flag ourselves as not joined a server or game
		m_bJoinedServer = false;
		m_bJoinedGame = false;
	}
}

void CNetworkManager::RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel)
{
	m_pNetClient->RPC(rpcId, pBitStream, priority, reliability, cOrderingChannel);
}
