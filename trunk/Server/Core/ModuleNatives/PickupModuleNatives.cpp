//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "../CPickupManager.h"

extern CPickupManager * g_pPickupManager;

// Pickup functions
namespace Modules
{
	// createPickup(modelhash, type, value?, x, y, z, rx, ry, rz)
	EntityId CPickupModuleNatives::Create(int modelhash, unsigned char type, unsigned int value, CVector3 vecPosition, CVector3 vecRotation)
	{
		return g_pPickupManager->Create(modelhash, type, value, vecPosition.fX, vecPosition.fY, vecPosition.fZ, vecRotation.fX, vecRotation.fY, vecRotation.fZ);
	}

	// deletePickup(pickupid)
	bool CPickupModuleNatives::Delete(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			g_pPickupManager->Delete(pickupid);
			return true;
		}

		return false;
	}

	// getPickupModel(pickupid)
	int CPickupModuleNatives::GetModel(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->GetModel(pickupid);
		}

		return -1;
	}

	// getPickupType(pickupid)
	int CPickupModuleNatives::GetType(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->GetType(pickupid);
		}

		return -1;
	}

	// setPickupValue(pickupid, value)
	bool CPickupModuleNatives::SetValue(EntityId pickupid, unsigned int pvalue)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->SetValue(pickupid, pvalue);
		}

		return false;
	}

	// getPickupValue(pickupid)
	unsigned int CPickupModuleNatives::GetValue(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->GetValue(pickupid);
		}

		return -1;
	}

	// setPickupCoordinates(Pickupid, x, y, z)
	bool CPickupModuleNatives::SetCoordinates(EntityId pickupid, CVector3 vecPosition)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->SetPosition(pickupid, vecPosition);
		}

		return false;
	}

	// getPickupCoordinates(Pickupid)
	CVector3 CPickupModuleNatives::GetCoordinates(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			CVector3 vecPosition;
			if(g_pPickupManager->GetPosition(pickupid, &vecPosition))
			{
				return vecPosition;
			}
		}

		return CVector3();
	}


	// setPickupRotation(Pickupid, x, y, z)
	bool CPickupModuleNatives::SetRotation(EntityId pickupid, CVector3 vecPosition)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			return g_pPickupManager->SetRotation(pickupid, vecPosition);
		}

		return false;
	}

	// getPickupRotation(Pickupid)
	CVector3 CPickupModuleNatives::GetRotation(EntityId pickupid)
	{
		if(g_pPickupManager->DoesExist(pickupid))
		{
			CVector3 vecPosition;
			if(g_pPickupManager->GetRotation(pickupid, &vecPosition))
			{
				return vecPosition;
			}
		}

		return CVector3();
	}
}