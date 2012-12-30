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
	: m_iSocket(INVALID_SOCKET),
	m_bConnected(false),
	m_usPort(DEFAULT_PORT),
	m_status(HTTP_STATUS_NONE),
	m_lastError(HTTP_ERROR_NONE),
	m_strUserAgent(DEFAULT_USER_AGENT),
	m_strReferer(DEFAULT_REFERER),
	m_uiRequestTimeout(30000),
	m_uiRequestStart(0),
	m_pfnReceiveHandler(NULL),
	m_pReceiveHandlerUserData(NULL),
	m_file(NULL)
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Reset the host
	m_strHost.Clear();

	// Reset the headers
	m_headerMap.clear();

	// Reset the data
	m_strData.Clear();
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
#ifdef _LINUX
	int flags = fcntl(m_iSocket, F_GETFL, 0);
	UNSET_BIT(flags, O_NONBLOCK);
	fcntl(m_iSocket, F_SETFL, flags);
#else
	u_long sockopt = 1;
	ioctlsocket(m_iSocket, FIONBIO, &sockopt);
#endif

	// Try to receive
	int iBytesRead = recv(m_iSocket, szBuffer, iLen, 0);

	// Set the socket to blocking
#ifdef _LINUX
	SET_BIT(flags, O_NONBLOCK);
	fcntl(m_iSocket, F_SETFL, flags);
#else
	sockopt = 0;
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
				  strPath.Get(), m_strHost.Get(), 
				  m_strUserAgent.Get(), m_strReferer.Get());

	// Send the GET command
	if(!Write(strGet.C_String(), strGet.GetLength()))
	{
		//CLogFile::Printf("HTTP REQUEST FAILED!");
		// Send failed
		return false;
	}

	// Set the status to get data
	m_status = HTTP_STATUS_GET_DATA;

	// Set the request start
	m_uiRequestStart = SharedUtility::GetTime();
	return true;
}

// Send Report writes data into the header(can be readout with php://input http://php.net/manual/en/function.http-get-request-body.php
void CHttpClient::SendReport(String strPath, String strReport)
{
	// Connect to the host
	if(!Connect())
	{
		// Connect failed
		return;
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
				  "\n%s" \
				  "\r\n",
				  strPath.Get(), m_strHost.Get(), 
				  m_strUserAgent.Get(), m_strReferer.Get(), strReport.Get());

	// Send the GET command
	if(!Write(strGet.C_String(), strGet.GetLength()))
	{
		//CLogFile::Printf("HTTP REQUEST FAILED!");
		// Send failed
		return;
	}

	// Set the status to get data
	m_status = HTTP_STATUS_GET_DATA;

	// Set the request start
	m_uiRequestStart = SharedUtility::GetTime();
	return;
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
				  strPath.Get(), m_strHost.Get(), m_strUserAgent.Get(), 
				  m_strReferer.Get(), strContentType.Get(), strData.GetLength(),
				  strData.Get());

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
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct mg_request_info {
  char *request_method;  // "GET", "POST", etc
  char *uri;             // URL-decoded URI
  char *http_version;    // E.g. "1.0", "1.1"
  char *query_string;    // URL part after '?' (not including '?') or NULL
  char *remote_user;     // Authenticated user, or NULL if no auth used
  long remote_ip;        // Client's IP address
  int remote_port;       // Client's port
  int is_ssl;            // 1 if SSL-ed, 0 if not
  int num_headers;       // Number of headers
  struct mg_header {
    char *name;          // HTTP header name
    char *value;         // HTTP header value
  } http_headers[64];    // Maximum 64 headers
};

static char *skip_quoted(char **buf, const char *delimiters,
                         const char *whitespace, char quotechar) {
  char *p, *begin_word, *end_word, *end_whitespace;

  begin_word = *buf;
  end_word = begin_word + strcspn(begin_word, delimiters);

  // Check for quotechar
  if (end_word > begin_word) {
    p = end_word - 1;
    while (*p == quotechar) {
      // If there is anything beyond end_word, copy it
      if (*end_word == '\0') {
        *p = '\0';
        break;
      } else {
        size_t end_off = strcspn(end_word + 1, delimiters);
        memmove (p, end_word, end_off + 1);
        p += end_off; // p must correspond to end_word - 1
        end_word += end_off + 1;
      }
    }
    for (p++; p < end_word; p++) {
      *p = '\0';
    }
  }

  if (*end_word == '\0') {
    *buf = end_word;
  } else {
    end_whitespace = end_word + 1 + strspn(end_word + 1, whitespace);

    for (p = end_word; p < end_whitespace; p++) {
      *p = '\0';
    }

    *buf = end_whitespace;
  }

  return begin_word;
}

// Simplified version of skip_quoted without quote char
// and whitespace == delimiters
static char *skip(char **buf, const char *delimiters) {
  return skip_quoted(buf, delimiters, delimiters, 0);
}

// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
static int get_request_len(const char *buf, int buflen) {
  const char *s, *e;
  int len = 0;

  for (s = buf, e = s + buflen - 1; len <= 0 && s < e; s++)
    // Control characters are not allowed but >=128 is.
    if (!isprint(* (const unsigned char *) s) && *s != '\r' &&
        *s != '\n' && * (const unsigned char *) s < 128) {
      len = -1;
      break; // [i_a] abort scan as soon as one malformed character is found; don't let subsequent \r\n\r\n win us over anyhow
    } else if (s[0] == '\n' && s[1] == '\n') {
      len = (int) (s - buf) + 2;
    } else if (s[0] == '\n' && &s[1] < e &&
        s[1] == '\r' && s[2] == '\n') {
      len = (int) (s - buf) + 3;
    }

  return len;
}

