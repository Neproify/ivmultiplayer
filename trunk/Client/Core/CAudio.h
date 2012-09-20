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
	bool				m_bPlayed;
	int					m_iRepeatsNum;
	float				m_fOldVolume;

	CVector3			m_vecPosition;
	float				m_fRange;
	bool				m_bUrl;
	bool				m_bReplay;
	bool			    m_bUsingPositionSystem;
	char 				m_szSoundFile[MAX_PATH+64];
	DWORD				m_dwChannel;

public:

	CAudio	(bool bUrl, bool bReplay, const char * szSoundFile );
	~CAudio ( );

	// Functions
	void	Play ( );
	void	Stop ( );
	void	Pause ( );
	bool	IsStarted ( );
	void	SetVolume ( float fVolume );
	void	RestoreVolume ( );
	float	GetVolume ( );
	void	ClearPosition();
	void	SetPosition ( CVector3 &vecPositon, float fRange );
	void	UsePositionSystem( bool bUse );
	void	Process ();
	DWORD	GetChannel() { return m_dwChannel; }
	char	GetSoundfile() { return m_szSoundFile[MAX_PATH+64]; }
	void	SetChannel(DWORD dwChannel) { m_dwChannel = dwChannel; }
	bool	IsUrlUsed() { return m_bUrl; }
};

class CAudioManager
{
private:
	static CLibrary		*	m_pLibrary;
	static bool				m_bActive;
public:
	static std::list<CAudio *> m_Audio;
	static void Init();
	static void AddToList ( CAudio *pAudio );
	static void RemoveAll ( );
	static void SetAllVolume ( float fVolume );
	static void RestoreAllVolume ( );
	static void Process ( );
};
