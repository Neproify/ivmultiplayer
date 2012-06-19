//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CStreamer.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CStreamer.h"
#include "CLocalPlayer.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include <SharedUtility.h>

extern CLocalPlayer * g_pLocalPlayer;
extern CStreamer    * g_pStreamer;

CStreamableEntity::CStreamableEntity(eStreamEntityType eType, float fDistance)
{
	// set the type
	m_eType = eType;

	// set the streaming distance
	m_fStreamingDistance = fDistance;

	// streamed out by default
	m_bIsStreamedIn = false;

	// no dimension by default
	m_dimensionId = INVALID_DIMENSION_ID;

	// can't be streamed in by default
	m_bCanBeStreamedIn = false;

	// add it to the streamer
	g_pStreamer->push_back(this);
}

CStreamableEntity::~CStreamableEntity()
{
	if(m_bIsStreamedIn)
	{
		// Nothing we can do at this point, as it'd go only for pure virtual function call with StreamOut()
		CLogFile::Printf("CStreamableEntity::Deleting 0x%x (%d) while it is streamed in", this, m_eType);
	}
}

float CStreamableEntity::GetStreamingDistance()
{
	return m_fStreamingDistance;
}

bool CStreamableEntity::IsStreamedIn()
{
	return m_bIsStreamedIn;
}

void CStreamableEntity::SetDimension(DimensionId dimensionId)
{
	// NOTE: This should trigger an instant restream of ourselves
	m_dimensionId = dimensionId;
}

void CStreamableEntity::StreamInInternal()
{
	// check if the entity is streamed in
	if(!m_bIsStreamedIn)
	{
		//CLogFile::Printf("CStreamableEntity::Stream In 0x%x (%d)", this, m_eType);

		// stream it in
		StreamIn();

		// flag us as streamed in
		m_bIsStreamedIn = true;
	}
}

void CStreamableEntity::StreamOutInternal()
{
	// check if the entity is streamed in
	if(m_bIsStreamedIn)
	{
		//CLogFile::Printf("CStreamableEntity::Stream Out 0x%x (%d)", this, m_eType);

		// stream it out
		StreamOut();

		// flag us as streamed out
		m_bIsStreamedIn = false;
	}
}

void CStreamableEntity::OnDelete()
{
	// remove it from the streamer
	g_pStreamer->remove(this);
}

// --

CStreamer::CStreamer()
{
	// Set the streaming limits (Adjust if needed)
	m_uiStreamingLimits[STREAM_ENTITY_VEHICLE] = 64; // no more than vehicle pool size
	m_uiStreamingLimits[STREAM_ENTITY_PICKUP] = 128; // no more than pickup pool size
	m_uiStreamingLimits[STREAM_ENTITY_OBJECT] = 512; // no more than object pool size
	m_uiStreamingLimits[STREAM_ENTITY_CHECKPOINT] = 64; // no more than INTERNAL_CHECKPOINT_LIMIT

	// Reset the streamer
	Reset();
}

void CStreamer::Reset()
{
	m_ulLastStreamTime = 0;
	m_dimensionId = INVALID_DIMENSION_ID;
	
	// Loop through all types
	for(int i = 0; i < STREAM_ENTITY_MAX; ++i)
	{
		// Check if we have streamed in elements for this
		if(m_streamedElements[i].size() > 0)
		{
			// Output a message
			CLogFile::Printf("CStreamer::Reset with %d objects of type %d", m_streamedElements[i].size(), i);

			// Loop through all entities
			for(std::list<CStreamableEntity*>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++ iter)
			{
				// Stream it out
				(*iter)->StreamOutInternal();
			}

			// Clear the list of entities specific to that type
			m_streamedElements[i].clear();
		}
	}

	// Clear the list of all entities
	clear();
}

inline bool SortStreamableEntites(CStreamableEntity * pEntity, CStreamableEntity * pOther)
{
	// Get the local player position
	CVector3 vecPlayerPos;
	g_pLocalPlayer->GetPosition(vecPlayerPos);

	// Get the first entities position
	CVector3 vecPos;
	pEntity->GetStreamPosition(vecPos);

	// Get the second entities position
	CVector3 vecPosOther;
	pOther->GetStreamPosition(vecPosOther);

	// Compare the positions against the local player position
	return (Math::GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecPos.fX, vecPos.fY, vecPos.fZ) < Math::GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecPosOther.fX, vecPosOther.fY, vecPosOther.fZ));
}

