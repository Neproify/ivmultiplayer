//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ClientNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterClientNatives(CScriptingManager * pScriptingManager);
void RegisterGUINatives(CScriptingManager * pScriptingManager);
void RegisterScriptNatives(CScriptingManager * pScriptingManager);

SQUIRREL_FUNCTION(addChatMessage);
SQUIRREL_FUNCTION(toggleChatWindow);
SQUIRREL_FUNCTION(isChatWindowVisible);
SQUIRREL_FUNCTION(guiShowMessageBox);
SQUIRREL_FUNCTION(guiToggleCursor);
SQUIRREL_FUNCTION(guiGetScreenSize);
SQUIRREL_FUNCTION(guiDrawRectangle);
SQUIRREL_FUNCTION(triggerServerEvent);
SQUIRREL_FUNCTION(guiIsCursorVisible);
SQUIRREL_FUNCTION(getScripts);
SQUIRREL_FUNCTION(getScriptName);
SQUIRREL_FUNCTION(getWeather);
SQUIRREL_FUNCTION(setWeather);
SQUIRREL_FUNCTION(getGameScrollBarText);
SQUIRREL_FUNCTION(setGameScrollBarText);
SQUIRREL_FUNCTION(resetGameScrollBarText);
SQUIRREL_FUNCTION(getGameScrollBarColor);
SQUIRREL_FUNCTION(setGameScrollBarColor);
SQUIRREL_FUNCTION(getMoney);
SQUIRREL_FUNCTION(getCurrentAreaName);
SQUIRREL_FUNCTION(getCurrentStreetName);
SQUIRREL_FUNCTION(getVehicleName);
SQUIRREL_FUNCTION(getFPS);
SQUIRREL_FUNCTION(getScreenPositionFromWorldPosition);
SQUIRREL_FUNCTION(isGameFocused);
SQUIRREL_FUNCTION(setRadarZoom);
SQUIRREL_FUNCTION(getActorCoordinates);
SQUIRREL_FUNCTION(drawLightWithRange);
SQUIRREL_FUNCTION(tuneRadio);