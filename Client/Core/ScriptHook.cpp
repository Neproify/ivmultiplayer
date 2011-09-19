//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptHookcpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Create local player in CNetworkPlayer

#include "ScriptHook.h"
#include "COffsets.h"
#include "CGame.h"
#include "Scripting.h"
#include <Patcher/CPatcher.h>

GtaThread * m_pScriptThread = NULL;
bool        g_bScriptLoaded = false;

void GameLoad();
void GameScriptProcess();

void _declspec(naked) GetRunningScriptThread_Hook()
{
	_asm
	{
		mov eax, m_pScriptThread
		retn
	}
}

int iFrames = 0;

void CScriptVM__Process()
{
	if(!g_bScriptLoaded)
	{
		// Get the script thread pool
		CSimpleCollection<GtaThread> * pThreads = (CSimpleCollection<GtaThread> *)COffsets::VAR_ScrVM__ThreadPool;

		// Set the current script count and script thread pool size to 0
		pThreads->Size = 0;
		pThreads->Count = 0;

		// Set the current script thread to ourselves
		*(GtaThread **)COffsets::VAR_ScrVM__ActiveThread = m_pScriptThread;

		// Create the local player
		unsigned int uiPlayerIndex;
		Scripting::CreatePlayer(0, 0.0f, 0.0f, 0.0f, &uiPlayerIndex);

		// TODO: Don't fade screen in here, wait until connected to server
		// then fade in (means we don't have liberty island screen)
		// This doesn't work here, what utter faggotry!
		// Fade in the screen
		//Scripting::DoScreenFadeIn(0);
		//g_pClient->GetGame()->FadeScreen(FADE_TYPE_IN, 0);

		// Call the game load callback
		GameLoad();
		//g_pClient->OnGameLoad();

		// Set the game state to in game
		CGame::SetState(GAME_STATE_INGAME);

		// Flag the script as loaded
		g_bScriptLoaded = true;
	}
	else
	{
		// Call the game process callback
		GameScriptProcess();
		//g_pClient->OnGameProcess();
	}

	if(iFrames < 2)
	{
		Scripting::DoScreenFadeIn(0);
		iFrames++;
	}
}

void _declspec(naked) CScriptVM__Process_Hook()
{
	// Usually this would loop through all running script threads
	// and call pThread->m0C with opsToExecute set to 1100000
	_asm
	{
		pushad
	}

	CScriptVM__Process();

	_asm
	{
		popad
		mov al, 1
		retn
	}
}

void InstallScriptHooks()
{
	// Create new script thread structure
	m_pScriptThread = new GtaThread;
	memset(m_pScriptThread, 0, sizeof(GtaThread));

	// Hook GetRunningScriptThread to return our own script thread structure
	CPatcher::InstallJmpPatch(COffsets::FUNC_GetRunningScriptThread, (DWORD)GetRunningScriptThread_Hook, 5);

	// Hook for CScriptVM::Process
	CPatcher::InstallCallPatch(COffsets::CALL_CScriptVM__Process, (DWORD)CScriptVM__Process_Hook, 5);
}

void UninstallScriptHooks()
{
	// Delete the script thread structure
	SAFE_DELETE(m_pScriptThread);
}
