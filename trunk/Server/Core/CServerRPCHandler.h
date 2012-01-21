//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerRPCHandler.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Network/CRPCHandler.h>

class CServerRPCHandler : public CRPCHandler
{
private:
	static void PlayerJoin(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Chat(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Command(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Spawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Death(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void InVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PassengerSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void SmallSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NameChange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void CheckpointEntered(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void CheckpointLeft(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void EventCall(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetHazardLights(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

public:
	void        Register();
	void        Unregister();
};
