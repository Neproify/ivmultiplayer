//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptingManager.h
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <string>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include "CSquirrel.h"

template <typename T>
static SQRESULT sq_setinstance(SQVM * pVM, T pInstance, int iIndex = 1)
{
	return sq_setinstanceup(pVM, iIndex, (SQUserPointer *)pInstance);
}

// Now a function
template <typename T>
static T sq_getinstance(SQVM * pVM, int iIndex = 1)
{
	SQUserPointer pInstance = NULL;

	if(SQ_FAILED(sq_getinstanceup(pVM, iIndex, &pInstance, NULL)))
		pInstance = NULL;

	return (T)pInstance;
}

#define _SET_RELEASE_HOOK(classname) \
	sq_setreleasehook(pVM, 1, __##classname##_releasehook);

#define _MEMBER_FUNCTION_IMPL(classname, name) \
	SQInteger __##classname##_##name(SQVM * pVM)

#define _MEMBER_FUNCTION_RELEASE_HOOK(classname) \
	SQInteger __##classname##_releasehook(SQUserPointer pInst, SQInteger size)

#define _BEGIN_CLASS(classname)  \
	SQInteger __##classname##__typeof(SQVM * pVM) \
	{ \
		sq_pushstring(pVM, #classname, -1); \
		return 1; \
	} \
	struct ScriptClassMemberDecl __##classname##_members[] = \
	{ \
		{ "_typeof", __##classname##__typeof },

#define _MEMBER_FUNCTION(classname, name, parametercount, functiontemplate) \
		{ #name, __##classname##_##name, parametercount, functiontemplate },

#define _END_CLASS(classname) \
		{ NULL, NULL } \
	}; \
	struct SquirrelClassDecl __##classname##_decl = \
		{  #classname, NULL, __##classname##_members }; 

#define _END_CLASS_BASE(classname, baseclassname) \
		{ NULL, NULL } \
	}; \
	struct SquirrelClassDecl __##classname##_decl = \
{  #classname, #baseclassname, __##classname##_members }; 

#define _CLASS_DECL(classname) \
	__##classname##_decl

struct ScriptingFunction
{
	String     strName;
	SQFUNCTION pfnFunction;
	int        iParameterCount;
	String     strTemplate;
};

struct ScriptingConstant
{
	String            strName;
	CSquirrelArgument value;
};

class CScriptingManager
{
private:
	static CScriptingManager     * m_pInstance;
	std::list<CSquirrel *>         m_scripts;
	std::list<ScriptingFunction *> m_funcs;
	std::list<SquirrelClassDecl *> m_classes;
	std::list<ScriptingConstant *> m_constants;

public:
	CScriptingManager();
	~CScriptingManager();

	static CScriptingManager * GetInstance() { return m_pInstance; }
	CSquirrel                * Load(String strName, String strPath);
	bool                       Unload(String strName);
	void                       UnloadAll();
	void                       RegisterFunction(String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate);
	void                       RegisterClass(SquirrelClassDecl * pClassDeclaration);
	void                       RegisterConstant(String strConstantName, CSquirrelArgument value);
	void                       RegisterDefaultConstants();
	CSquirrel                * Get(String strName);
	CSquirrel                * Get(SQVM * pVM);
	std::list<CSquirrel *>   * GetScriptList() { return &m_scripts; }
	unsigned int               GetScriptCount() { return m_scripts.size(); }
};
