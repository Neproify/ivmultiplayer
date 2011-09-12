//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSQLite.h
// Project: Server.Core
// Author(s): Einstein
//            jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "sqlite/sqlite3.h"
#include <CString.h>

class CSQLite
{
private:
	sqlite3 * m_pDB;

public:
	CSQLite();
	~CSQLite();

	sqlite3 * getDatabase() { return m_pDB; }
	bool      isopen() { return (m_pDB != NULL); }
	bool      open(String strFileName);
	bool      close();
	bool      query(const char * szQuery);
};
