/**
 * Indicators script
 *
 * Press 'Q' or 'E' to enable your vehicle's indicators
 *
 * Author: mabako
 */
addEvent("keyPress",
	function(button, state)
	{
		if(state == "down" && getPlayerVehicleId(getLocalPlayer()) != INVALID_VEHICLE_ID)
		{
			// Q - toggle left indicator
			if(button == "q")
			{
				triggerServerEvent("indicators:left");
			}
			// E - toggle right indicator
			if(button == "e")
			{
				triggerServerEvent("indicators:right")
			}
		}
	}
);
