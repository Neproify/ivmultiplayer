//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CStreamer.h
// Project: Client.Core
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <list>
#include "CIVVehicle.h"

// Type used for dimension ids
typedef unsigned char DimensionId;

// Define used for invalid dimension ids
#define INVALID_DIMENSION_ID 0xFF

// TODO: Adjust?
#define STREAMING_TICK 1000

enum eStreamEntityType
{
	STREAM_ENTITY_VEHICLE,
	STREAM_ENTITY_PICKUP,
	STREAM_ENTITY_OBJECT,
	STREAM_ENTITY_CHECKPOINT,
	STREAM_ENTITY_MAX,
};

class CStreamer;

class CStreamableEntity
{
	friend class CStreamer;

private:
	float             m_fStreamingDistance;
	bool              m_bIsStreamedIn;
	eStreamEntityType m_eType;
	DimensionId       m_dimensionId;
	bool              m_bCanBeStreamedIn;

	void              StreamInInternal();
	void              StreamOutInternal();

public:
	CStreamableEntity(eStreamEntityType eType, float fDistance = 200.0f);
	virtual ~CStreamableEntity();

	float             GetStreamingDistance();
	bool              IsStreamedIn();
	eStreamEntityType GetType() { return m_eType; }
	void              SetDimension(DimensionId dimensionId);
	DimensionId       GetDimension() { return m_dimensionId; }
	void              SetCanBeStreamedIn(bool bCanBeStreamedIn) { m_bCanBeStreamedIn = bCanBeStreamedIn; }
	bool              CanBeStreamedIn() { return m_bCanBeStreamedIn; }

	virtual void      GetStreamPosition(CVector3& vecPosition) { };
	virtual void      UpdateInterior(unsigned int uiInterior) { };

	virtual void      StreamIn() { };
	virtual void      StreamOut() { };

protected:
	void              OnDelete();
};

class CNetworkVehicle;

class CStreamer : public std::list<CStreamableEntity*>
{
private:
	unsigned long                  m_ulLastStreamTime;
	DimensionId                    m_dimensionId;
	std::list<CStreamableEntity *> m_streamedElements[STREAM_ENTITY_MAX];
	std::list<CStreamableEntity *> m_newlyStreamedElements[STREAM_ENTITY_MAX];
	unsigned int                   m_uiStreamingLimits[STREAM_ENTITY_MAX]; // max number of each entity type the game can handle	

public:
	CStreamer();

	void                             Reset();
	void                             Pulse();
	void                             UpdateInterior(unsigned int uiInterior);
	void                             ForceStreamIn(CStreamableEntity * pEntity);
	void                             ForceStreamOut(CStreamableEntity * pEntity);
	void                             remove(CStreamableEntity* pEntity);
	std::list<CStreamableEntity *> * GetStreamedInEntitiesOfType(eStreamEntityType eType);
	unsigned int                     GetStreamedInEntityCountOfType(eStreamEntityType eType);
	unsigned int                     GetStreamedInLimitOfType(eStreamEntityType eType);
	//CNetworkPlayer                 * GetPlayerFromGamePlayerPed(IVPlayerPed * pGamePlayerPed);
	CNetworkVehicle                * GetVehicleFromGameVehicle(IVVehicle * pGameVehicle);
};
