//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPadState.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPadState.h"
#include <string.h>
#include <Network/CBitStream.h>

CPadState::CPadState()
{
	memset(this, 0, sizeof(CPadState));
}

bool CPadState::operator== (const CPadState& o) const
{
	// todo: alternative
	return ((!memcmp(this, &o, sizeof(CPadState))) != 0);
}

bool CPadState::operator!= (const CPadState& o) const
{
	// todo: alternative
	return (memcmp(this, &o, sizeof(CPadState)) != 0);
}

void CPadState::Serialize(CBitStream * pBitStream) const
{
	pBitStream->Write((char *)ucOnFootMove, sizeof(ucOnFootMove));
	pBitStream->Write((char *)ucInVehicleMove, sizeof(ucInVehicleMove));
	pBitStream->WriteBits((unsigned char *)&keys, KEY_COUNT);
}

bool CPadState::Deserialize(CBitStream * pBitStream)
{
	if(!pBitStream->Read((char *)ucOnFootMove, sizeof(ucOnFootMove)))
		return false;

	if(!pBitStream->Read((char *)ucInVehicleMove, sizeof(ucInVehicleMove)))
		return false;

	if(!pBitStream->ReadBits((unsigned char *)&keys, KEY_COUNT))
		return false;

	return true;
}
