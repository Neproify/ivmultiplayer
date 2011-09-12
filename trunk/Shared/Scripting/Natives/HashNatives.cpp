//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: HashNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "Scripting/CScriptingManager.h"
#include "md5/md5.h"
#include <SharedUtility.h>

// Hash functions

void CHashNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("md5", md5, 1, "s");
	pScriptingManager->RegisterFunction("md5File", md5File, 1, "s");
}

SQInteger CHashNatives::md5(SQVM * pVM)
{
	const char * toHash;
	char result[33];
	MD5 md5bytes;
	CMD5Hasher hasher;

	sq_getstring(pVM, -1, &toHash);

	hasher.Calculate(toHash, strlen(toHash), md5bytes);
	hasher.ConvertToHex(md5bytes, result);

	sq_pushstring(pVM, result, 32);

	return 1;
}

SQInteger CHashNatives::md5File(SQVM * pVM)
{
	const char * szFileName;
	char result[33];
	MD5 md5bytes;
	CMD5Hasher hasher;
	sq_getstring(pVM, -1, &szFileName);
	String strFileName(szFileName);
	SharedUtility::RemoveIllegalCharacters(strFileName);
	String strFilePath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
	hasher.Calculate(strFilePath.Get(), md5bytes);
	hasher.ConvertToHex(md5bytes, result);
	sq_pushstring(pVM, result, 32);
	return 1;
}
