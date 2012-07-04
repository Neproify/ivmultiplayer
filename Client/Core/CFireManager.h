//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.h
// Project: Client.Core
// Author(s): FRi
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once
#include <Common.h>

struct _Fire
{
	unsigned int uiHandle;
	float fdensity;
	CVector3 vecPos;
};

class CFireManager
{
private:
	bool m_bActive[MAX_FIRE];
	_Fire m_Fire[MAX_FIRE];

public:
	CFireManager();
	~CFireManager();

	void Create(EntityId fireId, CVector3 vecPos, float fdensity);
	void Delete(EntityId fireId);
	bool DoesExist(EntityId fireId) { return m_bActive[fireId]; };
};
