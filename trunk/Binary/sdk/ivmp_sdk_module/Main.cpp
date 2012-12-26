//============== Copyright © 2012 IV:MP Team. All rights reserved. ==============
// File: Main.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

const char * m_szModuleName = "Test Module";

/*
	This function is called when the module was loaded,
	events should be added here.

	Return
		true: Show a message confirming the module was loaded
		false: Don't show a message confirming the module was loaded
*/
EXPORT bool InitModule(char * szModuleName)
{
	strcpy(szModuleName, "Test Module");
	
	IVMP::Events::Manager()->AddModuleEvent("playerSpawn", event_playerSpawn);

	int slots = IVMP::Server()->GetPlayerSlots();
	LogPrintf("[%s] maximum players online: %i/%i", szModuleName, slots, MAX_PLAYERS);

	LogPrintf("[%s] Our module got loaded!", m_szModuleName);
	return true;
}

/*
	This function is called when a some script is loaded.
*/
EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
	// Register your scripting functions here
	RegisterFunction(pVM, "helloWorld", sq_helloworld);

	LogPrintf("[%s] A script got loaded!", m_szModuleName);
}

/*
	This function is called when a some script is unloaded.
*/
EXPORT void ScriptUnload(HSQUIRRELVM pVM)
{
	LogPrintf("[%s] A script got unloaded!", m_szModuleName);
}

/*
	This function is called every server pulse.
	It is not recommended to run CPU-intensive code here.
*/
EXPORT void Pulse()
{
}

/*
	Example simple event registered in InitModule
*/

void event_playerSpawn(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk)
{
	// You can see event call arguments and return on IV:MP wiki
	// http://wiki.iv-multiplayer.com

	EntityId playerId = pArguments->Get(0)->GetInteger();

	LogPrintf("[%s] %s(%i) spawned...", m_szModuleName, IVMP::Players()->GetName(playerId), playerId);
	IVMP::Players()->SendMessage(playerId, "hello from module", 0, false);	

	pReturn->SetInteger(1);
}