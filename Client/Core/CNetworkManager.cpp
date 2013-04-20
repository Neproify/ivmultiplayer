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
#include "CClient.h"
#include "CActorManager.h"
#include <Network/CNetworkModule.h>
#include "CAudio.h"
#include <Network/PacketIdentifiers.h>

extern CClient * g_pClient;

CNetworkManager * CNetworkManager::m_pInstance = NULL;

CNetworkManager::CNetworkManager()
	: m_pNetClient(CNetworkModule::GetNetClientInterface()),
	m_pClientPacketHandler(new CClientPacketHandler()),
	m_pClientRPCHandler(new CClientRPCHandler()),
	m_bJoinedServer(false),
	m_bJoinedGame(false)
{
	// Set our instance
	m_pInstance = this;

	// Set the default host name
	m_sHostName.Set("IV:MP");

	// Set the net client packet handler function
	m_pNetClient->SetPacketHandler(PacketHandler);
	g_pClient->GetChatWindow()->AddInfoMessage(VERSION_IDENTIFIER_2 " Initialized");
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

	// Reset our instance
	m_pInstance = NULL;
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
	CNetworkManager * pInstance = GetInstance();

	if(!pInstance)
		return;

	// Pass it to the packet handler, if that doesn't handle it, pass it to the rpc handler
	if(!pInstance->m_pClientPacketHandler->HandlePacket(pPacket) && !pInstance->m_pClientRPCHandler->HandlePacket(pPacket)) 
	{
		if(g_pClient->GetChatWindow())
		{
			if(pPacket->packetId == PACKET_RPC)
			{
				// Construct the bit stream
				CBitStream bitStream(pPacket->ucData, pPacket->uiLength, false);
				RPCIdentifier rpcId;

				// Read the rpc id
				if(bitStream.Read(rpcId))
					g_pClient->GetChatWindow()->AddNetworkMessage("[NETWORK] Unhandled RPC (Type: %d)", rpcId);
			}
			else
				g_pClient->GetChatWindow()->AddNetworkMessage("[NETWORK] Unhandled packet (Type: %d)", pPacket->packetId);
		} 
	}
}

void CNetworkManager::Reset()
{
	// Unregister the packets
	if(m_pClientPacketHandler)
		m_pClientPacketHandler->Unregister();

	// Unregister the rpcs
	if(m_pClientRPCHandler)
		m_pClientRPCHandler->Unregister();

	// Reset network module
	if(m_pNetClient)
	{
		m_pNetClient->Shutdown(500);
		m_pNetClient->SetPacketHandler(PacketHandler);
		Startup(g_pClient->GetHost(), g_pClient->GetPort(), g_pClient->GetPassword());
	}

	m_bJoinedServer = false;
	m_bJoinedGame = false;
	m_sHostName.Set("IV:MP");
}

void CNetworkManager::Process()
{
	// Get our file transfer manager
	CFileTransferManager * pFileTransferManager = g_pClient->GetFileTransfer();

	// If our file transfer class exists process it
	if(pFileTransferManager)
		pFileTransferManager->Process();

	// Have we joined a server and not joined a game yet?
	if(m_bJoinedServer && !m_bJoinedGame)
	{
		// Get our local player
		CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

		// Is the file transfer list empty?
		if(pFileTransferManager->IsComplete() && pLocalPlayer->IsConnectFinished())
		{
			// Flag ourselves as joined a game
			m_bJoinedGame = true;

			// Respawn the local player
			pLocalPlayer->Respawn();
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
		CStreamer * pStreamer = g_pClient->GetStreamer();

		if(pStreamer)
			pStreamer->Pulse();

		// Is our script timer manager exists, process it
		CScriptTimerManager * pScriptTimerManager = g_pClient->GetClientScriptManager()->GetScriptTimerManager();
		if(pScriptTimerManager)
			pScriptTimerManager->Pulse();

		// If our player manager exists process it
		CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();

		if(pPlayerManager)
			pPlayerManager->Pulse();

		// If our vehicle manager exists process it
		CVehicleManager * pVehicleManager = g_pClient->GetVehicleManager();

		if(pVehicleManager)
			pVehicleManager->Pulse();

		// If our checkpoint manager exists process it
		CCheckpointManager * pCheckpointManager = g_pClient->GetCheckpointManager();

		if(pCheckpointManager)
			pCheckpointManager->Pulse();

		// If our object manager exists process it
		CObjectManager * pObjectManager = g_pClient->GetObjectManager();

		if(pObjectManager)
			pObjectManager->Process();

		// Process the audio manager
		CAudioManager * pAudioManager = g_pClient->GetAudioManager();

		if(pAudioManager)
			pAudioManager->Process();

		// Process the actor manager
		CActorManager * pActorManager = g_pClient->GetActorManager();

		if(pActorManager)
			pActorManager->Process();
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
			g_pClient->GetChatWindow()->AddInfoMessage("Establishing connection to %s:%d...", m_pNetClient->GetHost(), m_pNetClient->GetPort());
			break;
		case INVALID_PARAMETER:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (Invalid Parameter(Different Client-Server version))");
			break;
		case CANNOT_RESOLVE_DOMAIN_NAME:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (Cannot Resolve Domain Name)");
			break;
		case ALREADY_CONNECTED_TO_ENDPOINT:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (Already Connected To Endpoint)");
			break;
		case CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (Connection Attempt Already In Progress)");
			break;
		case SECURITY_INITIALIZATION_FAILED:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (Security Initialization Failed)");
			break;
		case NO_HOST_SET:
			g_pClient->GetChatWindow()->AddInfoMessage("Connection failed! (No Host Set)");
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

		// Remove all audio
		g_pClient->GetAudioManager()->RemoveAll();

		// Flag ourselves as not joined a server or game
		m_bJoinedServer = false;
		m_bJoinedGame = false;
	}
}

void CNetworkManager::RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel)
{
	m_pNetClient->RPC(rpcId, pBitStream, priority, reliability, cOrderingChannel);
}
