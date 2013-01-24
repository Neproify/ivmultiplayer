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
	DWORD			dwModelHash;
	CVector3		vecPosition;
	CVector3		vecRotation;
	bool			bAttached;
	bool			bVehicleAttached;
	unsigned int	uiVehiclePlayerId;
	CVector3		vecAttachPosition;
	CVector3		vecAttachRotation;
	DimensionId 	ucDimension;
	unsigned int	iBone;
	int				m_iInterior;
};

struct _Fire
{
	CVector3	vecPosition;
	float		fdensity;
};

class CObjectManager : public CObjectManagerInterface
{
private:
	bool    m_bActive[MAX_OBJECTS];
	_Object m_Objects[MAX_OBJECTS];
	bool    m_bFireActive[MAX_OBJECTS];
	_Fire	m_FireObject[MAX_OBJECTS];

public:
	CObjectManager();
	~CObjectManager();

	EntityId		Create(DWORD dwModelHash, const CVector3& vecPosition, const CVector3& vecRotation);
	void			Delete(EntityId objectId);
	void			HandleClientJoin(EntityId playerId);
	bool			DoesExist(EntityId objectId);

	EntityId		GetObjectCount();
	bool			GetAttachState(EntityId objectId) { return m_Objects[objectId].bAttached; }
	bool			IsVehicleAttached(EntityId objectId) { return m_Objects[objectId].bVehicleAttached; }
	unsigned int	GetAttachId(EntityId objectId) { return m_Objects[objectId].uiVehiclePlayerId; }

	DWORD			GetModel(EntityId objectId);
	bool			SetPosition(EntityId objectId, const CVector3& vecPosition);
	bool			GetPosition(EntityId objectId, CVector3& vecPosition);
	bool			SetRotation(EntityId objectId, const CVector3& vecRotation);
	bool			GetRotation(EntityId objectId, CVector3& vecRotation);
	void			AttachToVehicle(EntityId objectId, EntityId vehicleId,const CVector3& vecPos, const CVector3& vecRot);
	void			AttachToPlayer(EntityId objectId, EntityId playerId, const CVector3& vecPos, const CVector3& vecRot, int iBone = -1);
	void			MoveObject(EntityId objectId, const CVector3& vecMoveTarget, int iTime, bool bHasRot = false, const CVector3& vecMoveRot = CVector3());
	void			RotateObject(EntityId objectId, const CVector3& vecMoveRot, int iTime);
	void			Detach(EntityId objectId);

	EntityId		CreateFire(const CVector3& vecPosition, float fdensity);
	void			DeleteFire(EntityId fireId);
	void			HandleClientJoinFire(EntityId playerId);
	void			CreateExplosion(const CVector3& vecPosition, float fdensity);
	void			SetDimension(EntityId objectId, DimensionId ucDimension);
	DimensionId 	GetDimension(EntityId objectId) { if(DoesExist(objectId)) return m_Objects[objectId].ucDimension; else return 0; }

	void			SetInterior(EntityId objectId, int iInterior);
	int				GetInterior(EntityId objectId) { return m_Objects[objectId].m_iInterior; }
};
