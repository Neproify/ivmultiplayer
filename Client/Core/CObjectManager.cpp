//============== IV:Multiplayer - https://github.com/Neproify/ivmultiplayer ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
//            XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CObjectManager.h"

void CObjectManager::Process()
{
	for(EntityId i = 0; i < GetMax(); ++i)
	{
		CObject * pObject = Get(i);

		if(pObject)
			pObject->Process();
	}
}
