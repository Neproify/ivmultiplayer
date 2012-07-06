const White = 0xFFFFFFFF;

// Function hooking example that adds all created vehicles to
// an array then deletes them when the script unloads
local vehicles = {};
local createVehicle_Original = createVehicle;
local blip1, blip2;
local blip;
blip = createBlip(75, 0.0, 0.0, 0.0,true);
local cp1;
local actor1;

function createVehicle(model, x, y, z, rx, ry, rz, c1, c2, c3, c4)
{
	local vehicleId = createVehicle_Original(model, x, y, z, rx, ry, rz, c1, c2, c3, c4);

	if(vehicleId != INVALID_VEHICLE_ID)
	{
		log("Created a vehicle with id " + vehicleId + " (Model " + model + " (" + getVehicleName(model) + "))");
		vehicles[vehicles.len()] <- vehicleId;
	}


	return vehicleId;
}

function onScriptInit()
{
	createVehicle(65, -343.447662, 1176.119263, 14.146016, 0.0, 0.0, 268.219513, 0, 0, 0, 0); // Police Patriot
	createVehicle(90, -343.527313, 1172.740479, 14.199832, 0.0, 0.0, 268.217010, 0, 0, 0, 0); // Sultan RS 2
	createVehicle(90, -343.501099, 1168.932739, 14.199870, 0.0, 0.0, 269.241302, 0, 0, 0, 0); // Sultan RS 3
	createVehicle(90, -343.609344, 1166.145630, 14.199894, 0.0, 0.0, 268.305298, 0, 0, 0, 0); // Sultan RS 4
	createVehicle(90, -343.623444, 1162.919067, 14.199799, 0.0, 0.0, 269.975037, 0, 0, 0, 0); // Sultan RS 5
	createVehicle(90, -343.633057, 1158.563843, 14.126013, 0.0, 0.0, 270.858826, 0, 0, 0, 0); // Sultan RS 6
	cp1 = createCheckpoint(3, -343.447662, 1171.119263, 14.146016, -343.447662, 1176.119263, 14.146016, 3.0);
	actor1 = createActor(242, -341.36, 1142.80, 14.79, 5.0);
	local actor2 = createActor(242, -343.36, 1142.80, 14.79, 5.0);
	local actor3 = createActor(242, -342.36, 1142.80, 14.79, 5.0);
	sendConsoleInput("uptime");
	log(format("Hello %s 0x%x %d", "World", 1337, 1337));
	log(_version_);
	local config = getConfig();
	log("Port = " + config["port"]);
	log("---------------------");
	log(SCRIPT_NAME + " loaded");
	log("---------------------");

	return 1;
}
addEvent("scriptInit", onScriptInit);

function onScriptExit()
{	
	for(local i = 0; i < vehicles.len(); i++)
		deleteVehicle(vehicles[i]);

	return 1;
}
addEvent("scriptExit", onScriptExit);

function onPlayerConnect(playerID, playerName, playerIP, playerSerial, bHasModdedGameFiles)
{
	// Don't use natives here, the player sends only the request, he's NOT connected to the server!
	sendMessageToAll(playerName + " (" + playerID + ") has connected.", White);
	if(bHasModdedGameFiles) {
		return 0; // Don't allow modified game files!
	}
	return 1;
}
addEvent("playerConnect", onPlayerConnect);

function onPlayerJoin(playerID) {
	// Now you can use the natives, because the player is registered at the server
	setPlayerSpawnLocation(playerID, -341.36, 1144.80, 14.79, 40.114815);
	sendPlayerMessage(playerID, "Welcome to Central Park", White);
	sendPlayerMessage(playerID, "Checkpoint: " + cp1, White);
}
addEvent("playerJoin", onPlayerJoin);

function onPlayerDisconnect(playerid, reason)
{
	local strreason = "Quit";

	if(reason == 1)
		strreason = "Timeout/Crash";

	sendMessageToAll(getPlayerName(playerid) + " (" + playerid + ") has left the server (" + strreason + ").", White);
	deletePlayerBlip(playerid);
	return 1;
}
addEvent("playerDisconnect", onPlayerDisconnect);

function onSpawn(playerid)
{
	createPlayerBlip(playerid, 3);
	togglePlayerBlipShortRange(playerid,true);
}
addEvent("playerSpawn", onSpawn);

