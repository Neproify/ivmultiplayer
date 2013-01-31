//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Commands.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClient.h"
#include "SharedUtility.h"

extern CClient * g_pClient;

void AddNetworkPlayer(char * szParams)
{
	// Get our player manager
	CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();

	EntityId playerId = -1;
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(!pPlayerManager->DoesExist(i)) {
			playerId = i;
			break;
		}
	}

	if(playerId != -1) {
		// Add the player to the player manager
		pPlayerManager->Add(playerId, "IV:MP NetworkPlayer");
		if(pPlayerManager->DoesExist(playerId)) {
			CVector3 vecPos; g_pClient->GetLocalPlayer()->GetPosition(vecPos);
			pPlayerManager->GetAt(playerId)->SetColor((unsigned int)0xFFFFFFAA);
			pPlayerManager->GetAt(playerId)->SetPosition(vecPos);
			pPlayerManager->GetAt(playerId)->SetModel(7);
			g_pClient->GetChatWindow()->AddInfoMessage("Networkplayer Added");
		}
	}
	else
		g_pClient->GetChatWindow()->AddInfoMessage("Couldn't find free player slot..");
}

void QuitCommand(char * szParams)
{
	// If we are connected to a server, disconnect
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();

	if(pNetworkManager && pNetworkManager->IsConnected())
		pNetworkManager->Disconnect();
	
	// Delete the network manager interface
	SAFE_DELETE(pNetworkManager);

	// Exit
	TerminateProcess(GetCurrentProcess(), 0);
}

void QuickQuitCommand(char * szParams)
{
	CNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();

	if(pNetworkManager)
		pNetworkManager->Disconnect();

	TerminateProcess(GetCurrentProcess(), 0);
}

void GetPing(char * szParams)
{
	g_pClient->GetChatWindow()->AddInfoMessage("Your current [FFFFFFAA]Ping: [F60000FF]%d", g_pClient->GetLocalPlayer()->GetPing());
}

void GetFPS(char * szParams)
{
	g_pClient->GetChatWindow()->AddInfoMessage("Your current [FFFFFFAA]FPS: [F60000FF]%d", g_pClient->GetFPSCounter()->Get());
}

void SavePosCommand(char * szParams)
{
	FILE * file = fopen(SharedUtility::GetAbsolutePath("SavedData.txt"), "a");
	if(!file)
	{
		g_pClient->GetChatWindow()->AddInfoMessage("Failed to open 'SavedData.txt'");
		return;
	}

	CVector3 vecPosition;

	// Get our local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(pLocalPlayer->IsInVehicle())
	{
		CNetworkVehicle * pVehicle = pLocalPlayer->GetVehicle();

		if(pVehicle)
		{
			pVehicle->GetPosition(vecPosition);
			CVector3 vecRotation;
			pVehicle->GetRotation(vecRotation);
			BYTE byteColors[4];
			pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
			fprintf(file, "createVehicle(%d, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d);%s%s\n", g_pClient->GetModelManager()->ModelHashToVehicleId(pVehicle->GetModelInfo()->GetHash()), vecPosition.fX, vecPosition.fY, vecPosition.fZ, vecRotation.fX, vecRotation.fY, vecRotation.fZ, byteColors[0], byteColors[1], byteColors[2], byteColors[3], szParams ? " // " : "", szParams ? szParams : "");
		}
	}
	else
	{
		pLocalPlayer->GetPosition(vecPosition);
		int iModelId = ModelHashToSkinId(pLocalPlayer->GetModelInfo()->GetHash());
		fprintf(file, "PlayerData(%d, %f, %f, %f, %f(%f));%s%s\n", iModelId, vecPosition.fX, vecPosition.fY, vecPosition.fZ, pLocalPlayer->GetCurrentHeading(), pLocalPlayer->GetDesiredHeading(),szParams ? " // " : "", szParams ? szParams : "");
	}

	fclose(file);
	g_pClient->GetChatWindow()->AddInfoMessage("Position data saved to 'SavedData.txt'");
}

void DisableVehicleInfos(char * szParams)
{
	// Get our local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();
	pLocalPlayer->SetVehicleInfos(!pLocalPlayer->GetVehicleInfos());
}
#ifdef DEBUG_COMMANDS_ENABLED

void AddPlayerCommand(char * szParams)
{
	// Get our player manager
	CPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();

	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	pPlayerManager->Add(playerId, "jenkstax");
	pPlayerManager->Spawn(playerId, 35, CVector3(-341.36f, 1144.80f, 14.79f), 40.114815f);
}

void DeletePlayerCommand(char * szParams)
{
	int playerId = 0;
	sscanf(szParams, "%d", &playerId);
	g_pClient->GetPlayerManager()->Remove(playerId);
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
	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecPosition;
		g_pClient->GetLocalPlayer()->GetPosition(&vecPosition);
		pVehicle->GetGameVehicle()->SetPosition(&vecPosition);
		pVehicle->GetGameVehicle()->RemoveFromWorld();
		pVehicle->GetGameVehicle()->AddToWorld();
	}
}
#endif

void RegisterCommands()
{
	// Get our input window
	CInputWindow * pInputWindow = g_pClient->GetInputWindow();

	pInputWindow->RegisterCommand("q", QuitCommand);
	pInputWindow->RegisterCommand("quit", QuitCommand);
	pInputWindow->RegisterCommand("qq", QuickQuitCommand);
	pInputWindow->RegisterCommand("quickquit", QuickQuitCommand);
	pInputWindow->RegisterCommand("savepos", SavePosCommand);
	pInputWindow->RegisterCommand("ping", GetPing);
	pInputWindow->RegisterCommand("fps", GetFPS);
	pInputWindow->RegisterCommand("dvi", DisableVehicleInfos);
	pInputWindow->RegisterCommand("anp", AddNetworkPlayer);
	#ifdef DEBUG_COMMANDS_ENABLED
	pInputWindow->RegisterCommand("ap", AddPlayerCommand);
	pInputWindow->RegisterCommand("dp", DeletePlayerCommand);
	/*pInputWindow->RegisterCommand("av", AddVehicleCommand);
	pInputWindow->RegisterCommand("dv", DeleteVehicleCommand);*/
	/*pInputWindow->RegisterCommand("ppiv", PutPlayerInVehicle);
	pInputWindow->RegisterCommand("pev", PlayerEnterVehicle);*/
	/*pInputWindow->RegisterCommand("gpw", GivePlayerWeapon);
	pInputWindow->RegisterCommand("ns", NetworkSettings);
	pInputWindow->RegisterCommand("lde", LastDamageEntity);*/
	/*pInputWindow->RegisterCommand("ape", AddPlayerEntity);
	pInputWindow->RegisterCommand("rpe", RemovePlayerEntity);*/
	pInputWindow->RegisterCommand("vtm", VehicleToMe);
	#endif
}
