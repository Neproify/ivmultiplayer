//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.cpp
// Project: Client.Core
// Author(s):	jenksta
//				CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileTransfer.h"
#include <CLogFile.h>

CFileTransfer::CFileTransfer(bool bIsResource, String strHost, unsigned short usPort, String strName, CFileChecksum checksum)
							: m_bIsResource(bIsResource), m_strName(strName), m_checksum(checksum), 
							  m_bComplete(false), m_bSucceeded(false), m_strError("None")
{
	// Set up a local file/directory for this resource
	String m_strFolderName = "clientscripts";
	if(bIsResource)
		m_strFolderName = "resources";
	m_strPath.Set(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", m_strFolderName.Get(), m_strName.Get()));

	// Set up our http client:
	m_httpClient = CHttpClient();
	m_httpClient.SetHost(strHost);
	m_httpClient.SetPort(usPort);
	m_strHttpUrl.Format("/%s/%s", m_strFolderName.Get(), m_strName.Get());
}

CFileTransfer::~CFileTransfer() { }

bool CFileTransfer::Download()
{
	// Does the file already exist?
	if(SharedUtility::Exists(m_strPath))
	{
		// Does the checksum of our existing file match our target checksum?
		CFileChecksum fileChecksum;
		fileChecksum.Calculate(m_strPath);

		if(fileChecksum == m_checksum)
		{
			m_bComplete = true;
			m_bSucceeded = true;
			m_strError = "None";
			return true;
		}
	}

	// Attempt to open our destination file
	FILE * fFile = fopen(m_strPath.Get(), "wb");
	if(!fFile)
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Failed to open destination file %s for file %s", m_strName.Get(), m_strPath.Get());
		return false;
	}

	// Send a request:
	if(!m_httpClient.Get(m_strHttpUrl))
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Http GET failed for file %s (%s)", m_strName.Get(), m_httpClient.GetLastErrorString().Get());
		return false;
	}

	// Download data to file:
	m_httpClient.SetFile(fFile);	
	while(m_httpClient.IsBusy())
		m_httpClient.Process();

	// Close our downloaded file
	fclose(fFile);

	// Calculate our downloaded file checksum
	CFileChecksum fileChecksum;
	fileChecksum.Calculate(m_strPath);

	// Check the checksum of the downloaded file against our target checksum
	if(fileChecksum != m_checksum)
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Checksum mismatch for file %s (Expected 0x%x, Got 0x%x)", m_strName.Get(), m_checksum.GetChecksum(), fileChecksum.GetChecksum());
		return false;
	}

	// File was downloaded successfully
	m_bComplete = true;
	m_bSucceeded = true;
	m_strError = "None";
	return true;
}
