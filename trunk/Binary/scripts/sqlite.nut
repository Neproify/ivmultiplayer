function onScriptInit()
{
	local database = db("test.db");
	database.query("CREATE TABLE test (id INT)");
	local table = database.query("SELECT * FROM test");
	print(table.tostring());
	database.close();
	return 1;
}
addEvent("scriptInit", onScriptInit);