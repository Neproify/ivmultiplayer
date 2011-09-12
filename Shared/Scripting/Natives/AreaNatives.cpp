//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../CScriptingManager.h"

// Area functions

void CAreaNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getDistanceBetweenPoints2D", Points2D, 4, "ffff");
	pScriptingManager->RegisterFunction("getDistanceBetweenPoints3D", Points3D, 6, "ffffff");
	pScriptingManager->RegisterFunction("isPointInCircle", PointInCircle, 5, "fffff");
	pScriptingManager->RegisterFunction("isPointInTube", PointInTube, 8, "ffffffff");
	pScriptingManager->RegisterFunction("isPointInBall", PointInBall, 7, "fffffff");
	pScriptingManager->RegisterFunction("isPointInArea", PointInArea, 6, "ffffff");
	pScriptingManager->RegisterFunction("isPointInCuboid", PointInCuboid, 9, "fffffffff");
	pScriptingManager->RegisterFunction("isPointInPolygon", PointInPolygon, -1, NULL);
}

// getDistanceBetweenPoints2D(x, y, xx, yy);
SQInteger CAreaNatives::Points2D(SQVM * pVM)
{
	float x, y, xx, yy;
	sq_getfloat(pVM, -4, &x);
	sq_getfloat(pVM, -3, &y);
	sq_getfloat(pVM, -2, &xx);
	sq_getfloat(pVM, -1, &yy);

	sq_pushfloat(pVM, Math::GetDistanceBetweenPoints2D(x, y, xx, yy));
	return 1;
}

// getDistanceBetweenPoints3D(x, y, z, xx, yy, zz);
SQInteger CAreaNatives::Points3D(SQVM * pVM)
{
	float x, y, z, xx, yy, zz;
	sq_getfloat(pVM, -6, &x);
	sq_getfloat(pVM, -5, &y);
	sq_getfloat(pVM, -4, &z);
	sq_getfloat(pVM, -3, &xx);
	sq_getfloat(pVM, -2, &yy);
	sq_getfloat(pVM, -1, &zz);

	sq_pushfloat(pVM, Math::GetDistanceBetweenPoints3D(x, y, z, xx, yy, zz));
	return 1;
}

// isPointInCircle(x, y, xx, yy, distance)
SQInteger CAreaNatives::PointInCircle(SQVM * pVM)
{
	float x, y, xx, yy, distance;

	sq_getfloat(pVM, -5, &x);
	sq_getfloat(pVM, -4, &y);
	sq_getfloat(pVM, -3, &xx);
	sq_getfloat(pVM, -2, &yy);
	sq_getfloat(pVM, -1, &distance);

	sq_pushbool(pVM, Math::IsPointInCircle(x, y, distance, xx, yy));
	return 1;
}

//isPointInTube(tubeX, tubeY, tubeZ, tubeHeight, tubeRadius, x, y, z)
SQInteger CAreaNatives::PointInTube(SQVM * pVM)
{
	float tubex, tubey, tubez, tubeheight, tubedistance, x, y, z;

	sq_getfloat(pVM, -8, &z);
	sq_getfloat(pVM, -7, &y);
	sq_getfloat(pVM, -6, &x);
	sq_getfloat(pVM, -5, &tubedistance);
	sq_getfloat(pVM, -4, &tubeheight);
	sq_getfloat(pVM, -3, &tubez);
	sq_getfloat(pVM, -2, &tubey);
	sq_getfloat(pVM, -1, &tubex);	

	sq_pushbool(pVM, Math::IsPointInTube(tubex, tubey, tubez, tubeheight, tubedistance, x, y, z));
	return 1;
}

// isPointInBall(x, y, z, xx, yy, zz, distance)
SQInteger CAreaNatives::PointInBall(SQVM * pVM)
{
	float x, y, z, xx, yy, zz, distance;

	sq_getfloat(pVM, -7, &x);
	sq_getfloat(pVM, -6, &y);
	sq_getfloat(pVM, -5, &z);
	sq_getfloat(pVM, -4, &xx);
	sq_getfloat(pVM, -3, &yy);
	sq_getfloat(pVM, -2, &zz);
	sq_getfloat(pVM, -1, &distance);

	sq_pushbool(pVM, Math::IsPointInBall(x, y, z, distance, xx, yy, zz));
	return 1;
}

// isPointInArea(areaX, areaY, areaX2, areaY2, pointX, pointY)
SQInteger CAreaNatives::PointInArea(SQVM * pVM)
{
	float areax, areay, areaxx, areayy, pointx, pointy;

	sq_getfloat(pVM, -6, &areax);
	sq_getfloat(pVM, -5, &areay);
	sq_getfloat(pVM, -4, &areaxx);
	sq_getfloat(pVM, -3, &areayy);
	sq_getfloat(pVM, -2, &pointx);
	sq_getfloat(pVM, -1, &pointy);

	sq_pushbool(pVM, Math::IsPointInArea(areax, areay, areaxx, areayy, pointx, pointy));
	return 1;
}

// isPointInCuboid(areaX, areaY, areaZ, areaX2, areaY2, areaZ2, pointX, pointY, pointZ)
SQInteger CAreaNatives::PointInCuboid(SQVM * pVM)
{
	float areax, areay, areaz, areaxx, areayy, areazz, pointx, pointy, pointz;

	sq_getfloat(pVM, -9, &areax);
	sq_getfloat(pVM, -8, &areay);
	sq_getfloat(pVM, -7, &areaz);
	sq_getfloat(pVM, -6, &areaxx);
	sq_getfloat(pVM, -5, &areayy);
	sq_getfloat(pVM, -4, &areazz);
	sq_getfloat(pVM, -3, &pointx);
	sq_getfloat(pVM, -2, &pointy);
	sq_getfloat(pVM, -1, &pointz);

	sq_pushbool(pVM, Math::IsPointInCuboid(areax, areay, areaz, areaxx, areayy, areazz, pointx, pointy, pointz));
	return 1;
}

//isPointInPolygon(pointX, pointY, ...)
SQInteger CAreaNatives::PointInPolygon(SQVM * pVM)
{
	CHECK_PARAMS_MIN("isPointInPolygon", 4);

	SQInteger iTop = sq_gettop(pVM) - 1;

	CHECK_TYPE("isPointInPolygon", 1, -iTop, OT_FLOAT);
	CHECK_TYPE("isPointInPolygon", 2, -(iTop - 1), OT_FLOAT);

	float pointX, pointY;
	sq_getfloat(pVM, -iTop, &pointX);
	sq_getfloat(pVM, -(iTop + 1), &pointY);

	float polyX[32], polyY[32];
	SQInteger currentPoly = 0;

	SQInteger iArgCount = iTop - 2;
	if(iArgCount > 0)
	{
		for(SQInteger i = 2; i < iTop; ++i)
		{
			if(i&1) {
				sq_getfloat(pVM, -(iTop - i), &polyX[currentPoly]);
			} else {
				sq_getfloat(pVM, -(iTop - i), &polyY[currentPoly]);
				currentPoly++;
			}
		}
	}

	sq_pushbool(pVM, Math::IsPointInPolygon(currentPoly, polyX, polyY, pointX, pointY));
	return 1;
}
