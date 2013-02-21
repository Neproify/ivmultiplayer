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

// TODO: Adjust?
#define STREAMING_TICK 1000

enum eStreamEntityType
{
	STREAM_ENTITY_VEHICLE,
	STREAM_ENTITY_PICKUP,
	STREAM_ENTITY_OBJECT,
	STREAM_ENTITY_CHECKPOINT,
	STREAM_ENTITY_PLAYER,
	STREAM_ENTITY_LABEL,
	STREAM_ENTITY_MAX,
};

class CStreamer;

class CStreamableEntity
{
	friend class CStreamer;
public:
	bool              m_bIsStreamedIn;
private:
	float             m_fStreamingDistance;
	
	eStreamEntityType m_eType;
	DimensionId       m_pDimensionId;
	bool              m_bCanBeStreamedIn;
	unsigned short    m_usStreamReferences, m_usStreamReferencesScript;

	void              StreamInInternal();
	void              StreamOutInternal();

public:
	CStreamableEntity(eStreamEntityType eType, float fDistance = 200.0f);
	virtual ~CStreamableEntity();

		void			  SetStreamingDistance(float fDistance) { m_fStreamingDistance = fDistance; }
		float             GetStreamingDistance();
		bool              IsStreamedIn();
		eStreamEntityType GetType() { return m_eType; }
		void              SetDimension(DimensionId dimensionId);
		DimensionId       GetDimension() { return m_pDimensionId; }
		void              SetCanBeStreamedIn(bool bCanBeStreamedIn) { m_bCanBeStreamedIn = bCanBeStreamedIn; }
		bool              CanBeStreamedIn() { return m_bCanBeStreamedIn; }

	virtual void      GetStreamPosition(CVector3& vecPosition) { };
	virtual void      UpdateInterior(unsigned int uiInterior) { };
	unsigned long	  GetTotalStreamReferences() { return m_usStreamReferences + m_usStreamReferencesScript; }

	virtual void      StreamIn() { };
	virtual void      StreamOut() { };

protected:
	void              OnDelete();
};

class CNetworkVehicle;

class CStreamer : public std::list<CStreamableEntity *>
{
	friend class CStreamableEntity;
private:
	static CStreamer             * m_pInstance;
	unsigned long                  m_ulLastStreamTime;
	DimensionId                    m_dimensionId;
	std::list<CStreamableEntity *> m_streamedElements[STREAM_ENTITY_MAX];
	std::list<CStreamableEntity *> m_newlyStreamedElements[STREAM_ENTITY_MAX];
	unsigned int                   m_uiStreamingLimits[STREAM_ENTITY_MAX]; // max number of each entity type the game can handle	

public:
	CStreamer();
	~CStreamer();

	static CStreamer               * GetInstance() { return m_pInstance; }
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
	CNetworkVehicle					*GetVehicleFromGameVehicle(IVVehicle * pGameVehicle);
};
