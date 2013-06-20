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

class IVDynamicEntity : public IVEntity
{
public:
	// 000-074
	DWORD m_pPtrNodeDouble;			  // PtrNodeDouble
	DWORD m_pAnim;                    // 078-07C
	// 0x80 - 0xCC = CPortalTracker m_portalTracker; (+0x30 = CDynamicEntity * m_pDynamicEntity)
	PAD(IVDynamicEntity, pad1, 0x90); // 07C-10C

	virtual void 		Function51(); // idk
	virtual void		Function52(); // return 0;
	virtual void		Function53(); // return 0;
	virtual void		Function54(); // CPortalTracker
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