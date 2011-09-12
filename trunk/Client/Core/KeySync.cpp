//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: KeySync.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "KeySync.h"
#include "CIVPed.h"
#include <Patcher/CPatcher.h>
#include "CLocalPlayer.h"
#include "CVehicleManager.h"
#include "COffsets.h"
#include "CPools.h"

PadState playerPadStates[MAX_PLAYERS];
Matrix34 playerCamMatrixs[MAX_PLAYERS];
CIVPlayerPed * playerPedSlots[MAX_PLAYERS];

IVPed * m_pPed;
IVVehicle * m_pKeySyncIVVehicle;
unsigned int m_uiLocalPlayerId;
BYTE m_byteEnterExitVehicleKey[2];
BYTE byteValue[GTA_KEY_MAX];

extern CLocalPlayer * g_pLocalPlayer;
extern CVehicleManager * g_pVehicleManager;

void SetLocalPlayerId(unsigned int uiPlayerId)
{
	*(unsigned int *)(COffsets::VAR_LocalPlayerId) = uiPlayerId;
}

unsigned int GetLocalPlayerId()
{
	return *(unsigned int *)(COffsets::VAR_LocalPlayerId);
}

void ResetPlayerPadState(unsigned int uiPlayerId)
{
	playerPadStates[uiPlayerId] = PadState();
}

void ResetPlayerPadStates()
{
	for(unsigned int i = 0; i < MAX_PLAYERS; i++)
		ResetPlayerPadState(i);
}

void SetPlayerPadState(unsigned int uiPlayerId, PadState * padState)
{
	memcpy(&playerPadStates[uiPlayerId], padState, sizeof(PadState));
}

void GetPlayerPadState(unsigned int uiPlayerId, PadState * padState)
{
	memcpy(padState, &playerPadStates[uiPlayerId], sizeof(PadState));
}

CPad * GetGamePad()
{
	return (CPad *)(COffsets::VAR_Pads);
}

void ResetGamePadState()
{
	SetGamePadState(&PadState());
}

void SetGamePadState(PadState * padState)
{
	CPad * pPad = (CPad *)GetGamePad();

	if(pPad)
	{
		for(int i = 0; i < GTA_KEY_MAX; i++)
		{
			pPad->m_padData[i].m_byteUnknown6 = padState->byteCurrentKeys[i];
			pPad->m_padData[i].m_byteUnknown7 = padState->bytePreviousKeys[i];
		}
	}
}

void GetGamePadState(PadState * padState)
{
	CPad * pPad = (CPad *)GetGamePad();

	if(pPad)
	{
		for(int i = 0; i < GTA_KEY_MAX; i++)
		{
			padState->byteCurrentKeys[i] = pPad->m_padData[i].m_byteUnknown6;
			padState->bytePreviousKeys[i] = pPad->m_padData[i].m_byteUnknown7;
		}
	}
}

void ResetPlayerCameraMatrix(unsigned int uPlayerId)
{
	memset(&playerCamMatrixs[uPlayerId], 0, sizeof(Matrix34));
}

void ResetPlayerCameraMatrixs()
{
	for(unsigned int i = 0; i < MAX_PLAYERS; i++)
		ResetPlayerCameraMatrix(i);
}

void SetPlayerCameraMatrix(unsigned int uiPlayerId, Matrix34 * matMatrix)
{
	memcpy(&playerCamMatrixs[uiPlayerId], matMatrix, sizeof(Matrix34));
}

void GetPlayerCameraMatrix(unsigned int uiPlayerId, Matrix34 * matMatrix)
{
	memcpy(matMatrix, &playerCamMatrixs[uiPlayerId], sizeof(Matrix34));
}

CCam * GetGameCam()
{
	CCam * pCam = NULL;
	_asm
	{
		mov ecx, offset COffsets::VAR_CameraManager
		mov eax, [ecx+0Ch]
		mov pCam, eax
	}
	//DWORD dwCameraManager = *(DWORD **)COffsets::VAR_CameraManager;
	//return (CCam *)(dwCameraManager + 0xC);
}

void SetGameCameraMatrix(Matrix34 * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
		memcpy(&pCam->m_matMatrix, matMatrix, sizeof(Matrix34));
}

void GetGameCameraMatrix(Matrix34 * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
		memcpy(matMatrix, &pCam->m_matMatrix, sizeof(Matrix34));
}

void SetPlayerPedSlot(unsigned int uiPlayerId, CIVPlayerPed * pPlayerPed)
{
	playerPedSlots[uiPlayerId] = pPlayerPed;
}

unsigned int GetPlayerIdFromPedPointer(IVPed * pPed)
{
	// Loop through all player info slots
	for(unsigned int i = 0; i < MAX_PLAYERS; i++)
	{
		// Get the player info
		IVPlayerInfo * pPlayerInfo = CGame::GetPools()->GetPlayerInfoFromIndex(i);

		// Is the player info valid and does it match our ped?
		if(pPlayerInfo && pPlayerInfo->m_pPlayerPed == pPed)
			return i;
	}

	// Player info slot not found
	return INVALID_ENTITY_ID;
}

