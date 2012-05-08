//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Server socket natives

#include "Main.h"
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

#define HEIPHEN_GEN(string, stringname) \
	{ \
		stringname[0] = '\0'; \
		\
		for(int i = 0; i < sizeof(string) + 1; i++) \
			strcat(stringname, "-"); \
	}

CNetworkManager    * g_pNetworkManager = NULL;
CPlayerManager     * g_pPlayerManager = NULL;
CVehicleManager    * g_pVehicleManager = NULL;
CObjectManager     * g_pObjectManager = NULL;
CBlipManager       * g_pBlipManager = NULL;
CActorManager      * g_pActorManager = NULL;
CPickupManager     * g_pPickupManager = NULL;
CCheckpointManager * g_pCheckpointManager = NULL;
CScriptingManager  * g_pScriptingManager = NULL;
CClientFileManager * g_pClientScriptFileManager = NULL;
CClientFileManager * g_pClientResourceFileManager = NULL;
CModuleManager     * g_pModuleManager = NULL;
CMasterList        * g_pMasterList = NULL;
CWebServer         * g_pWebserver = NULL;
CTime              * g_pTime = NULL;
CTrafficLights     * g_pTrafficLights = NULL;
CEvents            * g_pEvents = NULL;
unsigned long        g_ulStartTick = 0;
CMutex               consoleInputQueueMutex;
std::queue<String>   consoleInputQueue;
CQuery             * g_pQuery = NULL;

extern CScriptTimerManager * g_pScriptTimerManager;

Modules::CActorModuleNatives * g_pActorModuleNatives;
Modules::CBlipModuleNatives * g_pBlipModuleNatives;
Modules::CCheckpointModuleNatives * g_pCheckpointModuleNatives;
Modules::CObjectModuleNatives * g_pObjectModuleNatives;
Modules::CPickupModuleNatives * g_pPickupModuleNatives;
Modules::CPlayerModuleNatives * g_pPlayerModuleNatives;
Modules::CServerModuleNatives * g_pServerModuleNatives;
Modules::CVehicleModuleNatives * g_pVehicleModuleNatives;
Modules::CScriptModuleNatives * g_pScriptModuleNatives;

void SendConsoleInput(String strInput)
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
				bsSend.Write(strParameters);
				g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
				CLogFile::Print(strParameters);
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
			g_pNetworkManager->bRunning = false;
		}
		else
		{
			CSquirrelArguments pArguments;
			pArguments.push(strInput);
			g_pEvents->Call("consoleInput", &pArguments);
		}
	}
}

void InputThread(CThread * pCreator)
{
	// Buffer MUST be 3 or more (input char, \n, \0)
	char szInputBuffer[512];
	String strInputString;

	// Loop until server shutdown
	while(pCreator->GetUserData<bool>())
	{
		// Wait for input from the console
		// TODO: Make non-blocking
		fgets(szInputBuffer, sizeof(szInputBuffer), stdin);

		// Do we have anything in the input?
		if(szInputBuffer[0] != '\n')
		{
			// Append the input to the input string
			strInputString += szInputBuffer;

			// Get the input string length
			size_t sLength = strInputString.GetLength();

			// Is this the last of the input?
			if(strInputString[sLength - 1] == '\n')
			{
				// Check for CRLF
				if(strInputString[sLength - 2] == '\r')
				{
					// Replace '\r\n' with '\0'
					strInputString.Truncate(sLength - 2);
				}
				else
				{
					// Replace '\n' with '\0'
					strInputString.Truncate(sLength - 1);
				}

				// Add the input string to the console input queue
				consoleInputQueueMutex.Lock();
				consoleInputQueue.push(strInputString);
				consoleInputQueueMutex.Unlock();

				// Clear the input string
				strInputString.Clear();
			}
		}

		// Wait
		Sleep(10);
	}
}

#if 0
// test
#include <Squirrel/squirrel.h>
#include <Squirrel/sqstate.h>
#include <Squirrel/sqvm.h>
#include <Squirrel/sqtable.h>
#include <Squirrel/sqstring.h>
// test end
#endif

