//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Definitions.h
//===============================================================================

#pragma once

#include <string.h>

#include "Interfaces/InterfaceCommon.h"
#include "ModuleNatives/Interface/IModuleNatives.h"

using namespace Modules;

#ifndef _LINUX
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

#define MAX_VEHICLES 0xFFFE
#define MAX_OBJECTS 0xFFFE
#define MAX_CHECKPOINTS 0xFFFE
#define MAX_BLIPS 1300
#ifndef MAX_ACTORS
#define MAX_ACTORS 12
#endif
#define MAX_PICKUPS 0xFFFE
#define MAX_NAME 32

// Define used for invalid entity ids
#define INVALID_ENTITY_ID 0xFFFF

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction, int iParameterCount = -1, String strFunctionTemplate = NULL);
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, int value);
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, bool value);
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, const char* value, int len);
void RegisterConstant(HSQUIRRELVM pVM, const char *szName, float value);

typedef void (* LogPrintf_t)(const char * szFormat, ...);
struct InterfaceContainer_t
{
	CNetworkManagerInterface* g_pNetworkManager;
	CPlayerManagerInterface* g_pPlayerManager;
	CVehicleManagerInterface* g_pVehicleManager;
	CObjectManagerInterface* g_pObjectManager;
	CBlipManagerInterface* g_pBlipManager;
	CActorManagerInterface* g_pActorManager;
	CPickupManagerInterface* g_pPickupManager;
	CCheckpointManagerInterface* g_pCheckpointManager;
	void* g_pModelManager;
	void* g_pScriptingManager;
	CModuleManagerInterface* g_pModuleManager;
	void* g_pRakServer;
	void* g_pConfig;
	CTimeInterface* g_pTime;
	CTrafficLightsInterface* g_pTrafficLights;
	CEventsInterface* g_pEvents;
	SquirrelArgumentManager* pSquirrelArgumentManager;
};
struct NewInterfaceContainer_t
{
	ICActorModuleNatives* g_pActorNatives;
	ICBlipModuleNatives* g_pBlipNatives;
	ICCheckpointModuleNatives* g_pCheckpointNatives;
	ICObjectModuleNatives* g_pObjectNatives;
	ICPickupModuleNatives* g_pPickupNatives;
	ICPlayerModuleNatives* g_pPlayerNatives;
	ICServerModuleNatives* g_pServerNatives;
	ICVehicleModuleNatives* g_pVehicleNatives;
	ICScriptModuleNatives* g_pScriptNatives;
	ICAreaModuleNatives* g_pAreaNatives;
	ICHashModuleNatives* g_pHashNatives;
	ICWorldModuleNatives* g_pWorldNatives;
};
struct FuncContainer_t
{
	sq_open_t sqopen;
	sq_newthread_t sqnewthread;
	sq_seterrorhandler_t sqseterrorhandler;
	sq_close_t sqclose;
	sq_setforeignptr_t sqsetforeignptr;
	sq_getforeignptr_t sqgetforeignptr;
	sq_setprintfunc_t sqsetprintfunc;
	sq_getprintfunc_t sqgetprintfunc;
	sq_geterrorfunc_t sqgeterrorfunc;
	sq_suspendvm_t sqsuspendvm;
	sq_wakeupvm_t sqwakeupvm;
	sq_getvmstate_t sqgetvmstate;
	sq_compile_t sqcompile;
	sq_compilebuffer_t sqcompilebuffer;
	sq_enabledebuginfo_t sqenabledebuginfo;
	sq_notifyallexceptions_t sqnotifyallexceptions;
	sq_setcompilererrorhandler_t sqsetcompilererrorhandler;
	sq_push_t sqpush;
	sq_pop_t sqpop;
	sq_poptop_t sqpoptop;
	sq_remove_t sqremove;
	sq_gettop_t sqgettop;
	sq_settop_t sqsettop;
	sq_reservestack_t sqreservestack;
	sq_cmp_t sqcmp;
	sq_move_t sqmove;
	sq_newuserdata_t sqnewuserdata;
	sq_newtable_t sqnewtable;
	sq_newarray_t sqnewarray;
	sq_newclosure_t sqnewclosure;
	sq_setparamscheck_t sqsetparamscheck;
	sq_bindenv_t sqbindenv;
	sq_pushstring_t sqpushstring;
	sq_pushfloat_t sqpushfloat;
	sq_pushinteger_t sqpushinteger;
	sq_pushbool_t sqpushbool;
	sq_pushuserpointer_t sqpushuserpointer;
	sq_pushnull_t sqpushnull;
	sq_gettype_t sqgettype;
	sq_getsize_t sqgetsize;
	sq_getbase_t sqgetbase;
	sq_instanceof_t sqinstanceof;
	sq_tostring_t sqtostring;
	sq_tobool_t sqtobool;
	sq_getstring_t sqgetstring;
	sq_getinteger_t sqgetinteger;
	sq_getfloat_t sqgetfloat;
	sq_getbool_t sqgetbool;
	sq_getthread_t sqgetthread;
	sq_getuserpointer_t sqgetuserpointer;
	sq_getuserdata_t sqgetuserdata;
	sq_settypetag_t sqsettypetag;
	sq_gettypetag_t sqgettypetag;
	sq_setreleasehook_t sqsetreleasehook;
	sq_getscratchpad_t sqgetscratchpad;
	sq_getfunctioninfo_t sqgetfunctioninfo;
	sq_getclosureinfo_t sqgetclosureinfo;
	sq_setnativeclosurename_t sqsetnativeclosurename;
	sq_setinstanceup_t sqsetinstanceup;
	sq_getinstanceup_t sqgetinstanceup;
	sq_setclassudsize_t sqsetclassudsize;
	sq_newclass_t sqnewclass;
	sq_createinstance_t sqcreateinstance;
	sq_setattributes_t sqsetattributes;
	sq_getattributes_t sqgetattributes;
	sq_getclass_t sqgetclass;
	sq_weakref_t sqweakref;
	sq_getdefaultdelegate_t sqgetdefaultdelegate;
	sq_pushroottable_t sqpushroottable;
	sq_pushregistrytable_t sqpushregistrytable;
	sq_pushconsttable_t sqpushconsttable;
	sq_setroottable_t sqsetroottable;
	sq_setconsttable_t sqsetconsttable;
	sq_newslot_t sqnewslot;
	sq_deleteslot_t sqdeleteslot;
	sq_set_t sqset;
	sq_get_t sqget;
	sq_rawget_t sqrawget;
	sq_rawset_t sqrawset;
	sq_rawdeleteslot_t sqrawdeleteslot;
	sq_arrayappend_t sqarrayappend;
	sq_arraypop_t sqarraypop;
	sq_arrayresize_t sqarrayresize;
	sq_arrayreverse_t sqarrayreverse;
	sq_arrayremove_t sqarrayremove;
	sq_arrayinsert_t sqarrayinsert;
	sq_setdelegate_t sqsetdelegate;
	sq_getdelegate_t sqgetdelegate;
	sq_clone_t sqclone;
	sq_setfreevariable_t sqsetfreevariable;
	sq_next_t sqnext;
	sq_getweakrefval_t sqgetweakrefval;
	sq_clear_t sqclear;
	sq_call_t sqcall;
	sq_resume_t sqresume;
	sq_getlocal_t sqgetlocal;
	sq_getfreevariable_t sqgetfreevariable;
	sq_throwerror_t sqthrowerror;
	sq_reseterror_t sqreseterror;
	sq_getlasterror_t sqgetlasterror;
	sq_getstackobj_t sqgetstackobj;
	sq_pushobject_t sqpushobject;
	sq_addref_t sqaddref;
	sq_release_t sqrelease;
	sq_resetobject_t sqresetobject;
	sq_objtostring_t sqobjtostring;
	sq_objtobool_t sqobjtobool;
	sq_objtointeger_t sqobjtointeger;
	sq_objtofloat_t sqobjtofloat;
	sq_getobjtypetag_t sqgetobjtypetag;
	sq_collectgarbage_t sqcollectgarbage;
	sq_writeclosure_t sqwriteclosure;
	sq_readclosure_t sqreadclosure;
	sq_malloc_t sqmalloc;
	sq_realloc_t sqrealloc;
	sq_free_t sqfree;
	sq_stackinfos_t sqstackinfos;
	sq_setdebughook_t sqsetdebughook;
	LogPrintf_t Printf;
};

