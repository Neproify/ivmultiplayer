//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptGUIManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

//include "CD3D9Webkit.hpp"
#include "CClientScriptGUIManager.h"
#include "CGUI.h"
#include <Common.h>

//extern CD3D9WebKit * g_pWebkit;

CClientScriptGUIManager::CClientScriptGUIManager()
{
	// Flag ourselves as not hidden
	m_bHidden = false;
}

CClientScriptGUIManager::~CClientScriptGUIManager()
{
	// Delete all elements
	DeleteAll();
}

void CClientScriptGUIManager::Add(CEGUI::Window * pWindow, CSquirrel * pScript)
{
	// Create a new GUIElement structure
	GUIElement * pElement = new GUIElement;

	// Set the new structure
	pElement->pWindow = pWindow;
	pElement->pScript = pScript;
	pElement->bState = true;

	// Add the new element to the element list
	m_elements.push_back(pElement);
}

void CClientScriptGUIManager::Delete(CEGUI::Window * pWindow)
{
	// Loop through all elements
	for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
	{
		// Get the GUIElement pointer
		GUIElement * pElement = (*iter);

		// Is this the element we are looking for?
		if(pElement && pElement->pWindow == pWindow)
		{
			/*
			// Check if this window present in d3d9webkit
			CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
			if(pView)
			{
				g_pWebkit->DestroyView(pView);
			}

			// Remove the GUI window
			g_pGUI->RemoveGUIWindow(pWindow);
			*/

			// Delete the element
			SAFE_DELETE( pElement );

			// Erase the element from the element list
			m_elements.erase(iter);

			break;
		}
	}
}

void CClientScriptGUIManager::DeleteAll()
{
	for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
	{
		// (*iter)->pScript->Unload(); Why do we unload script here?
		(*iter)->pWindow->hide();
	}	
			
	// CrackHD: who did this lol to iterate elements and call SAFE_DELETE on every iter??
	m_elements.clear();
}

bool CClientScriptGUIManager::Exists(CEGUI::Window * pWindow)
{
	// Loop through all elements
	for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
	{
		// Is this the element we are looking for?
		if((*iter)->pWindow == pWindow)
			return true;
	}

	// Window not found
	return false;
}

void CClientScriptGUIManager::Show()
{
	// Are we hidden?
	if(m_bHidden)
	{
		// Restore element visible state
		for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
		{
			(*iter)->pWindow->setVisible((*iter)->bState);
		}
		// Flag ourselves as not hidden
		m_bHidden = false;
	}
}

void CClientScriptGUIManager::Hide()
{
	// Are we not already hidden?
	if(!m_bHidden)
	{
		// Firstly save visible state
		for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
		{
			(*iter)->bState = (*iter)->pWindow->isVisible();
		}

		// After, set all elements invisible
		for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
		{
			(*iter)->pWindow->setVisible(false);
		}

		// Flag ourselves as hidden
		m_bHidden = true;
	}
}

CSquirrel * CClientScriptGUIManager::GetScript(CEGUI::Window * pWindow)
{
	// Loop through all elements
	for(std::list<GUIElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
	{
		// Is this the element we are looking for?
		if((*iter)->pWindow == pWindow)
			return (*iter)->pScript;
	}

	// Window not found
	return NULL;
}
