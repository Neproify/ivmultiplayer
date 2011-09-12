//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
//
// File: ClientScriptTransfer.h
// Author(s): JeNkStA
//
// Unless specified elsewhere, The copyright to the contents herein is the
// property of the IV:MP Team. The contents may be used and/or copied only
// with the written permission of the current IV:MP Developer(s).
//
//===============================================================================

#ifdef CLIENTSCRIPTS
#pragma once

// TODO: Fix the includes then use NetGame.h instead of RakNet.h
#include "CNetworkManager.h"
#include "..\Shared\RakNet\FileListTransfer.h"
#include "..\Shared\RakNet\IncrementalReadInterface.h"

class CClientScriptTransfer : public FileListProgress
{
private:
	FileListTransfer m_fileListTransfer;
	FileList m_fileList;
	IncrementalReadInterface m_incrementalReadInterface;

	virtual void OnFilePush(const char * fileName, unsigned int fileLengthBytes, unsigned int offset, unsigned int bytesBeingSent, bool done, SystemAddress targetSystem);

public:
	CClientScriptTransfer() { }
	~CClientScriptTransfer() { }
	void Send(SystemAddress systemAddress);
};
#endif