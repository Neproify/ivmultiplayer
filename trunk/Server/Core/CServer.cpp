//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServer.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Server socket natives?

#include "CServer.h"
#include <stdarg.h>
#include <queue>
#include "CNetworkManager.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CCheckpointManager.h"
#include "CPickupManager.h"
#include "Scripting/CScriptingManager.h"
#include "CClientFileManager.h"
#include "Natives.h"
#include "CModuleManager.h"
#include "Scripting/CScriptTimerManager.h"
#include "CMasterList.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/ticpp.h"
#include "SharedUtility.h"
#include "CWebserver.h"
#include <CSettings.h>
#include <Game/CTime.h>
#include "CEvents.h"
#include <Game/CTrafficLights.h>
#include <Network/CNetworkModule.h>
#include <Threading/CMutex.h>
#include <Threading/CThread.h>
#include "CQuery.h"
#include <CExceptionHandler.h>
#include "ModuleNatives/ModuleNatives.h"
#include "C3DLabels.h"

#define SERVER_TITLE MOD_NAME " v" MOD_VERSION_STRING " Server [" OS_STRING "]" DEBUG_IDENTIFIER

#define HEIPHEN_GEN(string, stringname) \
	{ \
		stringname[0] = '\0'; \
		\
		for(int i = 0; i < sizeof(string) + 1; i++) \
			strcat(stringname, "-"); \
	}

CNetworkManager                   * g_pNetworkManager = NULL;
CPlayerManager                    * g_pPlayerManager = NULL;
CVehicleManager                   * g_pVehicleManager = NULL;
CObjectManager                    * g_pObjectManager = NULL;
CBlipManager                      * g_pBlipManager = NULL;
CActorManager                     * g_pActorManager = NULL;
CPickupManager                    * g_pPickupManager = NULL;
CCheckpointManager                * g_pCheckpointManager = NULL;
CScriptingManager                 * g_pScriptingManager = NULL;
CScriptTimerManager               * g_pScriptTimerManager = NULL;
CClientFileManager                * g_pClientScriptFileManager = NULL;
CClientFileManager                * g_pClientResourceFileManager = NULL;
CModuleManager                    * g_pModuleManager = NULL;
CMasterList                       * g_pMasterList = NULL;
CWebServer                        * g_pWebserver = NULL;
CTime                             * g_pTime = NULL;
CTrafficLights                    * g_pTrafficLights = NULL;
CEvents                           * g_pEvents = NULL;
unsigned long                       g_ulStartTick = 0;
CQuery                            * g_pQuery = NULL;

C3DLabelManager		              * g_p3DLabelManager = NULL;

Modules::CActorModuleNatives      * g_pActorModuleNatives;
Modules::CBlipModuleNatives       * g_pBlipModuleNatives;
Modules::CCheckpointModuleNatives * g_pCheckpointModuleNatives;
Modules::CObjectModuleNatives     * g_pObjectModuleNatives;
Modules::CPickupModuleNatives     * g_pPickupModuleNatives;
Modules::CPlayerModuleNatives     * g_pPlayerModuleNatives;
Modules::CServerModuleNatives     * g_pServerModuleNatives;
Modules::CVehicleModuleNatives    * g_pVehicleModuleNatives;
Modules::CScriptModuleNatives     * g_pScriptModuleNatives;
Modules::CAreaModuleNatives       * g_pAreaModuleNatives;
Modules::CHashModuleNatives       * g_pHashModuleNatives;
Modules::CWorldModuleNatives      * g_pWorldModuleNatives;

CServer::CServer()
{
	m_bActive = true;
	m_bShowFPS = true;
	m_ulLastFPSUpdateTime = 0;
	m_ulFrameCount = 0;
	m_ulFramesPerSecond = 0;
}

CServer::~CServer()
{

}

