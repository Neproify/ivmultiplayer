//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CSquirrelArguments.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CSquirrelArguments.h"
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqstring.h>

CSquirrelArgument::CSquirrelArgument(const CSquirrelArguments &array, bool isArray)
{
	type = (isArray ? OT_ARRAY : OT_TABLE);
	data.pArray = new CSquirrelArguments(array);
}

CSquirrelArgument::CSquirrelArgument(SQObject o)
{
	type = o._type;

	switch(type)
	{
	case OT_INTEGER:
		data.i = o._unVal.nInteger;
		break;
	case OT_BOOL:
		data.b = (o._unVal.nInteger != 0);
		break;
	case OT_FLOAT:
		data.f = o._unVal.fFloat;
		break;
	case OT_STRING:
		data.str = new String(o._unVal.pString->_val);
		break;
	case OT_NATIVECLOSURE:
	case OT_CLOSURE:
		data.sqObject = o;
		break;
	case OT_INSTANCE:
		data.pInstance = o._unVal.pInstance;
		break;
	}
}

CSquirrelArgument::CSquirrelArgument(CBitStream * pBitStream)
{
	deserialize(pBitStream);
}

CSquirrelArgument::CSquirrelArgument(const CSquirrelArgument& p)
{
	set(p);
}

CSquirrelArgument::~CSquirrelArgument()
{
	reset();
}

String CSquirrelArgument::GetTypeString() const
{
	switch(type)
	{
	case OT_NULL:
		return "Null";
		break;
	case OT_INTEGER:
		return "Integer";
		break;
	case OT_BOOL:
		return "Boolean";
		break;
	case OT_FLOAT:
		return "Floating Point";
		break;
	case OT_STRING:
		return "String";
		break;
	case OT_NATIVECLOSURE:
		return "Native Closure";
		break;
	case OT_CLOSURE:
		return "Closure";
		break;
	case OT_INSTANCE:
		return "Instance";
		break;
	}

	return "Unknown";
}

void CSquirrelArgument::reset()
{
	if(type == OT_STRING)
		delete data.str;
	else if(type == OT_ARRAY || type == OT_TABLE)
		delete data.pArray;

	type = OT_NULL;
}

bool CSquirrelArgument::push(SQVM * pVM)
{
	switch(type)
	{
		case OT_NULL:
			sq_pushnull(pVM);
			break;
		case OT_INTEGER:
			sq_pushinteger(pVM, data.i);
			break;
		case OT_BOOL:
			sq_pushbool(pVM, data.b);
			break;
		case OT_FLOAT:
			sq_pushfloat(pVM, data.f);
			break;
		case OT_STRING:
			sq_pushstring(pVM, data.str->Get(), data.str->GetLength());
			break;
		case OT_ARRAY:
			{
				sq_newarray(pVM, 0);

				for(CSquirrelArguments::iterator iter = data.pArray->begin(); iter != data.pArray->end(); iter++)
				{
					(*iter)->push(pVM);
					sq_arrayappend(pVM, -2);
				}

				break;
			}
		case OT_TABLE:
			{
				assert(data.pArray->size() % 2 == 0);
				sq_newtable(pVM);

				for(CSquirrelArguments::iterator iter = data.pArray->begin(); iter != data.pArray->end(); iter++)
				{
					(*iter)->push(pVM);
					++iter;
					(*iter)->push(pVM);
					sq_createslot(pVM, -3);
				}

				break;
			}
			break;
		case OT_CLOSURE:
		case OT_NATIVECLOSURE:
			sq_pushobject(pVM, data.sqObject);
			break;
		case OT_INSTANCE:
			{
				SQObject obj;
				obj._type = OT_INSTANCE;
				obj._unVal.pInstance = data.pInstance;
				sq_pushobject(pVM, obj);
			}
			break;
		default:
			sq_pushnull(pVM); // whatsoever. do not even care. fix it if you dare.
			assert(0);
			return false;
	}

	return true;
}

