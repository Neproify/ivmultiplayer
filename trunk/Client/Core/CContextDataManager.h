//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CContextDataManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVPlayerInfo.h"
#include "CIVPlayerPed.h"
#include "CIVPad.h"
#include <Math/CMath.h>
#include <list>

class CContextData
{
private:
	CIVPlayerInfo * m_pPlayerInfo;
	CIVPlayerPed  * m_pPlayerPed;
	CIVPad        * m_pPad;
	CVector3        m_vecWeaponAimTarget;
	CVector3        m_vecWeaponShotSource;
	CVector3        m_vecWeaponShotTarget;

public:
	CContextData()
	{
		m_pPlayerInfo = NULL;
		m_pPlayerPed = NULL;
		m_pPad = new CIVPad();
	}

	CContextData(CIVPlayerInfo * pPlayerInfo)
	{
		m_pPlayerInfo = NULL;
		m_pPlayerPed = NULL;
		m_pPad = new CIVPad();
		m_pPlayerInfo = pPlayerInfo;
	}

	~CContextData()
	{
		delete m_pPad;
	}

	void            SetPlayerInfo(CIVPlayerInfo * pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	CIVPlayerInfo * GetPlayerInfo() { return m_pPlayerInfo; }
	void            SetPlayerPed(CIVPlayerPed * pPlayerPed) { m_pPlayerPed = pPlayerPed; }
	CIVPlayerPed  * GetPlayerPed() { return m_pPlayerPed; }
	CIVPad        * GetPad() { return m_pPad; }
	void            SetWeaponAimTarget(const CVector3& vecWeaponAimTarget) { m_vecWeaponAimTarget = vecWeaponAimTarget; }
	void            GetWeaponAimTarget(CVector3& vecWeaponAimTarget) { vecWeaponAimTarget = m_vecWeaponAimTarget; }
	void            SetWeaponShotSource(const CVector3& vecWeaponShotSource) { m_vecWeaponShotSource = vecWeaponShotSource; }
	void            GetWeaponShotSource(CVector3& vecWeaponShotSource) { vecWeaponShotSource = m_vecWeaponShotSource; }
	void            SetWeaponShotTarget(const CVector3& vecWeaponShotTarget) { m_vecWeaponShotTarget = vecWeaponShotTarget; }
	void            GetWeaponShotTarget(CVector3& vecWeaponShotTarget) { vecWeaponShotTarget = m_vecWeaponShotTarget; }
};

class CContextDataManager
{
private:
	static std::list<CContextData *> m_contextDataList;

public:
	CContextDataManager();
	~CContextDataManager();

	static CContextData * CreateContextData(CIVPlayerInfo * pPlayerInfo);
	static void           DestroyContextData(CContextData * pContextInfo);
	static CContextData * GetContextData(BYTE bytePlayerNumber);
	static CContextData * GetContextData(CIVPlayerInfo * pPlayerInfo);
	static CContextData * GetContextData(IVPlayerInfo * pPlayerInfo);
	static CContextData * GetContextData(CIVPlayerPed * pPlayerPed);
	static CContextData * GetContextData(IVPlayerPed * pPlayerPed);
};