function onPlayerCommand(playerid, command)
{
	local cmd = split(command, " ");

	if(cmd[0] == "/atest")
	{
		setActorName(actor1, "adamixcock");
		toggleActorNametag(actor1, true);
		toggleActorHelmet(actor1, false);
		toggleActorFrozen(actor1, false);
		//warpActorIntoVehicle(actor1, 2, 2);
	}
	if(cmd[0] == "/behind")
	{
		setCameraBehindPlayer(playerid);
	}
	if(cmd[0] == "/lockVehicle")
	{
		setVehicleLocked(cmd[1].tointeger(),cmd[2].tointeger());
	}
	if(cmd[0] == "/setammo")
	{
		setPlayerAmmo(playerid,cmd[1].tointeger(),cmd[2].tointeger());
	}
	if(cmd[0] == "/setammoinclip")
	{
		setPlayerAmmoInClip(playerid,cmd[1].tointeger());
	}
	if(cmd[0] == "/displayHudMessage")
	{
		displayHudNotification(playerid,1,"UNREAD_MESSAGES");
	}
	if(cmd[0] == "/vehiclefollow")
	{
		setPlayerFollowVehicleMode(playerid,0);
	}
	if(cmd[0] == "/attachcameratovehicle")
	{
		attachPlayerCameraToVehicle(playerid,cmd[1].tointeger());
	}
	if(cmd[0] == "/attachcameratoplayer")
	{
		attachPlayerCameraToPlayer(playerid,cmd[1].tointeger());
	}
	if(cmd[0] == "/respawn")
	{
		setPlayerSpawnLocation(playerid, -341.36, 1144.80, 14.79, 40.114815);
		respawnPlayer(playerid);
	}
	if(cmd[0] == "/name")
	{
		sendPlayerMessage(playerid,"VEHILCENAME:"+getVehicleName(getVehicleModel(getPlayerVehicleId(playerid))));
	}
	if(cmd[0] == "/fadein")
	{
		fadePlayerScreenIn(playerid,5);
	}
	if(cmd[0] == "/fadeout")
	{
		fadePlayerScreenOut(playerid, 5);
	}
	if(cmd[0] == "/policereport")
	{
		triggerPlayerPoliceReport(playerid,"SCRIPTED_REPORTS_C4");
	}
	if(cmd[0] == "/mission")
	{
		triggerPlayerMissionSound(playerid,6);
	}
	if(cmd[0] == "/request")
	{
		requestPlayerAnimations(playerid);
	}
	if(cmd[0] == "/sound")
	{
		triggerPlayerGameSound(playerid,"armor.wav");
	}
	if(cmd[0] == "/blib")
	{
		toggleBlipRoute(blip,true,playerid);
	}
	if(cmd[0] == "/changeblip")
	{
		switchBlipIconForPlayer(blip,playerid,true);
	}
	if(cmd[0] == "/changeblip2")
	{
		switchBlipIconForPlayer(blip,playerid,false);
	}
	if(cmd[0] == "/forceanimplayer")
	{
		forcePlayerPlayAnimation(playerid,"amb@smoking_spliff","create_spliff");
	}
	if(cmd[0] == "/createfire")
	{
		local pos = getPlayerCoordinates(playerid);
		createFire(pos[0]+10.0,pos[1]+10.0,pos[2],10.0);
	}
	if(cmd[0] == "/deletefire")
	{
		deleteFire(cmd[1].tointeger());
	}
	if(cmd[0] == "/createexplosion")
	{
		local pos = getPlayerCoordinates(playerid);
		createExplosion(pos[0]+10.0,pos[1]+10.0,pos[2],10.0);
	}
	if(cmd[0] == "/remoteon")
	{
		setVehicleEngineState(cmd[1].tointeger(), true);
	}
	if(cmd[0] == "/remoteoff")
	{
		setVehicleEngineState(cmd[1].tointeger(),false);
	}
	if(cmd[0] == "/lights")
	{
		setVehicleLights(getPlayerVehicleId(playerid),true);
	}
	if(cmd[0] == "/lightsoff")
	{
		setVehicleLights(getPlayerVehicleId(playerid),false);
	}
	if(cmd[0] == "/ccd")
	{
		controlCarDoors(getPlayerVehicleId(playerid),cmd[1].tointeger(),true,cmd[2].tofloat());
	}
	if(cmd[0] == "/taxi")
	{
		setVehicleTaxiLights(getPlayerVehicleId(playerid),true);
	}
	if(cmd[0] == "/taxioff")
	{
		setVehicleTaxiLights(getPlayerVehicleId(playerid),false);
	}
	if(cmd[0] == "/engine")
	{
		setVehicleEngineState(getPlayerVehicleId(playerid),true);
	}
	if(cmd[0] == "/engineoff")
	{
		setVehicleEngineState(getPlayerVehicleId(playerid),false);
	}
	if(cmd[0] == "/createActor")
	{
		local pos = getPlayerCoordinates(playerid);
		local actor = createActor(8,pos[0],pos[1],pos[2],90.0);
		toggleActorHelmet(actor,false);
		toggleActorFrozen(actor,false);
		setActorName(actor,"JeNkStAX");
		toggleActorNametag(actor,true);
		forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff");
	}
	if(cmd[0] == "/toghelmet1")
	{
		toggleActorHelmet(0,true);
	}
	if(cmd[0] == "/toghelmet2")
	{
		toggleActorHelmet(0,false);
	}
	if(cmd[0] == "/togfrozen1")
	{
		toggleActorFrozen(0,true);
	}
	if(cmd[0] == "/togfrozen2")
	{
		toggleActorFrozen(0,false);
	}
	if(cmd[0] == "/walk")
	{
		actorWalkToCoordinatesForPlayer(playerid,0,0.0,0.0,0.0,1);
	}
	if(cmd[0] == "/setname")
	{
		setActorName(0,"Fred");
	}
	if(cmd[0] == "/togname1")
	{
		toggleActorNametag(0,true);
	}
	if(cmd[0] == "/togname2")
	{
		toggleActorNametag(0,false);
	}
	if(cmd[0] == "/forceanim")
	{
		forceActorAnimation(0,"hello", "gestures@niko");
	}
	if(cmd[0] == "/grav")
	{
		setPlayerGravity(playerid, 0);
		return 1;
	}
	
	if(cmd[0] == "/givehelm")
	{
		givePlayerHelmet(playerid);
		return 1;
	}
	
	if(cmd[0] == "/removehelm")
	{
		removePlayerHelmet(playerid);
		return 1;
	}
	
	if(cmd[0] == "/grav1")
	{
		setPlayerGravity(playerid, 1);
		return 1;
	}	
	
	if(cmd[0] == "/kill")
	{
		setPlayerHealth(playerid, -1);
		return 1;
	}
	
	if(cmd[0] == "/model")
	{
		if(cmd.len() == 2)
		{
			if(setPlayerModel(playerid, cmd[1].tointeger()))
				sendPlayerMessage(playerid, "Model set to " + cmd[1], White);
		}

		return 1;
	}
	
	if(cmd[0] == "/testtime")
	{
		setPlayerTime(playerid, 23, 0);
		return 1;
	}
	
	if(cmd[0] == "/testweapon")
	{
		givePlayerWeapon(playerid, 9, 20);
		return 1;
	}

	if(cmd[0] == "/time")
	{
		if(cmd.len() == 3)
			setPlayerTime(playerid, cmd[1].tointeger(), cmd[2].tointeger());

		return 1;
	}

	if(cmd[0] == "/wep")
	{
		if(cmd.len() == 3)
			givePlayerWeapon(playerid, cmd[1].tointeger(), cmd[2].tointeger());

		return 1;
	}

	if(cmd[0] == "/h" || cmd[0] == "/health")
	{
		if(cmd.len() == 2)
		{
			if(cmd[1].tofloat() >= 0 && cmd[1].tofloat() <= 200)
				setPlayerHealth(playerid, cmd[1].tointeger());
		}

		return 1;
	}

	if(cmd[0] == "/a" || cmd[0] == "/armour")
	{
		if(cmd.len() == 2)
		{
			if(cmd[1].tofloat() > 0 && cmd[1].tofloat() <= 200)
			{
				sendPlayerMessage(playerid, "Armour change to " + cmd[1]);
				setPlayerArmour(playerid, cmd[1].tointeger());
			}
		}

		return 1;
	}

	if(cmd[0] == "/autoaim")
	{
		if(cmd.len() == 2)
		{
			if(cmd[1] == "true") togglePlayerAutoAim(playerid, true);
			else togglePlayerAutoAim(playerid, false);
		}

		return 1;
	}

	if(cmd[0] == "/v")
	{
		if(cmd.len() == 2)
		{
			local model = cmd[1].tointeger();
			local pos;
			if(isPlayerInAnyVehicle(playerid))
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid));
			else
				pos = getPlayerCoordinates(playerid);
			local heading = getPlayerHeading(playerid);
			local veh = createVehicle(model, pos[0], pos[1], pos[2], 0.0, 0.0, heading, 1, 1, 1, 1);
			if(veh != INVALID_VEHICLE_ID)
			{
				warpPlayerIntoVehicle(playerid, veh);
				sendPlayerMessage(playerid, getVehicleName(model) + " spawned at your position (ID " + veh + ").");
			}
		}

		return 1;
	}

	if(cmd[0] == "/dv")
	{
		if(isPlayerInAnyVehicle(playerid))
		{
			local vehicleId = getPlayerVehicleId(playerid);
			deleteVehicle(vehicleId);
			sendPlayerMessage(playerid, "Your vehicle has been deleted.");
		}
		else
			sendPlayerMessage(playerid, "You are not in a vehicle.");

		return 1;
	}
	
	if(cmd[0] == "/out")
	{
		sendPlayerMessage(playerid, "You are being warped out of your vehicle.");
		removePlayerFromVehicle(playerid);
		return 1;
	}

	if(cmd[0] == "/out2")
	{
		sendPlayerMessage(playerid, "You are leaving your vehicle.");
		removePlayerFromVehicle(playerid, true);
		return 1;
	}

	if(cmd[0] == "/testmessage")
	{
		sendPlayerMessage(playerid, "Hello [FF0000FF]World", White, true);
		return 1;
	}

	if(cmd[0] == "/testobject")
	{
		// http://www.gtamodding.com/index.php?title=Category:Static_Model_Hashes
		createObject(0xB4FC2C1B, -341.36, 1144.80, 14.79, 0.0, 0.0, 0.0);
		return 1;
	}

	if(cmd[0] == "/money")
	{
		if(cmd.len() == 2)
		{
			setPlayerMoney(playerid, cmd[1].tointeger());
			sendPlayerMessage(playerid, "Money set", White);
		}

		return 1;
	}
	
	if(cmd[0] == "/text")
	{
		if(cmd.len() == 3)
			displayPlayerText(playerid, 0.1, 0.1, cmd[1].tostring(), cmd[2].tointeger());

		return 1;
	}
	
	if(cmd[0] == "/infotext")
	{
		if(cmd.len() == 3)
			displayPlayerInfoText(playerid, cmd[1].tostring(), cmd[2].tointeger());

		return 1;
	}

	if(cmd[0] == "/texttoall")
	{
		if(cmd.len() == 3)
			displayTextToAll(0.1, 0.1, cmd[1].tostring(), cmd[2].tointeger());

		return 1;
	}

	if(cmd[0] == "/physics")
	{
		if(cmd.len() == 2)
			togglePlayerPhysics(playerid, cmd[1].tointeger());

		return 1;
	}

	if(cmd[0] == "/controls")
	{
		if(cmd.len() == 2)
			togglePlayerControls(playerid, cmd[1].tointeger());

		return 1;
	}

	if(cmd[0] == "/wanted")
	{
		if(cmd.len() == 2)
			setPlayerWantedLevel(playerid, cmd[1].tointeger());
		
		return 1;
	}

	if(cmd[0] == "/goto")
	{
		if(cmd.len() == 2)
		{
			local toPlayerId = cmd[1].tointeger();

			if(!isPlayerConnected(toPlayerId))
			{
				sendPlayerMessage(playerid, "That player is not connected.");
				return 1;
			}

			local pos;
			if(isPlayerInAnyVehicle(toPlayerId))
				pos = getVehicleCoordinates(getPlayerVehicleId(toPlayerId));
			else
				pos = getPlayerCoordinates(toPlayerId);

			if(isPlayerInAnyVehicle(playerid))
				setVehicleCoordinates(getPlayerVehicleId(playerid), pos[0], pos[1], pos[2]);
			else
				setPlayerCoordinates(playerid, pos[0], pos[1], pos[2]);
			
			sendPlayerMessage(playerid, "You have been teleported.");
		}

		return 1;
	}

	if(cmd[0] == "/cptest")
	{
		hideCheckpointForAll(cp1);
	}
	
	if(cmd[0] == "/testcheckpoint")
	{
		if(cmd.len() == 2)
		{
			for(local i = 0; i < 32; i++)
			{
				hideCheckpointForAll(i);
				deleteCheckpoint(i);
			}
			local pos;
			if(isPlayerInAnyVehicle(playerid))
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid));
			else
				pos = getPlayerCoordinates(playerid);
			local checkpointid = createCheckpoint(cmd[1].tointeger(), pos[0], pos[1], pos[2], pos[0], pos[1], (pos[2] + 10), 5.0);
			showCheckpointForAll(checkpointid);
			sendPlayerMessage(playerid, "Checkpoint created with id " + checkpointid);
		}
	}
		
	if(cmd[0] == "/vh")
	{
		if(cmd.len() == 2)
		{
			if(isPlayerInAnyVehicle(playerid))
			{
				local vehicleid = getPlayerVehicleId(playerid);
				local oldhealth = getVehicleEngineHealth(vehicleid);
				local health = cmd[1].tointeger();
				setVehicleEngineHealth(vehicleid, health);
				sendPlayerMessage(playerid, "Vehicle health set from " + oldhealth + " to " + health);
			}
		}
	}

	if(cmd[0] == "/getrot")
	{
		if(isPlayerInAnyVehicle(playerid))
		{
			local rot = getVehicleRotation(getPlayerVehicleId(playerid));
			sendPlayerMessage(playerid, "Rot(X: " + rot[0] + ", Y: " + rot[1] + ", Z: " + rot[2] + ")");
		}
	}

	if(cmd[0] == "/setrot")
	{
		if(cmd.len() == 4)
		{
			if(isPlayerInAnyVehicle(playerid))
			{
				setVehicleRotation(getPlayerVehicleId(playerid), cmd[1].tofloat(), cmd[2].tofloat(), cmd[3].tofloat());
				sendPlayerMessage(playerid, "Rotation set");
			}
		}
	}

	if(cmd[0] == "/ind")
	{
		if(cmd.len() == 3)
		{
			if(isPlayerInAnyVehicle(playerid))
			{
				local index = cmd[1].tointeger();

				if(index < 0 || index > 3)
				{
					sendPlayerMessage(playerid, "Invalid indicator index.");
				}
				else
				{
					local vehicle = getPlayerVehicleId(playerid);
					local indicators = getVehicleIndicators(vehicle);
					local toggle = ((cmd[2].tointeger() == 1) ? true : false);
					indicators[index] = toggle;
					setVehicleIndicators(vehicle, indicators[0], indicators[1], indicators[2], indicators[3]);
					sendPlayerMessage(playerid, "Indicator index " + index + " set to " + toggle + ".");
				}
			}
		}
	}
	
	return 0;
}
addEvent("playerCommand", onPlayerCommand);

