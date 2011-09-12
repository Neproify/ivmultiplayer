//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSquirrel.h
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <assert.h>
#include <stdlib.h>
#include <Squirrel/squirrel.h>
#include <Squirrel/sqobject.h>
#include "CSquirrelArguments.h"

#if defined(WIN32) && defined(RegisterClass)
#undef RegisterClass
#endif

struct ScriptClassMemberDecl 
{
	char * szFunctionName;
	SQFUNCTION sqFunc;
	int iParameterCount;
	char * szFunctionTemplate;
};

struct SquirrelClassDecl
{
	char * name;
	char * base;
	ScriptClassMemberDecl * members;
};

struct SquirrelArguments
{
	int iArgumentCount;
	SQObjectPtr * pArguments;
};

class CSquirrel
{
private:
	SQVM * m_pVM;
	String m_strName;
	String m_strPath;

	static void PrintFunction(SQVM * pVM, const char * szFormat, ...);
	static void ErrorFunction(SQVM * pVM, const char * szFormat, ...);
	static void CompilerErrorFunction(SQVM * pVM, const char * szError, const char * szSource, int iLine, int iColumn);

public:
	SQVM *      GetVM() { return m_pVM; }
	String      GetName() { return m_strName; }
	bool        Load(String strName, String strPath);
	bool        Execute();
	void        Unload();
	void        RegisterFunction(String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate);
	bool        RegisterClass(SquirrelClassDecl * pClassDecl);
	void        RegisterConstant(String strConstantName, CSquirrelArgument value);
	void        Call(SQObjectPtr pFunction, CSquirrelArguments * pArguments = NULL, CSquirrelArgument * pReturn = NULL);
};
