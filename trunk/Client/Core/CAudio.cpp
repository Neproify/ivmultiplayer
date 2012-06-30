//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CAudio.cpp
// Project: Client.Core
// Author(s): RootKiller
//			  FRi
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
#include "CChatWindow.h"

extern CLocalPlayer *g_pLocalPlayer;
extern CChatWindow * g_pChatWindow;
std::list<CAudio *> CAudioManager::m_Audio;
CLibrary * CAudioManager::m_pLibrary = NULL;

CAudio::CAudio (bool url, const char * szSoundFile )
{
	if(!url)
	{
		sprintf( m_szSoundFile, SharedUtility::GetAbsolutePath("clientfiles\\resources\\%s", szSoundFile ).Get() );
		m_dwChannel = NULL;
		SetVolume ( 100.0f );

		if ( HIWORD(BASS_GetVersion()) != BASSVERSION ) 
		{
			g_pChatWindow->AddInfoMessage("Audio: Failed to play music %s, invalid audio client version!",szSoundFile);
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
	else {
		sprintf( m_szSoundFile, szSoundFile);
		m_dwChannel = NULL;
		SetVolume ( 100.0f );

		if ( HIWORD(BASS_GetVersion()) !=BASSVERSION ) 
		{
			g_pChatWindow->AddInfoMessage("Audio: Failed to play music %s, invalid audio client version!",szSoundFile);
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
		return true;

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

void CAudioManager::Init( )
{
	m_pLibrary = new CLibrary();

}

void CAudioManager::AddToList ( CAudio *pAudio )
{
	m_Audio.push_back ( pAudio );
}

void CAudioManager::RemoveAll ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_Audio.clear();
}

void CAudioManager::SetAllVolume ( float fVolume )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		if((*iter) && (*iter)->IsStarted())
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
		if((*iter) && (*iter)->IsStarted())
			(*iter)->Process ( );
	}
}
