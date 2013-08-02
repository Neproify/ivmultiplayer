//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSquirrelArguments.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Squirrel/squirrel.h>
#include <list>
// FIXUPDATE
// jenksta: this is very hacky :/
#ifdef _SERVER
#include "../../Server/Core/Interfaces/CEventsInterface.h"
#endif
#include <Network/CBitStream.h>

class CSquirrelArguments;

class CSquirrelArgument
#ifdef _SERVER
	: public SquirrelArgumentInterface
#endif
{
	SQObjectType type;
public:
	union
	{
		int i;
		bool b;
		float f;
		String * str;
		CSquirrelArguments * pArray;
		SQObject sqObject;
		SQInstance * pInstance;
	} data;

	enum
	{
		NET_SQ_INVALID,
		NET_SQ_NULL,
		NET_SQ_BYTE_POS,
		NET_SQ_BYTE_NEG,
		NET_SQ_SHORT_POS,
		NET_SQ_SHORT_NEG,
		NET_SQ_INT,
		NET_SQ_BOOL_TRUE,
		NET_SQ_BOOL_FALSE,
		NET_SQ_FLOAT,
		NET_SQ_STRING_TINY,
		NET_SQ_STRING_SMALL,
		NET_SQ_STRING,
		NET_SQ_TABLE,
		NET_SQ_ARRAY,
		NET_SQ_MAX
	};

	CSquirrelArgument() { type = OT_NULL; }
	CSquirrelArgument(int i) { type = OT_INTEGER; data.i = i; }
	CSquirrelArgument(bool b) { type = OT_BOOL; data.b = b; }
	CSquirrelArgument(float f) { type = OT_FLOAT; data.f = f; }
	CSquirrelArgument(String str) { type = OT_STRING; data.str = new String(str); }
	CSquirrelArgument(String* str) { type = OT_STRING; data.str = str; }
	CSquirrelArgument(const CSquirrelArguments &array, bool isArray);
	CSquirrelArgument(CSquirrelArguments * pArray, bool isArray) { type = (isArray ? OT_ARRAY : OT_TABLE); data.pArray = pArray; }
	CSquirrelArgument(SQObject o);
	CSquirrelArgument(SQInstance * pInstance) { type = OT_INSTANCE; data.pInstance = pInstance; }
	CSquirrelArgument(CBitStream * pBitStream);
	CSquirrelArgument(const CSquirrelArgument& p);
	~CSquirrelArgument();

	SQObjectType         GetType() const { return type; }
	String               GetTypeString() const;

	void                 reset();

	bool                 push(SQVM * pVM);
	bool                 pushFromStack(SQVM * pVM, int idx);

	void                 serialize(CBitStream * pBitStream);
	void                 deserialize(CBitStream * pBitStream);

	void                 set(const CSquirrelArgument& p);
	void                 SetNull()                 { reset(); type = OT_NULL; }
	void                 SetInteger(int i)         { reset(); type = OT_INTEGER; data.i = i; }
	void                 SetBool   (bool b)        { reset(); type = OT_BOOL; data.b = b; }
	void                 SetFloat  (float f)       { reset(); type = OT_FLOAT; data.f = f; }
	void                 SetString (const char* s) { reset(); type = OT_STRING; data.str = new String(s); }
	void                 SetArray(CSquirrelArguments * pArray) { reset(); type = OT_ARRAY; data.pArray = pArray; }
	void                 SetTable(CSquirrelArguments * pTable) { reset(); type = OT_TABLE; data.pArray = pTable; }
	void                 SetInstance(SQInstance * pInstance) { reset(); type = OT_INSTANCE; data.pInstance = pInstance; }

	int                  GetInteger() const { return (type == OT_INTEGER) ? data.i : 0; }
	bool                 GetBool()    const { return (type == OT_BOOL)    ? data.b : (type == OT_INTEGER ? data.i != 0 : false); }
	float                GetFloat()   const { return (type == OT_FLOAT)   ? data.f : 0.0f; }
	const char         * GetString()  const { return (type == OT_STRING)  ? data.str->Get() : NULL; }
	CSquirrelArguments * GetTable() const { return (type == OT_TABLE) ? data.pArray : NULL; }
	CSquirrelArguments * GetArray() const { return (type == OT_ARRAY) ? data.pArray : NULL; }
	SQInstance         * GetInstance() const { return (type == OT_INSTANCE) ? data.pInstance : NULL; }
};

class CSquirrelArguments : public std::list<CSquirrelArgument *>
#ifdef _SERVER
	, public SquirrelArgumentsInterface
#endif
{
public:
	CSquirrelArguments(){}
	CSquirrelArguments(SQVM * pVM, int idx);
	CSquirrelArguments(CBitStream * pBitStream);
	CSquirrelArguments(const CSquirrelArguments& p);
	~CSquirrelArguments();

	void reset();

	void push_to_vm(SQVM * pVM);

	void push();
	void pushObject(SQObject o);
	void push(int i);
	void push(bool b);
	void push(float f);
	void push(const char* c);
	void push(String str);
	void push(const CSquirrelArgument &arg);
	void push(const CSquirrelArguments &array, bool isArray);
	void push(CSquirrelArguments* pArray, bool isArray);
	void pushVector3(const CVector3 &vec3);
	bool pushFromStack(SQVM* pVM, int idx);

	CSquirrelArgument pop();
	bool popVector3(CVector3 &vec3);

	void serialize(CBitStream * pBitStream);
	void deserialize(CBitStream * pBitStream);

#ifdef _SERVER
	SquirrelArgumentInterface* Get(unsigned int i) const;
	unsigned int GetSize() const { return size(); }

	SquirrelArgumentInterface* Add() { push(); return back(); }
	void Remove() { pop_back(); }
#endif
};

#ifdef _SERVER
class CSquirrelArgumentManager : public SquirrelArgumentManager
{
	SquirrelArgumentInterface *  CreateArgument() const { return new CSquirrelArgument; }
	SquirrelArgumentsInterface * CreateArguments() const { return new CSquirrelArguments; }

	void                         DeleteArgument(SquirrelArgumentInterface* p) const { delete reinterpret_cast<CSquirrelArgument *>(p); }
	void                         DeleteArguments(SquirrelArgumentsInterface* p) const { delete reinterpret_cast<CSquirrelArguments *>(p); }
};
#endif
