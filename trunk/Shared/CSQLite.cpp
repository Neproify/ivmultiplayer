//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSQLite.cpp
// Project: Shared
// Author(s): Einstein
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CSQLite.h"

CSQLite::CSQLite()
{
	m_pDB = NULL;
}

CSQLite::~CSQLite()
{
	if(m_pDB)
		close();
}

bool CSQLite::open(String strFileName)
{
	if(m_pDB)
		return false;

	return (sqlite3_open(strFileName.Get(), &m_pDB) == SQLITE_OK);
}

bool CSQLite::close()
{
	if(!m_pDB)
		return false;

	return (sqlite3_close(m_pDB) == SQLITE_OK);
}

bool CSQLite::query(const char * szQuery)
{
	if(szQuery)
	{
		//char *zErrMsg;
		//int rc = sqlite3_exec(m_pDB, szQuery, this->sqlitecallback, 0, &zErrMsg);
		return false;
	}
	return false;
}
