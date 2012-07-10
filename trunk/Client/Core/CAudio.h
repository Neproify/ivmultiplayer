//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.h
// Project: Client.Core
// Author(s): RootKiller, FRi
// License: See LICENSE in root directory
//
//==============================================================================
#include <String.h>
#include <CLibrary.h>
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
	bool				m_bUrl;
	bool				m_bReplay;
	bool			    m_bUsingPositionSystem;
public:
	CAudio	(bool bUrl, bool bReplay, const char * szSoundFile );
	~CAudio ( );

	// Functions
	void Play ( );
	void Stop ( );
	void Pause ( );
	bool IsStarted ( );
	void  SetVolume ( float fVolume );
	void  RestoreVolume ( );
	float GetVolume ( );
	void  ClearPosition();
	void  SetPosition ( CVector3 &vecPositon, float fRange );
	void  UsePositionSystem( bool bUse );
	void  Process ();
};

class CAudioManager
{
private:
	static CLibrary		*	m_pLibrary;
	static bool				m_bActive;
public:
	static void Init();
	static std::list<CAudio *> m_Audio;
	static void AddToList ( CAudio *pAudio );
	static void RemoveAll ( );
	static void SetAllVolume ( float fVolume );
	static void RestoreAllVolume ( );
	static void Process ( );
};
