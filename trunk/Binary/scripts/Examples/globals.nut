// Globals are global variables
// PVars are global variables for players

// Global variable can be set, removed or accesed from any script,
// Types supported: int, string, float, bool, table, array
// Use of other types may cause crash.

// Globals usage example:
// Script 1.nut:
	setGlobal("test", 500);
// Script 2.nut
	if(isGlobalSet("test"))
	{
		print("Test global value: " + getGlobal("test"));
		removeGlobal("test");
	}
	else
		print("Global 'test' not set");
		

// PVars usage example:
// Script 1.nut:
	// Note: player must be connected to server, otherwise pVars will not work for him
	// After player has disconnected, all his PVars removed
	setPVar(playerid, "test", 500);
// Script 2.nut
	if(isPVarSet(playerid, "test"))
	{
		print("Test PVar value: " + getPVar(playerid, "test"));
		removePVar(playerid, "test");
	}
	else
		print("Player's PVar 'test' not set");