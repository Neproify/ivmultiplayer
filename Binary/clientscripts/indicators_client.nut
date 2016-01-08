/*
	* Indicators & lights script (client)
	* IV:MP RC3
	* Author: mabako
*/

local waitingTurn = 0;
local doneTurn = 0;

function onKeyPress(key, state)
{
	if(state != "down")
		return;
		
	switch(key)
	{
		case "q":
			// indicator already on?
			if(waitingTurn > 0)
			{
				triggerServerEvent("indicators:off");
				waitingTurn = 0;
			}
			// turn indicator on
			else
			{
				triggerServerEvent("indicators:left");
				waitingTurn = 1;
				doneTurn = 0;
			}
			break;
			
		case "e":
			// indicator already on?
			if(waitingTurn > 0)
			{
				triggerServerEvent("indicators:off");
				waitingTurn = 0;
			}
			// turn indicator on
			else
			{
				triggerServerEvent("indicators:right");
				waitingTurn = 2;
				doneTurn = 0;
			}
			break;
			
		case "l":
			triggerServerEvent("lights:toggle");
			break;
	}
	
	return 1;
}
addEvent("keyPress", onKeyPress);

// Tick function:
function onTick()
{
	if(waitingTurn > 0 || doneTurn > 0)
	{
		local pad = getPlayerPadState(getLocalPlayer());
		local left = pad.inVehicleMove[0];
		local right = pad.inVehicleMove[1];

		if(waitingTurn > 0)
		{
			// waiting to turn left?
			if(waitingTurn == 1 && left > 128)
			{
				doneTurn = 1;
				waitingTurn = 0;
			}
			// waiting to turn right?
			else if(waitingTurn == 2 && right > 128)
			{
				doneTurn = 2;
				waitingTurn = 0;
			}
		}
		else if(doneTurn > 0)
		{
			// waiting to stop turning left?
			if(doneTurn == 1 && left == 128)
			{
				triggerServerEvent("indicators:off");
				doneTurn = 0;
			}
			// waiting to stop turning right?
			else if(doneTurn == 2 && right == 128)
			{
				triggerServerEvent("indicators:off");
				doneTurn = 0;
			}
		}
	}
}
timer(onTick, 500, -1);