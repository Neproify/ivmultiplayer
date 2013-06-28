//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CMasterListQuery.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CMasterListQuery.h"
#include <CLogFile.h>

CMasterListQuery::CMasterListQuery(String strHost, String strVersion)
{
	m_pHttpClient = new CHttpClient();
	m_pHttpClient->SetRequestTimeout(10000);
	m_pHttpClient->SetHost(strHost);
	m_strVersion = strVersion;
	m_pfnMasterListQueryHandler = NULL;
}

CMasterListQuery::~CMasterListQuery()
{
	delete m_pHttpClient;
}

void CMasterListQuery::Reset()
{

}

bool CMasterListQuery::Query(int iType)
{
	String strPostPath("/list.php?version=01RC3");

	if(!m_pHttpClient->Get(strPostPath))
	{
		CLogFile::Print("FAILED TO CONTACT MASTERLIST");
		return false;
	}
	else
		return true;
}

void CMasterListQuery::Process()
{
	// Is the http client busy?
	if(m_pHttpClient->IsBusy())
	{
		// Process the http client
		m_pHttpClient->Process();

		// Check if we have got a response from our server query
		if(m_pHttpClient->GotData())
		{
			// Get the data
			String * strData = m_pHttpClient->GetData();

			// Did we not get any data?
			if(strData->IsEmpty())
				return;

			// Process received servers
			size_t sCurrent = 0;
			size_t sPosition = 0;
			std::vector<String> serverVector;

			while((sPosition = strData->Find('\n', sCurrent)) != String::nPos)
			{
				// Get the server
				String strAddress = strData->SubStr(sCurrent, (sPosition - sCurrent));

				// Add the server to the server vector
				serverVector.push_back(strAddress);

				// Set the current position
				sCurrent = (sPosition + 1);
				CLogFile::PrintDebugf("MASTERLIST-SERVER: %s",strAddress.C_String());
			}

			// Call the master list query callback (if we have one)
			if(m_pfnMasterListQueryHandler)
				m_pfnMasterListQueryHandler(serverVector);
		}
	}
}