bool CServer::OnLoad(int argc, char ** argv)
{
	SetTitle(SERVER_TITLE);

	// Install the exception handler
	CExceptionHandler::Install();

	// Open the log file
	CLogFile::Open("ivmp-svr.log", true);

	// Open the settings file
	if(!CSettings::Open(SharedUtility::GetAbsolutePath("settings.xml"), false, false))
	{
		CLogFile::Print("Failed to open settings.xml, server will shut down in 3 seconds..");
		Sleep(3000);
		return false;
	}

	// Parse the command line
	CSettings::ParseCommandLine(argc, argv);

	char heiphens[128];

	HEIPHEN_GEN(" " VERSION_IDENTIFIER " " OS_STRING " Server", heiphens);

#ifdef WIN32
	// Color stuff
	CONSOLE_SCREEN_BUFFER_INFO csbiScreen;
	WORD wOldColAttr;	// For input process.

	GetConsoleScreenBufferInfo((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), &csbiScreen);
	wOldColAttr = csbiScreen.wAttributes;

	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);

	// Print message to console.
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

#ifdef WIN32
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
#endif
	CLogFile::Print(" " VERSION_IDENTIFIER " " OS_STRING " Server");
	CLogFile::Print(" Copyright (C) 2009-2013 IV:MP Team");
	CLogFile::Printf(" Port: %d", CVAR_GET_INTEGER("port"));
	CLogFile::Printf(" HTTP Port: %d", CVAR_GET_INTEGER("httpport"));
	CLogFile::Printf(" Query Port: %d", (CVAR_GET_INTEGER("port") + QUERY_PORT_OFFSET));

	if(CVAR_GET_STRING("hostaddress").IsNotEmpty())
		CLogFile::Printf(" Host Address: %s", CVAR_GET_STRING("hostaddress").Get());

	if(CVAR_GET_STRING("httpserver").IsNotEmpty())
		CLogFile::Printf(" HTTP Server: %s", CVAR_GET_STRING("httpserver").Get());

	CLogFile::Printf(" Max Players: %d", CVAR_GET_INTEGER("maxplayers"));

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	CLogFile::Print("====================================================================");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("====================================================================");
#endif
	//----------------------------------------------------------

	g_pEvents = new CEvents();
	g_pScriptingManager = new CScriptingManager();
	g_pClientScriptFileManager = new CClientFileManager(true);
	g_pClientResourceFileManager = new CClientFileManager(false);

	// Initialize the network module, if it fails, exit
	if(!CNetworkModule::Init())
	{
		CLogFile::Print("Failed to initialize the network module!\n Server shuts down in 3 seconds");
		Sleep(3000);
		return false;
	}

	g_pNetworkManager = new CNetworkManager();

	// Startup the network manager, if it fails exit
	if(!g_pNetworkManager->Startup(CVAR_GET_INTEGER("port"), CVAR_GET_INTEGER("maxplayers"), CVAR_GET_STRING("password"), CVAR_GET_STRING("hostaddress")))
	{
		CLogFile::Print("Failed to startup the network manager!\n Server shuts down in 3 seconds");
		Sleep(3000);
		return false;
	}

	g_pPlayerManager = new CPlayerManager();
	g_pVehicleManager = new CVehicleManager();
	g_pObjectManager = new CObjectManager();
	g_pPickupManager = new CPickupManager();
	g_pBlipManager = new CBlipManager();
	g_pActorManager = new CActorManager();
	g_pCheckpointManager = new CCheckpointManager();
	g_pModuleManager = new CModuleManager();
	g_pScriptTimerManager = new CScriptTimerManager();
	g_p3DLabelManager = new C3DLabelManager();

	g_pWebserver = new CWebServer(CVAR_GET_INTEGER("httpport"));
	g_pTime = new CTime();
	g_pTrafficLights = new CTrafficLights();

	g_pPickupModuleNatives = new Modules::CPickupModuleNatives;
	g_pActorModuleNatives = new Modules::CActorModuleNatives;
	g_pBlipModuleNatives = new Modules::CBlipModuleNatives;
	g_pCheckpointModuleNatives = new Modules::CCheckpointModuleNatives;
	g_pObjectModuleNatives = new Modules::CObjectModuleNatives;
	g_pPlayerModuleNatives = new Modules::CPlayerModuleNatives;
	g_pServerModuleNatives = new Modules::CServerModuleNatives;
	g_pVehicleModuleNatives = new Modules::CVehicleModuleNatives;
	g_pScriptModuleNatives = new Modules::CScriptModuleNatives;
	g_pAreaModuleNatives = new Modules::CAreaModuleNatives;
	g_pHashModuleNatives = new Modules::CHashModuleNatives;
	g_pWorldModuleNatives = new Modules::CWorldModuleNatives;

	std::list<String> modules = CVAR_GET_LIST("module");
	if(modules.size() > 0)
	{
#ifdef WIN32
		SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		CLogFile::Print("");
		CLogFile::Print("============ Loading Modules ===========");
		SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
		CLogFile::Print("");
		CLogFile::Print("============ Loading Modules ===========");
#endif

		for(std::list<String>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
		{
			CLogFile::Printf("Loading module %s.", iter->C_String());
			CModule * pModule = g_pModuleManager->LoadModule(iter->C_String());

			if(!pModule)
				CLogFile::Printf("Warning: Failed to load module %s.", iter->C_String());
		}

		CLogFile::Print("");
	}

	// Register the system natives
	CSystemNatives::Register(g_pScriptingManager);

	// Register the server natives
	CServerNatives::Register(g_pScriptingManager);

	// Register the world natives
	CWorldNatives::Register(g_pScriptingManager);

	// Register the player natives
	CPlayerNatives::Register(g_pScriptingManager);

	// Register the vehicle natives
	CVehicleNatives::Register(g_pScriptingManager);

	// Register the object natives
	CObjectNatives::Register(g_pScriptingManager);

	// Register the blip natives
	CBlipNatives::Register(g_pScriptingManager);

	// Register the actor natives
	CActorNatives::Register(g_pScriptingManager);

	// Register the checkpoint natives
	CCheckpointNatives::Register(g_pScriptingManager);

	// Register the pickup natives
	RegisterPickupNatives(g_pScriptingManager);

	// Register the area natives
	CAreaNatives::Register(g_pScriptingManager);

	// Register the hash natives
	CHashNatives::Register(g_pScriptingManager);

	// Register the event natives
	CEventNatives::Register(g_pScriptingManager);

	// Register the 3dlabel natives
	_3DLabelNatives::Register(g_pScriptingManager);

	// Register the script natives
	RegisterScriptNatives(g_pScriptingManager);

	// Register the SQLite natives
	RegisterSQLiteNatives(g_pScriptingManager);

	// Register the XML natives
	RegisterXMLNatives(g_pScriptingManager);

	// Register the timer natives
	RegisterTimerNatives(g_pScriptingManager);

	// Register the default constants
	g_pScriptingManager->RegisterDefaultConstants();

	g_ulStartTick = SharedUtility::GetTime();

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	CLogFile::Print("");
	CLogFile::Print("============ Loading Resources ===========");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("============ Loading Resources ===========");
#endif

	if(CVAR_GET_BOOL("query"))
		g_pQuery = new CQuery(CVAR_GET_INTEGER("port"), CVAR_GET_STRING("hostaddress"));

	int iResourcesLoaded = 0;
	int iFailedResources = 0;

	std::list<String> scripts = CVAR_GET_LIST("script");
	for(std::list<String>::iterator iter = scripts.begin(); iter != scripts.end(); iter++)
	{
		String strPath(SharedUtility::GetAbsolutePath("scripts/%s", (*iter).Get()));

		if(!g_pScriptingManager->Load(*iter, strPath))
		{
			CLogFile::Printf("Warning: Failed to load script %s.", (*iter).Get());
			iFailedResources++;
		}
		else
			iResourcesLoaded++;
	}

	std::list<String> clientscripts = CVAR_GET_LIST("clientscript");
	for(std::list<String>::iterator iter = clientscripts.begin(); iter != clientscripts.end(); iter++)
	{
		if(!g_pClientScriptFileManager->Start(*iter))
		{
			CLogFile::Printf("Warning: Failed to load client script %s.", (*iter).Get());
			iFailedResources++;
		}
		else
			iResourcesLoaded++;
	}

	std::list<String> clientresources = CVAR_GET_LIST("clientresource");
	for(std::list<String>::iterator iter = clientresources.begin(); iter != clientresources.end(); iter++)
	{	
		if(!g_pClientResourceFileManager->Start(*iter))
		{
			CLogFile::Printf("Warning: Failed to load client resource %s.", (*iter).Get());
			iFailedResources++;
		}
		else
			iResourcesLoaded++;
	}

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	CLogFile::Printf("Successfully loaded %d resources (%d failed).", iResourcesLoaded, iFailedResources);
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Printf("Successfully loaded %d resources (%d failed).", iResourcesLoaded, iFailedResources);
#endif

	if(CVAR_GET_BOOL("listed"))
		g_pMasterList = new CMasterList(MASTERLIST_ADDRESS, MASTERLIST_VERSION, MASTERLIST_TIMEOUT, CVAR_GET_INTEGER("port"));

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	CLogFile::Print("");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	CLogFile::Print("");
#endif
	return true;
}

