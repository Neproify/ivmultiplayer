//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClient.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include "CAudio.h"

class CAudioManager
{
private:
	std::list<CAudio *> m_audioList;
	bool                m_bMuted;

public:
	CAudioManager();
	~CAudioManager();

	bool Initialize();
	void Add(CAudio * pAudio);
	void Remove(CAudio * pAudio);
	void RemoveAll();
	void MuteAll();
	bool IsMuted() { return m_bMuted; }
	void UnmuteAll();
	void Process();
};
