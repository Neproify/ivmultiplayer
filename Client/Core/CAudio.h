//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.h
// Project: Client.Core
// Author(s): RootKiller
//            FRi
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <String.h>
#include <CLibrary.h>
#include <Common.h>
#include <list>

#pragma once

class CAudio
{
private:
	String   m_strStreamName;
	bool     m_bReplay;
	bool     m_bIsOnlineStream;
	bool     m_bIsGameFile;
	bool     m_bUsePosition;
	CVector3 m_vecPosition;
	float    m_fRange;
	float    m_fVolume;
	bool     m_bIsMuted;
	DWORD    m_dwChannel;

public:
	CAudio(String strStreamName, bool bReplay = false, bool bIsOnlineStream = false, bool bIsGameFile = false);
	~CAudio();

	bool  Load();
	bool  IsLoaded() { return (m_dwChannel != 0); }
	void  Unload();
	bool  Play();
	void  Pause();
	bool  IsPlaying();
	bool  IsPaused();
	bool  IsStalled();
	void  Stop();
	void  SetUsePosition(bool bUsePosition);
	bool  GetUsePosition();
	void  SetPosition(CVector3 vecPosition, float fRange);
	void  GetPosition(CVector3& vecPosition, float& fRange);
	void  SetVolume(float fVolume);
	float GetVolume();
	void  Mute();
	bool  IsMuted();
	void  Unmute();
	void  Process();
	int	  GetLength();
	bool  SetAt(int time);
	int	  GetAt();
};
