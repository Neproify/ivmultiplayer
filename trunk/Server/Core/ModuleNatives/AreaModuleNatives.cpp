//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include <Scripting/CScriptingManager.h>

// Area functions

namespace Modules
{

	// getDistanceBetweenPoints2D(x, y, xx, yy);
	float CAreaModuleNatives::Points2D(float x, float y, float xx, float yy)
	{
		return Math::GetDistanceBetweenPoints2D(x, y, xx, yy);
	}

	// getDistanceBetweenPoints3D(x, y, z, xx, yy, zz);
	float CAreaModuleNatives::Points3D(CVector3 vec1, CVector3 vec2)
	{
		return Math::GetDistanceBetweenPoints3D(vec1.fX, vec1.fY, vec1.fZ, vec2.fX, vec2.fY, vec2.fZ);
	}

	// isPointInCircle(x, y, xx, yy, distance)
	bool CAreaModuleNatives::PointInCircle(float x, float y, float xx, float yy, float distance)
	{
		return Math::IsPointInCircle(x, y, distance, xx, yy);
	}

	//isPointInTube(tubeX, tubeY, tubeZ, tubeHeight, tubeRadius, x, y, z)
	bool CAreaModuleNatives::PointInTube(float tubeX, float tubeY, float tubeZ, float tubeHeight, float tubeRadius, CVector3 vec)
	{	
		return Math::IsPointInTube(tubeX, tubeY, tubeZ, tubeHeight, tubeRadius, vec.fX, vec.fY, vec.fZ);
	}

	// isPointInBall(x, y, z, xx, yy, zz, distance)
	bool CAreaModuleNatives::PointInBall(CVector3 vec1, CVector3 vec2, float distance)
	{
		return Math::IsPointInBall(vec1.fX, vec1.fY, vec1.fZ, distance, vec2.fX, vec2.fY, vec2.fZ);
	}

	// isPointInArea(areaX, areaY, areaX2, areaY2, pointX, pointY)
	bool CAreaModuleNatives::PointInArea(float areax, float areay, float areaxx, float areayy, float pointx, float pointy)
	{
		return Math::IsPointInArea(areax, areay, areaxx, areayy, pointx, pointy);
	}

	// isPointInCuboid(areaX, areaY, areaZ, areaX2, areaY2, areaZ2, pointX, pointY, pointZ)
	bool CAreaModuleNatives::PointInCuboid(CVector3 area1, CVector3 area2, CVector3 point)
	{
		return Math::IsPointInCuboid(area1.fX, area1.fY, area1.fZ, area2.fX, area2.fY, area2.fZ, point.fX, point.fY, point.fZ);
		return 1;
	}
}
