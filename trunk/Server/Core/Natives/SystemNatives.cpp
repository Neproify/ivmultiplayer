//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SystemNatives.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "SystemNatives.h"
#include <SharedUtility.h>
#include <time.h>
#ifdef _LINUX
#include <sys/stat.h>
#include <sys/types.h>

#define mkdir(x) mkdir(x, 0777)
#else
#include <direct.h>
#endif

void CSystemNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("clock", Clock, 0, NULL);
	pScriptingManager->RegisterFunction("time", Time, 0, NULL);
	pScriptingManager->RegisterFunction("date", Date, -1, NULL);
	pScriptingManager->RegisterFunction("mkdir", MkDir, 1, "s");
	pScriptingManager->RegisterFunction("rmdir", RmDir, 1, "s");
	pScriptingManager->RegisterFunction("remove", Remove, 1, "s");
	pScriptingManager->RegisterFunction("rename", Rename, 2, "ss");
};

SQInteger CSystemNatives::Clock(SQVM * pVM)
{
	sq_pushfloat(pVM, (((SQFloat)clock()) / (SQFloat)CLOCKS_PER_SEC));
	return 1;
}

SQInteger CSystemNatives::Time(SQVM * pVM)
{
	time_t t;
	time(&t);
	sq_pushinteger(pVM, *((SQInteger *)&t));
	return 1;
}

void _set_integer_slot(HSQUIRRELVM v, const SQChar *name, SQInteger val)
{
	sq_pushstring(v,name,-1);
	sq_pushinteger(v,val);
	sq_rawset(v,-3);
}

SQInteger CSystemNatives::Date(SQVM * pVM)
{
	time_t t;
	SQInteger it;
	SQInteger format = 'l';

	if(sq_gettop(pVM) > 1)
	{
		sq_getinteger(pVM,2,&it);
		t = it;

		if(sq_gettop(pVM) > 2)
			sq_getinteger(pVM, 3, (SQInteger*)&format);
	}
	else
		time(&t);

	tm *date;

	if(format == 'u')
		date = gmtime(&t);
	else
		date = localtime(&t);

	if(!date)
		return sq_throwerror(pVM, "crt api failure");

	sq_newtable(pVM);
	_set_integer_slot(pVM, _SC("sec"), date->tm_sec);
	_set_integer_slot(pVM, _SC("min"), date->tm_min);
	_set_integer_slot(pVM, _SC("hour"), date->tm_hour);
	_set_integer_slot(pVM, _SC("day"), date->tm_mday);
	_set_integer_slot(pVM, _SC("month"), date->tm_mon);
	_set_integer_slot(pVM, _SC("year"), date->tm_year+1900);
	_set_integer_slot(pVM, _SC("wday"), date->tm_wday);
	_set_integer_slot(pVM, _SC("yday"), date->tm_yday);
	return 1;
}

SQInteger CSystemNatives::MkDir(SQVM * pVM)
{
	const char * s;
	sq_getstring(pVM, 2, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(mkdir(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return sq_throwerror(pVM, "mkdir() failed");

	sq_pushbool(pVM, true);
	return 0;
}

SQInteger CSystemNatives::RmDir(SQVM * pVM)
{
	const char * s;
	sq_getstring(pVM, 2, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(rmdir(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return sq_throwerror(pVM, "rmdir() failed");

	sq_pushbool(pVM, true);
	return 0;
}

SQInteger CSystemNatives::Remove(SQVM * pVM)
{
	const char * s;
	sq_getstring(pVM, 2, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(remove(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return sq_throwerror(pVM, "remove() failed");

	sq_pushbool(pVM, true);
	return 0;
}

SQInteger CSystemNatives::Rename(SQVM * pVM)
{
	const char *oldn, *newn;
	sq_getstring(pVM, 2, &oldn);
	sq_getstring(pVM, 3, &newn);
	String strOldName(oldn);
	String strNewName(newn);
	SharedUtility::RemoveIllegalCharacters(strOldName);
	SharedUtility::RemoveIllegalCharacters(strNewName);

	if(rename(SharedUtility::GetAbsolutePath("files/%s", strOldName.Get()), SharedUtility::GetAbsolutePath("files/%s", strNewName.Get())) == -1)
		return sq_throwerror(pVM, "rename() failed");

	sq_pushbool(pVM, true);
	return 0;
}
