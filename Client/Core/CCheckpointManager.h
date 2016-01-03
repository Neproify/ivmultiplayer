//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CCheckpointManager.h
// Project: Client.Core
// Author(s): MaVe
//            jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CCheckpoint.h"
#include "CNetworkEntityManager.h"

class CCheckpointManager : public CNetworkEntityManager<CCheckpoint, MAX_CHECKPOINTS>
{
public:
	void Pulse();
};
