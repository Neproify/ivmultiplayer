//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.h
// Project: Client.Core
// Author(s):	jenksta
//				CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CFileChecksum.h"
#include <curl\curl.h>

class CFileTransfer
{
private:
	bool			m_bIsResource;
	String			m_strName;
	unsigned short	m_usPort;
	String			m_strHostAddress;
	String			m_strFolderName;
	String			m_strHttpUrl;
	String			m_strPath;
	CFileChecksum	m_checksum;
	bool			m_bComplete;
	bool			m_bSucceeded;
	String			m_strError;

public:
	CFileTransfer(bool bIsResource, String strHost, unsigned short usPort, String strName, CFileChecksum checksum);
	~CFileTransfer();

	bool   IsResource() { return m_bIsResource; }
	String GetName() { return m_strName; }
	String GetHttpUrl() { return m_strHttpUrl; }
	String GetPath() { return m_strPath; }
	bool   IsComplete() { return m_bComplete; }
	bool   HasSucceeded() { return m_bSucceeded; }
	String GetError() { return m_strError; }
	bool   Download();

	static size_t curlWriteDataHandler(void* ptr, const size_t size, const size_t nmemb, FILE* stream)
	{
		// Save downloaded data to file:
		return fwrite(ptr, size, nmemb, stream);
	}
	static CURLcode curlDownloadToFile(const FILE* fWriteTo, const char* szUrl)
	{
		CURLcode ret = CURLE_FAILED_INIT;
		CURL* curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, szUrl);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fWriteTo);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteDataHandler);

			ret = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
		return ret;
	}
};
