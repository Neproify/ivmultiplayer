//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServerBrowser.cpp
// Project: Client.Core
// Author(s): jenksta, John
// License: See LICENSE in root directory
//
//==============================================================================
#include "CServerBrowser.h"
#include "CClient.h"
#include "CGame.h"

extern CClient * g_pClient;
CServerBrowser * CServerBrowser::m_pSingleton = NULL;
std::map<String, unsigned long>     serverPingStartMap;

// Custom ListboxTextItem class to automatically set the selection brush image on creation
class ServerBrowserListItem : public CEGUI::ListboxTextItem
{
public:
	ServerBrowserListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionColours(CEGUI::colour(0x0073AAFF));
		setSelectionBrushImage("WindowsLook", "Background");
	}
};

CServerBrowser::CServerBrowser(void)
{
	assert(!m_pSingleton);

	// Set our singleton
	m_pSingleton = this;

	// Create the master list query instance and set its handler
	m_pMasterListQuery = new CMasterListQuery(MASTERLIST_ADDRESS, MASTERLIST_VERSION);
	m_pMasterListQuery->SetMasterListQueryHandler(MasterListQueryHandler);

	// Create the server query instance and set its handler
	m_pServerQuery = new CServerQuery();
	m_pServerQuery->SetServerQueryHandler(ServerQueryHandler);

	// Setup the GUI
	CGUI * pGUI = g_pClient->GetGUI();

	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();

	m_GUIElements.pWindow = pGUI->CreateGUIFrameWindow();
	m_GUIElements.pWindow->setText("Server Browser");
	m_GUIElements.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, 900.0f), CEGUI::UDim(0, 500.0f)));
	m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-450.0f), CEGUI::UDim(0, fHeight/2-250.0f)));
	m_GUIElements.pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CServerBrowser::OnCloseClick, this));
	m_GUIElements.pWindow->setVisible(false);

	m_GUIElements.pServerMultiColumnList = pGUI->CreateGUIMultiColumnList(m_GUIElements.pWindow);
	m_GUIElements.pServerMultiColumnList->setText("Server Browser");
	m_GUIElements.pServerMultiColumnList->setSize(CEGUI::UVector2(CEGUI::UDim(0.950f, 0), CEGUI::UDim(0.8250f, 0)));
	m_GUIElements.pServerMultiColumnList->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0250f, 0), CEGUI::UDim(0.0250f, 0)));
	m_GUIElements.pServerMultiColumnList->setProperty("ColumnHeader", "text:Name width:{0.5,0} id:0");
	m_GUIElements.pServerMultiColumnList->setProperty("ColumnHeader", "text:Host width:{0.2,0} id:1");
	m_GUIElements.pServerMultiColumnList->setProperty("ColumnHeader", "text:Players width:{0.1,0} id:2");
	m_GUIElements.pServerMultiColumnList->setProperty("ColumnHeader", "text:Ping width:{0.1,0} id:3");
	m_GUIElements.pServerMultiColumnList->setProperty("ColumnHeader", "text:Locked width:{0.09,0} id:4");
	m_GUIElements.pServerMultiColumnList->setFont(pGUI->GetFont("tahoma-bold", 10));
	m_GUIElements.pServerMultiColumnList->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CServerBrowser::OnRowClick, this));

	m_GUIElements.pRefreshButton = pGUI->CreateGUIButton(m_GUIElements.pWindow);
	m_GUIElements.pRefreshButton->setText("Refresh");
	m_GUIElements.pRefreshButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.10f, 0)));
	m_GUIElements.pRefreshButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.550f, 0), CEGUI::UDim(0.8750f, 0)));
	m_GUIElements.pRefreshButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CServerBrowser::OnRefreshButtonClick, this));

	m_GUIElements.pConnectButton = pGUI->CreateGUIButton(m_GUIElements.pWindow);
	m_GUIElements.pConnectButton->setText("Connect");
	m_GUIElements.pConnectButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.10f, 0)));
	m_GUIElements.pConnectButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7750f, 0), CEGUI::UDim(0.8750f, 0)));
	m_GUIElements.pConnectButton->setEnabled(false);
	m_GUIElements.pConnectButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CServerBrowser::OnConnectButtonClick, this));

	m_bVisible = false;
}


