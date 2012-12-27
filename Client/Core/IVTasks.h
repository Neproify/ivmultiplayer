//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: IVTasks.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVTask.h"
#include "CIVVehicle.h"

class CIVTaskComplexNewGetInVehicle : public CIVTaskComplex
{
public:
	CIVTaskComplexNewGetInVehicle(CIVVehicle * pVehicle, int a3, int a4, unsigned int a5, float a6);
};

class CIVTaskComplexNewExitVehicle : public CIVTaskComplex
{
public:
	CIVTaskComplexNewExitVehicle(CIVVehicle * pVehicle, int iExitMode, int a4, int a5);
};

class CIVTaskComplexDie : public CIVTaskComplex
{
public:
	CIVTaskComplexDie(int a2, int a3, int a4, int a5, float a6, float a7, char a8);
};

class CIVTaskSimpleDead : public CIVTaskSimple
{
public:
	CIVTaskSimpleDead(DWORD dwDeathTime, char a3, char a4);
};

class CIVTaskSimpleCarSetPedInVehicle : public CIVTaskSimple
{
public:
	CIVTaskSimpleCarSetPedInVehicle(CIVVehicle * pVehicle, int a3, char a4, char a5);
};

class CIVTaskSimpleCarSetPedOut : public CIVTaskSimple
{
public:
	CIVTaskSimpleCarSetPedOut(CIVVehicle * pVehicle, int a3, char a4, char a5);
};

class CIVTaskSimpleStopWalking : public CIVTaskSimple
{
public:
	CIVTaskSimpleStopWalking(unsigned int uiPlayerIndex, char iType);
};

class CIVTaskSimpleStartWalking : public CIVTaskSimple
{
public:
	CIVTaskSimpleStartWalking(unsigned int playerIndex, float a1, float a2, float a3, int a4, int a5);
};
