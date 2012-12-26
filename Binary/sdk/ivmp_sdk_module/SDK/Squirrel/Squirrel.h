/*

This file has been modified to suit IV:MP's module system.
(C) http://www.iv-multiplayer.com/

*/

/*
Copyright (c) 2003-2009 Alberto Demichelis

This software is provided 'as-is', without any 
express or implied warranty. In no event will the 
authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software 
for any purpose, including commercial applications, 
and to alter it and redistribute it freely, subject 
to the following restrictions:

		1. The origin of this software must not be 
		misrepresented; you must not claim that 
		you wrote the original software. If you 
		use this software in a product, an 
		acknowledgment in the product 
		documentation would be appreciated but is 
		not required.

		2. Altered source versions must be plainly 
		marked as such, and must not be 
		misrepresented as being the original 
		software.

		3. This notice may not be removed or 
		altered from any source distribution.

*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQUIRREL_API
#define SQUIRREL_API extern
#endif

#if (defined(_WIN64) || defined(_LP64))
#define _SQ64
#endif

#ifdef _SQ64
#ifdef _MSC_VER
typedef __int64 int;
typedef unsigned __int64 SQUnsignedInteger;
typedef unsigned __int64 SQHash; /*should be the same size of a pointer*/
#else
typedef long int;
typedef unsigned long SQUnsignedInteger;
typedef unsigned long SQHash; /*should be the same size of a pointer*/
#endif
typedef int SQInt32; 
#else
typedef int SQInt32; /*must be 32 bits(also on 64bits processors)*/
typedef unsigned int SQUnsignedInteger;
typedef unsigned int SQHash; /*should be the same size of a pointer*/
#endif

#if defined(SQUSEDOUBLE) && !defined(_SQ64)
#ifdef _MSC_VER
typedef __int64 SQRawObjectVal; //must be 64bits
#else
typedef long SQRawObjectVal; //must be 64bits
#endif
#define SQ_OBJECT_RAWINIT() { _unVal.raw = 0; }
#else
typedef SQUnsignedInteger SQRawObjectVal; //is 32 bits on 32 bits builds and 64 bits otherwise
#define SQ_OBJECT_RAWINIT()
#endif

typedef void* SQUserPointer;
typedef SQUnsignedInteger SQBool;
typedef int SQRESULT;

#define SQTrue	(1)
#define SQFalse	(0)

struct SQVM;
struct SQTable;
struct SQArray;
struct SQString;
struct SQClosure;
struct SQGenerator;
struct SQNativeClosure;
struct SQUserData;
struct SQFunctionProto;
struct SQRefCounted;
struct SQClass;
struct SQInstance;
struct SQDelegable;
struct SQOuter;

#define MAX_CHAR 0xFF

#define SQUIRREL_VERSION	"Squirrel 3.0 alpha 2"
#define SQUIRREL_COPYRIGHT	"Copyright (C) 2003-2009 Alberto Demichelis"
#define SQUIRREL_AUTHOR		"Alberto Demichelis"

#define SQ_VMSTATE_IDLE			0
#define SQ_VMSTATE_RUNNING		1
#define SQ_VMSTATE_SUSPENDED	2

#define SQUIRREL_EOB 0
#define SQ_BYTECODE_STREAM_TAG	0xFAFA

#define SQOBJECT_REF_COUNTED	0x08000000
#define SQOBJECT_NUMERIC		0x04000000
#define SQOBJECT_DELEGABLE		0x02000000
#define SQOBJECT_CANBEFALSE		0x01000000

#define SQ_MATCHTYPEMASKSTRING (-99999)

#define _RT_MASK 0x00FFFFFF
#define _RAW_TYPE(type) (type&_RT_MASK)

