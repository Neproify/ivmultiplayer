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
#include "CCamera.h"

IVPed      * g_pPed = NULL;
IVVehicle  * g_pKeySyncIVVehicle = NULL;
unsigned int g_uiLocalPlayerIndex = 0;
IVPad        g_localPad;
Matrix       g_matLocalCameraMatrix;
bool         g_bInLocalContext = true;

extern CLocalPlayer			* g_pLocalPlayer;
extern CVehicleManager		* g_pVehicleManager;
extern CCamera              * g_pCamera;

#if 0
CCam * GetGameCam()
{
	/*CCam * pCam = NULL;
	_asm
	{
		mov ecx, offset COffsets::VAR_CameraManager
		mov eax, [ecx+0Ch]
		mov pCam, eax
	}*/
	//DWORD dwCameraManager = *(DWORD **)COffsets::VAR_CameraManager;
	//return (CCam *)(dwCameraManager + 0xC);
	// todo: defines for addresses
	//CCam * pCam = (DWORD **)(CGame::GetBase() + 0xF21A6C);
	void * unkn = (void **)(CGame::GetBase() + 0xF21A6C);
	DWORD dwFunc = (CGame::GetBase() + 0xC4E4C0);
	CCam * pCam = NULL;
	_asm
	{
		mov ecx, unkn
		mov eax, [ecx+0Ch]
		mov pCam, eax
	}
	_asm
	{
		mov ecx, unkn
		call dwFunc
		mov pCam, eax
	}
	return pCam;
}
#endif

void SetGameCameraMatrix(Matrix * matMatrix)
{
	/*CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&pCam->m_data1.m_matMatrix.vecRight, &matMatrix->vecRight, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecForward, &matMatrix->vecForward, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecUp, &matMatrix->vecUp, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecPosition, &matMatrix->vecPosition, sizeof(CVector3));
	}*/
}

void GetGameCameraMatrix(Matrix * matMatrix)
{
	/*CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&matMatrix->vecRight, &pCam->m_data1.m_matMatrix.vecRight, sizeof(CVector3));
		memcpy(&matMatrix->vecForward, &pCam->m_data1.m_matMatrix.vecForward, sizeof(CVector3));
		memcpy(&matMatrix->vecUp, &pCam->m_data1.m_matMatrix.vecUp, sizeof(CVector3));
		memcpy(&matMatrix->vecPosition, &pCam->m_data1.m_matMatrix.vecPosition, sizeof(CVector3));
	}*/
}

#include "CChatWindow.h"
extern CChatWindow * g_pChatWindow;

void ContextSwitch(IVPed * pPed, bool bPost)
{
	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Is this not the local player ped?
		if((IVPlayerPed *)pPed != CGame::GetPools()->GetPlayerInfoFromIndex(0)->m_pPlayerPed)
		{
			if(!bPost && !g_bInLocalContext)
			{
				CLogFile::Printf("Not switching due to not being in local context!");
				return;
			}

			if(bPost && g_bInLocalContext)
			{
				CLogFile::Printf("Not switching due to being in local context!");
				return;
			}

			// Get the remote players context info
			CContextData * pContextInfo = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

			// Do we have a valid context info?
			if(pContextInfo)
			{
				// Get the game pad
				CIVPad * pPad = CGame::GetPad();

				if(!bPost)
				{
					// Store the local players index
					g_uiLocalPlayerIndex = CGame::GetPools()->GetLocalPlayerIndex();

					// Store the local players pad
					memcpy(&g_localPad, pPad->GetPad(), sizeof(IVPad));

					// Store the local players camera matrix
					GetGameCameraMatrix(&g_matLocalCameraMatrix);

					// Swap the local player index with the remote players index
					CGame::GetPools()->SetLocalPlayerIndex(pContextInfo->GetPlayerInfo()->GetPlayerNumber());

					// Set the history values
					for(int i = 0; i < INPUT_COUNT; i++)
					{
						IVPadData * pPadData = &pContextInfo->GetPad()->GetPad()->m_padData[i];

						if(pPadData->m_pHistory)
						{
							pPadData->m_byteHistoryIndex++;

							if(pPadData->m_byteHistoryIndex >= MAX_HISTORY_ITEMS)
								pPadData->m_byteHistoryIndex = 0;

							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_byteValue = pContextInfo->GetPad()->GetPad()->m_padData[i].m_byteLastValue;
							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_dwLastUpdateTime = CGame::GetTime();
						}
					}

					CPadState padState;
					pContextInfo->GetPad()->GetCurrentClientPadState(padState);
					//g_pChatWindow->AddInfoMessage("L: %d R: %d U: %d D: %d", padState.ucOnFootMove[0], padState.ucOnFootMove[1], padState.ucOnFootMove[2], padState.ucOnFootMove[3]);

					// Swap the local players pad with the remote players pad
					memcpy(pPad->GetPad(), pContextInfo->GetPad()->GetPad(), sizeof(IVPad));

					// Swap the local players camera matrix with the remote players camera matrix
					//SetGameCameraMatrix(pContextInfo->GetCameraMatrix());

					// Flag ourselves as no longer in local context
					g_bInLocalContext = false;
				}
				else
				{
					// Restore the local players camera matrix
					SetGameCameraMatrix(&g_matLocalCameraMatrix);

					// Restore the local players pad
					memcpy(pPad->GetPad(), &g_localPad, sizeof(IVPad));

					// Restore the local players index
					CGame::GetPools()->SetLocalPlayerIndex(g_uiLocalPlayerIndex);

					// Flag ourselves as back in local context
					g_bInLocalContext = true;
				}
			}
			else
				CLogFile::Printf("ContextSwitch Warning: Invalid Player Ped");
		}
	}
}

void _declspec(naked) CPlayerPed__ProcessInput_Hook()
{
	_asm
	{
		mov g_pPed, ecx
		pushad
	}

	ContextSwitch(g_pPed, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlayerPed__ProcessInput
		pushad
	}

	ContextSwitch(g_pPed, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CAutmoobile_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CAutomobile__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBike__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBoat__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CTrain__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CHeli__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlane__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

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
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CAutomobile__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

// test
DWORD dwFunc = 0;
void _declspec(naked) CTaskSimpleAimGun__SetPedPosition_Hook()
{
	_asm
	{
		pushad
	}

	dwFunc = (CGame::GetBase() + 0xCC8140);

	//SetGameCameraMatrix(&playerCamMatrixs[0]);

	_asm
	{
		popad
		jmp dwFunc
	}
}
// end test

void InstallKeySyncHooks()
{
	// test
	CPatcher::InstallMethodPatch((CGame::GetBase() + 0xDCD6C0), (DWORD)CTaskSimpleAimGun__SetPedPosition_Hook);
	// end test

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
