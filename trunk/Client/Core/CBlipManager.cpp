//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "CBlipManager.h"
#include "CChatWindow.h"
#include "CModelmanager.h"
#include "CVehicleManager.h"

extern CVehicleManager * g_pVehicleManager;

CBlipManager::CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
		m_bActive[x] = false;
}

CBlipManager::~CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(m_bActive[x])
			Delete(x);
	}
}

void CBlipManager::Create(EntityId blipId, int iSprite, CVector3 vecPosition)
{
	// If a blip already exists in this slot delete it
	if(m_bActive[blipId])
		Delete(blipId);

	// Add the blip to the game
	Scripting::AddBlipForCoord(vecPosition.fX, vecPosition.fY, vecPosition.fZ, &m_Blips[blipId].uiBlipIndex);

	// Ensure we have a valid sprite id
	if(iSprite < 0 || iSprite > 94)
		iSprite = 3;

	// Set the blip sprite
	Scripting::ChangeBlipSprite(m_Blips[blipId].uiBlipIndex, (Scripting::eBlipSprite)iSprite);

	// Set the blip color
	SetColor(blipId, 0xFFFFFFFF);

	// Set the blip size
	SetSize(blipId, 1.0f);

	// Set short-range
	ToggleShortRange(blipId, false);

	// Set route
	ToggleRouteBlip(blipId, false);

	// Set name
	SetName(blipId, "");

	m_Blips[blipId].iSprite = iSprite;
	memcpy(&m_Blips[blipId].vecPosition, &vecPosition, sizeof(CVector3));
	m_Blips[blipId].attachedVehicle = INVALID_ENTITY_ID;
	m_bActive[blipId] = true;
}

bool CBlipManager::Delete(EntityId blipId)
{
	// Does no blip exist in this slot?
	if(!m_bActive[blipId])
	{
		CLogFile::Printf("Tried to delete inexistent blip %d", blipId);
		return false;
	}

	// Remove the blip from the game
	Scripting::RemoveBlip(m_Blips[blipId].uiBlipIndex);

	m_bActive[blipId] = false;
	return true;
}

void CBlipManager::SetColor(EntityId blipId, unsigned int uiColor)
{
	if(m_bActive[blipId])
	{
		// Change the blip color
		Scripting::ChangeBlipColour(m_Blips[blipId].uiBlipIndex, uiColor);

		// Set the blip alpha
		Scripting::ChangeBlipAlpha(m_Blips[blipId].uiBlipIndex, uiColor & 0xFF);

		m_Blips[blipId].uiColor = uiColor;
	}
}

void CBlipManager::SetSize(EntityId blipId, float fSize)
{
	if(m_bActive[blipId])
	{
		Scripting::ChangeBlipScale(m_Blips[blipId].uiBlipIndex, fSize);
		m_Blips[blipId].fSize = fSize;
	}
}

void CBlipManager::Flash(EntityId blipId, bool bFlash, int iFlashType)
{
	if(m_bActive[blipId])
	{
		bool bBlipFlash[2];

		if(!bFlash)
		{
			bBlipFlash[0] = bBlipFlash[1] = false;
		}
		else
		{
			if(iFlashType == 1)
			{
				bBlipFlash[0] = true;
				bBlipFlash[1] = false;
			}
			else
			{
				bBlipFlash[0] = false;
				bBlipFlash[1] = true;
			}
		}

		Scripting::FlashBlip(m_Blips[blipId].uiBlipIndex, bBlipFlash[0]);
		Scripting::FlashBlipAlt(m_Blips[blipId].uiBlipIndex, bBlipFlash[1]);
	}
}

void CBlipManager::ToggleShortRange(EntityId blipId, bool bToggle)
{
	if(m_bActive[blipId])
	{
		Scripting::SetBlipAsShortRange(m_Blips[blipId].uiBlipIndex, bToggle);
		m_Blips[blipId].bShortRange = bToggle;
	}
}

void CBlipManager::ToggleRouteBlip(EntityId blipId, bool bToggle)
{
	if(m_bActive[blipId])
	{
		Scripting::SetRoute(m_Blips[blipId].uiBlipIndex, bToggle);
		m_Blips[blipId].bRouteBlip = bToggle;
	}
}

void CBlipManager::SetName(EntityId blipId, String strName)
{
	if(m_bActive[blipId])
		Scripting::ChangeBlipNameFromAscii(blipId, strName);
}

void CBlipManager::AttachToVehicle(EntityId blipId, EntityId vehicleId) 
{
	if(m_bActive[blipId])
	{
		if(g_pVehicleManager->Exists(vehicleId)) {
			CNetworkVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);
			
			//Remove the position blip
			Scripting::RemoveBlip(m_Blips[blipId].uiBlipIndex);

			//And create one for the vehicle
			Scripting::AddBlipForCar(pVehicle->GetScriptingHandle(), &m_Blips[blipId].uiBlipIndex);

			// Set the blip sprite
			ChangeBlipSprite(m_Blips[blipId].uiBlipIndex, (Scripting::eBlipSprite)m_Blips[blipId].iSprite);

			// Set the blip color
			SetColor(blipId, m_Blips[blipId].uiColor);

			// Set the blip size
			SetSize(blipId, m_Blips[blipId].fSize);

			// Set short-range
			ToggleShortRange(blipId, m_Blips[blipId].bShortRange);

			// Set route
			ToggleRouteBlip(blipId, m_Blips[blipId].bRouteBlip);

			m_Blips[blipId].attachedVehicle = vehicleId;
		}
	}
}
