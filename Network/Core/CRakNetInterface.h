//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CRakNetInterface.h
// Project: Network.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <StdInc.h>

class CRakNetInterface : public RakNet::PluginInterface2
{
public:
	/// Called on a send to the socket, per datagram, that does not go through the reliability layer
	/// \param[in] data The data being sent
	/// \param[in] bitsUsed How many bits long \a data is
	/// \param[in] remoteSystemAddress Which system this message is being sent to
	virtual void OnDirectSocketSend(const char *data, const RakNet::BitSize_t bitsUsed, RakNet::SystemAddress remoteSystemAddress) {(void) data; (void) bitsUsed; (void) remoteSystemAddress;}

	/// Called on a receive from the socket, per datagram, that does not go through the reliability layer
	/// \param[in] data The data being sent
	/// \param[in] bitsUsed How many bits long \a data is
	/// \param[in] remoteSystemAddress Which system this message is being sent to
	virtual void OnDirectSocketReceive(const char *data, const RakNet::BitSize_t bitsUsed, RakNet::SystemAddress remoteSystemAddress) {(void) data; (void) bitsUsed; (void) remoteSystemAddress;}

	/// Called on a send or receive of a message within the reliability layer
	/// \param[in] internalPacket The user message, along with all send data.
	/// \param[in] frameNumber The number of frames sent or received so far for this player depending on \a isSend .  Indicates the frame of this user message.
	/// \param[in] remoteSystemAddress The player we sent or got this packet from
	/// \param[in] time The current time as returned by RakNet::GetTimeMS()
	/// \param[in] isSend Is this callback representing a send event or receive event?
	virtual void OnInternalPacket(RakNet::InternalPacket *internalPacket, unsigned frameNumber, RakNet::SystemAddress remoteSystemAddress, RakNet::TimeMS time, int isSend) {(void) internalPacket; (void) frameNumber; (void) remoteSystemAddress; (void) time; (void) isSend;}
};