int main(int argc, char ** argv)
{
#ifdef WIN32
	SetConsoleTitle(VERSION_IDENTIFIER_2 " Server");
#endif

	// Install the exception handler
	CExceptionHandler::Install();

	// Open the log file
	CLogFile::Open("ivmp-svr.log", true);

	// Open the settings file
	if(!CSettings::Open(SharedUtility::GetAbsolutePath("settings.xml"), false, false))
		return 0;

	// Parse the command line
	CSettings::ParseCommandLine(argc, argv);

	char heiphens[128];

	HEIPHEN_GEN(" " VERSION_IDENTIFIER " " OS_STRING " Server", heiphens);

	//----------------------------------------------------------
	CLogFile::Print("");
	CLogFile::Print(heiphens);
	CLogFile::Print(" " VERSION_IDENTIFIER " " OS_STRING " Server");
	CLogFile::Print(" Copyright (C) 2009-2012 IV:MP Team");
	CLogFile::Printf(" Port: %d", CVAR_GET_INTEGER("port"));
	CLogFile::Printf(" HTTP Port: %d", CVAR_GET_INTEGER("httpport"));
	CLogFile::Printf(" Query Port: %d", (CVAR_GET_INTEGER("port") + QUERY_PORT_OFFSET));

	if(CVAR_GET_STRING("hostaddress").IsNotEmpty())
		CLogFile::Printf(" Host Address: %s", CVAR_GET_STRING("hostaddress").Get());

	if(CVAR_GET_STRING("httpserver").IsNotEmpty())
		CLogFile::Printf(" HTTP Server: %s", CVAR_GET_STRING("httpserver").Get());

	CLogFile::Printf(" Max Players: %d", CVAR_GET_INTEGER("maxplayers"));
	CLogFile::Print(heiphens);
	CLogFile::Print("");
	//----------------------------------------------------------

	g_pEvents = new CEvents();
	g_pScriptingManager = new CScriptingManager();
	g_pClientScriptFileManager = new CClientFileManager(true);
	g_pClientResourceFileManager = new CClientFileManager(false);

	// Initialize the network module, if it fails, exit
	if(!CNetworkModule::Init())
	{
		CLogFile::Print("Failed to initialize the network module!\n");
		return 1;
	}

	g_pNetworkManager = new CNetworkManager();

	// Startup the network manager, if it fails exit
	if(!g_pNetworkManager->Startup(CVAR_GET_INTEGER("port"), CVAR_GET_INTEGER("maxplayers"), CVAR_GET_STRING("password"), CVAR_GET_STRING("hostaddress")))
	{
		CLogFile::Print("Failed to startup the network manager!\n");
		return 1;
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

	std::list<String> modules = CVAR_GET_LIST("module");
	if(modules.size() > 0)
	{
		CLogFile::Print("Loading modules");
		CLogFile::Print("----------------");

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

	CLogFile::Print("Loading resources");
	CLogFile::Print("------------------");
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

	CLogFile::Printf("Successfully loaded %d resources (%d failed).", iResourcesLoaded, iFailedResources);
	CLogFile::Print("------------------");

	// Start the input thread
	CThread inputThread;
	inputThread.SetUserData<bool>(true);
	inputThread.Start(InputThread);

	if(CVAR_GET_BOOL("query"))
		g_pQuery = new CQuery(CVAR_GET_INTEGER("port"), CVAR_GET_STRING("hostaddress"));

	if(CVAR_GET_BOOL("listed"))
		g_pMasterList = new CMasterList(MASTERLIST_ADDRESS, MASTERLIST_VERSION, MASTERLIST_TIMEOUT, CVAR_GET_INTEGER("port"));

	CLogFile::Printf("Server started.");

#if 0
	// test
	// Open a temporary VM
	SQVM * pVM = sq_open(1024);

	// Create the exports table
	SQTable * pTable = SQTable::Create(_ss(pVM), 0);

	// Close the temporary VM
	//sq_close(pVM);

	// Create the exports table SQObject
	SQObject value;
	value._type = OT_TABLE;
	value._unVal.pTable = pTable;

	for(std::list<CSquirrel *>::iterator iter = g_pScriptingManager->GetScriptList()->begin(); iter != g_pScriptingManager->GetScriptList()->end(); iter++)
	{
		// Get the VM pointer for this script
		SQVM * pVM = (*iter)->GetVM();

		// Push the root table onto the stack
		sq_pushroottable(pVM);

		// Push the table name onto the stack
		sq_pushstring(pVM, "exports",-1);

		// Push the exports table onto the stack
		sq_pushobject(pVM, value);

		// Create the new root table slot
		sq_newslot(pVM, -3, true);

		// Pop the root table from the stack
		sq_pop(pVM, 1);
	}
	// end test
#endif

	while(g_pNetworkManager->bRunning)
	{
		g_pNetworkManager->Process();

		if(g_pQuery)
			g_pQuery->Process();

		if(g_pMasterList)
			g_pMasterList->Pulse();

		g_pScriptTimerManager->Pulse();
		g_pModuleManager->Pulse();

		if(CVAR_GET_BOOL("frequentevents"))
			g_pEvents->Call("serverPulse");

		// Try and lock the console input queue mutex
		if(consoleInputQueueMutex.TryLock(0))
		{
			// Process the console input queue
			while(!consoleInputQueue.empty())
			{
				SendConsoleInput(consoleInputQueue.back().GetData());
				consoleInputQueue.pop();
			}

			// Unlock the console input queue mutex
			consoleInputQueueMutex.Unlock();
		}

		Sleep(5);
	}

	// Stop the input thread
	inputThread.SetUserData<bool>(true);
	inputThread.Stop(false, true);

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
	// HACK: To fix input thread not ending
#ifdef WIN32
	TerminateProcess(GetCurrentProcess(), 0);
#endif
	return 0;
}
