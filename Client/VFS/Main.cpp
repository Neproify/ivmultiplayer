//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Client.VFS
// Author(s): adamix
// License: See LICENSE in root directory
//
//============================================================================== 

#include "CVFS.h"

#ifdef WIN32
  #define EXPORT extern "C" __declspec(dllexport)
#else
  #define EXPORT extern "C"
#endif

bool bVersionVerified;

EXPORT bool VerifyVersion(unsigned char byteVersion)
{
	if(byteVersion == 0xFF)
	{
		bVersionVerified = true;
	}

	return bVersionVerified;
}

EXPORT CVFSInterface * GetVFSInterface()
{
	if(bVersionVerified)
	{
		CVFSInterface * pVFS = new CVFS();
		return pVFS;
	}

	return NULL;
}


int __stdcall DllMain(void *, unsigned long, void *)
{
	return 1;
}
