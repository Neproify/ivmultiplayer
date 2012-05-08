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

#include "IModuleNatives.h"

namespace Modules
{
	class ICObjectModuleNatives
	{
	public:
		virtual EntityId Create(int modelhash, CVector3 vecPosition, CVector3 vecRotation) = 0;
		virtual bool Delete(EntityId objectId) = 0;
		virtual int GetModel(EntityId objectId) = 0;
		virtual bool SetCoordinates(EntityId objectId, CVector3 vecPosition) = 0;
		virtual CVector3 GetCoordinates(EntityId objectId) = 0;
		virtual bool SetRotation(EntityId objectId, CVector3 vecRotation) = 0;
		virtual CVector3 GetRotation(EntityId objectId) = 0;
	};
}