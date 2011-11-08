//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================
#include <list>
#include <String.h>
#include <bass/bass.h>
#include "CAudio.h"
#include "SharedUtility.h"
#include "CLogFile.h"
#include "CLocalPlayer.h"

extern CLocalPlayer *g_pLocalPlayer;

CAudio::CAudio ( const char * szSoundFile )
{
	sprintf( m_szSoundFile, SharedUtility::GetAbsolutePath("clientfiles\\resources\\%s", szSoundFile ).Get() );
	m_dwChannel = NULL;
	SetVolume ( 100.0f );

	if ( HIWORD(BASS_GetVersion()) !=BASSVERSION ) 
	{
		// Invalid bass version message (ADD)
		return;
	}
	
	// Init bass
	BASS_Init(-1, 44100, 0, 0, NULL);

	m_dwChannel = BASS_StreamCreateFile(FALSE, m_szSoundFile, 0, 0, BASS_SAMPLE_LOOP);

	// Add audio entity to list.
	CAudioManager::AddToList(this);

	// Clear 3d sound position.
	ClearPosition();
}

CAudio::~CAudio ( )
{
	Pause ( );
	BASS_Free();
}

void CAudio::Play ( )
{
	BASS_ChannelPlay ( m_dwChannel, FALSE );
	m_bPlayed = true;
}

void CAudio::Stop ( )
{
	BASS_ChannelPause ( m_dwChannel );
	// TODO -add clear buffer.
	m_dwChannel = BASS_StreamCreateFile(FALSE, m_szSoundFile, 0, 0, BASS_SAMPLE_LOOP);
	m_bPlayed = false;
}

void CAudio::Pause ( )
{
	BASS_ChannelPause ( m_dwChannel );
	m_bPlayed = false;
}

bool CAudio::IsStarted ( )
{
	if ( m_bPlayed )
	{
		return true;
	}
	return false;
}

void CAudio::SetVolume ( float fVolume )
{
	// Check volume
	if ( fVolume > 100.0f)
	{	
		fVolume = 100.0f;
	}

	if ( fVolume < 0.0f )
	{
		fVolume = 0.0f;
	}

	// Get current volume.
	BASS_ChannelGetAttribute ( m_dwChannel, BASS_ATTRIB_VOL, &m_fOldVolume );

	// Set new volume.
	BASS_ChannelSetAttribute( m_dwChannel, BASS_ATTRIB_VOL, fVolume*0.01f);
}

void CAudio::PreRestoreVolume ( )
{

}

void CAudio::RestoreVolume ( )
{
	BASS_ChannelSetAttribute( m_dwChannel, BASS_ATTRIB_VOL, m_fOldVolume );
}

float CAudio::GetVolume ( )
{
	// Define volume varible.
	float fVolume;
	// Get current volume.
	BASS_ChannelGetAttribute ( m_dwChannel, BASS_ATTRIB_VOL, &fVolume );

	// Return current volume.
	return fVolume/0.01f;
}

bool CAudio::SetRepeats ( int iRepeatsNum )
{
	// TODO
	return false;
}

int CAudio::GetRepeats ( )
{
	// TODO
	return 0;
}

void  CAudio::ClearPosition()
{
	m_vecPosition = CVector3();
	m_fRange = 0.0f;
}

void CAudio::SetPosition ( CVector3 &vecPositon, float fRange )
{
	m_vecPosition = vecPositon;
	m_fRange = fRange;
}

void CAudio::Process ( )
{
	// Check 3d sound state. (Maybe bugged)
	if ( m_vecPosition.IsEmpty ( ) )
		return ;

	CVector3 vecLocalPlayer;

	g_pLocalPlayer->GetPosition(vecLocalPlayer);

	float fDistance = Math::GetDistanceBetweenPoints3D(vecLocalPlayer.fX, vecLocalPlayer.fY, vecLocalPlayer.fZ, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ );
	float fVolume = 0.0f;
	
	if ( fDistance < m_fRange )
	{
		fVolume = exp ( -fDistance * ( 5.0f / m_fRange ) );
		SetVolume ( fVolume/0.01f );
	} else {
		SetVolume ( 0.0f );
	}
}

std::list<CAudio *> CAudioManager::m_Audio;

//Audio manager
void CAudioManager::AddToList ( CAudio *pAudio )
{
	m_Audio.push_back ( pAudio );
}

void CAudioManager::RemoveAll ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		m_Audio.remove((*iter));
		SAFE_DELETE((*iter));
	}
}

void CAudioManager::SetAllVolume ( float fVolume )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		(*iter)->SetVolume ( fVolume );
	}
}

void CAudioManager::RestoreAllVolume ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		(*iter)->RestoreVolume ( );
	}
}

void CAudioManager::Process ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		(*iter)->Process ( );
	}
}