CServerBrowser::~CServerBrowser(void)
{
	// Delete the query instances
	SAFE_DELETE(m_pServerQuery);
	SAFE_DELETE(m_pMasterListQuery);

	// Delete the GUI elements
	CGUI * pGUI = g_pClient->GetGUI();
	pGUI->RemoveGUIWindow(m_GUIElements.pWindow);
}

void CServerBrowser::Process()
{
	// Process the server and master list queries
	m_pMasterListQuery->Process();
	m_pServerQuery->Process();
}

// (John): Do we really need to re-position the GUI every time the D3D device resets?
void CServerBrowser::OnResetDevice()
{
	CGUI * pGUI = g_pClient->GetGUI();
	
	float fWidth = (float)pGUI->GetDisplayWidth();
	float fHeight = (float)pGUI->GetDisplayHeight();
	
	m_GUIElements.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/2-450.0f), CEGUI::UDim(0, fHeight/2-250.0f)));
}

void CServerBrowser::SetVisible(bool bVisible)
{
	m_GUIElements.pWindow->setVisible(bVisible);
	m_GUIElements.pWindow->setProperty("AlwaysOnTop", "true");
    m_GUIElements.pWindow->activate();

	if(bVisible)
		OnMasterListQuery(0);

	m_bVisible = bVisible;
}

void CServerBrowser::ServerQueryHandler(String strHost, unsigned short usPort, String strQuery, CBitStream * pReply)
{
	// Read the query type
	char cQueryType;

	if(!pReply->Read(cQueryType))
		return;

	// Get the server host and port
	String strHostAndPort("%s:%d", strHost.Get(), usPort);

	if(cQueryType == 'p') // Ping
	{
		// Get the start time from the ping map
		unsigned long ulStartTime = serverPingStartMap[strHostAndPort];

		// Do we have a valid start time?
		if(ulStartTime > 0)
		{
			// Calculate the round trip time
			unsigned long ulPing = (SharedUtility::GetTime() - ulStartTime);

			// Set the server ping in the multi column list
			CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)CServerBrowser::GetSingleton()->m_GUIElements.pServerMultiColumnList;

			for(unsigned int i = 0; i < pMultiColumnList->getRowCount(); i++)
			{
				CEGUI::ListboxItem * pHost = pMultiColumnList->getItemAtGridReference(CEGUI::MCLGridRef(i, 1));

				if(!strHostAndPort.Compare(pHost->getText().c_str()))
				{
					CEGUI::ListboxItem * pPing = pMultiColumnList->getItemAtGridReference(CEGUI::MCLGridRef(i, 3));

					if(pPing)
					{
						char szTempBuf[64];
						pPing->setText(itoa(ulPing, szTempBuf, 10));
						pMultiColumnList->invalidate();
						break;
					}
				}
			}
		}
	}
	else
	{
		// Check if we have a valid stream
		if(!pReply || cQueryType != 'i')
			return;

		// Read the host name
		String strHostName;
		int iPlayerCount;
		int iMaxPlayers;
		bool bPassworded;

		pReply->Read(strHostName);
		pReply->Read(iPlayerCount);
		pReply->Read(iMaxPlayers);
		pReply->Read(bPassworded);

		// Add the server to the multi column list
		CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)CServerBrowser::GetSingleton()->m_GUIElements.pServerMultiColumnList;
		unsigned int iRowIndex = pMultiColumnList->addRow();
		pMultiColumnList->setItem(new ServerBrowserListItem(strHostName.Get()), 0, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem(strHostAndPort.Get()), 1, iRowIndex);
		char szPlayerCount[9];
		sprintf(szPlayerCount, "%s/%s", iPlayerCount, iMaxPlayers);
		pMultiColumnList->setItem(new ServerBrowserListItem(szPlayerCount), 2, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem("9999"), 3, iRowIndex);
		pMultiColumnList->setItem(new ServerBrowserListItem(bPassworded ? "Yes" : "No"), 4, iRowIndex);
		pMultiColumnList->invalidate();

		// Save the current time to the ping map
		serverPingStartMap[strHostAndPort] = SharedUtility::GetTime();

		// Send a ping query to the server
		CServerBrowser::GetSingleton()->m_pServerQuery->Query(strHost, usPort, "p");
	}
}

