//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptingManager.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: UnregisterConstant(constantname)
// TODO: Check if the constant already exists in RegisterConstant
// TODO: Free allocated constant memory

#include "CScriptingManager.h"
#include "../CEvents.h"
#include "../CLogFile.h"
#include <Common.h>

// FIXUPDATE
// jenksta: HACKY!!!
#ifdef _SERVER
#include "../../Server/Core/CModuleManager.h"
extern CModuleManager * g_pModuleManager;
#endif

CScriptingManager * CScriptingManager::m_pInstance = NULL;

#if 0
#include <SharedUtility.h>
class CTestClass
{
private:
	unsigned long m_ulTime;

public:
	CTestClass()
	{
		m_ulTime = SharedUtility::GetTime();
		CLogFile::Printf("CTestClass instance time is %d", m_ulTime);
	}

	unsigned long GetTime()
	{
		CLogFile::Printf("CTestClass instance GetTime returning %d", m_ulTime);
		return m_ulTime;
	}
};

class CInheritedTestClass : public CTestClass
{
public:
	CInheritedTestClass()
	{
		CLogFile::Printf("CInheritedTestClass instance");
	}
};

_MEMBER_FUNCTION_RELEASE_HOOK(testClass)
{
	CTestClass * pTestClass = (CTestClass *)pInst;
	CLogFile::Printf("testClass release hook with instance 0x%x", pTestClass);
	delete pTestClass;
	return 1;
}

