//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CPickupModuleNatives
	{
	public:
		EntityId Create(int modelhash, unsigned char type, unsigned int value, CVector3 vecPosition, CVector3 vecRotation);
		bool Delete(EntityId pickupid);
		int GetModel(EntityId pickupid);
		int GetType(EntityId pickupid);
		bool SetValue(EntityId pickupid, unsigned int pvalue);
		unsigned int GetValue(EntityId pickupid);
		bool SetCoordinates(EntityId pickupid, CVector3 vecPosition);
		CVector3 GetCoordinates(EntityId pickupid);
		bool SetRotation(EntityId pickupid, CVector3 vecPosition);
		CVector3 GetRotation(EntityId pickupid);
	};

}