bool CSquirrelArgument::pushFromStack(SQVM * pVM, int idx)
{
	reset();
	SQObjectPtr obj = stack_get(pVM, idx);
	type = obj._type;

	switch(obj._type)
	{
	case OT_NULL:
		// Nothing needed
		break;
	case OT_INTEGER:
		data.i = obj._unVal.nInteger;
		break;
	case OT_BOOL:
		data.b = (obj._unVal.nInteger != 0);
		break;
	case OT_FLOAT:
		data.f = obj._unVal.fFloat;
		break;
	case OT_STRING:
		data.str = new String(obj._unVal.pString->_val);
		break;
	case OT_TABLE:
		{
			CSquirrelArguments * pArguments = new CSquirrelArguments();
			sq_push(pVM, idx);
			sq_pushnull(pVM);

			while(SQ_SUCCEEDED(sq_next(pVM, -2)))
			{
				if(!pArguments->pushFromStack(pVM, -2) || !pArguments->pushFromStack(pVM, -1))
				{
					sq_pop(pVM, 4);
					delete pArguments;
					return false;
				}

				sq_pop(pVM,2);
			}

			sq_pop(pVM,2);
			SetTable(pArguments);
		}
		break;
	case OT_ARRAY:
		{
			CSquirrelArguments * pArguments = new CSquirrelArguments();
			sq_push(pVM, idx);
			sq_pushnull(pVM);

			while(SQ_SUCCEEDED(sq_next(pVM, -2)))
			{
				if(!pArguments->pushFromStack(pVM, -1))
				{
					sq_pop(pVM, 4);
					delete pArguments;
					return false;
				}

				sq_pop(pVM,2);
			}

			sq_pop(pVM,2);
			SetArray(pArguments);
		}
		break;
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
		data.sqObject = SQObject(obj);
		break;
	default:
		type = OT_NULL;
		return false;
	}

	return true;
}

void CSquirrelArgument::serialize(CBitStream * pBitStream)
{
	switch(type)
	{
		case OT_NULL:
		case OT_CLOSURE:
		case OT_NATIVECLOSURE:
			pBitStream->Write((unsigned char)NET_SQ_NULL);
			break;
		case OT_INTEGER:
			if(data.i >= 0)
			{
				if(data.i <= 0xFF-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)(data.i + NET_SQ_MAX));
				}
				else if(data.i < 0x200-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)NET_SQ_BYTE_POS);
					pBitStream->Write((unsigned char)(data.i - 0x100 + NET_SQ_MAX));
				}
				else if(data.i < 0x10200-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)NET_SQ_SHORT_POS);
					pBitStream->Write((unsigned short)(data.i - 0x10200 + NET_SQ_MAX));
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_INT);
					pBitStream->Write(data.i);
				}
			}
			else
			{
				if(data.i >= -0xFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_BYTE_NEG);
					pBitStream->Write((unsigned char)(-data.i));
				}
				else if(data.i >= -0xFFFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_SHORT_NEG);
					pBitStream->Write((unsigned short)(-data.i));
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_INT);
					pBitStream->Write(data.i);
				}
			}
			break;
		case OT_BOOL:
			pBitStream->Write((unsigned char)(data.b?NET_SQ_BOOL_TRUE:NET_SQ_BOOL_FALSE));
			break;
		case OT_FLOAT:
			pBitStream->Write((unsigned char)NET_SQ_FLOAT);
			pBitStream->Write(data.f);
			break;
		case OT_STRING:
			{
				size_t size = data.str->GetLength();
				if(size <= 0xFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING_TINY);
					pBitStream->Write((unsigned char)size);
				}
				else if(size <= 0xFFFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING_SMALL);
					pBitStream->Write((unsigned short)size);
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING);
					pBitStream->Write(size);
				}

				pBitStream->Write(data.str->C_String(), data.str->GetLength());
			}
			break;
		case OT_ARRAY:
		case OT_TABLE:
			pBitStream->Write((unsigned char)(type==OT_ARRAY?NET_SQ_ARRAY:NET_SQ_TABLE));
			data.pArray->serialize(pBitStream);
			break;
		default:
			assert(0 && "Unknown Data Type to Serialize");
	}
}

