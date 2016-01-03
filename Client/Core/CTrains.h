//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CIVTrain.h
// Project: Client.Core
// Author(s): DMA, FRi
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IV/CIVTrain.h"

class CTrains
{
private:
	CIVTrain	*	m_pTrain[4];
	DWORD			dwTrains[4];

	bool			m_bTrainActive[4];
	bool			m_bTrainCreated[4];
public:
	CTrains();	
	~CTrains();

	void CreateTrains();
	void ShutdownAndDeleteTrains();
};