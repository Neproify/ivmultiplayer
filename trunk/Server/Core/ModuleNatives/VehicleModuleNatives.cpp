//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "../CNetworkManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CVehicleManager.h"
#include "../CPlayer.h"

extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;

// Vehicle functions
namespace Modules
{

	// createVehicle(model, x, y, z, rx, ry, rz, color1, color2, color3, color4)
	// TODO: Rotation and colors optional
	int CVehicleModuleNatives::Create(int iModelId, CVector3 vecPosition, CVector3 vecRotation, int color1, int color2, int color3, int color4, int respawn_delay)
	{
		if(iModelId < 0 || iModelId == 41 || iModelId == 96 || iModelId == 107 || iModelId == 111 || iModelId > 123)
		{
			#ifdef IVMP_DEBUG
				CLogFile::Printf("Invalid vehicle model (%d)", iModelId);
			#endif
			return INVALID_ENTITY_ID;
		}

		if(iModelId >= 0 && iModelId <= 125)
		{
			return g_pVehicleManager->Add(iModelId, vecPosition, vecRotation, color1, color2, color3, color4, respawn_delay);
		}

		return INVALID_ENTITY_ID;
	}

	// deleteVehicle(vehicleid)
	bool CVehicleModuleNatives::Delete(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			g_pVehicleManager->Remove(vehicleid);
			return true;
		}

