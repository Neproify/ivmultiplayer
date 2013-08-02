/*
	* Simple Game mode script
	* IV:MP RC3
	* 
	* Basic testing gamemode, RCON/admin commands system example, 
	* deleting script vehicles on script unload (overriding native functions example),
	* iv:mp testing commands, several spawn locations, random vehicle colors, admins(RCON+console) chat,
	* PVars usage examples (/mute d & admin flag), and some other things just to show you the basics.
*/

// **** Global variables =============================================================================================
const RCON_PASSWORD = "changeme";	// Password for RCON. MUST be changed, otherwise ingame rcon login is impossible!
local SPAWN_LOCATIONS = [		 	// Pre-defined player spawn locations (x,y,z, heading)
 [ -160.861603, 1622.971680, 20.746132, -1.238948 ],
 [ -186.545914, 1605.564453, 20.894531, -1.132042 ],
 [ -178.650452, 1624.360352, 20.746187, -0.688780 ],
];
local modelSpawn = -1; // see scriptInit and playerSpawn events


// **** Events =======================================================================================================
// Triggered when script gets loaded
function onScriptInit()
{
	// Create static vehicles	
	createVehicle(50, -132.011780, 1641.562500, 19.869772, 1.107117, 356.045044, 181.004517);
	createVehicle(27, -128.552078, 1640.656128, 19.623537, 2.138916, 354.475098, 181.960266);
	createVehicle(37, -121.837975, 1641.140991, 19.010237, 2.389526, 354.595581, 178.394104);
	createVehicle(40, -101.096130, 1629.626831, 18.657572, 0.930115, 357.052246, 228.412720);
	createVehicle(93, -107.064522, 1620.649048, 19.261599, 359.712036, 4.158936, 42.878052);
	createVehicle(28, -141.508820, 1610.213379, 19.954632, 0.350220, 359.941772, 191.356384);
	createVehicle(95, -149.628662, 1625.943726, 19.971251, 359.574829, 1.169189, 4.717041);
	createVehicle(91, -143.423294, 1627.562256, 19.951933, 0.465576, 359.942993, 188.632202);
	createVehicle(105, -144.741043, 1609.551758, 19.912132, 0.629150, 359.862366, 185.592407, 65, 53, 12, 4);
	createVehicle(14, -154.549988, 1609.422852, 20.241369, 359.731567, 359.511719, 9.625427, 32, 40, 68, 104);
	createVehicle(91, -223.687286, 1662.505127, 18.369881, 1.525146, 356.955078, 178.152527, 120, 127, 64, 38);
	createVehicle(113, -412.369843, 1660.016113, 20.991337, 0.503174, 0.177185, 314.574036, 122, 120, 28, 31);
	createVehicle(118, -267.837128, 1831.023926, 0.079428, 2.539185, 358.901489, 227.599854, 74, 68, 41, 63);
	
	modelSpawn = random(0, 100); // see playerSpawn event
}
addEvent("scriptInit", onScriptInit);

// Triggered when script is being unloaded
function onScriptExit()
{
	// Delete all vehicles created by script, as our script can do this.
	// So re-loading script will not cause static gm vehicles spawned twice.
	deleteVehicles();
	// You also have to delete any other non/static objects like checkpoints and etc (tip)
}
addEvent("scriptExit", onScriptExit);

// Triggered each time administrator enters a line of text in server console (on win or via ssh/linux)
function onConsoleInput(inputText)
{
	// Execute admin command
	if(inputText.len() <= 1 || inputText.find("/") == null)
		return 0;
	
	checkAdminCommand(-1, split(inputText, " ") ); // #warning admin commands must note that playerid can be console (-1)
	return 1;
}
addEvent("consoleInput", onConsoleInput);

// Triggred when a player requests a connection to server.
// Do not use player functions with that playerid in this event!
// Return 1 to allow, 0 otherwise
function onPlayerAuth(playerid, name, ip, serial, hasModdedGameFiles)
{
	return 1;
} 
addEvent("playerAuth", onPlayerAuth);

