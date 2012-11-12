//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWorld.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVWorld.h"
#include "COffsets.h"
#include <CLogFile.h>

void CIVWorld::AddEntity(CIVEntity * pEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVEntity * pGameEntity = pEntity->GetEntity();
	_asm
	{
		push 0
		push pGameEntity
		call COffsets::FUNC_CWorld__AddEntity
		add esp, 8
	}
}

void CIVWorld::RemoveEntity(CIVEntity * pEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVEntity * pGameEntity = pEntity->GetEntity();
	_asm
	{
		push 0
		push pGameEntity
		call COffsets::FUNC_CWorld__RemoveEntity
		add esp, 8
	}
}
