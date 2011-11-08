//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.h
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#pragma once
#include <String.h>
#include "Common.h"
#include <list>

class CAudio
{
private:
	char 				m_szSoundFile[MAX_PATH+64];
	bool				m_bPlayed;
	int					m_iRepeatsNum;
	DWORD				m_dwChannel;
	float				m_fOldVolume;

	CVector3			m_vecPosition;
	float				m_fRange;
public:
	CAudio	( const char * szSoundFile );
	~CAudio ( );

	// Functions
	void Play ( );
	void Stop ( );
	void Pause ( );
	bool SetRepeats ( int iRepeatsNum );
	int	 GetRepeats ( );
	bool IsStarted ( );

	void  SetVolume ( float fVolume );
	void  RestoreVolume ( );
	void  PreRestoreVolume ( );
	float GetVolume ( );
	void  ClearPosition();
	void  SetPosition ( CVector3 &vecPositon, float fRange );

	void  Process ();
};

class CAudioManager
{
public:
	static std::list<CAudio *> m_Audio;
	static void AddToList ( CAudio *pAudio );
	static void RemoveAll ( );
	static void SetAllVolume ( float fVolume );
	static void RestoreAllVolume ( );
	static void Process ( );
};
