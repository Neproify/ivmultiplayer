function onScriptInitHash()
{
  local result = hash.md5("test");
  log(result);
	return 1;
}
addEvent("scriptInit", onScriptInitHash);
