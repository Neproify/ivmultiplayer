//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleManager.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CVehicleManager.h"
#include "CNetworkManager.h"
#include "Scripting/CScriptingManager.h"
#include <CSettings.h>
#include "CModuleManager.h"
#include "CEvents.h"
#include "SharedUtility.h"

extern CNetworkManager * g_pNetworkManager;
extern CScriptingManager * g_pScriptingManager;
extern CModuleManager * g_pModuleManager;
extern CEvents * g_pEvents;

CVehicleManager::CVehicleManager()
{
	for(EntityId x = 0; x < MAX_VEHICLES; x++)
		m_bActive[x] = false;
}

CVehicleManager::~CVehicleManager()
{
	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(m_bActive[x])
			Remove(x);
	}
}

EntityId CVehicleManager::Add(int iModelId, CVector3 vecSpawnPosition, CVector3 vecSpawnRotation, BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4, int respawn_delay)
{
	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(!m_bActive[x])
		{
			m_pVehicles[x] = new CVehicle(x, iModelId, vecSpawnPosition, vecSpawnRotation, byteColor1, byteColor2, byteColor3, byteColor4);

			if(m_pVehicles[x])
			{
				m_bActive[x] = true;
				m_pVehicles[x]->SetRespawnDelay(respawn_delay);
				CSquirrelArguments pArguments;
				pArguments.push(x);
				g_pEvents->Call("vehicleCreate", &pArguments);

				return x;
			}

			return INVALID_ENTITY_ID;
		}
	}

	return INVALID_ENTITY_ID;
}

void CVehicleManager::Process()
{
	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(m_bActive[x])
		{
			if(m_pVehicles[x]->GetRespawnDelay() > -1) {
				if(m_pVehicles[x]->IsOccupied()) {
					m_pVehicles[x]->SetLastTimeOccupied(SharedUtility::GetTime());
					continue;
				}
				if(m_pVehicles[x]->GetLastTimeOccupied() == 0) {
					m_pVehicles[x]->SetLastTimeOccupied(SharedUtility::GetTime());
					continue;
				}

				//CLogFile::Printf("Checkrespawn ( %i + %i < %i )", m_pVehicles[x]->GetLastTimeOccupied(), m_pVehicles[x]->GetRespawnDelay(), SharedUtility::GetTime());
				if((m_pVehicles[x]->GetLastTimeOccupied() + m_pVehicles[x]->GetRespawnDelay()) < SharedUtility::GetTime()) {
					m_pVehicles[x]->Respawn();
					BYTE colors[4];
					m_pVehicles[x]->GetColors(colors[0], colors[1], colors[2], colors[3]);
					m_pVehicles[x]->SetColors(colors[0], colors[1], colors[2], colors[3]);
					m_pVehicles[x]->SetLastTimeOccupied(SharedUtility::GetTime());
				}
			}
			if(m_pVehicles[x]->GetDeathTime() != 0 && m_pVehicles[x]->GetDeathTime() + 5000 <= SharedUtility::GetTime())
			{
				m_pVehicles[x]->SetDeathTime(0);
				m_pVehicles[x]->Respawn();
			}
		}
	}
}
void CVehicleManager::Remove(EntityId vehicleId)
{
	if(!DoesExist(vehicleId))
		return;

	CSquirrelArguments pArguments;
	pArguments.push(vehicleId);
	g_pEvents->Call("vehicleDelete", &pArguments);

	delete m_pVehicles[vehicleId];
	m_pVehicles[vehicleId] = NULL;
	m_bActive[vehicleId] = false;
}

void CVehicleManager::HandleClientJoin(EntityId playerId)
{
	if(GetVehicleCount() > 0)
	{
		for(EntityId x = 0; x < MAX_VEHICLES; x++)
		{
			if(m_bActive[x]) {
				m_pVehicles[x]->SpawnForPlayer(playerId);

				if(m_pVehicles[x]->GetDeathTime() != 0 && m_pVehicles[x]->GetDeathTime() + 5000 <= SharedUtility::GetTime())
					m_pVehicles[x]->ExplodeCar(playerId);
			}
		}
	}
}

bool CVehicleManager::DoesExist(EntityId vehicleId)
{
	if(vehicleId < 0 || vehicleId >= MAX_VEHICLES)
		return false;
	
	return m_bActive[vehicleId];
}

int CVehicleManager::GetVehicleCount()
{
	int iCount = 0;

	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(DoesExist(x))
			iCount++;
	}

	return iCount;
}

CVehicle * CVehicleManager::GetAt(EntityId vehicleId)
{
	if(!DoesExist(vehicleId))
		return NULL;

	return m_pVehicles[vehicleId];
}
