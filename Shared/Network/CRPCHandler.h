//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CRPCHandler.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include "CBitStream.h"
#include "CPlayerSocket.h"
#include "RPCIdentifiers.h"
#include "CPacket.h"

typedef void (* RPCFunction_t)(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

// Structure used for rpc functions
struct RPCFunction
{
	RPCIdentifier rpcId;
	RPCFunction_t rpcFunction;
};

class CRPCHandler
{
private:
	std::list<RPCFunction *> m_rpcFunctionList;

public:
	CRPCHandler();
	~CRPCHandler();

	void          AddFunction(RPCIdentifier rpcId, RPCFunction_t rpcFunction);
	void          RemoveFunction(RPCIdentifier rpcId);
	RPCFunction * GetFunctionFromIdentifier(RPCIdentifier rpcId);
	bool          HandlePacket(CPacket * pPacket);
};
