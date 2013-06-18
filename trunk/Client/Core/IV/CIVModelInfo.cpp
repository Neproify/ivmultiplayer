//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVModelInfo.cpp
// Project: Client.Core
// Author(s): jenksta
//            Multi Theft Auto Team
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Also increment/decrement the internal model info references
// on load/unload

#include "CIVModelInfo.h"
#include "../CGame.h"
#include <CLogFile.h>

CIVModelInfo::CIVModelInfo()
	: m_iModelIndex(-1),
	m_dwReferenceCount(0)
{
}

CIVModelInfo::CIVModelInfo(int iModelIndex)
{
	SetIndex(iModelIndex);
}

CIVModelInfo::~CIVModelInfo()
{
}

IVBaseModelInfo * CIVModelInfo::GetModelInfo()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_iModelIndex >= 0 && m_iModelIndex < NUM_ModelInfos)
		return *(IVBaseModelInfo **)((CGame::GetBase() + ARRAY_ModelInfos) + (m_iModelIndex * 4));

	return NULL;
}

void CIVModelInfo::SetIndex(int iModelIndex)
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	m_iModelIndex = iModelIndex;
}

BYTE CIVModelInfo::GetType()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
	{
		DWORD dwFunc = pModelInfo->m_VFTable->GetType;
		BYTE byteType = 0;
		_asm
		{
			mov ecx, pModelInfo
			call dwFunc
			mov byteType, al
		}
		return byteType;
	}

	return 0;
}

bool CIVModelInfo::IsLoaded()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(IsValid())
		return CGame::GetStreaming()->HasResourceLoaded(RESOURCE_TYPE_WDR, m_iModelIndex);

	return false;
}

void CIVModelInfo::Load(bool bWaitForLoad)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Is the model invalid?
	if(!IsValid())
		return;

	// Are we already loaded?
	if(IsLoaded())
		return;

	// Request the model
	CGame::GetStreaming()->RequestResource(RESOURCE_TYPE_WDR, m_iModelIndex);

	// Do we need to wait for the model load?
	if(bWaitForLoad)
	{
		// Load all requested models
		CGame::GetStreaming()->LoadAllResources();

		// Loop until the model is loaded
		while(!IsLoaded())
			Sleep(10);
	}
}

void CIVModelInfo::Unload()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Is the model invalid?
	if(!IsValid())
		return;

	// Are we not already loaded?
	if(!IsLoaded())
		return;

	// Release the model
	CGame::GetStreaming()->ReleaseResource(RESOURCE_TYPE_WDR, m_iModelIndex);
}

DWORD CIVModelInfo::GetHash()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		return pModelInfo->dwHash;

	return NULL;
}

void CIVModelInfo::AddReference(bool bWaitForLoad)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	//IVBaseModelInfo * pModelInfo = GetModelInfo();

	//if(pModelInfo)
	//	pModelInfo->dwReferenceCount++;

	// Is the model invalid?
	if(!IsValid())
		return;

	// Increment the reference count
	m_dwReferenceCount++;

	//CLogFile::Printf("Added model reference for model %d (Reference count is now %d)", m_iModelIndex, m_dwReferenceCount);

	// Is this the first reference?
	if(m_dwReferenceCount == 1)
	{
		//CLogFile::Printf("Loading model %d", m_iModelIndex);

		// Load the model
		Load(bWaitForLoad);
	}
}

void CIVModelInfo::RemoveReference()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	//IVBaseModelInfo * pModelInfo = GetModelInfo();

	//if(pModelInfo)
	//	pModelInfo->dwReferenceCount--;

	// Is the model invalid?
	if(!IsValid())
		return;

	if(m_dwReferenceCount > 0)
	{
		m_dwReferenceCount--;
		//CLogFile::Printf("Removed model reference for model %d (Reference count is now %d)", m_iModelIndex, m_dwReferenceCount);

		// Is this the last reference?
		if(m_dwReferenceCount == 0)
		{
			//CLogFile::Printf("Unlading model %d", m_iModelIndex);

			// Unload the model
			Unload();
		}
	}
	else
		CLogFile::Printf("Attempted to remove reference from zero reference model %d", m_iModelIndex);
}

DWORD CIVModelInfo::GetReferenceCount()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	// Is the model invalid?
	if(!IsValid())
		return 0;

	//IVBaseModelInfo * pModelInfo = GetModelInfo();

	//if(pModelInfo)
	//	return pModelInfo->dwReferenceCount;

	//return NULL;
	return m_dwReferenceCount;
}

WORD CIVModelInfo::GetAnimIndex()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		return pModelInfo->wAnimIndex;

	return NULL;
}

bool CIVModelInfo::IsAutomobile()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_AUTOMOBILE);

	return false;
}

bool CIVModelInfo::IsBike()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_BIKE);

	return false;
}

bool CIVModelInfo::IsBoat()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_BOAT);

	return false;
}

bool CIVModelInfo::IsTrain()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_TRAIN);

	return false;
}

bool CIVModelInfo::IsHeli()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_HELI);

	return false;
}

bool CIVModelInfo::IsPlane()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_PLANE);

	return false;
}