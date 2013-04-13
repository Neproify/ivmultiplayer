//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientPacketHandler.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClient.h"
#include <CString.h>
#include <Network/PacketIdentifiers.h>
#include "CGameFileChecker.h"
#include "CGame.h"

extern CClient * g_pClient;

void CClientPacketHandler::ConnectionRejected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->Disconnect();
	g_pClient->GetMainMenu()->ResetNetworkStats();
	g_pClient->ResetMainMenuCamera();
	pMainMenu->ShowMessageBox("The server rejected your connection request!", "Connection failed", true, false, false);
	pMainMenu->SetDisconnectButtonVisible(false);
}

void CClientPacketHandler::ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetChatWindow()->AddInfoMessage("Connection established, please wait..");
	CBitStream bsSend;
	bsSend.Write(NETWORK_VERSION);
	bsSend.Write(g_pClient->GetNick());

	// jenksta: wtf?
	// Its checking if the client has modified files
	CheckGTAFiles pCheckFiles;
	pCheckFiles.uiHandleFileChecksum = CGameFileChecker::CheckGameFile(0);
	pCheckFiles.uiGTAFileChecksum = CGameFileChecker::CheckGameFile(1);
	pCheckFiles.bGTAFileChecksum = CGameFileChecker::IsGameFileChanged(1);
	pCheckFiles.bHandleFileChanged = CGameFileChecker::IsGameFileChanged(0);
	bsSend.Write((char *)&pCheckFiles, sizeof(CheckGTAFiles));

	g_pClient->GetNetworkManager()->RPC(RPC_PlayerConnect, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
}

void CClientPacketHandler::ConnectionFailed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetChatWindow()->AddInfoMessage("Connection failed (Timed out), retrying...");
	g_pClient->GetMainMenu()->ResetNetworkStats();
	g_pClient->GetNetworkManager()->Connect();
}

void CClientPacketHandler::AlreadyConnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetMainMenu()->ShowMessageBox("You're already on that server!", "Connection failed", true, false, false);
}

void CClientPacketHandler::ServerFull(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetChatWindow()->AddInfoMessage("Connection failed (server is full), retrying...");
	g_pClient->GetMainMenu()->ResetNetworkStats();
	g_pClient->GetNetworkManager()->Connect();
}

void CClientPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetChatWindow()->AddInfoMessage("You have disconnected from the server!");
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	
	if(pNetworkManager && pNetworkManager->IsConnected())
	{
		//pNetworkManager->Disconnect(); //Disconnect from the server *Disabled temporary, causing issues and will re-write it later.*
		g_pClient->GetMainMenu()->ResetNetworkStats(); //Reset the Network Stats
		g_pClient->ResetMainMenuCamera(); //Reset the Main Menu Camera
		pMainMenu->ShowMessageBox("You have successfully disconnected!", "Disconnected.", true, false, false);
	}
}	

void CClientPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ShowMessageBox("The server has lost the connection...", "Lost Connection.", false, false, false);
	g_pClient->GetMainMenu()->ResetNetworkStats();
	g_pClient->ResetMainMenuCamera(); //Reset the Main Menu Camera
	pMainMenu->SetDisconnectButtonVisible(false);
}	

void CClientPacketHandler::Banned(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ShowMessageBox("You are banned from this server!", "You're banned!", false, false, false);
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->Disconnect();
	g_pClient->GetMainMenu()->ResetNetworkStats();
	g_pClient->ResetMainMenuCamera();
}

void CClientPacketHandler::PasswordInvalid(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetMainMenu()->ShowMessageBox("The password you entered does not match the server password!", "Incorrect Password.", true, false, false);
}

void CClientPacketHandler::Register()
{
	AddFunction(PACKET_CONNECTION_REJECTED, ConnectionRejected);
	AddFunction(PACKET_CONNECTION_SUCCEEDED, ConnectionSucceeded);
	AddFunction(PACKET_CONNECTION_FAILED, ConnectionFailed);
	AddFunction(PACKET_ALREADY_CONNECTED, AlreadyConnected);
	AddFunction(PACKET_SERVER_FULL, ServerFull);
	AddFunction(PACKET_DISCONNECTED, Disconnected);
	AddFunction(PACKET_LOST_CONNECTION, LostConnection);
	AddFunction(PACKET_BANNED, Banned);
	AddFunction(PACKET_PASSWORD_INVALID, PasswordInvalid);
}

void CClientPacketHandler::Unregister()
{
	RemoveFunction(PACKET_CONNECTION_REJECTED);
	RemoveFunction(PACKET_CONNECTION_SUCCEEDED);
	RemoveFunction(PACKET_CONNECTION_FAILED);
	RemoveFunction(PACKET_ALREADY_CONNECTED);
	RemoveFunction(PACKET_SERVER_FULL);
	RemoveFunction(PACKET_DISCONNECTED);
	RemoveFunction(PACKET_LOST_CONNECTION);
	RemoveFunction(PACKET_BANNED);
	RemoveFunction(PACKET_PASSWORD_INVALID);
}
