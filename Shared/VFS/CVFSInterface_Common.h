//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFSInterface_Common.h
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//============================================================================== 

#pragma once

#include <stdio.h>

#ifndef VFS_MAP_CNAMES

#ifndef MAX_PATH
#define MAX_PATH         260
#endif

#ifndef VFS_SEEK_CUR
	#define VFS_SEEK_CUR    1
#endif

#ifndef VFS_SEEK_END
	#define VFS_SEEK_END    2
#endif

#ifndef VFS_SEEK_SET
	#define VFS_SEEK_SET    0
#endif

#ifndef VFS_EOF
	#define VFS_EOF			(-1)
#endif


#else

#ifndef SEEK_CUR
	#define SEEK_CUR    1
#endif

#ifndef VFS_SEEK_END
	#define SEEK_END    2
#endif

#ifndef VFS_SEEK_SET
	#define SEEK_SET    0
#endif

#ifndef VFS_EOF
	#define EOF			(-1)
#endif

#endif