		return false;
	}

	// setVehicleCoordinates(vehicleid, x, y, z)
	bool CVehicleModuleNatives::SetCoordinates(EntityId vehicleid, CVector3 vecPosition)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);
		
			if(pVehicle)
			{
				pVehicle->SetPosition(vecPosition);
				return true;
			}
		}

		return false;
	}

	// getVehicleCoordinates(vehicleid)
	CVector3 CVehicleModuleNatives::GetCoordinates(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				CVector3 vecPosition;
				pVehicle->GetPosition(vecPosition);
				return vecPosition;
			}
		}

		return CVector3();
	}

	// setVehicleRotation(vehicleid, rotation)
	bool CVehicleModuleNatives::SetRotation(EntityId vehicleId, CVector3 vecRotation)
	{
		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

			if(pVehicle)
			{
				pVehicle->SetRotation(vecRotation);
				return true;
			}
		}

		return false;
	}

	// setVehicleDirtLevel(vehicleid, level)
	bool CVehicleModuleNatives::SetDirtLevel(EntityId vehicleid, float fLevel)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->SetDirtLevel(fLevel);
				return true;
			}
		}

		return false;
	}

	float CVehicleModuleNatives::GetDirtLevel(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->GetDirtLevel();
			return 1;
		}

		return -1.0f;
	}

	// setVehicleSirenState(vehicleid, state)
	bool CVehicleModuleNatives::SetSirenState(EntityId vehicleId, bool sqbState)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetSirenState(sqbState != 0);
			return true;
		}

		return false;
	}

	// getVehicleSirenState(vehicleid)
	bool CVehicleModuleNatives::GetSirenState(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->GetSirenState();
		}

		return false;
	}

	// soundVehicleHorn(vehicleid, duration)
	bool CVehicleModuleNatives::SoundHorn(EntityId vehicleId, int iDuration)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SoundHorn(iDuration);
			return true;
		}

		return false;
	}

	// getVehicleRotation(vehicleid)
	CVector3 CVehicleModuleNatives::GetRotation(EntityId vehicleId)
	{
		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

			if(pVehicle)
			{
				CVector3 vecRotation;
				pVehicle->GetRotation(vecRotation);
				return vecRotation;
			}
		}

		return CVector3();
	}

	// isVehicleValid(vehicleid)
	bool CVehicleModuleNatives::IsValid(EntityId vehicleid)
	{
		return g_pVehicleManager->DoesExist(vehicleid);
	}

	// setVehicleColor(vehicleid, color1, color2, color3, color4)
	// TODO: Only require two colors
	bool CVehicleModuleNatives::SetColor(EntityId vehicleid, int * colors)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->SetColors(colors[0], colors[1], colors[2], colors[3]);
				return true;
			}
		}

		return false;
	}

	// getVehicleColor(vehicleid)
	BYTE * CVehicleModuleNatives::GetColor(EntityId vehicleid)
	{
		BYTE * byteColors = new BYTE[4];
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
				return byteColors;
			}
		}

		return NULL;
	}

	// getVehicleModel(vehicleid)
	int CVehicleModuleNatives::GetModel(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				return pVehicle->GetModel();
			}
		}

		return -1;
	}

	// setVehicleHealth(vehicleid, health)
	bool CVehicleModuleNatives::SetHealth(EntityId vehicleid, int health)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->SetHealth(health);
				return true;
			}
		}

		return false;
	}

	// getVehicleHealth(vehicleid)
	int CVehicleModuleNatives::GetHealth(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				return pVehicle->GetHealth();
			}
		}

		return -1;
	}

	// setVehicleVelocity(vehicleid, x, y, z)
	bool CVehicleModuleNatives::SetVelocity(EntityId vehicleid, CVector3 vecMoveSpeed)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->SetMoveSpeed(vecMoveSpeed);
				return true;
			}
		}

		return false;
	}

	// getVehicleVelocity(vehicleid)
	CVector3 CVehicleModuleNatives::GetVelocity(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				CVector3 vecMoveSpeed;
				pVehicle->GetMoveSpeed(vecMoveSpeed);
				return vecMoveSpeed;
			}
		}

		return CVector3();
	}

	// setVehicleAngularVelocity(vehicleid, x, y, z)
	bool CVehicleModuleNatives::SetAngularVelocity(EntityId vehicleid, CVector3 vecTurnSpeed)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				pVehicle->SetTurnSpeed(vecTurnSpeed);
				return true;
			}
		}

		return false;
	}

	// getVehicleAngularVelocity(vehicleid)
	CVector3 CVehicleModuleNatives::GetAngularVelocity(EntityId vehicleid)
	{
		if(g_pVehicleManager->DoesExist(vehicleid))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

			if(pVehicle)
			{
				CVector3 vecTurnSpeed;
				pVehicle->GetTurnSpeed(vecTurnSpeed);
				return vecTurnSpeed;
			}
		}

		return CVector3();
	}

	// respawnVehicle(vehicleid)
	bool CVehicleModuleNatives::Respawn(EntityId vehicleId)
	{
		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

			if(pVehicle)
			{
				pVehicle->Respawn();
				return true;
			}
		}

		return false;
	}

	// isVehicleOccupied(vehicleid)
	bool CVehicleModuleNatives::IsOccupied(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->IsOccupied();
		}

		return false;
	}

	int * CVehicleModuleNatives::GetOccupants(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			int * occupants = new int[MAX_VEHICLE_PASSENGERS+1];
			for(int i = 0; i <= MAX_VEHICLE_PASSENGERS; i++)
			{
				CPlayer * pPlayer = pVehicle->GetOccupant(i);
				if(pPlayer)
					occupants[0] = pPlayer->GetPlayerId();
				else
					occupants[i] = INVALID_ENTITY_ID;
			}
			return occupants;
		}

		return false;
	}
	/*
	// getVehicleOccupants(vehicleid)
	SQInteger CVehicleModuleNatives::GetOccupants(SQVM * pVM)
	{
		EntityId vehicleId;
		sq_getentity(pVM, -1, &vehicleId);

		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			CSquirrelArguments args;

			for(BYTE i = 0; i < (MAX_VEHICLE_PASSENGERS + 1); i++)
			{
				args.push((int)(i + 1));
				CPlayer * pOccupant = pVehicle->GetOccupant(i);
				args.push(pOccupant ? (int)pOccupant->GetPlayerId() : (int)INVALID_ENTITY_ID);
			}

			sq_pusharg(pVM, CSquirrelArgument(args, false));
			return 1;
		}

		sq_pushbool(pVM, false);
		return 1;
	}
	// ADAMIX: todo
	*/


	// setVehicleLocked(vehicleid, locked)
	bool CVehicleModuleNatives::SetLocked(EntityId vehicleId, int iLocked)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->SetLocked(iLocked);
		}

		return false;
	}

	// getVehicleLocked(vehicleid)
	int CVehicleModuleNatives::GetLocked(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->GetLocked();
		}

		return -1;
	}

	// setVehicleIndicators(vehicleid, frontleft, frontright, backleft, backright)
	bool CVehicleModuleNatives::SetIndicators(EntityId vehicleId, bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetIndicatorState(bFrontLeft, bFrontRight, bBackLeft, bBackRight);
			return true;
		}

		return false;
	}

	int * CVehicleModuleNatives::GetIndicators(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			int * i = new int[4];
			for(unsigned char uc = 0; uc <= 3; ++ uc)
			{
				i[uc] = pVehicle->GetIndicatorState(uc);
			}
			return i;
		}

		return NULL;
	}

	bool CVehicleModuleNatives::SetComponent(EntityId vehicleId, int iSlot, bool bOn)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			if(iSlot >= 0 && iSlot <= 8)
			{
				pVehicle->SetComponentState((unsigned char)iSlot, bOn != 0);
				return true;
			}
		}

		return false;
	}

	bool CVehicleModuleNatives::ResetComponents(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			pVehicle->ResetComponents();
			return true;
		}

		return false;
	}

	int * CVehicleModuleNatives::GetComponents(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			int * ic = new int[10];
			for(unsigned char i = 0; i < 10; ++ i)
			{
				ic[i] = pVehicle->GetComponentState(i);
			}
			return ic;
		}

		return NULL;
	}

	bool CVehicleModuleNatives::SetVariation(EntityId vehicleId, int iVariation)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetVariation((unsigned char)iVariation);
			return true;
		}

		return false;
	}

	int CVehicleModuleNatives::GetVariation(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			return pVehicle->GetVariation();
		}

		return -1;
	}

	bool CVehicleModuleNatives::SetEngineStatus(EntityId vehicleId, bool bEngineStatusx)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetEngineStatus(bEngineStatusx);
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::GetEngineStatus(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			return pVehicle->GetEngineStatus();
		}

		return false;
	}

	bool CVehicleModuleNatives::SwitchTaxiLights(EntityId vehicleId, bool check)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			bool bToggle = (check != 0);
			pVehicle->TurnTaxiLights(bToggle);
			return true;
		}
		return false;
	}

	bool CVehicleModuleNatives::ControlCarDoors(EntityId vehicleId, int door, bool door2, float door3)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
		if(pVehicle)
		{
			bool bToggle = (door != 0);
			bool bToggle2 = (door2 != 0);
			bool bToggle3 = (door3 != 0);
			pVehicle->SetCarDoorAngle(bToggle,bToggle2,bToggle3);
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::SetLights(EntityId vehicleId, bool bLights)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			bool bToggle = (bLights != 0);
			pVehicle->SetLights(bToggle);
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::GetLights(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->GetLights();
		}

		return false;
	}

	bool CVehicleModuleNatives::GetTaxiLights(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			return pVehicle->GetTaxiLights();
		}
		return false;
	}

	bool CVehicleModuleNatives::RepairWheels(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->RepairWheels();
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::RepairWindows(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->RepairWindows();
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::Repair(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->RepairVehicle();
			return true;
		}

		return false;
	}

	bool CVehicleModuleNatives::SetDimension(EntityId vehicleId, int dimension)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetDimension(dimension);
			return true;
		}

		return false;
	}

	int CVehicleModuleNatives::GetDimension(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
			return pVehicle->GetDimension();

		return -1;
	}

	bool CVehicleModuleNatives::SetRespawnDelay(EntityId vehicleId, int respawn_delay)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetRespawnDelay(respawn_delay);
			return true;
		}

		return false;
	}

	int CVehicleModuleNatives::GetRespawnDelay(EntityId vehicleId)
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
			return pVehicle->GetRespawnDelay();

		return -1;
	}
}
