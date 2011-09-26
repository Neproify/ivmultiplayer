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
	CVector3 vecPosition;
	CVector3 vecRotation;
};

class CObjectManager : public CObjectManagerInterface
{
private:
	bool    m_bActive[MAX_OBJECTS];
	_Object m_Objects[MAX_OBJECTS];

public:
	CObjectManager();
	~CObjectManager();

	EntityId Create(DWORD dwModelHash, const CVector3& vecPosition, const CVector3& vecRotation);
	void     Delete(EntityId objectId);
	void     HandleClientJoin(EntityId playerId);
	bool     DoesExist(EntityId objectId);

	EntityId GetObjectCount();

	DWORD    GetModel(EntityId objectId);
	bool     SetPosition(EntityId objectId, const CVector3& vecPosition);
	bool     GetPosition(EntityId objectId, CVector3& vecPosition);
	bool     SetRotation(EntityId objectId, const CVector3& vecRotation);
	bool     GetRotation(EntityId objectId, CVector3& vecRotation);
};
