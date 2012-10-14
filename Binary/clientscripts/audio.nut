//audio.nut

//Definitions
// Audio(url or resource, replay, url to file or add only name from the ressource)
local audiourl = Audio(true,false,"http://8106.tv/blog/audio/2009_09/15%20Raindrops%20Keep%20Falling%20On%20My%20Head.mp3");
local audiourlposition = Audio(true,false,"http://8106.tv/blog/audio/2009_09/15%20Raindrops%20Keep%20Falling%20On%20My%20Head.mp3");
audiourl.play();

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
			audiourlposition.setPosition(1649.508179, 395.261627, 38.151573,30.0);
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
	else if(key == "n")
	{
		if(status == "down")
		{
			tuneRadio(true,19);
		}
	}
	else if(key == "m")
	{
		if(status == "down")
		{
			tuneRadio(false,19);
		}
	}
}
addEvent("keyPress", onKeyPress);
