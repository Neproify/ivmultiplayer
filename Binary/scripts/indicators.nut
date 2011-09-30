/**
 * Indicators script
 *
 * Server-side part that handles all events.
 *
 * Author: mabako
 */

addEvent("indicators:left",
	function(playerid)
	{
		if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
		{
			local vehicleid = getPlayerVehicleId(playerid);
			local indicators = getVehicleIndicators(vehicleid);
			setVehicleIndicators(vehicleid, true, false, true, false);
		}
	}
);

addEvent("indicators:right",
	function(playerid)
	{
		if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
		{
			local vehicleid = getPlayerVehicleId(playerid);
			local indicators = getVehicleIndicators(vehicleid);
			setVehicleIndicators(vehicleid, false, true, false, true);
		}
	}
);

addEvent("indicators:off",
	function(playerid)
	{
		if(isPlayerInAnyVehicle(playerid) && getPlayerSeatId(playerid) == 0)
		{
			local vehicleid = getPlayerVehicleId(playerid);
			setVehicleIndicators(vehicleid, false, false, false, false);
		}
	}
);

// make sure the client-side script is loaded
addEvent("scriptInit",
	function()
	{
		loadClientScript("indicators.nut");
	}
);

// make sure all indicators are unset
addEvent("scriptExit",
	function()
	{
		unloadClientScript("indicators.nut");

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
);
