//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSettings.h
// Project: Shared
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <map>
#include <list>
#include "Common.h"
#include "CString.h"
#include <tinyxml/tinyxml.h>
#include <tinyxml/ticpp.h>

// Set macros
#define CVAR_SET_BOOL CSettings::SetBool
#define CVAR_SET_INTEGER CSettings::SetInteger
#define CVAR_SET_FLOAT CSettings::SetFloat
#define CVAR_SET_STRING CSettings::SetString
#define CVAR_SET_LIST CSettings::AddToList
#define CVAR_SET_EX CSettings::SetEx

// Get macros
#define CVAR_GET_BOOL CSettings::GetBool
#define CVAR_GET_INTEGER CSettings::GetInteger
#define CVAR_GET_FLOAT CSettings::GetFloat
#define CVAR_GET_STRING CSettings::GetString
#define CVAR_GET_LIST CSettings::GetList
#define CVAR_GET_EX CSettings::GetEx

enum eSettingsFlags
{
	SETTINGS_FLAG_BOOL = 1,
	SETTINGS_FLAG_INTEGER = 2,
	SETTINGS_FLAG_FLOAT = 4,
	SETTINGS_FLAG_STRING = 8,
	SETTINGS_FLAG_LIST = 16
};

struct SettingsValue
{
	// TODO: For this we could just do the following
	// and represent all value types with a string
	/*
	std::list<String> value;
	std::list<String> minimumValue;
	std::list<String> maximumValue;
	*/
	char              cFlags;
	bool              bValue;
	int               iValue;
	float             fValue;
	String            strValue;
	std::list<String> listValue;
	int               iMinimumValue;
	float             fMinimumValue;
	int               iMaximimValue;
	float             fMaximimValue;

	bool IsBool() { return IS_BIT_SET(cFlags, SETTINGS_FLAG_BOOL); }
	bool IsInteger() { return IS_BIT_SET(cFlags, SETTINGS_FLAG_INTEGER); }
	bool IsFloat() { return IS_BIT_SET(cFlags, SETTINGS_FLAG_FLOAT); }
	bool IsString() { return IS_BIT_SET(cFlags, SETTINGS_FLAG_STRING); }
	bool IsList() { return IS_BIT_SET(cFlags, SETTINGS_FLAG_LIST); }
};

class CSettings
{
private:
	static std::map<String, SettingsValue *> m_values;
	static bool                              m_bOpen;
	static bool                              m_bSave;
	static TiXmlDocument                     m_XMLDocument;

	static void                                LoadDefaults();
	static SettingsValue                     * GetSetting(String strSetting);

public:
	CSettings();
	~CSettings();

	static std::map<String, SettingsValue *> * GetValues() { return &m_values; }
	static bool                                Open(String strPath, bool bCreate = true, bool bSave = true);
	static bool                                Close();
	static bool                                Save();

	static bool                                AddBool(String strSetting, bool bDefaultValue);
	static bool                                AddInteger(String strSetting, int iDefaultValue, int iMinimumValue, int iMaximumValue);
	static bool                                AddFloat(String strSetting, float fDefaultValue, float fMinimumValue, float fMaximumValue);
	static bool                                AddString(String strSetting, String strDefaultValue);
	static bool                                AddList(String strSetting);

	static bool                                SetBool(String strSetting, bool bValue);
	static bool                                SetInteger(String strSetting, int iValue);
	static bool                                SetFloat(String strSetting, float fValue);
	static bool                                SetString(String strSetting, String strValue);
	static bool                                AddToList(String strSetting, String strValue);
	static bool                                SetEx(String strSetting, String strValue);

	static bool                                GetBool(String strSetting);
	static int                                 GetInteger(String strSetting);
	static float                               GetFloat(String strSetting);
	static String                              GetString(String strSetting);
	static std::list<String>                   GetList(String strSetting);
	static String                              GetEx(String strSetting);

	static bool                                Exists(String strSetting);
	static bool                                IsBool(String strSetting);
	static bool                                IsInteger(String strSetting);
	static bool                                IsFloat(String strSetting);
	static bool                                IsString(String strSetting);
	static bool                                IsList(String strSetting);

	static bool                                Remove(String strSetting);

	static void                                ParseCommandLine(int argc, char ** argv);
	static void                                ParseCommandLine(char * szCommandLine);
};