// TODO: Notify server of stream in/out
void CStreamer::Pulse()
{
	unsigned long ulTime = SharedUtility::GetTime();

	if((ulTime - m_ulLastStreamTime) > STREAMING_TICK)
	{
		m_ulLastStreamTime = ulTime;

		std::list<CStreamableEntity *> newEntities[STREAM_ENTITY_MAX];

		CVector3 vecPlayerPos;
		g_pLocalPlayer->GetPosition(vecPlayerPos);

		// Loop through all streamable elements
		for(iterator iter = begin(); iter != end(); ++ iter)
		{
			// Can this entity be streamed in?
			if((*iter)->CanBeStreamedIn())
			{
				// check distance
				CVector3 vecPos;
				(*iter)->GetStreamPosition(vecPos);
				float fDistance = (vecPlayerPos - vecPos).Length();
				bool bInRange = (fDistance <= (*iter)->GetStreamingDistance());

				if(!bInRange || (m_dimensionId != INVALID_DIMENSION_ID && (*iter)->GetDimension() != INVALID_DIMENSION_ID && (m_dimensionId != (*iter)->GetDimension())))
				{
					// out of range or in another dimension, but streamed in?
					if((*iter)->IsStreamedIn())
					{
						//CLogFile::Printf("Streamout with Distance %f and Dimension %d/%d", fDistance, m_dimensionId, (*iter)->GetDimension());
						
						// remove it from the list of streamed in elements
						m_streamedElements[(*iter)->GetType()].remove(*iter);

						// stream it out
						(*iter)->StreamOutInternal();
					}
				}
				else if(bInRange && (m_dimensionId == INVALID_DIMENSION_ID || (*iter)->GetDimension() == INVALID_DIMENSION_ID || (m_dimensionId == (*iter)->GetDimension())))
				{
					// in range and in same/all dimension, but not streamed in?
					if(!(*iter)->IsStreamedIn())
					{
						// flag it for being streamed in (important to have gta's hardcoded limits enforced)
						newEntities[(*iter)->GetType()].push_back(*iter);
					}
				}
			}
			else
			{
				if((*iter)->IsStreamedIn())
				{
					CLogFile::Printf("Streamout of 0x%x due to not being allowed to be streamed in.", (*iter));

					// remove it from the list of streamed in elements
					m_streamedElements[(*iter)->GetType()].remove(*iter);

					// stream it out
					(*iter)->StreamOutInternal();
				}
			}
		}

		// Add all new entites
		for(int i = 0; i < STREAM_ENTITY_MAX; ++i)
		{
			// check if we have entites of that type to stream in
			if(newEntities[i].size() > 0)
			{
				// we have enough space within the GTA engine
				if((m_streamedElements[i].size() + newEntities[i].size()) <= m_uiStreamingLimits[i])
				{
					for(std::list<CStreamableEntity*>::iterator iter = newEntities[i].begin(); iter != newEntities[i].end(); ++ iter)
					{
						// Stream the entity in
						(*iter)->StreamInInternal();

						// add it to our list of streamed in entities
						m_streamedElements[i].push_back(*iter);
					}
				}
				else
				{
					// alone reaching this isn't good - we should stream more entities than GTA can handle so we need to pick the closest ones

					// add our own entites, but do not stream them in
					for(std::list<CStreamableEntity*>::iterator iter = newEntities[i].begin(); iter != newEntities[i].end(); ++ iter)
					{
						m_streamedElements[i].push_back(*iter);
					}

					// sort the list by distance
					m_streamedElements[i].sort(SortStreamableEntites);

					// remove the entities that are too far away
					while(m_streamedElements[i].size() > m_uiStreamingLimits[i])
					{
						// get the farest away entity
						CStreamableEntity* pEntity = m_streamedElements[i].back();

						// streamed in? (if not, it's new and we don't need to stream it out either)
						if(pEntity->IsStreamedIn())
						{
							// stream it out
							pEntity->StreamOutInternal();
						}

						// remove it from the streamed in entities
						m_streamedElements[i].pop_back();
					}

					// make sure all remaining entities are streamed in
					for(std::list<CStreamableEntity*>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++iter)
					{
						// not streamed in?
						if(!(*iter)->IsStreamedIn())
						{
							// stream it in
							(*iter)->StreamInInternal();
						}
					}
				}
			}
		}

		//CLogFile::Printf("CStreamer::Pulse (total = %d, time = %dms, veh = %d, pick = %d, obj = %d)", size(), SharedUtility::GetTime() - ulTime, m_streamedElements[STREAM_ENTITY_VEHICLE].size(), m_streamedElements[STREAM_ENTITY_PICKUP].size(), m_streamedElements[STREAM_ENTITY_OBJECT].size());
	}
}

