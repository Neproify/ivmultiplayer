/*
	* Scoreboard 2.0 script (client)
	* IV:MP RC3
	* Author: CrackHD
*/

local guiScoreboard = {};
const ACTIVATION_KEY = "b"; // Key to activate the scoreboard.

// Triggered on script load
function onScriptInit()
{
	// Create window for scoreboard
	guiScoreboard.window <- GUIWindow();
	guiScoreboard.window.setVisible(false);
	local screen = guiGetScreenSize();
	guiScoreboard.window.setSize(600.0, 400.0, false);
	guiScoreboard.window.setPosition(screen[0]/2 - 300, screen[1]/2 -200, false); // center screen
	guiScoreboard.window.setText("Players on server");

	// Create scoreboard list:
	guiScoreboard.list <- GUIMultiColumnList();
	guiScoreboard.list.setParent(guiScoreboard.window.getName());
	guiScoreboard.list.setSize(600.0, 400.0, false);
	
	// Add columns:
	guiScoreboard.list.setProperty("ColumnHeader", "text:ID width:{0.1,0} id:0");
	guiScoreboard.list.setProperty("ColumnHeader", "text:Name width:{0.49,0} id:1 ");
	guiScoreboard.list.setProperty("ColumnHeader", "text:Ping width:{0.1,0} id:2");
	guiScoreboard.list.setProperty("ColumnHeader", "text:Score width:{0.1,0} id:3");
	guiScoreboard.list.setProperty("ColumnHeader", "text:Money width:{0.1,0} id:4");
	guiScoreboard.list.setProperty("ColumnHeader", "text:Health width:{0.1,0} id:5");
	
	// Start update timer (tick):
												if(guiScoreboard.window.getVisible != null) // DEV
	timer(onTick, 1000, -1);
}
addEvent("scriptInit", onScriptInit);

// Triggered on script unload
function onScriptExit()
{
	// Delete gui...
	guiScoreboard.window.setVisible(false);
}
addEvent("scriptExit", onScriptExit);

// Triggered when a player disconnected from server (triggered by server's indicators.nut!)
function onPlayerDisconnect(playerid, reason)
{
	// Remove his row from scoreboard table
	local row = ensurePlayerRow(playerid);
	guiScoreboard.list.removeRow(row);
}
addEvent("playerDisconnectEx", onPlayerDisconnect);

// Triggered when player clicks a row in a list
/*function onRowClick(guiName, rowNumber)
{
	// Ensure it is scoreboard list
	if(guiName == guiScoreboard.list.getName())
	{
		// TODO: Do something..
	}
}
addEvent("rowClick", onRowClick);*/

// Triggered when player press a key
function onKeyPress(key, state)
{
	// Show/hide our window
	if(key == ACTIVATION_KEY)
	{
		local bActive = (state == "down");		
		
		guiScoreboard.window.setVisible(bActive);
		//guiToggleCursor(bActive);
		//togglePlayerControls(localplayerid, !bActive); on Server - TODO
		
		if(bActive)
			onTick();
	}
}
addEvent("keyPress", onKeyPress);

// Our tick event (timer)
function onTick()
{
	// If list is visible, update state
	if(guiScoreboard.window.getVisible() == false)
		return;
	
	// Simply update each player's row
	for(local i = 0; i < MAX_PLAYERS; i++)
		if(isPlayerConnected(i))
			updatePlayerRow(i);
}

// ===================================================================================================

// Update info about player in table
function updatePlayerRow(playerid)
{
	local row = ensurePlayerRow(playerid);
	guiScoreboard.list.setItem(playerid.tostring(), 0, row);
	guiScoreboard.list.setItem(getPlayerName(playerid), 1, row);
	guiScoreboard.list.setItem(getPlayerPing(playerid).tostring(), 2, row);
	guiScoreboard.list.setItem("0", 3, row);
	guiScoreboard.list.setItem(getPlayerMoney(playerid).tostring(), 4, row);
	guiScoreboard.list.setItem(getPlayerHealth(playerid).tostring(), 5, row);
}

// Make sure row for player exists, and return its row ID
function ensurePlayerRow(playerid)
{
	// Enumerate all rows and check their column#0 value == playerid
	local insert_at = 0; // index where we will insert a row for player if row doesn't exists
	for(local i = 0; i < MAX_PLAYERS; i++)
	{
		local id = guiScoreboard.list.getItem(i, 0);
		if(id == false || id.tointeger() > playerid)
		{
			// Current row is row of player which ID is bigger than playerid,
			// so we will insert a row for playerid before it.
			insert_at = i;
			break;
		}
		else if(id.tointeger() == playerid)
			return i;// return existing row id
	}
	
	// If row not found create it
	return guiScoreboard.list.addRow(insert_at);
}

