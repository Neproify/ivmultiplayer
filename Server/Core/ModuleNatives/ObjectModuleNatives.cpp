//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "../CObjectManager.h"
#include "../CNetworkManager.h"

extern CObjectManager * g_pObjectManager;
extern CNetworkManager * g_pNetworkManager;

// Object functions
namespace Modules
{
	EntityId CObjectModuleNatives::Create(int modelhash, CVector3 vecPosition, CVector3 vecRotation)
	{
		return g_pObjectManager->Create(modelhash, vecPosition, vecRotation);
	}

	bool CObjectModuleNatives::Delete(EntityId objectId)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->Delete(objectId);
			return true;
		}

		return false;
	}

	int CObjectModuleNatives::GetModel(EntityId objectId)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			return g_pObjectManager->GetModel(objectId);
		}

		return -1;
	}

	bool CObjectModuleNatives::SetCoordinates(EntityId objectId, CVector3 vecPosition)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			return g_pObjectManager->SetPosition(objectId, vecPosition);
		}

		return false;
	}

	CVector3 CObjectModuleNatives::GetCoordinates(EntityId objectId)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			CVector3 vecPosition;

			if(g_pObjectManager->GetPosition(objectId, vecPosition))
			{
				return vecPosition;
			}
		}

		return CVector3();
	}


	bool CObjectModuleNatives::SetRotation(EntityId objectId, CVector3 vecRotation)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			return g_pObjectManager->SetRotation(objectId, vecRotation);
		}

		return false;
	}

	CVector3 CObjectModuleNatives::GetRotation(EntityId objectId)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			CVector3 vecRotation;

			if(g_pObjectManager->GetRotation(objectId, vecRotation))
			{
				return vecRotation;
			}
		}

		return CVector3();
	}

	bool CObjectModuleNatives::Move(EntityId objectId, CVector3 vecMoveTarget, CVector3 vecMoveRot, int iTime)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, iTime, true, vecMoveRot);
			return true;
		}

		return false;
	}
	
	bool CObjectModuleNatives::Rotate(EntityId objectId, CVector3 vecRotation, int iTime)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->RotateObject(objectId, vecRotation, iTime);
			return true;
		}

		return false;
	}

	bool CObjectModuleNatives::AttachToPlayer(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot, int iBone)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot, iBone);
			return true;
		}
		return false;
	}

	bool CObjectModuleNatives::SetDimension(EntityId objectId, unsigned char ucDimension)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->SetDimension(objectId, ucDimension);
			return true;
		}

		return false;
	}
	
	unsigned char CObjectModuleNatives::GetDimension(EntityId objectId)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			return g_pObjectManager->GetDimension(objectId);
		}

		return 0;
	}

	void CObjectModuleNatives::CreateExplosion(CVector3 vecPos, unsigned int uiExplosionType, float fRadius, bool bSound, bool bInvisible, float fCameraShake)
	{
		g_pObjectManager->CreateExplosion(vecPos, uiExplosionType, fRadius, bSound, bInvisible, fCameraShake);
	}

	EntityId CObjectModuleNatives::CreateFire(CVector3 vecPos, float fDensity)
	{
		return g_pObjectManager->CreateFire(vecPos,fDensity);
	}

	void CObjectModuleNatives::DeleteFire(EntityId fireId)
	{
		g_pObjectManager->DeleteFire(fireId);
	}

	bool CObjectModuleNatives::AttachPed(EntityId objectId, EntityId playerId, CVector3 vecPos, CVector3 vecRot)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot);
			return true;
		}
		return false;
	}

	bool CObjectModuleNatives::AttachVehicle(EntityId objectId, EntityId vehicleId, CVector3 vecPos, CVector3 vecRot)
	{
		if(g_pObjectManager->DoesExist(objectId))
		{
			g_pObjectManager->AttachToVehicle(objectId,vehicleId,vecPos,vecRot);
			return true;
		}
		return false;
	}
}