_MEMBER_FUNCTION_IMPL(testClass, constructor)
{
	CTestClass * pTestClass = new CTestClass();
	CLogFile::Printf("testClass constructor with instance 0x%x", pTestClass);

	if(SQ_FAILED(sq_setinstance(pVM, pTestClass)))
	{
		CLogFile::Print("Failed to set the instance.");
		SAFE_DELETE(pTestClass);
		sq_pushbool(pVM, false);
		return 1;
	}

	_SET_RELEASE_HOOK(testClass);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(testClass, test)
{
	CTestClass * pTestClass = sq_getinstance<CTestClass *>(pVM);

	if(!pTestClass)
	{
		CLogFile::Print("Failed to get the instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	pTestClass->GetTime();
	CLogFile::Printf("testClass tes with instance 0x%x", pTestClass);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_RELEASE_HOOK(inheritedTestClass)
{
	CInheritedTestClass * pInheritedTestClass = (CInheritedTestClass *)pInst;
	CLogFile::Printf("inheritedTestClass release hook with instance 0x%x", pInheritedTestClass);
	delete pInheritedTestClass;
	return 1;
}

_MEMBER_FUNCTION_IMPL(inheritedTestClass, constructor)
{
	CInheritedTestClass * pInheritedTestClass = new CInheritedTestClass();
	CLogFile::Printf("inheritedTestClass constructor with instance 0x%x", pInheritedTestClass);

	if(SQ_FAILED(sq_setinstance(pVM, pInheritedTestClass)))
	{
		CLogFile::Print("Failed to set the instance.");
		SAFE_DELETE(pInheritedTestClass);
		sq_pushbool(pVM, false);
		return 1;
	}

	_SET_RELEASE_HOOK(inheritedTestClass);
	sq_pushbool(pVM, true);
	return 1;
}

_BEGIN_CLASS(testClass)
_MEMBER_FUNCTION(testClass, constructor, 0, NULL)
_MEMBER_FUNCTION(testClass, test, 0, NULL)
_END_CLASS(testClass)

_BEGIN_CLASS(inheritedTestClass)
_MEMBER_FUNCTION(inheritedTestClass, constructor, 0, NULL)
_END_CLASS_BASE(inheritedTestClass, testClass)

#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqtable.h>
#include <Squirrel/sqclass.h>
#include <map>
std::map<SQVM *, SQInstance *> inst;

SQInteger sq_getTest(SQVM * pVM)
{
	SQObject obj;
	obj._type = OT_INSTANCE;
	obj._unVal.pInstance = inst[pVM];
	sq_pushobject(pVM, obj);
	return 1;
}

// Begin new squirrel
#undef Yield
#include <Squirrel/squirrel.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqobject.h>
#include <Squirrel/sqfuncproto.h>
#include <Squirrel/sqclosure.h>
#include <Squirrel/sqstdaux.h>
#include <Squirrel/sqstdsystem.h>
#include <Squirrel/sqstdio.h>
#include <Squirrel/sqstdstring.h>
#include <Squirrel/sqstdmath.h>
#include <Squirrel/sqstdblob.h>

#include <map>
#include <vector>

class CNewSquirrel;

typedef CSquirrelArgument (* SquirrelFunction_t)(CNewSquirrel * pSquirrel, CSquirrelArguments * pArguments);

struct SquirrelParameterCheck
{
	int                       iMinimumParameters; // min amount of params (min 0)
	int                       iMaximumParameters; // max amount of params (-1 for unlimited)
	std::vector<SQObjectType> typeList;           // parameter types
};

class CNewSquirrel
{
private:
	String                                                 m_strName;
	String                                                 m_strPath;
	SQVM                                                 * m_pVM;
	std::map<String, SquirrelFunction_t>                   m_functionMap;
	std::map<SquirrelFunction_t, SquirrelParameterCheck *> m_parameterMap;

	static void PrintFunction(SQVM * pVM, const char * szFormat, ...)
	{
		va_list args;
		char szBuffer[512];
		va_start(args, szFormat);
		vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
		va_end(args);
		CLogFile::Print(szBuffer);
	}

	static void CompileErrorFunction(SQVM * pVM, const char * szDescription, const char * szSource, int iLine, int iColumn)
	{
		CLogFile::Printf("Failed to compile %s: %s (Line %d, Column %d)", szSource, szDescription, iLine, iColumn);
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

	static SQInteger PrintErrorFunction(SQVM * pVM)
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
				if(si.funcname) fn=si.funcname;
				if(si.source) src=si.source;
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
				CLogFile::Printf("<%s (%d)>", strName.Get(), arg.GetType());
			}
			CLogFile::Printf("</Locals>");

			CLogFile::Printf("</Error>");

			/*
			switch(sq_gettype(v,-1))
			{
			case OT_NULL:
			pf(v,_SC("[%s] NULL\n"),name);
			break;
			case OT_INTEGER:
			sq_getinteger(v,-1,&i);
			pf(v,_SC("[%s] %d\n"),name,i);
			break;
			case OT_FLOAT:
			sq_getfloat(v,-1,&f);
			pf(v,_SC("[%s] %.14g\n"),name,f);
			break;
			case OT_USERPOINTER:
			pf(v,_SC("[%s] USERPOINTER\n"),name);
			break;
			case OT_STRING:
			sq_getstring(v,-1,&s);
			pf(v,_SC("[%s] \"%s\"\n"),name,s);
			break;
			case OT_TABLE:
			pf(v,_SC("[%s] TABLE\n"),name);
			break;
			case OT_ARRAY:
			pf(v,_SC("[%s] ARRAY\n"),name);
			break;
			case OT_CLOSURE:
			pf(v,_SC("[%s] CLOSURE\n"),name);
			break;
			case OT_NATIVECLOSURE:
			pf(v,_SC("[%s] NATIVECLOSURE\n"),name);
			break;
			case OT_GENERATOR:
			pf(v,_SC("[%s] GENERATOR\n"),name);
			break;
			case OT_USERDATA:
			pf(v,_SC("[%s] USERDATA\n"),name);
			break;
			case OT_THREAD:
			pf(v,_SC("[%s] THREAD\n"),name);
			break;
			case OT_CLASS:
			pf(v,_SC("[%s] CLASS\n"),name);
			break;
			case OT_INSTANCE:
			pf(v,_SC("[%s] INSTANCE\n"),name);
			break;
			case OT_WEAKREF:
			pf(v,_SC("[%s] WEAKREF\n"),name);
			break;
			case OT_BOOL:{
			sq_getinteger(v,-1,&i);
			pf(v,_SC("[%s] %s\n"),name,i?_SC("true"):_SC("false"));
			}
			break;
			default: assert(0); break;
			}
			sq_pop(v,1);
			}
			*/
		}

		return 0;
	}

	static SQInteger DefaultFunction(SQVM * pVM)
	{
		// Get our squirrel instance
		CNewSquirrel * pThis = (CNewSquirrel *)pVM->_userpointer;

		// Is our squirrel instance invalid?
		if(!pThis)
			return 0;

		// Get the current call from the call stack
		SQInteger cssize = pVM->_callsstacksize;
		SQVM::CallInfo &ci = pVM->_callsstack[cssize - 1]; // if not work -2 not -1

		// Is the current call not a native call?
		if(type(ci._closure) != OT_NATIVECLOSURE)
			return 0;

		// Get the current call native closure name
		SQObjectPtr name = ci._closure._unVal.pNativeClosure->_name;

		// Is the name invalid?
		if(name._type != OT_STRING)
			return 0;

		// Create a string instance from the name
		String strName(name._unVal.pString->_val);

		// Find the function from the name
		SquirrelFunction_t pfnFunction = NULL;

		for(std::map<String, SquirrelFunction_t>::iterator iter = pThis->m_functionMap.begin(); iter != pThis->m_functionMap.end(); iter++)
		{
			// Is this the function we are looking for?
			if(iter->first == strName)
			{
				pfnFunction = iter->second;
				break;
			}
		}

		// Did we not find the function?
		if(!pfnFunction)
		{
			CLogFile::Printf("Native function %s does not exist", strName.Get());
			return 0;
		}

		// Get our parameter check
		SquirrelParameterCheck * pParameterCheck = NULL;

		for(std::map<SquirrelFunction_t, SquirrelParameterCheck *>::iterator iter = pThis->m_parameterMap.begin(); iter != pThis->m_parameterMap.end(); iter++)
		{
			// Is this the parameter check we are looking for?
			if(iter->first == pfnFunction)
			{
				pParameterCheck = iter->second;
				break;
			}
		}

		// Did we not find the parameter check?
		if(!pParameterCheck)
		{
			CLogFile::Printf("Native function %s does not have parameter check", strName.Get());
			return 0;
		}

		// Ensure we have enough parameters
		int iTop = (sq_gettop(pVM) - 1);

		if(iTop < pParameterCheck->iMinimumParameters)
		{
			CLogFile::Printf("Call to function %s with not enough parameters (Expected %d+, Got %d)", strName.Get(), pParameterCheck->iMinimumParameters, iTop);
			return 0;
		}

		// Ensure we don't have too many parameters
		if(pParameterCheck->iMaximumParameters != -1 && iTop > pParameterCheck->iMaximumParameters)
		{
			CLogFile::Printf("Call to function %s with too many parameters (Expected %d, Got %d)", strName.Get(), pParameterCheck->iMaximumParameters, iTop);
			return 0;
		}

		// Ensure we don't have any invalid types
		int iParam = 0;

		for(std::vector<SQObjectType>::iterator iter = pParameterCheck->typeList.begin(); iter != pParameterCheck->typeList.end(); iter++)
		{
			if(iParam > iTop)
				break;

			SQObjectType type = sq_gettype(pVM, (iParam + 2));

			if(!(type & *iter))
			{
				CLogFile::Printf("Invalid type for function %s, parameter %d (Expected %d, Got %d)", strName.Get(), iParam, *iter, type);
				return 0;
			}

			CLogFile::Printf("Valid type for param %d, in function %s (%d, %d)", iParam, strName.Get(), type, *iter);
			iParam++;
		}

		// Create the arguments
		CSquirrelArguments arguments(pVM, 2);

		// Call the function
		CSquirrelArgument argument = pfnFunction(pThis, &arguments);

		// Push the return value to the VM
		argument.push(pVM);
		return 1;
	}

public:
	CNewSquirrel()
	{
		// Reset the VM pointer
		m_pVM = NULL;
		Test();
	}

	~CNewSquirrel()
	{
		// Are we started?
		if(IsStarted())
		{
			// Stop
			Stop();
		}
	}

	bool IsStarted() { return (m_pVM != NULL); }

	bool Start(bool bSystemLib = true, bool bIOLib = true)
	{
		// Create the squirrel VM with an initial stack size of 1024 bytes (will adjust stack size as needed)
		m_pVM = sq_open(1024);

		// Did the squirrel VM not create successfully?
		if(!m_pVM)
			return false;

		// Set the squirrel VM user pointer
		m_pVM->_userpointer = (SQUserPointer)this;

		// Register the default error handles
		//sqstd_seterrorhandlers(m_pVM);

		// Set the print and error functions
		sq_setprintfunc(m_pVM, PrintFunction, PrintFunction);

		// Set the compile error handler
		sq_setcompilererrorhandler(m_pVM, CompileErrorFunction);

		// Set our error handler
		sq_newclosure(m_pVM, PrintErrorFunction, 0);
		sq_seterrorhandler(m_pVM);

		// Push the root table onto the stack
		sq_pushroottable(m_pVM);

		// Register the string library
		sqstd_register_stringlib(m_pVM);

		// Register the math library
		sqstd_register_mathlib(m_pVM);

		// Register the blob library
		sqstd_register_bloblib(m_pVM);

		// Register the system library
		if(bSystemLib)
			sqstd_register_systemlib(m_pVM);

		// Register the input/out library
		if(bIOLib)
			sqstd_register_iolib(m_pVM);

		// Pop the root table from the stack
		sq_pop(m_pVM, 1);
		return true;
	}

	bool Stop()
	{
		// Are we started?
		if(IsStarted())
		{
			// Close the squirrel VM
			sq_close(m_pVM);
			m_pVM = NULL;
			return true;
		}

		// Not started
		return false;
	}

	bool DoFile(String strPath)
	{
		// Does the script not exist?
		if(!SharedUtility::Exists(strPath.Get()))
		{
			CLogFile::Printf("Script %s does not exist", strPath.Get());
			return false;
		}

		// Get the name
		m_strName = SharedUtility::FileNameFromPath(strPath);

		// Set the path
		m_strPath = strPath;

		// Push the root table onto the stack
		sq_pushroottable(m_pVM);

		// Compile and load the script
		if(SQ_FAILED(sqstd_dofile(m_pVM, strPath.Get(), SQFalse, SQTrue)))
		{
			CLogFile::Printf("Failed to compile and load script %s", strPath.Get());
			return false;
		}

		// Pop the root table from the stack
		sq_pop(m_pVM, 1);
		return true;
	}

	bool RegisterConstant(String strName, CSquirrelArgument value)
	{
		// Get the value type
		SQObjectType type = value.GetType();

		// Is the value type invalid?
		if(type != OT_INTEGER && type != OT_FLOAT && type != OT_STRING)
			return false;

		// Push the root table onto the stack
		sq_pushroottable(m_pVM);

		// Push the constant name onto the stack
		sq_pushstring(m_pVM, strName.Get(), strName.GetLength());

		// Push the constant value onto the stack
		value.push(m_pVM);

		// Create a new root table slot
		sq_createslot(m_pVM, -3);

		// Pop the root table from the stack
		sq_pop(m_pVM, 1);
		return true;
	}

	// "i"   // expect 1 param (int)
	// "is   // expect 2 param (int, string)
	// "i?s" // expect 1 param (int) (optional string)
	// "i??" // expect 1 param (int) (optional amount of args (for print functions e.t.c.))
	SquirrelParameterCheck * GenerateParameterCheck(String strTemplate)
	{
		CLogFile::Printf("Compiling type mask %s", strTemplate.Get());

		// Create the parameter check
		SquirrelParameterCheck * pParameterCheck = new SquirrelParameterCheck;
		pParameterCheck->iMinimumParameters = 0;
		pParameterCheck->iMaximumParameters = 0;

		// Is the template empty?
		if(strTemplate.IsEmpty())
		{
			CLogFile::Printf("Type mask is empty");
			CLogFile::Printf("Compiled type mask %s (Min %d, Max %d)", strTemplate.Get(), pParameterCheck->iMinimumParameters, pParameterCheck->iMaximumParameters);
			return pParameterCheck;
		}

		// Loop through each char of the template
		bool bOptional = false;

		for(unsigned int i = 0; i < strTemplate.GetLength(); i++)
		{
			bool bFinished = false;
			SQObjectType type = (SQObjectType)-1;

			switch(strTemplate[i])
			{
			case ' ': break; // ignore spaces
			case '.': type = (SQObjectType)0; CLogFile::Printf("Added parameter with type ANY"); break;
			case '?':
				{
					// Do we have a next character?
					if((i + 1) < strTemplate.GetLength())
					{
						// Is the next character a '?'?
						if(strTemplate[i + 1] == '?')
						{
							// All parameters from now on are allowed regardless of type
							bFinished = true;
							CLogFile::Printf("All parameters from now on are allowed regardless of type");
							break;
						}
					}

					// All parameters from now on are optional
					bOptional = true;
					CLogFile::Printf("All parameters from now on are optional");
				}
				break;
			case 'o': type = OT_NULL; CLogFile::Printf("Added parameter with type NULL"); break;
			case 'i': type = OT_INTEGER; CLogFile::Printf("Added parameter with type INTEGER"); break;
			case 'f': type = OT_FLOAT; CLogFile::Printf("Added parameter with type FLOAT"); break;
			case 'b': type = OT_BOOL; CLogFile::Printf("Added parameter with type BOOL"); break;
			case 's': type = OT_STRING; CLogFile::Printf("Added parameter with type STRING"); break;
			case 't': type = OT_TABLE; CLogFile::Printf("Added parameter with type TABLE"); break;
			case 'a': type = OT_ARRAY; CLogFile::Printf("Added parameter with type ARRAY"); break;
			case 'u': type = OT_USERDATA; CLogFile::Printf("Added parameter with type USERDATA"); break;
			case 'c': type = OT_CLOSURE; CLogFile::Printf("Added parameter with type CLOSURE"); break;
			case 'n': type = OT_NATIVECLOSURE; CLogFile::Printf("Added parameter with type NATIVECLOSURE"); break;
			case 'g': type = OT_GENERATOR; CLogFile::Printf("Added parameter with type GENERATOR"); break;
			case 'p': type = OT_USERPOINTER; CLogFile::Printf("Added parameter with type USERPOINTER"); break;
			case 'v': type = OT_THREAD; CLogFile::Printf("Added parameter with type THREAD"); break;
			case 'x': type = OT_INSTANCE; CLogFile::Printf("Added parameter with type INSTANCE"); break;
			case 'y': type = OT_CLASS; CLogFile::Printf("Added parameter with type CLASS"); break;
			case 'r': type = OT_WEAKREF; CLogFile::Printf("Added parameter with type WEAKREF"); break;
			default: assert(0); break;
			}

			// Have we finished early?
			if(bFinished)
			{
				pParameterCheck->iMaximumParameters = -1;
				break;
			}

			// Did we get a parameter?
			if(type != (SQObjectType)-1)
			{
				// Are we not using optional parameters?
				if(!bOptional)
				{
					// Increment the minimum amount of parameters
					pParameterCheck->iMinimumParameters++;
				}

				// Increment the maximum amount of parameters
				pParameterCheck->iMaximumParameters++;

				// Add the current parameter type to the parameter type list
				pParameterCheck->typeList.push_back(type);
			}
		}

		CLogFile::Printf("Compiled type mask %s (Min %d, Max %d)", strTemplate.Get(), pParameterCheck->iMinimumParameters, pParameterCheck->iMaximumParameters);
		return pParameterCheck;
	}

	void Test()
	{
		GenerateParameterCheck(NULL);
		GenerateParameterCheck("");
		GenerateParameterCheck("s");
		GenerateParameterCheck("sif");
		GenerateParameterCheck("sififififififififififi");
		GenerateParameterCheck("??");
		GenerateParameterCheck("sf??f");
		GenerateParameterCheck("s?i");
		GenerateParameterCheck("s?sss");
		GenerateParameterCheck("is?f");
		GenerateParameterCheck("is?.");
	}

	bool RegisterFunction(String strName, SquirrelFunction_t pfnFunction, String strParameterTemplate, bool bPushRootTable = true)
	{
		// Does the function already exist?
		if(m_functionMap[strName] != NULL)
			return false;

		// Generate our parameter check
		SquirrelParameterCheck * pParameterCheck = GenerateParameterCheck(strParameterTemplate);

		if(!pParameterCheck)
			return false;

		// Push the root table onto the stack if needed
		if(bPushRootTable)
			sq_pushroottable(m_pVM);

		// Push the function name onto the stack
		sq_pushstring(m_pVM, strName.Get(), strName.GetLength());

		// Create a new function
		sq_newclosure(m_pVM, DefaultFunction, 0);

		// Set the function name
		sq_setnativeclosurename(m_pVM, -1, strName.Get());

		// Create a new slot
		sq_createslot(m_pVM, -3);

		// Add the function to the function map
		m_functionMap[strName] = pfnFunction;

		// Add the parameter check to the parameter map
		m_parameterMap[pfnFunction] = pParameterCheck;

		// Pop the root table from the stack if needed
		if(bPushRootTable)
			sq_pop(m_pVM, 1);

		// Success
		return true;
	}
};

class CNewSquirrelClass
{
private:
	String      m_strName;
	String      m_strBaseClassName;
	std::vector m_functions;

	static CSquirrelArgument TypeOf(CNewSquirrel * pSquirrel, CSquirrelArguments * pArguments)
	{
		// Get the class instance
		SQInstance * pInstance = pArguments->pop().GetInstance();

		// Is the class instance valid?
		if(pInstance)
		{
			// Get our squirrel class instance
			CNewSquirrelClass * pThis = (CNewSquirrelClass *)pInstance->_class->_userpointer;

			// Is our squirrel class instance valid?
			if(pThis)
				return CSquirrelArgument(pThis->m_strName);
		}

		// Invalid class instance
		return CSquirrelArgument("unknown");
	}

public:
	CNewSquirrelClass(String strName, String strBaseClassName)
	{
		// Set the name
		m_strName = strName;

		// Set the base class name
		m_strBaseClassName = strBaseClassName;

		// Register the default typeof method
	}

	void Register(CNewSquirrel * pSquirrel)
	{
		// Get the squirrel VM
		//SQVM * pVM = pSquirrel->
	}
};
// End new squirrel

bool bFirstLoad = true;

CSquirrelArgument TestNative(CNewSquirrel * pSquirrel, CSquirrelArguments * pArguments)
{
	CLogFile::Printf("TestNative(%s)", pArguments->pop().GetString());
	return CSquirrelArgument("Hello World");
}
#endif

CScriptingManager::CScriptingManager()
{
	// Set our instance
	m_pInstance = this;
}

CScriptingManager::~CScriptingManager()
{
	// Reset our instance
	m_pInstance = NULL;
}

CSquirrel * CScriptingManager::Load(String strName, String strPath)
{
#if 0
	if(bFirstLoad)
	{
		CLogFile::Printf("Loading new style squirrel");
		CLogFile::Printf("Step 1");
		CNewSquirrel * pNewSquirrel = new CNewSquirrel();
		CLogFile::Printf("Step 2");
		pNewSquirrel->Start();
		CLogFile::Printf("Step 3");
		pNewSquirrel->RegisterFunction("testNative", TestNative, "?s");
		CLogFile::Printf("Step 33");
		pNewSquirrel->DoFile("C:/test.nut");
		CLogFile::Printf("Step 4");
		pNewSquirrel->Stop();
		CLogFile::Printf("Step 5");
		delete pNewSquirrel;
		CLogFile::Printf("Step 6");
		CLogFile::Printf("Loaded new style squirrel");
		bFirstLoad = false;
		getc(stdin);
	}
#endif
	CSquirrel * pScript = new CSquirrel();

	if(!pScript->Load(strName, strPath))
	{
		delete pScript;
		return NULL;
	}

	m_scripts.push_back(pScript);

	if(m_funcs.size() > 0)
	{
		std::list<ScriptingFunction *>::iterator iter;

		for(iter = m_funcs.begin(); iter != m_funcs.end(); iter++)
			pScript->RegisterFunction((*iter)->strName, (*iter)->pfnFunction, (*iter)->iParameterCount, 
			(*iter)->strTemplate);
	}

#if 0
	pScript->RegisterClass(&_CLASS_DECL(testClass));
	pScript->RegisterClass(&_CLASS_DECL(inheritedTestClass));

	// Get the script VM
	SQVM * pVM = pScript->GetVM();

	// Push the base class name onto the stack
	sq_pushstring(pVM, "testClass", -1);

	// Attempt to get the class
	if(SQ_FAILED(sq_get(pVM, -2)))
	{
		// Failed to get the class
		sq_pop(pVM, 1);
		CLogFile::Printf("FAILED TO FIND 'testClass' CLASS");
		return false;
	}

	// Did we find the class?
	if(sq_gettype(pVM, -1) == OT_CLASS)
	{
		CLogFile::Printf("FOUND CLASS");
		SQObject obj(stack_get(pVM, -1));
		SQClass * pClass = _class(obj);
		CLogFile::Printf("GOT CLASS 0x%x", pClass);
		pClass->_hook = __testClass_releasehook;
		inst[pVM] = pClass->CreateInstance();
		inst[pVM]->_userpointer = new CTestClass();
		inst[pVM]->_hook = __testClass_releasehook;
		CLogFile::Printf("Created class instance 0x%x with class 0x%x", inst[pVM]->_userpointer, inst[pVM]->_class);
	}

	// Pop the class from the stack
	sq_pop(pVM, 1);

	if(inst[pVM])
	{
		pScript->RegisterFunction("getTest", sq_getTest, 0, NULL);
		CLogFile::Printf("OK DONE");
	}
#endif

	if(m_classes.size() > 0)
	{
		std::list<SquirrelClassDecl *>::iterator iter;

		for(iter = m_classes.begin(); iter != m_classes.end(); iter++)
			pScript->RegisterClass(*iter);
	}

	if(m_constants.size() > 0)
	{
		std::list<ScriptingConstant *>::iterator iter;

		for(iter = m_constants.begin(); iter != m_constants.end(); iter++)
			pScript->RegisterConstant((*iter)->strName, (*iter)->value);
	}

#ifdef _SERVER
	if(g_pModuleManager)
		g_pModuleManager->ScriptLoad(pScript->GetVM());
#endif

	if(!pScript->Execute())
	{
		delete pScript;
		m_scripts.remove(pScript);
		CLogFile::Printf("Failed to execute script %s", strName.Get());
		return NULL;
	}

	CEvents * pEvents = CEvents::GetInstance();
	pEvents->Call("scriptInit", pScript);

	CSquirrelArguments arguments;
	arguments.push(strName);
	pEvents->Call("scriptLoad", &arguments);
	return pScript;
}

bool CScriptingManager::Unload(CSquirrel * pScript)
{
	if(pScript)
	{
		CEvents * pEvents = CEvents::GetInstance();
		pEvents->Call("scriptExit", pScript);

		CSquirrelArguments pArguments;
		pArguments.push(pScript->GetName());
		pEvents->Call("scriptUnload", &pArguments);
		pEvents->RemoveScript(pScript->GetVM());

#ifdef _SERVER
		if(g_pModuleManager)
			g_pModuleManager->ScriptUnload(pScript->GetVM());
#endif

		pScript->Unload();
		m_scripts.remove(pScript);
		delete pScript;
		return true;
	}

	return false;
}

bool CScriptingManager::Unload(String strName)
{
	CSquirrel * pScript = Get(strName);

	if(pScript)
		return Unload(pScript);

	return false;
}

void CScriptingManager::UnloadAll()
{
	if(m_scripts.size() > 0)
	{
		for(std::list<CSquirrel *>::iterator iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
		{
			Unload((*iter)->GetName());
			iter = m_scripts.begin();
		}
	}

	m_scripts.clear();
}

void CScriptingManager::RegisterFunction(String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate)
{
	ScriptingFunction * pFunction = new ScriptingFunction;
	pFunction->strName = strFunctionName;
	pFunction->pfnFunction = pfnFunction;
	pFunction->iParameterCount = iParameterCount;
	pFunction->strTemplate = strFunctionTemplate;
	m_funcs.push_back(pFunction);

	if(m_scripts.size() > 0)
	{
		std::list<CSquirrel *>::iterator iter;

		for(iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
			(*iter)->RegisterFunction(strFunctionName, pfnFunction, iParameterCount, strFunctionTemplate);
	}
}

void CScriptingManager::RegisterClass(SquirrelClassDecl * pClassDeclaration)
{
	m_classes.push_back(pClassDeclaration);

	if(m_scripts.size() > 0)
	{
		std::list<CSquirrel *>::iterator iter;

		for(iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
			(*iter)->RegisterClass(pClassDeclaration);
	}
}

void CScriptingManager::RegisterConstant(String strConstantName, CSquirrelArgument value)
{
	ScriptingConstant * pConstant = new ScriptingConstant;
	pConstant->strName = strConstantName;
	pConstant->value = value;
	m_constants.push_back(pConstant);

	if(m_scripts.size() > 0)
	{
		std::list<CSquirrel *>::iterator iter;

		for(iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
			(*iter)->RegisterConstant(strConstantName, value);
	}
}

void CScriptingManager::RegisterDefaultConstants()
{
	RegisterConstant("MAX_PLAYERS", MAX_PLAYERS);
	RegisterConstant("MAX_VEHICLES", MAX_VEHICLES);
	RegisterConstant("MAX_OBJECTS", MAX_OBJECTS);
	RegisterConstant("MAX_CHECKPOINTS", MAX_CHECKPOINTS);
	RegisterConstant("MAX_BLIPS", MAX_BLIPS);
	RegisterConstant("MAX_ACTORS", MAX_ACTORS);
	RegisterConstant("MAX_NAME_LENGTH", MAX_NAME_LENGTH);
	// TODO: Merge all into INVALID_ENTITY_ID or leave as is?
	RegisterConstant("INVALID_PLAYER_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_VEHICLE_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_OBJECT_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_ACTOR_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_CHECKPOINT_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_BLIP_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_PICKUP_ID", INVALID_ENTITY_ID);
	RegisterConstant("IVMP_VERSION", MOD_VERSION_STRING);
}

CSquirrel * CScriptingManager::Get(String strName)
{
	if(m_scripts.size() > 0)
	{
		std::list<CSquirrel *>::iterator iter;

		for(iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
		{
			if((*iter)->GetName() == strName)
				return (*iter);
		}
	}

	return NULL;
}

CSquirrel * CScriptingManager::Get(SQVM * pVM)
{
	if(m_scripts.size() > 0)
	{
		std::list<CSquirrel *>::iterator iter;

		for(iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
		{
			if((*iter)->GetVM() == pVM)
				return (*iter);
		}
	}
	return NULL;
}
