//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CRPCHandler.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CRPCHandler.h"
#include "PacketIdentifiers.h"

CRPCHandler::CRPCHandler()
{

}

CRPCHandler::~CRPCHandler()
{
	std::list<RPCFunction *>::iterator iter;

	for(iter = m_rpcFunctionList.begin(); iter != m_rpcFunctionList.end(); iter++)
	{
		delete (*iter);
	}
}

void CRPCHandler::AddFunction(RPCIdentifier rpcId, RPCFunction_t rpcFunction)
{
	// Make sure it isn't already added
	RPCFunction * pFunction = GetFunctionFromIdentifier(rpcId);

	if(pFunction)
	{
		// Function already added
		return;
	}

	// Create the rpc function
	pFunction = new RPCFunction;
	pFunction->rpcId = rpcId;
	pFunction->rpcFunction = rpcFunction;
	
	// Add it to the rpc function list
	m_rpcFunctionList.push_back(pFunction);
}

void CRPCHandler::RemoveFunction(RPCIdentifier rpcId)
{
	// Get the function
	RPCFunction * pFunction = GetFunctionFromIdentifier(rpcId);

	// Is the function not added?
	if(!pFunction)
		return;

	// Remove it from the rpc function list
	m_rpcFunctionList.remove(pFunction);

	// Delete it
	delete pFunction;
}

RPCFunction * CRPCHandler::GetFunctionFromIdentifier(RPCIdentifier rpcId)
{
	// Iterate through all rpc functions
	std::list<RPCFunction *>::iterator iter;
	for(iter = m_rpcFunctionList.begin(); iter != m_rpcFunctionList.end(); iter++)
	{
		// Is this the function we're after?
		if((*iter)->rpcId == rpcId)
			return (*iter);
	}

	// Function not found
	return NULL;
}

bool CRPCHandler::HandlePacket(CPacket * pPacket)
{
	// Is it an rpc packet?
	if(pPacket->packetId == PACKET_RPC)
	{
		// Construct the bit stream
		CBitStream bitStream(pPacket->ucData, pPacket->uiLength, false);
		RPCIdentifier rpcId;

		// Read the rpc id
		if(bitStream.Read(rpcId))
		{
			RPCFunction * pFunction = GetFunctionFromIdentifier(rpcId);

			// Does the function exist?
			if(pFunction)
			{
				// Call the function
				pFunction->rpcFunction(&bitStream, pPacket->pPlayerSocket);
				return true;
			}
		}
	}

	// Not handled
	return false;
}