void CStreamer::UpdateInterior(unsigned int uiInterior)
{
	// loop through all entity types
	for(int i = 0; i < STREAM_ENTITY_MAX; ++ i)
	{
		// look through all spawned entites of that type
		for(std::list<CStreamableEntity*>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++ iter)
		{
			// Update the interior
			(*iter)->UpdateInterior(uiInterior);
		}
	}
}

void CStreamer::ForceStreamIn(CStreamableEntity * pEntity)
{
	// not enough space to stream it in?
	if(m_streamedElements[pEntity->GetType()].size() > m_uiStreamingLimits[pEntity->GetType()])
	{
		// Get the last entity
		CStreamableEntity* pOtherEntity = m_streamedElements[pEntity->GetType()].back();

		// Check if it is streamed in
		if(pOtherEntity->IsStreamedIn())
		{
			// Stream it out
			pOtherEntity->StreamOutInternal();
		}

		// remove it from the list so we'll have space
		m_streamedElements[pEntity->GetType()].pop_back();
	}

	// stream the entity in
	pEntity->StreamInInternal();

	// add it to the list of streamed entities
	m_streamedElements[pEntity->GetType()].push_back(pEntity);
}

void CStreamer::ForceStreamOut(CStreamableEntity * pEntity)
{
	// Remove it from the streamed in elements
	m_streamedElements[pEntity->GetType()].remove(pEntity);

	// stream the entity out
	pEntity->StreamOutInternal();
}

void CStreamer::remove(CStreamableEntity* pEntity)
{
	// force it to be streamed out
	ForceStreamOut(pEntity);

	// remove it from the global list
	std::list<CStreamableEntity*>::remove(pEntity);
}

std::list<CStreamableEntity *> * CStreamer::GetStreamedInEntitiesOfType(eStreamEntityType eType)
{
	return &m_streamedElements[eType];
}

unsigned int CStreamer::GetStreamedInEntityCountOfType(eStreamEntityType eType)
{
	return m_streamedElements[eType].size();
}

unsigned int CStreamer::GetStreamedInLimitOfType(eStreamEntityType eType)
{
	return m_uiStreamingLimits[eType];
}

/*CNetworkVehicle * CStreamer::GetPlayerFromGamePlayerPed(IVPlayerPed * pGamePlayerPed)
{
	// Get the streamed in players list
	std::list <CStreamableEntity *> * m_streamedVehicles = &m_streamedElements[STREAM_ENTITY_PLAYER];

	// Loop through the streamed in players list
	for(std::list<CStreamableEntity *>::iterator iter = m_streamedVehicles->begin(); iter != m_streamedVehicles->end(); iter++)
	{
		// Get the player pointer
		CClientPlayer * pTestPlayer = reinterpret_cast<CClientPlayer *>(*iter);

		// Is this the player we are looking for?
		if(pTestPlayer->GetGamePlayerPed()->GetPlayerPed() == pGamePlayerPed)
			return pTestPlayer;
	}

	// No player found
	return NULL;
}*/

CNetworkVehicle * CStreamer::GetVehicleFromGameVehicle(IVVehicle * pGameVehicle)
{
	// Get the streamed in vehicles list
	std::list <CStreamableEntity *> * m_streamedVehicles = &m_streamedElements[STREAM_ENTITY_VEHICLE];

	// Loop through the streamed in vehicles list
	for(std::list<CStreamableEntity *>::iterator iter = m_streamedVehicles->begin(); iter != m_streamedVehicles->end(); iter++)
	{
		// Get the vehicle pointer
		CNetworkVehicle * pTestVehicle = reinterpret_cast<CNetworkVehicle *>(*iter);

		// Is this the vehicle we are looking for?
		if(pTestVehicle->GetGameVehicle()->GetVehicle() == pGameVehicle)
			return pTestVehicle;
	}

	// No vehicle found
	return NULL;
}
