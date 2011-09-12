//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CHttpClient.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CHttpClient.h"

// OS Dependent Defines
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define closesocket close
#include <string.h>
#else
#include <winsock2.h>
#include <winsock.h>
#endif
#include <SharedUtility.h>

// OS Independent Defines
#define MAX_BUFFER 8192
#define DEFAULT_PORT 80
#define DEFAULT_USER_AGENT "IV: Multiplayer/1.0"
#define DEFAULT_REFERER "http://iv-multiplayer.com"

CHttpClient::CHttpClient()
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Invalidate the socket handle
	m_iSocket = INVALID_SOCKET;

	// Set the connected flag to false
	m_bConnected = false;

	// Reset the host
	m_strHost.Clear();

	// Reset the port
	m_usPort = DEFAULT_PORT;

	// Set the status to none
	m_status = HTTP_STATUS_NONE;

	// Reset the headers
	m_headerMap.clear();

	// Reset the data
	m_strData.Clear();

	// Set the last error to none
	m_lastError = HTTP_ERROR_NONE;

	// Set the default user agent
	m_strUserAgent = DEFAULT_USER_AGENT;

	// Set the default referer
	m_strReferer = DEFAULT_REFERER;

	// Set the default request timeout
	m_uiRequestTimeout = 30000;

	// Reset the request start
	m_uiRequestStart = 0;

	// Reset the receive handler
	m_pfnReceiveHandler = NULL;

	// Reset the receive handler user data
	m_pReceiveHandlerUserData = NULL;
}

CHttpClient::~CHttpClient()
{
	// If we are connected to a host disconnect
	if(m_bConnected)
		Disconnect();

	// If windows cleanup winsock
#ifdef WIN32
	WSACleanup();
#endif
}

bool CHttpClient::Connect()
{
	// Prepare the socket
	m_iSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(m_iSocket == INVALID_SOCKET)
	{
		// Failed to prepare the socket, set the last error
		m_lastError = HTTP_ERROR_SOCKET_PREPARE_FAILED;
		return false;
	}

	// Get the host
	hostent * heHost = gethostbyname(m_strHost.Get());

	if(heHost == NULL)
	{
		// Failed to get the host, set the last error
		m_lastError = HTTP_ERROR_INVALID_HOST;
		return false;
	}

	// Prepare a socket address
	sockaddr_in sinAddress;
	sinAddress.sin_family = AF_INET;
	sinAddress.sin_port = htons(m_usPort);
	sinAddress.sin_addr = *((in_addr *)heHost->h_addr);
	memset(&sinAddress.sin_zero, 0, (sizeof(char) * 8));

	// Set the socket to non blocking
	/*u_long sockopt = 1;

	if(ioctlsocket(m_iSocket, FIONBIO, &sockopt) != 0)
	{
		// Failed to ioctl the socket, set the last error
		m_lastError = HTTP_ERROR_IOCTL_FAILED;
		return false;
	}*/

	// Try to connect
	if(connect(m_iSocket, (sockaddr *)&sinAddress, sizeof(sockaddr)) < 0)
	{
		// Disconnect
		Disconnect();

		// Connection failed, set the last error
		m_lastError = HTTP_ERROR_CONNECTION_FAILED;
		return false;
	}

	// Set the connected flag to true
	m_bConnected = true;
	return true;
}

void CHttpClient::Disconnect()
{
	// Is the socket valid?
	if(m_iSocket != INVALID_SOCKET)
	{
		// Close the socket
		closesocket(m_iSocket);

		// Invalidate the socket
		m_iSocket = INVALID_SOCKET;
	}

	// Set the connected flag to false
	m_bConnected = false;
}

bool CHttpClient::Write(const char * szData, int iLen)
{
	// Try to send
	if(send(m_iSocket, szData, iLen, 0) < 0)
	{
		// Send failed
		m_lastError = HTTP_ERROR_SEND_FAILED;
		return false;
	}

	// Send success
	return true;
}

int CHttpClient::Read(char * szBuffer, int iLen)
{
	// HACK: Method for non blocking recv calls only
	// Set the socket to non blocking
	u_long sockopt = 1;
#ifdef _LINUX
        int flags = fcntl(m_iSocket, F_GETFL, 0);
	flags = flags &= ~O_NONBLOCK;
        fcntl(m_iSocket, F_SETFL, flags);
#else
        ioctlsocket(m_iSocket, FIONBIO, &sockopt);
#endif
	// Try to receive
	int iBytesRead = recv(m_iSocket, szBuffer, iLen, 0);
	// Set the socket to blocking
	sockopt = 0;
#ifdef _LINUX
	flags = fcntl(m_iSocket, F_GETFL, 0); 
	fcntl(m_iSocket, F_SETFL, flags | O_NONBLOCK);
#else
	ioctlsocket(m_iSocket, FIONBIO, &sockopt);
#endif
	return iBytesRead;
}