// Parse HTTP headers from the given buffer, advance buffer to the point
// where parsing stopped.
static void parse_http_headers(char **buf, struct mg_request_info *ri) {
  int i;

  for (i = 0; i < (int) ARRAY_SIZE(ri->http_headers); i++) {
    ri->http_headers[i].name = skip_quoted(buf, ":", " ", 0);
    ri->http_headers[i].value = skip(buf, "\r\n");
    if (ri->http_headers[i].name[0] == '\0')
      break;
    ri->num_headers = i + 1;
  }
}

static int is_valid_http_method(const char *method) {
  return !strcmp(method, "GET") || !strcmp(method, "POST") ||
    !strcmp(method, "HEAD") || !strcmp(method, "CONNECT") ||
    !strcmp(method, "PUT") || !strcmp(method, "DELETE") ||
    !strcmp(method, "OPTIONS") || !strcmp(method, "PROPFIND");
}

// Parse HTTP request, fill in mg_request_info structure.
// This function modifies the buffer by NUL-terminating
// HTTP request components, header names and header values.
static int parse_http_message(char *buf, int len, struct mg_request_info *ri) {
  int request_length = get_request_len(buf, len);
  if (request_length > 0) {
    // Reset attributes. DO NOT TOUCH is_ssl, remote_ip, remote_port
    ri->remote_user = ri->request_method = ri->uri = ri->http_version = NULL;
    ri->num_headers = 0;

    buf[request_length - 1] = '\0';

    // RFC says that all initial whitespaces should be ingored
    while (*buf != '\0' && isspace(* (unsigned char *) buf)) {
      buf++;
    }
    ri->request_method = skip(&buf, " ");
    ri->uri = skip(&buf, " ");
    ri->http_version = skip(&buf, "\r\n");
    parse_http_headers(&buf, ri);
  }
  return request_length;
}

static int parse_http_request(char *buf, int len, struct mg_request_info *ri) {
  int result = parse_http_message(buf, len, ri);
  if (result > 0 &&
      is_valid_http_method(ri->request_method) &&
      !strncmp(ri->http_version, "HTTP/", 5)) {
    ri->http_version += 5;   // Skip "HTTP/"
  } else {
    result = -1;
  }
  return result;
}

static int parse_http_response(char *buf, int len, struct mg_request_info *ri) {
  int result = parse_http_message(buf, len, ri);
  return result > 0 && !strncmp(ri->request_method, "HTTP/", 5) ? result : -1;
}

bool CHttpClient::ParseHeaders(String& strBuffer, int& iBufferSize)
{
	// Find the header size, testing code, but should work
	mg_request_info info;
	
	char* buf = new char[iBufferSize];
	memcpy(buf, strBuffer.C_String(), iBufferSize);
	parse_http_response(buf, strBuffer.GetLength(), &info);
	
	String buf_header;
	int iHeaderSize = 0;
	for(int i = 0; i < info.num_headers; ++i)
	{
		buf_header.AppendF("%s: %s\r\n", info.http_headers[i].name, info.http_headers[i].value);
	}

	iHeaderSize = buf_header.GetLength();
	iHeaderSize += (info.request_method != NULL ? strlen(info.request_method) : 0) + (info.uri != NULL ? strlen(info.uri) : 0) + (info.http_version != NULL ? strlen(info.http_version) : 0) + strlen("\r\n\r\n\r\n");
	iBufferSize -= iHeaderSize;
	strBuffer.Erase(0, iHeaderSize);
	m_headerMap["HeaderSize"] = iHeaderSize;


	// ADAMIX/JENKSTA: commented out this code because doesn't work properly. Are we really need to parse headers?

	/*// Ignore all initial whitespace
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

		// If this is an accept-ranges header get the value from the next header
		// jenksta: not sure if this is right, but this is how it works with 'accept-ranges: bytes'
		// in mongoose
		if(!strName.ICompare("accept-ranges"))
		{
			// Find the value end
			uiValueSplit = strBuffer.Find("\r\n", (uiValueSplit + 2));

			// Did we not find a value end?
			if(uiValueSplit == String::nPos)
				return false;
		}

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

	// Did we not get any headers?
	if(m_headerMap.empty())
		return false;
	*/
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

				int iSkipBytes = 0;

				// Did we get anything?
				if(iBytesRecieved > 0)
				{
					// Create a string from the received data
					String strBuffer;
					strBuffer.Set(szBuffer, iBytesRecieved);
					
					// Are the headers empty?
					if(m_headerMap.empty())
					{
						iSkipBytes = iBytesRecieved;

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

						iSkipBytes -= iBytesRecieved;

						// Do we not have any data?
						if(iBytesRecieved == 0)
							return;
					}

					// Call the receive handler if we have one
					if(m_pfnReceiveHandler)
					{
						if(m_pfnReceiveHandler(szBuffer + iSkipBytes, iBytesRecieved, m_pReceiveHandlerUserData))
							m_strData.Append(szBuffer + iSkipBytes, iBytesRecieved);
					}
					// Write to file if we have set one
					else if(m_file != NULL)
						fwrite(szBuffer, 1, iBytesRecieved, m_file);
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