extern InterfaceContainer_t InterfaceContainer;
extern NewInterfaceContainer_t NewInterfaceContainer;
extern FuncContainer_t FuncContainer;

inline CNetworkManagerInterface * GetNetworkManager()
{
	return InterfaceContainer.g_pNetworkManager;
}

inline CPlayerManagerInterface * GetPlayerManager()
{
	return InterfaceContainer.g_pPlayerManager;
}

inline CVehicleManagerInterface * GetVehicleManager()
{
	return InterfaceContainer.g_pVehicleManager;
}

inline CObjectManagerInterface * GetObjectManager()
{
	return InterfaceContainer.g_pObjectManager;
}

inline CBlipManagerInterface * GetBlipManager()
{
	return InterfaceContainer.g_pBlipManager;
}

inline CActorManagerInterface * GetActorManager()
{
	return InterfaceContainer.g_pActorManager;
}

inline CPickupManagerInterface * GetPickupManager()
{
	return InterfaceContainer.g_pPickupManager;
}

inline CCheckpointManagerInterface * GetCheckpointManager()
{
	return InterfaceContainer.g_pCheckpointManager;
}

inline CModuleManagerInterface * GetModuleManager()
{
	return InterfaceContainer.g_pModuleManager;
}

inline CTimeInterface * GetTimeManager()
{
	return InterfaceContainer.g_pTime;
}

