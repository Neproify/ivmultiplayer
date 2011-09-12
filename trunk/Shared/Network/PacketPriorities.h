//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PacketPriorities.h
// Project: Shared
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

enum ePacketPriority
{
	// The highest possible priority. These message trigger sends immediately, and are generally not buffered or aggregated into a single datagram.
	PRIORITY_IMMEDIATE,

	// For every 2 PRIORITY_IMMEDIATE messages, 1 PRIORITY_HIGH will be sent.
	// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond intervals to reduce UDP overhead and better measure congestion control. 
	PRIORITY_HIGH,

	// For every 2 PRIORITY_HIGH messages, 1 PRIORITY_MEDIUM will be sent.
	// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond intervals to reduce UDP overhead and better measure congestion control. 
	PRIORITY_MEDIUM,

	// For every 2 PRIORITY_MEDIUM messages, 1 PRIORITY_LOW will be sent.
	// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond intervals to reduce UDP overhead and better measure congestion control. 
	PRIORITY_LOW,

	// Number of packet priorities
	PRIORITY_COUNT
};
