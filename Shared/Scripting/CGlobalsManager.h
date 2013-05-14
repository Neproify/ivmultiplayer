//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGlobalsManager.h
// Project: Server.Core
// Author(s): CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Scripting/Natives/Natives.h>
#include <list>
#include <map>

class CGlobal
{
public:
	String Key;
	CSquirrelArgument Value;
	CGlobal(String key, CSquirrelArgument value) : Key(key), Value(value) { }
};

class CGlobalsManager : std::map< EntityId, std::list< CGlobal > >
{
private:
	static SQInteger IsSet(SQVM * pVM);
	static SQInteger Set(SQVM * pVM);
	static SQInteger Get(SQVM * pVM);
	static SQInteger Remove(SQVM * pVM);

	static SQInteger GetPVar(SQVM * pVM);
	static SQInteger SetPVar(SQVM * pVM);
	static SQInteger IsPVarSet(SQVM * pVM);
	static SQInteger RemovePVar(SQVM * pVM);
	//static SQInteger ClearPVars(SQVM * pVM);

public:
	static void RegisterNatives(CScriptingManager * pScriptingManager);
	
	bool IsSet(EntityId id, String key = NULL)
	{
		iterator i = find(id);
		if(i != end())
		{
			// ID exists, check if key exists:
			for(std::list< CGlobal >::iterator i2 = (*i).second.begin(); i2 != (*i).second.end(); ++ i2)
			{
				if((*i2).Key == key)
					return true;
			}
		}
		return false;
	}
	void Set(EntityId id, String key, CSquirrelArgument value)
	{
		// Do we have such ID?
		iterator i = find(id);
		if(i != end())
		{
			if(IsSet(id, key))
				Remove(id, key);

			// If not, create new CGlobal here:
			(*i).second.insert((*i).second.end(), CGlobal(key, value));
		}
		else
		{
			// Create an entry for this ID:
			std::list< CGlobal > list = std::list< CGlobal >();
			list.insert(list.begin(), CGlobal(key, value));
			insert(std::make_pair(id, list));
		}
	}
	CSquirrelArgument Get(EntityId id, String key)
	{
		// Do we have such ID?
		iterator i = find(id);
		if(i != end())
		{
			// ID exists, check if Key is exists already:
			for(std::list< CGlobal >::iterator i2 = (*i).second.begin(); i2 != (*i).second.end(); ++ i2)
			{
				if((*i2).Key == key)
					return (*i2).Value;
			}
		}

		// Not found?
		return CSquirrelArgument();
	}
	bool Remove(EntityId id, String key)
	{
		// Do we have such ID?
		iterator i = find(id);
		if(i != end())
		{
			// ID exists, check if Key is exists:
			for(std::list< CGlobal >::iterator i2 = (*i).second.begin(); i2 != (*i).second.end(); ++ i2)
			{
				if((*i2).Key == key)
				{
					(*i).second.erase(i2);
					return true;
				}
			}
		}

		return false;
	}
	bool Remove(EntityId id)
	{
		// Do we have such ID?
		iterator i = find(id);
		if(i != end())
		{
			erase((*i).first);
			return true;
		}

		return false;
	}
};
