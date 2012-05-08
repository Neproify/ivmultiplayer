//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: HashNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "md5/md5.h"
#include <SharedUtility.h>

// Hash functions
namespace Modules
{
	const char * CHashModuleNatives::md5(const char * toHash)
	{
		char * result = new char[33];
		MD5 md5bytes;
		CMD5Hasher hasher;

		hasher.Calculate(toHash, strlen(toHash), md5bytes);
		hasher.ConvertToHex(md5bytes, result);

		return result;
	}

	const char * CHashModuleNatives::md5File(const char * szFileName)
	{
		char * result = new char[33];
		MD5 md5bytes;
		CMD5Hasher hasher;
		String strFileName(szFileName);
		SharedUtility::RemoveIllegalCharacters(strFileName);
		String strFilePath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
		hasher.Calculate(strFilePath.Get(), md5bytes);
		hasher.ConvertToHex(md5bytes, result);
		return result;
	}
}