function onPlayerDeath(playerid, killerid, killerweapon, killervehicle)
{
	if(killerid != INVALID_PLAYER_ID)
		sendMessageToAll("[FF0000FF]" + getPlayerName(playerid) + " [FFFFFFFF](" + playerid + ") was killed by [FFFF0000]" + getPlayerName(killerid) + " [FFFFFFFF](" + killerid + ")(Weapon: "+killerweapon+").", White, true);
	else
		sendMessageToAll(getPlayerName(playerid) + " died");

	return 1;
}
addEvent("playerDeath", onPlayerDeath);

function onWebRequest(uri, remote_ip, remote_method)
{
	log(format("Web request: %s [%s] [%s]", uri, remote_ip, remote_method));
}
addEvent("webRequest", onWebRequest);

function onVehicleDamage(vehicleid,oldhealth,oldpetrol,newhealth,newpetrol)
{
	log(format("VEHICLE DAMAGE: %d, %d, %d, %d, %d",vehicleid,oldhealth,oldpetrol,newhealth,newpetrol));
}
addEvent("vehicleDamage",onVehicleDamage);

function onPlayerShot(playerid,x,y,z,shot)
{
	if(shot)
		log(format("WEP: %d SHOT(%f, %f, %f)",playerid,x,y,z));
	else
		log(format("WEP: %d AIM(%f, %f, %f)",playerid,x,y,z));
}
addEvent("playerShot",onPlayerShot);

function onVehicleDeath(vehicleid)
{
	log("Vehicle Death "+vehicleid);
}
addEvent("vehicleDeath",onVehicleDeath);

function onVehicleRespawn(vehicleid)
{
	log("Vehicle Respawn "+vehicleid);
}
addEvent("vehicleRespawn",onVehicleRespawn);

function onHeadMovement(playerid, oldx, oldy, oldz, newx, newy, newz)
{
	log(format("HEAD: %d OLD(%f, %f, %f) | NEW (%f, %f, %f)",playerid,oldx,oldy,oldz,newx,newy,newz));
}
addEvent("headMove",onHeadMovement);

function onVehicleRequest(playerid,vehicleid,seat)
{
	return 1;
}
addEvent("vehicleEntryRequest", onVehicleRequest);