#include "RakNetSocket.h"
#include "SocketIncludes.h"
#include "SocketDefines.h"
#include "RakMemoryOverride.h"

using namespace RakNet;

#if defined(__native_client__)
using namespace pp;
#endif

RakNetSocket::RakNetSocket() {
	s = INVALID_SOCKET;

#if defined (_WIN32) && defined(USE_WAIT_FOR_MULTIPLE_EVENTS)
	recvEvent=INVALID_HANDLE_VALUE;
#endif
}
RakNetSocket::~RakNetSocket() 
{
	if ((SOCKET)s != INVALID_SOCKET)
		closesocket__(s);

#if defined (_WIN32) && defined(USE_WAIT_FOR_MULTIPLE_EVENTS)
	if (recvEvent!=INVALID_HANDLE_VALUE)
	{
		CloseHandle( recvEvent );
		recvEvent = INVALID_HANDLE_VALUE;
	}
#endif
}
