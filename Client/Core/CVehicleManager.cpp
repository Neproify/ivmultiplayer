//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "CVehicleManager.h"
#include "CClient.h"

extern CClient * g_pClient;

void CVehicleManager::Pulse()
{
	std::list<CStreamableEntity *> * streamedVehicles = g_pClient->GetStreamer()->GetStreamedInEntitiesOfType(STREAM_ENTITY_VEHICLE);

	for(std::list<CStreamableEntity *>::iterator iter = streamedVehicles->begin(); iter != streamedVehicles->end(); ++iter)
	{
		CNetworkVehicle * pVehicle = reinterpret_cast<CNetworkVehicle *>(*iter);

		// Process the vehicle
		pVehicle->Pulse();
	}
}
