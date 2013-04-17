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

void CClientPacketHandler::ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetMainMenu()->SetDisconnectButtonVisible(true);
	g_pClient->GetChatWindow()->AddInfoMessage("Connection established, please wait..");
	CBitStream bsSend;
	bsSend.Write(NETWORK_VERSION);
	bsSend.Write(g_pClient->GetNick());

	// jenksta: wtf?
	//We're checking to see if the client have any modified game files before passing into the server
	CheckGTAFiles pCheckFiles;
	pCheckFiles.uiHandleFileChecksum = CGameFileChecker::CheckGameFile(0);
	pCheckFiles.uiGTAFileChecksum = CGameFileChecker::CheckGameFile(1);
	pCheckFiles.bGTAFileChecksum = CGameFileChecker::IsGameFileChanged(1);
	pCheckFiles.bHandleFileChanged = CGameFileChecker::IsGameFileChanged(0);
	bsSend.Write((char *)&pCheckFiles, sizeof(CheckGTAFiles));

	g_pClient->GetNetworkManager()->RPC(RPC_PlayerConnect, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
}


void CClientPacketHandler::ConnectionRejected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Disconnect from the server
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	//pNetworkManager->Disconnect(); The server already rejected our connection so we're already disconnected

	// Show the main menu
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ResetNetworkStats();
	pMainMenu->SetVisible(true);
	g_pClient->ResetMainMenuCamera();
	pMainMenu->ShowMessageBox("The server rejected your connection!", "Connection Failed", true, false, false);
}

void CClientPacketHandler::ConnectionFailed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Disconnect from the server
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	//pNetworkManager->Disconnect(); What are we disconnecting from lul?

	// Show the main menu
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ResetNetworkStats();
	pMainMenu->SetVisible(true);
	g_pClient->ResetMainMenuCamera();
	pMainMenu->ShowMessageBox("Failed to connect to the server.", "Connection Failed", true, false, false);
}

void CClientPacketHandler::AlreadyConnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pClient->GetMainMenu()->ShowMessageBox("You are already connect to this server!", "Connection Failed", true, false, false);
}

void CClientPacketHandler::ServerFull(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Disconnect from the server
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->Disconnect(); //Disconnect from the server

	// Show the main menu
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ResetNetworkStats();
	g_pClient->ResetMainMenuCamera();
	pMainMenu->SetVisible(true);
	pMainMenu->ShowMessageBox("This server is full, try again later.", "Server Full", true, false, false);
}

void CClientPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Formally close the connection
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	if(pNetworkManager && pNetworkManager->IsConnected())
	{
		pNetworkManager->Disconnect(); //Disconnect from the server

		// Reset the game
		//g_pClient->ResetGame(true); ViruZz: No, just no!

		// Show a message box (main menu should already be shown due to reset)
		pMainMenu->ResetNetworkStats();
		g_pClient->ResetMainMenuCamera();
		pMainMenu->ShowMessageBox("You have successfully disconnected!", "Disconnected.", true, false, false);
	}	
}	

void CClientPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Formally close the connection
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	if(pNetworkManager && pNetworkManager->IsConnected())
	{	
		//pNetworkManager->Disconnect(); //Don't need to disconnect if the player already lost connection!

	// Reset the game
	//g_pClient->ResetGame(true); ViruZz: No, just no!

	// Show a message box (main menu should already be shown due to reset)
		pMainMenu->ResetNetworkStats();
		g_pClient->ResetMainMenuCamera();
		pMainMenu->ShowMessageBox("The connection to the server timed out.", "Disconnected", true, false, false);
	}	
}	

void CClientPacketHandler::Banned(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Disconnect from the server
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->Disconnect();
	
	// Reset the game
	//g_pClient->ResetGame(true); ViruZz: No, just no!

	// Show a message box (main menu should already be shown due to reset)
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ResetNetworkStats();
	g_pClient->ResetMainMenuCamera();
	pMainMenu->ShowMessageBox("You are banned from this server!", "Banned", true, false, false);
}

void CClientPacketHandler::PasswordInvalid(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	// Disconnect from the server
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();
	pNetworkManager->Disconnect();

	// Show the main menu
	CMainMenu * pMainMenu = g_pClient->GetMainMenu();
	pMainMenu->ResetNetworkStats();
	pMainMenu->SetVisible(true);
	g_pClient->ResetMainMenuCamera();
	pMainMenu->ShowMessageBox("The password you entered was incorrect.", "Invalid Password", true, false, false);
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
