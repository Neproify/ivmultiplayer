//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModuleManager.cpp
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "CServer.h"
#include "CModuleManager.h"

CModuleManager::CModuleManager()
{

}

CModuleManager::~CModuleManager()
{
	for(std::list<CModule *>::iterator i = m_lstModules.begin(); i != m_lstModules.end(); ++i)
	{
		if(*i)
		{
			delete *i;
		}
	}
}

CModule * CModuleManager::LoadModule(const char * szName)
{
	CModule *pModule = new CModule(szName);

	if(!pModule->IsValid())
	{
		delete pModule;
		return NULL;
	}

	m_lstModules.push_back(pModule);

	return pModule;
}

void CModuleManager::ScriptLoad(HSQUIRRELVM pVM)
{
	for(std::list<CModule *>::iterator i = m_lstModules.begin(); i != m_lstModules.end(); ++i)
	{
		if(*i)
			(*i)->ScriptLoad(pVM);
	}
}

void CModuleManager::ScriptUnload(HSQUIRRELVM pVM)
{
	for(std::list<CModule *>::iterator i = m_lstModules.begin(); i != m_lstModules.end(); ++i)
	{
		if(*i)
			(*i)->ScriptUnload(pVM);
	}
}

void CModuleManager::Pulse()
{
	for(std::list<CModule *>::iterator i = m_lstModules.begin(); i != m_lstModules.end(); ++i)
	{
		if(*i)
			(*i)->Pulse();
	}
}