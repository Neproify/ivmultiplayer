function onScriptInit()
{
	log("Loading dofile2.nut...");
	dofile("scripts/dofile2.nut");
	dofileTest();
}
addEvent("scriptInit", onScriptInit);