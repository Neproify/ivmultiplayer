//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGameFileChecker.cpp
// Project: Client.Core
// Author(s): funkyPresident
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CGameFileChecker.h"
#include <CFileChecksum.h>
#include <CLogFile.h>

struct GameFile
{
	unsigned int uiChecksum;
	String       strFileName;
};

GameFile gameFiles[] = 
{
	{ 0x509BEB, "common\\data\\handling.dat" },
	{ 0x0F9105, "common\\data\\gta.dat" }
};

bool CGameFileChecker::CheckFiles()
{
	CFileChecksum fileChecksum;

	for(int i = 0; i < ARRAY_LENGTH(gameFiles); i++)
	{
		// No need to check if file exists as gta will do that for us
		fileChecksum.Calculate(String("%s%s", SharedUtility::GetExePath(), gameFiles[i].strFileName.Get()));

		if(fileChecksum.GetChecksum() != gameFiles[i].uiChecksum)
		{
			CLogFile::Printf("Checksum for file %s failed (Expected 0x%x, Got 0x%x)", gameFiles[i].strFileName.Get(), fileChecksum.GetChecksum(), gameFiles[i].uiChecksum);
			MessageBox(NULL, String("GTA:IV file %s has been modified, please restore the original file if you want to play IV:MP", gameFiles[i].strFileName.Get()), "IV:MP", NULL);
			return false;
		}
	}

	return true;
}
