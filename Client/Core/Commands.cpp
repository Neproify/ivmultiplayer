//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Commands.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CInputWindow.h"
#include "CChatWindow.h"
#include "CNetworkManager.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CModelManager.h"
#include "SharedUtility.h"
#include "CFPSCounter.h"

extern CInputWindow * g_pInputWindow;
extern CChatWindow * g_pChatWindow;
extern CLocalPlayer * g_pLocalPlayer;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CModelManager * g_pModelManager;
extern CFPSCounter * g_pFPSCounter;

void ChangeChatBackgroundColor(char * szParams)
{
	int a,r,g,b;
	sscanf(szParams, "%d %d %d %d", &r, &g, &b, &a);
}

void QuitCommand(char * szParams)
{
	if(g_pNetworkManager)
		g_pNetworkManager->Disconnect();

	ExitProcess(0);
}

void QuickQuitCommand(char * szParams)
{
	if(g_pNetworkManager)
		g_pNetworkManager->Disconnect();

	TerminateProcess(GetCurrentProcess(), 0);
}

void SendPlayerPing(char * szParams)
{
	g_pChatWindow->AddInfoMessage("Your current [FFFFFFAA]Ping: [F60000FF]%d",g_pLocalPlayer->GetPing());
}

void SendPlayerFPS(char * szParams)
{
	g_pChatWindow->AddInfoMessage("Your current [FFFFFFAA]FPS: [F60000FF]%d",g_pFPSCounter->Get());
}

void SavePosCommand(char * szParams)
{
	FILE * file = fopen(SharedUtility::GetAbsolutePath("SavedData.txt"), "a");
	if(!file)
	{
		g_pChatWindow->AddInfoMessage("Failed to open 'SavedData.txt'");
		return;
	}

	CVector3 vecPosition;

	if(g_pLocalPlayer->IsInVehicle())
	{
		CNetworkVehicle * pVehicle = g_pLocalPlayer->GetVehicle();

		if(pVehicle)
		{
			pVehicle->GetPosition(vecPosition);
			CVector3 vecRotation;
			pVehicle->GetRotation(vecRotation);
			BYTE byteColors[4];
			pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
			fprintf(file, "createVehicle(%d, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d);%s%s\n", g_pModelManager->ModelHashToVehicleId(pVehicle->GetModelInfo()->GetHash()), vecPosition.fX, vecPosition.fY, vecPosition.fZ, vecRotation.fX, vecRotation.fY, vecRotation.fZ, byteColors[0], byteColors[1], byteColors[2], byteColors[3], szParams ? " // " : "", szParams ? szParams : "");
		}
	}
	else
	{
		g_pLocalPlayer->GetPosition(vecPosition);
		float fHeading = g_pLocalPlayer->GetCurrentHeading();
		int iModelId = ModelHashToSkinId(g_pLocalPlayer->GetModelInfo()->GetHash());
		fprintf(file, "PlayerData(%d, %f, %f, %f, %f);%s%s\n", iModelId, vecPosition.fX, vecPosition.fY, vecPosition.fZ, fHeading, szParams ? " // " : "", szParams ? szParams : "");
	}

	fclose(file);
	g_pChatWindow->AddInfoMessage("Position data saved to 'SavedData.txt'");
}

#ifdef DEBUG_COMMANDS_ENABLED
DWORD SkinIdToModelHash(int modelid);

void AddPlayerCommand(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	g_pPlayerManager->Add(playerId, "jenkstax");
	g_pPlayerManager->Spawn(playerId, 35, CVector3(-341.36f, 1144.80f, 14.79f), 40.114815f);
}

void DeletePlayerCommand(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	g_pPlayerManager->Remove(playerId);
}

/*void AddVehicleCommand(char * szParams)
{
	int vehicleId = 0;
	sscanf(szParams, "%d", &vehicleId);
	CVector3 vecPos;
	float fHeading;
	g_pLocalPlayer->GetPosition(&vecPos);
	vecPos.Z += 5;
	fHeading = g_pLocalPlayer->GetHeading();
	CNetworkVehicle * pVehicle = g_pVehicleManager->Add(vehicleId, 3);
	VehicleColors colors;
	pVehicle->Create(vecPos, fHeading, colors);
	pVehicle->SetPosition(vecPos);
	pVehicle->SetHeading(fHeading);
	g_pLocalPlayer->EnterVehicle(vehicleId, 0, true);
}

void DeleteVehicleCommand(char * szParams)
{
	int vehicleId = 0;
	sscanf(szParams, "%d", &vehicleId);
	g_pVehicleManager->Remove(vehicleId);
}*/

