//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMasterList.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CMasterList.h"
#include <SharedUtility.h>
#include <CLogFile.h>

CMasterList::CMasterList(String strHost, String strVersion, unsigned long ulTimeout, int iServerPort)
{
	m_pHttpClient = new CHttpClient();
	m_pHttpClient->SetRequestTimeout(10000);
	m_pHttpClient->SetHost(strHost);
	m_ulLastPulse = NULL;
	m_strPostPath.Format("/add.php?version=%s&port=%d", strVersion.Get(), iServerPort);
	m_ulTimeout = ulTimeout;
	m_bSentListedMessage = false;
	m_bSentErrorMessage = false;
}

CMasterList::~CMasterList()
{
	delete m_pHttpClient;
}

void CMasterList::Pulse()
{
	unsigned long ulTime = SharedUtility::GetTime();

	if((ulTime - m_ulLastPulse) > m_ulTimeout)
	{
		if(!m_bSentListedMessage && !m_bSentErrorMessage)
			CLogFile::Print("[Master List] Posting server to server list.");

		// Post the update
		if(!m_pHttpClient->Post(true, m_strPostPath) && !m_bSentErrorMessage)
		{
			CLogFile::Printf("[Master List] Failed to send post request to server list (%s)!", m_pHttpClient->GetLastErrorString().Get());
			m_bSentErrorMessage = true;
		}

		m_ulLastPulse = ulTime;
	}

	// Is the http client busy?
	if(m_pHttpClient->IsBusy())
	{
		// Process the http client
		m_pHttpClient->Process();

		// Check if we have got a response from our server listing
		if(m_pHttpClient->GotData())
		{
			// We have a response, check it
			if(*m_pHttpClient->GetData() == "Added")
			{
				// Server listed successfully
				if(!m_bSentListedMessage)
				{
					CLogFile::Print("[Master List] Server posted to server list successfully!");
					m_bSentListedMessage = true;
				}

				if(m_bSentErrorMessage)
					m_bSentErrorMessage = false;
			}
			else
			{
				// Server not listed
				if(!m_bSentErrorMessage)
				{
					String strError("Unknown error");

					if(m_pHttpClient->GetData()->IsEmpty())
						strError = "No data received";
					if(*m_pHttpClient->GetData() == "ERROR: Banned")
						strError = "You are banned from listing this server";
					else if(*m_pHttpClient->GetData() == "ERROR: Invalid port")
						strError = "You have specified an invalid port";
					else if(*m_pHttpClient->GetData() == "ERROR: Invalid version")
						strError = "You have specified an invalid version";

					CLogFile::Printf("[Master List] Failed to post server to server list (%s)!", strError.Get());
					m_bSentErrorMessage = true;
				}

				if(m_bSentListedMessage)
					m_bSentListedMessage = false;
			}
		}
	}
}
