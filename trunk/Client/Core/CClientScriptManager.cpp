//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClientScriptManager.h"
#include "SharedUtility.h"
#include "Natives.h"
#include <CLogFile.h>
#include "Scripting/CScriptTimerManager.h"

extern CScriptingManager * g_pScriptingManager;
extern CScriptTimerManager * g_pScriptTimerManager;

// GUIFont
_BEGIN_CLASS(GUIFont)
_MEMBER_FUNCTION(GUIFont, constructor, -1, NULL)
_MEMBER_FUNCTION(GUIFont, getTextExtent, 1, "s")
_MEMBER_FUNCTION(GUIFont, getFontHeight, 0, NULL)
_MEMBER_FUNCTION(GUIFont, drawText, 4, "ffsb")
_END_CLASS(GUIFont)

// GUIElement
_BEGIN_CLASS(GUIElement)
_MEMBER_FUNCTION(GUIElement, constructor, 1, "s")
_MEMBER_FUNCTION(GUIElement, setText, 1, "s")
_MEMBER_FUNCTION(GUIElement, setSize, 3, "ffb")
_MEMBER_FUNCTION(GUIElement, setPosition, 3, "ffb")
_MEMBER_FUNCTION(GUIElement, setRotation, 3, "fff")
_MEMBER_FUNCTION(GUIElement, setAlpha, 1, "f")
_MEMBER_FUNCTION(GUIElement, setVisible, 1, "b")
_MEMBER_FUNCTION(GUIElement, setParent, 1, "s")
_MEMBER_FUNCTION(GUIElement, getName, 0, NULL)
_MEMBER_FUNCTION(GUIElement, getText, 0, NULL)
_MEMBER_FUNCTION(GUIElement, setFont, 1, "x")
_MEMBER_FUNCTION(GUIElement, setProperty, 2, "ss")
_MEMBER_FUNCTION(GUIElement, activate, 0, NULL)
_MEMBER_FUNCTION(GUIElement, deactivate, 0, NULL)
_END_CLASS(GUIElement)

// GUIWindow
_BEGIN_CLASS(GUIWindow)
_MEMBER_FUNCTION(GUIWindow, constructor, 0, NULL)
_END_CLASS_BASE(GUIWindow, GUIElement)

// GUIText
_BEGIN_CLASS(GUIText)
_MEMBER_FUNCTION(GUIText, constructor, 0, NULL)
_MEMBER_FUNCTION(GUIText, setText, 1, "s")
_END_CLASS_BASE(GUIText, GUIElement)

// GUIButton
_BEGIN_CLASS(GUIButton)
_MEMBER_FUNCTION(GUIButton, constructor, 0, NULL)
_END_CLASS_BASE(GUIButton, GUIElement)

// GUIEditBox
_BEGIN_CLASS(GUIEditBox)
_MEMBER_FUNCTION(GUIEditBox, constructor, 0, NULL)
_END_CLASS_BASE(GUIEditBox, GUIElement)

// GUIMultiLineEditBox
_BEGIN_CLASS(GUIMultiLineEditBox)
_MEMBER_FUNCTION(GUIMultiLineEditBox, constructor, 0, NULL)
_END_CLASS_BASE(GUIMultiLineEditBox, GUIElement)

//GUIProgressBar
_BEGIN_CLASS(GUIProgressBar)
_MEMBER_FUNCTION(GUIProgressBar, constructor, 0, NULL)
_MEMBER_FUNCTION(GUIProgressBar, setValue, 1, "f")
_END_CLASS_BASE(GUIProgressBar, GUIElement)

// GUICheckBox
_BEGIN_CLASS(GUICheckBox)
_MEMBER_FUNCTION(GUICheckBox, constructor, 0, NULL)
_MEMBER_FUNCTION(GUICheckBox, setChecked, 1, "b")
_MEMBER_FUNCTION(GUICheckBox, isChecked, 0, NULL)
_END_CLASS_BASE(GUICheckBox, GUIElement)

// GUIImage
_BEGIN_CLASS(GUIImage)
_MEMBER_FUNCTION(GUIImage, constructor, 1, "s")
_END_CLASS_BASE(GUIImage, GUIElement)

