//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICPickupModuleNatives
	{
	public:
		virtual EntityId Create(int modelhash, unsigned char type, unsigned int value, CVector3 vecPosition, CVector3 vecRotation) = 0;
		virtual bool Delete(EntityId pickupid) = 0;
		virtual int GetModel(EntityId pickupid) = 0;
		virtual int GetType(EntityId pickupid) = 0;
		virtual bool SetValue(EntityId pickupid, unsigned int pvalue) = 0;
		virtual unsigned int GetValue(EntityId pickupid) = 0;
		virtual bool SetCoordinates(EntityId pickupid, CVector3 vecPosition) = 0;
		virtual CVector3 GetCoordinates(EntityId pickupid) = 0;
		virtual bool SetRotation(EntityId pickupid, CVector3 vecPosition) = 0;
		virtual CVector3 GetRotation(EntityId pickupid) = 0;
	};

}
