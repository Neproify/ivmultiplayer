local levelarr = array(100, 0); // Creates a static 100 size array with all values at 0

function onScriptInit()
{
	// Static array test
	for(local i = 0; i < 100; i++)
	{
		levelarr[i] = i;
	}
	for(local i = 0; i < 100; i++)
	{
		if(levelarr[i] != i)
		{
			log("Fail " + i);
		}
		else
		{
			log("levelarr[" + i + "] = " + levelarr[i]);
			levelarr[i] = i + 2;
		}
	}
	for(local i = 0; i < 100; i++)
	{
		if(levelarr[i] != i + 2)
		{
			log("Fail " + i);
		}
		else
		{
			log("levelarr[" + i + "] = " + levelarr[i]);
		}
	}
	// Dynamic array test
	local dylevelarr = {};
	for(local i = 0; i < 100; i++)
	{
		// Creates a new slot at the specified index (i)
		dylevelarr[i] <- i;
	}
	for(local i = 0; i < 100; i++)
	{
		if(dylevelarr[i] != i)
		{
			log("Fail " + i);
		}
		else
		{
			log("dylevelarr[" + i + "] = " + dylevelarr[i]);
			dylevelarr[i] = i + 2;
		}
	}
	for(local i = 0; i < 100; i++)
	{
		if(dylevelarr[i] != i + 2)
		{
			log("Fail " + i);
		}
		else
		{
			log("dylevelarr[" + i + "] = " + dylevelarr[i]);
		}
	}
}
addEvent("scriptInit", onScriptInit);