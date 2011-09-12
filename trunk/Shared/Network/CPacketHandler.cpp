//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPacketHandler.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPacketHandler.h"

CPacketHandler::CPacketHandler()
{

}

CPacketHandler::~CPacketHandler()
{
	std::list<PacketFunction *>::iterator iter;

	for(iter = m_packetFunctionList.begin(); iter != m_packetFunctionList.end(); iter++)
	{
		delete (*iter);
	}
}

void CPacketHandler::AddFunction(PacketIdentifier packetId, PacketFunction_t packetFunction)
{
	// Make sure it isn't already added
	PacketFunction * pFunction = GetFunctionFromIdentifier(packetId);

	if(pFunction)
	{
		// Function already added
		return;
	}

	// Create the packet function
	pFunction = new PacketFunction;
	pFunction->packetId = packetId;
	pFunction->packetFunction = packetFunction;

	// Add it to the packet function list
	m_packetFunctionList.push_back(pFunction);
}

void CPacketHandler::RemoveFunction(PacketIdentifier packetId)
{
	// Get the function
	PacketFunction * pFunction = GetFunctionFromIdentifier(packetId);

	// Is the function not added?
	if(!pFunction)
		return;

	// Remove it from the packet function list
	m_packetFunctionList.remove(pFunction);

	// Delete it
	delete pFunction;
}

PacketFunction * CPacketHandler::GetFunctionFromIdentifier(PacketIdentifier packetId)
{
	// Iterate through all packet functions
	std::list<PacketFunction *>::iterator iter;
	for(iter = m_packetFunctionList.begin(); iter != m_packetFunctionList.end(); iter++)
	{
		// Is this the function we're after?
		if((*iter)->packetId == packetId)
			return (*iter);
	}

	// Function not found
	return NULL;
}

bool CPacketHandler::HandlePacket(CPacket * pPacket)
{
	PacketFunction * pFunction = GetFunctionFromIdentifier(pPacket->packetId);

	// Does the function exist?
	if(pFunction)
	{
		// Construct the bit stream
		CBitStream bitStream(pPacket->ucData, pPacket->uiLength, false);

		// Call the function
		pFunction->packetFunction(&bitStream, pPacket->pPlayerSocket);
		return true;
	}

	// Not handled
	return false;
}
