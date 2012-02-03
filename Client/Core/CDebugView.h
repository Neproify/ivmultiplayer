//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDebugView.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>
#include "CIVTask.h"
#include "CIVPedTaskManager.h"
#include "CNetworkPlayer.h"

class CDebugView
{
private:
	float m_fDebugTextTop;

	// Draw text string
	void DrawText(String strText);

	// Dump tasks
	void DumpTask(String strName, CIVTask * pTask);
	void DumpTasks(CIVPedTaskManager * pPedTaskManager, int iType);

	// Dump player
	void DumpPlayer(CNetworkPlayer * pPlayer);

public:
	CDebugView();
	~CDebugView();

	void Draw();
};
