//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CWebserver.h
// Project: Server.Core
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "SharedUtility.h"
#include <mongoose/mongoose.h>
#include <CFileChecksum.h>

class CWebServer
{
private:
	mg_context * m_pMongooseContext;

	//static void * MongooseEventHandler(mg_event event, mg_connection * conn);

public:
	CWebServer(unsigned short usHTTPPort);
	~CWebServer();

	bool FileCopy(String strClientFile, bool bIsScript, CFileChecksum &fileChecksum);
};
