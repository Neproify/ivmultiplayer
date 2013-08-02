/*
	* Indicators & lights script (server)
	* IV:MP RC3
	* Author: mabako
*/

// Make sure the client-side script is loaded on server script load
function onScriptInit()
{
	loadClientScript("indicators_client.nut");
}
addEvent("scriptInit", onScriptInit);

// Make sure all indicators are reset on script unload
function onScriptExit()
{
	unloadClientScript("indicators_client.nut");

	for(local vehicleid = 0; vehicleid < MAX_VEHICLES; ++vehicleid)
	{
		local indicators = getVehicleIndicators(vehicleid);
		if(indicators)
		{
			if(indicators[0] || indicators[1] || indicators[2] || indicators[3])
				setVehicleIndicators(vehicleid, false, false, false, false);
		}
	}
}
addEvent("scriptExit", onScriptExit);

// ==================================================================================

function onILeft(playerid)
{
	if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
	{
		local vehicleid = getPlayerVehicleId(playerid);
		local indicators = getVehicleIndicators(vehicleid);
		setVehicleIndicators(vehicleid, true, false, true, false);
	}
}
addEvent("indicators:left", onILeft);

function onIRight(playerid)
{
	if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
	{
		local vehicleid = getPlayerVehicleId(playerid);
		local indicators = getVehicleIndicators(vehicleid);
		setVehicleIndicators(vehicleid, false, true, false, true);
	}
}
addEvent("indicators:right", onIRight);

function onIOff(playerid)
{
	if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
	{
		local vehicleid = getPlayerVehicleId(playerid);
		setVehicleIndicators(vehicleid, false, false, false, false);
	}
}
addEvent("indicators:off", onIOff);

function onLightsToggle(playerid)
{
	if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
	{
		local vehicleid = getPlayerVehicleId(playerid);
		setVehicleLights(vehicleid, !getVehicleLights(vehicleid));
	}
}
addEvent("lights:toggle", onLightsToggle);

// ==================================================================================

function onPlayerJoin(playerid)
{
	for(local i = 0; i < MAX_PLAYERS; i++)
		if(i != playerid && isPlayerConnected(playerid))
			triggerClientEvent(i, "playerConnectEx", playerid);
}
addEvent("playerJoin", onPlayerJoin);

function onPlayerDisconnect(playerid, reason)
{
	for(local i = 0; i < MAX_PLAYERS; i++)
		if(i != playerid && isPlayerConnected(playerid))
			triggerClientEvent(i, "playerDisconnectEx", playerid, reason);
}
addEvent("playerDisconnect", onPlayerDisconnect);