void CHttpClient::Reset()
{
	// Are we connected?
	if(IsConnected())
	{
		// Disconnect
		Disconnect();
	}

	// Set the status to none
	m_status = HTTP_STATUS_NONE;
}

bool CHttpClient::Get(String strPath)
{
	// Connect to the host
	if(!Connect())
	{
		// Connect failed
		return false;
	}

	// Reset the header and data
	m_headerMap.clear();
	m_strData.Clear();

	// Prepare the GET command
	String strGet("GET %s HTTP/1.0\r\n" \
				  "Host: %s\r\n" \
				  "User-Agent: %s\r\n" \
				  "Referer: %s\r\n" \
				  "Connection: close\r\n" \
				  "\r\n", 
				  strPath.C_String(), m_strHost.C_String(), 
				  m_strUserAgent.C_String(), m_strReferer.C_String());

	// Send the GET command
	if(!Write(strGet.C_String(), strGet.GetLength()))
	{
		// Send failed
		return false;
	}

	// Set the status to get data
	m_status = HTTP_STATUS_GET_DATA;

	// Set the request start
	m_uiRequestStart = SharedUtility::GetTime();
	return true;
}

bool CHttpClient::Post(bool bHasResponse, String strPath, String strData, String strContentType)
{
	// Connect to the host
	if(!Connect())
	{
		// Connect failed
		return false;
	}

	// Reset the header and data
	m_headerMap.clear();
	m_strData.Clear();

	// Prepare the POST command
	String strGet("POST %s HTTP/1.0\r\n" \
				  "Host: %s\r\n" \
				  "User-Agent: %s\r\n\r\n" \
				  "Referer: %s\r\n" \
				  "Content-Type: %s\r\n" \
				  "Content-Length: %d\r\n" \
				  "Connection: close\r\n" \
				  "\r\n" \
				  "%s", 
				  strPath.C_String(), m_strHost.C_String(), m_strUserAgent.C_String(), 
				  m_strReferer.C_String(), strContentType.C_String(), strData.GetLength(), 
				  strData.C_String());

	// Send the POST command
	if(!Write(strGet.C_String(), strGet.GetLength()))
	{
		// Send failed
		return false;
	}

	// Do we have a response
	if(bHasResponse)
	{
		// Set the status to get data
		m_status = HTTP_STATUS_GET_DATA;

		// Set the request start
		m_uiRequestStart = SharedUtility::GetTime();
	}
	else
	{
		// Disconnect from the host
		Disconnect();
	}

	return true;
	
}

bool CHttpClient::ParseHeaders(String& strBuffer, int& iBufferSize)
{
	// Ignore all initial whitespace
	unsigned int uiWhiteSpace = 0;

	for(unsigned int i = 0; i < (unsigned int)iBufferSize; i++)
	{
		// Is this whitespace?
		if(strBuffer[i] == ' ')
		{
			// Increment the whitespace amount
			uiWhiteSpace++;
			continue;
		}

		// Finished
		break;
	}

	// Remove whitespace
	if(uiWhiteSpace > 0)
		strBuffer.Erase(0, uiWhiteSpace);

	// Ignore the version, status code and status message
	// TODO: Use this information?
	// Will be in format 'HTTP/1.0 200 OK\r\n'
	unsigned int uiIgnore = strBuffer.Find("\r\n");

	if(uiIgnore == String::nPos)
		return false;

	strBuffer.Erase(0, (uiIgnore + 2));
	iBufferSize -= (uiIgnore + 2);

	// Find all headers
	unsigned int uiContentLength = String::nPos;
	unsigned int uiNameSplit;
	unsigned int uiValueSplit;

	while((uiNameSplit = strBuffer.Find(": ")) != String::nPos)
	{
		// Do we have a content length?
		if(uiContentLength != String::nPos)
		{
			// Get the content start
			unsigned int uiContentStart = (iBufferSize - uiContentLength);

			// Is the find over the content start?
			if(uiNameSplit >= uiContentStart)
				break;
		}

		// Find the value end
		uiValueSplit = strBuffer.Find("\r\n");

		// Did we not find a value end?
		if(uiValueSplit == String::nPos)
			return false;

		// Get the header name
		String strName = strBuffer.SubStr(0, uiNameSplit);

		// Get the header value
		String strValue = strBuffer.SubStr((uiNameSplit + 2), (uiValueSplit - (uiNameSplit + 2)));

		// Add the header to the header map
		m_headerMap[strName] = strValue;

		// Erase the header from the buffer
		strBuffer.Erase(0, (uiValueSplit + 2));
		iBufferSize -= (uiValueSplit + 2);

		// Is this the content length header?
		if(!strName.ICompare("content-length"))
		{
			// Set the content length
			uiContentLength = strValue.ToInteger();
		}
	}

	// HACK: Trim the buffer to the content length
	// jenksta: This can be caused by things such as accept-ranges headers
	if(iBufferSize > (int)uiContentLength)
	{
		strBuffer.Erase(0, (iBufferSize - uiContentLength));
		iBufferSize -= (iBufferSize - uiContentLength);
	}

	// Did we not get any headers?
	if(m_headerMap.empty())
		return false;

	// Success
	return true;
}

