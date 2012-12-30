//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CHttpClient.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <CString.h>
#include <map>

// OS Independent Defines
#define DEFAULT_CONTENT_TYPE "text/plain"
//#define DEFAULT_CONTENT_TYPE "application/x-www-form-urlencoded"

#ifndef WIN32
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

// Status codes
enum eHttpStatus
{
	HTTP_STATUS_NONE,
	HTTP_STATUS_INVALID,
	HTTP_STATUS_GET_DATA,
	HTTP_STATUS_GOT_DATA
};

// Error codes
enum eHttpError
{
	HTTP_ERROR_NONE,
	HTTP_ERROR_SOCKET_PREPARE_FAILED,
	HTTP_ERROR_INVALID_HOST,
	HTTP_ERROR_IOCTL_FAILED,
	HTTP_ERROR_CONNECTION_FAILED,
	HTTP_ERROR_SEND_FAILED,
	HTTP_ERROR_REQUEST_TIMEOUT,
	HTTP_ERROR_NO_HEADER
};

typedef bool (* ReceieveHandler_t)(const char * szData, unsigned int uiDataSize, void * pUserData);

class CHttpClient
{
private:
	int                      m_iSocket;
	bool                     m_bConnected;
	String                   m_strHost;
	unsigned short           m_usPort;
	eHttpStatus              m_status;
	std::map<String, String> m_headerMap;
	String                   m_strData;
	eHttpError               m_lastError;
	String                   m_strUserAgent;
	String                   m_strReferer;
	unsigned int             m_uiRequestTimeout;
	unsigned int             m_uiRequestStart;
	ReceieveHandler_t        m_pfnReceiveHandler;
	void                   * m_pReceiveHandlerUserData;
	FILE					* m_file;

	bool                   Connect();
	void                   Disconnect();
	bool                   Write(const char * szData, int iLen);
	int                    Read(char * szBuffer, int iLen);
	bool                   ParseHeaders(String& strBuffer, int& iBufferSize);

public:
	CHttpClient();
	~CHttpClient();

	virtual bool           IsConnected() { return m_bConnected; }
	virtual bool           IsInvalid() { return (m_status == HTTP_STATUS_INVALID); }
	virtual bool           GettingData() { return (m_status == HTTP_STATUS_GET_DATA); }
	virtual bool           GotData() { return (m_status == HTTP_STATUS_GOT_DATA); }
	virtual bool           IsBusy() { return (m_status == HTTP_STATUS_GET_DATA); }
	virtual String         GetHeader(String strName) { return m_headerMap[strName]; }
	virtual String       * GetData() { return &m_strData; }
	virtual eHttpError     GetLastError() { return m_lastError; }
	virtual void           SetUserAgent(String strUserAgent) { m_strUserAgent = strUserAgent; }
	virtual String         GetUserAgent() { return m_strUserAgent; }
	virtual void           SetReferer(String strReferer) { m_strReferer = strReferer; }
	virtual String         GetReferer() { return m_strReferer; }
	virtual void           SetRequestTimeout(unsigned int uiRequestTimeout) { m_uiRequestTimeout = uiRequestTimeout; }
	virtual unsigned int   GetRequestTimeout() { return m_uiRequestTimeout; }
	virtual void           SetHost(String strHost) { m_strHost = strHost; }
	virtual String         GetHost() { return m_strHost; }
	virtual void           SetPort(unsigned short usPort) { m_usPort = usPort; }
	virtual unsigned short GetPort() { return m_usPort; }
	virtual void           Reset();
	virtual bool           Get(String strPath);
	virtual void		   SendReport(String strPath, String strReport);
	virtual bool           Post(bool bHasResponse, String strPath, String strData = "", String strContentType = DEFAULT_CONTENT_TYPE);
	virtual void           Process();
	virtual String         GetLastErrorString();
	virtual void           SetReceiveHandle(ReceieveHandler_t pfnRecieveHandler, void * pUserData = NULL);
	// Sets a file which we have to write received data to
	virtual void           SetFile(FILE * f = NULL) { m_file = f; }
};
