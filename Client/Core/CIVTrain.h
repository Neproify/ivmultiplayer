//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVTrain.h
// Project: Client.Core
// Author(s): DMA, FRi
// License: See LICENSE in root directory
//
//==============================================================================

// NOTE: WORK IN PROGRESS; DO NOT CHANGE THIS FILE!!

#pragma once
#include <Common.h>
#include "CGame.h"

class IVTrain
{
public:

};

class CIVTrain
{
private:
	DWORD dwTrains[4];
public:
	CIVTrain();
	~CIVTrain();

	unsigned int		GetTrainHandle(EntityId trainId);
};