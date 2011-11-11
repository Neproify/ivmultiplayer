function runCode(code)
{
	local scriptBuffer;

	try {
		scriptBuffer = compilestring("return " + code);
	} catch(errorstring) {
		scriptBuffer = compilestring(code);
	}

	if(scriptBuffer)
	{
		local returnString = scriptBuffer();

		if(returnString)
			addChatMessage("Command executed! Result: " + returnString);
		else
			addChatMessage("Command executed!");
	}
}
addEvent("runCode", runCode);