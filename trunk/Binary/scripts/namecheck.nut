function onPlayerNameCheck(playerid, name)
{
	for(local i = 0; i < name.len(); i++)
	{
		// Allow 'a' to 'z'
		if(name[i] >= 'a' && name[i] <= 'z')
			continue;

		// Allow 'A' to 'Z'
		if(name[i] >= 'A' && name[i] <= 'Z')
			continue;

		// Allow '0' to '9'
		if(name[i] >= '0' && name[i] <= '9')
			continue;

		// Allow '[' and ']'
		if(name[i] == '[' || name[i] == ']')
			continue;

		// Allow '_'
		if(name[i] == '_')
			continue;

		// Disallow all other characters
		return 0;
	}

	return 1;
}
addEvent("playerNameCheck", onPlayerNameCheck);