function test(tab)
{
	tab.password <- "HI";
}

local t = {};
test(t);
log(t.password);