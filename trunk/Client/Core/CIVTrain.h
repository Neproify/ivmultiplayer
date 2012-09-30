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
#include "CLogFile.h"

// Not sure.. 
class IVTrain
{
public:
	PAD(IVTrain,pad0,4);			// 00-03
	DWORD dwHandle;					// 04-07
    CVector3 vecPosition;			// 08-19
    PAD(IVTrain,pad2,19);			// 20-38
    DWORD dwUnkown1;				// 39-43
    BYTE byteState;                 // 44
    BYTE byteUnkown2;				// 45
	PAD(IVTrain,pad3,2);			// 46-78
    DWORD dwHashValue;				// 48-52
    PAD(IVTrain,pad4,347);			// 53-399
    BYTE byteIsDriving;             // 400-401
    PAD(IVTrain,pad5,205);          // 402-607
    DWORD dwScriptingHandle;        // 608-612
};

class CIVTrain
{
private:
	IVTrain *	m_pTrain;
	DWORD		m_dwPointer;
	EntityId	m_trainId;
	String		m_strTrainName;
public:
	CIVTrain(EntityId trainId, DWORD dwTrainPointer, String strTrainName);
	~CIVTrain();

	void				SetTrainOffsetsAndCreate(EntityId trainRegisterId);

	unsigned int		GetTrainHandle();
	DWORD				GetTrainPointer();
};