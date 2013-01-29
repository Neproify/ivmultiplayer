//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.cpp
// Project: Client.Core
// Author(s):	jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileTransfer.h"
#include <CLogFile.h>

CFileTransfer::CFileTransfer(bool bIsResource, String strHost, unsigned short usPort, String strName, CFileChecksum checksum)
							: m_bIsResource(bIsResource), m_strName(strName), m_checksum(checksum), 
							  m_bComplete(false), m_bSucceeded(false), m_strError("None")
{
	// Set our http client host and port
	m_httpClient.SetHost(strHost);
	m_httpClient.SetPort(usPort);

	// Get the type name
	String strTypeName;

	if(bIsResource)
		strTypeName = "resources";
	else
		strTypeName = "clientscripts";

	// Set our http path
	m_strHttpPath.Format("/%s/%s", strTypeName.Get(), strName.Get());

	// Set our save path
	m_strPath.Set(SharedUtility::GetAbsolutePath("clientfiles\\%s\\%s", strTypeName.Get(), strName.Get()));
}

CFileTransfer::~CFileTransfer()
{

}

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

	// Attempt to request our file transfer
	if(!m_httpClient.Get(m_strHttpPath))
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("GET failed for file %s (%s)", m_strName.Get(), m_httpClient.GetLastErrorString().Get());
		return false;
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

	// Set http download file
	m_httpClient.SetFile(fFile);

	// Process our http client
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
