//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CWebserver.cpp
// Project: Server.Core
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "CWebserver.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <CSettings.h>
#include "CEvents.h"
#include <algorithm>
#ifdef _LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif
#include <CLogFile.h>

extern CEvents * g_pEvents;
CMutex           g_webMutex;

static int begin_request_handler(struct mg_connection *conn) 
{
	if(conn)
	{
		// Lock the web mutex
		g_webMutex.Lock();
		const mg_request_info* request_info = mg_get_request_info(conn);
		// Get the ip address
		in_addr sa;
		memset(&sa, 0, sizeof(in_addr));
		sa.s_addr = htonl(request_info->remote_ip);
		char * szIpAddress = inet_ntoa(sa);

		// Call the scripting event
		CSquirrelArguments args;
		args.push(request_info->uri);
		args.push(szIpAddress);
		args.push(request_info->request_method);
		CSquirrelArgument ret = g_pEvents->Call("webRequest", &args);

		// Do we have a string returned?
		if(ret.GetType() == SQObjectType::OT_BOOL)
		{
			g_webMutex.Unlock();
			return !ret.GetBool();
		}
		else if(ret.GetType() == SQObjectType::OT_INTEGER)
		{
			g_webMutex.Unlock();
			return ret.GetInteger() ? 0 : 1;
		}
		else
		{
			printf("wrong return type in webRequest\n");
			g_webMutex.Unlock();
			return 0;
		}
	}
}

CWebServer::CWebServer(unsigned short usHTTPPort) 
{
	// Reset the mongoose context pointer
	m_pMongooseContext = NULL;

	// Do we not have an external Webserver configured?
	if(CVAR_GET_STRING("httpserver").IsEmpty())
	{
		// Allocate the options
		char * options[7];

		for(int i = 0; i < 6; i++)
			options[i] = new char[256];

		// Set the options
		strcpy(options[0], "listening_ports");
		strcpy(options[1], String("%d", usHTTPPort).Get());
		strcpy(options[2], "document_root");
		strcpy(options[3], SharedUtility::GetAbsolutePath("webserver").Get());
		strcpy(options[4], "num_threads");
		strcpy(options[5], "50");
		options[6] = NULL;

		struct mg_callbacks callbacks;
		memset(&callbacks, 0, sizeof(callbacks));
		callbacks.begin_request = begin_request_handler;
		// Start the mongoose context
		m_pMongooseContext = mg_start(&callbacks, NULL, (const char**)options);

		// Free the options
		for(int i = 0; i < 6; i++)
			delete [] options[i];

		// Did the mongoose context not start successfully?
		if(!m_pMongooseContext)
			CLogFile::Printf("Failed to start Webserver on port %d.\n", usHTTPPort);
	}
}

CWebServer::~CWebServer() 
{
	// Stop the mongoose context
	if(m_pMongooseContext)
	{
		mg_stop(m_pMongooseContext);
		m_pMongooseContext = NULL;
	}
}

bool CWebServer::FileCopy(String strClientFile, bool bIsScript, CFileChecksum &fileChecksum)
{
	String strType = bIsScript ? "clientscripts" : "resources";
	String strClientFilePath(SharedUtility::GetAbsolutePath("%s/%s", strType.Get(), strClientFile.Get()));

	// Do we not have an external Webserver configured?
	if(CVAR_GET_STRING("httpserver").IsEmpty())
	{
		String strWebServerFolder(SharedUtility::GetAbsolutePath("webserver"));
		String strClientWebServerFilePath("%s/%s/%s", strWebServerFolder.Get(), strType.Get(), strClientFile.Get());

		// Create the web server folder if needed
		if(!SharedUtility::Exists(strWebServerFolder.Get()))
			SharedUtility::CreateDirectory(strWebServerFolder.Get());

		// Create the web server type folder if needed
		String strWebServerTypeFolder("%s/%s", strWebServerFolder.Get(), strType.Get());

		if(!SharedUtility::Exists(strWebServerTypeFolder.Get()))
			SharedUtility::CreateDirectory(strWebServerTypeFolder.Get());

		if(!SharedUtility::CopyFile(strClientFilePath.Get(), strClientWebServerFilePath.Get()))
			return false;
	}

	return fileChecksum.Calculate(strClientFilePath.Get());
}
