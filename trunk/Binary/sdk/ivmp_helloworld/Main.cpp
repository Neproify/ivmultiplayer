//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Main.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

const char * m_szModuleName = "Test Module";

/*
	Example Event to show when a player connects
*/
void playerConnect(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk)
{
	if(pArguments->GetSize() >= 1 && pArguments->Get(0)->GetType() == OT_INTEGER)
	{
		int playerId = pArguments->Get(0)->GetInteger();

		LogPrintf("Player [%d] %s connected :)", playerId, InterfaceContainer.g_pPlayerManager->GetAt(playerId)->GetNameConst());
	}
}

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

	LogPrintf("[%s] Our module got loaded!", m_szModuleName);
	InterfaceContainer.g_pEvents->AddModuleEvent("playerConnect", playerConnect);
	return true;
}

/*
	This function is called when a script is loaded.
*/
EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
	// Register your scripting functions here
	RegisterFunction(pVM, "helloWorld", sq_helloworld);

	LogPrintf("[%s] A script got loaded!", m_szModuleName);
}

/*
	This function is called when a script is unloaded.
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