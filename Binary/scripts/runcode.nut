addEvent("scriptInit", 
	function()
	{
		// Ensure the client script is loaded
		loadClientScript("runcode.nut");
	}
);

addEvent("scriptExit", 
	function()
	{
		// Ensure the client script is unloaded
		unloadClientScript("runcode.nut");
	}
);

function runCode(codeString)
{
	local scriptString = codeString;
	local scriptBuffer;

	try {
		scriptBuffer = compilestring("return " + scriptString);
	} catch(errorstring) {
		scriptBuffer = compilestring(scriptString);
	}

	if(scriptBuffer)
	{
		local returnString = scriptBuffer();

		if(returnString)
			print("Command executed! Result: " + returnString);
		else
			print("Command executed!");
	}
}

function onConsoleInput(input)
{
	local splitstring = split(input, " ");

	if(splitstring[0] == "runcode")
	{
		if(splitstring.len() > 1)
		{
			runCode(input.slice(8));
		}
	}
	else if(splitstring[0] == "cruncode")
	{
		if(splitstring.len() > 2)
		{
			local playerid = splitstring[1].tointeger();
			local code = input.slice(10 + splitstring[1].len());
			triggerClientEvent(playerid, "runCode", code);
		}
	}
}
addEvent("consoleInput", onConsoleInput);