void CServer::Process()
{
	// Get the current time
	unsigned long ulTime = SharedUtility::GetTime();

	// Is show fps enabled?
	if(m_bShowFPS)
	{
		// Get the amount of time elapsed since the last fps update
		unsigned long ulTimeElapsed = (ulTime - m_ulLastFPSUpdateTime);

		// Update the fps if a second or more has passed
		if(ulTimeElapsed >= 1000)
		{
			// Set the fps
			m_ulFramesPerSecond = m_ulFrameCount;

			// Set the server title
#ifdef WIN32
			String strTitle(SERVER_TITLE " - FPS: %d", m_ulFramesPerSecond);
			SetTitle(strTitle.C_String());
#endif

			// Reset the frame count
			m_ulFrameCount = 0;

			// Set the last fps update tick count
			m_ulLastFPSUpdateTime = ulTime;
		}

		// Increment the frame count
		m_ulFrameCount++;
	}

	g_pNetworkManager->Process();

	g_pVehicleManager->Process();

	if(g_pQuery)
		g_pQuery->Process();

	if(g_pMasterList)
		g_pMasterList->Pulse();

	g_pScriptTimerManager->Pulse();
	g_pModuleManager->Pulse();

	if(CVAR_GET_BOOL("frequentevents"))
		g_pEvents->Call("serverPulse");

	// Process the input queue
	ProcessInputQueue();
}