// Triggered when a player connected (You can use player functions now for him). He will start downloading client files.
function onPlayerConnect(playerid, playerName)
{
	sendMessageToAll(format(" ** %s(%i) connected to server!", getPlayerName(playerid), playerid));

	// Generate a spawn location from array
	local playerSpawn = SPAWN_LOCATIONS[random(0, SPAWN_LOCATIONS.len()-1)]; // get random spawn location
	setPlayerSpawnLocation(playerid, playerSpawn[0], playerSpawn[1], playerSpawn[2], playerSpawn[3]);
} 

addEvent("playerConnect", onPlayerConnect);

// Triggered when a player finished clientscripts/resources download and will spawn in a second
function onPlayerJoin(playerid)
{
	sendPlayerMessage(playerid, " ** Use /help for commands list. Thank you for testing!");
} 
addEvent("playerJoin", onPlayerJoin);

// Triggered when a player spawns (or re-spawns)
function onPlayerSpawn(playerid)
{
	// Do some actions on his first spawn after connect
	if(getPVar(playerid, "FirstSpawn") != true)
	{			
		if(!isModelIdValid(++modelSpawn))
			modelSpawn = random(0, 100); // reset
		setPlayerModel(playerid, modelSpawn);
		
		setPVar(playerid, "FirstSpawn", true);
	}
	
	// Each respawn we:
	setPlayerHealth(playerid, 50);
	setPlayerArmour(playerid, 50);
	createPlayerBlip(playerid, 3); // (re)create his blip for all players

	// Give him a dirty drug money lol
	givePlayerMoney(playerid, 5);
	
	// Give him some weapons
	givePlayerWeapon(playerid, 7, 300);
	givePlayerWeapon(playerid, 12, 100);
} 
addEvent("playerSpawn", onPlayerSpawn);

// Triggered when a player disconnects
function onPlayerDisconnect(playerid, reason)
{	
	if(reason = reason == 0 ? "exited" : "crashed")
		;// crashed
	else if(getPVar(playerid, "Kick") == true)
			reason = "kicked";
	
	local msg = format(" ** %s(%i) disconnected from server (%s)", 
		getPlayerName(playerid), playerid, reason);
	sendMessageToAll(msg);
	print(msg);
	deletePlayerBlip(playerid);
} 
addEvent("playerDisconnect", onPlayerDisconnect);

// Triggered when a player send a command (text starts with '/' symbol)
function onPlayerCommand(playerid, commandText)
{
	// Split a string with full command to array by whitespaces
	local cmd = split(commandText, " ");
	cmd[0] = cmd[0].tolower();
	
	if(checkPlayerCommand(playerid, cmd) == true)
		return 1;
	else if(getPVar(playerid, "ADMIN") == true
		&& checkAdminCommand(playerid, cmd))
		return 1;
	else
		sendPlayerMessage(playerid, " ** Command is not recognised; or you have no access to it.");
	return 0;
} 
addEvent("playerCommand", onPlayerCommand);

// Triggered when player send a chat message
// Return 1 to allow, 0 otherwise
function onPlayerText(playerid, chatText)
{
	// Simply do not allow chat messages fron muted playes:
	local muted = (getPVar(playerid, "MUTED") == true);
	if(muted)
	{
		sendPlayerMessage(playerid, " ** You cannot use chat (muted by admin) currently. Sorry.");
		return 0;
	}
	return 1;
} 
addEvent("playerText", onPlayerText);

// Triggered when a player done entering a vehicle
function onVehicleEntryComplete(playerid, vehicleid, seatid)
{
	// If he's entered as a driver, launch vehicle's engine
	if(seatid == 0 && getVehicleEngineState(vehicleid) == false)
		setVehicleEngineState(vehicleid, true);
} 
addEvent("vehicleEntryComplete", onVehicleEntryComplete);



