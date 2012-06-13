//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManagerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CObjectManagerInterface
{
public:
	virtual EntityId Create(DWORD dwModelHash, const CVector3& vecPosition, const CVector3& vecRotation) = 0;
	virtual void Delete(EntityId objectId) = 0;
	virtual bool DoesExist(EntityId objectId) = 0;

	virtual EntityId GetObjectCount() = 0;

	virtual DWORD GetModel(EntityId objectId) = 0;
	virtual bool SetPosition(EntityId objectId, const CVector3& vecPosition) = 0;
	virtual bool GetPosition(EntityId objectId, CVector3& vecPosition) = 0;
	virtual bool SetRotation(EntityId objectId, const CVector3& vecRotation) = 0;
	virtual bool GetRotation(EntityId objectId, CVector3& vecRotation) = 0;

	virtual EntityId CreateFire(const CVector3& vecPosition, float fdensity) = 0;
	virtual void DeleteFire(EntityId fireId) = 0;
	virtual void HandleClientJoinFire(EntityId playerId) = 0;
	virtual void CreateExplosion(const CVector3& vecPosition, float fdensity) = 0;
};
