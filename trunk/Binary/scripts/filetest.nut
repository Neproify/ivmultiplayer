class TestFile extends file
{
	function writestr(str)
	{
		if(typeof(str) != "string")
		{
			return 0;
		}
		writen(str.len(), 'i');
		for(local i = 0; i < str.len(); i++)
		{
			writen(str[i], 'c');
		}
		return 1;
	}
	function readstr()
	{
		local size = readn('i');
		local str = "";
		for(local i = 0; i < size; i++)
		{
			str += readn('c').tochar();
		}
		return str.tostring();
	}
	function writestr2(str)
	{
		if(typeof(str) != "string")
		{
			return 0;
		}
		for(local i = 0; i < str.len(); i++)
		{
			writen(str[i], 'c');
		}
		return 1;
	}
	function readstr2(size)
	{
		local str = "";
		for(local i = 0; i < size; i++)
		{
			str += readn('c').tochar();
		}
		return str.tostring();
	}
}

function onScriptInit()
{
	local fp = TestFile("test.txt", "wb+");
	fp.writestr("Hello World");
	fp = TestFile("test.txt", "rb+");
	log(fp.readstr());
	return 1;
}
addEvent("scriptInit", onScriptInit);