// **** Functions ====================================================================================================
// Generates a pseudo-random number in specified range
function random(min = 0, max = RAND_MAX)
{
   srand(getTickCount() * rand());
   return (rand() % ((max + 1) - min)) + min;
}

// Returns player id (or INVALID_PLAYER_ID) by specified text expression - playerid or name part
// expression is string passed by player with command, for example
function findPlayer(expression)
{
	try
	{
		// Firstly check if it is playerid in string 
		local playerid = expression.tointeger();
		if( isPlayerConnected(playerid) )
			return playerid;
	}
	catch(ex)
	{
		// It must be the part of the name of some player
		expression = expression.tolower();
		for(local playerid = 0; playerid < MAX_PLAYERS; playerid++)
			if(isPlayerConnected(playerid) 
				&& getPlayerName(playerid).tolower().find(expression) != null)
				return playerid;
	}
	
	return INVALID_PLAYER_ID;
}

// Function for normal players commands only
// Return true if command is known
function checkPlayerCommand(playerid, cmd)
{
	switch(cmd[0])
	{
		// Rcon login command (needed before player will be able to use any of admin commands)
		case "/alogin":
		case "/rcon_login":
			if(cmd.len() >= 2)
			{
				if(getPVar(playerid, "ADMIN") == true)
					sendPlayerMessage(playerid, " ** You are already logged in as administrator.");									
				else if(buildString(cmd, 1) != RCON_PASSWORD)
					sendPlayerMessage(playerid, " ** Access denied.");
				else if(RCON_PASSWORD == "changeme")
					sendPlayerMessage(playerid, " ** Cannot login with default password 'changeme'.");
				else
				{
					setPVar(playerid, "ADMIN", true);
					sendPlayerMessage(playerid, " ** Access granted, welcome aboard.");
					textToAdmins(format(" ** Admin %s(%i) logged to RCON.", getPlayerName(playerid), playerid));
				}
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /alogin [playerid or name part]");
			break;
			
			
		// ** IV:MP Testing commands (public access, some of them can be moved to admin commands function for real game mode)
		case "/weather":
			// Change server weather command
			if(cmd.len() > 1)
			{
				local weather = cmd[1].tointeger();
				if(isWeatherIdValid(weather))
				{
					setWeather(weather);
					sendPlayerMessage(playerid, " ** Weather set to " + getWeatherName(weather));
				}
				else
					sendPlayerMessage(playerid, " ** Specified weather id is not valid: " + cmd[1]);
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /weather [weather_id]");
			break;
			
		case "/v":
		case "/veh":
			if(cmd.len() > 1)
			{
				local modelid = cmd[1].tointeger();
				if(isVehicleModelIdValid(modelid))
				{
					if(isPlayerInAnyVehicle(playerid))
					{
						if(getPlayerSeatId(playerid) == 0)
							deleteVehicle(getPlayerVehicleId(playerid));
						else
						{
							sendPlayerMessage(playerid, " ** Exit vehicle you are currently in as passanger, or enter as a driver.");
							break;							
						}
					}
				
					local pos = getPlayerCoordinates(playerid);
					local vehicleid = createVehicle(modelid, pos[0], pos[1], pos[2], 
						0, 0, getPlayerHeading(playerid));
					
					if(vehicleid != INVALID_VEHICLE_ID)
					{
						warpPlayerIntoVehicle(playerid, vehicleid, 0);
						sendPlayerMessage(playerid, format(" ** %s spawned for you.", getVehicleName(modelid)) );
					}
					else
						sendPlayerMessage(playerid, " ** Cannot spawn a vehicle...");
				}
				else
					sendPlayerMessage(playerid, " ** Invalid vehicle model id.");
			}
			else
				sendPlayerMessage(playerid, " ** Usage /v(eh) [int_model]");
			break;
			
		case "/setwantedlevel":
			if(cmd.len() > 1)
			{
				local wantedLevel = cmd[1].tointeger();
				setPlayerWantedLevel(playerid, wantedLevel);
				sendPlayerMessage(playerid, " ** Fake wanted level changed ..");
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /setWantedLevel [int_wanted_level]");
			break;
			
		case "/repair":
		case "/repairvehicle":
			if(isPlayerInAnyVehicle(playerid))
			{
				repairVehicle(getPlayerVehicleId(playerid));
				sendPlayerMessage(playerid, " ** Vehicle repaired.");
			}
			else
				sendPlayerMessage(playerid, " ** You are not in any vehicle.");
			break;
			
		case "/flip":
		case "/flipvehicle":
			if(isPlayerInAnyVehicle(playerid))
			{
				local vehicleid = getPlayerVehicleId(playerid);
				setVehicleRotation(vehicleid, 0.0, 0.0, getVehicleRotation(vehicleid)[2]);
				sendPlayerMessage(playerid, " ** Vehicle flipped.");
			}
			else
				sendPlayerMessage(playerid, " ** You are not in any vehicle.");
			break;
			
		case "/kill":
			setPlayerHealth(playerid, -1);
			sendPlayerMessage(playerid, " ** Arivederchi");
			break;
			
		case "/h":
		case "/health":
			local h = cmd.len() > 1 ? clamp(-1, 200, cmd[1].tointeger()) : 100;
			setPlayerHealth(playerid, h);
			sendPlayerMessage(playerid, " ** Health set to " + h);
			break;
			
		case "/a":
		case "/armour":
			local a = cmd.len() > 1 ? clamp(-1, 100, cmd[1].tointeger()) : 100;
			setPlayerArmour(playerid, a);
			sendPlayerMessage(playerid, " ** Armour set to " + a);
			break;
			
		case "/dim":
		case "/dimension":
			if(cmd.len() > 1)
			{
				local dim = cmd[1].tointeger();
				setPlayerDimension(playerid, dim);
				sendPlayerMessage(playerid, format(" ** You have been moved to dimension %i.", dim));
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /dimension [int_dimension_id]");
			break;
			
		case "/w":
		case "/whisper":
		case "/pm":
			if(cmd.len() > 2)
			{
				local toPlayerId = findPlayer(cmd[1]);
				if(toPlayerId == playerid)
					sendPlayerMessage(playerid, " ** You cannot whisper to yourself.");
				else if(toPlayerId != INVALID_PLAYER_ID)
				{				
					local msg = buildString(cmd, 2);
					sendPlayerMessage(playerid, format(" << %s(%i): %s", getPlayerName(toPlayerId), toPlayerId, msg));
					sendPlayerMessage(toPlayerId, format(" >> %s(%i): %s", getPlayerName(playerid), playerid, msg));
					
					if(getPVar(toPlayerId, "WHISPER_TIP") != true)
						sendPlayerMessage(toPlayerId, " ** Use /w(hisper) [player] [message ..] to send private message.");
						
					setPVar(playerid, "WHISPER_TIP", true);
				}
				else
					sendPlayerMessage(playerid, " ** Player not found.");
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /w(hisper) [player] [message ...]");
			break;
			
		case "/goto":
		case "/tp":
			if(cmd.len() > 1)
			{
				local toPlayerId = findPlayer(cmd[1]);
				//if(toPlayerId == playerid)
				//	sendPlayerMessage(playerid, " ** You cannot teleport to yourself.");
				//else
				if(toPlayerId != INVALID_PLAYER_ID)
				{		
					local pos = getPlayerCoordinates(toPlayerId);
					pos[2] += 0.4;	
					
					if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0) // if he is in a vehicle, as driver
					{					
						local vehicleid = getPlayerVehicleId(playerid);
						setVehicleCoordinates(vehicleid, pos[0], pos[1], pos[2]);
						setVehicleRotation(vehicleid, 0.0, 0.0, getPlayerHeading(toPlayerId));
					}
					else
					{
						setPlayerCoordinates(playerid, pos[0], pos[1], pos[2]);
						setPlayerHeading(playerid, getPlayerHeading(toPlayerId));
					}
					sendPlayerMessage(playerid, format(" ** You have been teleported to %s(%i)", getPlayerName(toPlayerId), toPlayerId));
				}
				else
					sendPlayerMessage(playerid, " ** Player not found.");
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /goto(/tp) [player]");
			break;
			
		case "/m":
		case "/model":
			if(cmd.len() > 1)
			{
				local m = cmd[1].tointeger();
				if(isModelIdValid(m))
				{
					setPlayerModel(playerid, m);
					sendPlayerMessage(playerid, format(" ** Model set to %i.", m));
				}
				else
					sendPlayerMessage(playerid, format(" ** %i - is not valid skin/model id.", m));
			}
			else
				sendPlayerMessage(playerid, format(" ** Your skin/model id: %i. To change model use: /m(odel) [int_model_skin_id]", getPlayerModel(playerid)));
			break;
			
		case "/wep":
		case "/weapon":
			if(cmd.len() > 1)
			{
				local wep = cmd[1].tointeger();
				local ammo = cmd.len() > 2 // if argument 2 passed, use it as ammo. If not, use 1.
					? clamp(1, 9999, cmd[2].tointeger())
					: 1;
					
				if(isWeaponModelIdValid(wep))
				{
					givePlayerWeapon(playerid, wep, ammo);
					sendPlayerMessage(playerid, " ** Take your " + getWeaponName(wep));
				}
				else
					sendPlayerMessage(playerid, format(" ** %i - is not valid weapon model id.", m));
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /wep(/weapon) [int_weapon_model] [int_ammo = 1]");
			break;
			
		case "/mypos":
			local cds = getPlayerCoordinates(playerid);
			sendPlayerMessage(playerid, format("Your coordinates in world: %f,%f,%f.", pos[0], pos[1], pos[2]));
			break;
			
		case "/engine":
			if(isPlayerInAnyVehicle(playerid))
			{
				local vehicleid = getPlayerVehicleId(playerid);
				local toggle = !getVehicleEngineState(vehicleid);
				setVehicleEngineState(vehicleid, toggle);
				sendPlayerMessage(playerid, format(" ** Vehicle engine %s.", toggle ? "launched" : "stopped"));
			}
			else
				sendPlayerMessage(playerid, " ** You are not in any vehicle.");
			break;
			
		case "/help":
			local page = cmd.len() > 1 ? cmd[1].tointeger() : 1;//by default it is first page
			switch(page)
			{
				case 0:
				case 1:
					sendPlayerMessage(playerid, " ** /help may have several pages, use < /help [int_page] >");
					sendPlayerMessage(playerid, " ** /weather, /setWantedLevel, /v(eh), /repair(vehicle), /kill");
					sendPlayerMessage(playerid, " ** /w(hisper), /dim(ension), /goto(/tp), /myBlip, /h(ealth), /a(rmour)");
					sendPlayerMessage(playerid, " ** /vehAlarm, /useMobile(Phone), /vehVar(iation), /vehName, /engine, /wep(/weapon)");
					sendPlayerMessage(playerid, " ** /vehTaxi(Lights), /label, /cp(/checkpoint), /blip, /object, /m(odel)");
					sendPlayerMessage(playerid, " ** /mypos, /print, /global, /exp(losion), /explode, /flip(Vehicle)");
					break;				
				default:
					sendPlayerMessage(playerid, " ** We don't have requested help page.");
					break;
			}
			break;
		// ** ......................................
	
	
	
		default:		// this command is not known
			return false;
	}
	
	return true; // command is recognized (break from some case).
}

// Function for admin commands only.
// Return true if command is known.
function checkAdminCommand(playerid, cmd)
{		
	local fromConsole = (playerid == -1); // -1 is passed as playerid if it is input from console
	switch(cmd[0])
	{
		// Mute / unmute
		case "/mute":
			if(cmd.len() >= 2)
			{
				local toPlayerId = findPlayer(cmd[1]);
				if(toPlayerId == INVALID_PLAYER_ID)
					sendPlayerMessage(playerid, " ** Script cannot find this player.");
				else
				{
					setPVar(toPlayerId, "MUTED", true);
					sendPlayerMessage(playerid, format(" ** Player %s(%i) muted", getPlayerName(toPlayerId), toPlayerId));
				}
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /mute [playerid or name part]");
			break;		
		case "/unmute":
			if(cmd.len() >= 2)
			{
				local toPlayerId = findPlayer(cmd[1]);
				if(toPlayerId == INVALID_PLAYER_ID)
					sendPlayerMessage(playerid, " ** Script cannot find this player.");
				else
				{
					setPVar(toPlayerId, "MUTED", false);
					sendPlayerMessage(playerid, format(" ** Player %s(%i) unmuted", getPlayerName(toPlayerId), toPlayerId));
				}
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /unmute [playerid or name part]");
			break;
			
		// Execute code from string
		case "/runcode":
			if(cmd.len() > 1)
			{
				local code = buildString(cmd, 1);				
				try { sendPlayerMessage(playerid, " ** Return: " + runCode(code)); }
				catch(ex) { sendPlayerMessage(playerid, " ** Error: " + ex); }
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /runCode [squirrel code]");
			break;
			
		case "/ban":
		case "/banplayer":
			if(cmd.len() > 1)
			{
				local playerid = findPlayer(cmd[1]);
				if(playerid != INVALID_PLAYER_ID)
				{
					local reason = cmd.len() > 2 ? buildString(cmd, 2) : "No reason message";
					local time = 40; // ban time in minutes (see banPlayer usage)
					sendMessageToAll(format(" ** %s(%i) banned by admin for %i minutes: %s", getPlayerName(playerid), playerid, time, reason));
					setPVar(playerid, "Kick", true); // see playerDisconnect event
					banPlayer(playerid, time * 60000);
				}
				else
					sendPlayerMessage(playerid, " ** Player not found.");
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /ban(Player) [player] [reason ...]");
			break;
			
		case "/kick":
		case "/kickplayer":
			if(cmd.len() > 1)
			{
				local playerid = findPlayer(cmd[1]);
				if(playerid != INVALID_PLAYER_ID)
				{
					local reason = cmd.len() > 2 ? buildString(cmd, 2) : "No reason message";
					sendMessageToAll(format(" ** %s(%i) kicked by admin: %s", getPlayerName(playerid), playerid, reason));
					setPVar(playerid, "Kick", true); // see playerDisconnect event
					kickPlayer(playerid, true);
				}
				else
					sendPlayerMessage(playerid, " ** Player not found.");
			}
			else
				sendPlayerMessage(playerid, " ** Usage: /kick(Player) [player] [reason ...]");
			break;
			
		// Chat channel only for admins
		case "/achat":
			local msg = buildString(cmd, 1);
			textToAdmins(format(" ** Admin %s(%i): %s", getPlayerName(playerid), playerid, msg));
			break;
	
		default:		// this command is not known
			return false;
	}
	
	return true; // command recognized, break; used
}

// Checks if player model id specified is valid for IV:MP
function isModelIdValid(modelid)
{
	return modelid >= 0 && modelid <= 345 && modelid != 1 && modelid != 2 && modelid != 3;
}

// Checks if weapon model id specified is valid for IV:MP
function isWeaponModelIdValid(modelid)
{
	return modelid > 0 && modelid <= 18 && modelid != 6;
}

// Checks if vehicle model id specified is valid for IV:MP
function isVehicleModelIdValid(modelid)
{
	return modelid >= 0 && modelid < 124 && modelid != 111 && modelid != 96 && modelid != 41 && modelid != 107;
}

// Checks if weather id specified is valid for IV:MP
function isWeatherIdValid(id)
{
	return id > 0 && id < 10;
}

// Returns a weather name by weather id
function getWeatherName(weatherid)
{
	switch(weatherid)
	{
		case 1: return "Extra sunny";
		case 2: return "Sunny";
		case 3: return "Sunny and windy";
		case 4: return "Cloudy";
		case 5: return "Raining";
		case 6: return "Drizzle";
		case 7: return "Foggy";
		case 8: return "Thunderstorm";
		case 9: return "Extra sunny 2";
		case 10:return "Sunny and windy 2";
		
		default: return "Not supported";
	}
}

// Send a message to all admins
function textToAdmins(msg)
{
	for(local i = -1; i < MAX_PLAYERS; i++)
		if(i == -1 || getPVar(i, "ADMIN") == true)
			sendPlayerMessage(i, msg);
	print(" :: Admins chat :: " + msg);
}

// Runcode function, needed for execute squirrel code from string only
function runCode(code)
{
	local scriptBuffer;
	try { scriptBuffer = compilestring("return " + code); }
	catch(ex) { scriptBuffer = compilestring(code); }

	if(scriptBuffer)
		return scriptBuffer();
	else
		throw "!scriptBuffer";
}

// Simply build string from array of strings
function buildString(ar, skip, sep = " ")
{
	local ss = "";
	for(local i = skip; i < ar.len(); i++)
		ss += ar[i] + (i != ar.len() - 1 ? sep : "");
	return ss;
}

// Simply generate random color for car
function generateVehicleColor()
{
	// TODO: May be improved if needed..
	return [ random(0, 136), random(0, 136), random(0, 136), random(0, 136) ];
}

// Well-known function
function clamp(min, max, check)
{
	return check >= min && check <= max ? check : (check < min ? min : max);
}



// **** Hooking functions & etc ====================================================================================================

// Overriding native createVehicle function to store all vehicles created by script in array
// and be able to delete all vehicles on script un(re)load.

local vehicles = {}; 					// a table where vehicle IDs will be stored
local _createVehicle = createVehicle;	// Store original function pointer
createVehicle = 						// overriding function (which will be used by script)
	function (model, x,y,z, rx,ry,rz, c1=-1,c2=-1,c3=-1,c4=-1, respawn_delay = -1)
	{
		// Our function will allow to generate random colors, however this will not work so cool.
		if(c1 < 0)
		{
			local colors = generateVehicleColor();
			c1 = colors[0];
			c2 = colors[1];
			c3 = colors[2];
			c4 = colors[3];
		}
	
		// Using base (original) function to create vehicle
		local vehicleid = _createVehicle(model, x, y, z, rx, ry, rz, c1, c2, c3, c4, respawn_delay);
	
		// Store vehicle id in vehicles array:
		if(vehicleid != INVALID_VEHICLE_ID)
			vehicles[vehicles.len()] <- vehicleid;
		return vehicleid;
	}
	
// Function to delete any vehicles created by script (actually using overriden function)
function deleteVehicles()
{	
	for(local i = vehicles.len() - 1; i >= 0; i--)
		deleteVehicle(vehicles[i]); // we don't care if it was deleted before, this will not cause error.
	vehicles.clear();
}

// Overriding sendPlayerMessage native:
// This will allow to use -1 playerid for console print, for RCON commands (which can be  used by players and console)
local _sendPlayerMessage = sendPlayerMessage;
function sendPlayerMessage(playerid, message, color = 0xFFFFFFFF, allowTextFormatting = false)
{
	if(playerid == -1)
		return print(" :: Terminal: " + message);
	else
		return _sendPlayerMessage(playerid, message, color, allowTextFormatting);
}