void CSquirrelArgument::deserialize(CBitStream * pBitStream)
{
	unsigned char type;
	pBitStream->Read(type);

	if(type >= NET_SQ_MAX)
	{
		this->type = OT_INTEGER;
		data.i = type - NET_SQ_MAX;
	}
	else
	{
		switch(type)
		{
		case NET_SQ_NULL:
			{
				this->type = OT_NULL;
			}
			break;
		case NET_SQ_BYTE_POS:
			{
				this->type = OT_INTEGER;
				unsigned char temp;
				pBitStream->Read(temp);
				data.i = 0x100 - NET_SQ_MAX + temp;
			}
			break;
		case NET_SQ_BYTE_NEG:
			{
				this->type = OT_INTEGER;
				unsigned char temp;
				pBitStream->Read(temp);
				data.i = -(int)temp;
			}
			break;
		case NET_SQ_SHORT_POS:
			{
				this->type = OT_INTEGER;
				unsigned short temp;
				pBitStream->Read(temp);
				data.i = 0x200 - NET_SQ_MAX + temp;
			}
			break;
		case NET_SQ_SHORT_NEG:
			{
				this->type = OT_INTEGER;
				unsigned short temp;
				pBitStream->Read(temp);
				data.i = -(int)temp;
			}
			break;
		case NET_SQ_INT:
			{
				this->type = OT_INTEGER;
				pBitStream->Read(data.i);
			}
			break;
		case NET_SQ_FLOAT:
			{
				this->type = OT_FLOAT;
				pBitStream->Read(data.f);
			}
			break;
		case NET_SQ_BOOL_TRUE:
		case NET_SQ_BOOL_FALSE:
			{
				this->type = OT_BOOL;
				data.b = (type == NET_SQ_BOOL_TRUE);
			}
			break;
		case NET_SQ_STRING_TINY:
		case NET_SQ_STRING_SMALL:
		case NET_SQ_STRING:
			{
				this->type = OT_STRING;
				size_t size = 0;

				if(type == NET_SQ_STRING_TINY)
				{
					unsigned char temp;
					pBitStream->Read(temp);
					size = temp;
				}
				else if(type == NET_SQ_STRING_SMALL)
				{
					unsigned short temp;
					pBitStream->Read(temp);
					size = temp;
				}
				else
					pBitStream->Read(size);

				data.str = new String();
				data.str->Resize(size);
				char * szString = data.str->GetData();
				pBitStream->Read(szString, size);
				data.str->Truncate(size);
			}
			break;
		case NET_SQ_TABLE:
			this->type = OT_TABLE;
			data.pArray = new CSquirrelArguments(pBitStream);
			break;
		case NET_SQ_ARRAY:
			this->type = OT_ARRAY;
			data.pArray = new CSquirrelArguments(pBitStream);
			break;
		default:
			assert( 0 && "Invalid Data Type to Unserialize" );
		}
	}
}

void CSquirrelArgument::set(const CSquirrelArgument& p)
{
	type = p.type;

	switch(type)
	{
	case OT_NULL:
		break;
	case OT_INTEGER:
		data.i = p.data.i;
		break;
	case OT_BOOL:
		data.b = p.data.b;
		break;
	case OT_FLOAT:
		data.f = p.data.f;
		break;
	case OT_STRING:
		data.str = new String(p.data.str->Get());
		break;
	case OT_ARRAY:
	case OT_TABLE:
		data.pArray = new CSquirrelArguments(*p.data.pArray);
		break;
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
		data.sqObject = p.data.sqObject;
		break;
	case OT_INSTANCE:
		data.pInstance = p.data.pInstance;
		break;
	}
}

//==============================================================================

CSquirrelArguments::CSquirrelArguments(SQVM * pVM, int idx)
{
	int top = sq_gettop(pVM);

	for(int i = idx; i <= top; i++)
		pushFromStack(pVM, i);
}

CSquirrelArguments::CSquirrelArguments(CBitStream * pBitStream)
{
	deserialize(pBitStream);
}

CSquirrelArguments::CSquirrelArguments(const CSquirrelArguments& p)
{
	for(CSquirrelArguments::const_iterator iter = p.begin(); iter != p.end(); ++ iter)
		push_back(new CSquirrelArgument(**iter));
}

CSquirrelArguments::~CSquirrelArguments()
{
	reset();
}

void CSquirrelArguments::reset()
{
	for(iterator iter = begin(); iter != end(); iter++)
		delete *iter;

	clear();
}

void CSquirrelArguments::push_to_vm(SQVM* pVM)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
		(*iter)->push(pVM);
}

void CSquirrelArguments::push()
{
	push_back(new CSquirrelArgument());
}

void CSquirrelArguments::pushObject(SQObject o)
{
	push_back(new CSquirrelArgument(o));
}

void CSquirrelArguments::push(int i)
{
	push_back(new CSquirrelArgument(i));
}

