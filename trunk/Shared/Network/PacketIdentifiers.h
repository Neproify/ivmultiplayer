//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PacketIdentifiers.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#define INTERNAL_PACKET_END 131

enum ePacketIdentfiers
{
	// The server rejected our connection
	PACKET_CONNECTION_REJECTED = INTERNAL_PACKET_END,

	// We have successfully connected to the server
	PACKET_CONNECTION_SUCCEEDED,

	// We failed to connect to the server
	PACKET_CONNECTION_FAILED,

	// We are already connected to the server
	PACKET_ALREADY_CONNECTED,

	// The server is full
	PACKET_SERVER_FULL,

	// A client has connected to us
	PACKET_NEW_CONNECTION,

	// We have been disconnected from the client/server
	PACKET_DISCONNECTED,

	// We have lost connection to the client/server
	PACKET_LOST_CONNECTION,

	// We are banned from this server
	PACKET_BANNED,

	// The password we supplied to the server was invalid
	PACKET_PASSWORD_INVALID,

	// Remote procedure call
	PACKET_RPC,

	// Number of packet identifiers
	PACKET_COUNT
};
