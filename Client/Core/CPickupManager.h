//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CPickupManager.h
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CNetworkEntityManager.h"
#include "CPickup.h"

class CPickupManager : public CNetworkEntityManager<CPickup, MAX_PICKUPS>
{
};
