//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCheckpointManager.cpp
// Project: Client.Core
// Author(s): MaVe
//            jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCheckpointManager.h"
#include "CPlayerManager.h"
#include "CNetworkManager.h"

extern CLocalPlayer    * g_pLocalPlayer;
extern CNetworkManager * g_pNetworkManager;
extern CStreamer       * g_pStreamer;

void CCheckpointManager::Pulse()
{
	std::list<CStreamableEntity *> * streamedCheckpoints = g_pStreamer->GetStreamedInEntitiesOfType(STREAM_ENTITY_CHECKPOINT);

	for(std::list<CStreamableEntity *>::iterator iter = streamedCheckpoints->begin(); iter != streamedCheckpoints->end(); ++iter)
	{
		CCheckpoint * pCheckpoint = reinterpret_cast<CCheckpoint *>(*iter);

		// Process the checkpoint
		pCheckpoint->Pulse();
	}
}
