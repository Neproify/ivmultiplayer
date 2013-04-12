const White = 0xFFFFFFFF;

// Function hooking example that adds all created vehicles to
// an array then deletes them when the script unloads
local vehicles = {};
local createVehicle_Original = createVehicle;
local blip1, blip2;
local blip = createBlip(75, 0.0, 0.0, 0.0,true);
setBlipName(blip,"ILoveIVMP");
local cp1;
local actor1;
local blipcolortest;

function createVehicle(model, x, y, z, rx, ry, rz, c1, c2, c3, c4, dimension, respawn_delay = -1)
{
	local vehicleId = createVehicle_Original(model, x, y, z, rx, ry, rz, c1, c2, c3, c4, dimension, respawn_delay);

	if(vehicleId != INVALID_VEHICLE_ID)
	{
		log("Created a vehicle with id " + vehicleId + " (Model " + model + " (" + getVehicleName(model) + ")) (Dimension: " + dimension + " )");
		vehicles[vehicles.len()] <- vehicleId;
	}
	return vehicleId;
}

function onScriptError(name, callstack, locals)
{
	log("<scriptError (" + name + ")>");
	log("<Callstack>");
	foreach(name, info in callstack)
	{
		log("<" + name + " (" + info[0] + ", " + info[1] + ")>");
	}
	log("</Callstack>");
	log("<Locals>");
	foreach(name, type in locals)
	{
		log("<" + name + " (" + type + ")>");
	}
	log("</Locals>");
	log("</scriptError>");
	return 0; // Don't print error to console
}
addEvent("scriptError", onScriptError);

function onScriptInit()
{
	createVehicle(50, 1682.030884, 415.455414, 28.492632, 359.998413, 358.788818, 47.536011, 1, 1, 1, 1, 0, 30000); // car1
	createVehicle(27, 1667.659546, 432.233398, 28.631674, 0.607056, 351.466797, 88.324951, 1, 1, 1, 0, 1); // car2
	createVehicle(37, 1666.811768, 421.175903, 28.643723, 359.297852, 351.982788, 268.549927, 0, 0, 0, 0, 0); // car3
	createVehicle(40, 1657.118408, 421.462982, 28.569500, 359.828613, 352.884033, 267.583008, 0, 0, 0, 0, 0); // car4
	createVehicle(93, 1649.273560, 421.382935, 28.761145, 0.001465, 351.280945, 270.491577, 1, 1, 1, 1, 0); // car5
	createVehicle(28, 1641.365967, 421.592712, 28.691023, 359.782715, 352.587891, 269.953735, 131, 131, 131, 131, 0); // car6
	createVehicle(95, 1631.719482, 420.940094, 28.562838, 1.065430, 352.694153, 277.920654, 0, 0, 0, 0, 0); // car7
	createVehicle(91, 1628.712646, 430.695221, 28.632729, 359.860657, 352.000366, 96.860474, 0, 0, 0, 0, 0); // car8
	//createVehicle(12, 1644.682373, 443.919067, 28.959641, 0.186646, 359.772217, 181.766296, 1, 1, 1, 1, 0); // car9
	createVehicle(90, 1655.603516, 439.904358, 28.416683, 359.700562, 352.003357, 0.547791, 0, 0, 0, 0, 0); // car10
	createVehicle(18, 1655.166138, 448.823395, 28.452307, 0.021606, 352.756836, 358.443848, 0, 0, 0, 0, 0); // car11
	createVehicle(106, 1650.725464, 411.433807, 30.725628, 0.017944, 0.080933, 28.687622, 0, 0, 0, 0, 0); // car12
	createVehicle(108, 1647.794922, 411.647644, 30.763536, 359.298218, 2.161499, 335.835205, 1, 1, 1, 1, 0); // car13
	//cp1 = createCheckpoint(3, -343.447662, 1171.119263, 14.146016, -343.447662, 1176.119263, 14.146016, 3.0);
	getVehicleDimension(vehicles[0]);
	//actor1 = createActor(242, -341.36, 1142.80, 14.79, 5.0);
	log(_version_);
	local config = getConfig();
	log("Port = " + config["port"]);
	log(SCRIPT_NAME + " sucessfully loaded");
	blipcolortest = createBlip(78,1000.0,1000.0,0.0,true);
	setBlipColor(blipcolortest,0xE59338FF,-1);
	setBlipName(blipcolortest, "idk");
	return 1;
}
addEvent("scriptInit", onScriptInit);

