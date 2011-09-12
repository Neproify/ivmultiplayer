function onPlayerNameCheck(playerid, name)
{
	for(local i = 0; i < name.len(); i++)
	{
		if(name[i] >= 'a' && name[i] <= 'z')
			continue;

		if(name[i] >= 'A' && name[i] <= 'Z')
			continue;

		if(name[i] >= '0' && name[i] <= '9')
			continue;

		if(name[i] == '[' || name[i] == ']')
			continue;

		if(name[i] == '_')
			continue;

		return 0;
	}
	return 1;
}
addEvent("playerNameCheck", onPlayerNameCheck);