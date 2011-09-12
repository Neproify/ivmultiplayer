//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetStats.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "PacketPriorities.h"
#include "../SharedUtility.h"
#include <stdio.h>

typedef unsigned long long int NetStat_t;

enum NetStatMetrics
{
	// How many bytes per pushed via a call to RakPeerInterface::Send()
	USER_MESSAGE_BYTES_PUSHED,

	// How many user message bytes were sent via a call to RakPeerInterface::Send(). This is less than or equal to USER_MESSAGE_BYTES_PUSHED.
	// A message would be pushed, but not yet sent, due to congestion control
	USER_MESSAGE_BYTES_SENT,

	// How many user message bytes were resent. A message is resent if it is marked as RELIABILITY_RELIABLE, and either the message didn't arrive or the message ack didn't arrive.
	USER_MESSAGE_BYTES_RESENT,

	// How many user message bytes were received, and processed successfully.
	USER_MESSAGE_BYTES_RECEIVED_PROCESSED,

	// How many user message bytes were received, but ignored due to data format errors. This will usually be 0.
	USER_MESSAGE_BYTES_RECEIVED_IGNORED,

	// How many actual bytes were sent, including per-message and per-datagram overhead, and RELIABILITY_RELIABLE message acks
	ACTUAL_BYTES_SENT,

	// How many actual bytes were received, including overead and acks.
	ACTUAL_BYTES_RECEIVED,

	NET_STAT_METRICS_COUNT
};

class CNetStats
{
public:
	// For each type in NetStatMetrics, what is the value over the last 1 second?
	NetStat_t ulValueOverLastSecond[NET_STAT_METRICS_COUNT];

	/// For each type in NetStatMetrics, what is the total value over the lifetime of the connection?
	NetStat_t ulRunningTotal[NET_STAT_METRICS_COUNT];

	// When did the connection start?
	NetStat_t ulConnectionStartTime;

	// Is our current send rate throttled by congestion control?
	// This value should be true if you send more data per second than your bandwidth capacity
	bool bIsLimitedByCongestionControl;

	/// If bIsLimitedByCongestionControl is true, what is the limit, in bytes per second?
	NetStat_t ulBPSLimitByCongestionControl;

	/// Is our current send rate throttled by a call to RakPeer::SetPerConnectionOutgoingBandwidthLimit()?
	bool bIsLimitedByOutgoingBandwidthLimit;

	// If bIsLimitedByOutgoingBandwidthLimit is true, what is the limit, in bytes per second?
	NetStat_t ulBPSLimitByOutgoingBandwidthLimit;

	// For each priority level, how many messages are waiting to be sent out?
	unsigned int uiMessageInSendBuffer[PRIORITY_COUNT];

	// For each priority level, how many bytes are waiting to be sent out?
	double dBytesInSendBuffer[PRIORITY_COUNT];

	// How many messages are waiting in the resend buffer? This includes messages waiting for an ack, so should normally be a small value
	// If the value is rising over time, you are exceeding the bandwidth capacity. See BPSLimitByCongestionControl 
	unsigned int uiMessagesInResendBuffer;

	// How many bytes are waiting in the resend buffer. See also uiMessagesInResendBuffer
	NetStat_t ulBytesInResendBuffer;

	// Over the last second, what was our packetloss? This number will range from 0.0 (for none) to 1.0 (for 100%)
	float fPacketlossLastSecond;

	// What is the average total packetloss over the lifetime of the connection?
	float fPacketlossTotal;