function onScriptExit()
{	
	log("scriptExit();");
	for(local i = 0; i < vehicles.len(); i++)
		deleteVehicle(vehicles[i]);

	log("done scriptExit();");
	return 1;
}
addEvent("scriptExit", onScriptExit);

function onPlayerAuth(playerID, playerName, playerIP, playerSerial, bHasModdedGameFiles)
{
	// Don't use natives here, the player sends only the request, he's NOT connected to the server!
	sendMessageToAll(playerName + " (" + playerID + ") is connecting.", White);
	if(bHasModdedGameFiles) {
	//	return 0; // Don't allow modified game files!
	}
	return 1;
}
addEvent("playerAuth", onPlayerAuth);

function onPlayerConnect(playerid, playerName)
{
	// Now you can use the natives, because the player is registered at the server
	setPlayerSpawnLocation(playerid, /*-341.36, 1144.80, 14.79, 40.114815*/1649.508179, 395.261627, 38.151573, -2.931725);
	sendPlayerMessage(playerid, "Welcome to Brooks", White);
	sendPlayerMessage(playerid, "Press [FFFF0000]'I'[FFFFFFAA] to stop the music!",0xFFFFFFAA,true);
	setPlayerTime(playerid, 6, 50);
}
addEvent("playerConnect", onPlayerConnect);

function onPlayerJoin(playerid) {
	// After he has downloaded all files, we're checking his gta files
	sendMessageToAll(getPlayerName(playerid) + " (" + playerid + ") has successfully connected.", White);
	// Check files
	log("FileChecksum from user "+getPlayerName(playerid)+" handling.dat: 0x"+getPlayerFileChecksum(playerid,0));
	log("FileChecksum from user "+getPlayerName(playerid)+" gta.dat: 0x"+getPlayerFileChecksum(playerid,1));
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
	//setPlayerModel(playerid, 7);
	createPlayerBlip(playerid, 3);
	togglePlayerBlipShortRange(playerid,true);
	
	// Request here all animations, which you use
	// ATTENTION: It can take 1 up to 5 seconds to load the animation(s) successfully!
	requestPlayerAnimations(playerid,"amb@smoking_spliff");
	requestPlayerAnimations(playerid,"amb@taxi_wait_i_f_b");
	requestPlayerAnimations(playerid,"amb@taxi_hail_f");
	
	// NOTE: IF you USED the animations and don't need it anymore -> unrequestplayeranimations
	// ATTENTION: You must load at all players the animations, if you force it!
	//unRequestPlayerAnimations(playerid,"amb@smoking_spliff");
}
addEvent("playerSpawn", onSpawn);

