//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModule.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Squirrel/squirrel.h"
#include "CLibrary.h"
#include "Interfaces/InterfaceCommon.h"
#include <CLogFile.h>

static void * FunctionContainer[] =
{
	(void *)sq_open,
	(void *)sq_newthread,
	(void *)sq_seterrorhandler,
	(void *)sq_close,
	(void *)sq_setforeignptr,
	(void *)sq_getforeignptr,
	(void *)sq_setprintfunc,
	(void *)sq_getprintfunc,
	(void *)sq_geterrorfunc,
	(void *)sq_suspendvm,
	(void *)sq_wakeupvm,
	(void *)sq_getvmstate,
	(void *)sq_compile,
	(void *)sq_compilebuffer,
	(void *)sq_enabledebuginfo,
	(void *)sq_notifyallexceptions,
	(void *)sq_setcompilererrorhandler,
	(void *)sq_push,
	(void *)sq_pop,
	(void *)sq_poptop,
	(void *)sq_remove,
	(void *)sq_gettop,
	(void *)sq_settop,
	(void *)sq_reservestack,
	(void *)sq_cmp,
	(void *)sq_move,
	(void *)sq_newuserdata,
	(void *)sq_newtable,
	(void *)sq_newarray,
	(void *)sq_newclosure,
	(void *)sq_setparamscheck,
	(void *)sq_bindenv,
	(void *)sq_pushstring,
	(void *)sq_pushfloat,
	(void *)sq_pushinteger,
	(void *)sq_pushbool,
	(void *)sq_pushuserpointer,
	(void *)sq_pushnull,
	(void *)sq_gettype,
	(void *)sq_getsize,
	(void *)sq_getbase,
	(void *)sq_instanceof,
	(void *)sq_tostring,
	(void *)sq_tobool,
	(void *)sq_getstring,
	(void *)sq_getinteger,
	(void *)sq_getfloat,
	(void *)sq_getbool,
	(void *)sq_getthread,
	(void *)sq_getuserpointer,
	(void *)sq_getuserdata,
	(void *)sq_settypetag,
	(void *)sq_gettypetag,
	(void *)sq_setreleasehook,
	(void *)sq_getscratchpad,
	(void *)sq_getfunctioninfo,
	(void *)sq_getclosureinfo,
	(void *)sq_setnativeclosurename,
	(void *)sq_setinstanceup,
	(void *)sq_getinstanceup,
	(void *)sq_setclassudsize,
	(void *)sq_newclass,
	(void *)sq_createinstance,
	(void *)sq_setattributes,
	(void *)sq_getattributes,
	(void *)sq_getclass,
	(void *)sq_weakref,
	(void *)sq_getdefaultdelegate,
	(void *)sq_pushroottable,
	(void *)sq_pushregistrytable,
	(void *)sq_pushconsttable,
	(void *)sq_setroottable,
	(void *)sq_setconsttable,
	(void *)sq_newslot,
	(void *)sq_deleteslot,
	(void *)sq_set,
	(void *)sq_get,
	(void *)sq_rawget,
	(void *)sq_rawset,
	(void *)sq_rawdeleteslot,
	(void *)sq_arrayappend,
	(void *)sq_arraypop,
	(void *)sq_arrayresize,
	(void *)sq_arrayreverse,
	(void *)sq_arrayremove,
	(void *)sq_arrayinsert,
	(void *)sq_setdelegate,
	(void *)sq_getdelegate,
	(void *)sq_clone,
	(void *)sq_setfreevariable,
	(void *)sq_next,
	(void *)sq_getweakrefval,
	(void *)sq_clear,
	(void *)sq_call,
	(void *)sq_resume,
	(void *)sq_getlocal,
	(void *)sq_getfreevariable,
	(void *)sq_throwerror,
	(void *)sq_reseterror,
	(void *)sq_getlasterror,
	(void *)sq_getstackobj,
	(void *)sq_pushobject,
	(void *)sq_addref,
	(void *)sq_release,
	(void *)sq_resetobject,
	(void *)sq_objtostring,
	(void *)sq_objtobool,
	(void *)sq_objtointeger,
	(void *)sq_objtofloat,
	(void *)sq_getobjtypetag,
	(void *)sq_collectgarbage,
	(void *)sq_writeclosure,
	(void *)sq_readclosure,
	(void *)sq_malloc,
	(void *)sq_realloc,
	(void *)sq_free,
	(void *)sq_stackinfos,
	(void *)sq_setdebughook,
	(void *)CLogFile::Printf
};

static void * InterfacesContainer[32];
static void * NewInterfaceContainer[32];

typedef void (* SetupFunctions_t)(void *pContainer);
typedef void (* SetupInterfaces_t)(void *pContainer);
typedef void (* SetupNewInterfaces_t)(void *pContainer);
typedef bool (* InitModule_t)(char *);
typedef void (* ScriptLoad_t)(const char*, HSQUIRRELVM);
typedef void (* ScriptUnload_t)(const char*, HSQUIRRELVM);
typedef void (* Pulse_t)();

struct ModuleFunctions
{
	SetupFunctions_t pfnSetupFunctions;
	SetupInterfaces_t pfnSetupInterfaces;
	SetupNewInterfaces_t pfnSetupNewInterfaces;
	InitModule_t pfnInitModule;
	ScriptLoad_t pfnScriptLoad;
	ScriptUnload_t pfnScriptUnload;
	Pulse_t pfnPulse;
};

class CModule : public CModuleInterface
{
public:
	CModule(const char *szName);
	~CModule();

	bool IsValid();
	void ScriptLoad(const char* szName, HSQUIRRELVM pVM);
	void ScriptUnload(const char* szName, HSQUIRRELVM pVM);
	void Pulse();

private:
	CLibrary * m_pLibrary;
	ModuleFunctions m_ModuleFunctions;
};