//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVDynamicEntity.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVDynamicEntity.h"
#include <CLogFile.h>

CIVDynamicEntity::CIVDynamicEntity() : CIVEntity()
{
}

CIVDynamicEntity::CIVDynamicEntity(IVDynamicEntity * pDynamicEntity) : CIVEntity(pDynamicEntity)
{
}

CIVDynamicEntity::~CIVDynamicEntity()
{
}

void CIVDynamicEntity::SetDynamicEntity(IVDynamicEntity * pDynamicEntity)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	SetEntity((IVEntity *)pDynamicEntity);
}

IVDynamicEntity * CIVDynamicEntity::GetDynamicEntity()
{
#ifdef EXT_LOG
	//CLogFile::Printf(__FUNCSIG__);
#endif
	return (IVDynamicEntity *)GetEntity();
}