function onPlayerCommand(playerid, command)
{
	local cmd = split(command, " ");

	if(cmd[0] == "/mydimension")
	{
		sendPlayerMessage(playerid, "You're currently in dimension " + getPlayerDimension(playerid) + ".");
	}
	if(cmd[0] == "/setdimension")
	{
		if(cmd.len() < 2)
			sendPlayerMessage(playerid, "USAGE: /setdimension [playerid]", 0xFFFFFFAA);
		else {
			setPlayerDimension(playerid, cmd[1].tointeger());
			sendPlayerMessage(playerid, "You have set your dimension to " + cmd[1].tointeger() + ".", 0xFFFFFFAA);
		}
	}	
	if(cmd[0] == "/label")
	{
		if(cmd.len() < 2) 
			sendPlayerMessage(playerid, "USAGE: /label [labeltext]", 0xFFFFFFAA);
		else 
		{
			local pos = getPlayerCoordinates(playerid);
			local labeltext = command.slice(cmd[0].len() + 1);
			local keks = create3DLabel(labeltext, pos[0], pos[1], pos[2], 0xFF0000FF, true, 600.0);
			sendPlayerMessage(playerid, "Added successfull label "+keks+" with display string "+labeltext+"...",0xFFFFFFAA);
		}
	}
	if(cmd[0] == "/weather")
	{
		setWeather(cmd[1].tointeger());
	}
	if(cmd[0] == "/moveobject")
	{
		moveObject(cmd[1].tointeger(), 1727.337769, 705.455688, 25.831680, 5000.0);
	}
	if(cmd[0] == "/getwantedlevel")
	{
		sendPlayerMessage(playerid,"WANTEDLEVEL:"+getPlayerWantedLevel(playerid));
	}
	if(cmd[0] == "/togglenametag")
	{
		if(cmd[3].tointeger() == 1)
			togglePlayerNametagForPlayer(cmd[1].tointeger(),cmd[2].tointeger(), true);
		else
			togglePlayerNametagForPlayer(cmd[1].tointeger(),cmd[2].tointeger(), false);
	}
	if(cmd[0] == "/repairvehicle")
	{
		repairVehicle(getPlayerVehicleId(playerid));
	}
	if(cmd[0] == "/world")
	{
		setPlayerDimension(playerid, 1);
	}
	if(cmd[0] == "/world0")
	{
		setPlayerDimension(playerid, 0xFF-1);
	}
	if(cmd[0] == "/clothes")
	{
		setPlayerClothes(playerid, cmd[1].tointeger(), cmd[2].tointeger());
	}
	if(cmd[0] == "/attachobjecttoped")
	{
		attachObjectToPlayer(playerid,cmd[1].tointeger(),0.5,0.3,0.5,0.0,90.0,0.0);
	}
	if(cmd[0] == "/attachobjecttovehicle")
	{
		attachObjectToVehicle(cmd[1].tointeger(),cmd[2].tointeger(),0.0,0.0,1.0,0.0,0.0,0.0);
	}
	if(cmd[0] == "/detachobject")
	{
		detachObject(cmd[1].tointeger());
	}
	if(cmd[0] == "/createobject")
	{
		local pos = getPlayerCoordinates(playerid);
		local object = createObject(cmd[1].tointeger(), pos[0]+2.0, pos[1]+2.0, pos[2], 0.0, 0.0, 0.0);
		sendPlayerMessage(playerid, "Object: "+object, 0xFFFFFFAA);
	}
	if(cmd[0] == "/moveobject")
	{
		moveObject(cmd[1].tointeger(), 1727.337769, 705.455688, 25.831680, 5000.0);
	}
	if(cmd[0] == "/moveobjectrot")
	{
		moveObject(cmd[1].tointeger(), 1727.337769, 705.455688, 25.831680, 5000.0, 0, 0, 0);
	}
	if(cmd[0] == "/rotateobject")
	{
		rotateObject(cmd[1].tointeger(), 0, 0, 0, 5000.0);
	}
	if(cmd[0] == "/toggleblip")
	{
		if(cmd[1].tointeger() == 1)
			switchBlipIcon(blip,true);
		else if(cmd[1].tointeger() == 0)
			switchBlipIcon(blip,false);
	}
	if(cmd[0] == "/driveautomatic")
	{
		letPlayerDriveAutomaticAtCoords(playerid,getPlayerVehicleId(playerid),1044.286743, 255.983231, 31.037477,15.0,0);
	}
	if(cmd[0] == "/driveactor")
	{
		driveActorToCoordinates(cmd[1].tointeger(),1044.286743, 255.983231, 31.037477);
	}
	if(cmd[0] == "/sayspeech")
	{
		sayPlayerSpeech(playerid,"JIMMY_PEGORINO", "GET_OUT_OF_HERE");
	}
	if(cmd[0] == "/actorspeech")
	{
		sayActorSpeech(cmd[1].tointeger(),"JIMMY_PEGORINO", "GET_OUT_OF_HERE");
	}
	if(cmd[0] == "/gotovehicle")
	{
		local pos = getVehicleCoordinates(cmd[1].tointeger());
		setPlayerCoordinates(playerid,pos[0],pos[1],pos[2]);
	}
	if(cmd[0] == "/alarm")
	{
		setVehicleAlarm(getPlayerVehicleId(playerid),cmd[1].tointeger());
	}
	if(cmd[0] == "/mobilephone")
	{
		if(cmd[1].tointeger() == 1)
			setPlayerUseMobilePhone(playerid,true);
		else if(cmd[1].tointeger() == 0)
			setPlayerUseMobilePhone(playerid,false);
	}
	if(cmd[0] == "/setvehpos")
	{
		setVehicleCoordinates(cmd[1].tointeger(),cmd[2].tofloat(),cmd[3].tofloat(),cmd[4].tofloat());
	}
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
	if(cmd[0] == "/lockvehicle")
	{
		setVehicleLocked(cmd[1].tointeger(),cmd[2].tointeger());
	}
	if(cmd[0] == "/setammo")
	{
		setPlayerAmmo(playerid,cmd[1].tointeger(),cmd[2].tointeger());
	}
	if(cmd[0] == "/12")
	{
		sendPlayerMessage(playerid, getPlayerIp(playerid));
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
		// Offset like as taxi-passenger in singleplayer
		// NOTE: You have to set the coordinates for the vehicle! (width, lenght, height)(FROM THE VEHICLE)
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.325,-1.0,0.0);
		setPlayerFollowVehicleMode(playerid,0);
	}
	if(cmd[0] == "/vehiclefollow2")
	{
		// Offset like as taxi-passenger in singleplayer
		// NOTE: You have to set the coordinates for the vehicle! (width, lenght, height)(FROM THE VEHICLE)
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.325,-1.75,0.0);
		setPlayerFollowVehicleMode(playerid,0);
	}
	if(cmd[0] == "/variation")
	{
		setVehicleVariation(getPlayerVehicleId(playerid),cmd[1].tointeger());
	}
	if(cmd[0] == "/vehicleego")
	{
		setPlayerFollowVehicleMode(playerid,1);
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.5,-4.0,0.0);
	}
	if(cmd[0] == "/attachcameratovehicle")
	{
		attachPlayerCameraToVehicle(playerid,cmd[1].tointeger(), cmd[2].tointeger(), 0.0, -5.0, 2.0);
	}
	if(cmd[0] == "/attachcameratoplayer")
	{
		attachPlayerCameraToPlayer(playerid,cmd[1].tointeger(), cmd[2].tointeger(), 0.0, -2.5, 1.5);
	}
	if(cmd[0] == "/respawn")
	{
		setPlayerSpawnLocation(playerid, /*-341.36, 1144.80, 14.79, 40.114815*/1649.508179, 395.261627, 38.151573, -2.931725);
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
	if(cmd[0] == "/sound")
	{
		triggerPlayerGameSound(playerid,"MOBILE_PHONE_SMS_RECIEVE");
	}
	if(cmd[0] == "/gps")
	{
		if(isPlayerInAnyVehicle(playerid))
		{
			// NOTE: The route(on radar) will have the same color as the blip!
			setBlipColor(blip,0xE59338FF,playerid);
			sendPlayerMessage(playerid,"ROUTE",White);
			setVehicleGpsState(getPlayerVehicleId(playerid),true);
			toggleBlipRoute(blip,true,playerid);
		}
	}
	if(cmd[0] == "/gps2")
	{
		if(isPlayerInAnyVehicle(playerid))
		{
			// NOTE: The route(on radar) will have the same color as the blip!
			sendPlayerMessage(playerid,"ROUTE 2!",White);
			setVehicleGpsState(getPlayerVehicleId(playerid),true);
			toggleBlipRoute(blipcolortest,true,playerid);
		}
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
	if(cmd[0] == "/forcetaxi1")
	{
		forcePlayerPlayAnimation(playerid,"amb@taxi_wait_i_f_b","look_around");
	}
	if(cmd[0] == "/forcetaxi2")
	{
		forcePlayerPlayAnimation(playerid,"amb@taxi_hail_f","hail_right");
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
	if(cmd[0] == "/createactors")
	{
		for(local i = 0; i < cmd[1].tointeger(); i++) {
			local pos = getPlayerCoordinates(playerid);
			local actor = createActor(8,pos[0],pos[1],pos[2],90.0);
			toggleActorHelmet(actor,false);
			toggleActorFrozen(actor,false);
			setActorName(actor,"JeNkStAX");
			toggleActorNametag(actor,true);
			forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff");
			
			//
			local actorcount = getActorCount();
			sendPlayerMessage(playerid,"Actorcount: "+actorcount,0xFFFFFFAA);
		}
	}
	if(cmd[0] == "/createactor")
	{
		local pos = getPlayerCoordinates(playerid);
		local actor = createActor(8,pos[0],pos[1],pos[2],90.0);
		toggleActorHelmet(actor,false);
		toggleActorFrozen(actor,false);
		setActorName(actor,"JeNkStAX");
		toggleActorNametag(actor,true);
		forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff");
		
		//
		local actorcount = getActorCount();
		sendPlayerMessage(playerid,"Actorcount: "+actorcount,0xFFFFFFAA);
	}
	if(cmd[0] == "/createactor2")
	{
		local pos = getPlayerCoordinates(playerid);
		local actor = createActor(21,pos[0],pos[1],pos[2],90.0);
		toggleActorHelmet(actor,false);
		toggleActorFrozen(actor,false);
		setActorName(actor,"JeNkStAX");
		toggleActorNametag(actor,true);
		forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff");
		
		//
		local actorcount = getActorCount();
		sendPlayerMessage(playerid,"Actorcount: "+actorcount,0xFFFFFFAA);
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
		else if(cmd.len() == 6)
		{
			local model = cmd[1].tointeger();
			local pos;
			if(isPlayerInAnyVehicle(playerid))
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid));
			else
				pos = getPlayerCoordinates(playerid);
			local heading = getPlayerHeading(playerid);
			local veh = createVehicle(model, pos[0], pos[1], pos[2], 0.0, 0.0, heading, cmd[2].tointeger(),cmd[3].tointeger(),cmd[4].tointeger(),cmd[5].tointeger());
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
		//hideCheckpointForAll(cp1);
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
	//log(format("VEHICLE DAMAGE: %d, %d, %d, %d, %d",vehicleid,oldhealth,oldpetrol,newhealth,newpetrol));
}
addEvent("vehicleDamage",onVehicleDamage);

function onPlayerShot(playerid,x,y,z,xl,yl,zl,shot)
{
	/*if(shot)
		log(format("WEP: %d SHOT(%f, %f, %f)|LOOKAT(%f, %f, %f)",playerid,x,y,z,xl,yl,zl));
	else
		log(format("WEP: %d AIM(%f, %f, %f)|LOOKAT(%f, %f, %f)",playerid,x,y,z,xl,yl,zl));*/
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
	return 1;
}
addEvent("vehicleRespawn",onVehicleRespawn);

function onHeadMovement(playerid, oldx, oldy, oldz, newx, newy, newz)
{
	//log(format("HEAD: %d OLD(%f, %f, %f) | NEW (%f, %f, %f)",playerid,oldx,oldy,oldz,newx,newy,newz));
}
addEvent("headMove",onHeadMovement);

function onVehicleRequest(playerid,vehicleid,seat)
{
	return 1;
}
addEvent("vehicleEntryRequest", onVehicleRequest);

function onSwitchEngine(playerid)
{
	if(!isPlayerOnFoot(playerid) && getPlayerSeatId(playerid) == 0)
	{
		local enginestate = getVehicleEngineState(getPlayerVehicleId(playerid));
		setVehicleEngineState(getPlayerVehicleId(playerid),!enginestate);
		sendPlayerMessage(playerid,"Switched Engine",0xFFFFFFAA);
		triggerClientEvent(playerid, "engineResponse");
	}
}		
addEvent("switchEngine", onSwitchEngine);
function onVehicleEntryComplete( playerid, vehicleid, seatid )
{
	log("| Player:"+playerid+"| vehhicleid:"+vehicleid+"| seatid:"+seatid+" |");
	
	if(seatid == 0){
		sendPlayerMessage(playerid,"Press Y to turn the engine on/off",0xFFFFFFAA);
	}
}
addEvent("vehicleEntryComplete", onVehicleEntryComplete);