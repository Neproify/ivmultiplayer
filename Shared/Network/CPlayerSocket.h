//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerSocket.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#ifdef _LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <Common.h>
#include <CString.h>

class CPlayerSocket
{
public:
	// The player id
	EntityId playerId;

	// The player binary address
	unsigned long ulBinaryAddress;

	// The player port
	unsigned short usPort;

	// The player serial
	String strSerial;

	CPlayerSocket()
	{
		playerId = INVALID_ENTITY_ID;
		ulBinaryAddress = 0xFFFFFFFF;
		usPort = 0xFFFF;
		strSerial.Set("00000000000000000000000000000000");
	}

	EntityId       GetPlayerId() { return playerId; }
	unsigned long  GetBinaryAdress() { return ulBinaryAddress; }
	String         GetAddress(bool bAndPort = false)
	{
		in_addr in;
		memset(&in, 0, sizeof(in));
		in.s_addr = ulBinaryAddress;

		if(bAndPort)
			return String("%s:%d", inet_ntoa(in), usPort);

		return String(inet_ntoa(in));
	}
	unsigned short GetPort() { return usPort; }
	String         GetSerial() { return strSerial; }
};
