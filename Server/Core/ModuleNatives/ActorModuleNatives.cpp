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
}
