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
} gameFiles[] = 
{
	// TODO: Add more checks
	{ 0x3F19C08A, "common\\data\\handling.dat" },
	{ 0x6DA6A192, "common\\data\\gta.dat" }
};

bool CGameFileChecker::CheckFiles()
{
	CFileChecksum fileChecksum;

	for(int i = 0; i < ARRAY_LENGTH(gameFiles); i++)
	{
		// No need to check if the files exist as they should all be default game files
		fileChecksum.Calculate(String("%s%s", SharedUtility::GetExePath(), gameFiles[i].strFileName.Get()));

		if(fileChecksum.GetChecksum() != gameFiles[i].uiChecksum)
		{
			CLogFile::Printf("Checksum for file %s failed (Expected 0x%x, Got 0x%x).", gameFiles[i].strFileName.Get(), fileChecksum.GetChecksum(), gameFiles[i].uiChecksum);
			return false;
		}
	}

	return true;
}

unsigned int CGameFileChecker::CheckGameFile(int iFile)
{
	CFileChecksum fileChecksum;
	fileChecksum.Calculate(String("%s%s", SharedUtility::GetExePath(), gameFiles[iFile].strFileName.Get()));
	return fileChecksum.GetChecksum();
}

bool CGameFileChecker::IsGameFileChanged(int iFile)
{
	CFileChecksum fileChecksum;
	fileChecksum.Calculate(String("%s%s", SharedUtility::GetExePath(), gameFiles[iFile].strFileName.Get()));
	if(fileChecksum.GetChecksum() != gameFiles[iFile].uiChecksum)
		return true;
	else
		return false;
}