//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientTaskManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "IV/CIVTask.h"
#include <list>

struct ClientTaskPair
{
	IVTask * pGameTask;
	CIVTask * pClientTask;
};

class CClientTaskManager
{
private:
	std::list<ClientTaskPair *> m_taskList;

public:
	CClientTaskManager();
	~CClientTaskManager();

	bool      AddTask(CIVTask * pClientTask);
	bool      RemoveTask(CIVTask * pClientTask);
	IVTask  * GetGameTaskFromClientTask(CIVTask * pClientTask);
	CIVTask * GetClientTaskFromGameTask(IVTask * pGameTask, bool bCreateIfNotExist = true);
	bool      HandleTaskDelete(IVTask * pGameTask);
};
