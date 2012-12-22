//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CNetworkEntityManager.h
// Project: Client.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Common.h"

template<class Entity, EntityId max>
class CNetworkEntityManager
{
private:
	Entity * m_Entities[max];

public:
	CNetworkEntityManager()
	{
		// Set all entities to non-existant
		memset(&m_Entities, 0, sizeof(m_Entities));
	}
	
	virtual ~CNetworkEntityManager()
	{
		// Loop through all entities
		for(EntityId id = 0; id < max; ++id)
		{
			// Check if ID exists
			if(Exists(id))
			{
				// we could call Delete() here, but there's no need to get this done in a clean way as we're about to be deleted
				delete m_Entities[id];
			}
		}
	}

	void Add(EntityId id, Entity * entity)
	{
		// Check if the Entity didn't exist yet
		if(Exists(id))
		{
			// Delete it if it did
			Delete(id);
		}

		// save it
		m_Entities[id] = entity;
	}

	bool Delete(EntityId id)
	{
		// Check if the entity already existed
		if(!Exists(id))
			return false;

		// Delete the entity
		delete m_Entities[id];

		// mark the slot as free
		m_Entities[id] = 0;

		return true;
	}

	bool Exists(EntityId id)
	{
		// Entity IDs are unsigned types, so no id >= 0 check
		return (id < max && m_Entities[id] != 0);
	}

	Entity * Get(EntityId id)
	{
		if(Exists(id))
			return m_Entities[id];

		return 0;
	}

	EntityId GetMax()
	{
		return max;
	}

	/*void Pulse()
	{
		// Loop through all entities
		for(EntityId id = 0; id < max; ++id)
		{
			// Check if ID exists
			if(Exists(id))
			{
				// Pulse the entity
				m_Entities[id]->Pulse();
			}
		}
	}*/

	EntityId GetCount()
	{
		EntityId count = 0;

		// Loop through all entities
		for(EntityId id = 0; id < max; ++id)
		{
			// Check if ID exists
			if(Exists(id))
			{
				// Increment the count
				count++;
			}
		}

		return count;
	}
};
