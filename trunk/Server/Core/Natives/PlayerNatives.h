//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterPlayerNatives(CScriptingManager * pScriptingManager);

SQUIRREL_FUNCTION(player_isconnected);
SQUIRREL_FUNCTION(player_getname);
SQUIRREL_FUNCTION(player_setname);
SQUIRREL_FUNCTION(player_sethealth);
SQUIRREL_FUNCTION(player_gethealth);
SQUIRREL_FUNCTION(player_setarmour);
SQUIRREL_FUNCTION(player_getarmour);
SQUIRREL_FUNCTION(player_setcoordinates);
SQUIRREL_FUNCTION(player_getcoordinates);
SQUIRREL_FUNCTION(player_settime);
SQUIRREL_FUNCTION(player_setweather);
SQUIRREL_FUNCTION(player_setgravity);
SQUIRREL_FUNCTION(player_sendmessage);
SQUIRREL_FUNCTION(player_sendmessagetoall);
SQUIRREL_FUNCTION(player_isinanyvehicle);
SQUIRREL_FUNCTION(player_isinvehicle);
SQUIRREL_FUNCTION(player_getvehicleid);
SQUIRREL_FUNCTION(player_getseatid);
SQUIRREL_FUNCTION(player_isonfoot);
SQUIRREL_FUNCTION(player_togglepayandspray);
SQUIRREL_FUNCTION(player_toggleautoaim);
//SQUIRREL_FUNCTION(player_setplayerdrunk);
SQUIRREL_FUNCTION(player_giveweapon);
SQUIRREL_FUNCTION(player_removeweapons);
SQUIRREL_FUNCTION(player_setspawnlocation);
SQUIRREL_FUNCTION(player_setmodel);
SQUIRREL_FUNCTION(player_getmodel);
SQUIRREL_FUNCTION(player_togglecontrols);
SQUIRREL_FUNCTION(player_isspawned);
SQUIRREL_FUNCTION(player_setheading);
SQUIRREL_FUNCTION(player_getheading);
SQUIRREL_FUNCTION(player_togglephysics);
SQUIRREL_FUNCTION(player_kick);
SQUIRREL_FUNCTION(player_ban);
SQUIRREL_FUNCTION(player_getip);
SQUIRREL_FUNCTION(player_givemoney);
SQUIRREL_FUNCTION(player_setmoney);
SQUIRREL_FUNCTION(player_resetmoney);
SQUIRREL_FUNCTION(player_getmoney);
SQUIRREL_FUNCTION(player_displaytext);
SQUIRREL_FUNCTION(player_displaytextforall);
SQUIRREL_FUNCTION(player_displayinfotext);
SQUIRREL_FUNCTION(player_displayinfotextforall);
SQUIRREL_FUNCTION(player_togglefrozen);
SQUIRREL_FUNCTION(player_getstate);
SQUIRREL_FUNCTION(player_setvelocity);
SQUIRREL_FUNCTION(player_getvelocity);
SQUIRREL_FUNCTION(player_setwantedlevel);
SQUIRREL_FUNCTION(player_warpintovehicle);
SQUIRREL_FUNCTION(player_removefromvehicle);
SQUIRREL_FUNCTION(player_getweapon);
SQUIRREL_FUNCTION(player_getammo);
SQUIRREL_FUNCTION(player_getserial);
SQUIRREL_FUNCTION(player_setcamerabehind);
SQUIRREL_FUNCTION(player_setducking);
SQUIRREL_FUNCTION(player_togglehud);
SQUIRREL_FUNCTION(player_toggleradar);
SQUIRREL_FUNCTION(player_togglenames);
SQUIRREL_FUNCTION(player_toggleareanames);
SQUIRREL_FUNCTION(player_getemptypadstate);
SQUIRREL_FUNCTION(player_getpreviouspadstate);
SQUIRREL_FUNCTION(player_getpadstate);
SQUIRREL_FUNCTION(player_trigger_event);
SQUIRREL_FUNCTION(player_getcolor);
SQUIRREL_FUNCTION(player_setcolor);
SQUIRREL_FUNCTION(player_getping);
SQUIRREL_FUNCTION(player_setclothes);
SQUIRREL_FUNCTION(player_getclothes);
SQUIRREL_FUNCTION(player_resetclothes);
SQUIRREL_FUNCTION(player_respawn);
SQUIRREL_FUNCTION(player_givehelmet);
SQUIRREL_FUNCTION(player_removehelmet);
