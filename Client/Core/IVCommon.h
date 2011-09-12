//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: IVCommon.h
// Project: Client.Core
// Author(s): jenksta
//            listener
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Common.h>

#define PAD(prefix, name, size) BYTE prefix##_##name##[size]

template <typename T>
struct CSimpleCollection
{
	T ** Data;
	unsigned short Count;
	unsigned short Size;
};
