//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SQLiteNatives.cpp
// Project: Shared
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Move to server

#include "Natives.h"
#include "../CScriptingManager.h"
#include "../../CSQLite.h"
#include "sqlite/sqlite3.h"
#include <SharedUtility.h>

// SQLite Database
_BEGIN_CLASS(db)
_MEMBER_FUNCTION(db, constructor, 1, "s")
_MEMBER_FUNCTION(db, query, 1, "s")
_MEMBER_FUNCTION(db, close, 0, NULL)
_END_CLASS(db)

void RegisterSQLiteNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(db));
}

_MEMBER_FUNCTION_RELEASE_HOOK(db)
{
	CSQLite * pSQLite = (CSQLite *)pInst;
	pSQLite->close();
	delete pSQLite;
	return 1;
}

_MEMBER_FUNCTION_IMPL(db, constructor)
{
	const char * filename;
	sq_getstring(pVM, -1, &filename);

	if(filename)
	{
		CSQLite * pSQLite = new CSQLite();
		String strFileName(filename);
		SharedUtility::RemoveIllegalCharacters(strFileName);
		String strPath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
		pSQLite->open(strPath);

		if(SQ_FAILED(sq_setinstance(pVM, pSQLite)))
		{
			CLogFile::Print("Failed to set the database instance.");
			SAFE_DELETE(pSQLite);
			sq_pushbool(pVM, false);
			return 1;
		}

		_SET_RELEASE_HOOK(db);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// TODO: Fix this function
_MEMBER_FUNCTION_IMPL(db, query)
{
	char * query;
	sq_getstring(pVM, -1, (const char **)&query);
	if(query)
	{
		CSQLite * pSQLite = sq_getinstance<CSQLite *>(pVM);

		if(!pSQLite)
		{
			CLogFile::Print("Failed to get the database instance.");
			sq_pushbool(pVM, false);
			return 1;
		}

		sqlite3 * database = pSQLite->getDatabase();
		sqlite3_stmt * stmt;

		if(sqlite3_prepare(database, query, strlen(query) + 1, &stmt, 0) != SQLITE_OK)
		{
			//const char * errmsg = sqlite3_errmsg(database); // TODO: let the user get the error message using a seperate function
			sq_pushbool(pVM, false);
			//sq_pushstring(vm, errmsg, sizeof(errmsg));
			return 1;
		}

		sq_newtable(pVM);

		SQInteger rowCount = 0;
		//sqlite3_step(stmt);

		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			rowCount++;
			SQInteger colCount = sqlite3_column_count(stmt);

			sq_pushinteger(pVM, rowCount);
			sq_newtable(pVM);

			for(SQInteger i = 0; i < colCount; i++)
			{
				SQInteger columnType = sqlite3_column_type(stmt, i);
				const char * columnName = sqlite3_column_name(stmt, i);

				sq_pushstring(pVM, columnName, strlen(columnName));

				SQInteger type = sqlite3_column_type(stmt, i);

				switch(type)
				{
				case SQLITE_NULL:
					break;
				case SQLITE_INTEGER:
					sq_pushinteger(pVM, sqlite3_column_int(stmt, i));
					break;
				case SQLITE_FLOAT:
					sq_pushfloat(pVM, (float)sqlite3_column_double(stmt, i));
					break;
				case SQLITE_BLOB:
					/*SQInteger length = sqlite3_column_bytes(stmt, i);
					if(length > 0) {
					new unsigned char val[length];
					memcpy(val, (const void *)sqlite3_column_blob(stmt, i), length);
					sq_pushstring(vm, val, sizeof(val));
					}*/
					break;
				default:
					/*SQInteger length = sqlite3_column_bytes(stmt, i) + 1;
					new unsigned char val[length];
					memcpy(val, sqlite3_column_text(stmt, i), length);

					sq_pushstring(vm, val, sizeof(val));*/
					break;

				}

				sq_createslot(pVM, -3);
			}

			sq_createslot(pVM, -3);
		}

		sqlite3_finalize(stmt);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(db, close)
{
	CSQLite * pSQLite = sq_getinstance<CSQLite *>(pVM);

	if(!pSQLite)
	{
		CLogFile::Print("Failed to get the database instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pSQLite->close());
	return 1;
}
