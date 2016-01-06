//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: HashNatives.cpp
// Project: Shared
// Author(s): Einstein, Neproify
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "Scripting/CScriptingManager.h"
#include "md5/md5.h"
#include <SharedUtility.h>

// Hash functions

_BEGIN_CLASS(hash)
_MEMBER_FUNCTION(hash, md5, 1, "s")
_MEMBER_FUNCTION(hash, md5File, 1, "s")
_END_CLASS(hash)

void RegisterHashNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(hash));
}

_MEMBER_FUNCTION_IMPL(hash, md5)
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

_MEMBER_FUNCTION_IMPL(hash, md5File)
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