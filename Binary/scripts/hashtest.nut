function onScriptInitHash()
{
  local result = hash.md5("test");
  log(result);
	return 1;
}
event("scriptInit", onScriptInitHash);
