//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVTrain.cpp
// Project: Client.Core
// Author(s): DMA, FRi
// License: See LICENSE in root directory
//
//==============================================================================

// NOTE: WORK IN PROGRESS; DO NOT CHANGE THIS FILE!!

#include "CIVTrain.h"

CIVTrain::CIVTrain()
{
	// All Trains
	dwTrains[0] = *(DWORD *)(CGame::GetBase() + 0x15BE4BC); // Queens 1
	dwTrains[1] = *(DWORD *)(CGame::GetBase() + 0x15BE4C0); // Queens 2
	dwTrains[2] = *(DWORD *)(CGame::GetBase() + 0x15BE4C4); // Bronx 1
	dwTrains[3] = *(DWORD *)(CGame::GetBase() + 0x15BE4C8); // Bronx 2
}

CIVTrain::~CIVTrain()
{

}

unsigned int CIVTrain::GetTrainHandle(EntityId trainId)
{
	if(trainId < 4)
		return dwTrains[trainId];
	else
		return -1;
}