void CServerBrowser::MasterListQueryHandler(std::vector<String> serverVector)
{
	// Loop through all servers
	for(std::vector<String>::iterator iter = serverVector.begin(); iter != serverVector.end(); iter++)
	{
		// Get the address
		String strAddress = (*iter);

		// Get the ip and port
		size_t sSplit = strAddress.Find(':', 0);
		String strIp = strAddress.SubStr(0, sSplit++);
		String strPort = strAddress.SubStr(sSplit, (strAddress.GetLength() - sSplit));

		// Do we have a valid ip and port?
		if(strIp.IsEmpty() || strPort.IsEmpty())
			continue;

		// Query the server
		CServerBrowser::GetSingleton()->m_pServerQuery->Query(strIp, strPort.ToInteger(), "i");
	}
}

void CServerBrowser::OnMasterListQuery(int iType)
{
	// Reset the server browser list
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_GUIElements.pServerMultiColumnList;
	pMultiColumnList->resetList();

	// Reset the server and master list queries
	m_pMasterListQuery->Reset();
	m_pServerQuery->Reset();

	// Query the master list
	if(!m_pMasterListQuery->Query(iType))
	{
		String strError("Failed to contact the master list (%s).\nPlease check your internet connection.", m_pMasterListQuery->GetHttpClient()->GetLastErrorString().Get());
		g_pClient->GetGUI()->ShowMessageBox(strError.Get(), "Error");
	}
}

bool CServerBrowser::OnCloseClick(const CEGUI::EventArgs &eventArgs)
{
	SetVisible(false);
	return true;
}

bool CServerBrowser::OnRowClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_GUIElements.pServerMultiColumnList;
	m_GUIElements.pConnectButton->setEnabled((pMultiColumnList->getSelectedCount() > 0) ? true : false);
	return true;
}

bool CServerBrowser::OnRefreshButtonClick(const CEGUI::EventArgs &eventArgs)
{
	// Query the master list
	OnMasterListQuery(0);
	return true;
}

bool CServerBrowser::OnConnectButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEGUI::MultiColumnList * pMultiColumnList = (CEGUI::MultiColumnList *)m_GUIElements.pServerMultiColumnList;
	CEGUI::ListboxItem * pHostname = pMultiColumnList->getFirstSelectedItem();
	CEGUI::ListboxItem * pHost = pMultiColumnList->getNextSelected(pHostname);
	CEGUI::ListboxItem * pPlayerCount = pMultiColumnList->getNextSelected(pHost);
	CEGUI::ListboxItem * pMaxPlayers = pMultiColumnList->getNextSelected(pPlayerCount);
	CEGUI::ListboxItem * pPing = pMultiColumnList->getNextSelected(pMaxPlayers);
	CEGUI::ListboxItem * pPassworded = pMultiColumnList->getNextSelected(pPing);


	// Get the host and port
	String strHost;
	unsigned short usPort;

	if(!CMainMenu::GetHostAndPort(pHost->getText().c_str(), strHost, usPort))
	{
		g_pClient->GetGUI()->ShowMessageBox("You must enter a valid host and port!", "Error!");
		return false;
	}

	// Hide the server browser window
	SetVisible(false);

	// Does this server require a password?
	if(pPassworded->getText() == "yes")
	{
		// TODO: Show password entry window
	}
	else
	{
		// Call the connect function
		CMainMenu::GetSingleton()->OnConnect(strHost, usPort, "");
	}

	return true;
}