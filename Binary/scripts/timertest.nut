local tid;
local tick;
local ticks = 0;
local compareStartTick = 0;
local compareTicks = [0,0];

function someFunc()
{
	log("someFunc");
}

function timerFunc1(somestr, someint, somefunc)
{
	log("timerFunc1(" + somestr + ", " + someint + ")");
	somefunc();
}

function timerFunc2(somestr)
{
	ticks++;
	log("time = " + (getTickCount() - tick));
	log("timerFunc2(" + somestr + ")");
	tick = getTickCount();
	if(ticks == 4)
	{
		tid.kill();
		log("Killed timer");
	}
}

function compare(id)
{
	compareTicks[id] += 1;
	local passed = getTickCount() - compareStartTick;
	if(id == 1)
	log(id.tostring() + " passed time: " + passed + " ticks: " + compareTicks[0] + ", " + compareTicks[1] + " avg: " + format("%.3f, %.3f", passed.tofloat()/compareTicks[0], passed.tofloat()/compareTicks[1]));
}

function onScriptInit()
{
	log("Timer script loaded");
	timer(timerFunc1, 1000, 1, "hello world", 1337, someFunc);
	tid = timer(timerFunc2, 2000, -1, "BOO");
	tick = getTickCount();
	timer(log, (2000 * 5), 1, "Hello World");
	
	// Comparison of old(beta1) and new timer behavior
	/*
	compareStartTick = tick;
	timer(compare, 50, -1, 0);
	timer(compare, 50, -1, 1).setTraditionalBehavior(true);
	*/
}
addEvent("scriptInit", onScriptInit);