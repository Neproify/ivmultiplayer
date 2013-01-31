//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CTrafficLights.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

// FIXUPDATE
// jenksta: this is kinda hacky :/
#include "../Server/Core/Main.h"
#include "../Server/Core/Interfaces/InterfaceCommon.h"

class CTrafficLights : public CTrafficLightsInterface
{
private:
	static CTrafficLights * m_pInstance;
	unsigned long           m_ulTimeSet;
	bool                    m_bIsLocked;
	eTrafficLightState      m_eStateSet;
	unsigned int            m_uiGreenDuration;
	unsigned int            m_uiYellowDuration;
	unsigned int            m_uiRedDuration;
	unsigned int            m_uiTotalDuration;

public:
	CTrafficLights();
	~CTrafficLights();

	static CTrafficLights * GetInstance() { return m_pInstance; }

	void Reset();

	bool SetState(eTrafficLightState state);
	eTrafficLightState GetState();
	eTrafficLightState GetSetState();

	void SetTimeThisCycle(unsigned int uiTime);
	void CalculateCycleTime();
	unsigned int GetTimeThisCylce();

	bool IsUsingDefaultDurations();
	void ResetDefaultDurations();

	void SetGreenDuration(unsigned int uiDuration);
	void SetYellowDuration(unsigned int uiDuration);
	void SetRedDuration(unsigned int uiDuration);

	unsigned int GetGreenDuration();
	unsigned int GetYellowDuration();
	unsigned int GetRedDuration();

	void SetLocked(bool bLocked);
	bool IsLocked();

#ifdef _SERVER
	void SyncState();
#else
	eGTATrafficLightState GetTrafficLightState();
	eGTATrafficLightState GetTrafficLightAltState();
#endif
};
