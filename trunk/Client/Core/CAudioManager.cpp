//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClient.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CAudioManager.h"
#include <bass/bass.h>
#include "CClient.h"
#include <CLogFile.h>

extern CClient * g_pClient;

CAudioManager::CAudioManager() : m_bMuted(false)
{
	
}

CAudioManager::~CAudioManager()
{
	// Remove all audio
	RemoveAll();

	// Free the BASS audio library
	BASS_Free();
}

bool CAudioManager::Initialize()
{
	// Ensure we have a valid BASS version
	if(HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		CLogFile::Printf("Invalid BASS version");
		return false;
	}

	// Initialize the BASS audio library
	BASS_Init(-1, 44100, 0, 0, NULL);
	return true;
}

void CAudioManager::Add(CAudio * pAudio)
{
	m_audioList.push_back(pAudio);

	if(m_bMuted)
		pAudio->Mute();
}

void CAudioManager::Remove(CAudio * pAudio)
{
	m_audioList.remove(pAudio);
	SAFE_DELETE(pAudio);
}

void CAudioManager::RemoveAll()
{
	for(std::list<CAudio *>::iterator iter = m_audioList.begin(); iter != m_audioList.end(); iter++)
		SAFE_DELETE(*iter);

	m_audioList.clear();
}

void CAudioManager::MuteAll()
{
	// Are we already muted?
	if(m_bMuted)
		return;

	for(std::list<CAudio *>::iterator iter = m_audioList.begin(); iter != m_audioList.end(); iter++)
		(*iter)->Mute();

	// Flag us as muted
	m_bMuted = true;
}

void CAudioManager::UnmuteAll()
{
	// Are we not muted?
	if(!m_bMuted)
		return;

	for(std::list<CAudio *>::iterator iter = m_audioList.begin(); iter != m_audioList.end(); iter++)
		(*iter)->Unmute();

	// Flag us at not mutex
	m_bMuted = false;
}

void CAudioManager::Process()
{
	for(std::list<CAudio *>::iterator iter = m_audioList.begin(); iter != m_audioList.end(); iter++)
		(*iter)->Process();
}
