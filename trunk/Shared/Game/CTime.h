//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CTime.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

// FIXUPDATE
// jenksta: this is kinda hacky :/
#include "../Server/Core/Main.h"
#include "../Server/Core/Interfaces/InterfaceCommon.h"


class CTime/* : public CTimeInterface*/
{
public:
	static CTime            * m_pInstance;
	static const unsigned int DEFAULT_MINUTE_DURATION;

private:
	unsigned long m_ulTimeSet;
	unsigned char m_ucHour;
	unsigned char m_ucMinute;
	unsigned char m_ucDayOfWeek;
	unsigned int m_uiMinuteDuration;
public:
	CTime();
	~CTime();

	static CTime * GetInstance() { return m_pInstance; }

	void SetTime(const unsigned char ucHour, const unsigned char ucMinute);
	void GetTime(unsigned char * ucHour, unsigned char * ucMinute);

	void SetMinuteDuration(const unsigned int uiMinuteDuration);
	unsigned int GetMinuteDuration();

	void SetDayOfWeek(const unsigned char ucDayOfWeek);
	unsigned char GetDayOfWeek();

#ifdef _SERVER
	void SyncTime();
#endif
};
