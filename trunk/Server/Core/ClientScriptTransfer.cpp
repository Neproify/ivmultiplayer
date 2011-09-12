//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
//
// File: ClientScriptTransfer.cpp
// Author(s): JeNkStA
//
// Unless specified elsewhere, The copyright to the contents herein is the
// property of the IV:MP Team. The contents may be used and/or copied only
// with the written permission of the current IV:MP Developer(s).
//
//===============================================================================

#ifdef CLIENTSCRIPTS
#include <sys/stat.h>
#include "ClientScriptTransfer.h"
#include "NetGame.h"
#include "..\Shared\RakNet\FileOperations.h"

extern CNetGame * m_pNetGame;
extern StringList m_pClientScripts;

void LogPrintf(const char * szFormat, ...);

void CClientScriptTransfer::OnFilePush(const char * fileName, unsigned int fileLengthBytes, unsigned int offset, unsigned int bytesBeingSent, bool done, SystemAddress targetSystem)
{
	printf("Sending %s bytes=%i offset=%i\n", fileName, bytesBeingSent, offset);
}

void CClientScriptTransfer::Send(SystemAddress systemAddress)
{
	/*char file[128];
	strcpy(file, "C:\\Users\\JeNkStA\\Desktop\\jessicac.jpg");*/
	m_pNetGame->GetRakServer()->GetRakPeer()->SetSplitMessageProgressInterval(9);
	m_pNetGame->GetRakServer()->GetRakPeer()->AttachPlugin(&m_fileListTransfer);
	m_fileListTransfer.SetCallback(this);
	char szPath[MAX_PATH];
	StringList::iterator iter;
	for(iter = m_pClientScripts.begin(); iter != m_pClientScripts.end(); iter++)
	{
		sprintf(szPath, ".\\clientscripts\\%s", (char*)(*iter));
		struct stat stFileInfo;
		if(stat(szPath, &stFileInfo) == 0)
		{
			unsigned int fileLength = GetFileLength(szPath);
			m_fileList.AddFile((*iter), szPath, 0, fileLength, fileLength, FileListNodeContext(0, 0), true);
		}
		else
		{
			LogPrintf("Warning: Clientscript %s does not exist.", (char*)(*iter));
		}
	}
	m_fileListTransfer.Send(&m_fileList, m_pNetGame->GetRakServer()->GetRakPeer(), systemAddress, 0, HIGH_PRIORITY, 0, false, &m_incrementalReadInterface, 50000);
}
#endif