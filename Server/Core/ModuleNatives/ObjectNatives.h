//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CObjectModuleNatives
	{
	public:
		EntityId Create(int modelhash, CVector3 vecPosition, CVector3 vecRotation);
		bool Delete(EntityId objectId);
		int GetModel(EntityId objectId);
		bool SetCoordinates(EntityId objectId, CVector3 vecPosition);
		CVector3 GetCoordinates(EntityId objectId);
		bool SetRotation(EntityId objectId, CVector3 vecRotation);
		CVector3 GetRotation(EntityId objectId);
	};
}