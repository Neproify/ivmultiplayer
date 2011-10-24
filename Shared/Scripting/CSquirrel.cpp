//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSquirrel.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include <stdarg.h>
#include "CScriptingManager.h"
#include <Squirrel/sqstdio.h>
#include <Squirrel/sqstdaux.h>
#include <Squirrel/sqstdblob.h>
#include <Squirrel/sqstdio.h>
#include <Squirrel/sqstdsystem.h>
#include <Squirrel/sqstdmath.h>
#include <Squirrel/sqstdstring.h>
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include "../SharedUtility.h"
#include "../CEvents.h"
#include "../CLogFile.h"
#include "CSquirrel.h"

extern CScriptingManager * g_pScriptingManager;
extern CEvents * g_pEvents;

void CSquirrel::PrintFunction(SQVM * pVM, const char * szFormat, ...)
{
	va_list args;
	char szBuffer[512];
	va_start(args, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	va_end(args);
	CLogFile::Print(szBuffer);
}

void CSquirrel::ErrorFunction(SQVM * pVM, const char * szFormat, ...)
{
	// TODO: Fix for clients
	va_list args;
	char szBuffer[512];
	va_start(args, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	va_end(args);

	char *tmp = (char *)szBuffer;

	size_t offstart = 0, offend = 0;

	size_t len = strlen(tmp);
	for (size_t i = 0; i < len; ++i)
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offstart;
			break;
		default:
			i = len - 1;
			break;
		}
	}

	tmp += offstart;
	len -= offstart;

	for (size_t i = len - 1; i > 0; --i)
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offend;
			break;
		default:
			i = 1;
			break;
		}
	}

	tmp[len - offend] = '\0';

	// TODO: Parse it and change the format of script error (e.g. onScriptError(filename, line, e.t.c.))
	CSquirrel * pScript = g_pScriptingManager->Get(pVM);

	if(pScript)
	{
		CSquirrelArguments pArguments;
		pArguments.push(tmp);

		if(g_pEvents->Call("scriptError", &pArguments, pScript).GetInteger() == 1)
			CLogFile::Print(tmp);
	}
}

void CSquirrel::CompilerErrorFunction(SQVM * pVM, const char * szError, const char * szSource, int iLine, int iColumn)
{
	// Find the script
	CSquirrel * pScript = g_pScriptingManager->Get(pVM);

	if(pScript)
	{
		// Call the 'scriptError' event
		CSquirrelArguments arguments;
		arguments.push(szError);
		arguments.push(szSource);
		arguments.push(iLine);
		arguments.push(iColumn);

		if(g_pEvents->Call("compilerError", &arguments, pScript).GetInteger() == 1)
			CLogFile::Printf("Error: Failed to compile script %s on Line %d Column %d (%s).", pScript->GetName().Get(), iLine, iColumn, szError);
	}
}

bool CSquirrel::Load(String strName, String strPath)
{
	// Check if the script exists
	if(!SharedUtility::Exists(strPath.Get()))
		return false;

	// Set the script name
	m_strName = strName;

	// Set the script path
	m_strPath = strPath;

	// Create a squirrel VM with an initial stack size of 1024 bytes (stack will resize as needed)
	m_pVM = sq_open(1024);

	// Register the default error handlers
	sqstd_seterrorhandlers(m_pVM);

	// Set the print function and error function
	sq_setprintfunc(m_pVM, PrintFunction, ErrorFunction);

	// Set the compiler error function
	sq_setcompilererrorhandler(m_pVM, CompilerErrorFunction);

	// Push the root table onto the stack
	sq_pushroottable(m_pVM);

#ifdef _SERVER
	// Register the system library
	sqstd_register_systemlib(m_pVM);

	// Register the input/output library
	sqstd_register_iolib(m_pVM);
#endif

	// Register the blob library
	sqstd_register_bloblib(m_pVM);

	// Register the math library
	sqstd_register_mathlib(m_pVM);

	// Register the string library
	sqstd_register_stringlib(m_pVM);
	return true;
}

bool CSquirrel::Execute()
{
	// Add the script name constant
	RegisterConstant("SCRIPT_NAME", m_strName);

	// Add the script path constant
	RegisterConstant("SCRIPT_PATH", m_strPath);

	// Load and compile the script
	if(SQ_FAILED(sqstd_dofile(m_pVM, m_strPath.Get(), SQFalse, SQTrue)))
		return false;

	return true;
}

void CSquirrel::Unload()
{
	// Pop the root table from the stack
	sq_pop(m_pVM, 1);

	// Close the squirrel VM
	sq_close(m_pVM);
	m_pVM = NULL;
}

void CSquirrel::RegisterFunction(String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate)
{
	// Push the function name onto the stack
	sq_pushstring(m_pVM, strFunctionName.Get(), -1);

	// Create a new function
	sq_newclosure(m_pVM, pfnFunction, 0);

	// Set the function parameter template and count
	if(iParameterCount != -1)
	{
		String strTypeMask;

		if(strFunctionTemplate.IsNotEmpty())
			strTypeMask.Format(".%s", strFunctionTemplate.Get());

		sq_setparamscheck(m_pVM, (iParameterCount + 1), strTypeMask.Get());
	}

	// Create a new slot
	sq_createslot(m_pVM, -3);
}

bool CSquirrel::RegisterClass(SquirrelClassDecl * pClassDecl)
{
	// Get the stack top
	int oldtop = sq_gettop(m_pVM);

	// Push the class name onto the stack
	sq_pushstring(m_pVM, pClassDecl->name, -1);

	// Do we have a base class?
	if(pClassDecl->base)
	{
		// Push the base class name onto the stack
		sq_pushstring(m_pVM, pClassDecl->base, -1);

		// Attempt to get the base class
		if(SQ_FAILED(sq_get(m_pVM, -3)))
		{
			// Failed to get the base class
			sq_settop(m_pVM, oldtop);
			return false;
		}
	}

	// Create the class
	if(SQ_FAILED(sq_newclass(m_pVM, pClassDecl->base ? 1 : 0)))
	{
		// Failed to create the class, Restore the stack top
		sq_settop(m_pVM, oldtop);
		return false;
	}

	// Register the class members
	const ScriptClassMemberDecl * pMembers = pClassDecl->members;

	for(int x = 0; pMembers[x].szFunctionName; x++)
		RegisterFunction(pMembers[x].szFunctionName, pMembers[x].sqFunc, pMembers[x].iParameterCount, 
			pMembers[x].szFunctionTemplate);

	// Create a new slot
	sq_createslot(m_pVM, -3);
	return true;
}

void CSquirrel::RegisterConstant(String strConstantName, CSquirrelArgument value)
{
	// Push the constant name onto the stack
	sq_pushstring(m_pVM, strConstantName.Get(), -1);

	// Push the constant value onto the stack
	value.push(m_pVM);

	// Create a new slot
	sq_createslot(m_pVM, -3);
}

void CSquirrel::Call(SQObjectPtr pFunction, CSquirrelArguments * pArguments, CSquirrelArgument * pReturn)
{
	// Get the stack top
	int iTop = sq_gettop(m_pVM);

	// Process the parameters if needed
	int iParams = 1;

	if(pArguments)
	{
		pArguments->push_to_vm(m_pVM);
		iParams += pArguments->size();
	}

	// Call the function
	SQObjectPtr res;

	if(m_pVM->Call(pFunction, iParams, m_pVM->_top-iParams, res, true))
	{
		// Set the return value if needed
		if(pReturn)
			pReturn->set(res);
	}

	// Restore the stack top
	sq_settop(m_pVM, iTop);
}
