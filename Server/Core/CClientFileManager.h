//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientFileManager.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include <map>
#include "CServer.h"
#include <CFileChecksum.h>

class CClientFileManager : public std::map<String, CFileChecksum>
{
public:
	CClientFileManager(bool bScriptManager);
	~CClientFileManager() { };

	bool Start(String strName);
	bool Stop(String strName);
	bool Restart(String strName);
	bool Exists(String strName);
	void HandleClientJoin(EntityId playerId);

private:
	bool bIsScriptManager;
};
