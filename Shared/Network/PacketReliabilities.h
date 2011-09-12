//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PacketReliabilities.h
// Project: Shared
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

enum ePacketReliability
{
	// Same as regular UDP, except that it will also discard duplicate datagrams.  RakNet adds (6 to 17) + 21 bits of overhead, 16 of which is used to detect duplicate packets and 6 to 17 of which is used for message length.
	RELIABILITY_UNRELIABLE,


	// Regular UDP with a sequence counter.  Out of order messages will be discarded.  This adds an additional 13 bits on top what is used for UNRELIABLE.
	RELIABILITY_UNRELIABLE_SEQUENCED,


	// The message is sent reliably, but not necessarily in any order.  Same overhead as UNRELIABLE.
	RELIABILITY_RELIABLE,


	// This message is RELIABILITY_RELIABLE and will arrive in the order you sent it.  Messages will be delayed while waiting for out of order messages.  Same overhead as RELIABILITY_UNRELIABLE_SEQUENCED.
	RELIABILITY_RELIABLE_ORDERED,


	// This message is RELIABILITY_RELIABLE and will arrive in the sequence you sent it.  Out or order messages will be dropped.  Same overhead as RELIABILITY_UNRELIABLE_SEQUENCED.
	RELIABILITY_RELIABLE_SEQUENCED,


	// Same as UNRELIABLE, however the user will get either ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS based on the result of sending this message when calling RakPeerInterface::Receive(). Bytes 1-4 will contain the number returned from the Send() function. On disconnect or shutdown, all messages not previously acked should be considered lost.
	RELIABILITY_UNRELIABLE_WITH_ACK_RECEIPT,


	// Same as RELIABILITY_UNRELIABLE_SEQUENCED. The user will also get ID_SND_RECEIPT_ACKED after the message is delivered when calling RakPeerInterface::Receive(). Bytes 1-4 will contain the number returned from the Send() function. On disconnect or shutdown, all messages not previously acked should be considered lost.
	RELIABILITY_RELIABLE_WITH_ACK_RECEIPT,


	// Same as RELIABILITY_RELIABILITY_RELIABLE_ORDERED_ACK_RECEIPT. The user will also get ID_SND_RECEIPT_ACKED after the message is delivered when calling RakPeerInterface::Receive(). Bytes 1-4 will contain the number returned from the Send() function. On disconnect or shutdown, all messages not previously acked should be considered lost.
	RELIABILITY_RELIABILITY_RELIABLE_ORDERED_WITH_ACK_RECEIPT,

	// Number of packet reliabilities
	RELIABILITY_COUNT
};
