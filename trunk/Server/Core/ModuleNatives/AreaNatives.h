//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "ModuleNatives.h"

namespace Modules
{
	class CAreaModuleNatives : public ICAreaModuleNatives
	{
	public:
		float Points2D(float x, float y, float xx, float yy);
		float Points3D(CVector3 vec1, CVector3 vec2);
		bool PointInCircle(float x, float y, float xx, float yy, float distance);
		bool PointInTube(float tubeX, float tubeY, float tubeZ, float tubeHeight, float tubeRadius, CVector3 vec);
		bool PointInBall(CVector3 vec1, CVector3 vec2, float distance);
		bool PointInArea(float areax, float areay, float areaxx, float areayy, float pointx, float pointy);
		bool PointInCuboid(CVector3 area1, CVector3 area2, CVector3 point);
	};
}