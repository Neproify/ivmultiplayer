//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: XMLNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../../CXML.h"
#include "../CScriptingManager.h"
#include <SharedUtility.h>

// XML
_BEGIN_CLASS(xml)
_MEMBER_FUNCTION(xml, constructor, 1, "s")
_MEMBER_FUNCTION(xml, save, 0, NULL)
_MEMBER_FUNCTION(xml, setTabSize, 1, "i")
_MEMBER_FUNCTION(xml, nodeAttribute, 1, "s")
_MEMBER_FUNCTION(xml, nodeRemoveAttribute, 1, "s")
_MEMBER_FUNCTION(xml, nodeSetAttribute, 2, "ss")
_MEMBER_FUNCTION(xml, nodeRoot, 0, NULL)
_MEMBER_FUNCTION(xml, nodeNext, 0, NULL)
_MEMBER_FUNCTION(xml, nodePrevious, 0, NULL)
_MEMBER_FUNCTION(xml, nodeClear, 0, NULL)
_MEMBER_FUNCTION(xml, nodeNew, 2, "bs")
_MEMBER_FUNCTION(xml, nodeFind, 1, "s")
_MEMBER_FUNCTION(xml, nodeFirstChild, 0, NULL)
_MEMBER_FUNCTION(xml, nodeParent, 0, NULL)
_MEMBER_FUNCTION(xml, nodeName, 0, NULL)
_MEMBER_FUNCTION(xml, nodeContent, 0, NULL)
_MEMBER_FUNCTION(xml, nodeSetContent, 1, "s")
_MEMBER_FUNCTION(xml, commentNew, 1, "s")
_MEMBER_FUNCTION(xml, lastError, 0, NULL)
_MEMBER_FUNCTION(xml, isComment, 0, NULL)
_END_CLASS(xml)

void RegisterXMLNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(xml));
}

_MEMBER_FUNCTION_RELEASE_HOOK(xml)
{
	CXML * pXML = (CXML *)pInst;
	delete pXML;
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, constructor)
{
	const char * filename;
	sq_getstring(pVM, -1, &filename);

	if(filename)
	{
		CXML * pXML = new CXML();
		String strFileName(filename);
		SharedUtility::RemoveIllegalCharacters(strFileName);
		String strPath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
		pXML->load(strPath);

		if(!pXML || SQ_FAILED(sq_setinstance(pVM, pXML)))
		{
			CLogFile::Print("Failed to load the xml.");
			SAFE_DELETE(pXML);
			sq_pushbool(pVM, false);
			return 1;
		}

		_SET_RELEASE_HOOK(xml);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, save)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pXML->save());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, setTabSize)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	SQInteger tabSize;
	sq_getinteger(pVM, -1, &tabSize);

	pXML->setTabSize(tabSize);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeAttribute)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	sq_getstring(pVM, -1, &attributeName);
	const char * attribute = pXML->getAttribute(attributeName);
	if(attribute != NULL)
		sq_pushstring(pVM, attribute, strlen(attribute));
	else
		sq_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeRemoveAttribute)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	sq_getstring(pVM, -1, &attributeName);
	pXML->removeAttribute(attributeName);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeSetAttribute)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	const char * attributeContent;
	sq_getstring(pVM, -1, &attributeContent);
	sq_getstring(pVM, -2, &attributeName);

	pXML->setAttribute(attributeName, attributeContent);
	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeRoot)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}
	pXML->nodeToRoot();

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeNext)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pXML->nextNode());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodePrevious)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pXML->previousNode());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeClear)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}
	pXML->nodeClear();

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeNew) 
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}
	
	SQBool linkNewXml;
	const char * XmlName;

	sq_getbool(pVM, -2, &linkNewXml);
	sq_getstring(pVM, -1, &XmlName);

#pragma warning(disable:4800)
	bool linkXml = (bool)linkNewXml;

	pXML->newNode(XmlName, linkXml);
	sq_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, nodeFind)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * nodeName;
	sq_getstring(pVM, -1, &nodeName);

	sq_pushbool(pVM, pXML->findNode(nodeName));
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, nodeFirstChild)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pXML->childNodeFirst());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeParent)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	pXML->nodeParent();

	sq_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeName)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * name = pXML->nodeName();
	if(name == 0) sq_pushbool(pVM, false);
	else sq_pushstring(pVM, name, strlen(name));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeContent)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * content = pXML->nodeContent();
	if(content == 0) sq_pushbool(pVM, false);
	else sq_pushstring(pVM, content, strlen(content));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeSetContent)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * content;
	sq_getstring(pVM, -1, &content);

	pXML->nodeSetContent(content);

	sq_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, commentNew)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * val;
	sq_getstring(pVM, -1, &val);

	pXML->newComment(val);

	sq_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, lastError)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	const char * error = pXML->lastError(0, 0);
	if(error == 0) sq_pushbool(pVM, false);
	else sq_pushstring(pVM, error, strlen(error));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, isComment)
{
	CXML * pXML = sq_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		sq_pushbool(pVM, false);
		return 1;
	}

	sq_pushbool(pVM, pXML->isComment());
	return 1;
}
