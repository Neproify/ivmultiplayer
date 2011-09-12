chatLimit = 25

function onPlayerText(playerid, text)
{
	local owncoordinates = getPlayerCoordinates(playerid)
	for(i = 0; i < MAX_PLAYERS; i++) {
		if(isPlayerConnected(i) && playerid != i) {
			local coordinates = getPlayerCoordinates(i)
			if(isPointInBall(owncoordinates[0], owncoordinates[1], owncoordinates[2], coordinates[0], coordinates[1], coordinates[2], chatLimit)) {
				sendPlayerMessage(i, text)
			}
		}
	}
}
addEvent("playerText", onPlayerText);