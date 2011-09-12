local config = getConfig();

if(!areFrequentEventsEnabled())
{
	log("NOTE: Frequent events are disabled, they will now be enabled to ensure this script will work.");
	toggleFrequentEvents(true);
}

function onPlayerChangePadState(playerid)
{
	local oldpad = getPlayerPreviousPadState(playerid);
	local newpad = getPlayerPadState(playerid);
	log("got sync from player " + playerid + " and keys " + newpad["keys"]);

	if(!(oldpad["keys"] & PAD_KEY_JUMP) && (newpad["keys"] & PAD_KEY_JUMP))
	{
		// jump pressed
		sendPlayerMessage(playerid, "You pressed your jump key");
	}
	if((oldpad["keys"] & PAD_KEY_JUMP) && (newpad["keys"] & PAD_KEY_JUMP))
	{
		// jump held
		sendPlayerMessage(playerid, "You are holding your jump key");
	}
	if((oldpad["keys"] & PAD_KEY_JUMP) && !(newpad["keys"] & PAD_KEY_JUMP))
	{
		// jump released
		sendPlayerMessage(playerid, "You released your jump key");
	}

	return 1;
}
addEvent("playerChangePadState", onPlayerChangePadState);