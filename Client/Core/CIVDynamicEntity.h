//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVDynamicEntity.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVEntity.h"

class IVDynamicEntityVFTable : public IVEntityVFTable
{
public:
	DWORD mCC;
	DWORD mD0;
	DWORD mD4;
	DWORD mD8;
};

class IVDynamicEntity : public IVEntity
{
public:
	// 000-074
	PAD(IVDynamicEntity, pad0, 0x4);  // 074-078
	DWORD m_pAnim;                    // 078-07C
	// 0x80 = CPortalTracker * m_pPortalTracker
	PAD(IVDynamicEntity, pad1, 0x90); // 07C-10C
};

class CIVDynamicEntity : public CIVEntity
{
public:
	CIVDynamicEntity();
	CIVDynamicEntity(IVDynamicEntity * pDynamicEntity);
	~CIVDynamicEntity();

	void              SetDynamicEntity(IVDynamicEntity * pDynamicEntity);
	IVDynamicEntity * GetDynamicEntity();
};