inline CTrafficLightsInterface * GetTrafficLights()
{
	return InterfaceContainer.g_pTrafficLights;
}

inline CEventsInterface * GetEventManager()
{
	return InterfaceContainer.g_pEvents;
}

inline SquirrelArgumentManager * GetSquirrelArgumentManager()
{
	return InterfaceContainer.pSquirrelArgumentManager;
}

namespace IVMP
{
	inline ICActorModuleNatives * Actors()
	{
		return NewInterfaceContainer.g_pActorNatives;
	}
	inline ICBlipModuleNatives * Blips()
	{
		return NewInterfaceContainer.g_pBlipNatives;
	}
	inline ICCheckpointModuleNatives * Checkpoints()
	{
		return NewInterfaceContainer.g_pCheckpointNatives;
	}
	inline ICObjectModuleNatives * Objects()
	{
		return NewInterfaceContainer.g_pObjectNatives;
	}
	inline ICPickupModuleNatives * Pickups()
	{
		return NewInterfaceContainer.g_pPickupNatives;
	}
	inline ICPlayerModuleNatives * Players()
	{
		return NewInterfaceContainer.g_pPlayerNatives;
	}
	inline ICServerModuleNatives * Server()
	{
		return NewInterfaceContainer.g_pServerNatives;
	}
	inline ICVehicleModuleNatives * Vehicles()
	{
		return NewInterfaceContainer.g_pVehicleNatives;
	}
	inline ICScriptModuleNatives * Scripts()
	{
		return NewInterfaceContainer.g_pScriptNatives;
	}
	inline ICAreaModuleNatives * Area()
	{
		return NewInterfaceContainer.g_pAreaNatives;
	}
	inline ICHashModuleNatives * Hash()
	{
		return NewInterfaceContainer.g_pHashNatives;
	}
	inline ICWorldModuleNatives * World()
	{
		return NewInterfaceContainer.g_pWorldNatives;
	}
	namespace Events
	{
		inline CEventsInterface * Manager()
		{
			return InterfaceContainer.g_pEvents;
		}
		inline SquirrelArgumentManager* SquirrelArgumentManager()
		{
			return InterfaceContainer.pSquirrelArgumentManager;
		}
	}
}
