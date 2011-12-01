//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CControlState.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CControlState.h"
#include <string.h>
#include <Network/CBitStream.h>

CControlState::CControlState()
{
	memset(this, 0, sizeof(CControlState));

	for(int i = 0; i < 4; i++)
		ucOnFootMove[i] = 128;

	for(int i = 0; i < 4; i++)
		ucInVehicleMove[i] = 128;

	for(int i = 0; i < 2; i++)
		ucInVehicleTriggers[i] = 128;
}

bool CControlState::operator== (const CControlState& o) const
{
	// todo: alternative
	return ((!memcmp(this, &o, sizeof(CControlState))) != 0);
}

bool CControlState::operator!= (const CControlState& o) const
{
	// todo: alternative
	return (memcmp(this, &o, sizeof(CControlState)) != 0);
}

void CControlState::Serialize(CBitStream * pBitStream) const
{
	pBitStream->Write((char *)ucOnFootMove, sizeof(ucOnFootMove));
	pBitStream->Write((char *)ucInVehicleMove, sizeof(ucInVehicleMove));
	pBitStream->Write((char *)ucInVehicleTriggers, sizeof(ucInVehicleTriggers));
	pBitStream->WriteBits((unsigned char *)&keys, KEY_COUNT);
}

bool CControlState::Deserialize(CBitStream * pBitStream)
{
	if(!pBitStream->Read((char *)ucOnFootMove, sizeof(ucOnFootMove)))
		return false;

	if(!pBitStream->Read((char *)ucInVehicleMove, sizeof(ucInVehicleMove)))
		return false;

	if(!pBitStream->Read((char *)ucInVehicleTriggers, sizeof(ucInVehicleTriggers)))
		return false;

	if(!pBitStream->ReadBits((unsigned char *)&keys, KEY_COUNT))
		return false;

	return true;
}