void CServer::OnUnload()
{
	// Unload all loaded scripts
	g_pScriptingManager->UnloadAll();

	CLogFile::Print(" ===== IV:MP Server shutting down. ===== ");

	SAFE_DELETE(g_pMasterList);
	SAFE_DELETE(g_pQuery);
	SAFE_DELETE(g_pScriptTimerManager);
	SAFE_DELETE(g_pModuleManager);
	SAFE_DELETE(g_pCheckpointManager);
	SAFE_DELETE(g_pPickupManager);
	SAFE_DELETE(g_pObjectManager);
	SAFE_DELETE(g_pBlipManager);
	SAFE_DELETE(g_pActorManager);
	SAFE_DELETE(g_pVehicleManager);
	SAFE_DELETE(g_pPlayerManager);
	SAFE_DELETE(g_pNetworkManager);
	CNetworkModule::Shutdown();
	SAFE_DELETE(g_pClientResourceFileManager);
	SAFE_DELETE(g_pClientScriptFileManager);
	SAFE_DELETE(g_pScriptingManager);
	SAFE_DELETE(g_pWebserver);
	SAFE_DELETE(g_pTime);
	SAFE_DELETE(g_pTrafficLights);
	SAFE_DELETE(g_pEvents);
	CSettings::Close();
	CLogFile::Close();
}

