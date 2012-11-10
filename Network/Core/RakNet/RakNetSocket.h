#ifndef __RAKNET_SOCKET_H
#define __RAKNET_SOCKET_H

#include "RakNetTypes.h"
#include "RakNetDefines.h"
#include "Export.h"
#include "SocketIncludes.h"

namespace RakNet
{

struct RAK_DLL_EXPORT RakNetSocket
{
	RakNetSocket();
	~RakNetSocket();
	// SocketIncludes.h includes Windows.h, which messes up a lot of compiles
	// SOCKET s;
	SOCKET s;
	unsigned int userConnectionSocketIndex;
	SystemAddress boundAddress;
	unsigned short socketFamily;
	bool blockingSocket;

#if defined (_WIN32) && defined(USE_WAIT_FOR_MULTIPLE_EVENTS)
	void* recvEvent;
#endif






	unsigned short remotePortRakNetWasStartedOn_PS3_PSP2;

	unsigned int extraSocketOptions;
	_PP_Instance_ chromeInstance;
};

} // namespace RakNet

#endif
