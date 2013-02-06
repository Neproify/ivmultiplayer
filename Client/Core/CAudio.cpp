//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.cpp
// Project: Client.Core
// Author(s): RootKiller
//			  FRi
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CAudio.h"
#include "CClient.h"
#include <bass/bass.h>
#include <SharedUtility.h>

extern CClient * g_pClient;

CAudio::CAudio(String strStreamName, bool bReplay, bool bIsOnlineStream, bool bIsGameFile) : 
			   m_strStreamName(strStreamName), m_bReplay(bReplay), m_bIsOnlineStream(bIsOnlineStream), 
			   m_bIsGameFile(bIsGameFile), m_bUsePosition(false), m_fRange(0.0f), m_fVolume(100.0f),
			   m_bIsMuted(false), m_dwChannel(0)
{
	
}

CAudio::~CAudio()
{
	Stop();
	Unload();
}

bool CAudio::Load()
{
	// Are we not already loaded?
	if(m_dwChannel == 0)
	{
		// Create our BASS stream
		if(m_bIsOnlineStream)
			m_dwChannel = BASS_StreamCreateURL(m_strStreamName, 0, (BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE), 0, 0);
		else
			m_dwChannel = BASS_StreamCreateFile(FALSE, m_strStreamName, 0, 0, 0);

		// Did our stream create successfully?
		if(m_dwChannel != 0)
		{
			BASS_ChannelSetAttribute(m_dwChannel, BASS_ATTRIB_VOL, (m_fVolume * 0.01f));
			return true;
		}
	}

	// Already loaded
	return false;
}

void CAudio::Unload()
{
	// Are we loaded?
	if(m_dwChannel != 0)
	{
		// Free our BASS stream
		BASS_StreamFree(m_dwChannel);
		m_dwChannel = 0;
	}
}

bool CAudio::Play()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		return (BASS_ChannelPlay(m_dwChannel, FALSE) != 0);

	return false;
}

void CAudio::Pause()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		BASS_ChannelPause(m_dwChannel);
}

bool CAudio::IsPlaying()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		return (BASS_ChannelIsActive(m_dwChannel) == BASS_ACTIVE_PLAYING);

	return false;
}

bool CAudio::IsPaused()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		return (BASS_ChannelIsActive(m_dwChannel) == BASS_ACTIVE_PAUSED);

	return false;
}

bool CAudio::IsStalled()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		return (BASS_ChannelIsActive(m_dwChannel) == BASS_ACTIVE_STALLED);

	return false;
}

void CAudio::Stop()
{
	// Do we have a valid channel?
	if(m_dwChannel != 0)
		BASS_ChannelStop(m_dwChannel);
}

void CAudio::SetUsePosition(bool bUsePosition)
{
	m_bUsePosition = bUsePosition;
}

bool CAudio::GetUsePosition()
{
	return m_bUsePosition;
}

void CAudio::SetPosition(CVector3 vecPosition, float fRange)
{
	m_vecPosition = vecPosition;
	m_fRange = fRange;
}

void CAudio::GetPosition(CVector3& vecPosition, float& fRange)
{
	vecPosition = m_vecPosition;
	fRange = m_fRange;
}

void CAudio::SetVolume(float fVolume)
{
	// Ensure correct volume
	m_fVolume = Math::Clamp(0.0f, fVolume, 100.0f);

	// Do we have a valid channel?
	if(m_dwChannel != 0)
	{
		// Set new volume
		if(!m_bIsMuted)
			BASS_ChannelSetAttribute(m_dwChannel, BASS_ATTRIB_VOL, (m_fVolume * 0.01f));
	}
}

float CAudio::GetVolume()
{
	// Are we muted?
	if(m_bIsMuted)
		return 0.0f;

	// Do we have a valid channel?
	if(m_dwChannel != 0)
	{
		// Get current volume
		float fVolume;
		BASS_ChannelGetAttribute(m_dwChannel, BASS_ATTRIB_VOL, &fVolume);
		return fVolume/0.01f;
	}

	return 0.0f;
}

void CAudio::Mute()
{
	// Are we not already muted?
	if(!m_bIsMuted)
	{
		// Set our volume to 0
		if(m_dwChannel != 0)
			BASS_ChannelSetAttribute(m_dwChannel, BASS_ATTRIB_VOL, 0.0f);

		// Flag ourselves as muted
		m_bIsMuted = true;
	}
}

bool CAudio::IsMuted()
{
	return m_bIsMuted;
}

void CAudio::Unmute()
{
	// Are we muted?
	if(m_bIsMuted)
	{
		// Restore our volume
		if(m_dwChannel != 0)
			BASS_ChannelSetAttribute(m_dwChannel, BASS_ATTRIB_VOL, (m_fVolume * 0.01f));

		// Flag ourselves as not muted
		m_bIsMuted = false;
	}
}

void CAudio::Process()
{
	if(m_bUsePosition)
	{
		CVector3 vecLocalPlayer;
		g_pClient->GetLocalPlayer()->GetPosition(vecLocalPlayer);

		float fDistance = Math::GetDistanceBetweenPoints3D(vecLocalPlayer.fX, vecLocalPlayer.fY, vecLocalPlayer.fZ, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ);

		if(fDistance < m_fRange)
		{
			float fVolume = exp(-fDistance * (5.0f / m_fRange));
			SetVolume(fVolume / 0.01f);
		}
		else
			SetVolume(0.0f);
	}
}
