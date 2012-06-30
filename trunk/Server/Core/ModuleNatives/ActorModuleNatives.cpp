//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "Scripting/CScriptingManager.h"
#include "../CActorManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CActorManager * g_pActorManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Actor functions
namespace Modules
{
	// createActor(modelhash, x, y, z, r)
	EntityId CActorModuleNatives::Create(int iModelId, CVector3 vecPosition, float fHeading)
	{
		return g_pActorManager->Create(iModelId, vecPosition, fHeading);
	}

	// deleteActor(actorid)
	bool CActorModuleNatives::Delete(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->Delete(actorId);
			return true;
		}

		return false;
	}

	// isActorValid(actorid)
	bool CActorModuleNatives::IsValid(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// setActorCoordinates(actorid, x, y, z)
	bool CActorModuleNatives::SetCoordinates(EntityId actorId, CVector3 vecPosition)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->SetPosition(actorId, vecPosition);
			return true;
		}

		return false;
	}

	// getActorCoordinates(actorid)
	CVector3 CActorModuleNatives::GetCoordinates(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return g_pActorManager->GetPosition(actorId);
		}
		return CVector3();
	}

	// setActorHeading(actorid, r)
	bool CActorModuleNatives::SetHeading(EntityId actorId, float fHeading)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->SetHeading(actorId, fHeading);
			return true;
		}

		return false;
	}

	// actorWalkToCoordinatesForPlayer(playerid, actorid, x, y, z)
	bool CActorModuleNatives::WalkToCoordinates(EntityId playerId, EntityId actorId, CVector3 vecPosition, int iType)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(actorId);
			bsSend.Write(vecPosition);
			bsSend.Write(iType);
			g_pNetworkManager->RPC(RPC_ScriptingActorWalkToCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			return true;
		}

		return false;
	}

	// getActorHeading(actorid)
	float CActorModuleNatives::GetHeading(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return g_pActorManager->GetHeading(actorId);
		}
		return 0.0f;
	}

	// getActorModel(actorid)
	int CActorModuleNatives::GetModel(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return g_pActorManager->GetModel(actorId);
		}
		return 0;
	}

	// getActorCount()
	int CActorModuleNatives::GetCount()
	{
		if(g_pActorManager)
		{
			return g_pActorManager->GetActorCount();
		}
		return 0;
	}

	// setActorName(actorid, name)
	bool CActorModuleNatives::SetName(EntityId actorId, const char * szName)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->SetActorName(actorId, szName);
			return true;
		}

		return false;
	}

	// getActorName(actorid)
	const char * CActorModuleNatives::GetName(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return g_pActorManager->GetActorName(actorId);
		}
		return NULL;
	}

	// toggleActorNametag(actorid, toggle)
	bool CActorModuleNatives::ToggleNametag(EntityId actorId, bool bShow)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			bool bToggle = (bShow != 0);
			g_pActorManager->ToggleNametag(actorId, bToggle);
			return true;
		}
		return false;
	}

	// toggleActorBlip(actorid, toggle)
	bool CActorModuleNatives::ToggleBlip(EntityId actorId, bool bShow)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			bool bToggle = (bShow != 0);
			g_pActorManager->ToggleBlip(actorId, bToggle);
			return true;
		}
		return false;
	}
	// setActorNametagColor(actorid, color)
	bool CActorModuleNatives::SetColor(EntityId actorId, int color)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->SetColor(actorId,color);
			return true;
		}
		return false;
	}

	// getActorNametagColor(actorid)
	int CActorModuleNatives::GetColor(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			return g_pActorManager->GetColor(actorId);
		}
		return 0;
	}

	// toggleActorFrozen(actorid, toggle)
	bool CActorModuleNatives::ToggleFrozen(EntityId actorId, bool frozen)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			bool bToggle = (frozen != 0);
			g_pActorManager->ToggleFrozen(actorId, bToggle);
			return true;
		}

		return false;
	}

	// toggleActorHelmet(actorid, toggle)
	bool CActorModuleNatives::ToggleHelmet(EntityId actorId, bool helmet)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			bool bToggle = (helmet != 0);
			g_pActorManager->ToggleHelmet(actorId, bToggle);
			return true;
		}
		return false;
	}

	// warpActorIntoVehicle(actorid, vehicleid, seatid)
	bool CActorModuleNatives::WarpIntoVehicle(EntityId actorId, int vehicleid, int seatid)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			if(seatid > 0 &&  seatid <= 3)
			{
				g_pActorManager->WarpIntoVehicle(actorId,vehicleid,seatid);
				return true;
			}
			else
			{
				CLogFile::Printf("Can't warp actor %d on the seat %d from the vehicle %d(Seats are only supported from 1 to 3(passenger))",actorId,seatid,vehicleid);
			}
		}
		return false;
	}

	// removeActorFromVehicle(actorid)
	bool CActorModuleNatives::RemoveFromVehicle(EntityId actorId)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			g_pActorManager->RemoveFromVehicle(actorId);
			return true;
		}
		return false;
	}

	void CActorModuleNatives::DeleteAll()
	{
		for(EntityId x = 0; x < MAX_ACTORS; x++)
		{
			if(g_pActorManager->DoesExist(x))
			{
				g_pActorManager->Delete(x);
			}
		}
	}

	bool CActorModuleNatives::ForceAnim(EntityId actorId, const char * szGroup, const char * szAnim)
	{
		if(g_pActorManager->DoesExist(actorId))
		{
			CBitStream bsSend;
			bsSend.Write(actorId);
			bsSend.Write(String(szGroup));
			bsSend.Write(String(szAnim));
			g_pNetworkManager->RPC(RPC_ScriptingForceActorAnimation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			return true;
		}
		return false;
	}
}
