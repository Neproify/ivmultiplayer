//scoreboard.nut
//Made by Sebihunter
//www.iv-multiplayer.com

//Definitions
local maxPlayers = 48 //This needs to be changed if your server allows more than 48 players!
local serverName = "IV: Multiplayer" //Change it to your server name
local onlinePlayers = 0
local screen = guiGetScreenSize();
local showscoreboard = false;
local boldfont = GUIFont("tahoma-bold");
local font = GUIFont("tahoma");
local headerfont = GUIFont("bankgothic", 12);

//Gets the playercount on the server
function getPlayersOnServer()
{
	local players = 0
	for(local ply = 0; ply < maxPlayers; ply++)
	{
		if(isPlayerConnected(ply))
		{
			players++;
		}
	}
	return players;
}

//Draw the scoreboard
function onFrameRender()
{
	if(showscoreboard == true)
	{
		local y = screen[1]/2-(15*onlinePlayers)/2;
		guiDrawRectangle(screen[0]/2-200, y, 400.0, 15.0, 0x50505080, false);
		y = y+15;
		guiDrawRectangle(screen[0]/2-200, y, 400.0, 15.0+onlinePlayers*15, 0x00000080, false);
		boldfont.drawText(screen[0]/2-200+1, y,"ID", false);
		boldfont.drawText(screen[0]/2-200+50, y,"Name", false);
		boldfont.drawText(screen[0]/2-200+250, y,"Ping", false);	
		y = y+15;
		local players = 0;
		for(local ply = 0; ply < maxPlayers; ply++)
		{
			if(isPlayerConnected(ply))
			{
				font.drawText(screen[0]/2-200+1, y, ply.tostring(), false);
				font.drawText(screen[0]/2-200+50, y, getPlayerName(ply), false);
				local ping = getPlayerPing(ply);
				if (ping < 100) {
					font.drawText(screen[0]/2-200+250, y, "[00FF33FF]"+ping.tostring(), false);
				} else if (ping < 200){
					font.drawText(screen[0]/2-200+250, y, "[FF7D40FF]"+ping.tostring(), false);
				}else{
					font.drawText(screen[0]/2-200+250, y, "[CD0000FF]"+ping.tostring(), false);
				}	
				y = y+13;
				players++;
			}
		}
		headerfont.drawText(screen[0]/2-200+1,screen[1]/2-(15*onlinePlayers)/2, serverName+" | Players: "+players.tostring(), false);
		onlinePlayers = players
	}	
}
addEvent("frameRender", onFrameRender);

function onKeyPress(key, status)
{
	if(key == "tab")
	{
		if (status == "down")
		{
			toggleChatWindow(false);
			onlinePlayers = getPlayersOnServer()
			showscoreboard = true;
		}else{
			toggleChatWindow(true);
			showscoreboard = false;
		}
	}
}
addEvent("keyPress", onKeyPress);
