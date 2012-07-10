//audio.nut

//Definitions
// Audio(url or resource, replay, url to file or add only name from the ressource)
local audiourl = Audio(true,false,"http://martinkonrad.square7.ch/testmusic.mp3");
local audiourlposition = Audio(true,false,"http://martinkonrad.square7.ch/testmusic.mp3");

function onKeyPress(key, status)
{
	if(key == "u")
	{
		if (status == "down")
		{
			audiourl.play();
			addChatMessage("Music stream starts to play...");
		}
	}
	else if(key == "i")
	{
		if(status == "down")
		{
			audiourl.stop();
			addChatMessage("Stopped music stream");
		}
	}
	else if(key == "o")
	{
		if (status == "down")
		{
			audiourlposition.usePositionSystem(true);
			audiourlposition.setPosition(-341.36, 1144.80, 14.79,30.0);
			audiourlposition.play();
			addChatMessage("Music stream with position system starts to play...");
		}
	}
	else if(key == "p")
	{
		if(status == "down")
		{
			audiourlposition.stop();
			addChatMessage("Stopped music stream with position system");
		}
	}
}
addEvent("keyPress", onKeyPress);
