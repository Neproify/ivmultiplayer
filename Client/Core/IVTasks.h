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

class CIVTaskComplexNewGetInVehicle : public CIVTask
{
public:
	CIVTaskComplexNewGetInVehicle(CIVVehicle * pVehicle, int a3, int a4, unsigned int a5, float a6);
};

class CIVTaskComplexNewExitVehicle : public CIVTask
{
public:
	CIVTaskComplexNewExitVehicle(CIVVehicle * pVehicle, int iExitMode, int a4, int a5);
};

class CIVTaskComplexDie : public CIVTask
{
public:
	CIVTaskComplexDie(int a2, int a3, int a4, int a5, float a6, float a7, char a8);
};

class CIVTaskSimpleDead : public CIVTask
{
public:
	CIVTaskSimpleDead(DWORD dwDeathTime, char a3, char a4);
};

class CIVTaskSimpleCarSetPedOut : public CIVTask
{
public:
	CIVTaskSimpleCarSetPedOut(CIVVehicle * pVehicle, int a3, char a4, char a5);
};