void CSquirrelArguments::push(bool b)
{
	push_back(new CSquirrelArgument(b));
}

void CSquirrelArguments::push(float f)
{
	push_back(new CSquirrelArgument(f));
}

void CSquirrelArguments::push(const char* c)
{
	push_back(new CSquirrelArgument(new String(c)));
}

void CSquirrelArguments::push(String str)
{
	push_back(new CSquirrelArgument(new String(str)));
}

void CSquirrelArguments::push(const CSquirrelArgument &arg)
{
	push_back(new CSquirrelArgument(arg));
}

void CSquirrelArguments::push(const CSquirrelArguments &array, bool isArray)
{
	push_back(new CSquirrelArgument(array, isArray));
}

void CSquirrelArguments::push(CSquirrelArguments* pArray, bool isArray)
{
	push_back(new CSquirrelArgument(pArray, isArray));
}

void CSquirrelArguments::pushVector3(const CVector3 &vec3)
{
	push_back(new CSquirrelArgument(vec3.fX));
	push_back(new CSquirrelArgument(vec3.fY));
	push_back(new CSquirrelArgument(vec3.fZ));
}

bool CSquirrelArguments::pushFromStack(SQVM* pVM, int idx)
{
	CSquirrelArgument * arg = new CSquirrelArgument();

	if(arg->pushFromStack(pVM, idx))
	{
		push_back(arg);
		return true;
	}

	delete arg;
	return false;
}

CSquirrelArgument CSquirrelArguments::pop()
{
	// Do we have an argument to pop?
	if(size() > 0)
	{
		// Get an argument from the front
		CSquirrelArgument * pArgument = front();
		pop_front();

		// Create a new instance of the argument
		CSquirrelArgument argument(*pArgument);

		// Delete the argument
		delete pArgument;

		// Return the new argument instance
		return argument;
	}

	// Nothing we can do, return a NULL argument
	return CSquirrelArgument();
}

bool CSquirrelArguments::popVector3(CVector3 &vec3)
{
	// Do we have 3 arguments to pop?
	if(size() >= 3)
	{
		// Get 3 arguments from the front
		CSquirrelArgument * pArguments[3];

		for(int i = 0; i < 3; i++)
		{
			pArguments[i] = front();

			// Ensure this argument is a floating point value
			if(pArguments[i]->GetType() != OT_FLOAT)
				return false;

			pop_front();
		}

		// Set the vector
		vec3.fX = pArguments[0]->GetFloat();
		vec3.fY = pArguments[1]->GetFloat();
		vec3.fZ = pArguments[2]->GetFloat();
		return true;
	}

	// Not enough arguments
	return false;
}

void CSquirrelArguments::serialize(CBitStream * pBitStream)
{
	size_t size = this->size();

	/*
	First byte:
		00-7f: 1 byte size (0-127)
		80-fe: 2 byte size (including this one)
		ff: 4 byte size (following), 5 bytes in total. Assuming you sent over 32k arguments at once or an array with 32k elements or a table with 16k elements, this wastes one byte of bandwith.
	*/
	if(size < 0x80)
		pBitStream->Write((unsigned char)size);
	else if(size < 0x7f80)
		pBitStream->Write((unsigned short)(size+0x7f80));
	else
	{
		pBitStream->Write((unsigned char)0xff);
		pBitStream->Write(size);
	}

	// --

	for(iterator iter = begin(); iter != end(); ++ iter)
		(*iter)->serialize(pBitStream);
}

void CSquirrelArguments::deserialize(CBitStream * pBitStream)
{
	size_t size;
	unsigned char temp;
	pBitStream->Read(temp);

	if(temp < 0x80)
		size = temp;
	else if(temp < 0xFF)
	{
		size = temp << 8;
		pBitStream->Read(temp);
		size += temp;
		size -= 0x7f80;
	}
	else
		pBitStream->Read(size);

	for(size_t i = 0; i < size; ++i)
		push_back(new CSquirrelArgument(pBitStream));
}

#ifdef _SERVER
SquirrelArgumentInterface* CSquirrelArguments::Get(unsigned int i) const
{
	if(i >= size())
		return 0;

	unsigned int j = 0;
	for(const_iterator iter = begin(); iter != end(); ++ iter)
	{
		if(j == i)
			return *iter;
		
		++ j;
	}

	return 0;
}
#endif
