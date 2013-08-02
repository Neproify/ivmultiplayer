//audio.nut

//Definitions
// Audio(url or resource, replay, url to file or add only name from the ressource)
local url = "http://dl.dropbox.com/u/28171641/sounds/the_boondocks_-_riley%27s_theme.mp3"; // "http://files.seti.ee/music/one/SETI_ee_ti_feat_rihanna_-_live_your_life.mp3"
local audiourl = Audio(true,false,url);
local audiourlposition = Audio(true,false,url);
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
	else if(key == "b") // testcode
	{
		if(status == "down")
		{
			importAndLoadGameFile("hud.dat");
		}
	}
	else if(key == "v") // testcode
	{
		if(status == "down")
		{
			importAndLoadGameFile("RadioLogo.dat");
		}
	}
}
addEvent("keyPress", onKeyPress);