void CServer::SendInput(String strInput)
{
	if(strInput.IsNotEmpty())
	{
		// Print the input to the log
		CLogFile::PrintfToFile("[INPUT] %s", strInput.Get());

		// Get the command and parameters
		size_t sSplit = strInput.Find(' ', 0);
		String strCommand = strInput.SubStr(0, sSplit++);
		String strParameters = strInput.SubStr(sSplit, (strInput.GetLength() - sSplit));

		// Is the command empty?
		if(strCommand.IsEmpty())
			return;

		if(strCommand == "say" || strCommand == "echo" || strCommand == "print")
		{
			if(strParameters.IsNotEmpty())
			{
				CBitStream bsSend;
				bsSend.Write((DWORD)0xFFFFFFAA);
				bsSend.Write(String("Server says: %s",strParameters.Get()));
				g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
				CLogFile::Printf("Server says: %s", strParameters.Get());
			}
		}
		else if(strCommand == "loadscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Loading script %s.", strParameters.Get());

				if(g_pScriptingManager->Get(strParameters))
					CLogFile::Printf("Failed to load script %s (Script is already loaded).", strParameters.Get());
				else
				{
					String strPath(SharedUtility::GetAbsolutePath("scripts/%s", strParameters.Get()));
					CSquirrel * pScript = g_pScriptingManager->Load(strParameters, strPath);

					if(!pScript)
						CLogFile::Printf("Failed to load script %s (Script does not exist/Script compilation failed).", strParameters.Get());
					else
						CLogFile::Printf("Loaded script %s.", strParameters.Get());
				}
			}
		}
		else if(strCommand == "loadclientscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Loading client script %s.", strParameters.Get());

				if(g_pClientScriptFileManager->Exists(strParameters))
					CLogFile::Printf("Failed to load client script %s (Script is already loaded).", strParameters.Get());
				else if(!g_pClientScriptFileManager->Start(strParameters))
					CLogFile::Printf("Failed to load client script %s.", strParameters.Get());
				else
					CLogFile::Printf("Loaded client script %s.", strParameters.Get());
			}
		}
		else if(strCommand == "loadresource")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Loading client resource %s.", strParameters.Get());

				if(g_pClientResourceFileManager->Exists(strParameters))
					CLogFile::Printf("Failed to load client resource %s (Resource is already loaded).", strParameters.Get());
				else if(!g_pClientResourceFileManager->Start(strParameters))
					CLogFile::Printf("Failed to load client resource %s.", strParameters.Get());
				else
					CLogFile::Printf("Loaded client resource %s.", strParameters.Get());
			}
		}
		else if(strCommand == "unloadscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Unloading script %s.", strParameters.Get());
				CSquirrel * pScript = g_pScriptingManager->Get(strParameters);

				if(pScript)
				{
					g_pScriptTimerManager->HandleScriptUnload(pScript);
					g_pScriptingManager->Unload(strParameters);
					CLogFile::Printf("Unloaded script %s.", strParameters.Get());
				}
				else
					CLogFile::Printf("Failed to unload script %s (Script is not loaded).", strParameters.Get());

			}
		}
		else if(strCommand == "unloadclientscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Unloading client script %s.", strParameters.Get());

				if(!g_pClientScriptFileManager->Exists(strParameters))
					CLogFile::Printf("Unloading to load client script %s (Script is not loaded).", strParameters.Get());
				else if(!g_pClientScriptFileManager->Stop(strParameters))
					CLogFile::Printf("Failed to unload client script %s.", strParameters.Get());
				else
					CLogFile::Printf("Unloaded client script %s.", strParameters.Get());
			}
		}
		else if(strCommand == "unloadresource")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Unloading client resource %s.", strParameters.Get());

				if(!g_pClientResourceFileManager->Exists(strParameters))
					CLogFile::Printf("Unloading to load client resource %s (Resource is not loaded).", strParameters.Get());
				else if(!g_pClientResourceFileManager->Stop(strParameters))
					CLogFile::Printf("Failed to unload client resource %s.", strParameters.Get());
				else
					CLogFile::Printf("Unloaded client resource %s.", strParameters.Get());
			}
		}
		else if(strCommand == "reloadscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Reloading script %s.", strParameters.Get());
				CSquirrel * pScript = g_pScriptingManager->Get(strParameters);

				if(pScript)
				{
					g_pScriptTimerManager->HandleScriptUnload(pScript);

					if(g_pScriptingManager->Unload(strParameters))
					{
						String strPath(SharedUtility::GetAbsolutePath("scripts/%s", strParameters.Get()));
						pScript = g_pScriptingManager->Load(strParameters, strPath);

						if(pScript)
							CLogFile::Printf("Reloaded script %s.", strParameters.Get());
						else
							CLogFile::Printf("Failed to reload script %s (Failed to load script).", strParameters.Get());
					}
					else
						CLogFile::Printf("Failed to reload script %s (Failed to unload script).", strParameters.Get());
				}
				else
					CLogFile::Printf("Failed to reload script %s (Script is not loaded).", strParameters.Get());
			}
		}
		else if(strCommand == "reloadclientscript")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Reloading client script %s.", strParameters.Get());

				if(!g_pClientScriptFileManager->Exists(strParameters))
					CLogFile::Printf("Failed to reload script %s (Script is not loaded).", strParameters.Get());
				else if(!g_pClientScriptFileManager->Restart(strParameters))
					CLogFile::Printf("Failed to reload client script %s.", strParameters.Get());
				else
					CLogFile::Printf("Reloaded client script %s.", strParameters.Get());
			}
		}
		else if(strCommand == "reloadresource")
		{
			if(strParameters.IsNotEmpty())
			{
				CLogFile::Printf("Reloading client resource %s.", strParameters.Get());

				if(!g_pClientResourceFileManager->Exists(strParameters))
					CLogFile::Printf("Failed to reload resource %s (Resource is not loaded).", strParameters.Get());
				else if(!g_pClientResourceFileManager->Restart(strParameters))
					CLogFile::Printf("Failed to reload client resource %s.", strParameters.Get());
				else
					CLogFile::Printf("Reloaded client script %s.", strParameters.Get());
			}
		}
		else if(strCommand == "scriptinfo")
		{
			int iScriptsLoaded = 0;
			int iClientScriptsLoaded = 0;

			for(std::list<CSquirrel *>::iterator iter = g_pScriptingManager->GetScriptList()->begin(); iter != g_pScriptingManager->GetScriptList()->end(); iter++)
			{
				CLogFile::Printf("Script: %s (0x%p)", (*iter)->GetName().Get(), (*iter));
				iScriptsLoaded++;
			}

			for(CClientFileManager::iterator iter = g_pClientScriptFileManager->begin(); iter != g_pClientScriptFileManager->end(); ++ iter)
			{
				CLogFile::Printf("Client Script: %s (Checksum: 0x%p)", (*iter).first.Get(), (*iter).second.GetChecksum());
				iClientScriptsLoaded++;
			}

			for(CClientFileManager::iterator iter = g_pClientResourceFileManager->begin(); iter != g_pClientResourceFileManager->end(); ++ iter)
			{
				CLogFile::Printf("Resource: %s (Checksum: 0x%p)", (*iter).first.Get(), (*iter).second.GetChecksum());
				iClientScriptsLoaded++;
			}

			CLogFile::Printf("%d script(s) and %d client script(s) loaded.", iScriptsLoaded, iClientScriptsLoaded);
		}
		else if(strCommand == "uptime")
		{
			CLogFile::Printf("Server has been online for %s.", SharedUtility::GetTimePassedFromTime(g_ulStartTick).Get());
		}
		else if(strCommand == "quit" || strCommand == "exit")
		{
			SetActive(false);
		}
		else
		{
			CSquirrelArguments pArguments;
			pArguments.push(strInput);
			g_pEvents->Call("consoleInput", &pArguments);
		}
	}
}

void CServer::AddToInputQueue(String strString)
{
	m_inputQueueMutex.Lock();
	m_inputQueue.push(strString);
	m_inputQueueMutex.Unlock();
}

void CServer::SetTitle(String strTitle)
{
#ifdef WIN32
	SetConsoleTitle(strTitle);
#endif
}

void CServer::ProcessInputQueue()
{
	if(m_inputQueueMutex.TryLock())
	{
		while(!m_inputQueue.empty())
		{
			// Get the input string
			String strInput = m_inputQueue.front();

			// Send the input
			SendInput(strInput);

			// Erase the input string from the input queue
			m_inputQueue.pop();
		}

		m_inputQueueMutex.Unlock();
	}
}
