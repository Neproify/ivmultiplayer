//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientPacketHandler.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CChatWindow.h"
#include <CString.h>
#include "CNetworkManager.h"
#include <Network/PacketIdentifiers.h>
#include "CGameFileChecker.h"
#include "CMainMenu.h"
#include "CGame.h"

extern CChatWindow     * g_pChatWindow;
extern String            g_strNick;
extern CNetworkManager * g_pNetworkManager;
extern CMainMenu	   * g_pMainMenu;

void ResetGame(bool now);

void CClientPacketHandler::ConnectionRejected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	//g_pChatWindow->AddInfoMessage("Connection failed! (Rejected)");
	g_pNetworkManager->Disconnect();
	g_pMainMenu->ResetNetworkStats();
	g_pMainMenu->ShowMessageBox("The server rejected your connection request!", "Connection failed", true, false, false);
	g_pMainMenu->SetDisconnectButtonVisible(false);
}

void CClientPacketHandler::ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pChatWindow->AddInfoMessage("Connection established, please wait..");
	CBitStream bsSend;
	bsSend.Write(NETWORK_VERSION);
	bsSend.Write(g_strNick);

	CheckGTAFiles pCheckFiles;
	pCheckFiles.uiHandleFileChecksum = CGameFileChecker::CheckGameFile(0);
	pCheckFiles.uiGTAFileChecksum = CGameFileChecker::CheckGameFile(1);
	pCheckFiles.bGTAFileChecksum = CGameFileChecker::IsGameFileChanged(1);
	pCheckFiles.bHandleFileChanged = CGameFileChecker::IsGameFileChanged(0);

	bsSend.Write((char *)&pCheckFiles, sizeof(CheckGTAFiles));
	g_pNetworkManager->RPC(RPC_PlayerConnect, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
}

void CClientPacketHandler::ConnectionFailed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pChatWindow->AddInfoMessage("Connection failed(Timeout), retrying ....");
	g_pMainMenu->ResetNetworkStats();
	g_pNetworkManager->Connect();
}

void CClientPacketHandler::AlreadyConnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	//g_pChatWindow->AddInfoMessage("Connection failed(Already connected)!");
	g_pMainMenu->ShowMessageBox("You're already on the server!", "Connection failed", true, false, false);
}

void CClientPacketHandler::ServerFull(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pChatWindow->AddInfoMessage("Connection failed(server is full), retrying ....");
	g_pMainMenu->ResetNetworkStats();
	g_pNetworkManager->Connect();
	//g_pMainMenu->ShowMessageBox("The server is full!", "Connection failed", true, false, false);
}

void CClientPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	g_pChatWindow->AddInfoMessage("Server closed the connection ...");
	g_pMainMenu->ResetNetworkStats();
	CGame::SetKickedFromServer(true);

	if(g_pNetworkManager->IsConnected())
		g_pNetworkManager->Disconnect();

	ResetGame(true);
	//g_pMainMenu->ShowMessageBox("The server closed the connection!", "Disconnected", true, true, false);
	g_pMainMenu->SetDisconnectButtonVisible(false);
}

void CClientPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	//g_pChatWindow->AddInfoMessage("Connetion to the server lost, retrying ....");
	ResetGame(true);
	g_pMainMenu->ResetNetworkStats();
	g_pMainMenu->ShowMessageBox("Lost connection to the server!", "Timeout", true, true, false);
	g_pMainMenu->SetDisconnectButtonVisible(false);
}

void CClientPacketHandler::Banned(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	//g_pChatWindow->AddInfoMessage("Connection failed(You're banned)!");
	g_pMainMenu->ShowMessageBox("You are banned from the Server!", "Connection failed", false, false, false);
	g_pMainMenu->SetDisconnectButtonVisible(false);
}

void CClientPacketHandler::PasswordInvalid(CBitStream * pBitStream, CPlayerSocket * pSenderSocket)
{
	//g_pChatWindow->AddInfoMessage("Connection failed(Wrong password)!");
	g_pMainMenu->ShowMessageBox("Incorrect password!", "Connection failed", true, false, false);
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
