//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"

struct _Object
{
	DWORD dwModelHash;
	CVector3 vecSpawnPos;
	CVector3 vecSpawnRot;
};

class CObjectManager : public CObjectManagerInterface
{
private:
	bool m_bActive[MAX_OBJECTS];
	_Object m_Objects[MAX_OBJECTS];

public:
	CObjectManager();
	~CObjectManager();

	EntityId Create(DWORD dwModelHash, float fX, float fY, float fZ, float fRX, float fRY, float fRZ);
	void Delete(EntityId objectId);
	void HandleClientJoin(EntityId playerId);
	bool DoesExist(EntityId objectId);

	EntityId GetObjectCount();

	DWORD GetModel(EntityId objectId);
	bool SetPosition(EntityId objectId, CVector3 vecPosition);
	bool GetPosition(EntityId objectId, CVector3* vecPosition);
	bool SetRotation(EntityId objectId, CVector3 vecRotation);
	bool GetRotation(EntityId objectId, CVector3* vecRotation);
};
