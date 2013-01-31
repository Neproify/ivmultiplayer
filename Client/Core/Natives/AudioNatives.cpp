//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AudioNatives.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "AudioNatives.h"
#include <Scripting/CScriptingManager.h>
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqstring.h>
#include "../CClient.h"

extern CClient * g_pClient;

_MEMBER_FUNCTION_IMPL(Audio, constructor)
{
	SQBool sqbIsOnlineStream;
	sq_getbool(pVM, -3, &sqbIsOnlineStream);
	bool bIsOnlineStream = (sqbIsOnlineStream != 0);

	SQBool sqbReplay;
	sq_getbool(pVM, -2, &sqbReplay);
	bool bReplay = (sqbReplay != 0);

	const char * szSoundName;
	sq_getstring(pVM, -1, &szSoundName);

	CAudio * pAudio = new CAudio(szSoundName, bReplay, bIsOnlineStream);

	if(!pAudio || !pAudio->Load() || SQ_FAILED(sq_setinstance(pVM, pAudio)))
	{
		CLogFile::Printf("Failed to load audio from file %s",szSoundName);
		SAFE_DELETE(pAudio);
		sq_pushbool(pVM, false);
		return 1;
	}

	g_pClient->GetAudioManager()->Add(pAudio);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, deleteSound)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	g_pClient->GetAudioManager()->Remove(pAudio);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, play)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushbool(pVM, pAudio->Play());
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, stop)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	pAudio->Stop();
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, pause)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	pAudio->Pause();
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isStarted)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushbool(pVM, (pAudio->IsPlaying() || pAudio->IsStalled()));
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPlaying)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushbool( pVM, pAudio->IsPlaying() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPaused)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushbool( pVM, pAudio->IsPaused() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isStalled)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushbool( pVM, pAudio->IsStalled() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, setVolume)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	float fVolume;
	sq_getfloat ( pVM, 2, &fVolume );
	
	pAudio->SetVolume(fVolume);

	sq_pushbool ( pVM, true );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, restoreVolume)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	pAudio->Unmute( );

	sq_pushbool ( pVM, true );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, getVolume)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	sq_pushfloat ( pVM, pAudio->GetVolume( ) );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, setPosition)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	float fX, fY, fZ, fR;

	sq_getfloat ( pVM, 2, &fX );
	sq_getfloat ( pVM, 3, &fY );
	sq_getfloat ( pVM, 4, &fZ );
	sq_getfloat ( pVM, 5, &fR );

	pAudio->SetPosition (CVector3(fX, fY, fZ), fR );

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, clearPosition)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);

	pAudio->SetUsePosition(false);

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, usePositionSystem)
{
	CAudio * pAudio = sq_getinstance<CAudio *>(pVM);
	SQBool bGet;
	sq_getbool(pVM, -1, &bGet );
	bool bToggle = (bGet != 0);
	
	pAudio->SetUsePosition(bToggle);

	sq_pushbool ( pVM, true );
	return 1;
}
