//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CPlayerInterface.h"

class CPlayerManagerInterface
{
public:
 	virtual bool DoesExist(EntityId playerId) = 0;
	virtual void Add(EntityId playerId, char * sPlayerName) = 0;
	virtual bool Remove(EntityId playerId, BYTE byteReason) = 0;
	virtual bool IsNameInUse(char * szNick) = 0;
	virtual EntityId GetPlayerFromName(char * sNick) = 0;
	virtual EntityId GetPlayerCount() = 0;

	virtual CPlayerInterface * GetAt(EntityId playerId) = 0;
};