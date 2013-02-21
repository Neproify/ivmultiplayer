//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CEvents.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <map>
#include <list>

#include <Scripting/CSquirrelArguments.h>
#include <Scripting/CSquirrel.h>
#include <Scripting/CScriptingManager.h>
// FIXUPDATE
// jenksta: this is kinda hacky :/
#ifdef _SERVER
#include "../Server/Core/Main.h"
#include "../Server/Core/Interfaces/CEventsInterface.h"
#endif

class CEventHandler
{
public:
	virtual SQVM*          GetScript()   const { return 0; };
	virtual SQObjectPtr    GetFunction() const { return (SQInteger)0; };
	virtual void*          GetChunk()    const { return 0; };
#ifdef _SERVER
	virtual EventHandler_t GetHandler()  const { return 0; };
#endif
	virtual bool equals(const CEventHandler* other) const = 0;
	virtual void Call(CSquirrelArguments* pArguments, CSquirrelArgument* pReturn) = 0;
};

// Squirrel Event Handlers
class CSquirrelEventHandler : public CEventHandler
{
	SQVM * m_pVM;
	SQObjectPtr m_pFunction;

public:
	CSquirrelEventHandler(SQVM * pVM, SQObjectPtr pFunction)
	{
		m_pVM = pVM;
		m_pFunction = pFunction;
	}

	SQVM* GetScript() const { return m_pVM; }
	SQObjectPtr GetFunction() const { return m_pFunction; }

	bool equals(const CEventHandler* other) const
	{
		return other->GetScript() && other->GetScript() == GetScript() && other->GetFunction()._unVal.pClosure == GetFunction()._unVal.pClosure;
	}

	void Call(CSquirrelArguments* pArguments, CSquirrelArgument* pReturn)
	{
		// Check if the VM does exist
		CSquirrel* pScript = CScriptingManager::GetInstance()->Get(m_pVM); 
		if(pScript)
			pScript->Call(m_pFunction, pArguments, pReturn);
	}
};
#ifdef _SERVER
// Module Event Handlers
class CModuleEventHandler : public CEventHandler
{
	EventHandler_t m_pfnHandler;
	void *m_pChunk;
public:
	CModuleEventHandler(EventHandler_t pfnHandler, void* pChunk)
	{
		m_pfnHandler = pfnHandler;
		m_pChunk = pChunk;
	}

	bool equals(const CEventHandler* other) const
	{
		return other->GetHandler() && other->GetHandler() == GetHandler() && other->GetChunk() == GetChunk();
	}

	void Call(CSquirrelArguments* pArguments, CSquirrelArgument* pReturn)
	{
		m_pfnHandler(pArguments, pReturn, m_pChunk);
	}
};
#endif
class CEvents : public std::map< String, std::list< CEventHandler* > >
#ifdef _SERVER
	, public CEventsInterface
#endif
{
private:
	static CEvents * m_pInstance;

public:
	const char* currentEventName;

	CEvents()
	{
		// Set our instance
		m_pInstance = this;
	}

	~CEvents()
	{
		clear();

		// Reset our instance
		m_pInstance = NULL;
	}

	static CEvents * GetInstance() { return m_pInstance; }

	bool Add(String strName, CEventHandler* pEventHandler)
	{
		// Any events with that name?
		CEvents::iterator iter = find(strName);
		if(iter != end())
		{
			// Event exists, check if the function is registered already
			for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2)
			{
				if(pEventHandler->equals(*iter2))
					return false;
			}
		}
		else
		{
			// new - create the event
			std::pair< std::map< String, std::list< CEventHandler* > >::iterator, bool > ret;
			ret = insert(std::pair< String, std::list< CEventHandler* > >( strName, std::list< CEventHandler* >( ) ) );

			iter = ret.first;
		}

		// insert the handler
		(*iter).second.push_back(pEventHandler);
		return true;
	}

	bool Remove(String strName, CEventHandler* pEventHandler)
	{
		// Any events with that name?
		CEvents::iterator iter = find(strName);
		if(iter != end())
		{
			// Check if it exists, if so remove it
			bool bRemoved = false;
			for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); )
			{
				if(pEventHandler->equals(*iter2))
				{
					(*iter).second.erase(iter2++);
					bRemoved = true;
					break;
				}
				else
					iter2 ++;
			}

			if(bRemoved)
			{
				// remove the event list if it is empty
				if((*iter).second.size() == 0)
				{
					erase(iter);
				}
			}

			return bRemoved;
		}

		// no such event - can't remove handlers
		return false;
	}

	bool RemoveScript(SQVM * pVM)
	{
		for(CEvents::iterator iter = begin(); iter != end(); )
		{
			for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); )
			{
				if((*iter2)->GetScript() == pVM)
					(*iter).second.erase(iter2++);
				else
					iter2 ++;
			}

			// remove events with no handlers
			if((*iter).second.size() == 0)
				erase(iter++);
			else
				iter++;
		}

		return true;
	}

	bool IsEventRegistered(String eventName)
	{
		CEvents::iterator iter = find(eventName);
		return iter != end();
	}

#ifdef _SERVER
	bool AddModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0)
	{
		return Add(szName, new CModuleEventHandler(pfnHandler, pChunk));
	}

	bool RemoveModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0)
	{
		return Remove(szName, &CModuleEventHandler(pfnHandler, pChunk));
	}

	void CallModuleEvent(const char* szName, SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn)
	{
		Call(szName, reinterpret_cast<CSquirrelArguments*>(pArguments), reinterpret_cast<CSquirrelArgument*>(pReturn));
	}

	const char * GetCurrentEvent()
	{
		return currentEventName;
	}

#endif

	CSquirrelArgument Call(String strName, CSquirrel* pScript = NULL)
	{
		CSquirrelArgument pReturn((int)1);
		Call(strName, &CSquirrelArguments(), &pReturn, pScript);
		return pReturn;
	}

	CSquirrelArgument Call(String strName, CSquirrelArguments* pArguments, CSquirrel* pScript = NULL)
	{
		CSquirrelArgument pReturn((int)1);
		Call(strName, pArguments, &pReturn, pScript);
		return pReturn;
	}

	void Call(String strName, CSquirrelArguments* pArguments, CSquirrelArgument* pReturn, CSquirrel* pScript = NULL)
	{
		currentEventName = strName;
		SQVM* pVM = pScript ? pScript->GetVM() : 0;

		// Any events with that name?
		CEvents::iterator iter = find(strName);
		if(iter != end())
		{
			// loop through all handlers
			for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2)
			{
				// not for a specific script; or that script is the one we want
				if(!pVM || pVM == (*iter2)->GetScript())
				{
					(*iter2)->Call(pArguments, pReturn);
				}
			}
		}
	}
};
