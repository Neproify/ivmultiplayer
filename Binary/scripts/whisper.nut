function onPlayerCommand(playerid, command)
{
	local cmd = split(command, " ");

	if(cmd[0] == "/w" || cmd[0] == "/whisper")
	{
		if(cmd.len() < 3)
		{
			sendPlayerMessage(playerid, "Syntax: (/w)hisper [playerid] [text]");
			return 1;
		}

		if(!isPlayerConnected(cmd[1].tointeger()))
		{
			sendPlayerMessage(playerid, "Player " + cmd[1].tointeger() + " is not connected");
			return 1;
		}

		local text = command.slice(cmd[0].len() + cmd[1].len() + 2);
		sendPlayerMessage(cmd[1].tointeger(), "Whisper from " + getPlayerName(playerid) + " [" + playerid + "]: " + text);
		sendPlayerMessage(playerid, "Whisper sent to " + getPlayerName(cmd[1].tointeger()) + " [" + cmd[1].tointeger() + "]: " + text);
		log("[Whisper] " + getPlayerName(cmd[1].tointeger()) + " to " + getPlayerName(cmd[1].tointeger()) + ": " + text);			callEvent("playerWhisper", 1, playerid, cmd[1].tointeger(), text);
		return 1;
	}

	return 1;
}
addEvent("playerCommand", onPlayerCommand);