#define _RT_NULL			0x00000001
#define _RT_INTEGER			0x00000002
#define _RT_FLOAT			0x00000004
#define _RT_BOOL			0x00000008
#define _RT_STRING			0x00000010
#define _RT_TABLE			0x00000020
#define _RT_ARRAY			0x00000040
#define _RT_USERDATA		0x00000080
#define _RT_CLOSURE			0x00000100
#define _RT_NATIVECLOSURE	0x00000200
#define _RT_GENERATOR		0x00000400
#define _RT_USERPOINTER		0x00000800
#define _RT_THREAD			0x00001000
#define _RT_FUNCPROTO		0x00002000
#define _RT_CLASS			0x00004000
#define _RT_INSTANCE		0x00008000
#define _RT_WEAKREF			0x00010000
#define _RT_OUTER			0x00020000

typedef enum tagSQObjectType{
	OT_NULL =			(_RT_NULL|SQOBJECT_CANBEFALSE),
	OT_INTEGER =		(_RT_INTEGER|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_FLOAT =			(_RT_FLOAT|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_BOOL =			(_RT_BOOL|SQOBJECT_CANBEFALSE),
	OT_STRING =			(_RT_STRING|SQOBJECT_REF_COUNTED),
	OT_TABLE =			(_RT_TABLE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_ARRAY =			(_RT_ARRAY|SQOBJECT_REF_COUNTED),
	OT_USERDATA =		(_RT_USERDATA|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_CLOSURE =		(_RT_CLOSURE|SQOBJECT_REF_COUNTED),
	OT_NATIVECLOSURE =	(_RT_NATIVECLOSURE|SQOBJECT_REF_COUNTED),
	OT_GENERATOR =		(_RT_GENERATOR|SQOBJECT_REF_COUNTED),
	OT_USERPOINTER =	_RT_USERPOINTER,
	OT_THREAD =			(_RT_THREAD|SQOBJECT_REF_COUNTED) ,
	OT_FUNCPROTO =		(_RT_FUNCPROTO|SQOBJECT_REF_COUNTED), //internal usage only
	OT_CLASS =			(_RT_CLASS|SQOBJECT_REF_COUNTED),
	OT_INSTANCE =		(_RT_INSTANCE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_WEAKREF =		(_RT_WEAKREF|SQOBJECT_REF_COUNTED),
	OT_OUTER =			(_RT_OUTER|SQOBJECT_REF_COUNTED) //internal usage only
}SQObjectType;

#define ISREFCOUNTED(t) (t&SQOBJECT_REF_COUNTED)

typedef union tagSQObjectValue
{
	struct SQTable *pTable;
	struct SQArray *pArray;
	struct SQClosure *pClosure;
	struct SQOuter *pOuter;
	struct SQGenerator *pGenerator;
	struct SQNativeClosure *pNativeClosure;
	struct SQString *pString;
	struct SQUserData *pUserData;
	int nInteger;
	float fFloat;
	SQUserPointer pUserPointer;
	struct SQFunctionProto *pFunctionProto;
	struct SQRefCounted *pRefCounted;
	struct SQDelegable *pDelegable;
	struct SQVM *pThread;
	struct SQClass *pClass;
	struct SQInstance *pInstance;
	struct SQWeakRef *pWeakRef;
	SQRawObjectVal raw;
}SQObjectValue;

typedef struct tagSQObject
{
	SQObjectType _type;
	SQObjectValue _unVal;
}SQObject;

typedef struct tagSQStackInfos{
	const char* funcname;
	const char* source;
	int line;
}SQStackInfos;

typedef struct SQVM* HSQUIRRELVM;
typedef SQObject HSQOBJECT;
typedef int (*SQFUNCTION)(HSQUIRRELVM);
typedef int (*SQRELEASEHOOK)(SQUserPointer,int size);
typedef void (*SQCOMPILERERROR)(HSQUIRRELVM,const char * /*desc*/,const char * /*source*/,int /*line*/,int /*column*/);
typedef void (*SQPRINTFUNCTION)(HSQUIRRELVM,const char * ,...);
typedef void (*SQDEBUGHOOK)(HSQUIRRELVM /*v*/, int /*type*/, const char * /*sourcename*/, int /*line*/, const char * /*funcname*/);
typedef int (*SQWRITEFUNC)(SQUserPointer,SQUserPointer,int);
typedef int (*SQREADFUNC)(SQUserPointer,SQUserPointer,int);

typedef int (*SQLEXREADFUNC)(SQUserPointer);

typedef struct tagSQRegFunction{
	const char *name;
	SQFUNCTION f;
	int nparamscheck;
	const char *typemask;
}SQRegFunction;

typedef struct tagSQFunctionInfo {
	SQUserPointer funcid;
	const char *name;
	const char *source;
}SQFunctionInfo;

typedef HSQUIRRELVM (*sq_open_t)(int initialstacksize);
typedef HSQUIRRELVM ( *sq_newthread_t)(HSQUIRRELVM friendvm, int initialstacksize);
typedef void ( *sq_seterrorhandler_t)(HSQUIRRELVM v);
typedef void ( *sq_close_t)(HSQUIRRELVM v);
typedef void ( *sq_setforeignptr_t)(HSQUIRRELVM v,SQUserPointer p);
typedef SQUserPointer ( *sq_getforeignptr_t)(HSQUIRRELVM v);
typedef void ( *sq_setprintfunc_t)(HSQUIRRELVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
typedef SQPRINTFUNCTION ( *sq_getprintfunc_t)(HSQUIRRELVM v);
typedef SQPRINTFUNCTION ( *sq_geterrorfunc_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_suspendvm_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_wakeupvm_t)(HSQUIRRELVM v,SQBool resumedret,SQBool retval,SQBool raiseerror,SQBool throwerror);
typedef int ( *sq_getvmstate_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_compile_t)(HSQUIRRELVM v,SQLEXREADFUNC read,SQUserPointer p,const char *sourcename,SQBool raiseerror);
typedef SQRESULT ( *sq_compilebuffer_t)(HSQUIRRELVM v,const char *s,int size,const char *sourcename,SQBool raiseerror);
typedef void ( *sq_enabledebuginfo_t)(HSQUIRRELVM v, SQBool enable);
typedef void ( *sq_notifyallexceptions_t)(HSQUIRRELVM v, SQBool enable);
typedef void ( *sq_setcompilererrorhandler_t)(HSQUIRRELVM v,SQCOMPILERERROR f);
typedef void ( *sq_push_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_pop_t)(HSQUIRRELVM v,int nelemstopop);
typedef void ( *sq_poptop_t)(HSQUIRRELVM v);
typedef void ( *sq_remove_t)(HSQUIRRELVM v,int idx);
typedef int ( *sq_gettop_t)(HSQUIRRELVM v);
typedef void ( *sq_settop_t)(HSQUIRRELVM v,int newtop);
typedef void ( *sq_reservestack_t)(HSQUIRRELVM v,int nsize);
typedef int ( *sq_cmp_t)(HSQUIRRELVM v);
typedef void ( *sq_move_t)(HSQUIRRELVM dest,HSQUIRRELVM src,int idx);
typedef SQUserPointer ( *sq_newuserdata_t)(HSQUIRRELVM v,SQUnsignedInteger size);
typedef void ( *sq_newtable_t)(HSQUIRRELVM v);
typedef void ( *sq_newarray_t)(HSQUIRRELVM v,int size);
typedef void ( *sq_newclosure_t)(HSQUIRRELVM v,SQFUNCTION func,SQUnsignedInteger nfreevars);
typedef SQRESULT ( *sq_setparamscheck_t)(HSQUIRRELVM v,int nparamscheck,const char *typemask);
typedef SQRESULT ( *sq_bindenv_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_pushstring_t)(HSQUIRRELVM v,const char *s,int len);
typedef void ( *sq_pushfloat_t)(HSQUIRRELVM v,float f);
typedef void ( *sq_pushinteger_t)(HSQUIRRELVM v,int n);
typedef void ( *sq_pushbool_t)(HSQUIRRELVM v,SQBool b);
typedef void ( *sq_pushuserpointer_t)(HSQUIRRELVM v,SQUserPointer p);
typedef void ( *sq_pushnull_t)(HSQUIRRELVM v);
typedef SQObjectType ( *sq_gettype_t)(HSQUIRRELVM v,int idx);
typedef int ( *sq_getsize_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getbase_t)(HSQUIRRELVM v,int idx);
typedef SQBool ( *sq_instanceof_t)(HSQUIRRELVM v);
typedef void ( *sq_tostring_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_tobool_t)(HSQUIRRELVM v, int idx, SQBool *b);
typedef SQRESULT ( *sq_getstring_t)(HSQUIRRELVM v,int idx,const char **c);
typedef SQRESULT ( *sq_getinteger_t)(HSQUIRRELVM v,int idx,int *i);
typedef SQRESULT ( *sq_getfloat_t)(HSQUIRRELVM v,int idx,float *f);
typedef SQRESULT ( *sq_getbool_t)(HSQUIRRELVM v,int idx,SQBool *b);
typedef SQRESULT ( *sq_getthread_t)(HSQUIRRELVM v,int idx,HSQUIRRELVM *thread);
typedef SQRESULT ( *sq_getuserpointer_t)(HSQUIRRELVM v,int idx,SQUserPointer *p);
typedef SQRESULT ( *sq_getuserdata_t)(HSQUIRRELVM v,int idx,SQUserPointer *p,SQUserPointer *typetag);
typedef SQRESULT ( *sq_settypetag_t)(HSQUIRRELVM v,int idx,SQUserPointer typetag);
typedef SQRESULT ( *sq_gettypetag_t)(HSQUIRRELVM v,int idx,SQUserPointer *typetag);
typedef void ( *sq_setreleasehook_t)(HSQUIRRELVM v,int idx,SQRELEASEHOOK hook);
typedef char ( *sq_getscratchpad_t)(HSQUIRRELVM v,int minsize);
typedef SQRESULT ( *sq_getfunctioninfo_t)(HSQUIRRELVM v,int idx,SQFunctionInfo *fi);
typedef SQRESULT ( *sq_getclosureinfo_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger *nparams,SQUnsignedInteger *nfreevars);
typedef SQRESULT ( *sq_setnativeclosurename_t)(HSQUIRRELVM v,int idx,const char *name);
typedef SQRESULT ( *sq_setinstanceup_t)(HSQUIRRELVM v, int idx, SQUserPointer p);
typedef SQRESULT ( *sq_getinstanceup_t)(HSQUIRRELVM v, int idx, SQUserPointer *p,SQUserPointer typetag);
typedef SQRESULT ( *sq_setclassudsize_t)(HSQUIRRELVM v, int idx, int udsize);
typedef SQRESULT ( *sq_newclass_t)(HSQUIRRELVM v,SQBool hasbase);
typedef SQRESULT ( *sq_createinstance_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_setattributes_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getattributes_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getclass_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_weakref_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getdefaultdelegate_t)(HSQUIRRELVM v,SQObjectType t);
typedef void ( *sq_pushroottable_t)(HSQUIRRELVM v);
typedef void ( *sq_pushregistrytable_t)(HSQUIRRELVM v);
typedef void ( *sq_pushconsttable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_setroottable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_setconsttable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_newslot_t)(HSQUIRRELVM v, int idx, SQBool bstatic);
typedef SQRESULT ( *sq_deleteslot_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_set_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_get_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawget_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawset_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawdeleteslot_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_arrayappend_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_arraypop_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_arrayresize_t)(HSQUIRRELVM v,int idx,int newsize);
typedef SQRESULT ( *sq_arrayreverse_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_arrayremove_t)(HSQUIRRELVM v,int idx,int itemidx);
typedef SQRESULT ( *sq_arrayinsert_t)(HSQUIRRELVM v,int idx,int destpos);
typedef SQRESULT ( *sq_setdelegate_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getdelegate_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_clone_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_setfreevariable_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger nval);
typedef SQRESULT ( *sq_next_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getweakrefval_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_clear_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_call_t)(HSQUIRRELVM v,int params,SQBool retval,SQBool raiseerror);
typedef SQRESULT ( *sq_resume_t)(HSQUIRRELVM v,SQBool retval,SQBool raiseerror);
typedef const char *( *sq_getlocal_t)(HSQUIRRELVM v,SQUnsignedInteger level,SQUnsignedInteger idx);
typedef const char *( *sq_getfreevariable_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger nval);
typedef SQRESULT ( *sq_throwerror_t)(HSQUIRRELVM v,const char *err);
typedef void ( *sq_reseterror_t)(HSQUIRRELVM v);
typedef void ( *sq_getlasterror_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_getstackobj_t)(HSQUIRRELVM v,int idx,HSQOBJECT *po);
typedef void ( *sq_pushobject_t)(HSQUIRRELVM v,HSQOBJECT obj);
typedef void ( *sq_addref_t)(HSQUIRRELVM v,HSQOBJECT *po);
typedef SQBool ( *sq_release_t)(HSQUIRRELVM v,HSQOBJECT *po);
typedef void ( *sq_resetobject_t)(HSQOBJECT *po);
typedef const char *( *sq_objtostring_t)(HSQOBJECT *o);
typedef SQBool ( *sq_objtobool_t)(HSQOBJECT *o);
typedef int ( *sq_objtointeger_t)(HSQOBJECT *o);
typedef float ( *sq_objtofloat_t)(HSQOBJECT *o);
typedef SQRESULT ( *sq_getobjtypetag_t)(HSQOBJECT *o,SQUserPointer * typetag);
typedef int ( *sq_collectgarbage_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_writeclosure_t)(HSQUIRRELVM vm,SQWRITEFUNC writef,SQUserPointer up);
typedef SQRESULT ( *sq_readclosure_t)(HSQUIRRELVM vm,SQREADFUNC readf,SQUserPointer up);
typedef void ( *sq_malloc_t)(SQUnsignedInteger size);
typedef void ( *sq_realloc_t)(void* p,SQUnsignedInteger oldsize,SQUnsignedInteger newsize);
typedef void ( *sq_free_t)(void *p,SQUnsignedInteger size);
typedef SQRESULT ( *sq_stackinfos_t)(HSQUIRRELVM v,int level,SQStackInfos *si);
typedef void ( *sq_setdebughook_t)(HSQUIRRELVM v);

/*UTILITY MACRO*/
#define sq_isnumeric(o) ((o)._type&SQOBJECT_NUMERIC)
#define sq_istable(o) ((o)._type==OT_TABLE)
#define sq_isarray(o) ((o)._type==OT_ARRAY)
#define sq_isfunction(o) ((o)._type==OT_FUNCPROTO)
#define sq_isclosure(o) ((o)._type==OT_CLOSURE)
#define sq_isgenerator(o) ((o)._type==OT_GENERATOR)
#define sq_isnativeclosure(o) ((o)._type==OT_NATIVECLOSURE)
#define sq_isstring(o) ((o)._type==OT_STRING)
#define sq_isinteger(o) ((o)._type==OT_INTEGER)
#define sq_isfloat(o) ((o)._type==OT_FLOAT)
#define sq_isuserpointer(o) ((o)._type==OT_USERPOINTER)
#define sq_isuserdata(o) ((o)._type==OT_USERDATA)
#define sq_isthread(o) ((o)._type==OT_THREAD)
#define sq_isnull(o) ((o)._type==OT_NULL)
#define sq_isclass(o) ((o)._type==OT_CLASS)
#define sq_isinstance(o) ((o)._type==OT_INSTANCE)
#define sq_isbool(o) ((o)._type==OT_BOOL)
#define sq_isweakref(o) ((o)._type==OT_WEAKREF)
#define sq_isouter(o) ((o)._type==OT_OUTER)
#define sq_type(o) ((o)._type)

/* deprecated */
#define sq_createslot(v,n) sq_newslot(v,n,SQFalse)

#define SQ_OK (0)
#define SQ_ERROR (-1)

#define SQ_FAILED(res) (res<0)
#define SQ_SUCCEEDED(res) (res>=0)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#define sq_newthread FuncContainer.sqnewthread
#define sq_seterrorhandler FuncContainer.sqseterrorhandler
#define sq_close FuncContainer.sqclose
#define sq_setforeignptr FuncContainer.sqsetforeignptr
#define sq_getforeignptr FuncContainer.sqgetforeignptr
#define sq_setprintfunc FuncContainer.sqsetprintfunc
#define sq_getprintfunc FuncContainer.sqgetprintfunc
#define sq_geterrorfunc FuncContainer.sqgeterrorfunc
#define sq_suspendvm FuncContainer.sqsuspendvm
#define sq_wakeupvm FuncContainer.sqwakeupvm
#define sq_getvmstate FuncContainer.sqgetvmstate
#define sq_compile FuncContainer.sqcompile
#define sq_compilebuffer FuncContainer.sqcompilebuffer
#define sq_enabledebuginfo FuncContainer.sqenabledebuginfo
#define sq_notifyallexceptions FuncContainer.sqnotifyallexceptions
#define sq_setcompilererrorhandler FuncContainer.sqsetcompilererrorhandler
#define sq_push FuncContainer.sqpush
#define sq_pop FuncContainer.sqpop
#define sq_poptop FuncContainer.sqpoptop
#define sq_remove FuncContainer.sqremove
#define sq_gettop FuncContainer.sqgettop
#define sq_settop FuncContainer.sqsettop
#define sq_reservestack FuncContainer.sqreservestack
#define sq_cmp FuncContainer.sqcmp
#define sq_move FuncContainer.sqmove
#define sq_newuserdata FuncContainer.sqnewuserdata
#define sq_newtable FuncContainer.sqnewtable
#define sq_newarray FuncContainer.sqnewarray
#define sq_newclosure FuncContainer.sqnewclosure
#define sq_setparamscheck FuncContainer.sqsetparamscheck
#define sq_bindenv FuncContainer.sqbindenv
#define sq_pushstring FuncContainer.sqpushstring
#define sq_pushfloat FuncContainer.sqpushfloat
#define sq_pushinteger FuncContainer.sqpushinteger
#define sq_pushbool FuncContainer.sqpushbool
#define sq_pushuserpointer FuncContainer.sqpushuserpointer
#define sq_pushnull FuncContainer.sqpushnull
#define sq_gettype FuncContainer.sqgettype
#define sq_getsize FuncContainer.sqgetsize
#define sq_getbase FuncContainer.sqgetbase
#define sq_instanceof FuncContainer.sqinstanceof
#define sq_tostring FuncContainer.sqtostring
#define sq_tobool FuncContainer.sqtobool
#define sq_getstring FuncContainer.sqgetstring
#define sq_getinteger FuncContainer.sqgetinteger
#define sq_getfloat FuncContainer.sqgetfloat
#define sq_getbool FuncContainer.sqgetbool
#define sq_getthread FuncContainer.sqgetthread
#define sq_getuserpointer FuncContainer.sqgetuserpointer
#define sq_getuserdata FuncContainer.sqgetuserdata
#define sq_settypetag FuncContainer.sqsettypetag
#define sq_gettypetag FuncContainer.sqgettypetag
#define sq_setreleasehook FuncContainer.sqsetreleasehook
#define sq_getscratchpad FuncContainer.sqgetscratchpad
#define sq_getfunctioninfo FuncContainer.sqgetfunctioninfo
#define sq_getclosureinfo FuncContainer.sqgetclosureinfo
#define sq_setnativeclosurename FuncContainer.sqsetnativeclosurename
#define sq_setinstanceup FuncContainer.sqsetinstanceup
#define sq_getinstanceup FuncContainer.sqgetinstanceup
#define sq_setclassudsize FuncContainer.sqsetclassudsize
#define sq_newclass FuncContainer.sqnewclass
#define sq_createinstance FuncContainer.sqcreateinstance
#define sq_setattributes FuncContainer.sqsetattributes
#define sq_getattributes FuncContainer.sqgetattributes
#define sq_getclass FuncContainer.sqgetclass
#define sq_weakref FuncContainer.sqweakref
#define sq_getdefaultdelegate FuncContainer.sqgetdefaultdelegate
#define sq_pushroottable FuncContainer.sqpushroottable
#define sq_pushregistrytable FuncContainer.sqpushregistrytable
#define sq_pushconsttable FuncContainer.sqpushconsttable
#define sq_setroottable FuncContainer.sqsetroottable
#define sq_setconsttable FuncContainer.sqsetconsttable
#define sq_newslot FuncContainer.sqnewslot
#define sq_deleteslot FuncContainer.sqdeleteslot
#define sq_set FuncContainer.sqset
#define sq_get FuncContainer.sqget
#define sq_rawget FuncContainer.sqrawget
#define sq_rawset FuncContainer.sqrawset
#define sq_rawdeleteslot FuncContainer.sqrawdeleteslot
#define sq_arrayappend FuncContainer.sqarrayappend
#define sq_arraypop FuncContainer.sqarraypop
#define sq_arrayresize FuncContainer.sqarrayresize
#define sq_arrayreverse FuncContainer.sqarrayreverse
#define sq_arrayremove FuncContainer.sqarrayremove
#define sq_arrayinsert FuncContainer.sqarrayinsert
#define sq_setdelegate FuncContainer.sqsetdelegate
#define sq_getdelegate FuncContainer.sqgetdelegate
#define sq_clone FuncContainer.sqclone
#define sq_setfreevariable FuncContainer.sqsetfreevariable
#define sq_next FuncContainer.sqnext
#define sq_getweakrefval FuncContainer.sqgetweakrefval
#define sq_clear FuncContainer.sqclear
#define sq_call FuncContainer.sqcall
#define sq_resume FuncContainer.sqresume
#define sq_getlocal FuncContainer.sqgetlocal
#define sq_getfreevariable FuncContainer.sqgetfreevariable
#define sq_throwerror FuncContainer.sqthrowerror
#define sq_reseterror FuncContainer.sqreseterror
#define sq_getlasterror FuncContainer.sqgetlasterror
#define sq_getstackobj FuncContainer.sqgetstackobj
#define sq_pushobject FuncContainer.sqpushobject
#define sq_addref FuncContainer.sqaddref
#define sq_release FuncContainer.sqrelease
#define sq_resetobject FuncContainer.sqresetobject
#define sq_objtostring FuncContainer.sqobjtostring
#define sq_objtobool FuncContainer.sqobjtobool
#define sq_objtointeger FuncContainer.sqobjtointeger
#define sq_objtofloat FuncContainer.sqobjtofloat
#define sq_getobjtypetag FuncContainer.sqgetobjtypetag
#define sq_collectgarbage FuncContainer.sqcollectgarbage
#define sq_writeclosure FuncContainer.sqwriteclosure
#define sq_readclosure FuncContainer.sqreadclosure
#define sq_malloc FuncContainer.sqmalloc
#define sq_realloc FuncContainer.sqrealloc
#define sq_free FuncContainer.sqfree
#define sq_stackinfos FuncContainer.sqstackinfos
#define sq_setdebughook FuncContainer.sqsetdebughook
#define sq_open FuncContainer.sqopen
#define sq_close FuncContainer.sqclose
#define LogPrintf FuncContainer.Printf