	// Verbosity level currently supports 0 (low), 1 (medium), 2 (high)
	// Buffer must be hold enough to hold the output string.  See the source to get an idea of how many bytes will be output
	void ToString(char * buffer, int iVerbosityLevel)
	{
		if(iVerbosityLevel == 0)
		{
			sprintf(buffer,
				"Bytes per second sent     %llu\n"
				"Bytes per second received %llu\n"
				"Current packetloss        %.0f%%\n",
				(long long unsigned int) ulValueOverLastSecond[ACTUAL_BYTES_SENT],
				(long long unsigned int) ulValueOverLastSecond[ACTUAL_BYTES_RECEIVED],
				fPacketlossLastSecond
				);
		}
		else if(iVerbosityLevel == 1)
		{
			sprintf(buffer,
				"Actual bytes per second sent       %llu\n"
				"Actual bytes per second received   %llu\n"
				"Message bytes per second pushed    %llu\n"
				"Total actual bytes sent            %llu\n"
				"Total actual bytes received        %llu\n"
				"Total message bytes pushed         %llu\n"
				"Current packetloss                 %.0f%%\n"
				"Average packetloss                 %.0f%%\n"
				"Elapsed connection time in seconds %llu\n",
				(long long unsigned int)ulValueOverLastSecond[ACTUAL_BYTES_SENT],
				(long long unsigned int)ulValueOverLastSecond[ACTUAL_BYTES_RECEIVED],
				(long long unsigned int)ulValueOverLastSecond[USER_MESSAGE_BYTES_PUSHED],
				(long long unsigned int)ulRunningTotal[ACTUAL_BYTES_SENT],
				(long long unsigned int)ulRunningTotal[ACTUAL_BYTES_RECEIVED],
				(long long unsigned int)ulRunningTotal[USER_MESSAGE_BYTES_PUSHED],
				fPacketlossLastSecond,
				fPacketlossTotal,
				(long long unsigned int)(NetStat_t)((SharedUtility::GetTime()-ulConnectionStartTime)/1000)
				);

			if(ulBPSLimitByCongestionControl != 0)
			{
				char buff2[128];
				sprintf(buff2,
					"Send capacity                    %llu bytes per second (%.0f%%)\n",
					(long long unsigned int) ulBPSLimitByCongestionControl,
					100.0f * ulValueOverLastSecond[ACTUAL_BYTES_SENT] / ulBPSLimitByCongestionControl
					);
				strcat(buffer,buff2);
			}

			if(ulBPSLimitByOutgoingBandwidthLimit!=0)
			{
				char buff2[128];
				sprintf(buff2,
					"Send limit                       %llu (%.0f%%)\n",
					(long long unsigned int) ulBPSLimitByOutgoingBandwidthLimit,
					100.0f * ulValueOverLastSecond[ACTUAL_BYTES_SENT] / ulBPSLimitByOutgoingBandwidthLimit
					);
				strcat(buffer,buff2);
			}
		}	
		else
		{
			sprintf(buffer,
				"Actual bytes per second sent         %llu\n"
				"Actual bytes per second received     %llu\n"
				"Message bytes per second sent        %llu\n"
				"Message bytes per second resent      %llu\n"
				"Message bytes per second pushed      %llu\n"
				"Message bytes per second processed   %llu\n"
				"Message bytes per second ignored     %llu\n"
				"Total bytes sent                     %llu\n"
				"Total bytes received                 %llu\n"
				"Total message bytes sent             %llu\n"
				"Total message bytes resent           %llu\n"
				"Total message bytes pushed           %llu\n"
				"Total message bytes received         %llu\n"
				"Total message bytes ignored          %llu\n"
				"Messages in send buffer, by priority %i,%i,%i,%i\n"
				"Bytes in send buffer, by priority    %i,%i,%i,%i\n"
				"Messages in resend buffer            %i\n"
				"Bytes in resend buffer               %llu\n"
				"Current packetloss                   %.0f%%\n"
				"Average packetloss                   %.0f%%\n"
				"Elapsed connection time in seconds   %llu\n",
				(long long unsigned int) ulValueOverLastSecond[ACTUAL_BYTES_SENT],
				(long long unsigned int) ulValueOverLastSecond[ACTUAL_BYTES_RECEIVED],
				(long long unsigned int) ulValueOverLastSecond[USER_MESSAGE_BYTES_SENT],
				(long long unsigned int) ulValueOverLastSecond[USER_MESSAGE_BYTES_RESENT],
				(long long unsigned int) ulValueOverLastSecond[USER_MESSAGE_BYTES_PUSHED],
				(long long unsigned int) ulValueOverLastSecond[USER_MESSAGE_BYTES_RECEIVED_PROCESSED],
				(long long unsigned int) ulValueOverLastSecond[USER_MESSAGE_BYTES_RECEIVED_IGNORED],
				(long long unsigned int) ulRunningTotal[ACTUAL_BYTES_SENT],
				(long long unsigned int) ulRunningTotal[ACTUAL_BYTES_RECEIVED],
				(long long unsigned int) ulRunningTotal[USER_MESSAGE_BYTES_SENT],
				(long long unsigned int) ulRunningTotal[USER_MESSAGE_BYTES_RESENT],
				(long long unsigned int) ulRunningTotal[USER_MESSAGE_BYTES_PUSHED],
				(long long unsigned int) ulRunningTotal[USER_MESSAGE_BYTES_RECEIVED_PROCESSED],
				(long long unsigned int) ulRunningTotal[USER_MESSAGE_BYTES_RECEIVED_IGNORED],
				uiMessageInSendBuffer[PRIORITY_IMMEDIATE], uiMessageInSendBuffer[PRIORITY_HIGH], uiMessageInSendBuffer[PRIORITY_MEDIUM], uiMessageInSendBuffer[PRIORITY_LOW],
				(unsigned int)dBytesInSendBuffer[PRIORITY_IMMEDIATE], (unsigned int)dBytesInSendBuffer[PRIORITY_HIGH], (unsigned int)dBytesInSendBuffer[PRIORITY_MEDIUM], (unsigned int)dBytesInSendBuffer[PRIORITY_LOW],
				uiMessagesInResendBuffer,
				(long long unsigned int)ulBytesInResendBuffer,
				fPacketlossLastSecond,
				fPacketlossTotal,
				(long long unsigned int)(NetStat_t)((SharedUtility::GetTime()-ulConnectionStartTime)/1000000)
				);

			if(ulBPSLimitByCongestionControl != 0)
			{
				char buff2[128];
				sprintf(buff2,
					"Send capacity                    %llu bytes per second (%.0f%%)\n",
					(long long unsigned int)ulBPSLimitByCongestionControl,
					100.0f * ulValueOverLastSecond[ACTUAL_BYTES_SENT] / ulBPSLimitByCongestionControl
					);
				strcat(buffer,buff2);
			}

			if(ulBPSLimitByOutgoingBandwidthLimit != 0)
			{
				char buff2[128];
				sprintf(buff2,
					"Send limit                       %llu (%.0f%%)\n",
					(long long unsigned int)ulBPSLimitByOutgoingBandwidthLimit,
					100.0f * ulValueOverLastSecond[ACTUAL_BYTES_SENT] / ulBPSLimitByOutgoingBandwidthLimit
					);
				strcat(buffer,buff2);
			}
		}
	}
};
