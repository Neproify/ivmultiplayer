//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IModuleNatives.h"

namespace Modules
{
	class ICAreaModuleNatives
	{
	public:
		virtual float Points2D(float x, float y, float xx, float yy) = 0;
		virtual float Points3D(CVector3 vec1, CVector3 vec2) = 0;
		virtual bool PointInCircle(float x, float y, float xx, float yy, float distance) = 0;
		virtual bool PointInTube(float tubeX, float tubeY, float tubeZ, float tubeHeight, float tubeRadius, CVector3 vec) = 0;
		virtual bool PointInBall(CVector3 vec1, CVector3 vec2, float distance) = 0;
		virtual bool PointInArea(float areax, float areay, float areaxx, float areayy, float pointx, float pointy) = 0;
		virtual bool PointInCuboid(CVector3 area1, CVector3 area2, CVector3 point) = 0;
	};
}