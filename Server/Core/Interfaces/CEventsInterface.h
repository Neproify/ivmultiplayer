//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CEventsInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
#include <Squirrel/squirrel.h>

class SquirrelArgumentsInterface;

class SquirrelArgumentInterface
{
public:
	/* Returns the argument's type */
	virtual SQObjectType GetType()    const = 0;

	/* Retrieves the argument as either of these types */
	        bool         IsNull()     const { return GetType() == OT_NULL; }
	virtual int          GetInteger() const = 0;
	virtual bool         GetBool()    const = 0;
	virtual float        GetFloat()   const = 0;
	virtual const char * GetString()  const = 0;

	/* Retrieves a list with all array elements */
	//virtual SquirrelArgumentsInterface* GetArray() = 0;

	/* Retrieves a list with all table entries
			t = { a = 3, b = 5 }
		will result in
			t = ...->GetTable()
			t.Get(0).GetString() = a
			t.Get(1).GetInteger() = 3
			t.Get(2).GetString() = b
			t.Get(3).GetInteger() = 5
		etc. - so even indices are the table keys, odd indices are the values
	*/
	//virtual SquirrelArgumentsInterface* GetTable() = 0;

	/* Set various different stuffs */
	virtual void SetNull   ()              = 0;
	virtual void SetInteger(int i)         = 0;
	virtual void SetBool   (bool b)        = 0;
	virtual void SetFloat  (float f)       = 0;
	virtual void SetString (const char* s) = 0;

	//virtual void SetArray  (SquirrelArgumentsInterface* p) = 0;
	//virtual void SetTable  (SquirrelArgumentsInterface* p) = 0;
};

class SquirrelArgumentsInterface
{
public:
	/* Retrieves the i-th parameter or returns 0 if none */
	virtual SquirrelArgumentInterface* Get(unsigned int i) const = 0;

	/* Retrieves the number of arguments */
	virtual unsigned int GetSize() const = 0;

	/* Adds an element at the back (which is null) */
	virtual SquirrelArgumentInterface* Add() = 0;

	/* Removes the last element */
	virtual void Remove() = 0;
};

class SquirrelArgumentManager
{
public:
	/* Creates and deletes argument(s) - should ONLY ever use this on your own argument stuff, e.g. for calling events */
	virtual SquirrelArgumentInterface* CreateArgument() const = 0;
	virtual SquirrelArgumentsInterface* CreateArguments() const = 0;

	virtual void DeleteArgument(SquirrelArgumentInterface* p) const = 0;
	virtual void DeleteArguments(SquirrelArgumentsInterface* p) const = 0;
};

typedef void (* EventHandler_t)(SquirrelArgumentsInterface * pArguments, SquirrelArgumentInterface * pReturn, void * pChunk);

class CEventsInterface
{
public:
	virtual bool AddModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0) = 0;
	virtual bool RemoveModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0) = 0;
	virtual void CallModuleEvent(const char* szName, SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn) = 0;
	virtual const char* GetCurrentEvent() = 0;
};
