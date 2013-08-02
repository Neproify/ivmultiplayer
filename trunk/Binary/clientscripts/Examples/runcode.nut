// This example shows how you can compile squirrel code from string variable and execute

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