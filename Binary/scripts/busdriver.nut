const White = 0xFFFFFFFF;

local bus = createVehicle(12, 1644.682373, 443.919067, 28.959641, 0.186646, 359.772217, 181.766296, 1, 1, 1, 1); // car9
setVehicleRotation(bus,0.186646, 359.772217, 181.766296);
local busdriver;

function onScriptInit()
{
	busdriver = createActor(124, 1640.639648, 438.945587, 29.167524, -1.475237);
	setActorName(busdriver, "Busdriver");
	setActorNametagColor(busdriver, White);
	markVehicleAsActorVehicle(bus,true);
	return 1;
}
addEvent("scriptInit", onScriptInit);

function onPlayerCommand(playerid, command)
{
	local cmd = split(command, " ");

	if(cmd[0] == "/warpactor")
	{
		warpActorIntoVehicle(busdriver,bus,0);
		setVehicleEngineState(bus,true);
	}
	if(cmd[0] == "/drive")
	{
		driveActorToCoordinates(busdriver, 1024.564575, 262.859406, 31.446772);
	}
}
addEvent("playerCommand", onPlayerCommand);