/*void PutPlayerInVehicle(char * szParams)
{
	int playerId = 0;
	int vehicleId = 0;
	sscanf(szParams, "%d %d", &playerId, &vehicleId);
	CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			pPlayer->EnterVehicle(vehicleId, 0, true);
		}
	}
}

void PlayerEnterVehicle(char * szParams)
{
	int playerId = 0;
	int vehicleId = 0;
	sscanf(szParams, "%d %d", &playerId, &vehicleId);
	CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		if(g_pVehicleManager->DoesExist(vehicleId))
		{
			pPlayer->EnterVehicle(vehicleId, 0, false);
		}
	}
}*/

/*void GivePlayerWeapon(char * szParams)
{
	int playerId = 0;
	int weaponId = 0;
	int ammo = 0;
	sscanf(szParams, "%d %d %d", &playerId, &weaponId, &ammo);
	CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		pPlayer->GiveWeapon(weaponId, ammo);
	}
}*/

#if 0
bool bNetworkSettings = false;

void NetworkSettings(char * szParams)
{
	// TODO: Import from ScriptingDirty.h
	/*if(!bNetworkSettings)
		Scripting::ActivateNetworkSettingsMenu();
	else
		Scripting::DeactivateNetworkSettingsMenu();*/

	bNetworkSettings = !bNetworkSettings;
}
#endif

/*void LastDamageEntity(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		CLogFile::Printf("LastDamageEntity(0x%p), LocalPed(0x%p), LocalVehicle(0x%p)", pPlayer->GetPlayerPed()->GetPed()->GetLastDamageEntity(), GetLocalPed(), GetLocalPed()->GetCurrentVehicle());
	}
}*/

/*void AddPlayerEntity(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(playerId);
	//CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	//if(pPlayer)
	if(pVehicle)
	{
		//CGame::AddEntityToWorld(pPlayer->GetPlayerPed()->m_pPed);
		CGame::AddEntityToWorld(pVehicle->GetGameVehicle());
	}
}

void RemovePlayerEntity(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(playerId);
	//CRemotePlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	//if(pPlayer)
	if(pVehicle)
	{
		//pPlayer->StopMoving();
		pVehicle->StopMoving();
		//CGame::AddEntityToWorld(pPlayer->GetPlayerPed()->m_pPed);
		//typedef char (__thiscall * sub_9E8630_t)(int pthis);
		//sub_9E8630_t psub_9E8630 = (sub_9E8630_t)(CGame::GetBase() + 0x9E8630);
		CLogFile::Printf("Result before call is %d", pVehicle->GetGameVehicle()->m_dwFlags2);
		pVehicle->GetGameVehicle()->RemoveFromWorld();
		CLogFile::Printf("Result after call is %d", pVehicle->GetGameVehicle()->m_dwFlags2);
	}
}*/

void VehicleToMe(char * szParams)
{
	int vehicleId = 0;
	sscanf(szParams, "%d", &vehicleId);
	CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecPosition;
		g_pLocalPlayer->GetPosition(&vecPosition);
		pVehicle->GetGameVehicle()->SetPosition(&vecPosition);
		pVehicle->GetGameVehicle()->RemoveFromWorld();
		pVehicle->GetGameVehicle()->AddToWorld();
	}
}
#endif

void RegisterCommands()
{
	g_pInputWindow->RegisterCommand("q", QuitCommand);
	g_pInputWindow->RegisterCommand("quit", QuitCommand);
	g_pInputWindow->RegisterCommand("qq", QuickQuitCommand);
	g_pInputWindow->RegisterCommand("quickquit", QuickQuitCommand);
	g_pInputWindow->RegisterCommand("savepos", SavePosCommand);
	g_pInputWindow->RegisterCommand("ping", SendPlayerPing);
	g_pInputWindow->RegisterCommand("fps", SendPlayerFPS);
	#ifdef DEBUG_COMMANDS_ENABLED
	g_pInputWindow->RegisterCommand("ap", AddPlayerCommand);
	g_pInputWindow->RegisterCommand("dp", DeletePlayerCommand);
	/*g_pInputWindow->RegisterCommand("av", AddVehicleCommand);
	g_pInputWindow->RegisterCommand("dv", DeleteVehicleCommand);*/
	/*g_pInputWindow->RegisterCommand("ppiv", PutPlayerInVehicle);
	g_pInputWindow->RegisterCommand("pev", PlayerEnterVehicle);*/
	/*g_pInputWindow->RegisterCommand("gpw", GivePlayerWeapon);
	g_pInputWindow->RegisterCommand("ns", NetworkSettings);
	g_pInputWindow->RegisterCommand("lde", LastDamageEntity);*/
	/*g_pInputWindow->RegisterCommand("ape", AddPlayerEntity);
	g_pInputWindow->RegisterCommand("rpe", RemovePlayerEntity);*/
	g_pInputWindow->RegisterCommand("vtm", VehicleToMe);
	#endif
}