// Audio
_BEGIN_CLASS(Audio)
_MEMBER_FUNCTION(Audio, constructor, 3, "bbs")
_MEMBER_FUNCTION(Audio, deleteSound, 0, NULL)
_MEMBER_FUNCTION(Audio, play, 0, NULL)
_MEMBER_FUNCTION(Audio, stop, 0, NULL)
_MEMBER_FUNCTION(Audio, pause, 0, NULL)
_MEMBER_FUNCTION(Audio, isStarted, 0, NULL)
_MEMBER_FUNCTION(Audio, setVolume, 1, "f")
_MEMBER_FUNCTION(Audio, getVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, restoreVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, setPosition, 4, "ffff")
_MEMBER_FUNCTION(Audio, clearPosition, 0, NULL)
_MEMBER_FUNCTION(Audio, usePositionSystem, 1, "b")
_END_CLASS(Audio)

CClientScriptManager::CClientScriptManager()
{
	m_pScripting = new CScriptingManager();
	g_pScriptingManager = m_pScripting;
	m_pGUIManager = new CClientScriptGUIManager();
	g_pScriptTimerManager = new CScriptTimerManager();

	// Register the client natives
	RegisterClientNatives(m_pScripting);

	// Register the world natives
	CWorldNatives::Register(m_pScripting);

	// Register the GUI natives
	RegisterGUINatives(m_pScripting);

	// Register the event natives
	CEventNatives::Register(m_pScripting);

	// Register the player natives
	CPlayerNatives::Register(m_pScripting);

	// Register the vehicle natives
	CVehicleNatives::Register(m_pScripting);

	// Register the area natives
	CAreaNatives::Register(m_pScripting);

	// Register the hash natives
	CHashNatives::Register(m_pScripting);

	// Register the script natives
	RegisterScriptNatives(m_pScripting);

	// Register the XML natives
	RegisterXMLNatives(m_pScripting);

	// Register the timer natives
	RegisterTimerNatives(m_pScripting);

	// Register the default constants
	m_pScripting->RegisterDefaultConstants();

	// GUI
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIFont));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIElement));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIWindow));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIText));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIButton));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIEditBox));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIMultiLineEditBox));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUICheckBox));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIImage));
	m_pScripting->RegisterClass(&_CLASS_DECL(GUIProgressBar));
	m_pScripting->RegisterClass(&_CLASS_DECL(Audio));
}

CClientScriptManager::~CClientScriptManager()
{
	SAFE_DELETE(g_pScriptTimerManager);
	SAFE_DELETE(m_pGUIManager);
	SAFE_DELETE(m_pScripting);

	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
		SAFE_DELETE (*iter);

	m_clientScripts.clear();
}

void CClientScriptManager::AddScript(String strName, String strPath)
{
	ClientScript * pClientScript = new ClientScript();
	pClientScript->strName = strName;
	pClientScript->strPath = strPath;
	m_clientScripts.push_back(pClientScript);
	CLogFile::Printf("ClientScript %s added.", strName.Get());
}

void CClientScriptManager::RemoveScript(String strName)
{
	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);

		if(pClientScript->strName == strName)
		{
			SAFE_DELETE (pClientScript);
			m_clientScripts.remove(pClientScript);
			CLogFile::Printf("ClientScript %s removed.", strName.Get());
			return;
		}
	}
}

void CClientScriptManager::Load(String strName)
{
	if(!Exists(strName))
		return;

	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);

		if(pClientScript->strName == strName)
		{
			m_pScripting->Load(pClientScript->strName, pClientScript->strPath);
			CLogFile::Printf("ClientScript %s loaded.", strName.Get());
			return;
		}
	}
}

void CClientScriptManager::Unload(String strName)
{
	if(!Exists(strName))
		return;

	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);

		if(pClientScript->strName == strName)
		{
			m_pScripting->Unload(strName);
			CLogFile::Printf("ClientScript %s unloaded.", strName.Get());
			return;
		}
	}
}

bool CClientScriptManager::Exists(String strName)
{
	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);

		if(pClientScript->strName == strName)
			return true;
	}

	return false;
}

void CClientScriptManager::LoadAll()
{
	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);
		Load(pClientScript->strName);
	}
}

void CClientScriptManager::RemoveAll()
{
	m_pScripting->UnloadAll();
	for(std::list<ClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++)
	{
		ClientScript * pClientScript = (*iter);
		SAFE_DELETE(pClientScript);
	}
	m_clientScripts.clear();
}