#include "CChatWindow.h"
extern CChatWindow * g_pChatWindow;

void ContextSwitch(IVPed * pPed, bool bPost)
{
	if(pPed)
	{
		unsigned int uiPlayerId = GetPlayerIdFromPedPointer(pPed);

		if(uiPlayerId > 0 && uiPlayerId < INVALID_ENTITY_ID)
		{
			if(!bPost)
			{
				// TODO: Context switch current/desired move states?
				// Context switch the local player id with the remote players
				m_uiLocalPlayerId = CGame::GetPools()->GetLocalPlayerIndex();
				CGame::GetPools()->SetLocalPlayerIndex(uiPlayerId);

				// Context switch the local players keys with the remote players
				GetGamePadState(&playerPadStates[0]);
				SetGamePadState(&playerPadStates[uiPlayerId]);
				CPad * pGamePad = GetGamePad();

				for(int i = 0; i < GTA_KEY_MAX; i++)
				{
					if(pGamePad->m_padData[i].m_pUnknown)
					{
						byteValue[i] = pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0;
						pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0 = playerPadStates[uiPlayerId].byteCurrentKeys[i];
					}
				}

				// Context switch the local players cam matrix with the remote players
				GetGameCameraMatrix(&playerCamMatrixs[0]);
				SetGameCameraMatrix(&playerCamMatrixs[uiPlayerId]);
			}
			else
			{
				// Restore the local players cam matrix
				SetGameCameraMatrix(&playerCamMatrixs[0]);

				// Restore the local players keys
				SetGamePadState(&playerPadStates[0]);
				CPad * pGamePad = GetGamePad();

				for(int i = 0; i < GTA_KEY_MAX; i++)
				{
					if(pGamePad->m_padData[i].m_pUnknown)
					{
						pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0 = byteValue[i];
						byteValue[i] = 0;
					}
				}

				// Restore the local player id
				CGame::GetPools()->SetLocalPlayerIndex(m_uiLocalPlayerId);
			}
		}
		else if(uiPlayerId != 0)
		{
			CLogFile::Printf("ContextSwitch Warning: Invalid Player Ped");
		}

		//if(g_pChatWindow)
		//{
		//	GetGameCameraMatrix(&playerCamMatrixs[0]);
		//	g_pChatWindow->AddInfoMessage("{%.02f,%.02f, %.02f},{%.02f,%.02f, %.02f},{%.02f,%.02f, %.02f},{%.02f,%.02f, %.02f}", playerCamMatrixs[0].vecRight.fX, playerCamMatrixs[0].vecRight.fY, playerCamMatrixs[0].vecRight.fZ, playerCamMatrixs[0].vecForward.fX, playerCamMatrixs[0].vecForward.fY, playerCamMatrixs[0].vecForward.fZ, playerCamMatrixs[0].vecUp.fX, playerCamMatrixs[0].vecUp.fY, playerCamMatrixs[0].vecUp.fZ, playerCamMatrixs[0].vecPosition.fX, playerCamMatrixs[0].vecPosition.fY, playerCamMatrixs[0].vecPosition.fZ);
		//}
	}
}

void _declspec(naked) CPlayerPed__ProcessInput_Hook()
{
	_asm
	{
		mov m_pPed, ecx
		pushad
	}

	ContextSwitch(m_pPed, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlayerPed__ProcessInput
		pushad
	}

	ContextSwitch(m_pPed, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CAutomobile_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CAutomobile__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CBike_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBike__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CBoat_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBoat__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CTrain_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CTrain__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CHeli_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CHeli__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CPlane_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlane__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void InstallKeySyncHooks()
{
	// Reset all player pad states
	ResetPlayerPadStates();

	// Reset all player cam matrixs
	ResetPlayerCameraMatrixs();

	// CPlayerPed::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlayerPed__VFTable + 0x74), (DWORD)CPlayerPed__ProcessInput_Hook);

	// CAutomobile::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CAutomobile__VFTable + 0x74), (DWORD)CAutomobile_ProcessInput_Hook);

	// CBike::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBike__VFTable + 0x74), (DWORD)CBike_ProcessInput_Hook);

	// CBoat::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBoat__VFTable + 0x74), (DWORD)CBoat_ProcessInput_Hook);

	// CTrain::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CTrain__VFTable + 0x74), (DWORD)CTrain_ProcessInput_Hook);

	// CHeli::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CHeli__VFTable + 0x74), (DWORD)CHeli_ProcessInput_Hook);

	// CPlane::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlane__VFTable + 0x74), (DWORD)CPlane_ProcessInput_Hook);
}
