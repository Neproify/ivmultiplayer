function onScriptInit()
{
	log("onScriptInit");
	callEvent("someEvent", 1, "Hello", "World", 1337);
}
addEvent("scriptInit", onScriptInit);
addEvent("scriptInit", log, "The script has just loaded.");

function scriptInitFunc(h, w, l)
{
	log("scriptInitFunc(" + h + ", " + w + ", " + l + ")");
	callEvent("myEvent", 1, "Hello", "World");
}
addEvent("scriptInit", scriptInitFunc, "Hello", "World", 1337);

function onMyEvent(h, w, l)
{
	log("onMyEvent(" + h + ", " + w + ", " + l + ")");
}
addEvent("myEvent", onMyEvent, 1337);

function onSomeEvent(h, w, l)
{
	log("onSomeEvent(" + h + ", " + w + ", " + l + ")");
	//removeEvent("someEvent", onSomeEvent);
}
addEvent("someEvent", onSomeEvent);

function onServerPulse()
{
	//log("onServerPulse");
}
addEvent("serverPulse", onServerPulse);