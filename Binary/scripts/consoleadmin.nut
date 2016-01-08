// NOTE: Script will not work without changing the password below
const CONSOLE_PASSWORD = "changeme";

const White = 0xFFFFFFAA;

// TODO: Console login events

local ConsoleAdmin = array(MAX_PLAYERS, 0);
local RecieveInput = array(MAX_PLAYERS, 0);

function onScriptInit()
{
	if(CONSOLE_PASSWORD == "changeme")
	{
		print("WARNING: The console password is set to default ('changeme'). You will have to change the console password before you are able to use this script.");
	}
}
addEvent("scriptInit", onScriptInit);

function onPlayerConnect(playerid)
{
	ConsoleAdmin[playerid] = 0;
	RecieveInput[playerid] = 0;
	return 1;
}
addEvent("playerConnect", onPlayerConnect);

function onPlayerCommand(playerid, command)
{
	local cmd = split(command, " ");
	if(cmd[0] == "/console")
	{
		if(cmd.len() == 3 && cmd[1] == "login" && !ConsoleAdmin[playerid])
		{
			if(cmd[2] == CONSOLE_PASSWORD && CONSOLE_PASSWORD != "changeme")
			{
				ConsoleAdmin[playerid] = 1;
				sendPlayerMessage(playerid, "CONSOLE: Successfully logged in.");	
				log(getPlayerName(playerid) + " (" + playerid + ", " + getPlayerIp(playerid) + ") logged in to the console.");
				return 1;
			}
		}
		else if(cmd.len() == 2 && cmd[1] == "logout" && ConsoleAdmin[playerid])
		{
			ConsoleAdmin[playerid] = 0;
			sendPlayerMessage(playerid, "CONSOLE: Successfully logged out.");
			log(getPlayerName(playerid) + " (" + playerid + ", " + getPlayerIp(playerid) + ") logged out of the console.");
			return 1;
		}
		if(ConsoleAdmin[playerid])
		{
			local consolecommand = command.slice(cmd[0].len() + 1);
			log(getPlayerName(playerid) + " (" + playerid + ", " + getPlayerIp(playerid) + ") sent command: " + consolecommand + ".");
			RecieveInput[playerid] = 1;
			sendConsoleInput(consolecommand);
			RecieveInput[playerid] = 0;
		}
	}
	return 1;
}
addEvent("playerCommand", onPlayerCommand);

function onConsoleOutput(output)
{
	for(local i = 0; i < MAX_PLAYERS; i++)
	{
		if(isPlayerConnected(i))
		{
			if(ConsoleAdmin[i] && RecieveInput[i])
			{
				sendPlayerMessage(i, "CONSOLE: " + output);
			}
		}
	}
	return 1;
}
addEvent("consoleOutput", onConsoleOutput);