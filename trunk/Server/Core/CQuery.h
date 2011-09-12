//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CQuery.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>
#include <list>

struct QueryRule
{
	String strRule;
	String strValue;
};

class CQuery
{
private:
	int                    m_iSocket;
	std::list<QueryRule *> m_rules;

public:
	CQuery(unsigned short usPort, String strHostAddress);
	~CQuery();

	void        Process();
	QueryRule * GetRule(String strRule);
	bool        DoesRuleExist(String strRule);
	bool        AddRule(String strRule, String strValue);
	bool        RemoveRule(String strRule);
	bool        SetRule(String strRule, String strValue);
	String      GetRuleValue(String strRule);
};
