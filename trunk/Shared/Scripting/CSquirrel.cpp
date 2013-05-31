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

void CSquirrel::PrintFunction(SQVM * pVM, const char * szFormat, ...)
{
	va_list args;
	char szBuffer[512];
	va_start(args, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
	va_end(args);
	CLogFile::Print(szBuffer);
}

void CSquirrel::CompilerErrorFunction(SQVM * pVM, const char * szError, const char * szSource, int iLine, int iColumn)
{
	// Find the script
	CSquirrel * pScript = CScriptingManager::GetInstance()->Get(pVM);

	if(pScript)
	{
		// Call the 'compilerError' event
		CSquirrelArguments arguments;
		arguments.push(szError);
		arguments.push(szSource);
		arguments.push(iLine);
		arguments.push(iColumn);

		if(CEvents::GetInstance()->Call("compilerError", &arguments, pScript).GetInteger() == 1)
			CLogFile::Printf("Error: Failed to compile script %s (Error: %s (Line %d, Column %d)).", pScript->GetName().Get(), szError, iLine, iColumn);
	}
}

typedef std::pair<String, int> ErrorSourcePair;
typedef std::pair<String, ErrorSourcePair> ErrorCallstackPair;
typedef std::list<ErrorCallstackPair> ErrorCallstackList;

typedef std::pair<String, CSquirrelArgument> ErrorLocalPair;
typedef std::list<ErrorLocalPair> ErrorLocalsList;

struct ErrorInfo
{
	String strError;
	ErrorCallstackList callstack;
	ErrorLocalsList locals;
};

SQInteger CSquirrel::PrintErrorFunction(SQVM * pVM)
{
	if(sq_gettop(pVM) >= 1)
	{
		const SQChar * szError = NULL;
		sq_getstring(pVM, 2, &szError);
				
		ErrorInfo info;
		info.strError = szError;

		SQStackInfos si;
		SQInteger level = 1; // 1 is to skip this function that is level 0
		const SQChar *name = 0; 
		SQInteger seq = 0;

		while(SQ_SUCCEEDED(sq_stackinfos(pVM, level, &si)))
		{
			const SQChar * fn = _SC("unknown");
			const SQChar * src = _SC("unknown");
			if(si.funcname) fn = si.funcname;
			if(si.source) src = si.source;
			info.callstack.push_back(ErrorCallstackPair(fn, ErrorSourcePair(src, si.line)));
			level++;
		}

		for(level = 0; level < 10; level++)
		{
			seq = 0;

			while((name = sq_getlocal(pVM, level, seq)))
			{
				seq++;
				CSquirrelArgument arg;
				arg.pushFromStack(pVM, -1);
				info.locals.push_back(ErrorLocalPair(name, arg));
				sq_pop(pVM, 1);
			}
		}

		CSquirrel * pScript = CScriptingManager::GetInstance()->Get(pVM);

		if(pScript)
		{
			CSquirrelArguments arguments;
			CSquirrelArguments tempArray;
			CSquirrelArguments callstackTable;
			CSquirrelArguments localsTable;
			arguments.push(info.strError);

			for(ErrorCallstackList::iterator iter = info.callstack.begin(); iter != info.callstack.end(); iter++)
			{
				String strFunction = iter->first;
				String strSource = iter->second.first;
				int iLine = iter->second.second;
				callstackTable.push(strFunction);
				tempArray.reset();
				tempArray.push(strSource);
				tempArray.push(iLine);
				callstackTable.push(tempArray, true);
			}

			arguments.push(callstackTable, false);

			for(ErrorLocalsList::iterator iter = info.locals.begin(); iter != info.locals.end(); iter++)
			{
				String strName = iter->first;
				CSquirrelArgument arg = iter->second;
				localsTable.push(strName);
				localsTable.push(arg);
			}

			arguments.push(localsTable, false);

			if(CEvents::GetInstance()->Call("scriptError", &arguments, pScript).GetInteger() == 1)
			{
				CLogFile::Printf("<Error (%s)>", info.strError.Get());

				CLogFile::Printf("<Callstack>");
				for(ErrorCallstackList::iterator iter = info.callstack.begin(); iter != info.callstack.end(); iter++)
				{
					String strFunction = iter->first;
					String strSource = iter->second.first;
					int iLine = iter->second.second;
					CLogFile::Printf("<%s (%s, %d)>", strFunction.Get(), strSource.Get(), iLine);
				}
				CLogFile::Printf("</Callstack>");

				CLogFile::Printf("<Locals>");
				for(ErrorLocalsList::iterator iter = info.locals.begin(); iter != info.locals.end(); iter++)
				{
					String strName = iter->first;
					CSquirrelArgument arg = iter->second;
					CLogFile::Printf("<%s (%s)>", strName.Get(), arg.GetTypeString().Get());
				}
				CLogFile::Printf("</Locals>");

				CLogFile::Printf("</Error>");
			}
		}
	}

	return 0;
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
	sq_setprintfunc(m_pVM, PrintFunction, PrintFunction);

	// Set the compiler error function
	sq_setcompilererrorhandler(m_pVM, CompilerErrorFunction);

	// Set our error handler
	sq_newclosure(m_pVM, PrintErrorFunction, 0);
	sq_seterrorhandler(m_pVM);

	// Push the root table onto the stack
	sq_pushroottable(m_pVM);

#ifdef _SERVER
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
	// Lock the squirrel call mutex
	m_squirrelCallMutex.Lock();

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

	// Unlock the squirrel call mutex
	m_squirrelCallMutex.Unlock();
}
