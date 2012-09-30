//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVTrain.cpp
// Project: Client.Core
// Author(s): DMA, FRi
// License: See LICENSE in root directory
//
//==============================================================================

#include "CTrains.h"

CTrains::CTrains() 
{
	// Define our trains
	dwTrains[0] = *(DWORD *)(CGame::GetBase() + 0x15BE4BC); // Queens 1
	dwTrains[1] = *(DWORD *)(CGame::GetBase() + 0x15BE4C0); // Queens 2
	dwTrains[2] = *(DWORD *)(CGame::GetBase() + 0x15BE4C4); // Bronx 1
	dwTrains[3] = *(DWORD *)(CGame::GetBase() + 0x15BE4C8); // Bronx 2

	// Reset all 
	for(int i = 0; i < 4; i++)
	{
		m_pTrain[i] = NULL;
		m_bTrainActive[i] = false;
		m_bTrainCreated[i] = false;
	}
}

CTrains::~CTrains()
{

}

void CTrains::CreateTrains()
{
	String trainNames[] = { "Queens1","Queens2","Bronx1","Bronx2" };

	// Initialize all trains
	for(int i = 0; i < 4; i++)
	{
		m_pTrain[i] = new CIVTrain(i,dwTrains[i],trainNames[i]);
		if(m_pTrain[i])
			m_bTrainActive[i] = true;
	}

	for(int i = 0; i < 4; i++)
	{
		if(m_pTrain[i])
		{
			m_pTrain[i]->SetTrainOffsetsAndCreate(i);
			m_bTrainCreated[i] = true;
		}
		else
			CLogFile::Printf("Failed to create train %d(no IVTrain reference...)",i);
	}
}

void CTrains::ShutdownAndDeleteTrains()
{
	for(int i = 0; i < 4; i++)
	{
		if(m_pTrain[i])
		{
			if(m_bTrainCreated[i])
			{
				m_pTrain[i]->~CIVTrain();
			}
			m_bTrainCreated[i] = false; // Before deleting the class
			SAFE_DELETE(m_pTrain[i]);
		}
		m_bTrainActive[i] = false;
	}
}