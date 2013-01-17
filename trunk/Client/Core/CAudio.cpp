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
#include <Threading/CThread.h>
#include <Threading/CMutex.h>

//void AudioThreadProcess(CThread * pCreator);

extern CLocalPlayer *g_pLocalPlayer;
extern CChatWindow * g_pChatWindow;
std::list<CAudio *> CAudioManager::m_Audio;
CLibrary * CAudioManager::m_pLibrary = NULL;

CAudio::CAudio (bool bUrl, bool bReplay, bool bGameFile, const char * szSoundFile )
{
	if(!bUrl)
	{
		if(!bGameFile)
			sprintf( m_szSoundFile, SharedUtility::GetAbsolutePath("clientfiles\\resources\\%s", szSoundFile ).Get() );
		else
			sprintf( m_szSoundFile, SharedUtility::GetAbsolutePath("gameplay_files\\audio\\%s", szSoundFile ).Get() );
		m_dwChannel = NULL;

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

		// Set default volume
		SetVolume ( 100.0f );
	}
	else if(bUrl)
	{
		/*char szPath[MAX_PATH];
		sprintf(szPath, szSoundFile);
		for(size_t i = strlen(szPath); i > 0; i--)
		{
			if(szPath[i] == '/')
			{
				szPath[i]='\\';
			}
		}
		sprintf( m_szSoundFile, "%s", szPath );

		CLogFile::Printf("%s", m_szSoundFile);*/
		sprintf( m_szSoundFile, "%s", szSoundFile );
		m_dwChannel = NULL;

		if ( HIWORD(BASS_GetVersion()) !=BASSVERSION )
		{
			g_pChatWindow->AddInfoMessage("Audio: Failed to play music %s, invalid audio client version!",szSoundFile);
			return;
		}
	
		// Init bass
		BASS_Init(-1, 44100, 0, 0, NULL);

		m_dwChannel = BASS_StreamCreateURL(m_szSoundFile,0,BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,0,0);

		// Add audio entity to list.
		CAudioManager::AddToList(this);

		// Clear 3d sound position.
		ClearPosition();

		// Set default volume
		SetVolume ( 100.0f );
	}

	// Apply url
	m_bUrl = bUrl;
	m_bReplay = bReplay;
	m_bUsingPositionSystem = false;
}

CAudio::~CAudio ( )
{
	Pause ( );
	BASS_Free();
}

void CAudio::Play ( )
{
	if( m_dwChannel == -1 )
	{
		// Recreate channel
		if(m_bUrl)
			m_dwChannel = BASS_StreamCreateURL(m_szSoundFile,0,BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,0,0);
		else if(!m_bUrl)
			m_dwChannel = BASS_StreamCreateFile(FALSE, m_szSoundFile, 0, 0, BASS_SAMPLE_LOOP);
	}

	// Fix this shit replay!
	if(m_bReplay)
		BASS_ChannelPlay ( m_dwChannel, (BOOL)TRUE );
	else if(!m_bReplay)
		BASS_ChannelPlay ( m_dwChannel, (BOOL)FALSE );

	m_bPlayed = true;
}

void CAudio::Stop ( )
{
	// stop channel
	BASS_ChannelStop( m_dwChannel );
	m_dwChannel = -1;

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

bool CAudio::IsPlaying ( )
{
	return BASS_ChannelIsActive ( m_dwChannel ) == BASS_ACTIVE_PLAYING;
	return false;
}

bool CAudio::IsPaused ( )
{
	return BASS_ChannelIsActive ( m_dwChannel ) == BASS_ACTIVE_PAUSED;
	return false;
}

bool CAudio::IsStalled ( )
{
	return BASS_ChannelIsActive ( m_dwChannel ) == BASS_ACTIVE_STALLED;
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

void CAudio::UsePositionSystem( bool bUse )
{
	m_bUsingPositionSystem = bUse;
}

void CAudio::Process ( )
{
	if ( m_vecPosition.IsEmpty ( ) )
		return ;

	if( !m_bUsingPositionSystem )
		return;

	CVector3 vecLocalPlayer;

	g_pLocalPlayer->GetPosition(vecLocalPlayer);

	float fDistance = Math::GetDistanceBetweenPoints3D(vecLocalPlayer.fX, vecLocalPlayer.fY, vecLocalPlayer.fZ, m_vecPosition.fX, m_vecPosition.fY, m_vecPosition.fZ );
	float fVolume = 0.0f;
	
	if ( fDistance < m_fRange )
	{
		fVolume = exp ( -fDistance * ( 5.0f / m_fRange ) );
		SetVolume ( fVolume/0.01f );
	} 
	else 
		SetVolume ( 0.0f );
}

void CAudioManager::Init( )
{
	//CThread	m_pThread;
	//m_pThread.SetUserData<bool>(true);
	//m_pThread.Start(AudioThreadProcess);

	m_pLibrary = new CLibrary();
}

void CAudioManager::AddToList ( CAudio *pAudio )
{
	m_Audio.push_back ( pAudio );
}

void CAudioManager::RemoveAll ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
		SAFE_DELETE((*iter));

	m_Audio.clear();
}

void CAudioManager::SetAllVolume ( float fVolume )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
		(*iter)->SetVolume ( fVolume );
}

void CAudioManager::RestoreAllVolume ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
		(*iter)->RestoreVolume ( );
}

void CAudioManager::Process ( )
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
			(*iter)->Process ( );
}

void CAudioManager::ProcessThread()
{
	for(std::list<CAudio *>::iterator iter = m_Audio.begin(); iter != m_Audio.end(); iter++)
	{
		if((*iter)->GetChannel() == -1)
		{
			if((*iter)->IsUrlUsed())
			{
				DWORD dwChannel = BASS_StreamCreateURL((const char*)(*iter)->GetSoundfile(),0,BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,0,0);
				(*iter)->SetChannel(dwChannel);
			}
			else
			{
				DWORD dwChannel = BASS_StreamCreateFile(FALSE, (const char*)(*iter)->GetSoundfile(), 0, 0, BASS_SAMPLE_LOOP);
				(*iter)->SetChannel(dwChannel);
			}
		}
	}
}