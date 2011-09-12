function onConsoleInput(input)
{
	local splitstring = split(input, " ");

	if(splitstring[0] == "runcode")
	{
		if(splitstring.len() > 1)
		{
			local scriptString = input.slice(8);
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
	}
}
addEvent("consoleInput", onConsoleInput);
