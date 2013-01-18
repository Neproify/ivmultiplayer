//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

// helloWorld ( fTest )
int sq_helloworld(HSQUIRRELVM pVM)
{
	// Get float param
	float fTest;
	sq_getfloat(pVM, -1, &fTest);

	// Return this float param incremented
	fTest += 0.15f;
	sq_pushfloat(pVM, fTest);

	// No errors.
	return 1;
}

// [Example] function which has several arguments and optional argument.
// createVehicleEx ( iModel, fX,fY,fZ, fRX,fRY,fRZ [, iRespawn_delay] )
int sq_createVehicleEx(HSQUIRRELVM pVM)
{
	// Check minimum parameters count (we need 7 at least)
	if(sq_gettop(pVM) < 8) // offset is +1 for calculating parameters count
	{
		sq_throwerror(pVM, "Invalid syntax (expected more params) at createVehicleEx");
		return -1;	// -1 enables 'raise error' in virtual machine
	}

	int iModel;
	float fX,fY,fZ;
	float fRX,fRY,fRZ;
	int iRespawn_delay = -1;	// default value for optional argument

	sq_getinteger(pVM, 2, &iModel);	// offset is +2 for parameter index
	sq_getfloat(pVM, 3, &fX);
	sq_getfloat(pVM, 4, &fY);
	sq_getfloat(pVM, 5, &fZ);
	sq_getfloat(pVM, 6, &fRX);
	sq_getfloat(pVM, 7, &fRY);
	sq_getfloat(pVM, 8, &fRZ);
	if(sq_gettop(pVM) >= 9)
		sq_getinteger(pVM, 9, &iRespawn_delay);

	// Create vehicle and return its identifier (ID):
	EntityId veh = IVMP::Vehicles()->Create(iModel, CVector3(fX,fY,fZ), CVector3(fRX,fRY,fRZ), 0,0,0,0, iRespawn_delay);
	sq_pushinteger(pVM, veh);

	// No errors.
	return 1;
}