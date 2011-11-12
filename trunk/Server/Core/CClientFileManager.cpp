//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientFileManager.cpp
// Project: Server.Core
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClientFileManager.h"
#include "CNetworkManager.h"
#include "CWebserver.h"
#include <map>
#include <CLogFile.h>

extern CNetworkManager * g_pNetworkManager;
extern CWebServer * g_pWebserver;

CClientFileManager::CClientFileManager(bool bScriptManager)
{
	bIsScriptManager = bScriptManager;
}

bool CClientFileManager::Start(String strName)
{
	if(Exists(strName))
		return false;

	CFileChecksum fileChecksum;
	
	if(!g_pWebserver->FileCopy(strName, bIsScriptManager, fileChecksum))
	{
		CLogFile::Printf("Failed to copy client file %s to web server.\n", strName.Get());
		return false;
	}

	insert(std::pair<String, CFileChecksum>(strName, fileChecksum));
	CBitStream bsSend;
	bsSend.Write(bIsScriptManager);
	bsSend.Write(strName);
	bsSend.Write((char *)&fileChecksum, sizeof(CFileChecksum));
	g_pNetworkManager->RPC(RPC_NewFile, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return true;
}

bool CClientFileManager::Stop(String strName)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
	{
		if((*iter).first == strName)
		{
			CBitStream bsSend;
			bsSend.Write(bIsScriptManager);
			bsSend.Write(strName);
			g_pNetworkManager->RPC(RPC_DeleteFile, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			erase(iter);
			return true;
		}
	}
	return false;
}

bool CClientFileManager::Restart(String strName)
{
	if(!Exists(strName))
		return false;

	Stop(strName);
	
	return Start(strName);
}

bool CClientFileManager::Exists(String strName)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
	{
		if((*iter).first == strName)
			return true;
	}

	return false;
}

void CClientFileManager::HandleClientJoin(EntityId playerId)
{
	CBitStream bitStream;

	for(iterator iter = begin(); iter != end(); ++ iter)
	{
		// Reset the bit stream
		bitStream.Reset();

		// Write if the file is a script or resource
		bitStream.Write(bIsScriptManager);

		// Write the file name
		bitStream.Write((*iter).first);

		// Write the file checksum
		bitStream.Write((char *)&((*iter).second), sizeof(CFileChecksum));

		// Send the rpc
		g_pNetworkManager->RPC(RPC_NewFile, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}	
}
