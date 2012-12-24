//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Main.cpp
//===============================================================================

#include "SDK/SDK.h"
#include "Functions.h"

// Name of our module
const char * m_szModuleName = "Test Module";

/*
	Example Event to show when a player connects
*/
void playerConnect(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk)
{
	if(pArguments->GetSize() >= 1 && pArguments->Get(0)->GetType() == OT_INTEGER)
	{
		int playerId = pArguments->Get(0)->GetInteger();

		printf("Player [%d] %s connected :)\n", playerId, IVMP::Players()->GetName(playerId));
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

	printf("[%s] Our module got loaded!\n", m_szModuleName);
	IVMP::Events::Manager()->AddModuleEvent("playerConnect", playerConnect);
	return true;
}

/*
	This function is called when a script is loaded.
*/


EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
	// Register your scripting functions here
	RegisterFunction(pVM, "helloWorld", sq_helloworld);

	IVMP::Vehicles()->Create(65, CVector3(-343.447662f, 1176.119263f, 14.146016f), CVector3(0.0f, 0.0f, 268.219513f), 0, 0, 0, 0, -1);

	printf("[%s] A script got loaded!\n", m_szModuleName);
}

/*
	This function is called when a script is unloaded.
*/
EXPORT void ScriptUnload(HSQUIRRELVM pVM)
{
	printf("[%s] A script got unloaded!\n", m_szModuleName);
}

/*
	This function is called every server pulse.
	It is not recommended to run CPU-intensive code here.
*/
EXPORT void Pulse()
{
	
}