void CHttpClient::Process()
{
	// Do we have a request start and has the request timed out?
	if(m_uiRequestStart > 0 && (SharedUtility::GetTime() - m_uiRequestStart) >= m_uiRequestTimeout)
	{
		// Set the status to none
		m_status = HTTP_STATUS_NONE;

		// Request timed out, set the last error
		m_lastError = HTTP_ERROR_REQUEST_TIMEOUT;

		// Reset the request start
		m_uiRequestStart = 0;

		// Disconnect from the host
		Disconnect();
		return;
	}

	// Are we not in idle status?
	if(m_status != HTTP_STATUS_NONE)
	{
		switch(m_status)
		{
		case HTTP_STATUS_GET_DATA:
			{
				// Prepare a buffer
				char szBuffer[MAX_BUFFER];
				memset(szBuffer, 0, sizeof(szBuffer));

				// Try to read from the socket
				int iBytesRecieved = Read(szBuffer, sizeof(szBuffer));

				// Did we get anything?
				if(iBytesRecieved > 0)
				{
					// Create a string from the received data
					String strBuffer;
					strBuffer.Set(szBuffer, iBytesRecieved);

					// Are the headers empty?
					if(m_headerMap.empty())
					{
						// Parse the headers
						if(!ParseHeaders(strBuffer, iBytesRecieved))
						{
							// We don't have a header, set the status
							m_status = HTTP_STATUS_INVALID;

							// Set the last error
							m_lastError = HTTP_ERROR_NO_HEADER;

							// Reset the request start
							m_uiRequestStart = 0;

							// Disconnect from the host
							Disconnect();
							return;
						}

						// Do we not have any data?
						if(iBytesRecieved == 0)
							return;
					}

					// Call the receive handler if we have one
					bool bAppendData = true;

					if(m_pfnReceiveHandler)
						bAppendData = m_pfnReceiveHandler(strBuffer.Get(), iBytesRecieved, m_pReceiveHandlerUserData);

					// Append the buffer to the data if needed
					if(bAppendData)
						m_strData.Append(strBuffer, iBytesRecieved);
				}
				else if(iBytesRecieved == 0)
				{
					// We got data, set the status
					m_status = HTTP_STATUS_GOT_DATA;

					// Reset the request start
					m_uiRequestStart = 0;

					// Disconnect from the host
					Disconnect();
				}
			}

			break;
		}
	}
}

String CHttpClient::GetLastErrorString()
{
	String strError("Unknown");

	switch(GetLastError())
	{
	case HTTP_ERROR_SOCKET_PREPARE_FAILED:
		strError.Set("Failed to prepare socket");
		break;
	case HTTP_ERROR_INVALID_HOST:
		strError.Set("Invalid host");
		break;
	case HTTP_ERROR_IOCTL_FAILED:
		strError.Set("IoCtl failed");
		break;
	case HTTP_ERROR_CONNECTION_FAILED:
		strError.Set("Connection failed");
		break;
	case HTTP_ERROR_SEND_FAILED:
		strError.Set("Send failed");
		break;
	case HTTP_ERROR_REQUEST_TIMEOUT:
		strError.Set("Request timed out");
		break;
	case HTTP_ERROR_NO_HEADER:
		strError.Set("No header");
		break;
	}

	return strError;
}

void CHttpClient::SetReceiveHandle(ReceieveHandler_t pfnRecieveHandler, void * pUserData)
{
	m_pfnReceiveHandler = pfnRecieveHandler;
	m_pReceiveHandlerUserData = pUserData;
}
